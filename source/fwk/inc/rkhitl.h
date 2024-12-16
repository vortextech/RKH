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
 *  \file       rkhitl.h
 *  \brief      RKH engine interface.This header file is directly included in 
 *              RKH interface file, rkh.h.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2015.10.24  LeFr  v2.4.05  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __RKHITL_H__
#define __RKHITL_H__

/* ----------------------------- Include files ----------------------------- */
/**
 *  Specific definitions to the platform being used.
 */
#include "rkhplat.h"

/**
 *  Defines standard constants and macros.
 *  Application specific configuration options.
 *  Event data type and related macros.
 */
#include "rkhevt.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
#if (((RKH_CFG_TRC_EN == 1) && (RKH_CFG_SMA_TRC_SNDR_EN == 1)) || \
      (defined(RKH_EN_DOXYGEN) && RKH_EN_DOXYGEN == 1))
    /**
     *  \brief
     *  This macro is used to verify the sender object usage on post an 
     *  event.
     *
     *  The macros RKH_TIM_TICK(), RKH_SMA_POST_FIFO(), and
     *  RKH_SMA_POST_LIFO() takes an additional argument \a sender, which 
     *  is a pointer to the sender object. This argument is actually only 
     *  used when software tracing is enabled (macro #RKH_USE_TRC_SENDER is
     *  defined).
     *  When software tracing is disabled, the macros RKH_TIM_TICK(),
     *  RKH_SMA_POST_FIFO(), and RKH_SMA_POST_LIFO() calls rkh_tmr_tick(),
     *  rkh_sma_post_fifo(), and rkh_sma_post_lifo() respectively without 
     *  any arguments, so the overhead of passing this extra argument is 
     *  entirely avoided.
     */
    #define RKH_USE_TRC_SENDER
#endif

/**
 *  \brief
 *  This macro retrieves a pointer to string describing the RKH version.
 *  For example, "2.2.04".
 */
#define RKH_RELEASE                 rkh_version

/**
 *  \brief
 *  This is the macro used to build an integer version code from the
 *  individual numbers that build up a version number.
 *
 *  For example, RKH_VERSION(2,2,04) expands to 0x2204. This macro is very
 *  useful when needs to compare the current version (#RKH_VERSION_CODE) and
 *  a known checkpoint version at compile-time.
 *
 *  \param[in] a    major revision
 *  \param[in] b    minor revision
 *  \param[in] c    release number
 *
 *  \usage
 *  \code
 *  #if RKH_VERSION_CODE <= RKH_VERSION(1, 2, 03)
 *  ...
 *  #else
 *  ...
 *  #endif
 *  \endcode
 */
#define RKH_VERSION(a, b, c) \
    (((((a) <<   12) & 0xF000) | \
      (((b) <<    8) & 0x0F00) | \
      (((c) &  0xFF) & 0x00FF)) & 0xFFFF)

/**
 *  \brief
 *	This macro is used to indicate the end of a transition table.
 */
#define RKH_ANY                     ((RKH_SIG_T)(-1))

/**
 *  \brief
 *	This macro is used to indicate the completion event.
 *
 *  A special kind of transition is a completion transition, which has an 
 *  implicit trigger. The event that enables this trigger is called a 
 *  completion event and it signifies that all behaviors associated with the 
 *  source state of the completion transition have completed execution.
 */
#define RKH_COMPLETION_EVENT        (RKH_ANY - 1)

/**
 *  \brief
 *	This macro is used to indicate the State Machine creation event.
 *  
 *  Upon creation, a State Machine will perform its initialization during 
 *  which it executes an initial compound transition prompted by the 
 *  creation, after which it enters a wait point, which is represented by 
 *  a stable state configuration. It remains thus until an Event stored in 
 *  its event pool is dispatched.
 */
#define RKH_SM_CREATION_EVENT       (RKH_ANY - 2)

/**
 *  \brief
 *  RKH allows up to RKH_CFG_FWK_MAX_SMA different priority levels
 *  (see rkhcfg.h).
 *
 *  In RKH, a low-priority number corresponds to a high-priority level.
 *  Priority level zero (0) is thus the highest priority level. Priority
 *	RKH_LOWEST_PRIO (RKH_CFG_FWK_MAX_SMA - 1) is the lowest priority level.
 */
#define RKH_LOWEST_PRIO             (RKH_CFG_FWK_MAX_SMA - 1)

/*
 *  Verifies port file from rkhport.h included in rkhplat.h.
 */

#ifndef RKHROM
#error "rkhport.h, Missing RKHROM: Qualifier for ROM data storage. \
    See Porting chapter in readme file for more information"
#endif

#define FOREVER                     for (;; )

/*
 *  Macros for casting
 */
#define CB(p)           ((RKH_BASE_T *)(p))
#define CST(p)          ((RKH_ST_T *)(p))
#define CCMP(p)         ((RKH_SCMP_T *)(p))
#define CBSC(p)         ((RKH_SBSC_T *)(p))
#define CCD(p)          ((RKH_SCOND_T *)(p))
#define CCH(p)          ((RKH_SCHOICE_T *)(p))
#define CH(p)           ((RKH_SHIST_T *)(p))
#define CSBM(p)         ((RKH_SSBM_T *)(p))
#define CRSM(p)         ((RKH_RSM_T *)(p))
#define CM(p)           ((RKHROM RKH_SM_T *)(p))
#define CT(p)           ((RKHROM RKH_TR_T *)(p))
#define CPT(p)          ((RKHROM RKH_SCMP_T *)(p))
#define CPP(p)          ((RKH_PPRO_T)(p))
#define CG(p)           ((RKH_GUARD_T)(p))
#define CA(p)           ((RKH_TRN_ACT_T)(p))
#define CENP(p)         ((RKH_SENP_T *)(p))
#define CEXP(p)         ((RKH_SEXP_T *)(p))
#define CENPCN(p)       ((RKH_ENPCN_T *)(p))
#define CEXPCN(p)       ((RKH_EXPCN_T *)(p))
#define CE(p)           ((RKH_EVT_T *)(p))
#define CCE(p)          ((const RKH_EVT_T *)(p))
#define CV(p)           ((void *)(p))
#define CSMA(p)         ((const RKH_SMA_T *)(p))
#define CQ(p)           ((RKH_QUEUE_T *)(p))
#define CTA(ta)         ((RKH_TRN_ACT_T)(ta))

/**
 *  \brief
 *  This macro declares a typed and external pointer to previously defined 
 *  object.
 *
 *  \param[in] type_    Data type of the defined object. 
 *  \param[in] me_	    Pointer to previously defined object.
 *
 *  \sa
 *  RKH_SMA_DCLR_TYPE(), RKH_SM_DCLR_TYPE().
 */
#define RKH_DCLR_PTR_TYPE(type_, me_) \
        extern type_ * const me_

/* ------------------------- Configuration errors -------------------------- */

/*
 *  LOOK FOR MISSING #define CONSTANTS
 *
 *  This section is used to generate ERROR messages at compile time if
 *  certain #define constants are MISSING in rkhcfg.h.  This allows you to
 *  quickly determine the source of the error.
 *
 *  You SHOULD NOT change this section UNLESS you would like to add more
 *  comments as to the source of the compile time error.
 */

/*  MEMORY POOL   --------------------------------------------------------- */
#ifndef RKH_CFG_MP_EN
    #error "RKH_CFG_MP_EN                         not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_MP_EN != RKH_ENABLED) && \
    (RKH_CFG_MP_EN != RKH_DISABLED))
    #error "RKH_CFG_MP_EN                   illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#elif   (RKH_CFG_MP_EN == RKH_ENABLED)

    #ifndef RKH_CFG_MP_REDUCED_EN
    #error "RKH_CFG_MP_REDUCED_EN                 not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

    #elif   ((RKH_CFG_MP_REDUCED_EN != RKH_ENABLED) && \
    (RKH_CFG_MP_REDUCED_EN != RKH_DISABLED))
    #error "RKH_CFG_MP_REDUCED_EN           illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "
    #endif

    #ifndef RKH_CFG_MP_SIZEOF_BSIZE
    #error "RKH_CFG_MP_SIZEOF_BSIZE               not #define'd in 'rkhcfg.h'"
    #error  "                               [MUST be  8  ( 8-bit size)]      "
    #error  "                               [     || 16  (16-bit size)]      "
    #error  "                               [     || 32  (32-bit size)]      "

    #elif  ((RKH_CFG_MP_SIZEOF_BSIZE != 8) && \
    (RKH_CFG_MP_SIZEOF_BSIZE != 16) && \
    (RKH_CFG_MP_SIZEOF_BSIZE != 32))
    #error  "RKH_CFG_MP_SIZEOF_BSIZE        illegally #define'd in 'rkhcfg.h'"
    #error  "                               [MUST be  8  ( 8-bit size)]      "
    #error  "                               [     || 16  (16-bit size)]      "
    #error  "                               [     || 32  (32-bit size)]      "
    #endif

    #ifndef RKH_CFG_MP_SIZEOF_NBLOCK
    #error "RKH_CFG_MP_SIZEOF_NBLOCK              not #define'd in 'rkhcfg.h'"
    #error  "                               [MUST be  8  ( 8-bit size)]      "
    #error  "                               [     || 16  (16-bit size)]      "
    #error  "                               [     || 32  (32-bit size)]      "

    #elif  ((RKH_CFG_MP_SIZEOF_NBLOCK != 8) && \
    (RKH_CFG_MP_SIZEOF_NBLOCK != 16) && \
    (RKH_CFG_MP_SIZEOF_NBLOCK != 32))
    #error  "RKH_CFG_MP_SIZEOF_NBLOCK       illegally #define'd in 'rkhcfg.h'"
    #error  "                               [MUST be  8  ( 8-bit size)]      "
    #error  "                               [     || 16  (16-bit size)]      "
    #error  "                               [     || 32  (32-bit size)]      "
    #endif

    #ifndef RKH_CFG_MP_GET_BSIZE_EN
    #error "RKH_CFG_MP_GET_BSIZE_EN               not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

    #elif   ((RKH_CFG_MP_GET_BSIZE_EN != RKH_ENABLED) && \
    (RKH_CFG_MP_GET_BSIZE_EN != RKH_DISABLED))
    #error "RKH_CFG_MP_GET_BSIZE_EN         illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "
    #endif

    #ifndef RKH_CFG_MP_GET_NFREE_EN
    #error "RKH_CFG_MP_GET_NFREE_EN               not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

    #elif   ((RKH_CFG_MP_GET_NFREE_EN != RKH_ENABLED) && \
    (RKH_CFG_MP_GET_NFREE_EN != RKH_DISABLED))
    #error "RKH_CFG_MP_GET_NFREE_EN         illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "
    #endif

    #ifndef RKH_CFG_MP_GET_LWM_EN
    #error "RKH_CFG_MP_GET_LWM_EN                 not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

    #elif   ((RKH_CFG_MP_GET_LWM_EN != RKH_ENABLED) && \
    (RKH_CFG_MP_GET_LWM_EN != RKH_DISABLED))
    #error "RKH_CFG_MP_GET_LWM_EN           illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "
    #endif

    #ifndef RKH_CFG_MP_GET_INFO_EN
    #error "RKH_CFG_MP_GET_INFO_EN                not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

    #elif   ((RKH_CFG_MP_GET_INFO_EN != RKH_ENABLED) && \
    (RKH_CFG_MP_GET_INFO_EN != RKH_DISABLED))
    #error "RKH_CFG_MP_GET_INFO_EN          illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "
    #endif

#endif

/*  QUEUE         --------------------------------------------------------- */
#ifndef RKH_CFG_QUE_EN
    #error "RKH_CFG_QUE_EN                         not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_QUE_EN != RKH_ENABLED) && \
    (RKH_CFG_QUE_EN != RKH_DISABLED))
    #error "RKH_CFG_QUE_EN                   illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_QUE_SIZEOF_NELEM
    #error "RKH_CFG_QUE_SIZEOF_NELEM               not #define'd in 'rkhcfg.h'"
    #error  "                               [MUST be  8  ( 8-bit size)]      "
    #error  "                               [     || 16  (16-bit size)]      "
    #error  "                               [     || 32  (32-bit size)]      "

