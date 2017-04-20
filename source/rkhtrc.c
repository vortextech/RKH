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
 *  RKH web site:   http://sourceforge.net/projects/rkh-reactivesys/
 *  e-mail:         francuccilea@gmail.com
 *  ---------------------------------------------------------------------------
 */

/**
 *  \file       rkhtrc.c
 *  \ingroup    trc
 *
 *  \brief      Implements the RKH trace facility.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2015.10.24  LeFr  v2.4.05  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  francuccilea@gmail.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */

#include "rkhassert.h"
#include "rkhtrc.h"
#include "rkh.h"
#include "rkhrdy.h"

#if RKH_CFG_TRC_EN == RKH_ENABLED

/* ----------------------------- Local macros ------------------------------ */

/*
 * This macro is needed only if the module requires to check expressions 
 * that ought to be true as long as the program  is running.
 */
RKH_MODULE_NAME(rkhtrc)

/**
 *  \brief
 *  Inserts the previously calculated checksum as:
 *  checksum = 0 - sum mod-256 -> ~(sum mod-256) + 1.
 */
#if RKH_CFG_TRC_CHK_EN == RKH_ENABLED
    #define RKH_TRC_CHK() \
        chk = (rui8_t)(~chk + 1); \
        rkh_trc_u8(chk)
#else
    #define RKH_TRC_CHK()
#endif

/**
 *  \brief
 *  Inserts directly into the trace stream the flag byte in a raw (without
 *  escaped sequence) manner.
 */

#define RKH_TRC_FLG()       RKH_TRC_U8_RAW(RKH_FLG)

/**
 *  \brief
 *  Insert the sequence number byte.
 */
#if RKH_CFG_TRC_NSEQ_EN == RKH_ENABLED
    #define RKH_TRC_NSEQ() \
        RKH_TRC_UI8(nseq); \
        ++nseq
#else
    #define RKH_TRC_NSEQ()
#endif

#if RKH_CFG_TRC_TSTAMP_EN == RKH_ENABLED
    #if RKH_CFGPORT_TRC_SIZEOF_TSTAMP == 8
        #define RKH_TRC_TSTAMP() \
            RKH_TRC_UI8(rkh_trc_getts())
    #elif RKH_CFGPORT_TRC_SIZEOF_TSTAMP == 16
        #define RKH_TRC_TSTAMP() \
            RKH_TRC_UI16(rkh_trc_getts())
    #elif RKH_CFGPORT_TRC_SIZEOF_TSTAMP == 32
        #define RKH_TRC_TSTAMP() \
            RKH_TRC_UI32(rkh_trc_getts())
    #else
        #define RKH_TRC_TSTAMP() \
            RKH_TRC_UI16(rkh_trc_getts())
    #endif
#else
    #define RKH_TRC_TSTAMP()
#endif

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */

#if RKH_CFG_TRC_RTFIL_EN == RKH_ENABLED
/**
 *  \brief
 *  Filter table of trace events.
 *
 *  The trace filter management is similar to the native priority scheme.
 *  In this case, each trace event is assigned a unique number
 *  (See RKH_TE_<group>_<event> definitions). When a event is ready to record 
 *  a trace its corresponding bit in the filter table must be clear. The size 
 *  of trceftbl[] depends on #RKH_TOT_NUM_TRC_EVTS.
 *
 *  Trace event number = | 0 | Y | Y | Y | Y | X | X | X |\n
 *
 *  Y's:	index into trceftbl[ #RKH_TRC_MAX_EVENTS_IN_BYTES ] table.\n
 *  X's:	bit position in trceftbl[ Y's ].\n
 *
 *  The lower 3 bits (X's) of the trace event number are used to determine
 *  the bit position in trceftbl[], while the next four most significant bits
 *  (Y's) are used to determine the index into trceftbl[].
 */
static rui8_t trceftbl[RKH_TRC_MAX_EVENTS_IN_BYTES];

/**
 *  \brief
 *  Filter table of trace groups.
 *
 *  Each bit in #trcgfilter is used to indicate whenever any trace group
 *  is filtered out its events. See RKH_TG_<group> and RKH_TG_T.
 *
 *  \code
 *  bit position =   7   6   5   4   3   2   1   0   -- Groups
 *  trcgfilter   = | Y | Y | Y | Y | Y | Y | Y | Y |
 *                           |		         |   |___ RKH_TG_MP
 *						     |			     |_______ RKH_TG_RQ
 *                           |				          ...
 *                           |_______________________ RKH_TG_FWK
 *	\endcode
 */
static rui8_t trcgfilter;

