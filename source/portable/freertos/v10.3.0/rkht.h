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
 *  \file       rkht.h
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
#ifndef __RKHT_H__
#define __RKHT_H__

/* ----------------------------- Include files ----------------------------- */
#include "FreeRTOS.h"       /* the main freertos include file */
#include "stdint.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
/*
 * The RKH uses a set of integer quantities. That maybe machine or
 * compiler dependent.
 */
typedef int8_t ri8_t;
typedef int16_t ri16_t;
typedef int32_t ri32_t;
typedef uint8_t rui8_t;
typedef uint16_t rui16_t;
typedef uint32_t rui32_t;

/*
 * The 'ruint' and 'rInt' will normally be the natural size for a
 * particular machine. These types designates an integer type that is
 * usually fastest to operate with among all integer types.
 */
typedef unsigned int ruint;
typedef signed int rInt;

/*
 * Boolean data type and constants.
 *
 * \note
 * The true (RKH_TRUE) and false (RKH_FALSE) values as defined as macro
 * definitions in \c rkhdef.h file.
 */
typedef rInt rbool_t;

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ File footer ------------------------------ */

