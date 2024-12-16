/**
 *  \file       smPolymorphism.h
 *  \brief      Helper functions and object defines to facilitate the test of 
 *              production code.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2016.03.17  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */

#ifndef __SMPOLYMORPHISM_H__
#define __SMPOLYMORPHISM_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"

/* ---------------------- External C language linkage ---------------------- */

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
enum
{
    sigSync, sigSignalReady
};

/* ------------------------------- Data types ------------------------------ */
typedef struct Multiple Multiple;

typedef struct MultipleVtbl MultipleVtbl;
struct MultipleVtbl
{
    RKHSmaVtbl base;
    void (*toggle)(Multiple *me);
};

typedef struct Command Command;
typedef struct CallControl CallControl;

typedef struct PerNonReactWoutST PerNonReactWoutST;
typedef struct PerNonReactWithSTWoutQue PerNonReactWithSTWoutQue;

typedef struct SignalReady SignalReady;
struct SignalReady
{
    RKH_EVT_T base;
    double value;
};

/* -------------------------- External variables --------------------------- */
RKH_SMA_DCLR(singleton);

RKH_SMA_DCLR_TYPE(Multiple, multA);
RKH_SMA_DCLR_TYPE(Multiple, multB);

RKH_SMA_DCLR_TYPE(Command, cmdSignal);
RKH_SMA_DCLR_TYPE(Command, cmdRegister);

RKH_SMA_DCLR_TYPE(CallControl, theCallControl);
RKH_SMA_DCLR_TYPE(PerNonReactWoutST, signalMgr);
RKH_SMA_DCLR_TYPE(PerNonReactWithSTWoutQue, collector);

RKH_DCLR_BASIC_STATE idle, active;

/* -------------------------- Function prototypes -------------------------- */
void Singleton_ctor(int foo);
int Singleton_getFoo(void);

void Multiple_ctor(Multiple *const me, int foobar, RKHPostFifo postFifo);
int Multiple_getFoobar(Multiple *const me);
void Multiple_postFifoA(RKH_SMA_T *me, const RKH_EVT_T *e, 
                        const void *const sender);
void Multiple_postFifoB(RKH_SMA_T *me, const RKH_EVT_T *e, 
                        const void *const sender);

void Command_task(RKH_SMA_T *me, void *arg);
void Command_postFifo(RKH_SMA_T *me, const RKH_EVT_T *e, 
                      const void *const sender);
void Command_postLifo(RKH_SMA_T *me, const RKH_EVT_T *e, 
                      const void *const sender);
void Command_ctor(Command *const me, int bar);

void CallControl_activate(RKH_SMA_T *me, const RKH_EVT_T **qSto, 
                          RKH_QUENE_T qSize, void *stkSto, rui32_t stkSize);
void CallControl_task(RKH_SMA_T *me, void *arg);
void CallControl_ctorA(int foo);
void CallControl_ctorB(int foo);
int CallControl_getFoo(void);

void PerNonReactWoutST_ctor(PerNonReactWoutST* const me, int baz);
int PerNonReactWoutST_getBaz(PerNonReactWoutST* const me);

void PerNonReactWithSTWoutQue_ctor(PerNonReactWithSTWoutQue* const me, int baz);
int PerNonReactWithSTWoutQue_getBaz(PerNonReactWithSTWoutQue* const me);
rbool_t isGreaterThanLevel1(PerNonReactWithSTWoutQue*const me, RKH_EVT_T *pe);
rbool_t isLessThanLevel1(PerNonReactWithSTWoutQue* const me, RKH_EVT_T* pe);
void activeProcess(PerNonReactWithSTWoutQue* const me, RKH_EVT_T* pe);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
