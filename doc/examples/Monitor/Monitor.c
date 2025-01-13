/**
 *  \file       Monitor.c
 *  \brief      Monitor application. It demonstrates how to write a
 *              non-reactive active object.
 */

/* -------------------------- Development history -------------------------- */
/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "Monitor.h"
#include "priority.h"
#include "events.h"
#include "rkhfwk_cast.h"
#include "signals.h"
#include <stdbool.h>
#include <stdint.h>

/* ----------------------------- Local macros ------------------------------ */
/* ......................... Declares active object ........................ */
/* ................... Declares states and pseudostates .................... */
/* ........................ Declares effect actions ........................ */
/* ......................... Declares entry actions ........................ */
/* ......................... Declares exit actions ......................... */
/* ............................ Declares guards ............................ */
/* ........................ States and pseudostates ........................ */
/* ............................. Active object ............................. */
//! [Monitor definition]
struct Monitor
{
    RKH_SMA_T base;
    int baz;
    RKHTmEvt syncTmr;
};
//! [Monitor definition]

//! [Monitor instance]
RKH_SMA_CREATE(Monitor, monitor, MonitorPrio, 0, NULL, NULL, NULL);
//! [Monitor instance]
//! [Monitor pointer definition]
RKH_SMA_DEF_PTR_TYPE(Monitor, monitor);
//! [Monitor pointer definition]

/* ------------------------------- Constants ------------------------------- */
RKH_MODULE_NAME(Monitor)

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
processCmd(uint8_t cmdId)
{
}

static void
processData(uint8_t channel, double value)
{
}

static void
showKeepAliveOnScreen(void)
{
}

//! [Activate]
static void
activate(RKH_SMA_T* me, const RKH_EVT_T** qSto, RKH_QUENE_T qSize,
                   void* stkSto, rui32_t stkSize)
{
    (void)stkSto;
    (void)stkSize;
    Monitor* realMe = RKH_DOWNCAST(Monitor, me);
    RKH_SR_ALLOC();

    RKH_REQUIRE((qSto != (const RKH_EVT_T**)0) && (qSize != (RKH_QUENE_T)0));
    rkh_queue_init(&me->equeue, (const void**)qSto, qSize, me);
    rkh_sma_register(me);
    RKH_TMR_PERIODIC(&realMe->syncTmr.tmr, me, TIME_PER, TIME_PER);
    RKH_TR_SMA_ACT(me, RKH_GET_PRIO(me), qSize);
}
//! [Activate]

/**
 *  \brief
 *  Process received messages.
 *
 *  The Monitor active object can receive data messages directly from the ADCs
 *  ISRs or command messages from other active objects or task. When task() is
 *  called, the message is retrieved and processed in two different ways
 *  depending on the sender and the type of message. If the message comes from
 *  one of the ADC’s ISR then the message is processed by calling the function
 *  processData() and if the message comes from any other task then the message
 *  is processed by calling the function processCmd().
 */
//! [Task]
static void
task(RKH_SMA_T* me, void* arg)
{
    RKH_EVT_T* evt = RKH_EVT_CAST(arg);
    Monitor* realMe = RKH_DOWNCAST(Monitor, me);

    RKH_REQUIRE((me != RKH_CAST(RKH_SMA_T, 0)) && (evt != RKH_CAST_EVT(0)));
    switch (evt->e)
    {
        case sigTimeout:
            showKeepAliveOnScreen();
            break;
        case sigPushedButton:
            {
                PushedButton* realEvt = RKH_DOWNCAST(PushedButton, evt);
                processCmd(realEvt->which);
            }
            break;
        case sigSimulator:
            {
                Simulator* realEvt = RKH_DOWNCAST(Simulator, evt);
                // Deal with additional fields: Simulator::additional
                PushedButton* base = RKH_DOWNCAST(PushedButton, evt);
                processCmd(base->which);
            }
            break;
        case sigADC:
            {
                ADC* realEvt = RKH_DOWNCAST(ADC, evt);
                processData(realEvt->channel, realEvt->value);
            }
            break;
        default:
            /* Unknown event */
            break;
    }
    RKH_TR_SM_DCH(me, evt, RKH_CAST(RKH_ST_T, 0));
}
//! [Task]

/* ............................ Effect actions ............................. */
/* ............................. Entry actions ............................. */
/* ............................. Exit actions .............................. */
/* ................................ Guards ................................. */
/* ---------------------------- Global functions --------------------------- */
//! [Constructor definition]
void
Monitor_ctor(Monitor* const me, int baz)
{
    static const RKHSmaVtbl vtbl =
    {
        activate,
        task,
        rkh_sma_post_fifo,
        rkh_sma_post_lifo
    };

    RKH_REQUIRE(me != RKH_CAST(Monitor, 0));
    rkh_sma_ctor(&me->base, &vtbl);
    RKH_SET_STATIC_EVENT(&me->syncTmr, sigTimeout);
    RKH_TMR_INIT(&me->syncTmr.tmr, RKH_UPCAST(RKH_EVT_T, &me->syncTmr), NULL);
    me->baz = baz;
}
//! [Constructor definition]

int
Monitor_getBaz(Monitor* const me)
{
    RKH_REQUIRE(me != RKH_CAST(Monitor, 0));
    return me->baz;
}

/* ------------------------------ End of file ------------------------------ */