/**
 *  \brief
 *  Filter table of trace points associated with the SMA (AO).
 *
 *  The trace filter management is similar to the native priority scheme.
 *  In this case, each SMA is assigned a unique priority number. When a SMA
 *  is ready to record a trace its corresponding bit in the filter table
 *  must be clear. The size of #trcsmaftbl[] depends on
 *  #RKH_CFG_FWK_MAX_SMA (see rkhcfg.h).
 *
 *  SMA priority number = | Y | Y | Y | Y | Y | X | X | X |\n
 *
 *  Y's:	index into trcsmaftbl[ #RKH_TRC_MAX_SMA ] table.\n
 *  X's:	bit position in trcsmaftbl[ Y's ].\n
 *
 *  The lower 3 bits (X's) of the SMA priority number are used to determine
 *  the bit position in trcsmaftbl[], while the next five most significant bits
 *  (Y's) are used to determine the index into trcsmaftbl[].
 */
static rui8_t trcsmaftbl[RKH_TRC_MAX_SMA];

/**
 *  \brief
 *  Filter table of trace points associated with the event signals.
 *	Similar to trcsmaftbl[].
 *
 *  Signal number = | Y | ... | Y | Y | Y | X | X | X |\n
 *
 *  Y's:	index into trcsigftbl[ #RKH_TRC_MAX_SIGNALS ] table.\n
 *  X's:	bit position in trcsigftbl[ Y's ].\n
 *
 *  The lower 3 bits (X's) of the event signal are used to determine the bit
 *  position in trcsigftbl[], while the next most significant bits (Y's) are
 *  used to determine the index into trcsigftbl[].
 */
static rui8_t trcsigftbl[RKH_TRC_MAX_SIGNALS];

/**
 *  \brief
 *  The tables to filter trace events related to signal and active objects.
 */
const RKH_TRC_FIL_T fsig = {RKH_TRC_MAX_SIGNALS,   trcsigftbl};
const RKH_TRC_FIL_T fsma = {RKH_TRC_MAX_SMA,       trcsmaftbl};

/** 
 *  \brief
 *  Map a trace event ID to its corresponding filter.
 */
static RKHROM RKH_GMTBL_T trcgmtbl[] =
{
    {RKH_MP_TTBL_OFFSET,    RKH_MP_TTBL_RANGE},
    {RKH_RQ_TTBL_OFFSET,    RKH_RQ_TTBL_RANGE},
    {RKH_SMA_TTBL_OFFSET,   RKH_SMA_TTBL_RANGE},
    {RKH_SM_TTBL_OFFSET,    RKH_SM_TTBL_RANGE},
    {RKH_TIM_TTBL_OFFSET,   RKH_TIM_TTBL_RANGE},
    {RKH_FWK_TTBL_OFFSET,   RKH_FWK_TTBL_RANGE},
    {RKH_USR_TTBL_OFFSET,   RKH_USR_TTBL_RANGE},
    {RKH_UT_TTBL_OFFSET,    RKH_UT_TTBL_RANGE}
};
#endif

static rui8_t trcstm[RKH_CFG_TRC_SIZEOF_STREAM];
static rui8_t *trcin, *trcout, *trcend;
static rui8_t chk;
static rui8_t nseq;
static TRCQTY_T trcqty;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */

static
void
setAllFilters(rui8_t *filterTbl, rui8_t value, ruint size)
{
    rui8_t *ft, ix, c;

    for (ft = filterTbl, ix = 0,
         c = (rui8_t)((value == FILTER_OFF) ? 0xFF : 0);
         ix < size; ++ix, ++ft)
    {
        *ft = c;
    }
}

static
void
setOneFilter(rui8_t *filterByte, rui8_t value, rui8_t bitPos)
{
    if (value == FILTER_OFF)
    {
        *filterByte |= rkh_maptbl[bitPos];
    }
    else
    {
        *filterByte &= ~rkh_maptbl[bitPos];
    }
}

static
rbool_t
isOffFilter(rui8_t *filterTbl, RKH_TE_ID_T filter)
{
    rui8_t x;
    RKH_TE_ID_T y;

    y = filter >> 3;
    x = (rui8_t)(filter & 7);
    return (*(filterTbl + y) & rkh_maptbl[x]) != 0;
}

/* ---------------------------- Global functions --------------------------- */
void
rkh_trcStream_init(void)
{
    trcin = trcout = trcstm;
    trcqty = 0;
    nseq = 0;
    trcend = &trcstm[RKH_CFG_TRC_SIZEOF_STREAM];
    RKH_TRC_U8_RAW(RKH_FLG);
}

void
rkh_trc_init(void)
{
    rkh_trcStream_init();
}

void
rkh_trc_put(rui8_t b)
{
    *trcin++ = b;
    ++trcqty;

    if (trcin == trcend)
    {
        trcin = trcstm;
    }

    if (trcqty >= RKH_CFG_TRC_SIZEOF_STREAM)
    {
        trcqty = RKH_CFG_TRC_SIZEOF_STREAM;
        trcout = trcin;
    }
}

