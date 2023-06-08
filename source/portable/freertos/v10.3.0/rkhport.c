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
 *  \file       rkhport.c
 *  \brief      FreeRTOS v10.0.1 port
 *
 *  \ingroup    port
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.03.18  DaBa Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña       db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "rkhfwk_dynevt.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "timers.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
RKH_MODULE_NAME(rkhport)
RKH_MODULE_VERSION(rkhport, 1.00)
RKH_MODULE_DESC(rkhport, "FreeRTOS v10.3.0")

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
#if defined(RKH_USE_TRC_SENDER)
static rui8_t l_isr_tick;
#endif
static rui8_t running;

StaticTimer_t xTimerBuffers[ 1 ];

static StaticQueue_t QueueBuff[RKH_CFG_FWK_MAX_SMA];

/* ----------------------- Local function prototypes ----------------------- */
static void thread_function(void *arg);

/* ---------------------------- Local functions ---------------------------- */
static
void
thread_function(void *arg)
{
    RKH_SMA_T *sma;
    RKH_SR_ALLOC();

    sma = (RKH_SMA_T *)arg;

    while (sma->running != (rbool_t)0)
    {
        RKH_EVT_T *e = rkh_sma_get(sma);
        RKH_SMA_DISPATCH(sma, e);
        RKH_FWK_GC(e, sma);
    }

    rkh_sma_unregister(sma);

    RKH_TR_SMA_TERM(sma, RKH_GET_PRIO(sma));

    vTaskDelete(NULL);
}

void
tick_timerCallback(TimerHandle_t xTimer)
{
	(void) xTimer;
	RKH_TIM_TICK(&l_isr_tick);
}

/* ---------------------------- Global functions --------------------------- */
const
char *
rkhport_get_version(void)
{
    return RKH_MODULE_GET_VERSION();
}

const
char *
rkhport_get_desc(void)
{
    return RKH_MODULE_GET_DESC();
}

void
rkh_fwk_init(void)
{
	RKH_TRC_OPEN();

#if defined(RKH_USE_TRC_SENDER)
	RKH_TR_FWK_OBJ(&l_isr_tick);
#endif

	TimerHandle_t tick_TimerHandle = NULL;

	tick_TimerHandle = xTimerCreateStatic( "isr_Timer",
			configTICK_RATE_HZ   /* wait for the tick interval */
			/ RKH_CFG_FWK_TICK_RATE_HZ,
			pdTRUE,
			( void * ) 0,
			tick_timerCallback,
			&( xTimerBuffers[0] )
	);

	RKH_ENSURE(tick_TimerHandle);

    xTimerStart( tick_TimerHandle, (TickType_t) NULL );

}

void
rkh_fwk_enter(void)
{
    RKH_SR_ALLOC();

    RKH_HOOK_START();                   /* start-up callback */
    RKH_TR_FWK_EN();

    vTaskStartScheduler();

    RKH_HOOK_EXIT();                    /* cleanup callback */
    RKH_TRC_CLOSE();                    /* cleanup the trace session */

    vTaskDelete(NULL);
}

void
rkh_fwk_exit(void)
{
    RKH_SR_ALLOC();

    RKH_TR_FWK_EX();
    RKH_HOOK_EXIT();
    running = (rui8_t)0;
}

void
rkh_sma_terminate(RKH_SMA_T *sma)
{
    sma->running = (rbool_t)0;
    vQueueDelete(sma->equeue);
}