#elif  ((RKH_CFG_QUE_SIZEOF_NELEM != 8) && \
    (RKH_CFG_QUE_SIZEOF_NELEM != 16) && \
    (RKH_CFG_QUE_SIZEOF_NELEM != 32))
    #error  "RKH_CFG_QUE_SIZEOF_NELEM        illegally #define'd in 'rkhcfg.h'"
    #error  "                               [MUST be  8  ( 8-bit size)]      "
    #error  "                               [     || 16  (16-bit size)]      "
    #error  "                               [     || 32  (32-bit size)]      "

#endif

#ifndef RKH_CFG_QUE_GET_LWMARK_EN
    #error "RKH_CFG_QUE_GET_LWMARK_EN              not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_QUE_GET_LWMARK_EN != RKH_ENABLED) && \
    (RKH_CFG_QUE_GET_LWMARK_EN != RKH_DISABLED))
    #error "RKH_CFG_QUE_GET_LWMARK_EN        illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_QUE_READ_EN
    #error "RKH_CFG_QUE_READ_EN                    not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_QUE_READ_EN != RKH_ENABLED) && \
    (RKH_CFG_QUE_READ_EN != RKH_DISABLED))
    #error "RKH_CFG_QUE_READ_EN              illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_QUE_DEPLETE_EN
    #error "RKH_CFG_QUE_DEPLETE_EN                 not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_QUE_DEPLETE_EN != RKH_ENABLED) && \
    (RKH_CFG_QUE_DEPLETE_EN != RKH_DISABLED))
    #error "RKH_CFG_QUE_DEPLETE_EN           illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_QUE_IS_FULL_EN
    #error "RKH_CFG_QUE_IS_FULL_EN                 not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_QUE_IS_FULL_EN != RKH_ENABLED) && \
    (RKH_CFG_QUE_IS_FULL_EN != RKH_DISABLED))
    #error "RKH_CFG_QUE_IS_FULL_EN           illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_QUE_GET_NELEMS_EN
    #error "RKH_CFG_QUE_GET_NELEMS_EN              not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_QUE_GET_NELEMS_EN != RKH_ENABLED) && \
    (RKH_CFG_QUE_GET_NELEMS_EN != RKH_DISABLED))
    #error "RKH_CFG_QUE_GET_NELEMS_EN        illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_QUE_PUT_LIFO_EN
    #error "RKH_CFG_QUE_PUT_LIFO_EN                not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_QUE_PUT_LIFO_EN != RKH_ENABLED) && \
    (RKH_CFG_QUE_PUT_LIFO_EN != RKH_DISABLED))
    #error "RKH_CFG_QUE_PUT_LIFO_EN          illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_QUE_GET_INFO_EN
    #error "RKH_CFG_QUE_GET_INFO_EN                not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_QUE_GET_INFO_EN != RKH_ENABLED) && \
    (RKH_CFG_QUE_GET_INFO_EN != RKH_DISABLED))
    #error "RKH_CFG_QUE_GET_INFO_EN          illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

/*  TIMER         --------------------------------------------------------- */
#ifndef RKH_CFG_TMR_EN
    #error "RKH_CFG_TMR_EN                        not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_TMR_EN != RKH_ENABLED) && \
    (RKH_CFG_TMR_EN != RKH_DISABLED))
    #error "RKH_CFG_TMR_EN                  illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#elif   (RKH_CFG_TMR_EN == RKH_ENABLED)
    #ifndef RKH_CFG_FWK_TICK_RATE_HZ
    #error "RKH_CFG_FWK_TICK_RATE_HZ              not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "
    #elif (RKH_CFG_FWK_TICK_RATE_HZ <= 0u)
    #error "RKH_CFG_FWK_TICK_RATE_HZ        illegally #define'd in 'rkhcfg.h'"
    #error  "                               [MUST be > 0]                    "
    #endif

    #ifndef RKH_CFG_TMR_SIZEOF_NTIMER
    #error "RKH_CFG_TMR_SIZEOF_NTIMER             not #define'd in 'rkhcfg.h'"
    #error  "                               [MUST be  8  ( 8-bit size)]      "
    #error  "                               [     || 16  (16-bit size)]      "
    #error  "                               [     || 32  (32-bit size)]      "

    #elif  ((RKH_CFG_TMR_SIZEOF_NTIMER != 8) && \
    (RKH_CFG_TMR_SIZEOF_NTIMER != 16) && \
    (RKH_CFG_TMR_SIZEOF_NTIMER != 32))
    #error  "RKH_CFG_TMR_SIZEOF_NTIMER      illegally #define'd in 'rkhcfg.h'"
    #error  "                               [MUST be  8  ( 8-bit size)]      "
    #error  "                               [     || 16  (16-bit size)]      "
    #error  "                               [     || 32  (32-bit size)]      "
    #endif

    #ifndef RKH_CFG_TMR_HOOK_EN
    #error "RKH_CFG_TMR_HOOK_EN                   not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

    #elif   ((RKH_CFG_TMR_HOOK_EN != RKH_ENABLED) && \
    (RKH_CFG_TMR_HOOK_EN != RKH_DISABLED))
    #error "RKH_CFG_TMR_HOOK_EN             illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "
    #endif

    #ifndef RKH_CFG_TMR_GET_INFO_EN
    #error "RKH_CFG_TMR_GET_INFO_EN               not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

    #elif   ((RKH_CFG_TMR_GET_INFO_EN != RKH_ENABLED) && \
    (RKH_CFG_TMR_GET_INFO_EN != RKH_DISABLED))
    #error "RKH_CFG_TMR_GET_INFO_EN         illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "
    #endif

#endif

/*  STATE MACHINE APPLICATIONS  -------------------------------------------- */
#ifndef RKH_CFG_SMA_GET_INFO_EN
    #error "RKH_CFG_SMA_GET_INFO_EN               not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_SMA_GET_INFO_EN != RKH_ENABLED) && \
    (RKH_CFG_SMA_GET_INFO_EN != RKH_DISABLED))
    #error "RKH_CFG_SMA_GET_INFO_EN         illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_SMA_PPRO_EN
    #error "RKH_CFG_SMA_PPRO_EN                   not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_SMA_PPRO_EN != RKH_ENABLED) && \
    (RKH_CFG_SMA_PPRO_EN != RKH_DISABLED))
    #error "RKH_CFG_SMA_PPRO_EN             illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_SMA_HCAL_EN
    #error "RKH_CFG_SMA_HCAL_EN                   not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_SMA_HCAL_EN != RKH_ENABLED) && \
    (RKH_CFG_SMA_HCAL_EN != RKH_DISABLED))
    #error "RKH_CFG_SMA_HCAL_EN             illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_SMA_MAX_HCAL_DEPTH
    #error "RKH_CFG_SMA_MAX_HCAL_DEPTH            not #define'd in 'rkhcfg.h'"
    #error "                                [MUST be >  0]                   "
    #error "                                [     && <= 8]                   "

#elif   ((RKH_CFG_SMA_MAX_HCAL_DEPTH == 0) || \
    (RKH_CFG_SMA_MAX_HCAL_DEPTH > 8))
    #error "RKH_CFG_SMA_MAX_HCAL_DEPTH      illegally #define'd in 'rkhcfg.h'"
    #error  "                               [MUST be >= 0]                   "
    #error  "                               [     && <= 8]                   "

#endif

#ifndef RKH_CFG_SMA_MAX_TRC_SEGS
    #error "RKH_CFG_SMA_MAX_TRC_SEGS              not #define'd in 'rkhcfg.h'"
    #error "                                [MUST be >  0]                   "
    #error "                                [     && <= 8]                   "

#elif   ((RKH_CFG_SMA_MAX_TRC_SEGS == 0) || \
    (RKH_CFG_SMA_MAX_TRC_SEGS > 8))
    #error "RKH_CFG_SMA_MAX_TRC_SEGS        illegally #define'd in 'rkhcfg.h'"
    #error  "                               [MUST be >= 0]                   "
    #error  "                               [     && <= 8]                   "

#endif

#ifndef RKH_CFG_SMA_PSEUDOSTATE_EN
    #error "RKH_CFG_SMA_PSEUDOSTATE_EN            not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_SMA_PSEUDOSTATE_EN != RKH_ENABLED) && \
    (RKH_CFG_SMA_PSEUDOSTATE_EN != RKH_DISABLED))
    #error "RKH_CFG_SMA_PSEUDOSTATE_EN      illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_SMA_DEEP_HIST_EN
    #error "RKH_CFG_SMA_DEEP_HIST_EN              not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_SMA_DEEP_HIST_EN != RKH_ENABLED) && \
    (RKH_CFG_SMA_DEEP_HIST_EN != RKH_DISABLED))
    #error "RKH_CFG_SMA_DEEP_HIST_EN        illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_SMA_SHALLOW_HIST_EN
    #error "RKH_CFG_SMA_SHALLOW_HIST_EN           not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_SMA_SHALLOW_HIST_EN != RKH_ENABLED) && \
    (RKH_CFG_SMA_SHALLOW_HIST_EN != RKH_DISABLED))
    #error "RKH_CFG_SMA_SHALLOW_HIST_EN     illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_SMA_CHOICE_EN
    #error "RKH_CFG_SMA_CHOICE_EN                 not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_SMA_CHOICE_EN != RKH_ENABLED) && \
    (RKH_CFG_SMA_CHOICE_EN != RKH_DISABLED))
    #error "RKH_CFG_SMA_CHOICE_EN           illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]     "

#endif

