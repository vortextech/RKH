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
 *  LeFr  Leandro Francucci  francuccilea@gmail.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */

#include "rkh.h"
#include "smTest.h"
#include "smTestAct.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */

/*
 *	Defines SMA (active object) 'smTest'.
 */
RKH_SMA_CREATE(SmTest, smTest, 0, HCAL, &waiting, smTest_init, NULL);

/*
 *	Defines states and pseudostates.
 */
RKH_CREATE_BASIC_STATE(waiting, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(waiting)
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(s0, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(s0)

    RKH_TRREG(A, NULL, NULL, &s1),
    RKH_TRREG(B, NULL, NULL, &s21),

RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(s1, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(s1)
RKH_END_TRANS_TABLE

RKH_CREATE_COMP_STATE(s2, NULL, NULL, RKH_ROOT, &s21, NULL);
RKH_CREATE_TRANS_TABLE(s2)

    RKH_TRREG(A, NULL, NULL, &s0),
    RKH_TRREG(D, NULL, NULL, &s21),

RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(s21, NULL, NULL, &s2, NULL);
RKH_CREATE_TRANS_TABLE(s21)

    RKH_TRREG(B, NULL, NULL, &s0),
    RKH_TRREG(C, NULL, NULL, &s2),

RKH_END_TRANS_TABLE
#if 0
RKH_CREATE_COMP_STATE(s, NULL, NULL, RKH_ROOT, &s11, NULL);
RKH_CREATE_TRANS_TABLE(s)

    RKH_TRINT(I, foo_isn_zero,  foo_set2zero ),
    RKH_TRREG(E, NULL,          NULL,           &s11),

RKH_END_TRANS_TABLE

RKH_CREATE_COMP_STATE(s1, NULL, NULL, &s, &s11, NULL);
RKH_CREATE_TRANS_TABLE(s1)

    RKH_TRINT(I, NULL,          NULL ),
    RKH_TRREG(A, NULL,          NULL,           &s1),
    RKH_TRREG(B, NULL,          NULL,           &s11),
    RKH_TRREG(C, NULL,          NULL,           &s2),
    RKH_TRREG(D, foo_is_zero,   foo_set2one,    &s),
    RKH_TRREG(F, NULL,          NULL,           &s211),

RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(s11, NULL, NULL, &s1, NULL);
RKH_CREATE_TRANS_TABLE(s11)

    RKH_TRREG(D, foo_isn_zero,  foo_set2zero,   &s1),
    RKH_TRREG(G, NULL,          NULL,           &s211),
    RKH_TRREG(H, NULL,          NULL,           &s),

RKH_END_TRANS_TABLE

RKH_CREATE_COMP_STATE(s2, NULL, NULL, &s, &s211, NULL);
RKH_CREATE_TRANS_TABLE(s2)

    RKH_TRINT(I, foo_is_zero,   foo_set2one ),
    RKH_TRREG(C, NULL,          NULL,           &s1),
    RKH_TRREG(F, NULL,          NULL,           &s11),

RKH_END_TRANS_TABLE

RKH_CREATE_COMP_STATE(s21, NULL, NULL, &s2, &s211, NULL);
RKH_CREATE_TRANS_TABLE(s21)

    RKH_TRREG(A, NULL,          NULL,           &s21),
    RKH_TRREG(B, NULL,          NULL,           &s211),
    RKH_TRREG(G, NULL,          NULL,           &s11),

RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(s211, NULL, NULL, &s21, NULL);
RKH_CREATE_TRANS_TABLE(s211)

    RKH_TRREG(D, NULL,          NULL,           &s21),
    RKH_TRREG(H, NULL,          NULL,           &s),

RKH_END_TRANS_TABLE
#endif

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
