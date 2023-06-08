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
 *  \file       rkhport.h
 *  \brief      FreeRTOS v10.2.0 port
 *
 *  \ingroup    port
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.03.18  DaBa Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Bali#a       db@vortexmakes.com
 */

/* --------------------------------- Module -------------------------------- */
#ifndef __RKHPORT_H__
#define __RKHPORT_H__

/* ----------------------------- Include files ----------------------------- */
#include "FreeRTOS.h"       /* the main freertos include file */
#include "task.h"
#include "queue.h"
#include "rkhqueue.h"
#include "rkhmempool.h"

/* --------------------------------- Macros -------------------------------- */
#define RKH_DIS_INTERRUPT()
#define RKH_ENA_INTERRUPT()
#define RKH_CPUSR_TYPE                  UBaseType_t

#define RKH_ENTER_CRITICAL(dummy) \
    do \
    {  \
        sr = taskENTER_CRITICAL_FROM_ISR(); \
    }  \
    while (0)

#define RKH_EXIT_CRITICAL(dummy) \
    do \
    { \
        taskEXIT_CRITICAL_FROM_ISR(sr);  \
    } \
    while (0)

/* ------------------------------- Constants ------------------------------- */
/**
 *	If the #RKH_CFGPORT_SMA_THREAD_EN is set to 1, each SMA (active object) has
 ***its own thread of execution.
 */
#define RKH_CFGPORT_SMA_THREAD_EN           RKH_ENABLED

/**
 *	If the #RKH_CFGPORT_SMA_THREAD_EN and #RKH_CFGPORT_SMA_THREAD_DATA_EN
 *	are set to 1, each SMA (active object) has its own thread of execution
 *	and its own object data.
 */
#define RKH_CFGPORT_SMA_THREAD_DATA_EN      RKH_ENABLED /* RKH_DISABLED// */

/**
 *  If the #RKH_CFGPORT_NATIVE_SCHEDULER_EN is set to 1 then RKH will
 *  include the simple, cooperative, and nonpreemptive scheduler RKHS.
 *  When #RKH_CFGPORT_NATIVE_SCHEDULER_EN is enabled RKH also will
 *  automatically define #RKH_EQ_TYPE, and include rkh_sma_block(),
 *  rkh_sma_setReady(), rkh_sma_setUnready(), and assume the native
 *  priority scheme.
 */
#define RKH_CFGPORT_NATIVE_SCHEDULER_EN     RKH_DISABLED

/**
 *  If the #RKH_CFGPORT_NATIVE_EQUEUE_EN is set to 1 and the native event
 *  queue is enabled (see #RKH_CFG_RQ_EN) then RKH will include its own
 *  implementation of rkh_sma_post_fifo(), rkh_sma_post_lifo(), and
 *  rkh_sma_get() functions.
 */
#define RKH_CFGPORT_NATIVE_EQUEUE_EN        RKH_DISABLED

/**
 *  If the #RKH_CFGPORT_NATIVE_DYN_EVT_EN is set to 1 and the native
 *  fixed-size memory block facility is enabled (see #RKH_CFG_MP_EN) then
 *  RKH will include its own implementation of dynamic memory management.
 *  When #RKH_CFGPORT_NATIVE_DYN_EVT_EN is enabled RKH also will provide
 *  the event pool manager implementation based on its native memory pool
 *  module.
 */
#define RKH_CFGPORT_NATIVE_DYN_EVT_EN       RKH_ENABLED

/**
 *	If the #RKH_CFGPORT_REENTRANT_EN is set to 1, the RKH event dispatch
 *	allows to be invoked from several threads of executions. Enable this
 *	only if the application is based on a multi-thread architecture.
 */
#define RKH_CFGPORT_REENTRANT_EN            RKH_ENABLED

/**
 *  Specify the size of void pointer. The valid values [in bits] are
 *  16 or 32. Default is 32. See RKH_TRC_SYM() macro.
 */
#define RKH_CFGPORT_TRC_SIZEOF_PTR          32u

/**
 *  Specify the size of function pointer. The valid values [in bits] are
 *  16 or 32. Default is 32. See RKH_TUSR_FUN() and RKH_TRC_FUN() macros.
 */
#define RKH_CFGPORT_TRC_SIZEOF_FUN_PTR      32u

/**
 *  Specify the number of bytes (size) used by the trace record timestamp.
 *  The valid values [in bits] are 8, 16 or 32. Default is 16.
 */
#define RKH_CFGPORT_TRC_SIZEOF_TSTAMP       32u

/**
 *  If the #RKH_CFGPORT_SMA_QSTO_EN is set to 1 then RKH_SMA_ACTIVATE()
 *  macro invokes the rkh_sma_activate() function ignoring the external
 *  event queue storage argument, \c qs.
 */
#define RKH_CFGPORT_SMA_QSTO_EN             RKH_ENABLED

/**
 *  If the #RKH_CFGPORT_SMA_STK_EN is set to 0 then RKH_SMA_ACTIVATE()
 *  macro invokes the rkh_sma_activate() function ignoring the thread's
 *  stack related arguments, \c stks and \c stksize.
 */
#define RKH_CFGPORT_SMA_STK_EN              RKH_ENABLED

/*
 *  Declaring an object RKHROM announces that its value will
 *  not be changed and it will be stored in ROM.
 */
#define RKHROM                              const

/**
 * Native event queue data type
 */
#define RKH_EQ_TYPE                         QueueHandle_t

/**
 * Operating system blocking primitive.
 */
#define RKH_OSSIGNAL_TYPE                   void*

/**
 * Thread handle type for definition
 */
#define RKH_THREAD_TYPE                     StaticTask_t

/**
 *  Data type to declare thread stack
 */
 #define RKH_THREAD_STK_TYPE                StackType_t

/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void rkh_enter_critical(void);
void rkh_exit_critical(void);
const char *rkhport_get_version(void);
const char *rkhport_get_desc(void);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif
/* ------------------------------ End of file ------------------------------ */
