/*
 *  --------------------------------------------------------------------------
 *
 *                                Framework RKH
 *                                -------------
 *
 *            State-machine framework for reactive embedded systems
 *
 *                      Copyright (C) 2010 Leandro Francucci.
 *          All rights reserved. Protected by international copyright laws.
 *
 *
 *  RKH is free software: you can redistribute it and/or modify it under the
 *  terms of the GNU General Public License as published by the Free Software
 *  Foundation, either version 3 of the License, or (at your option) any
 *  later version.
 *
 *  RKH is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with RKH, see copying.txt file.
 *
 *  Contact information:
 *  RKH site: http://vortexmakes.com/que-es/
 *  RKH GitHub: https://github.com/vortexmakes/RKH
 *  RKH Sourceforge: https://sourceforge.net/projects/rkh-reactivesys/
 *  e-mail: lf@vortexmakes.com
 *  ---------------------------------------------------------------------------
 */

/**
 *  \file       test_rkhsm.c
 *  \ingroup    test_sm
 *  \brief      Unit test for state machine module.
 *
 *  \addtogroup test
 *  @{
 *  \addtogroup test_sm State Machine
 *  @{
 *  \brief      Unit test for state machine module.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2016.12.15  LeFr  v2.4.05  ---
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <unitrazer.h>
#include <tzlink.h>
#include <tzparse.h>
#include "rkhsm.h"
#include "common.h"

#include "smTest.h"
#include "Mock_smTestAct.h"

#include "smPseudoConditionalTest.h"
#include "Mock_smPseudoConditionalTestAct.h"

#include "smPseudoTest.h"
#include "Mock_smPseudoTestAct.h"

#include "smInitialPseudoTest.h"
#include "Mock_smInitialPseudoTestAct.h"

#include "Mock_rkhassert.h"
#include "rkhport.h"
#include "rkhfwk_bittbl.h"
#include "rkhtrc.h"
#include "rkhtrc_filter.h"
#include "rkhtrc_record.h"
#include "rkhtrc_stream.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */

static RKH_STATIC_EVENT(evA, A);
static RKH_STATIC_EVENT(evB, B);
static RKH_STATIC_EVENT(evC, C);
static RKH_STATIC_EVENT(evD, D);
static RKH_STATIC_EVENT(evE, E);
static RKH_STATIC_EVENT(evF, F);
static RKH_STATIC_EVENT(evG, G);
static RKH_STATIC_EVENT(evH, H);
static RKH_STATIC_EVENT(evI, I);
static RKH_STATIC_EVENT(evTerminate, TERMINATE);
extern const RKH_EVT_T evCompletion;
extern const RKH_EVT_T evCreation;

/* ---------------------------- Local data types --------------------------- */
typedef struct StateMachine
{
    RKH_SM_T sm;
    int foo;
} StateMachine;

typedef struct Composite
{
    RKH_SMA_T ao;
    int foo;
    RKH_SM_T itsReactivePart;
} Composite;

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static const RKH_ST_T *targetStates[16], *exitStates[16], *entryStates[16];
static RKH_ST_T *expectedState;
static RKH_RCODE_T result;
static int first = 1;
int GlobalExpectCount;
int GlobalVerifyOrder;
char *GlobalOrderError;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static
void
setUpUnitrazer(void)
{
    if(first)
    {
        tzlink_open(0, NULL);
    }

    first = 0;

    sm_init();
    sm_ntrnact_ignore();
}

static
void
setUpWoutUnitrazer(void)
{
    sm_ignore();
}

static
void
loadStateMachineSymbols(void)
{
    RKH_TR_FWK_SIG(A);
    RKH_TR_FWK_SIG(B);
    RKH_TR_FWK_SIG(C);
    RKH_TR_FWK_SIG(D);
    RKH_TR_FWK_SIG(E);
    RKH_TR_FWK_SIG(F);

    RKH_TR_FWK_AO(smTest);
    RKH_TR_FWK_STATE(smTest, &waiting);
    RKH_TR_FWK_STATE(smTest, &s0);
    RKH_TR_FWK_STATE(smTest, &s1);
    RKH_TR_FWK_STATE(smTest, &s2);
    RKH_TR_FWK_STATE(smTest, &s21);
    RKH_TR_FWK_STATE(smTest, &s22);
    RKH_TR_FWK_STATE(smTest, &s221);
    RKH_TR_FWK_STATE(smTest, &s2211);
    RKH_TR_FWK_STATE(smTest, &s222);
    RKH_TR_FWK_STATE(smTest, &s2221);
    RKH_TR_FWK_STATE(smTest, &s22211);
    RKH_TR_FWK_STATE(smTest, &s3);
    RKH_TR_FWK_STATE(smTest, &s31);
    RKH_TR_FWK_STATE(smTest, &s4);
    RKH_TR_FWK_STATE(smTest, &s5);
    RKH_TR_FWK_STATE(smTest, &s2Final);
    RKH_TR_FWK_STATE(smTest, &SmTest_Final);

    RKH_TR_FWK_AO(smPseudoConditionalTest);
    RKH_TR_FWK_STATE(smPseudoConditionalTest, &smPCT_waiting);
    RKH_TR_FWK_STATE(smPseudoConditionalTest, &smPCT_choice1);
    RKH_TR_FWK_STATE(smPseudoConditionalTest, &smPCT_choice2);
    RKH_TR_FWK_STATE(smPseudoConditionalTest, &smPCT_choice3);
    RKH_TR_FWK_STATE(smPseudoConditionalTest, &smPCT_choice4);
    RKH_TR_FWK_STATE(smPseudoConditionalTest, &smPCT_choice5);
    RKH_TR_FWK_STATE(smPseudoConditionalTest, &smPCT_choice6);
    RKH_TR_FWK_STATE(smPseudoConditionalTest, &smPCT_s0);
    RKH_TR_FWK_STATE(smPseudoConditionalTest, &smPCT_s1);
    RKH_TR_FWK_STATE(smPseudoConditionalTest, &smPCT_s11);
    RKH_TR_FWK_STATE(smPseudoConditionalTest, &smPCT_s12);

    RKH_TR_FWK_AO(smPseudoTest);
    RKH_TR_FWK_STATE(smPseudoTest, &smPT_waiting);
    RKH_TR_FWK_STATE(smPseudoTest, &smPT_s0);
    RKH_TR_FWK_STATE(smPseudoTest, &smPT_s1);
    RKH_TR_FWK_STATE(smPseudoTest, &smPT_s11);
    RKH_TR_FWK_STATE(smPseudoTest, &smPT_s12);
    RKH_TR_FWK_STATE(smPseudoTest, &smPT_s1Hist);
    RKH_TR_FWK_STATE(smPseudoTest, &smPT_s121);
    RKH_TR_FWK_STATE(smPseudoTest, &smPT_s122);
    RKH_TR_FWK_STATE(smPseudoTest, &smPT_s12Hist);
    RKH_TR_FWK_STATE(smPseudoTest, &smPT_s2);
    RKH_TR_FWK_STATE(smPseudoTest, &smPT_s21);
    RKH_TR_FWK_STATE(smPseudoTest, &smPT_s22);
    RKH_TR_FWK_STATE(smPseudoTest, &smPT_s2Hist);

    RKH_TR_FWK_AO(smInitialPseudoTest);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s0);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s1);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s11);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s12);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s2);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s21);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s211);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s3);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s31);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s32);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s33);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s331);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s4);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s41);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s411);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s5);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s51);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s511);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s52);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s6);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s61);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s62);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s1Hist);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s2Hist);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s3Hist);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s7);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s71);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s72);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_choice1);
    RKH_TR_FWK_STATE(smInitialPseudoTest, &smIPT_s5Final);

    RKH_TR_FWK_AO(smInitial6);
    RKH_TR_FWK_STATE(smInitial6, &smI6_s0);
    RKH_TR_FWK_STATE(smInitial6, &smI6_s1);
}

static
void
setRKHTraceFilters(void)
{
    RKH_FILTER_OFF_ALL_SIGNALS();
    RKH_FILTER_OFF_GROUP_ALL_EVENTS(RKH_TG_SM);
    RKH_FILTER_OFF_SMA(smTest);
    RKH_FILTER_OFF_SMA(smPseudoConditionalTest);
    RKH_FILTER_OFF_SMA(smPseudoTest);
    RKH_FILTER_OFF_SMA(smInitialPseudoTest);
    RKH_FILTER_OFF_EVENT(RKH_TE_FWK_ASSERT);
}

static void 
MockEffectCallback0(SmTest *const me, RKH_EVT_T *pe, int cmock_num_calls)
{
    TEST_ASSERT_NULL(getPropagatedEvent(smTest));
    rkh_sm_propagate(RKH_UPCAST(RKH_SM_T, me), &evD);
    TEST_ASSERT_NOT_NULL(getPropagatedEvent(smTest));
}

static void 
MockEffectCallback1(SmTest *const me, RKH_EVT_T *pe, int cmock_num_calls)
{
    TEST_ASSERT_NULL(getPropagatedEvent(smTest));
}

/* ---------------------------- Global functions --------------------------- */
void 
setUp(void)
{
    setUpUnitrazer();
    
    Mock_smTestAct_Init();
    Mock_smPseudoConditionalTestAct_Init();
    Mock_smInitialPseudoTestAct_Init();

    loadStateMachineSymbols();
    setRKHTraceFilters();

    rkh_sm_clear_history(&smPT_s1Hist);
    rkh_sm_clear_history(&smPT_s12Hist);
    rkh_sm_clear_history(&smPT_s2Hist);
    rkh_sm_clear_history(&smIPT_s1Hist);
}

void 
tearDown(void)
{
    sm_verify(); /* Makes sure there are no unused expectations, if */
                 /* there are, this function causes the test to fail. */
    sm_cleanup();

    Mock_smTestAct_Verify();
    Mock_smTestAct_Destroy();

    Mock_smPseudoConditionalTestAct_Verify();
    Mock_smPseudoConditionalTestAct_Destroy();

    Mock_smInitialPseudoTestAct_Verify();
    Mock_smInitialPseudoTestAct_Destroy();
}

/**
 *  \addtogroup test_smTransition Transition test group
 *  @{
 *  \name Test cases of transition group
 *  @{ 
 */