#ifndef RKH_CFG_SMA_CONDITIONAL_EN
    #error "RKH_CFG_SMA_CONDITIONAL_EN            not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_SMA_CONDITIONAL_EN != RKH_ENABLED) && \
    (RKH_CFG_SMA_CONDITIONAL_EN != RKH_DISABLED))
    #error "RKH_CFG_SMA_CONDITIONAL_EN      illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_SMA_SUBMACHINE_EN
    #error "RKH_CFG_SMA_SUBMACHINE_EN             not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_SMA_SUBMACHINE_EN != RKH_ENABLED) && \
    (RKH_CFG_SMA_SUBMACHINE_EN != RKH_DISABLED))
    #error "RKH_CFG_SMA_SUBMACHINE_EN       illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_SMA_TRC_SNDR_EN
    #error "RKH_CFG_SMA_TRC_SNDR_EN               not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_SMA_TRC_SNDR_EN != RKH_ENABLED) && \
    (RKH_CFG_SMA_TRC_SNDR_EN != RKH_DISABLED))
    #error "RKH_CFG_SMA_TRC_SNDR_EN         illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_SMA_ENT_ARG_SMA_EN
    #error "RKH_CFG_SMA_ENT_ARG_SMA_EN            not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_SMA_ENT_ARG_SMA_EN != RKH_ENABLED) && \
    (RKH_CFG_SMA_ENT_ARG_SMA_EN != RKH_DISABLED))
    #error "RKH_CFG_SMA_ENT_ARG_SMA_EN      illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_SMA_ENT_ARG_STATE_EN
    #error "RKH_CFG_SMA_ENT_ARG_STATE_EN          not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_SMA_ENT_ARG_STATE_EN != RKH_ENABLED) && \
    (RKH_CFG_SMA_ENT_ARG_STATE_EN != RKH_DISABLED))
    #error "RKH_CFG_SMA_ENT_ARG_STATE_EN    illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_SMA_EXT_ARG_SMA_EN
    #error "RKH_CFG_SMA_EXT_ARG_SMA_EN            not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_SMA_EXT_ARG_SMA_EN != RKH_ENABLED) && \
    (RKH_CFG_SMA_EXT_ARG_SMA_EN != RKH_DISABLED))
    #error "RKH_CFG_SMA_EXT_ARG_SMA_EN      illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_SMA_EXT_ARG_STATE_EN
    #error "RKH_CFG_SMA_EXT_ARG_STATE_EN          not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_SMA_EXT_ARG_STATE_EN != RKH_ENABLED) && \
    (RKH_CFG_SMA_EXT_ARG_STATE_EN != RKH_DISABLED))
    #error "RKH_CFG_SMA_EXT_ARG_STATE_EN    illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_SMA_ACT_ARG_SMA_EN
    #error "RKH_CFG_SMA_ACT_ARG_SMA_EN            not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_SMA_ACT_ARG_SMA_EN != RKH_ENABLED) && \
    (RKH_CFG_SMA_ACT_ARG_SMA_EN != RKH_DISABLED))
    #error "RKH_CFG_SMA_ACT_ARG_SMA_EN      illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_SMA_ACT_ARG_EVT_EN
    #error "RKH_CFG_SMA_ACT_ARG_EVT_EN            not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_SMA_ACT_ARG_EVT_EN != RKH_ENABLED) && \
    (RKH_CFG_SMA_ACT_ARG_EVT_EN != RKH_DISABLED))
    #error "RKH_CFG_SMA_ACT_ARG_EVT_EN      illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_SMA_GRD_ARG_EVT_EN
    #error "RKH_CFG_SMA_GRD_ARG_EVT_EN            not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_SMA_GRD_ARG_EVT_EN != RKH_ENABLED) && \
    (RKH_CFG_SMA_GRD_ARG_EVT_EN != RKH_DISABLED))
    #error "RKH_CFG_SMA_GRD_ARG_EVT_EN      illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_SMA_GRD_ARG_SMA_EN
    #error "RKH_CFG_SMA_GRD_ARG_SMA_EN            not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_SMA_GRD_ARG_SMA_EN != RKH_ENABLED) && \
    (RKH_CFG_SMA_GRD_ARG_SMA_EN != RKH_DISABLED))
    #error "RKH_CFG_SMA_GRD_ARG_SMA_EN      illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_SMA_PPRO_ARG_SMA_EN
    #error "RKH_CFG_SMA_PPRO_ARG_SMA_EN           not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_SMA_PPRO_ARG_SMA_EN != RKH_ENABLED) && \
    (RKH_CFG_SMA_PPRO_ARG_SMA_EN != RKH_DISABLED))
    #error "RKH_CFG_SMA_PPRO_ARG_SMA_EN     illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_SMA_SM_CONST_EN
    #error "RKH_CFG_SMA_SM_CONST_EN               not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif ((RKH_CFG_SMA_SM_CONST_EN != RKH_ENABLED) && \
    (RKH_CFG_SMA_SM_CONST_EN != RKH_DISABLED))
    #error "RKH_CFG_SMA_SM_CONST_EN         illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_SMA_RT_CTOR_EN
    #error "RKH_CFG_SMA_RT_CTOR_EN                not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif ((RKH_CFG_SMA_RT_CTOR_EN != RKH_ENABLED) && \
    (RKH_CFG_SMA_RT_CTOR_EN != RKH_DISABLED))
    #error "RKH_CFG_SMA_RT_CTOR_EN          illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_SMA_VFUNCT_EN
    #error "RKH_CFG_SMA_VFUNCT_EN                 not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_SMA_VFUNCT_EN != RKH_ENABLED) && \
    (RKH_CFG_SMA_VFUNCT_EN != RKH_DISABLED))
    #error "RKH_CFG_SMA_VFUNCT_EN           illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

#ifndef RKH_CFG_SMA_ORTHREG_EN
    #error "RKH_CFG_SMA_ORTHREG_EN                not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_SMA_ORTHREG_EN != RKH_ENABLED) && \
    (RKH_CFG_SMA_ORTHREG_EN != RKH_DISABLED))
    #error "RKH_CFG_SMA_ORTHREG_EN          illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#endif

/*  TRACE         --------------------------------------------------------- */
#ifndef RKH_CFG_TRC_EN
    #error "RKH_CFG_TRC_EN                        not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

#elif   ((RKH_CFG_TRC_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_EN                  illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "

#elif   (RKH_CFG_TRC_EN == RKH_ENABLED)

    #ifndef RKH_CFG_TRC_RTFIL_EN
    #error "RKH_CFG_TRC_RTFIL_EN                  not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

    #elif   ((RKH_CFG_TRC_RTFIL_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_RTFIL_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_RTFIL_EN            illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "
    #endif

    #ifndef RKH_CFG_TRC_RTFIL_SMA_EN
    #error "RKH_CFG_TRC_RTFIL_SMA_EN              not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

    #elif   ((RKH_CFG_TRC_RTFIL_SMA_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_RTFIL_SMA_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_RTFIL_SMA_EN        illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "
    #endif

    #ifndef RKH_CFG_TRC_RTFIL_SIGNAL_EN
    #error "RKH_CFG_TRC_RTFIL_SIGNAL_EN           not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]       "
    #error "                                    [     || RKH_DISABLED]       "

    #elif   ((RKH_CFG_TRC_RTFIL_SIGNAL_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_RTFIL_SIGNAL_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_RTFIL_SIGNAL_EN     illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]      "
    #error "                                    [     ||  RKH_DISABLED]      "
    #endif

    #ifndef RKH_CFG_TRC_USER_TRACE_EN
    #error "RKH_CFG_TRC_USER_TRACE_EN             not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

    #elif   ((RKH_CFG_TRC_USER_TRACE_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_USER_TRACE_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_USER_TRACE_EN       illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "
    #endif

    #ifndef RKH_CFG_TRC_ALL_EN
    #error "RKH_CFG_TRC_ALL_EN                    not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

    #elif   ((RKH_CFG_TRC_ALL_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_ALL_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_ALL_EN              illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "
    #endif

    #ifndef RKH_CFG_TRC_MP_EN
    #error "RKH_CFG_TRC_MP_EN                     not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

    #elif   ((RKH_CFG_TRC_MP_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_MP_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_MP_EN               illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "
    #endif

    #ifndef RKH_CFG_TRC_QUE_EN
    #error "RKH_CFG_TRC_QUE_EN                     not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

    #elif   ((RKH_CFG_TRC_QUE_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_QUE_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_QUE_EN               illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "
    #endif

    #ifndef RKH_CFG_TRC_SMA_EN
    #error "RKH_CFG_TRC_SMA_EN                    not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

    #elif   ((RKH_CFG_TRC_SMA_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_SMA_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_SMA_EN              illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "
    #endif

    #ifndef RKH_CFG_TRC_TMR_EN
    #error "RKH_CFG_TRC_TMR_EN                    not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

    #elif   ((RKH_CFG_TRC_TMR_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_TMR_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_TMR_EN              illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "
    #endif

    #ifndef RKH_CFG_TRC_SM_EN
    #error "RKH_CFG_TRC_SM_EN                     not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

    #elif   ((RKH_CFG_TRC_SM_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_SM_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_SM_EN               illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "
    #endif

    #ifndef RKH_CFG_TRC_FWK_EN
    #error "RKH_CFG_TRC_FWK_EN                    not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

    #elif   ((RKH_CFG_TRC_FWK_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_FWK_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_FWK_EN              illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "
    #endif

    #ifndef RKH_CFG_TRC_ASSERT_EN
    #error "RKH_CFG_TRC_ASSERT_EN                 not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

    #elif   ((RKH_CFG_TRC_ASSERT_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_ASSERT_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_ASSERT_EN           illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "
    #endif

    #ifndef RKH_CFG_TRC_SM_INIT_EN
    #error "RKH_CFG_TRC_SM_INIT_EN                not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

    #elif   ((RKH_CFG_TRC_SM_INIT_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_SM_INIT_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_SM_INIT_EN          illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "
    #endif

    #ifndef RKH_CFG_TRC_SM_DCH_EN
    #error "RKH_CFG_TRC_SM_DCH_EN                  not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

    #elif   ((RKH_CFG_TRC_SM_DCH_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_SM_DCH_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_SM_DCH_EN            illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "
    #endif

    #ifndef RKH_CFG_TRC_SM_CLRH_EN
    #error "RKH_CFG_TRC_SM_CLRH_EN                not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

    #elif   ((RKH_CFG_TRC_SM_CLRH_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_SM_CLRH_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_SM_CLRH_EN          illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "
    #endif

    #ifndef RKH_CFG_TRC_SM_TRN_EN
    #error "RKH_CFG_TRC_SM_TRN_EN                 not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

    #elif   ((RKH_CFG_TRC_SM_TRN_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_SM_TRN_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_SM_TRN_EN           illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "
    #endif

    #ifndef RKH_CFG_TRC_SM_STATE_EN
    #error "RKH_CFG_TRC_SM_STATE_EN               not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

    #elif   ((RKH_CFG_TRC_SM_STATE_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_SM_STATE_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_SM_STATE_EN         illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "
    #endif

    #ifndef RKH_CFG_TRC_SM_ENSTATE_EN
    #error "RKH_CFG_TRC_SM_ENSTATE_EN             not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

    #elif   ((RKH_CFG_TRC_SM_ENSTATE_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_SM_ENSTATE_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_SM_ENSTATE_EN       illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "
    #endif

    #ifndef RKH_CFG_TRC_SM_EXSTATE_EN
    #error "RKH_CFG_TRC_SM_EXSTATE_EN             not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

    #elif   ((RKH_CFG_TRC_SM_EXSTATE_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_SM_EXSTATE_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_SM_EXSTATE_EN       illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "
    #endif

    #ifndef RKH_CFG_TRC_SM_NENEX_EN
    #error "RKH_CFG_TRC_SM_NENEX_EN               not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

    #elif   ((RKH_CFG_TRC_SM_NENEX_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_SM_NENEX_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_SM_NENEX_EN         illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "
    #endif

    #ifndef RKH_CFG_TRC_SM_NTRNACT_EN
    #error "RKH_CFG_TRC_SM_NTRNACT_EN             not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

    #elif   ((RKH_CFG_TRC_SM_NTRNACT_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_SM_NTRNACT_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_SM_NTRNACT_EN       illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "
    #endif

    #ifndef RKH_CFG_TRC_SM_TS_STATE_EN
    #error "RKH_CFG_TRC_SM_TS_STATE_EN            not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

    #elif   ((RKH_CFG_TRC_SM_TS_STATE_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_SM_TS_STATE_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_SM_TS_STATE_EN      illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "
    #endif

    #ifndef RKH_CFG_TRC_SM_PROCESS_EN
    #error "RKH_CFG_TRC_SM_PROCESS_EN             not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

    #elif   ((RKH_CFG_TRC_SM_PROCESS_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_SM_PROCESS_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_SM_PROCESS_EN       illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "
    #endif

    #ifndef RKH_CFG_TRC_SM_EXE_ACT_EN
    #error "RKH_CFG_TRC_SM_EXE_ACT_EN              not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

    #elif   ((RKH_CFG_TRC_SM_EXE_ACT_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_SM_EXE_ACT_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_SM_EXE_ACT_EN        illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "
    #endif

    #ifndef RKH_CFG_TRC_NSEQ_EN
    #error "RKH_CFG_TRC_NSEQ_EN                   not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

    #elif   ((RKH_CFG_TRC_NSEQ_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_NSEQ_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_NSEQ_EN             illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "
    #endif

    #ifndef RKH_CFG_TRC_CHK_EN
    #error "RKH_CFG_TRC_CHK_EN                    not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

    #elif   ((RKH_CFG_TRC_CHK_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_CHK_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_CHK_EN              illegally #define'd in 'rkhcfg.h'"
    #error "                                     MUST be  RKH_ENABLED ]       "
    #error "                                          ||  RKH_DISABLED]       "
    #endif

    #ifndef RKH_CFG_TRC_TSTAMP_EN
    #error "RKH_CFG_TRC_TSTAMP_EN                 not #define'd in 'rkhcfg.h'"
    #error "                                     MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

    #elif   ((RKH_CFG_TRC_TSTAMP_EN != RKH_ENABLED) && \
    (RKH_CFG_TRC_TSTAMP_EN != RKH_DISABLED))
    #error "RKH_CFG_TRC_TSTAMP_EN           illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "
    #endif

    #ifndef RKH_CFGPORT_TRC_SIZEOF_TSTAMP
    #error "RKH_CFGPORT_TRC_SIZEOF_TSTAMP         not #define'd in 'rkhcfg.h'"
    #error  "                               [MUST be  8  ( 8-bit size)]       "
    #error  "                               [     || 16  (16-bit size)]       "
    #error  "                               [     || 32  (32-bit size)]       "

    #elif  ((RKH_CFGPORT_TRC_SIZEOF_TSTAMP != 8) && \
    (RKH_CFGPORT_TRC_SIZEOF_TSTAMP != 16) && \
    (RKH_CFGPORT_TRC_SIZEOF_TSTAMP != 32))
    #error  "RKH_CFGPORT_TRC_SIZEOF_TSTAMP  illegally #define'd in 'rkhcfg.h'"
    #error  "                               [MUST be  8  ( 8-bit size)]       "
    #error  "                               [     || 16  (16-bit size)]       "
    #error  "                               [     || 32  (32-bit size)]       "
    #endif

    #ifndef RKH_CFGPORT_SMA_QSTO_EN
    #error "RKH_CFGPORT_SMA_QSTO_EN                not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

    #elif   ((RKH_CFGPORT_SMA_QSTO_EN != RKH_ENABLED) && \
    (RKH_CFGPORT_SMA_QSTO_EN != RKH_DISABLED))
    #error "RKH_CFGPORT_SMA_QSTO_EN          illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "
    #endif

    #ifndef RKH_CFGPORT_SMA_STK_EN
    #error "RKH_CFGPORT_SMA_STK_EN                 not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

    #elif   ((RKH_CFGPORT_SMA_STK_EN != RKH_ENABLED) && \
    (RKH_CFGPORT_SMA_STK_EN != RKH_DISABLED))
    #error "RKH_CFGPORT_SMA_STK_EN           illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "
    #endif

    #ifndef RKH_CFG_TRC_SIZEOF_STREAM
    #error "RKH_CFG_TRC_SIZEOF_STREAM             not #define'd in 'rkhcfg.h'"
    #error  "                               [MUST be >     0]                 "
    #error  "                               [     && <= 2048]                 "

    #elif   ((RKH_CFG_TRC_SIZEOF_STREAM == 0) || \
    (RKH_CFG_TRC_SIZEOF_STREAM > 2048))
    #error "RKH_CFG_TRC_SIZEOF_STREAM       illegally #define'd in 'rkhcfg.h'"
    #error  "                               [MUST be >    0]                  "
    #error  "                               [     && <= 2048]                 "
    #endif

