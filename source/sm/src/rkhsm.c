/*
 *  --------------------------------------------------------------------------
 *
 *                                Framework RKH
 *                                -------------
 *
 *            State-machine framework for reactive embedded systems
 *
 *                      Copyright (C) 2010 Leandro Francucci.
 *          All rights reserved. Protected by international copyright laws.
 *
 *
 *  RKH is free software: you can redistribute it and/or modify it under the
 *  terms of the GNU General Public License as published by the Free Software
 *  Foundation, either version 3 of the License, or (at your option) any
 *  later version.
 *
 *  RKH is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with RKH, see copying.txt file.
 *
 *  Contact information:
 *  RKH site: http://vortexmakes.com/que-es/
 *  RKH GitHub: https://github.com/vortexmakes/RKH
 *  RKH Sourceforge: https://sourceforge.net/projects/rkh-reactivesys/
 *  e-mail: lf@vortexmakes.com
 *  ---------------------------------------------------------------------------
 */

/**
 *  \file       rkhsm.c
 *  \brief      Platform - independent interface for supporting state-machines.
 *
 *  \ingroup    sm
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2017.04.14  LeFr  v2.4.05  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkhdef.h"
#include "rkhevt.h"
#include "rkhitl.h"
#include "rkhsm.h"
#include "rkhassert.h"
#include "rkhfwk_hook.h"
#include "rkhtrc_record.h"
#include "rkhtrc_filter.h"
#include "rkhfwk_dynevt.h"

RKH_MODULE_NAME(rkhsm)

/* ----------------------------- Local macros ------------------------------ */
#define IS_INTERNAL_TRANSITION(s)       ((s) == CST(0))
#define IS_EMPTY_HISTORY(s)             (*(CH(s))->target == (RKHROM void *)0)
#define IS_FOUND_TRN(t)                 ((t)->event != RKH_ANY)
#define IS_NOT_FOUND_TRN(t)             ((t)->event == RKH_ANY)
#define IS_VALID_GUARD(t)               ((t)->guard != CG(0))
#define IS_PSEUDO(s)                    ((CB((s))->type & RKH_REGULAR) == 0)
#define IS_COMPOSITE(s)                 (CB((s))->type == RKH_COMPOSITE)
#define IS_SUBMACHINE(s)                (CB((s))->type == RKH_SUBMACHINE)
#define IS_REF_SUBMACHINE(s)            (CB((s))->type == RKH_REF_SUBMACHINE)
#define IS_SIMPLE(s)                    (CB((s))->type == RKH_BASIC)
#define IS_FINAL(s)                     (CB((s))->type == RKH_FINAL)

#if (RKH_CFG_SMA_ORTHREG_EN == RKH_ENABLED || \
     RKH_CFGPORT_REENTRANT_EN == RKH_ENABLED)
    /* allocate the automatic variables of rkh_sm_dispatch() */
    /* function on the stack. */
    /* Therefore, the code is reentrant. */
    #define RKH_RAM
#else
    #define RKH_RAM     static
#endif

#define FIND_TRN(me_, evt_, trn_, trnTbl_, signal_) \
    for ((trn_) = (trnTbl_); ((trn_)->event != RKH_ANY); ++(trn_)) \
    { \
        if (((trn_)->event == signal_)) \
        { \
            if (IS_VALID_GUARD(trn_)) /* is enabled transition? */ \
            { \
                if (RKH_EXEC_GUARD((trn_), (me_), (evt_)) == RKH_GTRUE) \
                { \
                    break; /* Enabled transition */\
                } \
                else \
                { \
                    /* Disabled transition. Transitions that have a guard */ \
                    /* which evaluates to false are disabled */ \
                    RKH_TR_SM_GRD_FALSE(me_); \
                } \
            } \
            else \
            { \
                break; /* Enabled transition. A transition that does not */ \
                       /* have an associated guard is treated as if it */ \
                       /* has a guard that is always true */ \
            } \
        } \
    }

