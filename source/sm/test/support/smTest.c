/**
 *  \file       smTest.c
 *  \ingroup    Test
 *
 *  \brief      State machine to facilitate the test of production code.
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
/* ----------------------------- Include files ----------------------------- */

#include "rkh.h"
#include "smTest.h"
#include "rkhsm.h"
#include "smTestAct.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */

/*
 *	Defines SMA (active object) 'smTest'.
 */
RKH_SMA_CREATE(SmTest, smTest, 0, HCAL, &waiting, smTest_init, NULL);
RKH_SMA_DEF_PTR(smTest);

/*
 *	Defines states and pseudostates.
 */
RKH_CREATE_BASIC_STATE(waiting, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(waiting)
    RKH_TRREG(A, NULL, NULL, &s0),
    RKH_TRREG(B, NULL, NULL, &SmTest_Final),
    RKH_TRINT(C, NULL, smTest_tr16),
    RKH_TRINT(D, NULL, smTest_tr17),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(s0, smTest_nS0, smTest_xS0, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(s0)

    RKH_TRREG(A, NULL, smTest_tr11, &s1),
    RKH_TRREG(B, NULL, smTest_tr12, &s21),
    RKH_TRREG(C, NULL, smTest_tr21, &s2),
    RKH_TRREG(D, NULL, smTest_tr22, &s22),
    RKH_TRREG(E, NULL, NULL, &s22211),
    RKH_TRREG(F, NULL, smTest_tr55, &s3),

RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(s1, smTest_nS1, smTest_xS1, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(s1)

    RKH_TRINT(B, NULL, smTest_tr14),
    RKH_TRINT(C, smTest_falseGuard, smTest_tr14),
    RKH_TRREG(A, NULL, smTest_tr53, &s1),
    RKH_TRREG(D, smTest_falseGuard, NULL, &s0),

RKH_END_TRANS_TABLE

RKH_CREATE_COMP_REGION_STATE(s2, smTest_nS2, smTest_xS2, RKH_ROOT, &s21, 
                             smTest_iS2, RKH_NO_HISTORY, NULL, NULL, NULL, 
                             NULL);
RKH_CREATE_TRANS_TABLE(s2)

    RKH_TRREG(A, NULL, smTest_tr31, &s0),
    RKH_TRREG(D, NULL, smTest_tr32, &s21),
    RKH_TRREG(E, NULL, NULL,        &s5),
    RKH_TRCOMPLETION(smTest_guardS2, smTest_tr60, &s4),

RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(s21, smTest_nS21, smTest_xS21, &s2, NULL);
RKH_CREATE_TRANS_TABLE(s21)

    RKH_TRREG(B, NULL, smTest_tr13, &s0),
    RKH_TRREG(C, NULL, smTest_tr23, &s2),
    RKH_TRREG(F, NULL, NULL, &s2Final),

RKH_END_TRANS_TABLE

RKH_CREATE_COMP_REGION_STATE(s22, smTest_nS22, smTest_xS22, &s2, &s221, 
                             smTest_iS22, RKH_NO_HISTORY, NULL, NULL, NULL, 
                             NULL);
RKH_CREATE_TRANS_TABLE(s22)

    RKH_TRREG(B, NULL, smTest_tr33, &s0),
    RKH_TRREG(C, NULL, smTest_tr43, &s3),
    RKH_TRREG(D, NULL, smTest_tr54, &s22),

RKH_END_TRANS_TABLE

RKH_CREATE_COMP_REGION_STATE(s221, smTest_nS221, smTest_xS221, &s22, &s2211, 
                             smTest_iS221, RKH_NO_HISTORY, NULL, NULL, NULL, 
                             NULL);
RKH_CREATE_TRANS_TABLE(s221)
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(s2211, smTest_nS2211, smTest_xS2211, &s221, NULL);
RKH_CREATE_TRANS_TABLE(s2211)
RKH_END_TRANS_TABLE

RKH_CREATE_COMP_STATE(s222, NULL, NULL, &s22, &s2221, NULL);
RKH_CREATE_TRANS_TABLE(s222)
RKH_END_TRANS_TABLE

RKH_CREATE_COMP_STATE(s2221, NULL, NULL, &s222, &s22211, NULL);
RKH_CREATE_TRANS_TABLE(s2221)
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(s22211, NULL, NULL, &s2221, NULL);
RKH_CREATE_TRANS_TABLE(s22211)
RKH_END_TRANS_TABLE

RKH_CREATE_COMP_REGION_STATE(s3, smTest_nS3, smTest_xS3, RKH_ROOT, &s31, 
                             smTest_iS3, RKH_NO_HISTORY, NULL, NULL, NULL, 
                             NULL);
RKH_CREATE_TRANS_TABLE(s3)

    RKH_TRINT(E, NULL, smTest_tr15),
    RKH_TRREG(A, NULL, smTest_tr51, &s3),
    RKH_TRREG(B, NULL, smTest_tr41, &s2),
    RKH_TRREG(C, NULL, smTest_tr42, &s22),

RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(s31, smTest_nS31, smTest_xS31, &s3, NULL);
RKH_CREATE_TRANS_TABLE(s31)

    RKH_TRREG(D, NULL, smTest_tr52, &s31),

RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(s4, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(s4)

    RKH_TRINT(A, smTest_guard4a, NULL),
    RKH_TRINT(A, smTest_guard4b, NULL),
    RKH_TRINT(A, smTest_guard4c, NULL),
    RKH_TRINT(B, smTest_guard4a, NULL),
    RKH_TRINT(B, smTest_guard4b, NULL),
    RKH_TRREG(B, NULL, NULL, &s4),

RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(s5, smTest_nS5, smTest_xS5, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(s5)

    RKH_TRCOMPLETION(NULL, smTest_tr61, &s4),

RKH_END_TRANS_TABLE

RKH_CREATE_FINAL_STATE(SmTest_Final, RKH_ROOT);

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