#endif

/*  FRAMEWORK     --------------------------------------------------------- */
#ifndef RKH_CFG_FWK_MAX_SMA
    #error "RKH_CFG_FWK_MAX_SMA                   not #define'd in 'rkhcfg.h'"
    #error  "                               [MUST be >=  1]                   "
    #error  "                               [     && <= 64]                   "

#elif ((RKH_CFG_FWK_MAX_SMA == 0) || (RKH_CFG_FWK_MAX_SMA > 64))
    #error "RKH_CFG_FWK_MAX_SMA             illegally #define'd in 'rkhcfg.h'"
    #error  "                               [MUST be >=  1]                   "
    #error  "                               [     && <= 64]                   "

#endif

#ifndef RKH_CFG_FWK_DYN_EVT_EN
    #error "RKH_CFG_FWK_DYN_EVT_EN                not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

#elif   (RKH_CFG_FWK_DYN_EVT_EN == RKH_DISABLED)

    /*
     *	If the dynamic event support is disabled, RKH not allows to use
     *	events with parameters, defer/recall mechanism, allocating and
     *	recycling dynamic events, among other features.
     */
    #undef RKH_CFGPORT_NATIVE_DYN_EVT_EN
    #define RKH_CFGPORT_NATIVE_DYN_EVT_EN           RKH_DISABLED

#endif

#ifndef RKH_CFG_FWK_MAX_EVT_POOL
    #error "RKH_CFG_FWK_MAX_EVT_POOL              not #define'd in 'rkhcfg.h'"
    #error  "                               [MUST be >    0]                  "
    #error  "                               [     && <  256]                  "

#elif   (RKH_CFG_FWK_DYN_EVT_EN == RKH_ENABLED)
    #if ((RKH_CFG_FWK_MAX_EVT_POOL == 0) || (RKH_CFG_FWK_MAX_EVT_POOL > 255))
    #error "RKH_CFG_FWK_MAX_EVT_POOL        illegally #define'd in 'rkhcfg.h'"
    #error  "                               [MUST be >    0]                  "
    #error  "                               [     && <  256]                  "
    #endif

#endif

#ifndef RKH_CFG_FWK_SIZEOF_EVT
    #error "RKH_CFG_FWK_SIZEOF_EVT                not #define'd in 'rkhcfg.h'"
    #error  "                               [MUST be  8  ( 8-bit size)]       "
    #error  "                               [     || 16  (16-bit size)]       "
    #error  "                               [     || 32  (32-bit size)]       "

#elif  ((RKH_CFG_FWK_SIZEOF_EVT != 8) && \
    (RKH_CFG_FWK_SIZEOF_EVT != 16) && \
    (RKH_CFG_FWK_SIZEOF_EVT != 32))
    #error  "RKH_CFG_FWK_SIZEOF_EVT         illegally #define'd in 'rkhcfg.h'"
    #error  "                               [MUST be  8  ( 8-bit size)]       "
    #error  "                               [     || 16  (16-bit size)]       "
    #error  "                               [     || 32  (32-bit size)]       "

#endif

#ifndef RKH_CFG_FWK_MAX_SIGNALS
    #error "RKH_CFG_FWK_MAX_SIGNALS               not #define'd in 'rkhcfg.h'"
    #error  "                               [MUST be >  0]                    "
    #error  "                        [MUST be <  2^RKH_CFG_FWK_SIZEOF_EVT    "

#elif ((RKH_CFG_FWK_MAX_SIGNALS == 0u) || \
    (RKH_CFG_FWK_MAX_SIGNALS > (1u << RKH_CFG_FWK_SIZEOF_EVT)))
    #error "RKH_CFG_FWK_MAX_SIGNALS         illegally #define'd in 'rkhcfg.h'"
    #error  "                               [MUST be >  0]                    "
    #error  "                        [MUST be <  2^RKH_CFG_FWK_SIZEOF_EVT    "

#endif

#ifndef RKH_CFG_FWK_SIZEOF_EVT_SIZE
    #error "RKH_CFG_FWK_SIZEOF_EVT_SIZE           not #define'd in 'rkhcfg.h'"
    #error  "                               [MUST be  8  ( 8-bit size)]       "
    #error  "                               [     || 16  (16-bit size)]       "
    #error  "                               [     || 32  (32-bit size)]       "

#elif  ((RKH_CFG_FWK_SIZEOF_EVT_SIZE != 8) && \
    (RKH_CFG_FWK_SIZEOF_EVT_SIZE != 16) && \
    (RKH_CFG_FWK_SIZEOF_EVT_SIZE != 32))
    #error  "RKH_CFG_FWK_SIZEOF_EVT_SIZE    illegally #define'd in 'rkhcfg.h'"
    #error  "                               [MUST be  8  ( 8-bit size)]       "
    #error  "                               [     || 16  (16-bit size)]       "
    #error  "                               [     || 32  (32-bit size)]       "

#endif

#ifndef RKH_CFG_FWK_DEFER_EVT_EN
    #error "RKH_CFG_FWK_DEFER_EVT_EN              not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

#elif   ((RKH_CFG_FWK_DEFER_EVT_EN == RKH_ENABLED) && \
    (RKH_CFG_QUE_EN == RKH_DISABLED))
    #error "RKH_CFG_QUE_EN                    illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "

#endif

#ifndef RKH_CFG_FWK_ASSERT_EN
    #error "RKH_CFG_FWK_ASSERT_EN                not #define'd in 'rkhport.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

#elif   ((RKH_CFG_FWK_ASSERT_EN != RKH_ENABLED) && \
    (RKH_CFG_FWK_ASSERT_EN != RKH_DISABLED))
    #error "RKH_CFG_FWK_ASSERT_EN          illegally #define'd in 'rkhport.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "

#endif

#ifndef RKH_CFG_HOOK_DISPATCH_EN
    #error "RKH_CFG_HOOK_DISPATCH_EN              not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

#elif   ((RKH_CFG_HOOK_DISPATCH_EN != RKH_ENABLED) && \
    (RKH_CFG_HOOK_DISPATCH_EN != RKH_DISABLED))
    #error "RKH_CFG_HOOK_DISPATCH_EN        illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "

#endif

#ifndef RKH_CFG_HOOK_SIGNAL_EN
    #error "RKH_CFG_HOOK_SIGNAL_EN                not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

#elif   ((RKH_CFG_HOOK_SIGNAL_EN != RKH_ENABLED) && \
    (RKH_CFG_HOOK_SIGNAL_EN != RKH_DISABLED))
    #error "RKH_CFG_HOOK_SIGNAL_EN          illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "

#endif

#ifndef RKH_CFG_HOOK_TIMEOUT_EN
    #error "RKH_CFG_HOOK_TIMEOUT_EN               not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

#elif   ((RKH_CFG_HOOK_TIMEOUT_EN != RKH_ENABLED) && \
    (RKH_CFG_HOOK_TIMEOUT_EN != RKH_DISABLED))
    #error "RKH_CFG_HOOK_TIMEOUT_EN         illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "

#endif

#ifndef RKH_CFG_HOOK_START_EN
    #error "RKH_CFG_HOOK_START_EN                 not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

#elif   ((RKH_CFG_HOOK_START_EN != RKH_ENABLED) && \
    (RKH_CFG_HOOK_START_EN != RKH_DISABLED))
    #error "RKH_CFG_HOOK_START_EN           illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "

#endif

#ifndef RKH_CFG_HOOK_EXIT_EN
    #error "RKH_CFG_HOOK_EXIT_EN                  not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

#elif   ((RKH_CFG_HOOK_EXIT_EN != RKH_ENABLED) && \
    (RKH_CFG_HOOK_EXIT_EN != RKH_DISABLED))
    #error "RKH_CFG_HOOK_EXIT_EN            illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "

#endif

#ifndef RKH_CFG_HOOK_TIMETICK_EN
    #error "RKH_CFG_HOOK_TIMETICK_EN              not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

#elif   ((RKH_CFG_HOOK_TIMETICK_EN != RKH_ENABLED) && \
    (RKH_CFG_HOOK_TIMETICK_EN != RKH_DISABLED))
    #error "RKH_CFG_HOOK_TIMETICK_EN        illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "

#endif

#ifndef RKH_CFG_HOOK_PUT_TRCEVT_EN
    #error "RKH_CFG_HOOK_PUT_TRCEVT_EN             not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

#elif   ((RKH_CFG_HOOK_PUT_TRCEVT_EN != RKH_ENABLED) && \
    (RKH_CFG_HOOK_PUT_TRCEVT_EN != RKH_DISABLED))
    #error "RKH_CFG_HOOK_PUT_TRCEVT_EN       illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "

#endif

#ifndef RKH_CFG_SMA_INIT_EVT_EN
    #error "RKH_CFG_SMA_INIT_EVT_EN               not #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

#elif   ((RKH_CFG_SMA_INIT_EVT_EN != RKH_ENABLED) && \
    (RKH_CFG_SMA_INIT_EVT_EN != RKH_DISABLED))
    #error "RKH_CFG_SMA_INIT_EVT_EN         illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "

#endif

#ifdef RKH_CFG_FWK_AWARE_ISR_PRIO
#if ((RKH_CFG_FWK_AWARE_ISR_PRIO < 0) || \
     (RKH_CFG_FWK_AWARE_ISR_PRIO > 255))
    #error "RKH_CFG_FWK_AWARE_ISR_PRIO       illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be >= 0]                "
    #error "                                    [     && <= 255]              "
#endif
#endif

