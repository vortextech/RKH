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
 *  \file       rkhport.h
 * 	\brief 		ARM Cortex-M MCU's, STM32 port
 *  \ingroup    port
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.02.1  Daba  v2.4.05  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña       dariosb@gmail.com
 *  CaMa  Carlos Mancón      manconci@gmail.com
 *
 */

/* --------------------------------- Notes --------------------------------- */
/*
 *  This port is dependent of a CubeMX generated HAL file, so the port is
 *  specific to that tool.
 *  It could be used with the same core and CubeMX tool in another family,
 *  i.e. STM32F2 or STM32L1, modifying the inclusion. Or the same core with-
 *  out CubeMX providing the necessary modifications.
 *
 * */

/* --------------------------------- Module -------------------------------- */
#ifndef __RKHPORT_H__
#define __RKHPORT_H__

/* ----------------------------- Include files ----------------------------- */
#include "stm32f1xx_hal.h"

#include "rkhtype.h"
#include "rkhqueue.h"
#include "rkhmempool.h"
#include "rkhsma_prio.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
#define LIBRARY_MAX_SYSCALL_IRQ_PRIORITY    5

#define MAX_SYSCALL_IRQ_PRIORITY \
                   (LIBRARY_MAX_SYSCALL_IRQ_PRIORITY << (8-__NVIC_PRIO_BITS))


#define RKH_CPUSR_TYPE                  uint32_t 
#define RKH_DIS_INTERRUPT()				__asm volatile ("cpsid i")
#define RKH_ENA_INTERRUPT()				__asm volatile ("cpsie i")
#define RKH_ENTER_CRITICAL( sr ) \
        { \
           uint32_t srOrg, srNew; \
            __asm volatile \
           ( \
                "   mrs %0, basepri                                 \n" \
                "   mov %1, %2                                      \n" \
                "   msr basepri, %1                                 \n" \
                "   isb                                             \n" \
                "   dsb                                             \n" \
                :"=r" (srOrg), "=r" (srNew) : "i" ( MAX_SYSCALL_IRQ_PRIORITY ) \
            ); \
            sr = srOrg; \
            rkh_critical_nesting++; \
        }
         

#define RKH_EXIT_CRITICAL( sr ) \
        { \
            rkh_critical_nesting--; \
            if (rkh_critical_nesting == 0) \
            {  \
                __asm volatile \
                ( \
                    "   msr basepri, %0 " :: "r" ( sr ) \
                ); \
            } \
        }

/* ------------------------------- Constants ------------------------------- */

#define KERNEL_IRQ_PRIO		6
#define HIGHEST_IRQ_PRI		5
        
/**
 *	If the #RKH_CFGPORT_SMA_THREAD_EN is set to 1, each SMA (active object) 
 * has its own thread of execution.
 */
#define RKH_CFGPORT_SMA_THREAD_EN 			RKH_DISABLED

/**
 *	If the #RKH_CFGPORT_SMA_THREAD_EN and #RKH_CFGPORT_SMA_THREAD_DATA_EN 
 *	are set to 1, each SMA (active object) has its own thread of execution 
 *	and its own object data.
 */
#define RKH_CFGPORT_SMA_THREAD_DATA_EN		RKH_DISABLED

/**
 * 	If the #RKH_CFGPORT_NATIVE_SCHEDULER_EN is set to 1 then RKH will 
 * 	include the simple, cooperative, and nonpreemptive scheduler RKHS.
 * 	When #RKH_CFGPORT_NATIVE_SCHEDULER_EN is enabled RKH also will 
 * 	automatically define #RKH_EQ_TYPE, RKH_SMA_BLOCK(), RKH_SMA_READY(), 
 * 	RKH_SMA_UNREADY(), and assume the native priority scheme.
 */
#define RKH_CFGPORT_NATIVE_SCHEDULER_EN		RKH_ENABLED

/**
 * 	If the #RKH_CFGPORT_NATIVE_EQUEUE_EN is set to 1 and the native event 
 * 	queue is enabled (see #RKH_CFG_RQ_EN) then RKH will include its own 
 * 	implementation of rkh_sma_post_fifo(), rkh_sma_post_lifo(), and 
 * 	rkh_sma_get() functions.
 */
#define RKH_CFGPORT_NATIVE_EQUEUE_EN		RKH_ENABLED

/**
 * 	If the #RKH_CFGPORT_NATIVE_DYN_EVT_EN is set to 1 and the native 
 * 	fixed-size memory block facility is enabled (see #RKH_CFG_MP_EN) then 
 * 	RKH will include its own implementation of dynamic memory management.
 * 	When #RKH_CFGPORT_NATIVE_DYN_EVT_EN is enabled RKH also will 
 * 	automatically define RKH_DYNE_TYPE, RKH_DYNE_INIT(), 
 * 	RKH_DYNE_GET_ESIZE(), RKH_DYNE_GET(), RKH_DYNE_PUT(), 
 * 	RKH_DYNE_GET_NUSED(), RKH_DYNE_GET_NMIN(), and RKH_DYNE_GET_PSIZE() macros.
 */
#define RKH_CFGPORT_NATIVE_DYN_EVT_EN		RKH_ENABLED

/**
 *	If the #RKH_CFGPORT_REENTRANT_EN is set to 1, the RKH event dispatch 
 *	allows to be invoked from several threads of executions. Enable this 
 *	only if the application is based on a multi-thread architecture.
 */
#define RKH_CFGPORT_REENTRANT_EN			RKH_DISABLED

/**
 * 	Specify the size of void pointer. The valid values [in bits] are 
 * 	16 or 32. Default is 32. See RKH_TRC_SYM() macro.
 */
#define RKH_CFGPORT_TRC_SIZEOF_PTR			32u

/**
 * 	Specify the size of function pointer. The valid values [in bits] are 
 * 	16 or 32. Default is 32. See RKH_TUSR_FUN() and RKH_TRC_FUN() macros.
 */
#define RKH_CFGPORT_TRC_SIZEOF_FUN_PTR		32u

/** 
 * 	Specify the number of bytes (size) used by the trace record timestamp. 
 * 	The valid values [in bits] are 8, 16 or 32. Default is 16.
 */
#define RKH_CFGPORT_TRC_SIZEOF_TSTAMP		32u

/** 
 * 	If the #RKH_CFGPORT_SMA_QSTO_EN is set to 1 then RKH_SMA_ACTIVATE() 
 * 	macro invokes the rkh_sma_activate() function ignoring the external 
 * 	event queue storage argument, \c qs.
 */
#define RKH_CFGPORT_SMA_QSTO_EN				RKH_ENABLED

/** 
 * 	If the #RKH_CFGPORT_SMA_STK_EN is set to 0 then RKH_SMA_ACTIVATE() 
 * 	macro invokes the rkh_sma_activate() function ignoring the thread's 
 * 	stack related arguments, \c stks and \c stksize.
 */
#define RKH_CFGPORT_SMA_STK_EN				RKH_DISABLED

/*
 * 	Declaring an object RKHROM announces that its value will
 * 	not be changed and it will be stored in ROM.
 */
#define RKHROM								const

/**
 * Native event queue data type
 */
#define RKH_EQ_TYPE              		    RKH_QUEUE_T

/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
extern ruint rkh_critical_nesting;

/* -------------------------- Function prototypes -------------------------- */
const char *rkhport_get_version(void);
const char *rkhport_get_desc(void);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif
/* ------------------------------ End of file ------------------------------ */