#if defined(RKH_SHALLOW_ENABLED)
    #if RKH_CFG_SMA_SUBMACHINE_EN == RKH_ENABLED
    #define RKH_UPDATE_SHALLOW_HIST(s, h) \
    if (CST((s))->parent != CST(0) && \
        IS_COMPOSITE((s)->parent) && \
        ((h) = CCMP(CST((s))->parent)->history) != CH(0) && \
        CB((h))->type == RKH_SHISTORY) \
        *(h)->target = (s)
    #else
    #define RKH_UPDATE_SHALLOW_HIST(s, h) \
    if (CST((s))->parent != CST(0) && \
        ((h) = CCMP(CST((s))->parent)->history) != CH(0) && \
        CB((h))->type == RKH_SHISTORY) \
        *(h)->target = (s)
    #endif
#else
    #define RKH_UPDATE_SHALLOW_HIST(s, h)         ((void)0)
#endif

#if RKH_CFG_SMA_PPRO_EN == RKH_ENABLED
    #define RKH_PROCESS_INPUT(s, h, pe) \
    (RKH_SIG_T)(((s)->prepro != CPP(0)) ? \
                RKH_EXEC_PREPRO((s),(h),(pe)) : (pe)->e)
#else
    #define RKH_PROCESS_INPUT(s, h, pe) \
    (RKH_SIG_T)((pe)->e)
#endif

#define FIND_BRANCH(btbl, t, sma, pe) \
    for ((t) = (btbl); (t)->event != RKH_ANY; ++(t)) \
        if (IS_VALID_GUARD((t)) && \
            RKH_EXEC_GUARD((t), (sma), (pe)) == RKH_GTRUE) \
        { \
            break; \
        }

#define RKH_EXEC_TRANSITION(sma, e) \
    for (pal = al; nal != 0; ++pal, --nal) \
    { \
        RKH_EXEC_EFF(*pal, (sma), (e)); \
    } \
    pal = al

#if RKH_CFG_TRC_EN == RKH_ENABLED
    #define RKH_CLR_STEP()          (step = 0)
    #define RKH_INC_STEP()          ++ step
    #define RKH_GET_STEP()          step
#else
    #define RKH_CLR_STEP()          ((void)0)
    #define RKH_INC_STEP()          ((void)0)
    #define RKH_GET_STEP()          ((void)0)
#endif

#if RKH_CFG_SMA_GET_INFO_EN == RKH_ENABLED
    #define INFO_RCV_EVENTS(p)      ++ (p)->hinfo.rcvevt
    #define INFO_EXEC_TRS(p)        ++ (p)->hinfo.exectr
#else
    #define INFO_RCV_EVENTS(p)      ((void)0)
    #define INFO_EXEC_TRS(p)        ((void)0)
#endif

#if defined(RKH_SUBMACHINE_ENABLED)
    #define UPDATE_PARENT(s) \
    (s) = (s)->parent; \
    if ((s) != CST(0) && IS_REF_SUBMACHINE((s))) \
        (s) = *CRSM((s))->dyp
    #define UPDATE_IN_PARENT(s) \
    (s) = (s)->parent; \
    if ((s) != CST(0) && IS_REF_SUBMACHINE((s))) \
        (s) = *CRSM((s))->dyp
#else
    #define UPDATE_PARENT(s) \
    (s) = (s)->parent
    #define UPDATE_IN_PARENT(s) \
    (s) = (s)->parent
#endif

#if RKH_CFG_SMA_HCAL_EN == RKH_ENABLED
    #define RKH_EXEC_EXIT_ACTION(src, tgt, sma, nex) \
    for (ix_n = 0, ix_x = islca = 0, stx = src; \
         stx != CST(0); ++ix_x) \
    { \
        for (ix_n = 0, snl = sentry, stn = tgt; \
             stn != CST(0); ++snl, ++ix_n) \
        { \
            if (stx == stn) \
            { \
                islca = 1;      /* found LCA */ \
                break; \
            } \
            else if (ix_n < RKH_CFG_SMA_MAX_HCAL_DEPTH) \
            { \
                *snl = stn;     /* add state in entry state list */ \
            } \
            else \
            { \
                RKH_TR_SM_EX_HLEVEL(sma); \
                RKH_ERROR(); \
                return RKH_EX_HLEVEL; \
            } \
            UPDATE_PARENT(stn); \
        } \
        if (islca == 0 || ix_x == 0) \
        { \
            /* perform the exit actions of the exited states */ \
            RKH_EXEC_EXIT(stx, CM(sma)); \
            /* update histories of exited states */ \
            RKH_UPDATE_SHALLOW_HIST(stx, h); \
            RKH_TR_SM_EXSTATE(sma,      /* this state machine object */ \
                              stx);     /* exited state */ \
            if (islca == 1) \
            { \
                *snl = stn; \
                ++ix_n; \
                ++ix_x; \
                break; \
            } \
        } \
        else \
        { \
            break; \
        } \
        UPDATE_PARENT(stx); \
    } \
    /* save the # of entered states */ \
    nex = ix_n
