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
 *  \file       test_rkhsma.c
 *  \ingroup    test_sma
 *  \brief      Unit test for sma module.
 *
 *  \addtogroup test
 *  @{
 *  \addtogroup test_sma SMA
 *  @{
 *  \brief      Unit test for sma module.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2017.04.05  LeFr  v2.4.05  ---
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "unity.h"
#include "rkhsma.h"
#include "smInstance.h"     /* support files */
#include "smPolymorphism.h"
#include "Mock_rkhport.h"
#include "Mock_rkhtrc.h"
#include "Mock_rkhtrc_record.h"
#include "Mock_rkhtrc_filter.h"
#include "Mock_rkhsm.h"
#include "Mock_rkhqueue.h"
#include "Mock_rkhassert.h"
#include "Mock_rkhtmr.h"
#include "Mock_bsp.h"
#include <string.h>

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static RKHROM RKH_ROM_T base = {0, 0, "receiver"};
static RKH_SMA_T receiver;
const RKH_TRC_FIL_T fsma = {0, NULL};   /* Fake global variable of trace */
                                        /* module (using for mocking) */
const RKH_TRC_FIL_T fsig = {0, NULL};
static RKH_EVT_T event = {0, 0, 0};                                       

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
#if RKH_CFG_SMA_VFUNCT_EN == RKH_ENABLED
static void
checkVtbl(RKH_SMA_T *me, RKHActivate activate, RKHTask task, 
          RKHPostFifo postFifo, RKHPostLifo postLifo)
{
    TEST_ASSERT_EQUAL_PTR(activate, me->vptr->activate);
    TEST_ASSERT_EQUAL_PTR(task, me->vptr->task);
    TEST_ASSERT_EQUAL_PTR(postFifo, me->vptr->post_fifo);
    TEST_ASSERT_EQUAL_PTR(postLifo, me->vptr->post_lifo);
}

static void
testActivate(RKH_SMA_T *me, const RKH_EVT_T **qSto, RKH_QUENE_T qSize,
             void *stkSto, rui32_t stkSize)
{
    (void)me;
    (void)qSto;
    (void)qSize;
    (void)stkSto;
    (void)stkSize;
}

static void
testTask(RKH_SMA_T *me, void *arg)
{
    (void)me;
    (void)arg;
}

static void
testPostFifo(RKH_SMA_T *me, const RKH_EVT_T *e, const void *const sender)
{
    (void)me;
    (void)e;
    (void)sender;
}

static void
testPostLifo(RKH_SMA_T *me, const RKH_EVT_T *e, const void *const sender)
{
    (void)me;
    (void)e;
    (void)sender;
}
#endif

static void
setUp_polymorphism(void)
{
#if RKH_CFG_SMA_VFUNCT_EN == RKH_ENABLED
    /* Restore the default virtual table of RKH_SMA_T class */
    singleton->vptr = &rkhSmaVtbl;
#endif
    memset(rkh_sptbl, 0, sizeof(rkh_sptbl));
}

/* ---------------------------- Global functions --------------------------- */
void
setUp(void)
{
    receiver.sm.romrkh = &base;
    Mock_rkhtrc_filter_Init();
}

void
tearDown(void)
{
    Mock_rkhtrc_filter_Verify();
    Mock_rkhtrc_filter_Destroy();
}

/**
 *  \addtogroup test_rkhsma Active object (SMA) test group
 *  @{
 *  \name Test cases of active object test group
 *  @{ 
 */
void
test_Register(void)
{
    rkh_enter_critical_Expect();
    rkh_trc_isoff__ExpectAndReturn(RKH_TE_SMA_REG, RKH_FALSE);
    rkh_exit_critical_Expect();

    rkh_sma_register(&receiver);
    TEST_ASSERT_EQUAL(&receiver, rkh_sptbl[receiver.sm.romrkh->prio]);
}

void
test_UnRegister(void)
{
    rkh_enter_critical_Expect();
    rkh_trc_isoff__ExpectAndReturn(RKH_TE_SMA_UNREG, RKH_FALSE);
    rkh_exit_critical_Expect();

    rkh_sma_unregister(&receiver);
    TEST_ASSERT_EQUAL((RKH_SMA_T *)0, rkh_sptbl[receiver.sm.romrkh->prio]);
}

void
test_Constructor(void)
{
    rkh_sm_ctor_Expect(&(receiver.sm));

    rkh_sma_ctor(&receiver, (const RKHSmaVtbl *)0);
}

void
test_TerminateOneRegisteredAO(void)
{
    rkh_enter_critical_Expect();
    rkh_trc_isoff__ExpectAndReturn(RKH_TE_SMA_REG, RKH_FALSE);
    rkh_exit_critical_Expect();

    rkh_sma_register(&receiver);

    rkh_enter_critical_Expect();
    rkh_trc_isoff__ExpectAndReturn(RKH_TE_SMA_UNREG, RKH_FALSE);
    rkh_exit_critical_Expect();
    rkh_trc_isoff__ExpectAndReturn(RKH_TE_SMA_TERM, RKH_FALSE);

    rkh_sma_terminate(&receiver);

    TEST_ASSERT_EQUAL((RKH_SMA_T *)0, rkh_sptbl[receiver.sm.romrkh->prio]);
}

void
test_ActivateOneAO(void)
{
    char *buff;

    rkh_queue_init_Expect(&receiver.equeue, (const void **)&buff, 16, 
                          &receiver);

    rkh_enter_critical_Expect();
    rkh_trc_isoff__ExpectAndReturn(RKH_TE_SMA_REG, RKH_FALSE);
    rkh_exit_critical_Expect();

    rkh_sm_init_Expect((RKH_SM_T *)&receiver);

    rkh_trc_isoff__ExpectAndReturn(RKH_TE_SMA_ACT, RKH_FALSE);

    rkh_sma_activate(&receiver, (const RKH_EVT_T **)&buff, 16, NULL, 0);
}

void
test_PostFifo(void)
{
    rkh_enter_critical_Expect();
    rkh_queue_put_fifo_Expect(&receiver.equeue, &event);
    rkh_trc_isoff__ExpectAndReturn(RKH_TE_SMA_FIFO, RKH_FALSE);
    rkh_exit_critical_Expect();

    rkh_sma_post_fifo(&receiver, &event, &receiver);
}

void
test_PostLifo(void)
{
    rkh_enter_critical_Expect();
    rkh_queue_put_lifo_Expect(&receiver.equeue, &event);
    rkh_trc_isoff__ExpectAndReturn(RKH_TE_SMA_LIFO, RKH_FALSE);
    rkh_exit_critical_Expect();

    rkh_sma_post_lifo(&receiver, &event, &receiver);
}

void
test_Get(void)
{
    RKH_EVT_T *e;

    rkh_queue_get_ExpectAndReturn(&receiver.equeue, &event);
    rkh_trc_isoff__ExpectAndReturn(RKH_TE_SMA_GET, RKH_FALSE);

    e = rkh_sma_get(&receiver);
    TEST_ASSERT_EQUAL(&event, e);
}

void
test_Defer(void)
{
    rkh_enter_critical_Expect();
    rkh_queue_put_fifo_Expect(&receiver.equeue, &event);
    rkh_trc_isoff__ExpectAndReturn(RKH_TE_SMA_DEFER, RKH_FALSE);
    rkh_exit_critical_Expect();

    rkh_sma_defer(&receiver.equeue, &event);
}

void
test_Recall(void)
{
    RKH_EVT_T *e;

    rkh_queue_get_ExpectAndReturn(&receiver.equeue, NULL);

    e = rkh_sma_recall(&receiver, &receiver.equeue);
    TEST_ASSERT_EQUAL(NULL, e);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */

/**
 *  \addtogroup test_smInstance Instance test group
 *  @{
 *  \name Test cases of instance group
 *  @{ 
 */
void
test_ctorOfStaticPrivateAO(void)
{
    Single_ctor(4);

    TEST_ASSERT_EQUAL_STRING("single", RKH_SMA_ACCESS_CONST(single, name));
    TEST_ASSERT_EQUAL(4, Single_getFoo());
}

void
test_staticPublicAOWithoutRuntimeCtor(void)
{
    publicSingle->foo = 8;
    TEST_ASSERT_EQUAL_STRING("publicSingle", 
                             RKH_SMA_ACCESS_CONST(publicSingle, name));
}

void
test_ctorOfStaticPublicAO(void)
{
    PublicSingle_ctor(8);

    TEST_ASSERT_EQUAL_STRING("publicSingle", 
                             RKH_SMA_ACCESS_CONST(publicSingle, name));
    TEST_ASSERT_EQUAL(8, publicSingle->foo);
}

void
test_ctorOfStaticOpaqueAO(void)
{
    Opaque_ctor(opaque, 4);

    TEST_ASSERT_EQUAL(4, Opaque_getFoo(opaque));
}

void
test_ctorOfStaticMultipleAO(void)
{
    MultiplePublicSingle_ctor(single0, 8);

    TEST_ASSERT_EQUAL_STRING("single0", 
                             RKH_SMA_ACCESS_CONST(single0, name));
    TEST_ASSERT_EQUAL(8, single0->foo);
}

void
test_ctorOfStaticArrayOfAO(void)
{
    PublicSingle *pSingle = RKH_ARRAY_SMA(arrayOfSingles, 2);
    MultiplePublicSingle_ctor(pSingle, 8);

    TEST_ASSERT_EQUAL_STRING("single2", 
                             RKH_SMA_ACCESS_CONST(pSingle, name));
    TEST_ASSERT_EQUAL(8, pSingle->foo);
}

void
test_staticPrivateSMWithoutRuntimeCtor(void)
{
    TEST_ASSERT_EQUAL_STRING("stateMachine", 
                             RKH_SMA_ACCESS_CONST(stateMachine, name));
    TEST_ASSERT_EQUAL_PTR(NULL, stateMachine->state);
}

void
test_staticPublicSMWithoutRuntimeCtor(void)
{
    TEST_ASSERT_EQUAL_STRING("publicStateMachine", 
                             RKH_SMA_ACCESS_CONST(publicStateMachine, name));
    TEST_ASSERT_EQUAL(8, publicSingle->foo);
}

void
test_ctorOfStaticCompositePrivateSingletonAO(void)
{
    Composite_ctor(16);

    TEST_ASSERT_EQUAL_STRING("composite", 
                             RKH_SMA_ACCESS_CONST(composite, name));
    TEST_ASSERT_EQUAL_STRING("region", 
                 RKH_SMA_ACCESS_CONST(Composite_getItsReactivePart(), name));
    TEST_ASSERT_EQUAL(16, Composite_getFoo());
}

void
test_ctorOfStaticCompositePublicAO(void)
{
    PublicComposite_ctor(publicComposite, 16);

    TEST_ASSERT_EQUAL_STRING("publicComposite", 
                             RKH_SMA_ACCESS_CONST(publicComposite, name));
    TEST_ASSERT_EQUAL_STRING("publicRegion", 
             RKH_SMA_ACCESS_CONST(&publicComposite->itsReactivePart, name));
    TEST_ASSERT_EQUAL(16, publicComposite->foo);
}

void
test_ctorOfStaticCompositeAOWithDerivedPublicSM(void)
{
    PublicCompositeA_ctor(publicCompositeA, 16, 8);

    TEST_ASSERT_EQUAL_STRING("publicCompositeA", 
                             RKH_SMA_ACCESS_CONST(publicCompositeA, name));
    TEST_ASSERT_EQUAL_STRING("publicRegionA", 
             RKH_SMA_ACCESS_CONST(&publicCompositeA->itsReactivePart, name));
    TEST_ASSERT_EQUAL(16, publicCompositeA->foo);
    TEST_ASSERT_EQUAL(8, publicCompositeA->itsReactivePart.foo);
}

void
test_ctorOfDynamicCompositeAO(void)
{
    PublicSingle *actObj;

    actObj = PublicSingle_dynCtor(8);
    TEST_ASSERT_NOT_NULL(actObj);
    TEST_ASSERT_EQUAL(8, actObj->foo);

    TEST_ASSERT_EQUAL_STRING("publicSingleDyn", 
                             RKH_SMA_ACCESS_CONST(actObj, name));

    PublicSingle_dynDtor(actObj);
}

void
test_ctorOfANonReactiveAO(void)
{
    RKH_SMA_T* realSignalMgr = RKH_UPCAST(RKH_SMA_T, signalMgr);

    setUp_polymorphism();
    rkh_sm_ctor_Expect(RKH_UPCAST(RKH_SM_T, signalMgr));

    PerNonReactWoutST_ctor(signalMgr, 4);

    TEST_ASSERT_EQUAL(4, PerNonReactWoutST_getBaz(signalMgr));
    TEST_ASSERT_NULL(realSignalMgr->sm.state);
}

void
test_invokeActivateOfANonReactiveAO(void)
{
    RKH_SMA_T* realSignalMgr = RKH_UPCAST(RKH_SMA_T, signalMgr);
    const RKH_EVT_T *qs[1];

    setUp_polymorphism();
    rkh_sm_ctor_Expect(RKH_UPCAST(RKH_SM_T, signalMgr));
    rkh_queue_init_Ignore();
    rkh_enter_critical_Expect();
    rkh_trc_isoff__ExpectAndReturn(RKH_TE_SMA_REG, RKH_FALSE);
    rkh_exit_critical_Expect();
    rkh_trc_isoff__ExpectAndReturn(RKH_TE_SMA_ACT, RKH_FALSE);

    PerNonReactWoutST_ctor(signalMgr, 4);
    RKH_SMA_ACTIVATE(RKH_UPCAST(RKH_SMA_T, signalMgr), qs, 1, NULL, 0);
}

void
test_invokeDispatchOfANonReactiveAO(void)
{
    RKH_SMA_T* realSignalMgr = RKH_UPCAST(RKH_SMA_T, signalMgr);
    const RKH_EVT_T *qs[1];
    RKH_STATIC_EVENT(evt, sigSync); 

    setUp_polymorphism();
    rkh_sm_ctor_Expect(RKH_UPCAST(RKH_SM_T, signalMgr));
    rkh_trc_isoff__ExpectAndReturn(RKH_TE_SM_DCH, RKH_FALSE);
    Sensor_get_ExpectAndReturn(48);
    Sensor_process_ExpectAndReturn(48, 4);
    Actuator_set_Expect(4);

    PerNonReactWoutST_ctor(signalMgr, 4);
    RKH_SMA_DISPATCH(RKH_UPCAST(RKH_SMA_T, signalMgr), &evt);
}

void
test_ctorOfANonReactiveAOWithSTButWoutUsingEventQueue(void)
{
    RKH_SMA_T* realSignalMgr = RKH_UPCAST(RKH_SMA_T, collector);

    setUp_polymorphism();
    rkh_sm_ctor_Expect(RKH_UPCAST(RKH_SM_T, collector));
    rkh_tmr_init__Expect(NULL, NULL);
    rkh_tmr_init__IgnoreArg_t();
    rkh_tmr_init__IgnoreArg_e();

    PerNonReactWithSTWoutQue_ctor(collector, 4);

    TEST_ASSERT_EQUAL(4, PerNonReactWithSTWoutQue_getBaz(collector));
}

void
test_invokeActivateOfANonReactiveAOWithSTButWoutUsingEventQueue(void)
{
    RKH_SMA_T* realSignalMgr = RKH_UPCAST(RKH_SMA_T, collector);
    const RKH_EVT_T *qs[1];

    setUp_polymorphism();
    rkh_sm_ctor_Expect(RKH_UPCAST(RKH_SM_T, collector));
    rkh_tmr_init__Expect(NULL, NULL);
    rkh_tmr_init__IgnoreArg_t();
    rkh_tmr_init__IgnoreArg_e();
    rkh_queue_init_Ignore();
    rkh_enter_critical_Expect();
    rkh_trc_isoff__ExpectAndReturn(RKH_TE_SMA_REG, RKH_FALSE);
    rkh_exit_critical_Expect();
    rkh_sm_init_Expect(RKH_UPCAST(RKH_SM_T, collector));
    rkh_tmr_start_Expect(NULL, realSignalMgr, 4, 4);
    rkh_tmr_start_IgnoreArg_t();
    rkh_trc_isoff__ExpectAndReturn(RKH_TE_SMA_ACT, RKH_FALSE);

    PerNonReactWithSTWoutQue_ctor(collector, 4);
    RKH_SMA_ACTIVATE(RKH_UPCAST(RKH_SMA_T, collector), qs, 1, NULL, 0);
}

void
test_invokeDispatchOfANonReactiveAOWithSTButWoutUsingEventQueue(void)
{
    RKH_SMA_T* realSignalMgr = RKH_UPCAST(RKH_SMA_T, collector);
    const RKH_EVT_T *qs[1];
    RKH_STATIC_EVENT(evtSync, sigSync); 

    setUp_polymorphism();
    rkh_sm_ctor_Expect(RKH_UPCAST(RKH_SM_T, collector));
    rkh_tmr_init__Expect(NULL, NULL);
    rkh_tmr_init__IgnoreArg_t();
    rkh_tmr_init__IgnoreArg_e();
    rkh_queue_init_Ignore();
    rkh_enter_critical_Expect();
    rkh_trc_isoff__ExpectAndReturn(RKH_TE_SMA_REG, RKH_FALSE);
    rkh_exit_critical_Expect();
    rkh_sm_init_Expect(RKH_UPCAST(RKH_SM_T, collector));
    rkh_tmr_start_Expect(NULL, realSignalMgr, 4, 4);
    rkh_tmr_start_IgnoreArg_t();
    rkh_trc_isoff__ExpectAndReturn(RKH_TE_SMA_ACT, RKH_FALSE);
    rkh_trc_isoff__ExpectAndReturn(RKH_TE_SM_DCH, RKH_FALSE);
    Sensor_get_ExpectAndReturn(1.3);
    Sensor_process_ExpectAndReturn(1.3, 1.2);
    rkh_sm_dispatch_ExpectAndReturn(RKH_UPCAST(RKH_SM_T, collector),
                                    NULL,
                                    RKH_EVT_PROC);
    rkh_sm_dispatch_IgnoreArg_e();

    PerNonReactWithSTWoutQue_ctor(collector, 4);
    RKH_SMA_ACTIVATE(RKH_UPCAST(RKH_SMA_T, collector), qs, 1, NULL, 0);
    RKH_SMA_DISPATCH(RKH_UPCAST(RKH_SMA_T, collector), &evtSync);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */

/**
 *  \addtogroup test_smPolymorphism Polymorphism test group
 *  @{
 *  \name Test cases of polimorphism group
 *  @{
 */
void
test_defaultVirtualFunctions(void)
{
    setUp_polymorphism();
#if RKH_CFG_SMA_VFUNCT_EN == RKH_ENABLED
    checkVtbl(singleton, 
              rkh_sma_activate, rkh_sma_dispatch, rkh_sma_post_fifo, 
              rkh_sma_post_lifo);

    TEST_ASSERT_EQUAL_PTR(&rkhSmaVtbl, singleton->vptr);
#endif
}

void
test_callVirtualFunction(void)
{
    setUp_polymorphism();
#if RKH_CFG_SMA_VFUNCT_EN == RKH_ENABLED
    const RKH_EVT_T *qs[1];

    rkh_queue_init_Ignore();   /* for RKH_SMA_ACTIVATE() */
    rkh_enter_critical_Expect();
    rkh_trc_isoff__ExpectAndReturn(RKH_TE_SMA_REG, RKH_FALSE);
    rkh_exit_critical_Expect();
    rkh_sm_init_Ignore();
    rkh_trc_isoff__ExpectAndReturn(RKH_TE_SMA_ACT, RKH_FALSE);

    rkh_enter_critical_Expect();
    rkh_queue_put_fifo_Ignore();
    rkh_trc_isoff__ExpectAndReturn(RKH_TE_SMA_FIFO, RKH_FALSE);
    rkh_exit_critical_Expect();

    rkh_enter_critical_Expect();
    rkh_queue_put_lifo_Ignore();
    rkh_trc_isoff__ExpectAndReturn(RKH_TE_SMA_LIFO, RKH_FALSE);
    rkh_exit_critical_Expect();

    RKH_SMA_ACTIVATE(singleton, qs, 1, 0, 0);
    RKH_SMA_POST_FIFO(singleton, &event, NULL);
    RKH_SMA_POST_LIFO(singleton, &event, NULL);
#endif
}

void
test_setVirtualTable(void)
{
    setUp_polymorphism();
#if RKH_CFG_SMA_VFUNCT_EN == RKH_ENABLED
    const RKHSmaVtbl *vptr;
    static const RKHSmaVtbl vtbl =
    {
        testActivate, testTask, testPostFifo, testPostLifo
    };

    vptr = singleton->vptr = &vtbl;

    checkVtbl(singleton, 
              testActivate, testTask, testPostFifo, testPostLifo);
#endif
}

void
test_runtimeSingletonAOCtor(void)
{
    setUp_polymorphism();
#if RKH_CFG_SMA_VFUNCT_EN == RKH_ENABLED
    rkh_sm_ctor_Expect(&singleton->sm);

    Singleton_ctor(8);
    TEST_ASSERT_EQUAL(8, Singleton_getFoo());

    RKH_SMA_ACTIVATE(singleton, NULL, 0, NULL, 0);
    TEST_ASSERT_EQUAL(0, Singleton_getFoo());
#endif
}

void
test_runtimeMultipleAOCtorWithVtblForObj(void)
{
    setUp_polymorphism();
#if RKH_CFG_SMA_VFUNCT_EN == RKH_ENABLED
    rkh_sm_ctor_Ignore();
    rkh_sm_ctor_Ignore();

    Multiple_ctor(multA, 2, Multiple_postFifoA);
    Multiple_ctor(multB, 4, Multiple_postFifoB);

    checkVtbl((RKH_SMA_T *)multA, 
              rkh_sma_activate, rkh_sma_dispatch, Multiple_postFifoA, 
              rkh_sma_post_lifo);

    TEST_ASSERT_EQUAL(2, Multiple_getFoobar(multA));
    TEST_ASSERT_EQUAL(4, Multiple_getFoobar(multB));

    RKH_SMA_POST_FIFO((RKH_SMA_T *)multA, NULL, NULL);
    RKH_SMA_POST_FIFO((RKH_SMA_T *)multB, NULL, NULL);

    TEST_ASSERT_EQUAL(0, Multiple_getFoobar(multA));
    TEST_ASSERT_EQUAL(8, Multiple_getFoobar(multB));
#endif
}

void
test_runtimeMultipleAOCtorWithVtblForType(void)
{
    setUp_polymorphism();
#if RKH_CFG_SMA_VFUNCT_EN == RKH_ENABLED
    rkh_sm_ctor_Expect((RKH_SM_T *)cmdSignal);
    rkh_sm_ctor_Expect((RKH_SM_T *)cmdRegister);

    Command_ctor(cmdSignal, 128); 
    Command_ctor(cmdRegister, 64); 

    checkVtbl((RKH_SMA_T *)cmdSignal, 
              rkh_sma_activate, Command_task, 
              Command_postFifo, Command_postLifo);

    checkVtbl((RKH_SMA_T *)cmdRegister, 
              rkh_sma_activate, Command_task, 
              Command_postFifo, Command_postLifo);
#endif
}

void
test_runtimeSubObjectCtorOfSMAAndSM(void)
{
    setUp_polymorphism();
#if RKH_CFG_SMA_VFUNCT_EN == RKH_ENABLED
    rkh_sm_ctor_Expect((RKH_SM_T *)theCallControl);
    CallControl_ctorA(16);

    TEST_ASSERT_EQUAL(16, CallControl_getFoo());
    checkVtbl((RKH_SMA_T *)theCallControl, 
              CallControl_activate, CallControl_task, 
              rkh_sma_post_fifo, rkh_sma_post_lifo);
#endif
}

void
test_runtimeSubObjectCtorOfSMAAndSMWithDefaultVtbl(void)
{
    setUp_polymorphism();
#if RKH_CFG_SMA_VFUNCT_EN == RKH_ENABLED
    rkh_sm_ctor_Expect((RKH_SM_T *)theCallControl);
    CallControl_ctorB(8);

    TEST_ASSERT_EQUAL(8, CallControl_getFoo());
    checkVtbl((RKH_SMA_T *)theCallControl, 
              rkh_sma_activate, rkh_sma_dispatch, 
              rkh_sma_post_fifo, rkh_sma_post_lifo);
#endif
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/* ------------------------------ End of file ------------------------------ */