/*  PORT          --------------------------------------------------------- */
#ifndef RKH_CFGPORT_SMA_THREAD_EN
    #error "RKH_CFGPORT_SMA_THREAD_EN            not #define'd in 'rkhport.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

#elif   ((RKH_CFGPORT_SMA_THREAD_EN != RKH_ENABLED) && \
    (RKH_CFGPORT_SMA_THREAD_EN != RKH_DISABLED))
    #error "RKH_CFGPORT_SMA_THREAD_EN      illegally #define'd in 'rkhport.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "

#endif

#ifndef RKH_CFGPORT_SMA_THREAD_DATA_EN
    #error "RKH_CFGPORT_SMA_THREAD_DATA_EN       not #define'd in 'rkhport.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

#elif   ((RKH_CFGPORT_SMA_THREAD_DATA_EN != RKH_ENABLED) && \
    (RKH_CFGPORT_SMA_THREAD_DATA_EN != RKH_DISABLED))
    #error "RKH_CFGPORT_SMA_THREAD_DATA_EN illegally #define'd in 'rkhport.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "

#endif

#ifndef RKH_CFGPORT_NATIVE_SCHEDULER_EN
    #error "RKH_CFGPORT_NATIVE_SCHEDULER_EN      not #define'd in 'rkhport.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

#elif   ((RKH_CFGPORT_NATIVE_SCHEDULER_EN != RKH_ENABLED) && \
    (RKH_CFGPORT_NATIVE_SCHEDULER_EN != RKH_DISABLED))
    #error "RKH_CFGPORT_NATIVE_SCHEDULER_EN illegally #define'd in 'rkhport.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "

#endif

#ifndef RKH_CFGPORT_NATIVE_EQUEUE_EN
    #error "RKH_CFGPORT_NATIVE_EQUEUE_EN         not #define'd in 'rkhport.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

#elif   ((RKH_CFGPORT_NATIVE_EQUEUE_EN != RKH_ENABLED) && \
    (RKH_CFGPORT_NATIVE_EQUEUE_EN != RKH_DISABLED))
    #error "RKH_CFGPORT_NATIVE_EQUEUE_EN   illegally #define'd in 'rkhport.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "

#elif   ((RKH_CFGPORT_NATIVE_EQUEUE_EN == RKH_ENABLED) && \
    (RKH_CFG_QUE_EN != RKH_ENABLED))
    #error "RKH_CFG_QUE_EN                   illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

#endif

#ifndef RKH_CFGPORT_NATIVE_DYN_EVT_EN
    #error "RKH_CFGPORT_NATIVE_DYN_EVT_EN        not #define'd in 'rkhport.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

#elif   ((RKH_CFGPORT_NATIVE_DYN_EVT_EN != RKH_ENABLED) && \
    (RKH_CFGPORT_NATIVE_DYN_EVT_EN != RKH_DISABLED))
    #error "RKH_CFGPORT_NATIVE_DYN_EVT_EN  illegally #define'd in 'rkhport.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "

#elif   (RKH_CFGPORT_NATIVE_DYN_EVT_EN == RKH_ENABLED)
    #if ((RKH_CFG_FWK_DYN_EVT_EN == RKH_ENABLED) && \
    (RKH_CFG_MP_EN == RKH_DISABLED))
    #error "RKH_CFG_FWK_DYN_EVT_EN          illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "RKH_CFG_MP_EN                   illegally #define'd in 'rkhcfg.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #endif

#endif

#ifndef RKH_CFGPORT_REENTRANT_EN
    #error "RKH_CFGPORT_REENTRANT_EN             not #define'd in 'rkhport.h'"
    #error "                                    [MUST be RKH_ENABLED ]        "
    #error "                                    [     || RKH_DISABLED]        "

#elif   ((RKH_CFGPORT_REENTRANT_EN != RKH_ENABLED) && \
    (RKH_CFGPORT_REENTRANT_EN != RKH_DISABLED))
    #error "RKH_CFGPORT_REENTRANT_EN       illegally #define'd in 'rkhport.h'"
    #error "                                    [MUST be  RKH_ENABLED ]       "
    #error "                                    [     ||  RKH_DISABLED]       "

#endif

#ifndef RKH_CFGPORT_TRC_SIZEOF_PTR
    #error "RKH_CFGPORT_TRC_SIZEOF_PTR       not #define'd in 'rkhport.h'"
    #error  "                               [MUST be  8  ( 8-bit pointer)]    "
    #error  "                               [     || 16  (16-bit pointer)]    "
    #error  "                               [     || 32  (32-bit pointer)]    "

#elif  ((RKH_CFGPORT_TRC_SIZEOF_PTR != 8) && \
    (RKH_CFGPORT_TRC_SIZEOF_PTR != 16) && \
    (RKH_CFGPORT_TRC_SIZEOF_PTR != 32))
    #error  "RKH_CFGPORT_TRC_SIZEOF_PTR illegally #define'd in 'rkhport.h'"
    #error  "                               [MUST be  8  ( 8-bit pointer)]    "
    #error  "                               [     || 16  (16-bit pointer)]    "
    #error  "                               [     || 32  (32-bit pointer)]    "

#endif

#ifndef RKH_CFGPORT_TRC_SIZEOF_FUN_PTR
    #error "RKH_CFGPORT_TRC_SIZEOF_FUN_PTR       not #define'd in 'rkhport.h'"
    #error  "                               [MUST be  8  ( 8-bit pointer)]    "
    #error  "                               [     || 16  (16-bit pointer)]    "
    #error  "                               [     || 32  (32-bit pointer)]    "

#elif  ((RKH_CFGPORT_TRC_SIZEOF_FUN_PTR != 8) && \
    (RKH_CFGPORT_TRC_SIZEOF_FUN_PTR != 16) && \
    (RKH_CFGPORT_TRC_SIZEOF_FUN_PTR != 32))
    #error  "RKH_CFGPORT_TRC_SIZEOF_FUN_PTR illegally #define'd in 'rkhport.h'"
    #error  "                               [MUST be  8  ( 8-bit pointer)]    "
    #error  "                               [     || 16  (16-bit pointer)]    "
    #error  "                               [     || 32  (32-bit pointer)]    "

#endif

/**
 *  It can be used to convert ticks to milliseconds.
 *  This format is more convenient and natural than ticks.
 *  This configuration constant is not used by RKH, it is just
 *  a value to allow an application to deal with time when
 *  using timer services.
 *  You can use the global constant RKH_CFG_FWK_TICK_RATE_HZ (see
 *  rkhcfg.h) to convert time to ticks using the macros like
 *  RKH_TIME_MS(), RKH_TIME_SEC(), and RKH_TIME_MIN().
 */
#define RKH_TICK_RATE_MS    \
    ((RKH_TNT_T)(1000 / RKH_CFG_FWK_TICK_RATE_HZ))

/** @{
 *  \brief
 *  It can be used to convert ticks to time.
 *
 *  This format is more convenient and natural than ticks. You can use
 *  the global constant RKH_CFG_FWK_TICK_RATE_HZ (see rkhcfg.h) to convert
 *  time to ticks using the macros like RKH_TIME_MS(), RKH_TIME_SEC(),
 *  and RKH_TIME_MIN().
 */
#define RKH_TIME_MS(ms_)      ((ms_) / RKH_TICK_RATE_MS)
#define RKH_TIME_SEC(s_)      ((s_) * 1000u / RKH_TICK_RATE_MS)
#define RKH_TIME_MIN(m_)      ((m_) * 60 * 1000u / RKH_TICK_RATE_MS)
/**@}*/

/*
 *  The following macros and constants are INTERNAL to RKH and
 *  the user application should NOT call it.
 */

#define RKH_REGULAR                     0x80
#define RKH_PSEUDO                      0
#define RKH_TYPE(t,i)                   (t | i)

#define RKH_UNKNOWN_VERTEX              RKH_TYPE(0,            0)
#define RKH_BASIC                       RKH_TYPE(RKH_REGULAR,  0)
#define RKH_COMPOSITE                   RKH_TYPE(RKH_REGULAR,  0x01)
#define RKH_SUBMACHINE                  RKH_TYPE(RKH_REGULAR,  0x02)
#define RKH_REF_SUBMACHINE              RKH_TYPE(RKH_REGULAR,  0x04)
#define RKH_FINAL                       RKH_TYPE(RKH_REGULAR,  0x08)

#define RKH_CONDITIONAL                 RKH_TYPE(RKH_PSEUDO,   0x02)
#define RKH_CHOICE                      RKH_TYPE(RKH_PSEUDO,   0x04)
#define RKH_SHISTORY                    RKH_TYPE(RKH_PSEUDO,   0x08)
#define RKH_DHISTORY                    RKH_TYPE(RKH_PSEUDO,   0x10)
#define RKH_ENPOINT                     RKH_TYPE(RKH_PSEUDO,   0x20)
#define RKH_EXPOINT                     RKH_TYPE(RKH_PSEUDO,   0x40)

#define RKH_NO_HISTORY                  RKH_UNKNOWN_VERTEX

#if (RKH_CFG_SMA_PSEUDOSTATE_EN  == RKH_DISABLED || \
     (RKH_CFG_SMA_DEEP_HIST_EN    == RKH_DISABLED && \
      RKH_CFG_SMA_SHALLOW_HIST_EN == RKH_DISABLED && \
      RKH_CFG_SMA_SUBMACHINE_EN   == RKH_DISABLED && \
      RKH_CFG_SMA_CONDITIONAL_EN  == RKH_DISABLED && \
      RKH_CFG_SMA_CHOICE_EN       == RKH_DISABLED))
    #define RKH_PSEUDOSTATE     RKH_DISABLED
#else
    #define RKH_PSEUDOSTATE     RKH_ENABLED
#endif

#if (RKH_CFG_SMA_HCAL_EN         == RKH_ENABLED &&   \
     RKH_CFG_SMA_PSEUDOSTATE_EN  == RKH_ENABLED &&   \
     (RKH_CFG_SMA_SHALLOW_HIST_EN == RKH_ENABLED ||   \
      RKH_CFG_SMA_DEEP_HIST_EN    == RKH_ENABLED))
    #define RKH_HISTORY_ENABLED
#endif

#if (RKH_CFG_SMA_HCAL_EN         == RKH_ENABLED && \
     RKH_CFG_SMA_PSEUDOSTATE_EN  == RKH_ENABLED && \
     RKH_CFG_SMA_DEEP_HIST_EN    == RKH_ENABLED)
    #define RKH_DEEP_ENABLED
#endif

#if (RKH_CFG_SMA_HCAL_EN         == RKH_ENABLED && \
     RKH_CFG_SMA_PSEUDOSTATE_EN  == RKH_ENABLED && \
     RKH_CFG_SMA_SHALLOW_HIST_EN == RKH_ENABLED)
    #define RKH_SHALLOW_ENABLED
#endif

#if (RKH_CFG_SMA_PSEUDOSTATE_EN  == RKH_ENABLED && \
     RKH_CFG_SMA_SUBMACHINE_EN   == RKH_ENABLED)
    #define RKH_SUBMACHINE_ENABLED
#endif

#if (RKH_CFG_SMA_PSEUDOSTATE_EN  == RKH_ENABLED && \
     RKH_CFG_SMA_CHOICE_EN       == RKH_ENABLED)
    #define RKH_CHOICE_ENABLED
#endif

#if (RKH_CFG_SMA_PSEUDOSTATE_EN  == RKH_ENABLED && \
     (RKH_CFG_SMA_CONDITIONAL_EN  == RKH_ENABLED || \
      RKH_CFG_SMA_CHOICE_EN       == RKH_ENABLED))
    #define RKH_CHOICE_OR_CONDITIONAL_ENABLED
#endif

#if (RKH_CFG_TRC_EN == RKH_ENABLED  && \
     (RKH_CFG_TRC_ALL_EN == RKH_ENABLED || \
      RKH_CFG_TRC_SMA_EN == RKH_ENABLED || \
      RKH_CFG_TRC_SM_EN == RKH_ENABLED || \
      RKH_CFG_TRC_FWK_EN == RKH_ENABLED))
    #define R_TRC_AO_NAME_EN    RKH_ENABLED