#else
    #define RKH_EXEC_EXIT_ACTION(src, tgt, sma, nex) \
    stx = src; \
    nex = ix_n = ix_x = (rui8_t)(stx != tgt)
#endif

#if RKH_CFG_SMA_HCAL_EN == RKH_ENABLED
    #define RKH_EXEC_ENTRY_ACTION(nen, sma, stn, snl, ix_n) \
    for (ix_n = nen, snl = &sentry[ix_n]; ix_n != 0; --ix_n) \
    { \
        --snl; \
        RKH_EXEC_ENTRY(*snl, CM(sma)); \
        isCompletionEvent  = isCompletionTrn(*snl); \
        RKH_TR_SM_ENSTATE(sma, *snl); \
    } \
    stn = *snl;
#else
    #define RKH_EXEC_ENTRY_ACTION(nen, sma, stn, snl, ix_n) \
    nen = 0; \
    if (ix_n == ix_x && ix_x == 0) \
    { \
    } \
    else \
    { \
        stn = ets; \
    }
#endif

/* ------------------------------- Constants ------------------------------- */
RKH_ROM_STATIC_EVENT(evCompletion, RKH_COMPLETION_EVENT);
RKH_ROM_STATIC_EVENT(evCreation, RKH_SM_CREATION_EVENT);

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static rbool_t
findCompletionTrn(RKHROM RKH_TR_T *trnTable)
{
    RKHROM RKH_TR_T *trn;
    rbool_t res;

    for (res = RKH_FALSE, trn = trnTable; trn->event != RKH_ANY; ++trn)
    {
        if (trn->event == RKH_COMPLETION_EVENT)
        {
            res = RKH_TRUE;
            break;
        }
    }
    return res;
}

#if RKH_CFG_SMA_HCAL_EN == RKH_ENABLED
static rbool_t
isCompletionTrn(RKHROM RKH_ST_T *state)
{
    if ((IS_SIMPLE(state) && findCompletionTrn(CBSC(state)->trtbl)) ||
        (IS_FINAL(state) && (CST(state)->parent != CST(0))))
        return RKH_TRUE;
    else
        return RKH_FALSE;
}
#endif

static rbool_t
rkh_add_tr_action(RKH_TRN_ACT_T * *list, RKH_TRN_ACT_T act, rui8_t *num)
{
    if (*num >= RKH_CFG_SMA_MAX_TRC_SEGS)
    {
        return 1;
    }

    if (act != CTA(0))
    {
        **list = act;                       /* store a new transition action */
        ++(*list);                     /* increment the pointer to next slot */
        ++(*num);            /* increment the counter of actions in the list */
    }
    return 0;
}

#if defined(RKH_DEEP_ENABLED)
static void
rkh_update_deep_hist(RKHROM RKH_ST_T *from)
{
    RKHROM RKH_ST_T *s;
    RKHROM RKH_SHIST_T *h;

    for (s = from->parent;
         s != (RKHROM RKH_ST_T *)0; s = s->parent)
    {
#if defined(RKH_SUBMACHINE_ENABLED)
        if (IS_REF_SUBMACHINE(s))
        {
            s = *CRSM(s)->dyp;
            continue;
        }
#endif
        if (((h = CCMP(s)->history) != (RKHROM RKH_SHIST_T *)0) &&
            (CB(h)->type == RKH_DHISTORY))
        {
            *h->target = from;
        }
    }
}
#else
    #define rkh_update_deep_hist(f)       ((void)0)
#endif