rui8_t *
rkh_trc_get(void)
{
    rui8_t *trByte = (rui8_t *)0;

    if (trcqty == 0)
    {
        return trByte;
    }

    trByte = trcout++;
    --trcqty;

    if (trcout >= trcend)
    {
        trcout = trcstm;
    }

    return trByte;
}

rui8_t *
rkh_trc_get_block(TRCQTY_T *nget)
{
    rui8_t *trByte = (rui8_t *)0;
    TRCQTY_T n;

    if (trcqty == (TRCQTY_T)0)
    {
        *nget = (TRCQTY_T)0;
        return trByte;
    }

    trByte = trcout;

    /* Calculates the number of bytes to be retrieved */
    n = (TRCQTY_T)(trcend - trcout);    /* bytes until the end */
    if (n > trcqty)
    {
        n = trcqty;
    }
    if (n > *nget)
    {
        n = *nget;
    }

    *nget = n;
    trcout += n;
    trcqty -= n;

    if (trcout >= trcend)
    {
        trcout = trcstm;
    }

    return trByte;
}

#if RKH_CFG_TRC_RTFIL_EN == RKH_ENABLED
rbool_t
rkh_trc_isoff_(RKH_TE_ID_T e)
{
    RKH_TE_ID_T evt;
    RKH_TG_T grp;
    RKH_GM_OFFSET_T offset;

    evt = GETEVT(e);
    grp = GETGRP(e);
    offset = trcgmtbl[grp].offset;

    return (((trcgfilter & rkh_maptbl[grp]) != 0) && 
              isOffFilter(&trceftbl[offset], evt));
}

void
rkh_trc_filter_group_(rui8_t ctrl, RKH_TG_T grp, rui8_t mode)
{
    RKH_GM_OFFSET_T offset;
    RKH_GM_RANGE_T range;

    if (grp == RKH_TRC_ALL_GROUPS)
    {
        trcgfilter = (rui8_t)((ctrl == FILTER_OFF) ? 0xFF : 0);
        return;
    }

    setOneFilter(&trcgfilter, ctrl, grp);

    if (mode == ECHANGE)
    {
        offset = trcgmtbl[grp].offset;
        range = trcgmtbl[grp].range;
        setAllFilters(&trceftbl[offset], ctrl, range);
    }
}

void
rkh_trc_filter_event_(rui8_t ctrl, RKH_TE_ID_T evt)
{
    RKH_TG_T grp;
    RKH_TE_ID_T e;
    RKH_GM_OFFSET_T offset;

    RKH_ASSERT(evt <= RKH_TRC_ALL_EVENTS);

    if (evt == RKH_TRC_ALL_EVENTS)
    {
        setAllFilters(trceftbl, ctrl, RKH_TRC_MAX_EVENTS_IN_BYTES);
        trcgfilter = (rui8_t)((ctrl == FILTER_OFF) ? 0xFF : 0);
    }
    else
    {
        e = GETEVT(evt);
        grp = GETGRP(evt);
        offset = (RKH_GM_OFFSET_T)(trcgmtbl[grp].offset + (e >> 3));

        setOneFilter(&trceftbl[offset], ctrl, e & 7);
        if (ctrl == FILTER_OFF)
        {
            trcgfilter |= rkh_maptbl[grp];
        }
    }
}

rbool_t
rkh_trc_symFil_isoff(const RKH_TRC_FIL_T *filter, RKH_TRC_FSLOT slot)
{
    RKH_REQUIRE((filter != (const RKH_TRC_FIL_T *)0) && 
                (slot < (filter->size << 3)));
    return isOffFilter(filter->tbl, (RKH_TE_ID_T)slot);
}

void
rkh_trc_symFil(const RKH_TRC_FIL_T *filter, RKH_TRC_FSLOT slot, rui8_t mode)
{
    rui8_t x, onoff;
    RKH_TRC_FSLOT y;

    if ((filter == (const RKH_TRC_FIL_T *)0) || (slot > (filter->size << 3)))
    {
        RKH_ASSERT(0);
        return;
    }

    onoff = (rui8_t)(mode & RKH_FILTER_MODE_MASK);
    if (mode & RKH_TRC_ALL_FILTERS)
    {
        setAllFilters(filter->tbl, onoff, filter->size);
    }
    else
    {
        y = (RKH_TRC_FSLOT)(slot >> 3);
        x = (rui8_t)(slot & 7);
        setOneFilter(filter->tbl + y, onoff, x);
    }
}
#endif

void
rkh_trc_begin(RKH_TE_ID_T eid)
{
    chk = 0;            /* Initialize the trace record checksum */
    RKH_TRC_TE_ID(eid); /* Insert the event ID */
    RKH_TRC_NSEQ();     /* Insert the sequence number */
    RKH_TRC_TSTAMP();   /* Insert the timestamp */
}

