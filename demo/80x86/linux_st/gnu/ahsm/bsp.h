/**
 *  \file       bsp.h
 *  \brief      BSP for 80x86 Linux
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2016.03.17  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  francuccilea@gmail.com
 *  DaBa  Dar�o Bali�a       dariosb@gmail.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __BSP_H__
#define __BSP_H__

/* ----------------------------- Include files ----------------------------- */
/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
#define BSP_TICKS_PER_SEC   		100

/**
 * 	\brief
 * 	This macro represents the number of timestamp clock ticks per second. 
 * 	Dividing a count of timestamp ticks by this macro yields the number 
 * 	of seconds.
 */

#define BSP_TS_RATE_HZ		   		1u

/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void bsp_init( int argc, char *argv[] );

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