void
test_transitionFirstStateAfterInit(void)
{
    UtrzProcessOut *p;

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    sm_init_expect(RKH_STATE_CAST(&waiting));
    sm_trn_expect(RKH_STATE_CAST(&waiting), RKH_STATE_CAST(&waiting));
    sm_tsState_expect(RKH_STATE_CAST(&waiting));
    sm_enstate_expect(RKH_STATE_CAST(&waiting));
    sm_nenex_expect(1, 0);
    sm_state_expect(RKH_STATE_CAST(&waiting));
    sm_evtProc_expect();

    rkh_sm_init((RKH_SM_T *)smTest);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_transitionSimpleToSimpleAtEqualLevel(void)
{
    UtrzProcessOut *p;

    stateList_create(targetStates, 1, &s1);
    stateList_create(entryStates, 1, &s1);
    stateList_create(exitStates, 1, &s0);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS0_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr11_Expect(RKH_CAST(SmTest, smTest), &evA);
    smTest_nS1_Expect(RKH_CAST(SmTest, smTest));

    setProfile(smTest, RKH_STATE_CAST(&waiting), 
               RKH_STATE_CAST(&s0), RKH_STATE_CAST(&s0), 
               targetStates,
               entryStates, exitStates, RKH_STATE_CAST(&s1), 1,
               TRN_NOT_INTERNAL, INIT_STATE_MACHINE, &evA, 
               RKH_STATE_CAST(&s0));

    rkh_sm_dispatch((RKH_SM_T *)smTest, &evA);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_transitionSimpleToSimpleFromHighToLowLevel(void)
{
    UtrzProcessOut *p;

    stateList_create(targetStates, 1, &s21);
    stateList_create(entryStates, 2, &s2, &s21);
    stateList_create(exitStates, 1, &s0);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS0_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr12_Expect(RKH_CAST(SmTest, smTest), &evB);
    smTest_nS2_Expect(RKH_CAST(SmTest, smTest));
    smTest_nS21_Expect(RKH_CAST(SmTest, smTest));

    setProfile(smTest, RKH_STATE_CAST(&waiting), 
               RKH_STATE_CAST(&s0), RKH_STATE_CAST(&s0), 
               targetStates, 
               entryStates, exitStates, RKH_STATE_CAST(&s21), 1, 
               TRN_NOT_INTERNAL, INIT_STATE_MACHINE, &evB, 
               RKH_STATE_CAST(&s0));

    rkh_sm_dispatch((RKH_SM_T *)smTest, &evB);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_transitionSimpleToSimpleFromLowToHighLevel(void)
{
    UtrzProcessOut *p;

    stateList_create(targetStates, 1, &s0);
    stateList_create(entryStates, 1, &s0);
    stateList_create(exitStates, 2, &s21, &s2);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS21_Expect(RKH_CAST(SmTest, smTest));
    smTest_xS2_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr13_Expect(RKH_CAST(SmTest, smTest), &evB);
    smTest_nS0_Expect(RKH_CAST(SmTest, smTest));

    setProfile(smTest, RKH_STATE_CAST(&waiting), 
               RKH_STATE_CAST(&s21), RKH_STATE_CAST(&s21), 
               targetStates, 
               entryStates, exitStates, RKH_STATE_CAST(&s0), 1, 
               TRN_NOT_INTERNAL, INIT_STATE_MACHINE, &evB, 
               RKH_STATE_CAST(&s21));

    rkh_sm_dispatch((RKH_SM_T *)smTest, &evB);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_transitionSimpleToCompositeAtEqualLevel(void)
{
    UtrzProcessOut *p;
    int nExSt, nEnSt;
    RKH_EVT_T event;

    nEnSt = 2;
    nExSt = 1;
    event = evC;

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS0_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr21_Expect(RKH_CAST(SmTest, smTest), &event);
    smTest_nS2_Expect(RKH_CAST(SmTest, smTest));
    smTest_iS2_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&event);
    smTest_nS21_Expect(RKH_CAST(SmTest, smTest));

    expInitSm((RKH_SMA_T *)smTest, RKH_STATE_CAST(&waiting));
	sm_dch_expect(event.e, RKH_STATE_CAST(&s0));
	sm_trn_expect(RKH_STATE_CAST(&s0), RKH_STATE_CAST(&s2));
    sm_tsState_expect(RKH_STATE_CAST(&s2));
    sm_exstate_expect(RKH_STATE_CAST(&s0));
    sm_enstate_expect(RKH_STATE_CAST(&s2));
    sm_tsState_expect(RKH_STATE_CAST(&s21));
    sm_enstate_expect(RKH_STATE_CAST(&s21));
    sm_nenex_expect(nEnSt, nExSt);
    sm_state_expect(RKH_STATE_CAST(&s21));
	sm_evtProc_expect();

    rkh_sm_init((RKH_SM_T *)smTest);
    setState(smTest, RKH_STATE_CAST(&s0));
    rkh_sm_dispatch((RKH_SM_T *)smTest, &event);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_transitionSimpleToCompositeFromHighToLowLevel(void)
{
    UtrzProcessOut *p;
    int nExSt, nEnSt;
    RKH_EVT_T event;

    nEnSt = 4;
    nExSt = 1;
    event = evD;

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS0_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr22_Expect(RKH_CAST(SmTest, smTest), &event);
    smTest_nS2_Expect(RKH_CAST(SmTest, smTest));
    smTest_nS22_Expect(RKH_CAST(SmTest, smTest));
    smTest_iS22_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&event);
    smTest_nS221_Expect(RKH_CAST(SmTest, smTest));
    smTest_iS221_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&event);
    smTest_nS2211_Expect(RKH_CAST(SmTest, smTest));

    expInitSm((RKH_SMA_T *)smTest, RKH_STATE_CAST(&waiting));
	sm_dch_expect(event.e, RKH_STATE_CAST(&s0));
	sm_trn_expect(RKH_STATE_CAST(&s0), RKH_STATE_CAST(&s22));
    sm_tsState_expect(RKH_STATE_CAST(&s22));
    sm_exstate_expect(RKH_STATE_CAST(&s0));
    sm_enstate_expect(RKH_STATE_CAST(&s2));
    sm_enstate_expect(RKH_STATE_CAST(&s22));
    sm_tsState_expect(RKH_STATE_CAST(&s221));
    sm_enstate_expect(RKH_STATE_CAST(&s221));
    sm_tsState_expect(RKH_STATE_CAST(&s2211));
    sm_enstate_expect(RKH_STATE_CAST(&s2211));
    sm_nenex_expect(nEnSt, nExSt);
    sm_state_expect(RKH_STATE_CAST(&s2211));
	sm_evtProc_expect();

    rkh_sm_init((RKH_SM_T *)smTest);
    setState(smTest, RKH_STATE_CAST(&s0));
    rkh_sm_dispatch((RKH_SM_T *)smTest, &event);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_transitionSimpleToCompositeFromLowToHighLevel(void)
{
    UtrzProcessOut *p;
    int nExSt, nEnSt;
    RKH_EVT_T event;

    nEnSt = 1;
    nExSt = 1;
    event = evC;

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS21_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr23_Expect(RKH_CAST(SmTest, smTest), &event);
    smTest_iS2_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&event);
    smTest_nS21_Expect(RKH_CAST(SmTest, smTest));

    expInitSm((RKH_SMA_T *)smTest, RKH_STATE_CAST(&waiting));
	sm_dch_expect(event.e, RKH_STATE_CAST(&s21));
	sm_trn_expect(RKH_STATE_CAST(&s21), RKH_STATE_CAST(&s2));
    sm_tsState_expect(RKH_STATE_CAST(&s2));
    sm_exstate_expect(RKH_STATE_CAST(&s21));
    sm_tsState_expect(RKH_STATE_CAST(&s21));
    sm_enstate_expect(RKH_STATE_CAST(&s21));
    sm_nenex_expect(nEnSt, nExSt);
    sm_state_expect(RKH_STATE_CAST(&s21));
	sm_evtProc_expect();

    rkh_sm_init((RKH_SM_T *)smTest);
    setState(smTest, RKH_STATE_CAST(&s21));
    rkh_sm_dispatch((RKH_SM_T *)smTest, &event);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_transitionCompositeToSimpleAtEqualLevel(void)
{
    UtrzProcessOut *p;

    stateList_create(targetStates, 1, &s0);
    stateList_create(entryStates, 1, &s0);
    stateList_create(exitStates, 2, &s21, &s2);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS21_Expect(RKH_CAST(SmTest, smTest));
    smTest_xS2_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr31_Expect(RKH_CAST(SmTest, smTest), &evA);
    smTest_nS0_Expect(RKH_CAST(SmTest, smTest));

    setProfile(smTest, RKH_STATE_CAST(&waiting), 
               RKH_STATE_CAST(&s21), RKH_STATE_CAST(&s2), 
               targetStates, 
               entryStates, exitStates, RKH_STATE_CAST(&s0), 1, 
               TRN_NOT_INTERNAL, INIT_STATE_MACHINE, &evA,
               RKH_STATE_CAST(&s21));

    rkh_sm_dispatch((RKH_SM_T *)smTest, &evA);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_transitionCompositeToSimpleFromHighToLowLevel(void)
{
    UtrzProcessOut *p;

    stateList_create(targetStates, 1, &s21);
    stateList_create(entryStates, 1, &s21);
    stateList_create(exitStates, 1, &s21);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS21_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr32_Expect(RKH_CAST(SmTest, smTest), &evD);
    smTest_nS21_Expect(RKH_CAST(SmTest, smTest));

    setProfile(smTest, RKH_STATE_CAST(&waiting), 
               RKH_STATE_CAST(&s21), RKH_STATE_CAST(&s2), 
               targetStates, 
               entryStates, exitStates, RKH_STATE_CAST(&s21), 1, 
               TRN_NOT_INTERNAL, INIT_STATE_MACHINE, &evD,
               RKH_STATE_CAST(&s21));

    rkh_sm_dispatch((RKH_SM_T *)smTest, &evD);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_transitionLoopSimpleStateOnTop(void)
{
    UtrzProcessOut *p;

    stateList_create(targetStates, 1, &s1);
    stateList_create(entryStates, 1, &s1);
    stateList_create(exitStates, 1, &s1);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS1_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr53_Expect(RKH_CAST(SmTest, smTest), &evA);
    smTest_nS1_Expect(RKH_CAST(SmTest, smTest));

    setProfile(smTest, RKH_STATE_CAST(&waiting), 
               RKH_STATE_CAST(&s1), RKH_STATE_CAST(&s1), 
               targetStates, 
               entryStates, exitStates, RKH_STATE_CAST(&s1), 1, 
               TRN_NOT_INTERNAL, INIT_STATE_MACHINE, &evA,
               RKH_STATE_CAST(&s1));

    rkh_sm_dispatch((RKH_SM_T *)smTest, &evA);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_transitionLoopNestedSimpleState(void)
{
    UtrzProcessOut *p;

    stateList_create(targetStates, 1, &s31);
    stateList_create(entryStates, 1, &s31);
    stateList_create(exitStates, 1, &s31);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS31_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr52_Expect(RKH_CAST(SmTest, smTest), &evD);
    smTest_nS31_Expect(RKH_CAST(SmTest, smTest));

    setProfile(smTest, RKH_STATE_CAST(&waiting), 
               RKH_STATE_CAST(&s31), RKH_STATE_CAST(&s31), 
               targetStates, entryStates, exitStates, 
               RKH_STATE_CAST(&s31), 1, TRN_NOT_INTERNAL, INIT_STATE_MACHINE,
               &evD, RKH_STATE_CAST(&s31));

    rkh_sm_dispatch((RKH_SM_T *)smTest, &evD);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_transitionLoopCompositeStateOnTop(void)
{
    UtrzProcessOut *p;
    int nExSt, nEnSt;
    RKH_EVT_T event;

    nEnSt = 1;
    nExSt = 1;
    event = evA;

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS31_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr51_Expect(RKH_CAST(SmTest, smTest), &event);
    smTest_iS3_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&event);
    smTest_nS31_Expect(RKH_CAST(SmTest, smTest));

    expInitSm((RKH_SMA_T *)smTest, RKH_STATE_CAST(&waiting));
	sm_dch_expect(event.e, RKH_STATE_CAST(&s31));
	sm_trn_expect(RKH_STATE_CAST(&s3), RKH_STATE_CAST(&s3));
    sm_tsState_expect(RKH_STATE_CAST(&s3));
    sm_exstate_expect(RKH_STATE_CAST(&s31));
    sm_tsState_expect(RKH_STATE_CAST(&s31));
    sm_enstate_expect(RKH_STATE_CAST(&s31));
    sm_nenex_expect(nEnSt, nExSt);
    sm_state_expect(RKH_STATE_CAST(&s31));
	sm_evtProc_expect();

    rkh_sm_init((RKH_SM_T *)smTest);
    setState(smTest, RKH_STATE_CAST(&s31));
    rkh_sm_dispatch((RKH_SM_T *)smTest, &event);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_transitionLoopNestedCompositeState(void)
{
    UtrzProcessOut *p;
    int nExSt, nEnSt;
    RKH_EVT_T event;

    nEnSt = 2;
    nExSt = 2;
    event = evD;

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS2211_Expect(RKH_CAST(SmTest, smTest));
    smTest_xS221_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr54_Expect(RKH_CAST(SmTest, smTest), &event);
    smTest_iS22_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&event);
    smTest_nS221_Expect(RKH_CAST(SmTest, smTest));
    smTest_iS221_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&event);
    smTest_nS2211_Expect(RKH_CAST(SmTest, smTest));

    expInitSm((RKH_SMA_T *)smTest, RKH_STATE_CAST(&waiting));
	sm_dch_expect(event.e, RKH_STATE_CAST(&s2211));
	sm_trn_expect(RKH_STATE_CAST(&s22), RKH_STATE_CAST(&s22));
    sm_tsState_expect(RKH_STATE_CAST(&s22));
    sm_exstate_expect(RKH_STATE_CAST(&s2211));
    sm_exstate_expect(RKH_STATE_CAST(&s221));
    sm_tsState_expect(RKH_STATE_CAST(&s221));
    sm_enstate_expect(RKH_STATE_CAST(&s221));
    sm_tsState_expect(RKH_STATE_CAST(&s2211));
    sm_enstate_expect(RKH_STATE_CAST(&s2211));
    sm_nenex_expect(nEnSt, nExSt);
    sm_state_expect(RKH_STATE_CAST(&s2211));
	sm_evtProc_expect();

    rkh_sm_init((RKH_SM_T *)smTest);
    setState(smTest, RKH_STATE_CAST(&s2211));
    rkh_sm_dispatch((RKH_SM_T *)smTest, &event);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_transitionCompositeToSimpleFromLowToHighLevel(void)
{
    UtrzProcessOut *p;

    stateList_create(targetStates, 1, &s0);
    stateList_create(entryStates, 1, &s0);
    stateList_create(exitStates, 3, &s221, &s22, &s2);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS221_Expect(RKH_CAST(SmTest, smTest));
    smTest_xS22_Expect(RKH_CAST(SmTest, smTest));
    smTest_xS2_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr33_Expect(RKH_CAST(SmTest, smTest), &evB);
    smTest_nS0_Expect(RKH_CAST(SmTest, smTest));

    setProfile(smTest, RKH_STATE_CAST(&waiting),
               RKH_STATE_CAST(&s221), RKH_STATE_CAST(&s22), 
               targetStates, entryStates, exitStates, 
               RKH_STATE_CAST(&s0), 1, TRN_NOT_INTERNAL, INIT_STATE_MACHINE,
               &evB, RKH_STATE_CAST(&s221));

    rkh_sm_dispatch((RKH_SM_T *)smTest, &evB);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_transitionCompositeToCompositeAtEqualLevel(void)
{
    UtrzProcessOut *p;
    int nExSt, nEnSt;
    RKH_EVT_T event;

    nEnSt = 2;
    nExSt = 2;
    event = evB;

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS31_Expect(RKH_CAST(SmTest, smTest));
    smTest_xS3_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr41_Expect(RKH_CAST(SmTest, smTest), &event);
    smTest_nS2_Expect(RKH_CAST(SmTest, smTest));
    smTest_iS2_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&event);
    smTest_nS21_Expect(RKH_CAST(SmTest, smTest));

    expInitSm((RKH_SMA_T *)smTest, RKH_STATE_CAST(&waiting));
	sm_dch_expect(event.e, RKH_STATE_CAST(&s31));
	sm_trn_expect(RKH_STATE_CAST(&s3), RKH_STATE_CAST(&s2));
    sm_tsState_expect(RKH_STATE_CAST(&s2));
    sm_exstate_expect(RKH_STATE_CAST(&s31));
    sm_exstate_expect(RKH_STATE_CAST(&s3));
    sm_enstate_expect(RKH_STATE_CAST(&s2));
    sm_tsState_expect(RKH_STATE_CAST(&s21));
    sm_enstate_expect(RKH_STATE_CAST(&s21));
    sm_nenex_expect(nEnSt, nExSt);
    sm_state_expect(RKH_STATE_CAST(&s21));
	sm_evtProc_expect();

    rkh_sm_init((RKH_SM_T *)smTest);
    setState(smTest, RKH_STATE_CAST(&s31));
    rkh_sm_dispatch((RKH_SM_T *)smTest, &event);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_transitionCompositeToCompositeFromHighToLowLevel(void)
{
    UtrzProcessOut *p;
    int nExSt, nEnSt;
    RKH_EVT_T event;

    nEnSt = 4;
    nExSt = 2;
    event = evC;

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS31_Expect(RKH_CAST(SmTest, smTest));
    smTest_xS3_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr42_Expect(RKH_CAST(SmTest, smTest), &event);
    smTest_nS2_Expect(RKH_CAST(SmTest, smTest));
    smTest_nS22_Expect(RKH_CAST(SmTest, smTest));
    smTest_iS22_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&event);
    smTest_nS221_Expect(RKH_CAST(SmTest, smTest));
    smTest_iS221_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&event);
    smTest_nS2211_Expect(RKH_CAST(SmTest, smTest));

    expInitSm((RKH_SMA_T *)smTest, RKH_STATE_CAST(&waiting));
	sm_dch_expect(event.e, RKH_STATE_CAST(&s31));
	sm_trn_expect(RKH_STATE_CAST(&s3), RKH_STATE_CAST(&s22));
    sm_tsState_expect(RKH_STATE_CAST(&s22));
    sm_exstate_expect(RKH_STATE_CAST(&s31));
    sm_exstate_expect(RKH_STATE_CAST(&s3));
    sm_enstate_expect(RKH_STATE_CAST(&s2));
    sm_enstate_expect(RKH_STATE_CAST(&s22));
    sm_tsState_expect(RKH_STATE_CAST(&s221));
    sm_enstate_expect(RKH_STATE_CAST(&s221));
    sm_tsState_expect(RKH_STATE_CAST(&s2211));
    sm_enstate_expect(RKH_STATE_CAST(&s2211));
    sm_nenex_expect(nEnSt, nExSt);
    sm_state_expect(RKH_STATE_CAST(&s2211));
	sm_evtProc_expect();

    rkh_sm_init((RKH_SM_T *)smTest);
    setState(smTest, RKH_STATE_CAST(&s31));
    rkh_sm_dispatch((RKH_SM_T *)smTest, &event);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_transitionCompositeToCompositeFromLowToHighLevel(void)
{
    UtrzProcessOut *p;
    int nExSt, nEnSt;
    RKH_EVT_T event;

    nEnSt = 2;
    nExSt = 4;
    event = evC;

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS2211_Expect(RKH_CAST(SmTest, smTest));
    smTest_xS221_Expect(RKH_CAST(SmTest, smTest));
    smTest_xS22_Expect(RKH_CAST(SmTest, smTest));
    smTest_xS2_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr43_Expect(RKH_CAST(SmTest, smTest), &event);
    smTest_nS3_Expect(RKH_CAST(SmTest, smTest));
    smTest_iS3_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&event);
    smTest_nS31_Expect(RKH_CAST(SmTest, smTest));

    expInitSm((RKH_SMA_T *)smTest, RKH_STATE_CAST(&waiting));
	sm_dch_expect(event.e, RKH_STATE_CAST(&s2211));
	sm_trn_expect(RKH_STATE_CAST(&s22), RKH_STATE_CAST(&s3));
    sm_tsState_expect(RKH_STATE_CAST(&s3));
    sm_exstate_expect(RKH_STATE_CAST(&s2211));
    sm_exstate_expect(RKH_STATE_CAST(&s221));
    sm_exstate_expect(RKH_STATE_CAST(&s22));
    sm_exstate_expect(RKH_STATE_CAST(&s2));
    sm_enstate_expect(RKH_STATE_CAST(&s3));
    sm_tsState_expect(RKH_STATE_CAST(&s31));
    sm_enstate_expect(RKH_STATE_CAST(&s31));
    sm_nenex_expect(nEnSt, nExSt);
    sm_state_expect(RKH_STATE_CAST(&s31));
	sm_evtProc_expect();

    rkh_sm_init((RKH_SM_T *)smTest);
    setState(smTest, RKH_STATE_CAST(&s2211));
    rkh_sm_dispatch((RKH_SM_T *)smTest, &event);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_transitionInternalInSimpleState(void)
{
    UtrzProcessOut *p;

    stateList_create(targetStates, 1, &s1);
    stateList_create(entryStates, 0);
    stateList_create(exitStates, 0);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_tr14_Expect(RKH_CAST(SmTest, smTest), &evB);

    setProfile(smTest, RKH_STATE_CAST(&waiting), 
               RKH_STATE_CAST(&s1), RKH_STATE_CAST(&s1), 
               targetStates, entryStates, exitStates, 
               RKH_STATE_CAST(&s1), 1, TRN_INTERNAL, INIT_STATE_MACHINE, 
               &evB, RKH_STATE_CAST(&s1));

    rkh_sm_dispatch((RKH_SM_T *)smTest, &evB);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_transitionInternalInCompositeState(void)
{
    UtrzProcessOut *p;

    stateList_create(targetStates, 1, &s3);
    stateList_create(entryStates, 0);
    stateList_create(exitStates, 0);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_tr15_Expect(RKH_CAST(SmTest, smTest), &evE);

    setProfile(smTest, RKH_STATE_CAST(&waiting), 
               RKH_STATE_CAST(&s31), RKH_STATE_CAST(&s3), 
               targetStates, entryStates, exitStates, 
               RKH_STATE_CAST(&s31), 1, TRN_INTERNAL, INIT_STATE_MACHINE,
               &evE, RKH_STATE_CAST(&s31));

    rkh_sm_dispatch((RKH_SM_T *)smTest, &evE);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_transitionFails_EventNotFound(void)
{
    UtrzProcessOut *p;

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);

    expInitSm(smTest, RKH_STATE_CAST(&waiting));
	sm_dch_expect(evE.e, RKH_STATE_CAST(&s1));
	sm_evtNotFound_expect(E);

    rkh_sm_init((RKH_SM_T *)smTest);
    setState(smTest, RKH_STATE_CAST(&s1));
    rkh_sm_dispatch((RKH_SM_T *)smTest, &evE);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_transitionFails_GuardFalse(void)
{
    UtrzProcessOut *p;

    expInitSm(smTest, RKH_STATE_CAST(&waiting));
	sm_grdFalse_expect();
	sm_dch_expect(evC.e, RKH_STATE_CAST(&s1));
    sm_evtNotFound_expect(C);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_falseGuard_ExpectAndReturn(RKH_CAST(SmTest, smTest), &evC, 
                                      RKH_FALSE);

    rkh_sm_init((RKH_SM_T *)smTest);
    setState(smTest, RKH_STATE_CAST(&s1));
    rkh_sm_dispatch((RKH_SM_T *)smTest, &evC);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_transitionFails_ExceededHierarchicalLevel(void)
{
    UtrzProcessOut *p;

    expInitSm(smTest, RKH_STATE_CAST(&waiting));
    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
	sm_dch_expect(evE.e, RKH_STATE_CAST(&s0));
	sm_trn_expect(RKH_STATE_CAST(&s0), RKH_STATE_CAST(&s22211));
    sm_tsState_expect(RKH_STATE_CAST(&s22211));
	sm_exHLevel_expect();
    rkh_assert_Expect("rkhsm", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();

    rkh_sm_init((RKH_SM_T *)smTest);
    setState(smTest, RKH_STATE_CAST(&s0));
    rkh_sm_dispatch((RKH_SM_T *)smTest, &evE);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_transitionMultipleEnabledTrn_FiringFirstTrueGuard(void)
{
    UtrzProcessOut *p;

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_guard4a_ExpectAndReturn(RKH_CAST(SmTest, smTest), &evA, RKH_FALSE);
    smTest_guard4b_ExpectAndReturn(RKH_CAST(SmTest, smTest), &evA, RKH_TRUE);

	sm_init_ignore();
	sm_tsState_ignore();
	sm_nenex_ignore();
    sm_state_ignore();
	sm_enstate_ignore();
	sm_ntrnact_ignore();
	sm_dch_ignore();
	sm_trn_ignore();
	sm_evtProc_ignore();
    sm_grdFalse_expect();

    rkh_sm_init((RKH_SM_T *)smTest);
    setState(smTest, RKH_STATE_CAST(&s4));
    rkh_sm_dispatch((RKH_SM_T *)smTest, &evA);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_transitionMultipleEnabledTrn_FiringFirstEmptyGuard(void)
{
    UtrzProcessOut *p;

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_guard4a_ExpectAndReturn(RKH_CAST(SmTest, smTest), &evB, RKH_FALSE);
    smTest_guard4b_ExpectAndReturn(RKH_CAST(SmTest, smTest), &evB, RKH_FALSE);

	sm_init_ignore();
	sm_nenex_ignore();
    sm_state_ignore();
	sm_enstate_ignore();
	sm_exstate_ignore();
	sm_ntrnact_ignore();
	sm_evtProc_ignore();
	sm_trn_ignore();
    sm_nenex_ignore();
    sm_state_ignore();
    sm_dch_ignore();
    sm_tsState_expect(RKH_STATE_CAST(&waiting));
    sm_grdFalse_expect();
    sm_grdFalse_expect();
    sm_tsState_expect(RKH_STATE_CAST(&s4));

    rkh_sm_init((RKH_SM_T *)smTest);
    setState(smTest, RKH_STATE_CAST(&s4));
    rkh_sm_dispatch((RKH_SM_T *)smTest, &evB);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_transitionDefaultTrnWithAssociatedEffect(void)
{
    UtrzProcessOut *p;
    int nExSt, nEnSt;
    RKH_EVT_T event;

    nEnSt = 2;
    nExSt = 1;
    event = evF;

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS0_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr55_Expect(RKH_CAST(SmTest, smTest), &event);
    smTest_nS3_Expect(RKH_CAST(SmTest, smTest));
    smTest_iS3_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&event);
    smTest_nS31_Expect(RKH_CAST(SmTest, smTest));

    expInitSm(smTest, RKH_STATE_CAST(&waiting));
	sm_dch_expect(event.e, RKH_STATE_CAST(&s0));
	sm_trn_expect(RKH_STATE_CAST(&s0), RKH_STATE_CAST(&s3));
    sm_tsState_expect(RKH_STATE_CAST(&s3));
    sm_exstate_expect(RKH_STATE_CAST(&s0));
    sm_enstate_expect(RKH_STATE_CAST(&s3));
    sm_tsState_expect(RKH_STATE_CAST(&s31));
    sm_enstate_expect(RKH_STATE_CAST(&s31));
    sm_nenex_expect(nEnSt, nExSt);
    sm_state_expect(RKH_STATE_CAST(&s31));
	sm_evtProc_expect();

    rkh_sm_init((RKH_SM_T *)smTest);
    setState(smTest, RKH_STATE_CAST(&s0));
    rkh_sm_dispatch((RKH_SM_T *)smTest, &event);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_transitionGeneratedCompletionEventBySimpleState(void)
{
    UtrzProcessOut *p;

    stateList_create(targetStates, 1, &s5);
    stateList_create(entryStates, 1, &s5);
    stateList_create(exitStates, 2, &s21, &s2);

    /* Expectations for transition to s5 */
    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS21_Expect(RKH_CAST(SmTest, smTest));
    smTest_xS2_Expect(RKH_CAST(SmTest, smTest));
    smTest_nS5_Expect(RKH_CAST(SmTest, smTest));

    setProfile(smTest, RKH_STATE_CAST(&waiting), 
               RKH_STATE_CAST(&s21), RKH_STATE_CAST(&s2), 
               targetStates, entryStates, exitStates, 
               RKH_STATE_CAST(&s5), 0, TRN_NOT_INTERNAL, INIT_STATE_MACHINE,
               &evE, RKH_STATE_CAST(&s21));

    /* Expectations for completion transition */
    stateList_create(targetStates, 1, &s4);
    stateList_create(entryStates, 1, &s4);
    stateList_create(exitStates, 1, &s5);

    smTest_xS5_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr61_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCompletion);

    setProfile(smTest, RKH_STATE_CAST(&waiting), 
               NULL, RKH_STATE_CAST(&s5), 
               targetStates, entryStates, exitStates, 
               RKH_STATE_CAST(&s4), 1, TRN_NOT_INTERNAL, 
               NO_INIT_STATE_MACHINE, &evCompletion, RKH_STATE_CAST(&s5));

    /* Exercices */
    rkh_sm_dispatch((RKH_SM_T *)smTest, &evE);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_transitionGeneratedCompletionEventByFinalState(void)
{
    UtrzProcessOut *p;

    stateList_create(targetStates, 1, &s2Final);
    stateList_create(entryStates, 1, &s2Final);
    stateList_create(exitStates, 1, &s21);

    /* Expectations for transition to S2's FinalState */
    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS21_Expect(RKH_CAST(SmTest, smTest));

    setProfile(smTest, RKH_STATE_CAST(&waiting), 
               RKH_STATE_CAST(&s21), RKH_STATE_CAST(&s21), 
               targetStates, entryStates, exitStates, 
               RKH_STATE_CAST(&s2Final), 0, TRN_NOT_INTERNAL, 
               INIT_STATE_MACHINE, &evF, RKH_STATE_CAST(&s21));

    /* Expectations for completion transition */
    stateList_create(targetStates, 1, &s4);
    stateList_create(entryStates, 1, &s4);
    stateList_create(exitStates, 2, &s2Final, &s2);

    smTest_guardS2_ExpectAndReturn(RKH_CAST(SmTest, smTest), 
                                   (RKH_EVT_T *)&evCompletion, 
                                   RKH_TRUE);
    smTest_xS2_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr60_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCompletion);

    setProfile(smTest, RKH_STATE_CAST(&waiting), 
               NULL, RKH_STATE_CAST(&s2), 
               targetStates, entryStates, exitStates, 
               RKH_STATE_CAST(&s4), 1, TRN_NOT_INTERNAL, 
               NO_INIT_STATE_MACHINE, &evCompletion, RKH_STATE_CAST(&s2Final));

    /* Exercices */
    rkh_sm_dispatch((RKH_SM_T *)smTest, &evF);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_transitionSyncDispatchingToStateMachine(void)
{
    TEST_IGNORE();
}

void
test_transitionToStateMachineFinalState(void)
{
    UtrzProcessOut *p;

    stateList_create(targetStates, 1, &SmTest_Final);
    stateList_create(entryStates, 1, &SmTest_Final);
    stateList_create(exitStates, 1, &waiting);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);

    setProfile(smTest, RKH_STATE_CAST(&waiting),
               RKH_STATE_CAST(&waiting), RKH_STATE_CAST(&waiting), 
               targetStates, entryStates, exitStates, 
               RKH_STATE_CAST(&SmTest_Final), 1, TRN_NOT_INTERNAL, 
               INIT_STATE_MACHINE,
               &evB, RKH_STATE_CAST(&waiting));

    rkh_sm_dispatch((RKH_SM_T *)smTest, &evB);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */

/**
 *  \addtogroup test_smTransitionWoutUnitrazer Transition test group (wout Ut)
 *  @{
 *  \name Test cases of trnWoutUnitrazer group
 *  @{ 
 */

void
test_trnWoutUnitrazerFirstStateAfterInit(void)
{
    setUpWoutUnitrazer();

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    expectedState = RKH_STATE_CAST(&waiting);

    rkh_sm_init((RKH_SM_T *)smTest);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
}

void
test_trnWoutUnitrazerSimpleToSimpleAtEqualLevel(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&s1);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS0_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr11_Expect(RKH_CAST(SmTest, smTest), &evA);
    smTest_nS1_Expect(RKH_CAST(SmTest, smTest));
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&s0),
                            RKH_STATE_CAST(&s0),
                            expectedState,
                            INIT_STATE_MACHINE);

    result = rkh_sm_dispatch((RKH_SM_T *)smTest, &evA);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
    TEST_ASSERT_EQUAL(RKH_EVT_PROC, result);
}

void
test_trnWoutUnitrazerSimpleToSimpleFromHighToLowLevel(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&s21);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS0_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr12_Expect(RKH_CAST(SmTest, smTest), &evB);
    smTest_nS2_Expect(RKH_CAST(SmTest, smTest));
    smTest_nS21_Expect(RKH_CAST(SmTest, smTest));
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&s0),
                            RKH_STATE_CAST(&s0),
                            expectedState,
                            INIT_STATE_MACHINE);

    result = rkh_sm_dispatch((RKH_SM_T *)smTest, &evB);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
    TEST_ASSERT_EQUAL(RKH_EVT_PROC, result);
}

void
test_trnWoutUnitrazerSimpleToSimpleFromLowToHighLevel(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&s0);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS21_Expect(RKH_CAST(SmTest, smTest));
    smTest_xS2_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr13_Expect(RKH_CAST(SmTest, smTest), &evB);
    smTest_nS0_Expect(RKH_CAST(SmTest, smTest));
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&s21),
                            RKH_STATE_CAST(&s21),
                            expectedState,
                            INIT_STATE_MACHINE);

    result = rkh_sm_dispatch((RKH_SM_T *)smTest, &evB);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
    TEST_ASSERT_EQUAL(RKH_EVT_PROC, result);
}

void
test_trnWoutUnitrazerSimpleToCompositeAtEqualLevel(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&s21);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS0_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr21_Expect(RKH_CAST(SmTest, smTest), &evC);
    smTest_nS2_Expect(RKH_CAST(SmTest, smTest));
    smTest_iS2_Expect(RKH_CAST(SmTest, smTest), &evC);
    smTest_nS21_Expect(RKH_CAST(SmTest, smTest));
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&s0),
                            RKH_STATE_CAST(&s0),
                            expectedState,
                            INIT_STATE_MACHINE);

    result = rkh_sm_dispatch((RKH_SM_T *)smTest, &evC);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
    TEST_ASSERT_EQUAL(RKH_EVT_PROC, result);
}

void
test_trnWoutUnitrazerSimpleToCompositeFromHighToLowLevel(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&s2211);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS0_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr22_Expect(RKH_CAST(SmTest, smTest), &evD);
    smTest_nS2_Expect(RKH_CAST(SmTest, smTest));
    smTest_nS22_Expect(RKH_CAST(SmTest, smTest));
    smTest_iS22_Expect(RKH_CAST(SmTest, smTest), &evD);
    smTest_nS221_Expect(RKH_CAST(SmTest, smTest));
    smTest_iS221_Expect(RKH_CAST(SmTest, smTest), &evD);
    smTest_nS2211_Expect(RKH_CAST(SmTest, smTest));
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&s0),
                            RKH_STATE_CAST(&s0),
                            expectedState,
                            INIT_STATE_MACHINE);

    result = rkh_sm_dispatch((RKH_SM_T *)smTest, &evD);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
    TEST_ASSERT_EQUAL(RKH_EVT_PROC, result);
}