void
rkh_trc_end(void)
{
    RKH_TRC_CHK();
    RKH_TRC_FLG();
    RKH_HOOK_PUT_TRCEVT();
}

void
rkh_trc_clear_chk(void)
{
    chk = 0;
}

void
rkh_trc_u8(rui8_t d)
{
    chk = (rui8_t)(chk + d);
    if ((d == RKH_FLG) || (d == RKH_ESC))
    {
        rkh_trc_put(RKH_ESC);
        rkh_trc_put((rui8_t)(d ^ RKH_XOR));
    }
    else
    {
        rkh_trc_put(d);
    }
}

void
rkh_trc_u16(rui16_t d)
{
    rkh_trc_u8((rui8_t)d);
    d >>= 8;
    rkh_trc_u8((rui8_t)d);
}

void
rkh_trc_u32(rui32_t d)
{
    rkh_trc_u8((rui8_t)d);
    d >>= 8;
    rkh_trc_u8((rui8_t)d);
    d >>= 8;
    rkh_trc_u8((rui8_t)d);
    d >>= 8;
    rkh_trc_u8((rui8_t)d);
}

void
rkh_trc_str(const char *s)
{
    while (*s != '\0')
        rkh_trc_u8((rui8_t)*s++);
    rkh_trc_u8('\0');
}

void
rkh_trc_obj(RKH_TE_ID_T tre, rui8_t *obj, const char *obj_name)
{
    RKH_TRC_BEGIN_WOFIL(tre)
        RKH_TRC_SYM(obj);
        RKH_TRC_STR(obj_name);
    RKH_TRC_END_WOFIL()
    RKH_TRC_FLUSH();
}

void
rkh_trc_sig(RKH_SIG_T sig, const char *sig_name)
{
    RKH_TRC_BEGIN_WOFIL(RKH_TE_FWK_SIG)
        RKH_TRC_SIG(sig);
        RKH_TRC_STR(sig_name);
    RKH_TRC_END_WOFIL()
    RKH_TRC_FLUSH();
}

void
rkh_trc_ao(void *ao)
{
    RKH_TRC_BEGIN_WOFIL(RKH_TE_FWK_AO)
        RKH_TRC_SYM(ao);
        RKH_TRC_STR(RKH_GET_AO_NAME((RKH_SMA_T *)ao));
    RKH_TRC_END_WOFIL()
    RKH_TRC_FLUSH();
}

void
rkh_trc_state(void *ao, rui8_t *state)
{
    
    RKH_TRC_BEGIN_WOFIL((CB((state))->type & RKH_REGULAR) == 0 ? 
                                                        RKH_TE_FWK_PSTATE:
                                                        RKH_TE_FWK_STATE)
        RKH_TRC_SYM(ao);
        RKH_TRC_SYM(state);
        RKH_TRC_STR(RKH_GET_VERTEX_NAME(state));
    RKH_TRC_END_WOFIL()
    RKH_TRC_FLUSH();
}

#if RKH_CFG_TRC_USER_TRACE_EN == RKH_ENABLED
void
rkh_trc_fmt_u8(rui8_t fmt, rui8_t d)
{
    rkh_trc_u8(fmt);
    rkh_trc_u8(d);
}

void
rkh_trc_fmt_u16(rui8_t fmt, rui16_t d)
{
    rkh_trc_u8(fmt);
    rkh_trc_u16(d);
}

void
rkh_trc_fmt_u32(rui8_t fmt, rui32_t d)
{
    rkh_trc_u8(fmt);
    rkh_trc_u32(d);
}

void
rkh_trc_fmt_str(const char *s)
{
    rkh_trc_u8((rui8_t)RKH_STR_T);
    while (*s != '\0')
        rkh_trc_u8((rui8_t)*s++);
    rkh_trc_u8('\0');
}

void
rkh_trc_fmt_mem(const rui8_t *mem, rui8_t size)
{
    rkh_trc_u8((rui8_t)RKH_MEM_T);
    rkh_trc_u8(size);
    while (size != 0)
    {
        rkh_trc_u8((rui8_t)*mem++);
        --size;
    }
}
#endif

void 
rkh_trc_filter_get(RKH_FilterTbl *outFilterTbl)
{
    if (outFilterTbl == (RKH_FilterTbl *)0)
        return;
    outFilterTbl->signal = &fsig;
    outFilterTbl->ao = &fsma;
    outFilterTbl->event = trceftbl;
    outFilterTbl->group = &trcgfilter;
    outFilterTbl->grpFilMap = trcgmtbl;
}

#endif
/* ------------------------------ End of file ------------------------------ */