#if RKH_CFG_SMA_HCAL_EN == RKH_ENABLED
static rui8_t
addTargetSt(RKHROM RKH_ST_T *target, RKHROM RKH_ST_T **stList,
            RKHROM RKH_ST_T *cmpSt)
{
    RKHROM RKH_ST_T **missSt, *st;
    rui8_t nEnSt = 0;

    if (IS_COMPOSITE(target) || IS_SIMPLE(target) || IS_FINAL(target))
    {
        missSt = stList;
        st = target;
        nEnSt = 0;
        while (st != cmpSt)
        {
            *missSt++ = st;
            st = st->parent;
            ++nEnSt;
        }
    }
    return nEnSt;
}
#endif

/* ---------------------------- Global functions --------------------------- */
void
rkh_sm_init(RKH_SM_T *me)
{
    rkh_sm_dispatch((RKH_SM_T *)me, (RKH_EVT_T *)&evCreation);
}

#if defined(RKH_HISTORY_ENABLED)
void
rkh_sm_clear_history(RKHROM RKH_SHIST_T *h)
{
    *(h->target) = (RKHROM RKH_ST_T *)0;
}
#endif

ruint
rkh_sm_dispatch(RKH_SM_T *me, RKH_EVT_T *pe)
{
    RKHROM RKH_ST_T *cs, *ts;
    RKHROM void *ets;
    RKHROM RKH_TR_T *tr;
    rbool_t isIntTrn, isCompletionEvent, isMicroStep, isCreationEvent,
        isPropagatedEvent;
    RKH_SIG_T in;
#if RKH_CFG_TRC_EN == RKH_ENABLED
    rui8_t step;
#endif
#if defined(RKH_SHALLOW_ENABLED)
    RKHROM RKH_SHIST_T *h;
#endif
#if defined(RKH_SUBMACHINE_ENABLED)
    RKHROM RKH_SSBM_T *dp;
#endif
    /* to deal with Statechart's transition sequence */
    RKH_RAM RKHROM RKH_ST_T *stn, *stx;
#if RKH_CFG_SMA_HCAL_EN == RKH_ENABLED
    RKH_RAM RKHROM RKH_ST_T * *snl;
    RKH_RAM rui8_t islca;
#endif
    RKH_RAM rui8_t ix_n, ix_x, nn, nEntrySt;
#if RKH_CFG_SMA_HCAL_EN == RKH_ENABLED
    /* set of entered states */
    RKH_RAM RKHROM RKH_ST_T *sentry[RKH_CFG_SMA_MAX_HCAL_DEPTH];
#endif
    /* set of executed transition actions */
    RKH_RAM RKH_TRN_ACT_T al[RKH_CFG_SMA_MAX_TRC_SEGS];

    RKH_RAM RKH_TRN_ACT_T *pal;          /* pointer to transition action set */
    RKH_RAM rui8_t nal;                  /* # of executed transition actions */
    RKH_RAM RKH_TRN_ACT_T trnAct;
    RKH_SR_ALLOC();

    RKH_ASSERT(me && pe);
    isCompletionEvent = isIntTrn = isMicroStep = isPropagatedEvent = RKH_FALSE;
    isCreationEvent = (pe->e == RKH_SM_CREATION_EVENT);

    if (isCreationEvent == RKH_FALSE)
    {
        INFO_RCV_EVENTS(me);
        RKH_HOOK_DISPATCH(me, pe);
    }
    else
    {
        RKH_TR_SM_INIT(me, RKH_SMA_ACCESS_CONST(me, istate));
    }

    do
    {
    /* ---- Stage 1 -------------------------------------------------------- */
    if (isCreationEvent == RKH_FALSE)
    {
        cs = ((RKH_SM_T *)me)->state;                   /* get current state */
    }
    else
    {
        cs = CST(RKH_SMA_ACCESS_CONST(me, istate));  /* get dft vertex of SM */
                                                            /* (root region) */
    }
    /* ---- Stage 2 -------------------------------------------------------- */
    /* Determine the (compound) transition (CT) that will fire in response */
    /* to the event: traverse the states in the active configuration from */
    /* lowest states in the hierarchy upwards. A CT is enabled if its */
    /* trigger is the dispatched event, and the guard evaluates to true. */
    /* Once an enabled transition is found with a given source state */
    /* stop traversing the states that are higher than this state in the */
    /* hierarchy. */
    if (isCompletionEvent)
    {
        pe = (RKH_EVT_T *)&evCompletion;
        isCompletionEvent = RKH_FALSE;
    }
    else if (isPropagatedEvent)
    {
        pe = me->propagatedEvent;
        me->propagatedEvent = RKH_EVT_CAST(0);
        isPropagatedEvent = RKH_FALSE;
    }
    else
    {
    }

    if (isCreationEvent == RKH_FALSE)
    {
#if RKH_CFG_SMA_HCAL_EN == RKH_ENABLED
        for (stn = cs, tr = CT(0); stn != CST(0); UPDATE_IN_PARENT(stn))
        {
            in = RKH_PROCESS_INPUT(stn, me, pe);
            FIND_TRN(me, pe, tr, CBSC(stn)->trtbl, in);
            if (IS_FOUND_TRN(tr))
            {
                break;
            }
        }
#else
        stn = cs;
        in = RKH_PROCESS_INPUT(stn, me, pe);
        FIND_TRN(me, pe, tr, CBSC(stn)->trtbl, in);
#endif

        RKH_TR_SM_DCH(me,                       /* this state machine object */
                      pe,                                           /* event */
                      cs);                                  /* current state */
        if (IS_NOT_FOUND_TRN(tr))                       /* transition taken? */
        {
            RKH_TR_SM_EVT_NFOUND(me,            /* this state machine object */
                                 pe);                               /* event */
            return RKH_EVT_NFOUND;
        }

        ets = tr->target;   /* temporarily save the target of the transition */
        trnAct = tr->action;
    }
    else
    {
              /* After invoking the SM initialization method is dispatched a */
              /* special event, evCreation, initializing the behavior of the */
            /* reactive object and causing its statechart to enter an active */
        /* configuration according to the default transitions taken from the */
                                                                    /* root. */
        stn = ets = cs;
        trnAct = RKH_SMA_ACCESS_CONST(me, iaction);
        nn = 0;
        ix_x = 0;
    }

    RKH_CLR_STEP();
    nEntrySt = 0;
    ts = CST(ets);
    isIntTrn = IS_INTERNAL_TRANSITION(ts);     /* is an internal transition? */
    RKH_TR_SM_TRN(me,                           /* this state machine object */
                  stn,                            /* transition source state */
                  ts);                            /* transition target state */
    do
    {
        nal = 0;                        /* initialize transition action list */
        pal = al;
        if (rkh_add_tr_action(&pal, trnAct, &nal))
        {
            RKH_TR_SM_EX_TSEG(me);
            RKH_ERROR();
            return RKH_EX_TSEG;
        }
        RKH_INC_STEP();        /* increment the number of transition segment */
        if (isIntTrn == RKH_FALSE)
        {
            RKH_TR_SM_TS_STATE(me,              /* this state machine object */
                               ets);       /* target state of the transition */
                                                                   /*segment */
        }
        if (isIntTrn == RKH_FALSE)
        {
            /* ---- Stage 3 ------------------------------------------------ */
            /* ... traverses the taken transition until */
            /* the segment target state (ets) == simple or composite state */
#if RKH_PSEUDOSTATE == RKH_ENABLED
            while (IS_PSEUDO(ets) || IS_SUBMACHINE(ets))
            {
                switch (CB(ets)->type)
                {
#if defined(RKH_CHOICE_ENABLED)
                    case RKH_CHOICE:
                        /* perform the actions on the transition */
                        /* sequentially according to the order in which they */
                        /* are written on the transition, from the action */
                        /* closest to source state to the action closest to */
                        /* target state. */
                        RKH_EXEC_TRANSITION(me, pe);
#endif
#if defined(RKH_CHOICE_OR_CONDITIONAL_ENABLED)
                    case RKH_CONDITIONAL:
                        /* evaluates the guards of its outgoing transitions */
                        FIND_BRANCH(CCD(ets)->trtbl, tr, me, pe);

                        if (IS_NOT_FOUND_TRN(tr))
                        {
                            RKH_TR_SM_CND_NFOUND(me);
                            return RKH_CND_NFOUND;
                        }

                        if (rkh_add_tr_action(&pal, tr->action, &nal))
                        {
                            RKH_TR_SM_EX_TSEG(me);
                            RKH_ERROR();
                            return RKH_EX_TSEG;
                        }
                        /* another transition segment */
                        RKH_INC_STEP();
                        ets = tr->target;
                        if (isMicroStep)
                        {
                            nn = addTargetSt(CST(ets), sentry, stn);
                        }
                        break;
#endif
#if defined(RKH_HISTORY_ENABLED)
                    case RKH_SHISTORY:
                    case RKH_DHISTORY:
                        /* found a shallow or deep history pseudostate */
                        /* in the compound transition */
                        RKH_REQUIRE((CH(ets)->parent != (RKHROM RKH_ST_T *)0)
                                    && (CCMP(CH(ets)->parent)->history !=
                                    (RKHROM RKH_SHIST_T *)0));
                        stn = CH(ets)->parent;
                        if (IS_EMPTY_HISTORY(ets))
                        {
                            if (CH(ets)->trn.target)
                            {
                                if (IS_VALID_GUARD(&(CH(ets)->trn)) &&
                                    (RKH_EXEC_GUARD(&(CH(ets)->trn), me, pe)
                                     == RKH_GFALSE))
                                {
                                    RKH_TR_SM_GRD_FALSE(me);
                                    return RKH_GRD_FALSE;
                                }
                                /* Add action of transition segment into the */
                                /* list */
                                if (rkh_add_tr_action(&pal,
                                                      CH(ets)->trn.action,
                                                      &nal))
                                {
                                    RKH_TR_SM_EX_TSEG(me);
                                    RKH_ERROR();
                                    return RKH_EX_TSEG;
                                }
                                ets = CH(ets)->trn.target;
                            }
                            else
                            {
                                ets = CH(ets)->parent;
                            }
                        }
                        else
                        {
                            ets = *(CH(ets))->target;
                        }
                        if (isMicroStep)
                        {
                            nn = addTargetSt(CST(ets), sentry, stn);
                        }
                        break;
#endif
#if defined(RKH_SUBMACHINE_ENABLED)
                    case RKH_SUBMACHINE:
                        /* found a submachine state */
                        *CSBM(ets)->sbm->dyp = ets;
                        if (rkh_add_tr_action(&pal, CSBM(ets)->sbm->iaction,
                                              &nal))
                        {
                            RKH_TR_SM_EX_TSEG(me);
                            RKH_ERROR();
                            return RKH_EX_TSEG;
                        }
                        ets = CSBM(ets)->sbm->defchild;
                        break;
                    case RKH_ENPOINT:
                        /* found an entry point pseudostate */
                        /* in the compound transition */
                        *CSBM(CENP(ets)->parent)->sbm->dyp = CENP(ets)->parent;
                        if (rkh_add_tr_action(&pal, CENP(ets)->enpcn->action,
                                              &nal))
                        {
                            RKH_TR_SM_EX_TSEG(me);
                            RKH_ERROR();
                            return RKH_EX_TSEG;
                        }
                        ets = CENP(ets)->enpcn->target;
                        break;
                    case RKH_EXPOINT:
                        /* found an exit point pseudostate */
                        /* in the compound transition */
                        dp = CSBM(*CEXP(ets)->parent->dyp);
                        ets = CST(&(dp->exptbl[CEXP(ets)->ix]));
                        if (rkh_add_tr_action(&pal, CEXPCN(ets)->action, &nal))
                        {
                            RKH_TR_SM_EX_TSEG(me);
                            RKH_ERROR();
                            return RKH_EX_TSEG;
                        }
                        ets = CEXPCN(ets)->target;
                        break;
#endif
                    default:
                        /* fatal error: unknown state... */
                        RKH_TR_SM_UNKN_STATE(me);
                        RKH_ERROR();
                        return RKH_UNKN_STATE;
                }
                RKH_TR_SM_TS_STATE(me, ets);
            }
#endif
            ts = CST(ets);             /* finally, set the main target state */
        }

        if (isIntTrn == RKH_FALSE)
        {
            if (isMicroStep == RKH_FALSE)
            {
                if (isCreationEvent == RKH_FALSE)
                {
                    /* ---- Stage 4 ---------------------------------------- */
                    /* first of all, find the LCA then */
                    /* perform the exit actions of the exited states */
                    /* according to the order states are exited, from low */
                    /* state to high state, update histories of exited */
                    /* states, and, generate the set of entered states */
                    RKH_EXEC_EXIT_ACTION(cs, ts, me, nn);
                }
                else
                {
                    /* Upon state machine creation adds states to entry from */
                                            /* dft transition target to root */
#if RKH_CFG_SMA_HCAL_EN == RKH_ENABLED
                    nn += addTargetSt(CST(ts), sentry, RKH_ROOT);
#endif
                }
            }
            else
            {
#if RKH_CFG_SMA_HCAL_EN == RKH_ENABLED
                nn = addTargetSt(CST(ets), sentry, stn);
#endif
            }
        }
        /* ---- Stage 5 ---------------------------------------------------- */
        /* perform the actions on the transition sequentially */
        /* according to the order in which they are written on */
        /* the transition, from the action closest to source */
        /* state to the action closest to target state. */
        RKH_EXEC_TRANSITION(me, pe);

        if (isIntTrn == RKH_FALSE)
        {
            /* ---- Stage 6 ------------------------------------------------ */
            /* perform the entry actions of the entered states */
            /* according to the order states are entered, */
            /* from high state to low state. */
            /* Also, update 'stn' with the target state */
            RKH_EXEC_ENTRY_ACTION(nn, me, stn, snl, ix_n);
            nEntrySt += nn;

            if (IS_COMPOSITE(ts))
            {
#if RKH_CFG_SMA_HCAL_EN == RKH_ENABLED
                isMicroStep = RKH_TRUE;
                trnAct = CCMP(ts)->initialAction;
                ets = CCMP(ts)->defchild;
                stn = ts;                           /* tracking parent state */
#endif
            }
            else
            {
                isMicroStep = RKH_FALSE;   /* the main target state is found */
            }
        }
    } while (isMicroStep);

    if (isIntTrn == RKH_FALSE)
    {
        RKH_TR_SM_NENEX(me,                     /* this state machine object */
                        nEntrySt,                        /* # entered states */
                        ix_x);                            /* # exited states */
        /* ---- Stage 7 ---------------------------------------------------- */
        /* update deep history */
        rkh_update_deep_hist(CST(stn));
        /* ---- Stage 8 ---------------------------------------------------- */
        ((RKH_SM_T *)me)->state = CST(stn);      /* update the current state */
        RKH_TR_SM_STATE(me,                     /* this state machine object */
                        stn);                               /* current state */
    }

    RKH_TR_SM_EVT_PROC(me);
    isCreationEvent = RKH_FALSE;
    isPropagatedEvent =
        (me->propagatedEvent != (RKH_EVT_T*)0) ? RKH_TRUE : RKH_FALSE;
    } while (isCompletionEvent || isPropagatedEvent);

    INFO_EXEC_TRS(me);
    return RKH_EVT_PROC;
}

