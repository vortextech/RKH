/**
 *  \file       Monitor.h
 *  \brief      Monitor application. It demonstrates how to write a reactive
 *              active object but without using a state machine.
 */

/* -------------------------- Development history -------------------------- */
/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci lfrancucci@leafagrotronics.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __MONITOR_H__
#define __MONITOR_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
#define TIME_PER RKH_TIME_MS(1000)

/* ................................ Signals ................................ */
/* ........................ Declares active object ......................... */
typedef struct Monitor Monitor;
RKH_SMA_DCLR_TYPE(Monitor, monitor);

/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void Monitor_ctor(Monitor* const me, int baz);
int Monitor_getBaz(Monitor* const me);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
