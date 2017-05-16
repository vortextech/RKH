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
 *  \file       rkhsma_sync.c
 *  \brief      ...
 *  \ingroup    sma
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2017.05.15  LeFr  v2.4.05  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  francuccilea@gmail.com
 */

/* --------------------------------- Notes --------------------------------- */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkhsma_sync.h"
#include "rkhassert.h"
#include "rkhsma_prio.h"

RKH_MODULE_NAME(rkhsma_sync)

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void 
rkh_sma_block(RKH_SMA_T *const me)
{
    RKH_ASSERT(me->equeue.qty != 0);
}

void 
rkh_sma_setReady(RKH_SMA_T *const me)
{
    /* RKH_RDY_INSERT(readyGroup, RKH_SMA_ACCESS_CONST(me, prio)); */
}

void 
rkh_sma_setUnready(RKH_SMA_T *const me)
{
    /* RKH_RDY_REM(readyGroup, RKH_SMA_ACCESS_CONST(me, prio)); */
}

/* ------------------------------ End of file ------------------------------ */