void
test_trnWoutUnitrazerSimpleToCompositeFromLowToHighLevel(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&s21);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS21_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr23_Expect(RKH_CAST(SmTest, smTest), &evC);
    smTest_iS2_Expect(RKH_CAST(SmTest, smTest), &evC);
    smTest_nS21_Expect(RKH_CAST(SmTest, smTest));
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&s21),
                            RKH_STATE_CAST(&s21),
                            expectedState,
                            INIT_STATE_MACHINE);

    result = rkh_sm_dispatch((RKH_SM_T *)smTest, &evC);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
    TEST_ASSERT_EQUAL(RKH_EVT_PROC, result);
}

void
test_trnWoutUnitrazerCompositeToSimpleAtEqualLevel(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&s0);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS21_Expect(RKH_CAST(SmTest, smTest));
    smTest_xS2_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr31_Expect(RKH_CAST(SmTest, smTest), &evA);
    smTest_nS0_Expect(RKH_CAST(SmTest, smTest));
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&s21),
                            RKH_STATE_CAST(&s2),
                            expectedState,
                            INIT_STATE_MACHINE);

    result = rkh_sm_dispatch((RKH_SM_T *)smTest, &evA);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
    TEST_ASSERT_EQUAL(RKH_EVT_PROC, result);
}

