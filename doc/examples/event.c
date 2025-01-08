/**
 *  \file       smPseudoConditionalTest.c
 *  \ingroup    Test
 *
 *  \brief      State machine to facilitate the test of production code.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2017.06.05  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "smPseudoConditionalTest.h"
#include "smPseudoConditionalTestAct.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/*
 *	Defines SMA (active object) 'smPseudoConditionalTest'.
 */
RKH_SMA_CREATE(SmPseudoConditionalTest, smPseudoConditionalTest, 0, HCAL, 
               &smPCT_waiting, NULL, NULL);
RKH_SMA_DEF_PTR(smPseudoConditionalTest);

/*
 *	Defines states and pseudostates.
 */
RKH_CREATE_BASIC_STATE(smPCT_waiting, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(smPCT_waiting)
    RKH_TRREG(A, NULL, NULL, &smPCT_s0),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(smPCT_s0, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(smPCT_s0)
    RKH_TRREG(A, NULL, smPCT_setCondition,  &smPCT_choice1),
    RKH_TRREG(B, NULL, smPCT_setCondition,  &smPCT_choice1),
    RKH_TRREG(C, NULL, NULL,                &smPCT_choice2),
    RKH_TRREG(D, NULL, NULL,                &smPCT_choice1),
    RKH_TRREG(E, NULL, smPCT_tr1,           &smPCT_choice3),
RKH_END_TRANS_TABLE

RKH_CREATE_CHOICE_STATE(smPCT_choice1);
RKH_CREATE_BRANCH_TABLE(smPCT_choice1)
    RKH_BRANCH(smPCT_onEventA,  smPCT_tr1,  &smPCT_s1),
    RKH_BRANCH(smPCT_onEventD,  smPCT_tr4,  &smPCT_s0),
    RKH_BRANCH(ELSE,            smPCT_tr2,  &smPCT_s12),
RKH_END_BRANCH_TABLE

RKH_CREATE_CHOICE_STATE(smPCT_choice2);
RKH_CREATE_BRANCH_TABLE(smPCT_choice2)
    RKH_BRANCH(smPCT_onEventA,  smPCT_tr3,  &smPCT_s2),
RKH_END_BRANCH_TABLE

RKH_CREATE_COMP_REGION_STATE(smPCT_s1, NULL, NULL, RKH_ROOT, &smPCT_s11, NULL, 
                             RKH_NO_HISTORY, NULL, NULL, NULL, 
                             NULL);
RKH_CREATE_TRANS_TABLE(smPCT_s1)
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(smPCT_s11, NULL, NULL, &smPCT_s1, NULL);
RKH_CREATE_TRANS_TABLE(smPCT_s11)
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(smPCT_s12, NULL, NULL, &smPCT_s1, NULL);
RKH_CREATE_TRANS_TABLE(smPCT_s12)
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(smPCT_s2, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(smPCT_s2)
RKH_END_TRANS_TABLE

RKH_CREATE_CHOICE_STATE(smPCT_choice3);
RKH_CREATE_BRANCH_TABLE(smPCT_choice3)
    RKH_BRANCH(smPCT_guardTrue, smPCT_tr1,   &smPCT_choice4),
RKH_END_BRANCH_TABLE

RKH_CREATE_CHOICE_STATE(smPCT_choice4);
RKH_CREATE_BRANCH_TABLE(smPCT_choice4)
    RKH_BRANCH(smPCT_guardTrue, smPCT_tr1,   &smPCT_choice5),
RKH_END_BRANCH_TABLE

RKH_CREATE_CHOICE_STATE(smPCT_choice5);
RKH_CREATE_BRANCH_TABLE(smPCT_choice5)
    RKH_BRANCH(smPCT_guardTrue, smPCT_tr1,   &smPCT_choice6),
RKH_END_BRANCH_TABLE

RKH_CREATE_CHOICE_STATE(smPCT_choice6);
RKH_CREATE_BRANCH_TABLE(smPCT_choice6)
    RKH_BRANCH(smPCT_guardTrue, smPCT_tr1,   &smPCT_s1),
RKH_END_BRANCH_TABLE

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