void
rkh_sma_activate(RKH_SMA_T *sma, const RKH_EVT_T **qs, RKH_QUENE_T qsize,
                 void *stks, rui32_t stksize)
{
    rui8_t prio;
    TaskHandle_t TaskHandle = NULL;
    StaticQueue_t *pStaticQueue;

    RKH_SR_ALLOC();

    RKH_REQUIRE(qs != (const RKH_EVT_T **)0 && (qsize != 0));
    RKH_REQUIRE((stks != (void *)0) && (stksize != 0));

    RKH_ENTER_CRITICAL_();

    pStaticQueue = &QueueBuff[RKH_GET_PRIO(sma)];

    sma->equeue = xQueueCreateStatic(qsize,
                                     sizeof(void*),
                                     (uint8_t *)qs,
                                     pStaticQueue);

    RKH_ASSERT(sma->equeue);

#ifdef DEBUG
	#if RKH_CFG_TRC_EN == RKH_ENABLED
    	/* Registering the queue to inspect it with FreeRTOS debug tools */
    	vQueueAddToRegistry(sma->equeue,
    			sma->sm.romrkh->name);
	#endif
#endif

    rkh_sma_register(sma);
    rkh_sm_init((RKH_SM_T *)sma);

    prio = RKH_LOWEST_PRIO - RKH_GET_PRIO(sma) + tskIDLE_PRIORITY;

    sma->running = (rbool_t)1;

    TaskHandle = xTaskCreateStatic(thread_function,    /* function */
#if RKH_CFG_TRC_EN == RKH_ENABLED
                                   sma->sm.romrkh->name,    /* name */
#else
								   NULL,
#endif
                                   stksize,            /* stack size */
                                   sma,                /* function argument */
                                   prio,               /* priority */
                                   stks,               /* stack storage */
                                   &sma->thread);      /* task data structure */

    RKH_ASSERT(TaskHandle);

    RKH_TR_SMA_ACT(sma, RKH_GET_PRIO(sma), qsize);

    RKH_EXIT_CRITICAL_();
}

#if defined(RKH_USE_TRC_SENDER)
void
rkh_sma_post_fifo(RKH_SMA_T *sma, const RKH_EVT_T *e,
                  const void *const sender)
#else
void
rkh_sma_post_fifo(RKH_SMA_T *sma, const RKH_EVT_T *e)
#endif
{
    BaseType_t taskwoken = pdFALSE;
    BaseType_t rv;
    RKH_SR_ALLOC();

    RKH_ENTER_CRITICAL_();

    RKH_HOOK_SIGNAL(e);

    RKH_INC_REF(e);

    rv = xQueueSendFromISR(sma->equeue, &e, &taskwoken);

    RKH_ASSERT(rv == pdTRUE);

    RKH_TR_SMA_FIFO(sma, e, sender, e->pool, e->nref, 0, 0);

    RKH_EXIT_CRITICAL_();
}

#if (RKH_CFG_QUE_PUT_LIFO_EN == RKH_ENABLED) && \
    (defined(RKH_USE_TRC_SENDER))
void
rkh_sma_post_lifo(RKH_SMA_T *sma, const RKH_EVT_T *e,
                  const void *const sender)
#else
void
rkh_sma_post_lifo(RKH_SMA_T *sma, const RKH_EVT_T *e)
#endif
{
    BaseType_t taskwoken = pdFALSE;
    BaseType_t rv;
    RKH_SR_ALLOC();

    RKH_ENTER_CRITICAL_();

    RKH_HOOK_SIGNAL(e);

    RKH_INC_REF(e);

    rv = xQueueSendToFrontFromISR(sma->equeue, &e, &taskwoken);

    RKH_ASSERT(rv == pdTRUE);

    RKH_TR_SMA_LIFO(sma, e, sender, e->pool, e->nref, 0, 0);

    RKH_EXIT_CRITICAL_();
}

RKH_EVT_T *
rkh_sma_get(RKH_SMA_T *sma)
{
    BaseType_t rv;
    RKH_EVT_T *e;

    RKH_SR_ALLOC();

    RKH_ENTER_CRITICAL_();

    rv = xQueueReceive(sma->equeue, &e, portMAX_DELAY);

    RKH_ASSERT(rv == pdTRUE);

    RKH_TR_SMA_GET(sma, e, e->pool, e->nref, 0, 0);

    RKH_EXIT_CRITICAL_();

    return e;
}

/* ------------------------------ End of file ------------------------------ */
