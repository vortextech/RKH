/**
 *  \file       SignalMgr.c
 *  \brief      SignalMgr application.
 */

/* -------------------------- Development history -------------------------- */
/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci lfrancucci@leafagrotronics.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __SIGNALMGR_H__
#define __SIGNALMGR_H__

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
typedef struct SignalMgr SignalMgr;
RKH_SMA_DCLR_TYPE(SignalMgr, signalMgr);

/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void SignalMgr_ctor(SignalMgr* const me, int baz);
int SignalMgr_getBaz(SignalMgr* const me);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