#else
    #define R_TRC_AO_NAME_EN    RKH_DISABLED
#endif

#if (RKH_CFG_TRC_EN == RKH_ENABLED  && \
     (RKH_CFG_TRC_ALL_EN == RKH_ENABLED || \
      RKH_CFG_TRC_MP_EN == RKH_ENABLED))
    #define RKH_MEMPOOL_EN  RKH_ENABLED
#else
    #define RKH_MEMPOOL_EN  RKH_DISABLED
#endif

#define MKTRN(evt_, guard_, effect_, target_) \
    { \
        evt_, (RKH_GUARD_T)guard_, \
        (RKH_TRN_ACT_T)effect_, (RKHROM RKH_ST_T *)target_ \
    }

#if RKH_CFG_SMA_SM_CONST_EN == RKH_ENABLED
    #define MKSM(constSM, initialState) \
        (RKHROM RKH_ROM_T *)(constSM), /** RKH_SM_T::romrkh member */ \
        (RKHROM RKH_ST_T *)(initialState), /** RKH_SM_T::state member */ \
        (RKH_EVT_T *)(0) /** RKH_SM_T::propagatedEvent member */

    #if RKH_CFG_SMA_VFUNCT_EN == RKH_ENABLED
        #define MKSMA(constSM, initialState) \
            { \
                MKSM(constSM, initialState), /** RKH_SM_T members */ \
                &rkhSmaVtbl /** RKH_SMA_T::vptr member */ \
            }
    #else
        #define MKSMA(constSM, initialState) \
            { \
                MKSM(constSM, initialState)  /** RKH_SM_T members */ \
            }
    #endif
#else
    #if (RKH_CFG_SMA_INIT_EVT_EN == RKH_ENABLED)
        #if R_TRC_AO_NAME_EN == RKH_ENABLED
            #define MKSM(name, prio, ppty, initialState, initialAction, \
                         initialEvt) \
                (prio), (ppty), #name, (RKHROM RKH_ST_T*)initialState, \
                ((RKH_TRN_ACT_T)initialAction), (initialEvt), \
                (RKHROM RKH_ST_T*)initialState, (RKH_EVT_T*)0
        #else
            #define MKSM(name, prio, ppty, initialState, initialAction, \
                         initialEvt) \
                (prio), (ppty), (RKHROM RKH_ST_T*)initialState, \
                ((RKH_TRN_ACT_T)initialAction), (initialEvt), \
                (RKHROM RKH_ST_T*)initialState, (RKH_EVT_T*)0
        #endif
    #else
        #if R_TRC_AO_NAME_EN == RKH_ENABLED
            #define MKSM(name, prio, ppty, initialState, initialAction, \
                         initialEvt) \
                (prio), (ppty), #name, (RKHROM RKH_ST_T*)initialState, \
                ((RKH_TRN_ACT_T)initialAction), \
                (RKHROM RKH_ST_T*)initialState, (RKH_EVT_T*)0
        #else
            #define MKSM(name, prio, ppty, initialState, initialAction, \
                         initialEvt) \
                (prio), (ppty), (RKHROM RKH_ST_T*)initialState, \
                ((RKH_TRN_ACT_T)initialAction), \
                (RKHROM RKH_ST_T*)initialState, (RKH_EVT_T*)0
        #endif
    #endif
    #if RKH_CFG_SMA_VFUNCT_EN == RKH_ENABLED
        #define MKSMA(name, prio, ppty, \
                      initialState, initialAction, initialEvt) \
            { \
                MKSM(name, prio, ppty, initialState, \
                     initialAction, initialEvt), /* RKH_SM_T members */ \
                &rkhSmaVtbl /** RKH_SMA_T::vptr member */ \
            }
    #else
        #define MKSMA(name, prio, ppty, \
                      initialState, initialAction, initialEvt) \
            { \
                MKSM(name, prio, ppty, initialState, \
                     initialAction, initialEvt)  /* RKH_SM_T members */ \
            }
    #endif

    #define MKRT_SM(sm_, name_, prio_, ppty_, initialState_, initialAction_, \
                    initialEvt_) \
        ((RKH_SM_T *)(sm_))->prio = prio_; \
        ((RKH_SM_T *)(sm_))->ppty = ppty_; \
        MKSM_NAME(sm_, name_); \
        ((RKH_SM_T *)(sm_))->istate = (RKHROM RKH_ST_T*)initialState_; \
        ((RKH_SM_T *)(sm_))->iaction = (RKH_TRN_ACT_T)initialAction_; \
        MKSM_IEVENT(sm_, initialEvt_); \
        ((RKH_SM_T *)(sm_))->state = (RKHROM RKH_ST_T*)initialState_; \
        ((RKH_SM_T *)(sm_))->propagatedEvent = (RKH_EVT_T*)0;

    #if R_TRC_AO_NAME_EN == RKH_ENABLED
        #if RKH_CFG_SMA_INIT_EVT_EN == RKH_ENABLED
            #define MKSM_NAME(sm_, name_) \
                ((RKH_SM_T *)sm_)->name = #name_
            #define MKSM_IEVENT(sm_, ievent_) \
                ((RKH_SM_T *)sm_)->ievent = ievent_
        #else
            #define MKSM_NAME(sm_, name_) \
                ((RKH_SM_T *)sm_)->name = #name_
            #define MKSM_IEVENT(sm_, ievent_)
        #endif
    #else
        #if RKH_CFG_SMA_INIT_EVT_EN == RKH_ENABLED
            #define MKSM_NAME(sm_, name_) \
            #define MKSM_IEVENT(sm_, ievent_) \
                ((RKH_SM_T *)sm_)->ievent = ievent_
        #else
            #define MKSM_NAME(sm_, name_) \
            #define MKSM_IEVENT(sm_, ievent_)
        #endif
    #endif
#endif

#if (RKH_CFG_SMA_INIT_EVT_EN == RKH_ENABLED)
    #if R_TRC_AO_NAME_EN == RKH_ENABLED
        #define MKRRKH(name, prio, ppty, is, ia, ie) \
            { \
                (prio), (ppty), # name, (RKHROM RKH_ST_T *)is, \
                (RKH_TRN_ACT_T)(ia), (ie) \
            }
    #else
        #define MKRRKH(name, prio, ppty, is, ia, ie) \
            { \
                (prio), (ppty), (RKHROM RKH_ST_T *)is, \
                (RKH_TRN_ACT_T)(ia), (ie) \
            }
    #endif
#else
    #if R_TRC_AO_NAME_EN == RKH_ENABLED
        #define MKRRKH(name, prio, ppty, is, ia, ie) \
            { \
                (prio), (ppty), # name, (RKHROM RKH_ST_T *)is, \
                (RKH_TRN_ACT_T)(ia) \
            }
    #else
        #define MKRRKH(name, prio, ppty, is, ia, ie) \
            { \
                (prio), (ppty), (RKHROM RKH_ST_T *)is, \
                (RKH_TRN_ACT_T)(ia) \
            }
    #endif
#endif

#if R_TRC_AO_NAME_EN == RKH_ENABLED
    #define MKBASE(t, n)        {t, # n}
#else
    #define MKBASE(t, n)        {t}
#endif

