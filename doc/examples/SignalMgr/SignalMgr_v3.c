/**
 *  \file       SignalMgr.c
 *  \brief      SignalMgr application. It demonstrates how to write a periodic
 *              reactive active object using orthogonal regions.
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
#include "rkhevt.h"
#include "rkhfwk_cast.h"
#include "rkhitl.h"
#include "rkhsma.h"
#include "signals.h"
#include <stdbool.h>

/* ----------------------------- Local macros ------------------------------ */
/* ......................... Declares active object ........................ */
/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE idle, active;

/* ........................ Declares effect actions ........................ */
static void init(SignalMgr* const me, RKH_EVT_T* pe);
static void activeProcess(SignalMgr* const me, RKH_EVT_T* pe);
static void propagateEvent(SignalMgr* const me, RKH_EVT_T* pe);

/* ......................... Declares entry actions ........................ */
/* ......................... Declares exit actions ......................... */
/* ............................ Declares guards ............................ */
static rbool_t isGreaterThanLevel1(SignalMgr* const me, RKH_EVT_T *pe);
static rbool_t isLessThanLevel1(SignalMgr* const me, RKH_EVT_T *pe);

/* ........................ States and pseudostates ........................ */
RKH_CREATE_BASIC_STATE(sync, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(sync)
    RKH_TRINT(sigSync, NULL, propagateEvent),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(idle, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(idle)
    RKH_TRREG(sigDataReady, isGreaterThanLevel1, activeProcess, &active),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(active, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(active)
    RKH_TRREG(sigDataReady, isLessThanLevel1, NULL, &idle),
    RKH_TRINT(sigDataReady, isGreaterThanLevel1, NULL),
RKH_END_TRANS_TABLE

/* ............................. Active object ............................. */
struct SignalMgr
{
    RKH_SMA_T base;
    int baz;
    RKHTmEvt syncTmr;
    DataReady signalReady;
    RKH_SM_T processing;
};

RKH_SMA_CREATE(SignalMgr, 
        signalMgr, 
        SignalMgrPrio, 
        HCAL, &idle, init, NULL);
RKH_SMA_DEF_PTR_TYPE(SignalMgr, signalMgr);

/* ------------------------------- Constants ------------------------------- */
RKH_MODULE_NAME(SignalMgr)

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
RKH_DCLR_SM_CONST_GLOBAL(processing);

/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
dispatch(RKH_SMA_T* me, void* arg)
{
    RKH_SM_T *region;

    region = &(RKH_DOWNCAST(SignalMgr, me)->processing);
    rkh_sm_dispatch(RKH_UPCAST(RKH_SM_T, me), RKH_EVT_CAST(arg));
    rkh_sm_dispatch(RKH_CAST(RKH_SM_T, region), RKH_EVT_CAST(arg));
}

/* ............................ Effect actions ............................. */
static void
init(SignalMgr* const me, RKH_EVT_T *pe)
{
    RKH_TR_FWK_AO(me);
    RKH_TR_FWK_QUEUE(&RKH_UPCAST(RKH_SMA_T, me)->equeue);
    RKH_TR_FWK_STATE(me, &idle);
    RKH_TR_FWK_STATE(me, &active);
    RKH_TR_FWK_STATE(me, &sync);
    RKH_TR_FWK_SIG(sigSync);
    RKH_TR_FWK_SIG(sigDataReady);

    RKH_TMR_PERIODIC(&me->syncTmr.tmr,
                     RKH_UPCAST(RKH_SMA_T, me),
                     TIME_PER,
                     TIME_PER);
    rkh_sm_init(RKH_UPCAST(RKH_SM_T, &me->processing));
}

static void
activeProcess(SignalMgr* const me, RKH_EVT_T *pe)
{
    Actuator_set(me->signalReady.value);
}

static void
propagateEvent(SignalMgr* const me, RKH_EVT_T* pe)
{
    double sensorValue = Sensor_get();
    double actuatorOutput = Sensor_process(sensorValue);
    me->signalReady.value = actuatorOutput;
    rkh_sm_propagate(RKH_UPCAST(RKH_SM_T, me), RKH_EVT_CAST(&me->signalReady));
}

/* ............................. Entry actions ............................. */
/* ............................. Exit actions .............................. */
/* ................................ Guards ................................. */
static rbool_t
isGreaterThanLevel1(SignalMgr* const me, RKH_EVT_T* pe)
{
    return (RKH_DOWNCAST(DataReady, pe)->value > 1.23) ? true : false;
}

static rbool_t
isLessThanLevel1(SignalMgr* const me, RKH_EVT_T* pe)
{
    return (RKH_DOWNCAST(DataReady, pe)->value <= 1.23) ? true : false;
}

/* ---------------------------- Global functions --------------------------- */
void
SignalMgr_ctor(SignalMgr* const me, int baz)
{
    static const RKHSmaVtbl vtbl =
    {
        rkh_sma_activate,
        dispatch,
        rkh_sma_post_fifo,
        rkh_sma_post_lifo
    };

    RKH_REQUIRE(me != RKH_CAST(SignalMgr, 0));
    rkh_sma_ctor(&me->base, &vtbl);
    RKH_SET_STATIC_EVENT(&me->signalReady, sigDataReady);
    RKH_SET_STATIC_EVENT(&me->syncTmr, sigSync);
    RKH_TMR_INIT(&me->syncTmr.tmr, RKH_UPCAST(RKH_EVT_T, &me->syncTmr), NULL);
    me->baz = baz;
    RKH_SM_INIT(&me->processing,
                processing, 0, HCAL,
                idle, NULL, NULL);
}

int
SignalMgr_getBaz(SignalMgr* const me)
{
    RKH_REQUIRE(me != RKH_CAST(SignalMgr, 0));
    return me->baz;
}

/* ------------------------------ End of file ------------------------------ */
