/**
 *  \file       events.h
 *  \brief      Event specification.
 */

/* -------------------------- Development history -------------------------- */
/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci lfrancucci@leafagrotronics.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __EVENTS_H__
#define __EVENTS_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
typedef struct PushedButton PushedButton;
struct PushedButton
{
    RKH_EVT_T evt;
    uint8_t which;
};

typedef struct Simulator Simulator;
struct Simulator
{
    PushedButton base;
    uint32_t additional;
};

typedef struct ADC ADC;
struct ADC
{
    RKH_EVT_T evt;
    uint8_t channel;
    double value;
};

typedef struct DataReady DataReady;
struct DataReady
{
    RKH_EVT_T evt;
    double value;
};

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