#if RKH_CFG_SMA_RT_CTOR_EN == RKH_ENABLED
void
rkh_sm_ctor(RKH_SM_T *me)
{
    (void)me;
}
#endif

#if (RKH_CFG_SMA_GRD_ARG_EVT_EN == RKH_ENABLED && \
     RKH_CFG_SMA_GRD_ARG_SMA_EN == RKH_ENABLED)
rbool_t
rkh_sm_else(const struct RKH_SM_T *sma, RKH_EVT_T *pe)
{
    (void)sma;
    (void)pe;
    return RKH_GTRUE;
}
#elif (RKH_CFG_SMA_GRD_ARG_EVT_EN == RKH_ENABLED && \
       RKH_CFG_SMA_GRD_ARG_SMA_EN == RKH_DISABLED)
rbool_t
rkh_sm_else(RKH_EVT_T *pe)
{
    (void)pe;
    return RKH_GTRUE;
}
#elif (RKH_CFG_SMA_GRD_ARG_EVT_EN == RKH_DISABLED && \
       RKH_CFG_SMA_GRD_ARG_SMA_EN == RKH_ENABLED)
rbool_t
rkh_sm_else(const struct RKH_SM_T *sma)
{
    (void)sma;
    return RKH_GTRUE;
}
#else
rbool_t
rkh_sm_else(void)
{
    return RKH_GTRUE;
}
#endif

void
rkh_sm_propagate(RKH_SM_T* me, RKH_EVT_T* e)
{
    RKH_REQUIRE((me != (RKH_SM_T*)0) && (e != (RKH_EVT_T*)0));
    me->propagatedEvent = e;
}

/* ------------------------------ End of file ------------------------------ */