void
test_trnWoutUnitrazerCompositeToSimpleFromHighToLowLevel(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&s21);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS21_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr32_Expect(RKH_CAST(SmTest, smTest), &evD);
    smTest_nS21_Expect(RKH_CAST(SmTest, smTest));
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&s21),
                            RKH_STATE_CAST(&s2),
                            expectedState,
                            INIT_STATE_MACHINE);

    result = rkh_sm_dispatch((RKH_SM_T *)smTest, &evD);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
    TEST_ASSERT_EQUAL(RKH_EVT_PROC, result);
}

void
test_trnWoutUnitrazerLoopSimpleStateOnTop(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&s1);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS1_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr53_Expect(RKH_CAST(SmTest, smTest), &evA);
    smTest_nS1_Expect(RKH_CAST(SmTest, smTest));
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&s1),
                            RKH_STATE_CAST(&s1),
                            expectedState,
                            INIT_STATE_MACHINE);

    result = rkh_sm_dispatch((RKH_SM_T *)smTest, &evA);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
    TEST_ASSERT_EQUAL(RKH_EVT_PROC, result);
}

void
test_trnWoutUnitrazerLoopNestedSimpleState(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&s31);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS31_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr52_Expect(RKH_CAST(SmTest, smTest), &evD);
    smTest_nS31_Expect(RKH_CAST(SmTest, smTest));
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&s31),
                            RKH_STATE_CAST(&s31),
                            expectedState,
                            INIT_STATE_MACHINE);

    result = rkh_sm_dispatch((RKH_SM_T *)smTest, &evD);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
    TEST_ASSERT_EQUAL(RKH_EVT_PROC, result);
}

void
test_trnWoutUnitrazerLoopCompositeStateOnTop(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&s31);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS31_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr51_Expect(RKH_CAST(SmTest, smTest), &evA);
    smTest_iS3_Expect(RKH_CAST(SmTest, smTest), &evA);
    smTest_nS31_Expect(RKH_CAST(SmTest, smTest));
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&s31),
                            RKH_STATE_CAST(&s3),
                            expectedState,
                            INIT_STATE_MACHINE);

    result = rkh_sm_dispatch((RKH_SM_T *)smTest, &evA);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
    TEST_ASSERT_EQUAL(RKH_EVT_PROC, result);
}

void
test_trnWoutUnitrazerLoopNestedCompositeState(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&s2211);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS2211_Expect(RKH_CAST(SmTest, smTest));
    smTest_xS221_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr54_Expect(RKH_CAST(SmTest, smTest), &evD);
    smTest_iS22_Expect(RKH_CAST(SmTest, smTest), &evD);
    smTest_nS221_Expect(RKH_CAST(SmTest, smTest));
    smTest_iS221_Expect(RKH_CAST(SmTest, smTest), &evD);
    smTest_nS2211_Expect(RKH_CAST(SmTest, smTest));
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&s2211),
                            RKH_STATE_CAST(&s22),
                            expectedState,
                            INIT_STATE_MACHINE);

    result = rkh_sm_dispatch((RKH_SM_T *)smTest, &evD);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
    TEST_ASSERT_EQUAL(RKH_EVT_PROC, result);
}

void
test_trnWoutUnitrazerCompositeToSimpleFromLowToHighLevel(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&s0);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS2211_Expect(RKH_CAST(SmTest, smTest));
    smTest_xS221_Expect(RKH_CAST(SmTest, smTest));
    smTest_xS22_Expect(RKH_CAST(SmTest, smTest));
    smTest_xS2_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr33_Expect(RKH_CAST(SmTest, smTest), &evB);
    smTest_nS0_Expect(RKH_CAST(SmTest, smTest));
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&s2211),
                            RKH_STATE_CAST(&s22),
                            expectedState,
                            INIT_STATE_MACHINE);

    result = rkh_sm_dispatch((RKH_SM_T *)smTest, &evB);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
    TEST_ASSERT_EQUAL(RKH_EVT_PROC, result);
}

void
test_trnWoutUnitrazerCompositeToCompositeAtEqualLevel(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&s21);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS31_Expect(RKH_CAST(SmTest, smTest));
    smTest_xS3_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr41_Expect(RKH_CAST(SmTest, smTest), &evB);
    smTest_nS2_Expect(RKH_CAST(SmTest, smTest));
    smTest_iS2_Expect(RKH_CAST(SmTest, smTest), &evB);
    smTest_nS21_Expect(RKH_CAST(SmTest, smTest));
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&s31),
                            RKH_STATE_CAST(&s3),
                            expectedState,
                            INIT_STATE_MACHINE);

    result = rkh_sm_dispatch((RKH_SM_T *)smTest, &evB);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
    TEST_ASSERT_EQUAL(RKH_EVT_PROC, result);
}

void
test_trnWoutUnitrazerCompositeToCompositeFromHighToLowLevel(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&s2211);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS31_Expect(RKH_CAST(SmTest, smTest));
    smTest_xS3_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr42_Expect(RKH_CAST(SmTest, smTest), &evC);
    smTest_nS2_Expect(RKH_CAST(SmTest, smTest));
    smTest_nS22_Expect(RKH_CAST(SmTest, smTest));
    smTest_iS22_Expect(RKH_CAST(SmTest, smTest), &evC);
    smTest_nS221_Expect(RKH_CAST(SmTest, smTest));
    smTest_iS221_Expect(RKH_CAST(SmTest, smTest), &evC);
    smTest_nS2211_Expect(RKH_CAST(SmTest, smTest));
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&s31),
                            RKH_STATE_CAST(&s3),
                            expectedState,
                            INIT_STATE_MACHINE);

    result = rkh_sm_dispatch((RKH_SM_T *)smTest, &evC);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
    TEST_ASSERT_EQUAL(RKH_EVT_PROC, result);
}

void
test_trnWoutUnitrazerCompositeToCompositeFromLowToHighLevel(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&s31);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS2211_Expect(RKH_CAST(SmTest, smTest));
    smTest_xS221_Expect(RKH_CAST(SmTest, smTest));
    smTest_xS22_Expect(RKH_CAST(SmTest, smTest));
    smTest_xS2_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr43_Expect(RKH_CAST(SmTest, smTest), &evC);
    smTest_nS3_Expect(RKH_CAST(SmTest, smTest));
    smTest_iS3_Expect(RKH_CAST(SmTest, smTest), &evC);
    smTest_nS31_Expect(RKH_CAST(SmTest, smTest));
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&s2211),
                            RKH_STATE_CAST(&s3),
                            expectedState,
                            INIT_STATE_MACHINE);

    result = rkh_sm_dispatch((RKH_SM_T *)smTest, &evC);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
    TEST_ASSERT_EQUAL(RKH_EVT_PROC, result);
}

void
test_trnWoutUnitrazerInternalInSimpleState(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&s1);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_tr14_Expect(RKH_CAST(SmTest, smTest), &evB);
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&s1),
                            RKH_STATE_CAST(&s1),
                            expectedState,
                            INIT_STATE_MACHINE);

    result = rkh_sm_dispatch((RKH_SM_T *)smTest, &evB);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
    TEST_ASSERT_EQUAL(RKH_EVT_PROC, result);
}

void
test_trnWoutUnitrazerInternalInCompositeState(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&s31);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_tr15_Expect(RKH_CAST(SmTest, smTest), &evE);
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&s31),
                            RKH_STATE_CAST(&s3),
                            expectedState,
                            INIT_STATE_MACHINE);

    rkh_sm_dispatch((RKH_SM_T *)smTest, &evE);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
}

void
test_trnWoutUnitrazerFails_EventNotFound(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&s1);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&s1),
                            RKH_STATE_CAST(&s1),
                            expectedState,
                            INIT_STATE_MACHINE);

    result = rkh_sm_dispatch((RKH_SM_T *)smTest, &evE);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
    TEST_ASSERT_EQUAL(RKH_EVT_NFOUND, result);
}

void
test_trnWoutUnitrazerFails_GuardFalseOnInternalTrn(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&s1);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_falseGuard_ExpectAndReturn(RKH_CAST(SmTest, smTest), &evC, 
                                      RKH_FALSE);
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&s1),
                            RKH_STATE_CAST(&s1),
                            expectedState,
                            INIT_STATE_MACHINE);

    result = rkh_sm_dispatch((RKH_SM_T *)smTest, &evC);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
    TEST_ASSERT_EQUAL(RKH_EVT_NFOUND, result);
}

void
test_trnWoutUnitrazerFails_GuardFalseOnExternalTrn(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&s1);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_falseGuard_ExpectAndReturn(RKH_CAST(SmTest, smTest), &evD, 
                                      RKH_FALSE);
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&s1),
                            RKH_STATE_CAST(&s1),
                            expectedState,
                            INIT_STATE_MACHINE);

    result = rkh_sm_dispatch((RKH_SM_T *)smTest, &evD);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
    TEST_ASSERT_EQUAL(RKH_EVT_NFOUND, result);
}

void
test_trnWoutUnitrazerFails_ExceededHierarchicalLevel(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&s0);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&s0),
                            RKH_STATE_CAST(&s0),
                            expectedState,
                            INIT_STATE_MACHINE);
    rkh_assert_Expect("rkhsm", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();

    result = rkh_sm_dispatch((RKH_SM_T *)smTest, &evE);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
    TEST_ASSERT_EQUAL(RKH_EX_HLEVEL, result);
}

void
test_trnWoutUnitrazerMultipleEnabledTrn_FiringFirstTrueGuard(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&s4);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_guard4a_ExpectAndReturn(RKH_CAST(SmTest, smTest), &evA, RKH_FALSE);
    smTest_guard4b_ExpectAndReturn(RKH_CAST(SmTest, smTest), &evA, RKH_TRUE);
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&s4),
                            RKH_STATE_CAST(&s4),
                            expectedState,
                            INIT_STATE_MACHINE);

    result = rkh_sm_dispatch((RKH_SM_T *)smTest, &evA);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
    TEST_ASSERT_EQUAL(RKH_EVT_PROC, result);
}

void
test_trnWoutUnitrazerMultipleEnabledTrn_FiringFirstEmptyGuard(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&s4);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_guard4a_ExpectAndReturn(RKH_CAST(SmTest, smTest), &evB, RKH_FALSE);
    smTest_guard4b_ExpectAndReturn(RKH_CAST(SmTest, smTest), &evB, RKH_FALSE);
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&s4),
                            RKH_STATE_CAST(&s4),
                            expectedState,
                            INIT_STATE_MACHINE);

    result = rkh_sm_dispatch((RKH_SM_T *)smTest, &evB);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
    TEST_ASSERT_EQUAL(RKH_EVT_PROC, result);
}

void
test_trnWoutUnitrazerDefaultTrnWithAssociatedEffect(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&s31);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS0_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr55_Expect(RKH_CAST(SmTest, smTest), &evF);
    smTest_nS3_Expect(RKH_CAST(SmTest, smTest));
    smTest_iS3_Expect(RKH_CAST(SmTest, smTest), &evF);
    smTest_nS31_Expect(RKH_CAST(SmTest, smTest));
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&s0),
                            RKH_STATE_CAST(&s0),
                            expectedState,
                            INIT_STATE_MACHINE);

    result = rkh_sm_dispatch((RKH_SM_T *)smTest, &evF);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
    TEST_ASSERT_EQUAL(RKH_EVT_PROC, result);
}

void
test_trnWoutUnitrazerGeneratedCompletionEventBySimpleState(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&s4);

    /* Expectations for transition to s5 */
    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS21_Expect(RKH_CAST(SmTest, smTest));
    smTest_xS2_Expect(RKH_CAST(SmTest, smTest));
    smTest_nS5_Expect(RKH_CAST(SmTest, smTest));
    /* Expectations for completion transition */
    smTest_xS5_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr61_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCompletion);
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&s21),
                            RKH_STATE_CAST(&s2),
                            expectedState,
                            INIT_STATE_MACHINE);

    result = rkh_sm_dispatch((RKH_SM_T *)smTest, &evE);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
    TEST_ASSERT_EQUAL(RKH_EVT_PROC, result);
}

void
test_trnWoutUnitrazerGeneratedCompletionEventByFinalState(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&s4);

    /* Expectations for transition to S2's FinalState */
    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_xS21_Expect(RKH_CAST(SmTest, smTest));
    /* Expectations for completion transition */
    smTest_guardS2_ExpectAndReturn(RKH_CAST(SmTest, smTest), 
                                   (RKH_EVT_T *)&evCompletion, 
                                   RKH_TRUE);
    smTest_xS2_Expect(RKH_CAST(SmTest, smTest));
    smTest_tr60_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCompletion);
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&s21),
                            RKH_STATE_CAST(&s21),
                            expectedState,
                            INIT_STATE_MACHINE);

    result = rkh_sm_dispatch((RKH_SM_T *)smTest, &evF);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
    TEST_ASSERT_EQUAL(RKH_EVT_PROC, result);
}

