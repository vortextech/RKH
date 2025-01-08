/**
 *  \file       PulseCounterMgr.h
 *  \brief      PulseCounterMgr application. It demonstrates how to write an 
 *              active object with parameterized state machines.
 */

/* -------------------------- Development history -------------------------- */
/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __PULSECOUNTERMGR_H__
#define __PULSECOUNTERMGR_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
#define TIME_PER           RKH_TIME_MS(1000)
#define NUM_PULSE_COUNTERS 4
#define TACTMIN            8
#define TACTMAX            8
#define TINACTMAX          8

/* ................................ Signals ................................ */
/* ........................ Declares active object ......................... */
//! [Type declaration]
typedef struct PulseCounterMgr PulseCounterMgr;
//! [Type declaration]
//! [PulseCounterMgr pointer declaration]
RKH_SMA_DCLR_TYPE(PulseCounterMgr, pulseCounterMgr);
//! [PulseCounterMgr pointer declaration]

/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void PulseCounterMgr_ctor(PulseCounterMgr* const me);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