#if (RKH_CFG_SMA_HCAL_EN == RKH_ENABLED)
    #if (RKH_CFG_SMA_PPRO_EN == RKH_ENABLED)
        #if defined(RKH_HISTORY_ENABLED)
            #define MKBASIC(n,pp)               n##_trtbl, (RKH_PPRO_T)pp
            #define MKCOMP(n, d, dftTrn_, h)    n##_trtbl, NULL, d, \
                                                (RKH_TRN_ACT_T)dftTrn_, h
            #define MKHIST_INCOMP(name, kOfH, dTG, dTA, dTT, ramMem) \
                RKHROM RKH_SHIST_T name##Hist = \
                { \
                    MKBASE(kOfH, name##Hist), \
                    (RKHROM RKH_ST_T *)&name, ramMem, \
                    RKH_TRREG(0, dTG, dTA, dTT) \
                }
            #define MKHISTORY(name, parent, kOfH, dTG, dTA, dTT, ramMem) \
                RKHROM RKH_SHIST_T name = \
                { \
                    MKBASE(kOfH, name), \
                    (RKHROM RKH_ST_T *)parent, ramMem, \
                    RKH_TRREG(0, dTG, dTA, dTT) \
                }
            #define MKFINAL(name_) \
                name_##_trtbl, NULL
            #define MKFINAL_INCOMP(name_) \
                RKH_CREATE_FINAL_STATE(name_##Final, &name_)
        #else
            #define MKBASIC(n,pp)               n##_trtbl, (RKH_PPRO_T)pp
            #define MKCOMP(n, d, dftTrn_, h)    n##_trtbl, NULL, d, \
                                                (RKH_TRN_ACT_T)dftTrn_
            #define MKHIST_INCOMP(name, kOfH, dTG, dTA, dTT, ramMem)
            #define MKHISTORY(name, parent, kOfH, dTG, dTA, dTT, ramMem)
            #define MKFINAL(name_) \
                name_##_trtbl, NULL
            #define MKFINAL_INCOMP(name_) \
                RKH_CREATE_FINAL_STATE(name_##Final, &name_)
        #endif
    #else
        #if defined(RKH_HISTORY_ENABLED)
            #define MKBASIC(n,pp)               n##_trtbl
            #define MKCOMP(n, d, dftTrn_, h)    n##_trtbl, d, \
                                                (RKH_TRN_ACT_T)dftTrn_, h
            #define MKHIST_INCOMP(name, kOfH, dTG, dTA, dTT, ramMem) \
                RKHROM RKH_SHIST_T name##Hist = \
                { \
                    MKBASE(kOfH, name##Hist), \
                    (RKHROM RKH_ST_T *)&name, ramMem, \
                    RKH_TRREG(0, dTG, dTA, dTT) \
                }
            #define MKHISTORY(name, parent, kOfH, dTG, dTA, dTT, ramMem) \
                RKHROM RKH_SHIST_T name = \
                { \
                    MKBASE(kOfH, name), \
                    (RKHROM RKH_ST_T *)parent, ramMem, \
                    RKH_TRREG(0, dTG, dTA, dTT) \
                }
            #define MKFINAL(name_) \
                name_##_trtbl
            #define MKFINAL_INCOMP(name_) \
                RKH_CREATE_FINAL_STATE(name_##Final, &name_)
        #else
            #define MKBASIC(n,pp)               n##_trtbl
            #define MKCOMP(n, d, dftTrn_, h)    n##_trtbl, d, \
                                                (RKH_TRN_ACT_T)dftTrn_
            #define MKHIST_INCOMP(name, kOfH, dTG, dTA, dTT, ramMem)
            #define MKHISTORY(name, parent, kOfH, dTG, dTA, dTT, ramMem)
            #define MKFINAL(name_) \
                name_##_trtbl
            #define MKFINAL_INCOMP(name_) \
                RKH_CREATE_FINAL_STATE(name_##Final, &name_)
        #endif
    #endif
    #define MKST(en,ex,p)           (RKH_ENT_ACT_T)en, \
                                    (RKH_EXT_ACT_T)ex, \
                                    (RKHROM RKH_ST_T *)p
    #define MKSBM(n,sbm)            n##_trtbl,n##_exptbl,sbm
    #define MKMCH(d,i,n)            d, (RKH_TRN_ACT_T)i, \
                                    (RKHROM RKH_ST_T * *)&rdyp_##n
    #define MKENP(e,s)              e,(RKHROM RKH_ST_T *)s
#else
    #if (RKH_CFG_SMA_PPRO_EN == RKH_ENABLED)
        #define MKBASIC(n,pp)       n##_trtbl, (RKH_PPRO_T)pp
        #define MKCOMP(n, d, dftTrn_, h)        n##_trtbl, NULL
        #define MKFINAL(name_)
        #define MKFINAL_INCOMP(name_)
    #else
        #define MKBASIC(n,pp)       n##_trtbl
        #define MKCOMP(n, d, dftTrn_, h)        n##_trtbl
        #define MKFINAL(name_)
        #define MKFINAL_INCOMP(name_)
    #endif
    #define MKST(en,ex,p)
    #define MKSBM(n,sbm)            n##_trtbl,n##_exptbl,sbm
    #define MKMCH(d,i,n)            d, (RKH_TRN_ACT_T)i, \
                                    (RKHROM RKH_ST_T *)&rdyp_##n
    #define MKENP(e,s)              e,(RKHROM RKH_ST_T *)s
#endif

#define MK_SET_EVT(ev_obj, ev_sig) \
    ((RKH_EVT_T *)(ev_obj))->e = (RKH_SIG_T)ev_sig;  \
    ((RKH_EVT_T *)(ev_obj))->nref = 0; \
    ((RKH_EVT_T *)(ev_obj))->pool = 0

#define MK_EVT(ev_obj, ev_sig) \
    RKH_EVT_T ev_obj = {ev_sig, 0, 0}

#define MK_ROM_EVT(ev_obj, ev_sig) \
    RKHROM RKH_EVT_T ev_obj = {ev_sig,  0, 0}

#define MK_EVT_STRUCT(ev_sig) \
    {ev_sig, 0, 0}

#ifndef RKH_DIS_INTERRUPT
    #error \
    "rkhport.h, Must be defined the platform-dependent macro RKH_DIS_INTERRUPT"
#endif

#ifndef RKH_ENA_INTERRUPT
    #error \
    "rkhport.h, Must be defined the platform-dependent macro RKH_ENA_INTERRUPT"
#endif

#ifndef RKH_EN_DOXYGEN
    #define RKH_EN_DOXYGEN  0
#endif

#if RKH_EN_DOXYGEN == RKH_ENABLED
    /** @{
     *  \brief
     *  RKH need to disable interrupts in order to access critical sections
     *  of code, and re-enable interrupts when done. This allows RKH to
     *  protect critical code from being entered simultaneously. To hide the
     *  implementation method chosen by the processor, compiler, etc, RKH
     *  defines two macros to unconditionally disable and enable interrupts:
     *  RKH_DIS_INTERRUPT() and RKH_ENA_INTERRUPT() respectively. Obviously,
     *  they resides in \c rkhport.h file, which the user always need to
     *  provide.
     *
     *  \n <EM>Example for HCS08 CW6.3</EM>
     *  \code
     *  #define RKH_DIS_INTERRUPT()			DisableInterrupts
     *  #define RKH_ENA_INTERRUPT()			EnableInterrupts
     *  \endcode
     *  <EM>Example for uC/OS-III and KSDK</EM>
     *  \code
     *  #define RKH_DIS_INTERRUPT()         INT_SYS_DisableIRQGlobal()
     *  #define RKH_ENA_INTERRUPT()         INT_SYS_EnableIRQGlobal()
     *  \endcode
     */
    #define RKH_DIS_INTERRUPT()
    #define RKH_ENA_INTERRUPT()
    /**@}*/

    /** @{
     *  \brief
     *  RKH need to disable interrupts in order to access critical sections of
     *  code, and re-enable interrupts when done.
     *
     *  This allows RKH to protect
     *  critical code from being entered simultaneously from either multiple
     *  SMAs or ISRs. Every processor generally provide instructions to
     *  disable/enable interrupts and the C compiler must have a mechanism to
     *  perform these operations directly from C. Some compilers will allows
     *  to insert in-line assembly language statements in the C source code.
     *  This makes it quite easy to insert processor instructions to enable and
     *  disable interrupts. Other compilers will actually contain language
     *  extensions to enable and disable interrupts directly from C. To hide
     *  the implementation method chosen by the compiler manufacturer, RKH
     *  defines two macros to disable and enable interrupts:
     *  RKH_ENTER_CRITICAL() and RKH_EXIT_CRITICAL().
     *
     *  The RKH_ENTER_CRITICAL() macro saves the interrupt disable status onto
     *  the stack and then, disable interrupts.
     *  RKH_EXIT_CRITICAL() would simply be implemented by restoring the
     *  interrupt status from the stack. Using this scheme, if it's called a
     *  RKH service with either interrupts enabled or disabled then, the
     *  status would be preserved across the call. If calls a RKH service with
     *  interrupts disabled, is potentially extending the interrupt latency of
     *  application. The application can use RKH_ENTER_CRITICAL() and
     *  RKH_EXIT_CRITICAL() to also protect critical sections of code. As a
     *  general rule, should always call RKH services with interrupts enabled!.
     *
     *  \note
     *  These macros are internal to RKH and the user application should
     *  not call it.
     *
     *  <EM>Example for x86, VC2008, and win32 single thread:</EM>
     *  \code
     *  //#define RKH_CPUSR_TYPE
     *  #define RKH_ENTER_CRITICAL(dummy)		EnterCriticalSection(&csection)
     *  #define RKH_EXIT_CRITICAL(dummy)		LeaveCriticalSection(&csection)
     *  \endcode
     *  <EM>Example for uC/OS-III and KSDK</EM>
     *  \code
     *  //#define RKH_CPUSR_TYPE
     *  #define RKH_ENTER_CRITICAL(dummy)         \
     *              OSA_EnterCritical(kCriticalDisableInt)
     *  #define RKH_EXIT_CRITICAL(dummy)          \
     *              OSA_ExitCritical(kCriticalDisableInt)
     *  \endcode
     */
    #define RKH_CPUSR_TYPE
    #define RKH_SR_ALLOC() \
        RKH_CPUSR_TYPE sr = (RKH_CPUSR_TYPE)0
    #define RKH_ENTER_CRITICAL(dummy)
    #define RKH_EXIT_CRITICAL(dummy)
    /**@}*/

    /**
     *  \addtogroup config
     *  @{
     *  \addtogroup configPort Related to RKH port
     *  @{
     *  \brief Configuration options related to RKH port
     */

    /**
     *  \brief
     *	If the #RKH_CFGPORT_SMA_THREAD_EN is set to 1, each SMA (active
     *	object) has its own thread of execution.
     *
     * \type       Boolean 
     * \range      
     * \default    RKH_DISABLED
     */
    #define RKH_CFGPORT_SMA_THREAD_EN           RKH_ENABLED

    /**
     *  \brief
     *	If the #RKH_CFGPORT_SMA_THREAD_EN and
     *	#RKH_CFGPORT_SMA_THREAD_DATA_EN are set to 1, each SMA (active
     *	object) has its own thread of execution and its own object data.
     *  When enabling it, the option RKH_OSSIGNAL_TYPE must be defined. 
     *
     *  \copybrief RKH_OSSIGNAL_TYPE
     *
     * \type       Boolean 
     * \range      
     * \default    RKH_DISABLED
     */
    #define RKH_CFGPORT_SMA_THREAD_DATA_EN      RKH_ENABLED

    /**
     *  \brief
     *  If the #RKH_CFGPORT_NATIVE_SCHEDULER_EN is set to 1 then RKH will
     *  include the simple, cooperative, and nonpreemptive scheduler RKHS.
     *  When #RKH_CFGPORT_NATIVE_SCHEDULER_EN is enabled RKH also will
     *  automatically define #RKH_EQ_TYPE, and include rkh_sma_block(), 
     *  rkh_sma_setReady(), rkh_sma_setUnready(), and assume the native 
     *  priority scheme.
     *
     * \type       Boolean 
     * \range      
     * \default    RKH_ENABLED
     */
    #define RKH_CFGPORT_NATIVE_SCHEDULER_EN     RKH_ENABLED

    /**
     *  \brief
     *  If the #RKH_CFGPORT_NATIVE_EQUEUE_EN is set to 1 and the native
     *  event queue is enabled (see #RKH_CFG_QUE_EN) then RKH will include
     *  its own implementation of rkh_sma_post_fifo(), rkh_sma_post_lifo(),
     *  and rkh_sma_get() functions.
     *
     * \type       Boolean 
     * \range      
     * \default    RKH_ENABLED
     */
    #define RKH_CFGPORT_NATIVE_EQUEUE_EN        RKH_ENABLED

    /**
     *  \brief
     *  If the #RKH_CFGPORT_NATIVE_DYN_EVT_EN is set to 1 and the native 
     *  fixed-size memory block facility is enabled (see #RKH_CFG_MP_EN) then 
     *  RKH will include its own implementation of dynamic memory management.
     *  When #RKH_CFGPORT_NATIVE_DYN_EVT_EN is enabled RKH also will provide 
     *  the event pool manager implementation based on its native memory pool 
     *  module.
     *
     * \type       Boolean 
     * \range      
     * \default    RKH_ENABLED
     */
    #define RKH_CFGPORT_NATIVE_DYN_EVT_EN       RKH_ENABLED

    /**
     *  \brief
     *	If the #RKH_CFGPORT_REENTRANT_EN is set to 1, the RKH event dispatch
     *	allows to be invoked from several threads of executions. Enable this
     *	only if the application is based on a multi-thread architecture.
     *
     * \type       Boolean 
     * \range      
     * \default    RKH_DISABLED
     */
    #define RKH_CFGPORT_REENTRANT_EN            RKH_ENABLED

    /**
     *  \brief
     *  Specify the size of void pointer. The valid values [in bits] are
     *  16 or 32. Default is 32. See RKH_TRC_SYM() macro.
     *
     *	\type       Integer
     *	\range      [8, 16, 32]
     *	\default    32
     */
    #define RKH_CFGPORT_TRC_SIZEOF_PTR          32u

    /**
     *  \brief
     *  Specify the size of function pointer. The valid values [in bits] are
     *  16 or 32. Default is 32. See RKH_TUSR_FUN() and RKH_TRC_FUN() macros.
     *
     *	\type       Integer
     *	\range      [8, 16, 32]
     *	\default    32
     */
    #define RKH_CFGPORT_TRC_SIZEOF_FUN_PTR      32u

    /**
     *  \brief
     *  Specify the number of bytes (size) used by the trace record timestamp.
     *  The valid values [in bits] are 8, 16 or 32. Default is 16.
     *
     *	\type       Integer
     *	\range      [8, 16, 32]
     *	\default    16
     */
    #define RKH_CFGPORT_TRC_SIZEOF_TSTAMP       32u

    /**
     *  \brief
     *  If the #RKH_CFGPORT_SMA_QSTO_EN is set to 1 then RKH_SMA_ACTIVATE()
     *  macro invokes the rkh_sma_activate() function ignoring the external
     *  event queue storage argument, \c qs.
     *
     * \type       Boolean 
     * \range      
     * \default    RKH_ENABLED
     */
    #define RKH_CFGPORT_SMA_QSTO_EN             RKH_ENABLED

    /**
     *  \brief
     *  If the #RKH_CFGPORT_SMA_STK_EN is set to 0 then RKH_SMA_ACTIVATE()
     *  macro invokes the rkh_sma_activate() function ignoring the thread's
     *  stack related arguments, \c stks and \c stksize.
     *
     * \type       Boolean 
     * \range      
     * \default    RKH_DISABLED
     */
    #define RKH_CFGPORT_SMA_STK_EN              RKH_ENABLED

    /** @} doxygen end group definition */
    /** @} doxygen end group definition */

    /**
     *  \brief
     *  Defines the data type of the event queue for active objects.
     *  The event queue can be implemented with a message queue of
     *  the RTOS/OS. But it is also possible to use the native RKH
     *  queue RKH_QUEUE_T type if the underlying RTOS/OS does not provide
     *  an adequate queue.
     *
     *  <EM>Example for using the native queue</EM>
     *  \code
     *	// RKH's native queue
     *  #define RKH_EQ_TYPE			RKH_QUEUE_T
     *	// Message queue of ucos/II
     *  #define RKH_EQ_TYPE			OS_EVENT
     *	// Message queue of FreeRTOS
     *  #define RKH_EQ_TYPE			xQueueHandle
     *  \endcode
     */
    #define RKH_EQ_TYPE

    /**
     *  \brief
     *  Frequently, the active object has its own task processing loop that
     *  waits for the signal to be posted, and when it is, loops to remove
     *  and process all events that are currently queued.
     *  The RKH_SMA_POST_FIFO() macro enqueues an event and signals	the OS
     *  that an event has arrived. In this case, \c os_signal holds the OS
     *  object used to signal that an event has been queued.
     *  The data type RKH_THREAD_TYPE holds the thread handle associated
     *  with the active object.
     *
     *  <EM>Example for using the uCOS/II, FreeRTOS, Linux (Posix) and
     *  Win32</EM>
     *  \code
     *	// For uCOS/II
     *  #define RKH_THREAD_TYPE		INT8U
     *	// For FreeRTOS
     *  #define RKH_THREAD_TYPE		xTaskHandle
     *	// For Linux (Posix)
     *  #define RKH_THREAD_TYPE		pthread_t
     *	// For Win32
     *  #define RKH_THREAD_TYPE		void*
     *  \endcode
     */
    #define RKH_THREAD_TYPE

    /**
     *  \brief
     *  The \c os_signal member of RKH_SMA_T is necessary when the underlying
     *  OS does not provide an adequate queue facility, so the native RKH
     *  queue RKH_QUEUE_T must be used. In this case the RKH_OSSIGNAL_TYPE
     *  indicates an operating system specific primitive to efficiently
     *  block the native RKH event queue when the queue is empty.
     *  Frequently, the active object has its own task processing loop that
     *  waits for the signal to be posted, and when it is, loops to remove
     *  and process all events that are currently queued.
     *  The RKH_SMA_POST_FIFO() macro enqueues an event and signals	the OS
     *  that an event has arrived. In this case, \c os_signal holds the OS
     *  object used to signal that an event has been queued.
     *
     *  <EM>Example for using the Linux (Posix) and Win32</EM>
     *  \code
     *	// For Linux (Posix)
     *  #define RKH_OSSIGNAL_TYPE	pthread_cond_t
     *	// For Win32
     *  #define RKH_OSSIGNAL_TYPE	void*
     *  \endcode
     */
    #define RKH_OSSIGNAL_TYPE

    /**
     *  \brief
     *  Data type to declare thread stack, which is only used when the 
     *  underlying OS does not internally allocate the RAM storage to its 
     *  threads, in this case must be enabled RKH_CFGPORT_SMA_STK_EN option in 
     *  \c rkhport.h file.

     *  <EM>Example</EM>
     *  \code
     *  // In rkhport.h
     *  #define RKH_THREAD_STK_TYPE     rui8_t
     *
     *  // In application code
     *  // Defines the task's stack for active object 'server'
     *  static RKH_THREAD_STK_TYPE serverStk[SERVER_STK_SIZE];
     *  \endcode
     */
    #define RKH_THREAD_STK_TYPE 

#endif

#ifdef RKH_CPUSR_TYPE
    #if RKH_EN_DOXYGEN == RKH_DISABLED
        #define RKH_SR_ALLOC()          RKH_CPUSR_TYPE sr = (RKH_CPUSR_TYPE)0
        #define RKH_ENTER_CRITICAL_()   RKH_ENTER_CRITICAL(sr)
        #define RKH_EXIT_CRITICAL_()    RKH_EXIT_CRITICAL(sr)
    #endif
#else
    #define RKH_SR_ALLOC()
    #define RKH_ENTER_CRITICAL_()       RKH_ENTER_CRITICAL(dummy)
    #define RKH_EXIT_CRITICAL_()        RKH_EXIT_CRITICAL(dummy)
#endif

#if RKH_CFG_SMA_ENT_ARG_SMA_EN == RKH_ENABLED
    #if RKH_CFG_SMA_ENT_ARG_STATE_EN == RKH_ENABLED
        #define RKH_EXEC_ENTRY(state_, me_) \
            if ((state_)->enter) \
            { \
                (*(state_)->enter)(me_, state_); \
                RKH_TR_SM_EXE_ACT(RKH_SUBTE_SM_EXE_ACT_EN, \
                                  (me_), \
                                  (state_), \
                                  (state_)->enter); \
            }
    #else
        #define RKH_EXEC_ENTRY(state_, me_) \
            if ((state_)->enter) \
            { \
                (*(state_)->enter)(me_); \
                RKH_TR_SM_EXE_ACT(RKH_SUBTE_SM_EXE_ACT_EN, \
                                  (me_), \
                                  (state_), \
                                  (state_)->enter); \
            }
    #endif
#else
    #if RKH_CFG_SMA_ENT_ARG_STATE_EN == RKH_ENABLED
        #define RKH_EXEC_ENTRY(state_, me_) \
            if ((state_)->enter) \
            { \
                (*(state_)->enter)(state_); \
                RKH_TR_SM_EXE_ACT(RKH_SUBTE_SM_EXE_ACT_EN, \
                                  (me_), \
                                  (state_), \
                                  (state_)->enter); \
            }
    #else
    #define RKH_EXEC_ENTRY(state_, me_) \
        if ((state_)->enter) \
        { \
            (*(state_)->enter)(); \
            RKH_TR_SM_EXE_ACT(RKH_SUBTE_SM_EXE_ACT_EN, \
                              (me_), \
                              (state_), \
                              (state_)->enter); \
        }
    #endif
#endif

#if RKH_CFG_SMA_EXT_ARG_SMA_EN == RKH_ENABLED
    #if RKH_CFG_SMA_ENT_ARG_STATE_EN == RKH_ENABLED
        #define RKH_EXEC_EXIT(state_, me_) \
            if ((state_)->exit) \
            { \
                (*(state_)->exit)(me_, state_); \
                RKH_TR_SM_EXE_ACT(RKH_SUBTE_SM_EXE_ACT_EX, \
                                  (me_), \
                                  (state_), \
                                  (state_)->exit); \
            }
    #else
        #define RKH_EXEC_EXIT(state_, me_) \
            if ((state_)->exit) \
            { \
                (*(state_)->exit)(me_); \
                RKH_TR_SM_EXE_ACT(RKH_SUBTE_SM_EXE_ACT_EX,  \
                                  (me_), \
                                  (state_), \
                                  (state_)->exit); \
            }
    #endif
#else
    #if RKH_CFG_SMA_ENT_ARG_STATE_EN == RKH_ENABLED
        #define RKH_EXEC_EXIT(state_, me_) \
            if ((state_)->exit) \
            { \
                (*(state_)->exit)(state_); \
                RKH_TR_SM_EXE_ACT(RKH_SUBTE_SM_EXE_ACT_EX, \
                                  (me_), \
                                  (state_), \
                                  (state_)->exit); \
            }
    #else
        #define RKH_EXEC_EXIT(state_, me_) \
            if ((state_)->exit) \
            {  \
                (*(state_)->exit)(); \
                RKH_TR_SM_EXE_ACT(RKH_SUBTE_SM_EXE_ACT_EX, \
                                  (me_), \
                                  (state_), \
                                  (state_)->exit); \
            }
    #endif
#endif

#if RKH_CFG_SMA_PPRO_ARG_SMA_EN == RKH_ENABLED
    #define RKH_EXEC_PREPRO(state_, me_, evt_) \
        (*(state_)->prepro)(me_, evt_); \
        RKH_TR_SM_EXE_ACT(RKH_SUBTE_SM_EXE_ACT_PP, \
                          (me_), \
                          (((RKH_SM_T *)me_))->state, \
                          (state_)->prepro)
#else
    #define RKH_EXEC_PREPRO(state_, me_, evt_) \
        (*(state_)->prepro)(evt_); \
        RKH_TR_SM_EXE_ACT(RKH_SUBTE_SM_EXE_ACT_PP, \
                          (me_), \
                          (((RKH_SM_T *)me_))->state, \
                          (state_)->prepro)
#endif

#if (RKH_CFG_SMA_ACT_ARG_EVT_EN == RKH_ENABLED && \
     RKH_CFG_SMA_ACT_ARG_SMA_EN == RKH_ENABLED)
    #define RKH_EXEC_EFF(action_, me_, evt_) \
        (*CTA(action_))((me_), (evt_)); \
        RKH_TR_SM_EXE_ACT(RKH_SUBTE_SM_EXE_ACT_EFF, \
                          (me_), \
                          (((RKH_SM_T *)me_))->state, \
                          (action_))
#elif (RKH_CFG_SMA_ACT_ARG_EVT_EN == RKH_ENABLED && \
       RKH_CFG_SMA_ACT_ARG_SMA_EN == RKH_DISABLED)
    #define RKH_EXEC_EFF(action_, me_, evt_) \
        (*CTA(action_))((evt_)); \
        RKH_TR_SM_EXE_ACT(RKH_SUBTE_SM_EXE_ACT_EFF, \
                          (me_), \
                          (((RKH_SM_T *)me_))->state, \
                          (action_))
#elif (RKH_CFG_SMA_ACT_ARG_EVT_EN == RKH_DISABLED && \
       RKH_CFG_SMA_ACT_ARG_SMA_EN == RKH_ENABLED)
    #define RKH_EXEC_EFF(action_, me_, evt_) \
        (*CTA(action_))((me_)); \
        RKH_TR_SM_EXE_ACT(RKH_SUBTE_SM_EXE_ACT_EFF, \
                          (me_), \
                          (((RKH_SM_T *)me_))->state, \
                          (action_))
#else
    #define RKH_EXEC_EFF(action_, me_, evt_) \
        (*CTA(action_))(); \
        RKH_TR_SM_EXE_ACT(RKH_SUBTE_SM_EXE_ACT_EFF, \
                          (me_), \
                          (((RKH_SM_T *)me_))->state, \
                          (action_))
#endif

#if (RKH_CFG_SMA_GRD_ARG_EVT_EN == RKH_ENABLED && \
     RKH_CFG_SMA_GRD_ARG_SMA_EN == RKH_ENABLED)
    #define RKH_EXEC_GUARD(trn_, me_, evt_) \
        (*(trn_)->guard)(me_, evt_)
#elif (RKH_CFG_SMA_GRD_ARG_EVT_EN == RKH_ENABLED && \
       RKH_CFG_SMA_GRD_ARG_SMA_EN == RKH_DISABLED)
    #define RKH_EXEC_GUARD(trn_, me_, evt_) \
        (*(trn_)->guard)(evt_)
#elif (RKH_CFG_SMA_GRD_ARG_EVT_EN == RKH_DISABLED && \
       RKH_CFG_SMA_GRD_ARG_SMA_EN == RKH_ENABLED)
    #define RKH_EXEC_GUARD(trn_, me_, evt_) \
        (*(trn_)->guard)(me_)