void
test_trnWoutUnitrazerSyncDispatchingToStateMachine(void)
{
    TEST_IGNORE();

    setUpWoutUnitrazer();
}

void
test_trnWoutUnitrazerPropagateAnEvent(void)
{
    setUpWoutUnitrazer();

    expectedState = RKH_STATE_CAST(&waiting);

    smTest_init_Expect(RKH_CAST(SmTest, smTest), (RKH_EVT_T *)&evCreation);
    smTest_tr16_Expect(RKH_CAST(SmTest, smTest), &evC);
    smTest_tr16_StubWithCallback(MockEffectCallback0);
    smTest_tr17_Expect(RKH_CAST(SmTest, smTest), &evD);
    smTest_tr17_StubWithCallback(MockEffectCallback1);
    setProfileWoutUnitrazer(smTest,
                            RKH_STATE_CAST(&waiting),
                            RKH_STATE_CAST(&waiting),
                            expectedState,
                            INIT_STATE_MACHINE);

    result = rkh_sm_dispatch((RKH_SM_T *)smTest, &evC);

    TEST_ASSERT_TRUE(expectedState == getState(smTest));
    TEST_ASSERT_EQUAL(RKH_EVT_PROC, result);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/**
 *  \addtogroup test_smPseudostate Pseudostate test group
 *  @{
 *  \name Test cases of pseudostateConditional group
 *  @{ 
 */
void
test_pseudostateConditionalFirstStateAfterInit(void)
{
    UtrzProcessOut *p;

    sm_init_expect(RKH_STATE_CAST(&smPCT_waiting));
    sm_trn_expect(RKH_STATE_CAST(&smPCT_waiting), 
                                 RKH_STATE_CAST(&smPCT_waiting));
    sm_tsState_expect(RKH_STATE_CAST(&smPCT_waiting));
    sm_enstate_expect(RKH_STATE_CAST(&smPCT_waiting));
    sm_nenex_expect(1, 0);
    sm_state_expect(RKH_STATE_CAST(&smPCT_waiting));
    sm_evtProc_expect();

    rkh_sm_init((RKH_SM_T *)smPseudoConditionalTest);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_pseudostateConditionalTrnToChoiceWithTrueCondition(void)
{
    UtrzProcessOut *p;
    SmPseudoConditionalTest *me = RKH_CAST(SmPseudoConditionalTest, 
                                           smPseudoConditionalTest);

    /* Expect call actions */
    smPCT_setCondition_Expect(
        RKH_CAST(SmPseudoConditionalTest, me), &evA);
    smPCT_onEventA_ExpectAndReturn(
        RKH_CAST(SmPseudoConditionalTest, me), 
        &evA, RKH_TRUE);
    smPCT_tr1_Expect(
        RKH_CAST(SmPseudoConditionalTest, me), &evA);
    /* Expect init state machine */
    expInitSm((RKH_SMA_T *)me, RKH_STATE_CAST(&smPCT_waiting));
    /* Expect first transition segment */
	sm_dch_expect(evA.e, RKH_STATE_CAST(&smPCT_s0));
	sm_trn_expect(RKH_STATE_CAST(&smPCT_s0), RKH_STATE_CAST(&smPCT_choice1));
    sm_tsState_expect(RKH_STATE_CAST(&smPCT_choice1));
	//sm_ntrnact_expect(1, 1);
    /* Expect target state of last (second) transition */
    sm_tsState_expect(RKH_STATE_CAST(&smPCT_s1));
    /* Expect solve compoused transition */
    sm_exstate_expect(RKH_STATE_CAST(&smPCT_s0));
	//sm_ntrnact_expect(1, 2);
    sm_enstate_expect(RKH_STATE_CAST(&smPCT_s1));
    sm_tsState_expect(RKH_STATE_CAST(&smPCT_s11));
    sm_enstate_expect(RKH_STATE_CAST(&smPCT_s11));
    sm_nenex_expect(2, 1);
    /* Expect main target state */
    sm_state_expect(RKH_STATE_CAST(&smPCT_s11));
	sm_evtProc_expect();

    /* Initialize and set state */
    rkh_sm_init((RKH_SM_T *)me);
    setState((RKH_SMA_T *)me, RKH_STATE_CAST(&smPCT_s0));

    rkh_sm_dispatch((RKH_SM_T *)me, &evA);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_pseudostateConditionalTrnToChoiceWithFalseCondition(void)
{
    UtrzProcessOut *p;
    SmPseudoConditionalTest *me = RKH_CAST(SmPseudoConditionalTest, 
                                           smPseudoConditionalTest);

    /* Expect call actions */
    smPCT_setCondition_Expect(
        RKH_CAST(SmPseudoConditionalTest, me), &evB);
    smPCT_onEventA_ExpectAndReturn(
        RKH_CAST(SmPseudoConditionalTest, me), 
        &evB, RKH_FALSE);
    smPCT_onEventD_ExpectAndReturn(
        RKH_CAST(SmPseudoConditionalTest, me), 
        &evB, RKH_FALSE);
    smPCT_tr2_Expect(
        RKH_CAST(SmPseudoConditionalTest, me), &evB);
    /* Expect init state machine */
    expInitSm((RKH_SMA_T *)me, RKH_STATE_CAST(&smPCT_waiting));
    /* Expect first transition segment */
	sm_dch_expect(evB.e, RKH_STATE_CAST(&smPCT_s0));
	sm_trn_expect(RKH_STATE_CAST(&smPCT_s0), RKH_STATE_CAST(&smPCT_choice1));
    sm_tsState_expect(RKH_STATE_CAST(&smPCT_choice1));
	//sm_ntrnact_expect(1, 1);
    /* Expect target state of last (second) transition */
    sm_tsState_expect(RKH_STATE_CAST(&smPCT_s12));
    /* Expect solve compoused transition */
    sm_exstate_expect(RKH_STATE_CAST(&smPCT_s0));
	//sm_ntrnact_expect(1, 2);
    sm_enstate_expect(RKH_STATE_CAST(&smPCT_s1));
    sm_enstate_expect(RKH_STATE_CAST(&smPCT_s12));
    sm_nenex_expect(2, 1);
    /* Expect main target state */
    sm_state_expect(RKH_STATE_CAST(&smPCT_s12));
	sm_evtProc_expect();
    /* Initialize and set state */
    rkh_sm_init((RKH_SM_T *)me);
    setState((RKH_SMA_T *)me, RKH_STATE_CAST(&smPCT_s0));

    rkh_sm_dispatch((RKH_SM_T *)me, &evB);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_pseudostateConditionalTrnToChoiceWithoutElse(void)
{
    UtrzProcessOut *p;
    SmPseudoConditionalTest *me = RKH_CAST(SmPseudoConditionalTest, 
                                           smPseudoConditionalTest);

    /* Expect call actions */
    smPCT_onEventA_ExpectAndReturn(
        RKH_CAST(SmPseudoConditionalTest, me), 
        &evC, RKH_FALSE);
    /* Expect init state machine */
    expInitSm((RKH_SMA_T *)me, RKH_STATE_CAST(&smPCT_waiting));
    /* Expect first transition segment */
	sm_dch_expect(evC.e, RKH_STATE_CAST(&smPCT_s0));
	sm_trn_expect(RKH_STATE_CAST(&smPCT_s0), RKH_STATE_CAST(&smPCT_choice2));
    sm_tsState_expect(RKH_STATE_CAST(&smPCT_choice2));
	//sm_ntrnact_expect(0, 1);
	sm_cndNotFound_expect();
    /* Initialize and set state */
    rkh_sm_init((RKH_SM_T *)me);
    setState((RKH_SMA_T *)me, RKH_STATE_CAST(&smPCT_s0));

    rkh_sm_dispatch((RKH_SM_T *)me, &evC);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_pseudostateConditionalTrnToChoiceReturnToSource(void)
{
    UtrzProcessOut *p;
    SmPseudoConditionalTest *me = RKH_CAST(SmPseudoConditionalTest, 
                                           smPseudoConditionalTest);

    /* Expect call actions */
    smPCT_onEventA_ExpectAndReturn(RKH_CAST(SmPseudoConditionalTest, me), 
                                   &evD, RKH_FALSE);
    smPCT_onEventD_ExpectAndReturn(RKH_CAST(SmPseudoConditionalTest, me), 
                                   &evD, RKH_TRUE);
    smPCT_tr4_Expect(RKH_CAST(SmPseudoConditionalTest, me), &evD);
    /* Expect init state machine */
    expInitSm((RKH_SMA_T *)me, RKH_STATE_CAST(&smPCT_waiting));
    /* Expect first transition segment */
	sm_dch_expect(evD.e, RKH_STATE_CAST(&smPCT_s0));
	sm_trn_expect(RKH_STATE_CAST(&smPCT_s0), RKH_STATE_CAST(&smPCT_choice1));
    sm_tsState_expect(RKH_STATE_CAST(&smPCT_choice1));
	//sm_ntrnact_expect(0, 1);
    /* Expect target state of last (second) transition */
    sm_tsState_expect(RKH_STATE_CAST(&smPCT_s0));
    /* Expect solve compoused transition */
    sm_exstate_expect(RKH_STATE_CAST(&smPCT_s0));
	//sm_ntrnact_expect(1, 2);
    sm_enstate_expect(RKH_STATE_CAST(&smPCT_s0));
    sm_nenex_expect(1, 1);
    /* Expect main target state */
    sm_state_expect(RKH_STATE_CAST(&smPCT_s0));
	sm_evtProc_expect();
    /* Initialize and set state */
    rkh_sm_init((RKH_SM_T *)me);
    setState((RKH_SMA_T *)me, RKH_STATE_CAST(&smPCT_s0));

    rkh_sm_dispatch((RKH_SM_T *)me, &evD);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_pseudostateConditionalFailsTrnSegmentsExceeded(void)
{
    UtrzProcessOut *p;
    SmPseudoConditionalTest *me = RKH_CAST(SmPseudoConditionalTest, 
                                           smPseudoConditionalTest);

    /* Expect call actions */
    smPCT_tr1_Expect(RKH_CAST(SmPseudoConditionalTest, me), &evE);
    smPCT_guardTrue_ExpectAndReturn(RKH_CAST(SmPseudoConditionalTest, me), 
                                    &evE, RKH_TRUE);
    smPCT_tr1_Expect(RKH_CAST(SmPseudoConditionalTest, me), &evE);
    smPCT_guardTrue_ExpectAndReturn(RKH_CAST(SmPseudoConditionalTest, me), 
                                    &evE, RKH_TRUE);
    smPCT_tr1_Expect(RKH_CAST(SmPseudoConditionalTest, me), &evE);
    smPCT_guardTrue_ExpectAndReturn(RKH_CAST(SmPseudoConditionalTest, me), 
                                    &evE, RKH_TRUE);
    smPCT_tr1_Expect(RKH_CAST(SmPseudoConditionalTest, me), &evE);
    smPCT_guardTrue_ExpectAndReturn(RKH_CAST(SmPseudoConditionalTest, me), 
                                    &evE, RKH_TRUE);
    smPCT_tr1_Expect(RKH_CAST(SmPseudoConditionalTest, me), &evE);
    /* Expect init state machine */
    expInitSm((RKH_SMA_T *)me, RKH_STATE_CAST(&smPCT_waiting));
    /* Expect first transition segment */
	sm_dch_expect(evE.e, RKH_STATE_CAST(&smPCT_s0));
	sm_trn_expect(RKH_STATE_CAST(&smPCT_s0), RKH_STATE_CAST(&smPCT_choice3));
    sm_tsState_expect(RKH_STATE_CAST(&smPCT_choice3));
	//sm_ntrnact_expect(1, 1);
    /* Expect target state of next transition */
    sm_tsState_expect(RKH_STATE_CAST(&smPCT_choice4));
	//sm_ntrnact_expect(1, 2);
    sm_tsState_expect(RKH_STATE_CAST(&smPCT_choice5));
	//sm_ntrnact_expect(1, 3);
    sm_tsState_expect(RKH_STATE_CAST(&smPCT_choice6));
	//sm_ntrnact_expect(1, 4);
    sm_tsState_expect(RKH_STATE_CAST(&smPCT_s1));
    /* Expect solve compoused transition */
    sm_exstate_expect(RKH_STATE_CAST(&smPCT_s0));
	//sm_ntrnact_expect(1, 5);
    sm_enstate_expect(RKH_STATE_CAST(&smPCT_s1));
    sm_tsState_expect(RKH_STATE_CAST(&smPCT_s11));
    sm_enstate_expect(RKH_STATE_CAST(&smPCT_s11));
    sm_nenex_expect(2, 1);
    /* Expect main target state */
    sm_state_expect(RKH_STATE_CAST(&smPCT_s11));
	sm_evtProc_expect();
    /* Initialize and set state */
    rkh_sm_init((RKH_SM_T *)me);
    setState((RKH_SMA_T *)me, RKH_STATE_CAST(&smPCT_s0));

    rkh_sm_dispatch((RKH_SM_T *)me, &evE);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}
/** @} doxygen end group definition */
/** @} doxygen end group definition */

/**
 *  \addtogroup test_smPseudostate Pseudostate test group
 *  @{
 *  \name Test cases of pseudostate group
 *  @{ 
 */
void
test_pseudostateFirstStateAfterInit(void)
{
    UtrzProcessOut *p;

    sm_init_expect(RKH_STATE_CAST(&smPT_waiting));
    sm_trn_expect(RKH_STATE_CAST(&smPT_waiting), 
                                 RKH_STATE_CAST(&smPT_waiting));
    sm_tsState_expect(RKH_STATE_CAST(&smPT_waiting));
    sm_enstate_expect(RKH_STATE_CAST(&smPT_waiting));
    sm_nenex_expect(1, 0);
    sm_state_expect(RKH_STATE_CAST(&smPT_waiting));
    sm_evtProc_expect();

    rkh_sm_init((RKH_SM_T *)smPseudoTest);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_pseudostateTrnToEmptyShallowHistoryWithoutDefaultTrn(void)
{
    UtrzProcessOut *p;
    int nExSt, nEnSt;
    RKH_EVT_T event;

    nEnSt = 2;
    nExSt = 1;
    event = evB;
    expInitSm((RKH_SMA_T *)smPseudoTest, RKH_STATE_CAST(&smPT_waiting));
	sm_dch_expect(event.e, RKH_STATE_CAST(&smPT_s0));
	sm_trn_expect(RKH_STATE_CAST(&smPT_s0), RKH_STATE_CAST(&smPT_s1Hist));
    sm_tsState_expect(RKH_STATE_CAST(&smPT_s1Hist));
    sm_tsState_expect(RKH_STATE_CAST(&smPT_s1));
    sm_exstate_expect(RKH_STATE_CAST(&smPT_s0));
    sm_enstate_expect(RKH_STATE_CAST(&smPT_s1));
    sm_tsState_expect(RKH_STATE_CAST(&smPT_s11));
    sm_enstate_expect(RKH_STATE_CAST(&smPT_s11));
    sm_nenex_expect(nEnSt, nExSt);
    sm_state_expect(RKH_STATE_CAST(&smPT_s11));
	sm_evtProc_expect();

    rkh_sm_init((RKH_SM_T *)smPseudoTest);
    setState((RKH_SMA_T *)smPseudoTest, RKH_STATE_CAST(&smPT_s0));
    rkh_sm_dispatch((RKH_SM_T *)smPseudoTest, &event);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_pseudostateTrnToLoadedShallowHistoryWithoutDefaultTrn(void)
{
    UtrzProcessOut *p;
    int nExSt, nEnSt;
    RKH_EVT_T event;

    nEnSt = 3;
    nExSt = 1;
    event = evB;
    expInitSm((RKH_SMA_T *)smPseudoTest, RKH_STATE_CAST(&smPT_waiting));
	sm_dch_expect(event.e, RKH_STATE_CAST(&smPT_s0));
	sm_trn_expect(RKH_STATE_CAST(&smPT_s0), RKH_STATE_CAST(&smPT_s1Hist));
    sm_tsState_expect(RKH_STATE_CAST(&smPT_s1Hist));
    sm_tsState_expect(RKH_STATE_CAST(&smPT_s12));
    sm_exstate_expect(RKH_STATE_CAST(&smPT_s0));
    sm_enstate_expect(RKH_STATE_CAST(&smPT_s1));
    sm_enstate_expect(RKH_STATE_CAST(&smPT_s12));
    sm_tsState_expect(RKH_STATE_CAST(&smPT_s121));
    sm_enstate_expect(RKH_STATE_CAST(&smPT_s121));
    sm_nenex_expect(nEnSt, nExSt);
    sm_state_expect(RKH_STATE_CAST(&smPT_s121));
	sm_evtProc_expect();

    rkh_sm_init((RKH_SM_T *)smPseudoTest);
    setState((RKH_SMA_T *)smPseudoTest, RKH_STATE_CAST(&smPT_s0));
    setHistory(&smPT_s1Hist, RKH_STATE_CAST(&smPT_s12));
    rkh_sm_dispatch((RKH_SM_T *)smPseudoTest, &event);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_pseudostateExitFromCompositeWithLoadedShallowHistory(void)
{
    UtrzProcessOut *p;
    const RKH_ST_T *state;

    stateList_create(targetStates, 1, &smPT_s0);
    stateList_create(exitStates, 3, &smPT_s121, &smPT_s12, &smPT_s1);
    stateList_create(entryStates, 1, &smPT_s0);
    setProfile(smPseudoTest, RKH_STATE_CAST(&smPT_waiting), 
               RKH_STATE_CAST(&smPT_s121), 
               RKH_STATE_CAST(&smPT_s1), targetStates, 
               entryStates, exitStates, 
               RKH_STATE_CAST(&smPT_s0), 0, TRN_NOT_INTERNAL, 1, &evA,
               RKH_STATE_CAST(&smPT_s121));

    rkh_sm_dispatch((RKH_SM_T *)smPseudoTest, &evA);
    state = getHistory(&smPT_s1Hist);
    TEST_ASSERT_EQUAL_PTR(RKH_STATE_CAST(&smPT_s12), state);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_pseudostateTrnToEmptyDeepHistoryWithoutDefaultTrn(void)
{
    UtrzProcessOut *p;
    int nExSt, nEnSt;
    RKH_EVT_T event;

    nEnSt = 3;
    nExSt = 1;
    event = evC;
    expInitSm((RKH_SMA_T *)smPseudoTest, RKH_STATE_CAST(&smPT_waiting));
	sm_dch_expect(event.e, RKH_STATE_CAST(&smPT_s0));
	sm_trn_expect(RKH_STATE_CAST(&smPT_s0), RKH_STATE_CAST(&smPT_s12Hist));
    sm_tsState_expect(RKH_STATE_CAST(&smPT_s12Hist));
    sm_tsState_expect(RKH_STATE_CAST(&smPT_s12));
    sm_exstate_expect(RKH_STATE_CAST(&smPT_s0));
    sm_enstate_expect(RKH_STATE_CAST(&smPT_s1));
    sm_enstate_expect(RKH_STATE_CAST(&smPT_s12));
    sm_tsState_expect(RKH_STATE_CAST(&smPT_s121));
    sm_enstate_expect(RKH_STATE_CAST(&smPT_s121));
    sm_nenex_expect(nEnSt, nExSt);
    sm_state_expect(RKH_STATE_CAST(&smPT_s121));
	sm_evtProc_expect();

    rkh_sm_init((RKH_SM_T *)smPseudoTest);
    setState((RKH_SMA_T *)smPseudoTest, RKH_STATE_CAST(&smPT_s0));
    rkh_sm_dispatch((RKH_SM_T *)smPseudoTest, &event);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_pseudostateTrnToLoadedDeepHistoryWithoutDefaultTrn(void)
{
    UtrzProcessOut *p;

    stateList_create(targetStates, 2, &smPT_s12Hist, &smPT_s122);
    stateList_create(exitStates, 1, &smPT_s0);
    stateList_create(entryStates, 3, &smPT_s1, &smPT_s12, &smPT_s122);
    setProfile(smPseudoTest, RKH_STATE_CAST(&smPT_waiting), 
               RKH_STATE_CAST(&smPT_s0), 
               RKH_STATE_CAST(&smPT_s0), targetStates, 
               entryStates, exitStates, 
               RKH_STATE_CAST(&smPT_s122), 0, TRN_NOT_INTERNAL, 1, &evC,
               RKH_STATE_CAST(&smPT_s0));

    setHistory(&smPT_s12Hist, RKH_STATE_CAST(&smPT_s122));
    rkh_sm_dispatch((RKH_SM_T *)smPseudoTest, &evC);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_pseudostateExitFromCompositeWithLoadedDeepHistory(void)
{
    UtrzProcessOut *p;
    const RKH_ST_T *state;

    stateList_create(targetStates, 1, &smPT_s122);
    stateList_create(exitStates, 1, &smPT_s0);
    stateList_create(entryStates, 3, &smPT_s1, &smPT_s12, &smPT_s122);
    setProfile(smPseudoTest, RKH_STATE_CAST(&smPT_waiting), 
               RKH_STATE_CAST(&smPT_s0), 
               RKH_STATE_CAST(&smPT_s0), targetStates, 
               entryStates, exitStates, 
               RKH_STATE_CAST(&smPT_s122), 0, TRN_NOT_INTERNAL, 1, &evD,
               RKH_STATE_CAST(&smPT_s0));

    rkh_sm_dispatch((RKH_SM_T *)smPseudoTest, &evD);
    state = getHistory(&smPT_s12Hist);
    TEST_ASSERT_EQUAL_PTR(RKH_STATE_CAST(&smPT_s122), state);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_pseudostateTrnToEmptyShallowHistoryWithDefaultTrn(void)
{
    UtrzProcessOut *p;

    smPT_trueGuard_ExpectAndReturn(RKH_CAST(SmPseudoTest, smPseudoTest), 
                                   &evE, RKH_TRUE);
    smPT_trS2History_Expect(RKH_CAST(SmPseudoTest, smPseudoTest), &evE);

    stateList_create(targetStates, 2, &smPT_s2Hist, &smPT_s22);
    stateList_create(exitStates, 1, &smPT_s0);
    stateList_create(entryStates, 2, &smPT_s2, &smPT_s22);
    setProfile(smPseudoTest, RKH_STATE_CAST(&smPT_waiting), 
               RKH_STATE_CAST(&smPT_s0), 
               RKH_STATE_CAST(&smPT_s0), targetStates, 
               entryStates, exitStates, 
               RKH_STATE_CAST(&smPT_s22), 1, TRN_NOT_INTERNAL, 1, &evE,
               RKH_STATE_CAST(&smPT_s0));

    rkh_sm_dispatch((RKH_SM_T *)smPseudoTest, &evE);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_pseudostateTrnToLoadedShallowHistoryWithDefaultTrn(void)
{
    UtrzProcessOut *p;

    stateList_create(targetStates, 2, &smPT_s2Hist, &smPT_s21);
    stateList_create(exitStates, 1, &smPT_s0);
    stateList_create(entryStates, 2, &smPT_s2, &smPT_s21);
    setProfile(smPseudoTest, RKH_STATE_CAST(&smPT_waiting), 
               RKH_STATE_CAST(&smPT_s0), 
               RKH_STATE_CAST(&smPT_s0), targetStates, 
               entryStates, exitStates, 
               RKH_STATE_CAST(&smPT_s21), 0, TRN_NOT_INTERNAL, 1, &evE,
               RKH_STATE_CAST(&smPT_s0));

    setHistory(&smPT_s2Hist, RKH_STATE_CAST(&smPT_s21));
    rkh_sm_dispatch((RKH_SM_T *)smPseudoTest, &evE);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */

/**
 *  \addtogroup test_smInitialPseudostate Initial pseudostate test group
 *  @{
 *  \name Test cases of InitPseudostate group
 *  @{ 
 */
void
test_InitPseudostateFirstStateAfterInit(void)
{
    UtrzProcessOut *p;
    SmInitialPseudoTest *me = (SmInitialPseudoTest *)smInitialPseudoTest;

    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smIPT_nS0_Expect((SmInitialPseudoTest *)me);
    trnStepExpect((RKH_SM_T *)me, (RKH_ST_T *)&smIPT_s0, NULL, NULL, NULL, 
                  &evCreation);

    rkh_sm_init((RKH_SM_T *)me);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_InitPseudostateTrnToEmptyShHistoryToSimpleState(void)
{
    UtrzProcessOut *p;
    SmInitialPseudoTest *me = (SmInitialPseudoTest *)smInitialPseudoTest;
    TargetEntrySt tgEnSt[4] = {{(RKH_ST_T *)&smIPT_s1, (RKH_ST_T *)&smIPT_s1},
                               {(RKH_ST_T *)&smIPT_s1Hist, NULL},
                               {(RKH_ST_T *)&smIPT_s11, (RKH_ST_T *)&smIPT_s11},
                               {NULL, NULL}};

    stateList_create(exitStates, 1, &smIPT_s0);

    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smIPT_nS0_Expect((SmInitialPseudoTest *)smInitialPseudoTest);
    smIPT_xS0_Expect((SmInitialPseudoTest *)smInitialPseudoTest);
    smIPT_nS1_Expect((SmInitialPseudoTest *)smInitialPseudoTest);
    smIPT_tr1_Expect((SmInitialPseudoTest *)smInitialPseudoTest, &evA);
    smIPT_nS11_Expect((SmInitialPseudoTest *)smInitialPseudoTest);

    trnStepExpect((RKH_SM_T *)me, (RKH_ST_T *)&smIPT_s0, 
                  (RKH_ST_T *)&smIPT_s0, tgEnSt, exitStates, &evA);

    rkh_sm_init((RKH_SM_T *)me);
    rkh_sm_dispatch((RKH_SM_T *)smInitialPseudoTest, &evA);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_InitPseudostateTrnToLoadedShHistory(void)
{
    UtrzProcessOut *p;
    SmInitialPseudoTest *me = (SmInitialPseudoTest *)smInitialPseudoTest;
    TargetEntrySt tgEnSt[4] = {{(RKH_ST_T *)&smIPT_s1, (RKH_ST_T *)&smIPT_s1},
                               {(RKH_ST_T *)&smIPT_s1Hist, NULL },
                               {(RKH_ST_T *)&smIPT_s12, (RKH_ST_T *)&smIPT_s12},
                               {NULL, NULL }};

    stateList_create(exitStates, 1, &smIPT_s0);

    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smIPT_nS0_Expect((SmInitialPseudoTest *)me);
    smIPT_xS0_Expect((SmInitialPseudoTest *)me);
    smIPT_nS1_Expect((SmInitialPseudoTest *)me);
    smIPT_nS12_Expect((SmInitialPseudoTest *)me);

    trnStepExpect((RKH_SM_T *)me, (RKH_ST_T *)&smIPT_s0, 
                  (RKH_ST_T *)&smIPT_s0, tgEnSt, exitStates, &evA);

    rkh_sm_init((RKH_SM_T *)me);
    setHistory(&smIPT_s1Hist, RKH_STATE_CAST(&smIPT_s12));
    rkh_sm_dispatch((RKH_SM_T *)me, &evA);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_InitPseudostateTrnToEmptyShHistoryToCmpState(void)
{
    UtrzProcessOut *p;
    SmInitialPseudoTest *me = (SmInitialPseudoTest *)smInitialPseudoTest;
    TargetEntrySt tgEnSt[5] = {{(RKH_ST_T *)&smIPT_s2, (RKH_ST_T *)&smIPT_s2},
                               {(RKH_ST_T *)&smIPT_s2Hist, NULL},
                               {(RKH_ST_T *)&smIPT_s21, (RKH_ST_T *)&smIPT_s21},
                               {(RKH_ST_T *)&smIPT_s211, (RKH_ST_T *)&smIPT_s211    },
                               {NULL, NULL}};

    stateList_create(exitStates, 1, &smIPT_s0);

    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smIPT_nS0_Expect((SmInitialPseudoTest *)smInitialPseudoTest);
    smIPT_xS0_Expect((SmInitialPseudoTest *)smInitialPseudoTest);
    smIPT_nS2_Expect((SmInitialPseudoTest *)smInitialPseudoTest);
    smIPT_nS21_Expect((SmInitialPseudoTest *)smInitialPseudoTest);
    smIPT_nS211_Expect((SmInitialPseudoTest *)smInitialPseudoTest);

    trnStepExpect((RKH_SM_T *)me, (RKH_ST_T *)&smIPT_s0, 
                  (RKH_ST_T *)&smIPT_s0, tgEnSt, exitStates, &evD);

    rkh_sm_init((RKH_SM_T *)me);
    rkh_sm_dispatch((RKH_SM_T *)smInitialPseudoTest, &evD);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_InitPseudostateTrnToEmptyDeepHistoryToSimpleState(void)
{
    UtrzProcessOut *p;
    SmInitialPseudoTest *me = (SmInitialPseudoTest *)smInitialPseudoTest;
    TargetEntrySt tgEnSt[4] =   {{(RKH_ST_T *)&smIPT_s3, (RKH_ST_T *)&smIPT_s3},
                                 {(RKH_ST_T *)&smIPT_s3Hist, NULL},
                                 {(RKH_ST_T *)&smIPT_s31, (RKH_ST_T *)&smIPT_s31  },
                                 {NULL, NULL}};

    stateList_create(exitStates, 1, &smIPT_s0);

    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smIPT_nS0_Expect((SmInitialPseudoTest *)me);
    smIPT_xS0_Expect((SmInitialPseudoTest *)me);
    smIPT_nS3_Expect((SmInitialPseudoTest *)me);
    smIPT_nS31_Expect((SmInitialPseudoTest *)me);

    trnStepExpect((RKH_SM_T *)me, (RKH_ST_T *)&smIPT_s0, 
                  (RKH_ST_T *)&smIPT_s0, tgEnSt, exitStates, &evB);

    rkh_sm_init((RKH_SM_T *)me);
    rkh_sm_dispatch((RKH_SM_T *)smInitialPseudoTest, &evB);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_InitPseudostateTrnToLoadedDeepHistory(void)
{
    UtrzProcessOut *p;
    SmInitialPseudoTest *me = (SmInitialPseudoTest *)smInitialPseudoTest;
    TargetEntrySt tgEnSt[4] = {{(RKH_ST_T *)&smIPT_s3, (RKH_ST_T *)&smIPT_s3},
                               {(RKH_ST_T *)&smIPT_s3Hist, NULL},
                               {(RKH_ST_T *)&smIPT_s32, (RKH_ST_T *)&smIPT_s32},
                               {NULL, NULL}};

    stateList_create(exitStates, 1, &smIPT_s0);

    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smIPT_nS0_Expect((SmInitialPseudoTest *)me);
    smIPT_xS0_Expect((SmInitialPseudoTest *)me);
    smIPT_nS3_Expect((SmInitialPseudoTest *)me);
    smIPT_nS32_Expect((SmInitialPseudoTest *)me);

    trnStepExpect((RKH_SM_T *)me, (RKH_ST_T *)&smIPT_s0, 
                  (RKH_ST_T *)&smIPT_s0, tgEnSt, exitStates, &evB);

    rkh_sm_init((RKH_SM_T *)me);
    setHistory(&smIPT_s3Hist, RKH_STATE_CAST(&smIPT_s32));
    rkh_sm_dispatch((RKH_SM_T *)smInitialPseudoTest, &evB);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_InitPseudostateTrnToLoadedDeepHistoryToNestedState(void)
{
    UtrzProcessOut *p;
    SmInitialPseudoTest *me = (SmInitialPseudoTest *)smInitialPseudoTest;
    TargetEntrySt tgEnSt[5] = {{(RKH_ST_T *)&smIPT_s3, (RKH_ST_T *)&smIPT_s3 },
                               {(RKH_ST_T *)&smIPT_s3Hist, NULL},
                               {(RKH_ST_T *)&smIPT_s331, (RKH_ST_T *)&smIPT_s33},
                               {NULL, (RKH_ST_T *)&smIPT_s331},
                               {NULL, NULL}};

    stateList_create(exitStates, 1, &smIPT_s0);

    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smIPT_nS0_Expect((SmInitialPseudoTest *)me);
    smIPT_xS0_Expect((SmInitialPseudoTest *)me);
    smIPT_nS3_Expect((SmInitialPseudoTest *)me);
    smIPT_nS33_Expect((SmInitialPseudoTest *)me);
    smIPT_nS331_Expect((SmInitialPseudoTest *)me);

    trnStepExpect((RKH_SM_T *)me, (RKH_ST_T *)&smIPT_s0, 
                  (RKH_ST_T *)&smIPT_s0, tgEnSt, exitStates, &evB);

    rkh_sm_init((RKH_SM_T *)me);
    setHistory(&smIPT_s3Hist, RKH_STATE_CAST(&smIPT_s331));
    rkh_sm_dispatch((RKH_SM_T *)smInitialPseudoTest, &evB);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_InitPseudostateTrnToEmptyShHistoryToNestedSimpleState(void)
{
    UtrzProcessOut *p;
    SmInitialPseudoTest *me = (SmInitialPseudoTest *)smInitialPseudoTest;
    TargetEntrySt tgEnSt[5] = {{(RKH_ST_T *)&smIPT_s4, (RKH_ST_T *)&smIPT_s4},
                               {(RKH_ST_T *)&smIPT_s4Hist, NULL},
                               {(RKH_ST_T *)&smIPT_s411, (RKH_ST_T *)&smIPT_s41},
                               {NULL, (RKH_ST_T *)&smIPT_s411},
                               {NULL, NULL}};

    stateList_create(exitStates, 1, &smIPT_s0);

    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smIPT_nS0_Expect((SmInitialPseudoTest *)smInitialPseudoTest);
    smIPT_xS0_Expect((SmInitialPseudoTest *)smInitialPseudoTest);
    smIPT_nS4_Expect((SmInitialPseudoTest *)smInitialPseudoTest);
    smIPT_nS41_Expect((SmInitialPseudoTest *)smInitialPseudoTest);
    smIPT_nS411_Expect((SmInitialPseudoTest *)smInitialPseudoTest);

    trnStepExpect((RKH_SM_T *)me, (RKH_ST_T *)&smIPT_s0, 
                  (RKH_ST_T *)&smIPT_s0, tgEnSt, exitStates, &evC);

    rkh_sm_init((RKH_SM_T *)me);
    rkh_sm_dispatch((RKH_SM_T *)smInitialPseudoTest, &evC);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_InitPseudostateTrnToBranchToSimpleState(void)
{
    UtrzProcessOut *p;
    SmInitialPseudoTest *me = (SmInitialPseudoTest *)smInitialPseudoTest;
    TargetEntrySt tgEnSt[4] = {{(RKH_ST_T *)&smIPT_s5, (RKH_ST_T *)&smIPT_s5},
                               {(RKH_ST_T *)&smIPT_choice1, NULL},
                               {(RKH_ST_T *)&smIPT_s52, (RKH_ST_T *)&smIPT_s52},
                               {NULL, NULL}};

    //stateList_create(targetStates, 3, &smIPT_s5, &smIPT_choice1, &smIPT_s52);
    stateList_create(exitStates, 1, &smIPT_s0);
    //stateList_create(entryStates, 2, &smIPT_s5, &smIPT_s52);

    /* Expect call actions */
    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smIPT_nS0_Expect((SmInitialPseudoTest *)me);
    smIPT_xS0_Expect((SmInitialPseudoTest *)me);
    smIPT_nS5_Expect((SmInitialPseudoTest *)me);
    smIPT_tr2_Expect((SmInitialPseudoTest *)me, &evE);
    smIPT_isC1_ExpectAndReturn((RKH_SM_T *)me, &evE, RKH_TRUE);
    smIPT_nS52_Expect((SmInitialPseudoTest *)me);

    trnStepExpect((RKH_SM_T *)me, (RKH_ST_T *)&smIPT_s0, 
                  (RKH_ST_T *)&smIPT_s0, tgEnSt, exitStates, &evE);

    rkh_sm_init((RKH_SM_T *)me);
    rkh_sm_dispatch((RKH_SM_T *)me, &evE);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_InitPseudostateTrnToBranchToCmpState(void)
{
    UtrzProcessOut *p;
    SmInitialPseudoTest *me = (SmInitialPseudoTest *)smInitialPseudoTest;
    TargetEntrySt tgEnSt[5] = {{(RKH_ST_T *)&smIPT_s5, (RKH_ST_T *)&smIPT_s5},
                               {(RKH_ST_T *)&smIPT_choice1, NULL},
                               {(RKH_ST_T *)&smIPT_s51, (RKH_ST_T *)&smIPT_s51},
                               {(RKH_ST_T *)&smIPT_s511, (RKH_ST_T *)&smIPT_s511},
                               {NULL, NULL}};

    //stateList_create(targetStates, 4, &smIPT_s5, &smIPT_choice1, &smIPT_s51, 
    //                 &smIPT_s511);
    stateList_create(exitStates, 1, &smIPT_s0);
    //stateList_create(entryStates, 3, &smIPT_s5, &smIPT_s51, &smIPT_s511);

    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smIPT_nS0_Expect((SmInitialPseudoTest *)me);
    smIPT_xS0_Expect((SmInitialPseudoTest *)me);
    smIPT_nS5_Expect((SmInitialPseudoTest *)me);
    smIPT_tr2_Expect((SmInitialPseudoTest *)me, &evE);
    smIPT_isC1_ExpectAndReturn((RKH_SM_T *)me, &evE, RKH_FALSE);
    smIPT_isC2_ExpectAndReturn((RKH_SM_T *)me, &evE, RKH_FALSE);
    smIPT_isC3_ExpectAndReturn((RKH_SM_T *)me, &evE, RKH_FALSE);
    smIPT_isC4_ExpectAndReturn((RKH_SM_T *)me, &evE, RKH_FALSE);
    smIPT_nS51_Expect((SmInitialPseudoTest *)me);
    smIPT_nS511_Expect((SmInitialPseudoTest *)me);

    trnStepExpect((RKH_SM_T *)me, (RKH_ST_T *)&smIPT_s0, 
                  (RKH_ST_T *)&smIPT_s0, tgEnSt, exitStates, &evE);

    rkh_sm_init((RKH_SM_T *)me);
    rkh_sm_dispatch((RKH_SM_T *)me, &evE);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_InitPseudostateTrnToBranchToNestedSimpleState(void)
{
    UtrzProcessOut *p;
    SmInitialPseudoTest *me = RKH_CAST(SmInitialPseudoTest, 
                                       smInitialPseudoTest);
    TargetEntrySt tgEnSt[5] = {{(RKH_ST_T *)&smIPT_s5, (RKH_ST_T *)&smIPT_s5},
                               {(RKH_ST_T *)&smIPT_choice1, NULL},
                               {(RKH_ST_T *)&smIPT_s511, (RKH_ST_T *)&smIPT_s51},
                               {NULL, (RKH_ST_T *)&smIPT_s511},
                               {NULL, NULL}};

    stateList_create(exitStates, 1, &smIPT_s0);

    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smIPT_nS0_Expect((SmInitialPseudoTest *)me);
    smIPT_xS0_Expect((SmInitialPseudoTest *)me);
    smIPT_nS5_Expect((SmInitialPseudoTest *)me);
    smIPT_tr2_Expect((SmInitialPseudoTest *)me, &evE);
    smIPT_isC1_ExpectAndReturn((RKH_SM_T *)me, &evE, RKH_FALSE);
    smIPT_isC2_ExpectAndReturn((RKH_SM_T *)me, &evE, RKH_TRUE);
    smIPT_nS51_Expect((SmInitialPseudoTest *)me);
    smIPT_nS511_Expect((SmInitialPseudoTest *)me);

    trnStepExpect((RKH_SM_T *)me, (RKH_ST_T *)&smIPT_s0, 
                  (RKH_ST_T *)&smIPT_s0, tgEnSt, exitStates, &evE);

    rkh_sm_init((RKH_SM_T *)me);
    rkh_sm_dispatch((RKH_SM_T *)me, &evE);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_InitPseudostateTrnToBranchToNestedCmpState(void)
{
    UtrzProcessOut *p;
    SmInitialPseudoTest *me = RKH_CAST(SmInitialPseudoTest, 
                                       smInitialPseudoTest);
    TargetEntrySt tgEnSt[6] = {{(RKH_ST_T *)&smIPT_s5, (RKH_ST_T *)&smIPT_s5},
                               {(RKH_ST_T *)&smIPT_choice1, NULL},
                               {(RKH_ST_T *)&smIPT_s512, (RKH_ST_T *)&smIPT_s51},
                               {NULL, (RKH_ST_T *)&smIPT_s512},
                               {(RKH_ST_T *)&smIPT_s5121, (RKH_ST_T *)&smIPT_s5121},
                               {NULL, NULL}};

    stateList_create(exitStates, 1, &smIPT_s0);

    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smIPT_nS0_Expect((SmInitialPseudoTest *)me);
    smIPT_xS0_Expect((SmInitialPseudoTest *)me);
    smIPT_nS5_Expect((SmInitialPseudoTest *)me);
    smIPT_tr2_Expect((SmInitialPseudoTest *)me, &evE);
    smIPT_isC1_ExpectAndReturn((RKH_SM_T *)me, &evE, RKH_FALSE);
    smIPT_isC2_ExpectAndReturn((RKH_SM_T *)me, &evE, RKH_FALSE);
    smIPT_isC3_ExpectAndReturn((RKH_SM_T *)me, &evE, RKH_TRUE);
    smIPT_nS51_Expect((SmInitialPseudoTest *)me);
    smIPT_nS512_Expect((SmInitialPseudoTest *)me);
    smIPT_nS5121_Expect((SmInitialPseudoTest *)me);

    trnStepExpect((RKH_SM_T *)me, (RKH_ST_T *)&smIPT_s0, 
                  (RKH_ST_T *)&smIPT_s0, tgEnSt, exitStates, &evE);

    rkh_sm_init((RKH_SM_T *)me);
    rkh_sm_dispatch((RKH_SM_T *)me, &evE);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_InitPseudostateTrnToBranchToFinalState(void)
{
    UtrzProcessOut *p;
    SmInitialPseudoTest *me = (SmInitialPseudoTest *)smInitialPseudoTest;
    TargetEntrySt tgEnSt[4] = 
            {{(RKH_ST_T *)&smIPT_s5, (RKH_ST_T *)&smIPT_s5},
            {(RKH_ST_T *)&smIPT_choice1, NULL},
            {(RKH_ST_T *)&smIPT_s5Final, (RKH_ST_T *)&smIPT_s5Final},
            {NULL, NULL}};

    //stateList_create(targetStates, 3, &smIPT_s5, &smIPT_choice1, &smIPT_s52);
    stateList_create(exitStates, 1, &smIPT_s0);
    //stateList_create(entryStates, 2, &smIPT_s5, &smIPT_s52);

    /* Expect call actions */
    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smIPT_nS0_Expect((SmInitialPseudoTest *)me);
    smIPT_xS0_Expect((SmInitialPseudoTest *)me);
    smIPT_nS5_Expect((SmInitialPseudoTest *)me);
    smIPT_tr2_Expect((SmInitialPseudoTest *)me, &evE);
    smIPT_isC1_ExpectAndReturn((RKH_SM_T *)me, &evE, RKH_FALSE);
    smIPT_isC2_ExpectAndReturn((RKH_SM_T *)me, &evE, RKH_FALSE);
    smIPT_isC3_ExpectAndReturn((RKH_SM_T *)me, &evE, RKH_FALSE);
    smIPT_isC4_ExpectAndReturn((RKH_SM_T *)me, &evE, RKH_TRUE);

    trnStepExpect((RKH_SM_T *)me, (RKH_ST_T *)&smIPT_s0, 
                  (RKH_ST_T *)&smIPT_s0, tgEnSt, exitStates, &evE);

    /* Expectations for completion transition */
    stateList_create(targetStates, 1, &smIPT_s0);
    stateList_create(entryStates, 1, &smIPT_s0);
    stateList_create(exitStates, 2, &smIPT_s5Final, &smIPT_s5);

    smIPT_xS5_Expect((SmInitialPseudoTest *)me);
    smIPT_nS0_Expect((SmInitialPseudoTest *)me);

    setProfile(smInitialPseudoTest, RKH_STATE_CAST(&smIPT_s5), 
               NULL, RKH_STATE_CAST(&smIPT_s5), 
               targetStates, entryStates, exitStates, 
               RKH_STATE_CAST(&smIPT_s0), 1, TRN_NOT_INTERNAL, 
               NO_INIT_STATE_MACHINE, &evCompletion, 
               RKH_STATE_CAST(&smIPT_s5Final));

    rkh_sm_init((RKH_SM_T *)me);
    rkh_sm_dispatch((RKH_SM_T *)me, &evE);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_InitPseudostateTrnToJunctionToSimpleState(void)
{
    UtrzProcessOut *p;
    SmInitialPseudoTest *me = RKH_CAST(SmInitialPseudoTest, 
                                       smInitialPseudoTest);
    TargetEntrySt tgEnSt[4] = {{(RKH_ST_T *)&smIPT_s6, (RKH_ST_T *)&smIPT_s6},
                               {(RKH_ST_T *)&smIPT_junction1, NULL},
                               {(RKH_ST_T *)&smIPT_s62, (RKH_ST_T *)&smIPT_s62},
                               {NULL, NULL}};

    stateList_create(exitStates, 1, &smIPT_s0);

    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smIPT_nS0_Expect((SmInitialPseudoTest *)me);
    smIPT_xS0_Expect((SmInitialPseudoTest *)me);
    smIPT_nS6_Expect((SmInitialPseudoTest *)me);
    smIPT_isC1_ExpectAndReturn((RKH_SM_T *)me, &evF, RKH_TRUE);
    smIPT_tr3_Expect((SmInitialPseudoTest *)me, &evF);
    smIPT_nS62_Expect((SmInitialPseudoTest *)me);

    trnStepExpect((RKH_SM_T *)me, (RKH_ST_T *)&smIPT_s0, 
                  (RKH_ST_T *)&smIPT_s0, tgEnSt, exitStates, &evF);

    rkh_sm_init((RKH_SM_T *)me);
    rkh_sm_dispatch((RKH_SM_T *)me, &evF);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_InitPseudostateTrnToJunctionToCmpState(void)
{
    UtrzProcessOut *p;
    SmInitialPseudoTest *me = RKH_CAST(SmInitialPseudoTest, 
                                       smInitialPseudoTest);
    TargetEntrySt tgEnSt[5] = {{(RKH_ST_T *)&smIPT_s6, (RKH_ST_T *)&smIPT_s6},
                               {(RKH_ST_T *)&smIPT_junction1, NULL},
                               {(RKH_ST_T *)&smIPT_s61, (RKH_ST_T *)&smIPT_s61},
                               {(RKH_ST_T *)&smIPT_s611, (RKH_ST_T *)&smIPT_s611},
                               {NULL, NULL}};

    stateList_create(exitStates, 1, &smIPT_s0);

    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smIPT_nS0_Expect((SmInitialPseudoTest *)me);
    smIPT_xS0_Expect((SmInitialPseudoTest *)me);
    smIPT_nS6_Expect((SmInitialPseudoTest *)me);
    smIPT_isC1_ExpectAndReturn((RKH_SM_T *)me, &evF, RKH_FALSE);
    smIPT_isC2_ExpectAndReturn((RKH_SM_T *)me, &evF, RKH_FALSE);
    smIPT_isC3_ExpectAndReturn((RKH_SM_T *)me, &evF, RKH_FALSE);
    smIPT_tr3_Expect((SmInitialPseudoTest *)me, &evF);
    smIPT_nS61_Expect((SmInitialPseudoTest *)me);
    smIPT_nS611_Expect((SmInitialPseudoTest *)me);

    trnStepExpect((RKH_SM_T *)me, (RKH_ST_T *)&smIPT_s0, 
                  (RKH_ST_T *)&smIPT_s0, tgEnSt, exitStates, &evF);

    rkh_sm_init((RKH_SM_T *)me);
    rkh_sm_dispatch((RKH_SM_T *)me, &evF);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_InitPseudostateSMInitialToSimpleState(void)
{
    UtrzProcessOut *p;
    RKH_SM_T *me = smInitial0;

    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smI0_nS0_Expect((SmInitialPseudoTest *)me);
    trnStepExpect((RKH_SM_T *)me, (RKH_ST_T *)&smI0_s0, NULL, NULL, 
                  NULL, &evCreation);

    rkh_sm_init(me);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_InitPseudostateSMInitialToCmpState(void)
{
    UtrzProcessOut *p;
    RKH_SM_T *me = smInitial1;

    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smI1_nS0_Expect((SmInitialPseudoTest *)me);
    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smI1_nS01_Expect((SmInitialPseudoTest *)me);
    sm_init_expect(RKH_STATE_CAST(&smI1_s0));
    sm_trn_expect(RKH_STATE_CAST(&smI1_s0), RKH_STATE_CAST(&smI1_s0));
    sm_tsState_expect(RKH_STATE_CAST(&smI1_s0));
    sm_enstate_expect(RKH_STATE_CAST(&smI1_s0));
    sm_tsState_expect(RKH_STATE_CAST(&smI1_s01));
    sm_enstate_expect(RKH_STATE_CAST(&smI1_s01));
    sm_nenex_expect(2, 0);
    sm_state_expect(RKH_STATE_CAST(&smI1_s01));
    sm_evtProc_expect();

    rkh_sm_init(me);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_InitPseudostateSMInitialToNestedSimpleState(void)
{
    UtrzProcessOut *p;
    RKH_SM_T *me = smInitial2;

    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smI2_nS0_Expect((SmInitialPseudoTest *)me);
    smI2_nS01_Expect((SmInitialPseudoTest *)me);
    sm_init_expect(RKH_STATE_CAST(&smI2_s01));
    sm_trn_expect(RKH_STATE_CAST(&smI2_s01), RKH_STATE_CAST(&smI2_s01));
    sm_tsState_expect(RKH_STATE_CAST(&smI2_s01));
    sm_enstate_expect(RKH_STATE_CAST(&smI2_s0));
    sm_enstate_expect(RKH_STATE_CAST(&smI2_s01));
    sm_nenex_expect(2, 0);
    sm_state_expect(RKH_STATE_CAST(&smI2_s01));
    sm_evtProc_expect();

    rkh_sm_init(me);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_InitPseudostateSMInitialToNestedCmpState(void)
{
    UtrzProcessOut *p;
    RKH_SM_T *me = smInitial3;

    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smI3_nS0_Expect((SmInitialPseudoTest *)me);
    smI3_nS01_Expect((SmInitialPseudoTest *)me);
    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smI3_nS011_Expect((SmInitialPseudoTest *)me);
    sm_init_expect(RKH_STATE_CAST(&smI3_s01));
    sm_trn_expect(RKH_STATE_CAST(&smI3_s01), RKH_STATE_CAST(&smI3_s01));
    sm_tsState_expect(RKH_STATE_CAST(&smI3_s01));
    sm_enstate_expect(RKH_STATE_CAST(&smI3_s0));
    sm_enstate_expect(RKH_STATE_CAST(&smI3_s01));
    sm_tsState_expect(RKH_STATE_CAST(&smI3_s011));
    sm_enstate_expect(RKH_STATE_CAST(&smI3_s011));
    sm_nenex_expect(3, 0);
    sm_state_expect(RKH_STATE_CAST(&smI3_s011));
    sm_evtProc_expect();

    rkh_sm_init(me);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_InitPseudostateSMInitialToJunctionToSimpleState(void)
{
    UtrzProcessOut *p;
    RKH_SM_T *me = smInitial4;

    smIPT_isC1_ExpectAndReturn((RKH_SM_T *)me, (RKH_EVT_T *)&evCreation, 
                               RKH_TRUE);
    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smIPT_tr1_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smI4_nS0_Expect((SmInitialPseudoTest *)me);
    sm_init_expect(RKH_STATE_CAST(&smI4_junction1));
    sm_trn_expect(RKH_STATE_CAST(&smI4_junction1), 
                  RKH_STATE_CAST(&smI4_junction1));
    sm_tsState_expect(RKH_STATE_CAST(&smI4_junction1));
    sm_tsState_expect(RKH_STATE_CAST(&smI4_s0));
    sm_enstate_expect(RKH_STATE_CAST(&smI4_s0));
    sm_nenex_expect(1, 0);
    sm_state_expect(RKH_STATE_CAST(&smI4_s0));
    sm_evtProc_expect();

    rkh_sm_init(me);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_InitPseudostateSMInitialToJunctionToCmpState(void)
{
    UtrzProcessOut *p;
    RKH_SM_T *me = smInitial4;

    smIPT_isC1_ExpectAndReturn((RKH_SM_T *)me, (RKH_EVT_T *)&evCreation, 
                               RKH_FALSE);
    smIPT_isC2_ExpectAndReturn((RKH_SM_T *)me, (RKH_EVT_T *)&evCreation, 
                               RKH_TRUE);
    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smIPT_tr2_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smI4_nS1_Expect((SmInitialPseudoTest *)me);
    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smI4_nS11_Expect((SmInitialPseudoTest *)me);
    sm_init_expect(RKH_STATE_CAST(&smI4_junction1));
    sm_trn_expect(RKH_STATE_CAST(&smI4_junction1), 
                  RKH_STATE_CAST(&smI4_junction1));
    sm_tsState_expect(RKH_STATE_CAST(&smI4_junction1));
    sm_tsState_expect(RKH_STATE_CAST(&smI4_s1));
    sm_enstate_expect(RKH_STATE_CAST(&smI4_s1));
    sm_tsState_expect(RKH_STATE_CAST(&smI4_s11));
    sm_enstate_expect(RKH_STATE_CAST(&smI4_s11));
    sm_nenex_expect(2, 0);
    sm_state_expect(RKH_STATE_CAST(&smI4_s11));
    sm_evtProc_expect();

    rkh_sm_init(me);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_InitPseudostateSMInitialToBranchToSimpleState(void)
{
    UtrzProcessOut *p;
    RKH_SM_T *me = smInitial5;

    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smIPT_isC1_ExpectAndReturn((RKH_SM_T *)me, (RKH_EVT_T *)&evCreation, 
                               RKH_TRUE);
    smIPT_tr1_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smI5_nS0_Expect((SmInitialPseudoTest *)me);
    sm_init_expect(RKH_STATE_CAST(&smI5_branch1));
    sm_trn_expect(RKH_STATE_CAST(&smI5_branch1), 
                  RKH_STATE_CAST(&smI5_branch1));
    sm_tsState_expect(RKH_STATE_CAST(&smI5_branch1));
    sm_tsState_expect(RKH_STATE_CAST(&smI5_s0));
    sm_enstate_expect(RKH_STATE_CAST(&smI5_s0));
    sm_nenex_expect(1, 0);
    sm_state_expect(RKH_STATE_CAST(&smI5_s0));
    sm_evtProc_expect();

    rkh_sm_init(me);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_InitPseudostateSMInitialToBranchToCmpState(void)
{
    UtrzProcessOut *p;
    RKH_SM_T *me = smInitial5;

    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smIPT_isC1_ExpectAndReturn((RKH_SM_T *)me, (RKH_EVT_T *)&evCreation, 
                               RKH_FALSE);
    smIPT_isC2_ExpectAndReturn((RKH_SM_T *)me, (RKH_EVT_T *)&evCreation, 
                               RKH_TRUE);
    smIPT_tr2_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smI5_nS1_Expect((SmInitialPseudoTest *)me);
    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smI5_nS11_Expect((SmInitialPseudoTest *)me);
    sm_init_expect(RKH_STATE_CAST(&smI5_branch1));
    sm_trn_expect(RKH_STATE_CAST(&smI5_branch1), 
                  RKH_STATE_CAST(&smI5_branch1));
    sm_tsState_expect(RKH_STATE_CAST(&smI5_branch1));
    sm_tsState_expect(RKH_STATE_CAST(&smI5_s1));
    sm_enstate_expect(RKH_STATE_CAST(&smI5_s1));
    sm_tsState_expect(RKH_STATE_CAST(&smI5_s11));
    sm_enstate_expect(RKH_STATE_CAST(&smI5_s11));
    sm_nenex_expect(2, 0);
    sm_state_expect(RKH_STATE_CAST(&smI5_s11));
    sm_evtProc_expect();

    rkh_sm_init(me);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_InitPseudostateTrnToInitialToSimpleStateWithNullTrn(void)
{
    UtrzProcessOut *p;
    SmInitialPseudoTest *me = (SmInitialPseudoTest *)smInitialPseudoTest;
    TargetEntrySt tgEnSt[3] = {{(RKH_ST_T *)&smIPT_s7, (RKH_ST_T *)&smIPT_s7},
                               {(RKH_ST_T *)&smIPT_s71, (RKH_ST_T *)&smIPT_s71},
                               {NULL, NULL}};

    stateList_create(exitStates, 1, &smIPT_s0);

    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smIPT_nS0_Expect((SmInitialPseudoTest *)me);
    smIPT_xS0_Expect((SmInitialPseudoTest *)me);
    smIPT_nS7_Expect((SmInitialPseudoTest *)me);
    smIPT_nS71_Expect((SmInitialPseudoTest *)me);
    smIPT_xS71_Expect((SmInitialPseudoTest *)me);
    smIPT_tr4_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCompletion);
    smIPT_nS72_Expect((SmInitialPseudoTest *)me);

    trnStepExpect((RKH_SM_T *)me, (RKH_ST_T *)&smIPT_s0, 
                  (RKH_ST_T *)&smIPT_s0, tgEnSt, exitStates, &evG);

    /* Transition (null or completion) from s71 to s72 */
    sm_dch_expect(evCompletion.e, RKH_STATE_CAST(&smIPT_s71));
    sm_trn_expect(RKH_STATE_CAST(&smIPT_s71), RKH_STATE_CAST(&smIPT_s72));
    sm_tsState_expect(RKH_STATE_CAST(&smIPT_s72));
    sm_exstate_expect(RKH_STATE_CAST(&smIPT_s71));
    sm_enstate_expect(RKH_STATE_CAST(&smIPT_s72));
    sm_nenex_expect(1, 1);
    sm_state_expect(RKH_STATE_CAST(&smIPT_s72));
    sm_evtProc_expect();

    rkh_sm_init((RKH_SM_T *)me);
    rkh_sm_dispatch((RKH_SM_T *)me, &evG);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

void
test_InitPseudostateSMInitialToSimpleStateWithNullTrn(void)
{
    UtrzProcessOut *p;
    SmInitialPseudoTest *me = (SmInitialPseudoTest *)smInitial6;

    smIPT_init_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCreation);
    smIPT_tr4_Expect((SmInitialPseudoTest *)me, (RKH_EVT_T *)&evCompletion);
    trnStepExpect((RKH_SM_T *)me, (RKH_ST_T *)&smI6_s0, NULL, NULL, 
                  NULL, &evCreation);

    /* Transition (null or completion) from s0 to s1 */
    sm_dch_expect(evCompletion.e, RKH_STATE_CAST(&smI6_s0));
    sm_trn_expect(RKH_STATE_CAST(&smI6_s0), RKH_STATE_CAST(&smI6_s1));
    sm_tsState_expect(RKH_STATE_CAST(&smI6_s1));
    sm_exstate_expect(RKH_STATE_CAST(&smI6_s0));
    sm_enstate_expect(RKH_STATE_CAST(&smI6_s1));
    sm_nenex_expect(1, 1);
    sm_state_expect(RKH_STATE_CAST(&smI6_s1));
    sm_evtProc_expect();

    rkh_sm_init((RKH_SM_T *)me);

    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */

/* ------------------------------ End of file ------------------------------ */
