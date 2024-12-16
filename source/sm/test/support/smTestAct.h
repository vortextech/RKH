/**
 *  \file       smTestAct.h
 *  \ingroup    Test
 *
 *  \brief      Production code.
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

#ifndef __SMTESTACT_H__
#define __SMTESTACT_H__

/* ----------------------------- Include files ----------------------------- */

#include "rkh.h"
#include "smTest.h"

/* ---------------------- External C language linkage ---------------------- */

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */

/* ============================ Initial actions ============================ */
void smTest_init(SmTest *const me, RKH_EVT_T *pe);
void smTest_iS3(SmTest *const me, RKH_EVT_T *pe);
void smTest_iS2(SmTest *const me, RKH_EVT_T *pe);
void smTest_iS22(SmTest *const me, RKH_EVT_T *pe);
void smTest_iS221(SmTest *const me, RKH_EVT_T *pe);

/* ============================ Effect actions ============================= */
void smTest_tr11(SmTest *const me, RKH_EVT_T *pe);
void smTest_tr12(SmTest *const me, RKH_EVT_T *pe);
void smTest_tr13(SmTest *const me, RKH_EVT_T *pe);
void smTest_tr14(SmTest *const me, RKH_EVT_T *pe);
void smTest_tr15(SmTest *const me, RKH_EVT_T *pe);
void smTest_tr16(SmTest *const me, RKH_EVT_T *pe);
void smTest_tr17(SmTest *const me, RKH_EVT_T *pe);
void smTest_tr21(SmTest *const me, RKH_EVT_T *pe);
void smTest_tr22(SmTest *const me, RKH_EVT_T *pe);
void smTest_tr23(SmTest *const me, RKH_EVT_T *pe);
void smTest_tr31(SmTest *const me, RKH_EVT_T *pe);
void smTest_tr32(SmTest *const me, RKH_EVT_T *pe);
void smTest_tr33(SmTest *const me, RKH_EVT_T *pe);
void smTest_tr41(SmTest *const me, RKH_EVT_T *pe);
void smTest_tr42(SmTest *const me, RKH_EVT_T *pe);
void smTest_tr43(SmTest *const me, RKH_EVT_T *pe);
void smTest_tr51(SmTest *const me, RKH_EVT_T *pe);
void smTest_tr52(SmTest *const me, RKH_EVT_T *pe);
void smTest_tr53(SmTest *const me, RKH_EVT_T *pe);
void smTest_tr54(SmTest *const me, RKH_EVT_T *pe);
void smTest_tr55(SmTest *const me, RKH_EVT_T *pe);
void smTest_tr61(SmTest *const me, RKH_EVT_T *pe);
void smTest_tr60(SmTest *const me, RKH_EVT_T *pe);

/* ============================= Entry actions ============================= */
void smTest_nS0(SmTest *const me);
void smTest_nS1(SmTest *const me);
void smTest_nS2(SmTest *const me);
void smTest_nS3(SmTest *const me);
void smTest_nS21(SmTest *const me);
void smTest_nS22(SmTest *const me);
void smTest_nS221(SmTest *const me);
void smTest_nS2211(SmTest *const me);
void smTest_nS31(SmTest *const me);
void smTest_nS5(SmTest *const me);

/* ============================= Exit actions ============================== */
void smTest_xS0(SmTest *const me);
void smTest_xS1(SmTest *const me);
void smTest_xS2(SmTest *const me);
void smTest_xS3(SmTest *const me);
void smTest_xS21(SmTest *const me);
void smTest_xS22(SmTest *const me);
void smTest_xS221(SmTest *const me);
void smTest_xS2211(SmTest *const me);
void smTest_xS31(SmTest *const me);
void smTest_xS5(SmTest *const me);

/* ================================ Guards ================================= */
rbool_t smTest_falseGuard(SmTest *const me, RKH_EVT_T *pe);
rbool_t smTest_guard4a(SmTest *const me, RKH_EVT_T *pe);
rbool_t smTest_guard4b(SmTest *const me, RKH_EVT_T *pe);
rbool_t smTest_guard4c(SmTest *const me, RKH_EVT_T *pe);
rbool_t smTest_guardS2(SmTest *const me, RKH_EVT_T *pe);

/* -------------------- External C language linkage end -------------------- */

#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */

#endif

/* ------------------------------ End of file ------------------------------ */