#else
    #define RKH_EXEC_GUARD(trn_, me_, evt_) \
        (*(trn_)->guard)()
#endif

/* -------------------------------- Constants ------------------------------ */
#ifndef NULL
#define NULL    (void *)0
#endif

/* ------------------------------- Data types ------------------------------ */
typedef struct RKH_ROM_T RKH_ROM_T;
typedef struct RKHSmaVtbl RKHSmaVtbl;
typedef struct RKH_SM_T RKH_SM_T;
typedef struct RKH_SMA_T RKH_SMA_T;
typedef struct RKH_TR_T RKH_TR_T;
typedef struct RKH_BASE_T RKH_BASE_T;
typedef struct RKH_ST_T RKH_ST_T;
typedef struct RKH_SHIST_T RKH_SHIST_T;
typedef struct RKH_SBSC_T RKH_SBSC_T;
typedef struct RKH_SCMP_T RKH_SCMP_T;
typedef struct RKH_FINAL_T RKH_FINAL_T;
typedef struct RKH_EXPCN_T RKH_EXPCN_T;
typedef struct RKH_ENPCN_T RKH_ENPCN_T;
typedef struct RKH_SSBM_T RKH_SSBM_T;
typedef struct RKH_RSM_T RKH_RSM_T;
typedef struct RKH_SENP_T RKH_SENP_T;
typedef struct RKH_SEXP_T RKH_SEXP_T;
typedef struct RKH_SCOND_T RKH_SCOND_T;
typedef struct RKH_SCHOICE_T RKH_SCHOICE_T;

/* -------------------------- External variables --------------------------- */
/**
 *  \brief
 *  String representing the name of undefined object name.
 */
extern RKHROM char noname[];

/* -------------------------- Function prototypes -------------------------- */
/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
