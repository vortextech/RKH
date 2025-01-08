/**
 *  \file       SignalMgr.c
 *  \brief      SignalMgr application. It demonstrates how to write a
 *              periodic non-reactive active object.
 */

/* -------------------------- Development history -------------------------- */
/* -------------------------------- Authors -------------------------------- */
/*
     *  LeFr  Leandro Francucci lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "SignalMgr.h"
#include "Sensor.h"
#include "Actuator.h"
#include "priority.h"
#include "events.h"
#include "rkhfwk_cast.h"
#include "signals.h"
#include <stdbool.h>

/* ----------------------------- Local macros ------------------------------ */
/* ......................... Declares active object ........................ */
/* ................... Declares states and pseudostates .................... */
/* ........................ Declares effect actions ........................ */
/* ......................... Declares entry actions ........................ */
/* ......................... Declares exit actions ......................... */
/* ............................ Declares guards ............................ */
/* ........................ States and pseudostates ........................ */
/* ............................. Active object ............................. */
struct SignalMgr
{
    RKH_SMA_T base;
    int baz;
    RKHTmEvt syncTmr;
};

RKH_SMA_CREATE(SignalMgr, 
        signalMgr, 
        SignalMgrPrio, 
        0, NULL, NULL, NULL);
RKH_SMA_DEF_PTR_TYPE(SignalMgr, signalMgr);

/* ------------------------------- Constants ------------------------------- */
RKH_MODULE_NAME(SignalMgr)

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
activate(RKH_SMA_T* me, const RKH_EVT_T** qSto, RKH_QUENE_T qSize,
                   void* stkSto, rui32_t stkSize)
{
    (void)stkSto;
    (void)stkSize;
    SignalMgr* realMe = RKH_DOWNCAST(SignalMgr, me);
    RKH_SR_ALLOC();

    RKH_REQUIRE((qSto != (const RKH_EVT_T**)0) && (qSize != (RKH_QUENE_T)0));
    rkh_queue_init(&me->equeue, (const void**)qSto, qSize, me);
    rkh_sma_register(me);
    rkh_sm_init((RKH_SM_T *)me);
    RKH_TMR_PERIODIC(&realMe->syncTmr.tmr, me, TIME_PER, TIME_PER);
    RKH_TR_SMA_ACT(me, RKH_GET_PRIO(me), qSize);
}

static void
task(RKH_SMA_T* me, void* arg)
{
    RKH_EVT_T* evt = RKH_EVT_CAST(arg);
    SignalMgr* realMe = RKH_DOWNCAST(SignalMgr, me);

    RKH_REQUIRE((me != RKH_CAST(RKH_SMA_T, 0)) && (evt != RKH_CAST_EVT(0)));
    if (evt->e == sigSync)
    {
        double sensorValue = Sensor_get();
        double actuatorOutput = Sensor_process(sensorValue);
        Actuator_set(actuatorOutput);
        RKH_TR_SM_DCH(me, evt, RKH_CAST(RKH_ST_T, 0));
    }
}

/* ............................ Effect actions ............................. */
/* ............................. Entry actions ............................. */
/* ............................. Exit actions .............................. */
/* ................................ Guards ................................. */
/* ---------------------------- Global functions --------------------------- */
void
SignalMgr_ctor(SignalMgr* const me, int baz)
{
    static const RKHSmaVtbl vtbl =
    {
        activate,
        task,
        rkh_sma_post_fifo,
        rkh_sma_post_lifo
    };

    RKH_REQUIRE(me != RKH_CAST(SignalMgr, 0));
    rkh_sma_ctor(&me->base, &vtbl);
    RKH_SET_STATIC_EVENT(&me->syncTmr, sigSync);
    RKH_TMR_INIT(&me->syncTmr.tmr, RKH_UPCAST(RKH_EVT_T, &me->syncTmr), NULL);
    me->baz = baz;
}

int
SignalMgr_getBaz(SignalMgr* const me)
{
    RKH_REQUIRE(me != RKH_CAST(SignalMgr, 0));
    return me->baz;
}

/* ------------------------------ End of file ------------------------------ */
