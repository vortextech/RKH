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
 *  \file       rkhsm.h
 *  \brief      Specifies the interface of the state-machine manager. 
 *
 *  \ingroup    apiSM
 *  \ingroup    apiAO
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2017.04.14  LeFr  v2.4.05  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __RKHSM_H__
#define __RKHSM_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkhevt.h"
#include "rkhitl.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
#define RKH_SM_NAME(smName_)         s_##smName_
#define RKH_SM_CONST_NAME(smName_)   rs_##smName_

/**
 *  \brief
 *  Each condition connector can have one special branch with a guard
 *	labeled ELSE, which is taken if all the guards on the other
 *	branches are false.
 *
 *  A guard function takes the state machine pointer and the event
 *  pointer as arguments.
 *  These arguments are optional, thus they could be eliminated in
 *  compile-time by means of RKH_EN_GRD_EVT_ARG and RKH_EN_GRD_HSM_ARG
 *  preprocessor directives.
 */
#define ELSE        rkh_sm_else

/**
 *  \brief
 *	This macro creates a composite state with a single region, including its 
 *	own initial pseudostate, history (shallow and deep type) pseudostate, as 
 *	well as its own final state.
 *
 *	The names of history pseudostate and final state are internally conformed 
 *	as "<nameOfCompositeState>Hist" and "<nameOfCompositeState>Final" 
 *	respectively. Thus, use these names as target vertex in a transition. 
 *
 *	Shallow history means that history applies to the current nesting context
 *	only states nested more deeply are not affected by the presence of a
 *	history pseudostates in a higher context.
 *	Deep history applies downwards to all levels of nesting.
 *
 *  \param[in] name     state name. Represents a composite state structure.
 *  \param[in] en       pointer to state entry action. This argument is
 *                      optional, thus it could be declared as NULL.
 *                      The RKH implementation preserves the transition sequence
 *                      imposed by Harel's Statechart and UML.
 *  \param[in] ex	    pointer to state exit action. This argument is
 *                      optional, thus it could be declared as NULL.
 *                      The RKH implementation preserves the transition sequence
 *                      imposed by Harel's Statechart and UML.
 *  \param[in] parent	pointer to parent state.
 *  \param[in] defchild	pointer to default child state or pseudostate.
 *  \param[in] initialTrn
 *                      pointer to initial transition action of the composite 
 *                      state's region. This argument is optional, thus it 
 *                      could be declared as NULL. The RKH implementation 
 *                      preserves the transition sequence imposed by UML.
 *  \param[in] kindOfHistory 
 *                      Kind of history pseudostate. It could be defined
 *                      either shallow history (RKH_SHISTORY), deep history 
 *                      (RKH_DHISTORY) or without history (RKH_NO_HISTORY).
 *                      When it is defined as RKH_NO_HISTORY each of 
 *                      parameters related to history default transition are 
 *                      ignored. The name of this history pseudostate is 
 *                      automatically generated from the name of composite 
 *                      state parent as "<composite_state_name>Hist".
 *  \param[in] hDftTrnGuard	
 *                      Pointer to guard function associated to history default 
 *                      transition. 
 *                      This argument is optional, thus it could be declared 
 *                      as NULL.
 *  \param[in] hDftTrnAction
 *                      Pointer to effect action function associated to 
 *                      history default transition. 
 *                      This argument is optional, thus it could be declared 
 *                      as NULL.
 *  \param[in] hDftTarget
 *                      Pointer to target vertex of history default transition.
 *                      If a default history transition is defined 
 *                      (the target parameter is not NULL) originating from 
 *                      the History Pseudostate, it will be taken. Otherwise, 
 *                      default State entry is applied. 
 *  \param[in] hRamMem
 *                      Pointer to a RAM location which maintains the last 
 *                      visited state of this region. If composite state don't 
 *                      include the history pseudostate then this parameter 
 *                      should be passed as NULL.
 *                      Please, use RKH_CREATE_HISTORY_STORAGE() macro to 
 *                      create the history RAM location.
 *
 *	\sa
 *	RKH_SCMP_T structure definition for more information.
 *
 *  \ingroup apiSM
 */
#define RKH_CREATE_COMP_REGION_STATE(name, en, ex, parent, defchild, \
                                     initialTrn, \
                                     kindOfHistory, hDftTrnGuard, \
                                     hDftTrnAction, hDftTarget, hRamMem) \
                                                                         \
    MKFINAL_INCOMP(name); \
    MKHIST_INCOMP(name, kindOfHistory, hDftTrnGuard, hDftTrnAction, \
                  hDftTarget, hRamMem); \
                                        \
    extern RKHROM RKH_TR_T name##_trtbl[]; \
    RKHROM RKH_SCMP_T name = \
    { \
        {MKBASE(RKH_COMPOSITE, name), MKST(en, ex, parent)}, \
        MKCOMP(name, defchild, initialTrn, &name##Hist) \
    }

/**
 *  \brief
 *	This macro creates a composite state.
 *
 *  \param[in] name     state name. Represents a composite state structure.
 *  \param[in] en       pointer to state entry action. This argument is
 *                      optional, thus it could be declared as NULL.
 *                      The RKH implementation preserves the transition sequence
 *                      imposed by Harel's Statechart and UML.
 *  \param[in] ex	    pointer to state exit action. This argument is
 *                      optional, thus it could be declared as NULL.
 *                      The RKH implementation preserves the transition sequence
 *                      imposed by Harel's Statechart and UML.
 *  \param[in] parent	pointer to parent state.
 *  \param[in] defchild	pointer to default child state or pseudostate.
 * 	\param[in] history	pointer history pseudostate. This argument is 
 *                      optional, thus it could be declared as NULL.
 *
 *	\sa
 *	RKH_SCMP_T structure definition for more information.
 *
 *  \ingroup apiSM
 */
#define RKH_CREATE_COMP_STATE(name, en, ex, parent, defchild, history) \
                                                                       \
    extern RKHROM RKH_TR_T name##_trtbl[]; \
    RKHROM RKH_SCMP_T name = \
    { \
        {MKBASE(RKH_COMPOSITE, name), MKST(en, ex, parent)}, \
        MKCOMP(name, defchild, NULL, history) \
    }

/**
 *  \brief
 *  This macro creates a final state.
 *
 *  \param[in] name     state name. Represents a composite state structure.
 *  \param[in] parent	pointer to parent state.
 *
 *	\sa
 *	RKH_FINAL_T structure definition for more information.
 *
 *  \ingroup apiSM
 */
#define RKH_CREATE_FINAL_STATE(name, parent) \
                                             \
    static RKHROM RKH_TR_T name##_trtbl[] = \
        RKH_TRREG(RKH_ANY, NULL, NULL, NULL); \
    RKHROM RKH_FINAL_T name = \
    { \
        {MKBASE(RKH_FINAL, name), MKST(NULL, NULL, parent)}, \
        MKFINAL(name) \
    }

/**
 *  \brief
 *	This macro creates a basic state.
 *
 *  \param[in] name	state name. Represents a basic state structure.
 *  \param[in] en	pointer to state entry action. This argument is
 *					optional, thus it could be declared as NULL.
 *					The RKH implementation preserves the transition sequence
 *					imposed by Harel's Statechart and UML.
 *  \param[in] ex	pointer to state exit action. This argument is
 *					optional, thus it could be declared as NULL.
 *					The RKH implementation preserves the transition sequence
 *					imposed by Harel's Statechart and UML.
 *  \param[in] parent	pointer to parent state.
 *  \param[in] prepro	pointer to input preprocessor function. This function
 *                  could be called "Moore" action.
 *                  This argument is optional, thus it could be declared
 *                  as NULL.
 *					Aditionally, by means of single inheritance in C it
 *					could be used as state's abstract data.
 *					Moreover, implementing the single inheritance in C
 *					is very simply by literally embedding the base type,
 *					RKH_PPRO_T in this case, as the first member of the
 *					derived structure. See \a prepro member of RKH_SBSC_T
 *					structure for more information. 
 *					\usage
 *                  \code
 *					static RKH_SIG_T
 *					preprocessor( RKH_EVT_T *pe )
 *					{
 *						...
 *					}
 *
 *					typedef struct
 *					{
 *						RKH_PPRO_T prepro;  // extend the RKH_PPRO_T class
 *						unsigned min:4;
 *						unsigned max:4;
 *						char *buff;
 *					} SDATA_T;
 *
 *					static const SDATA_T option = { preprocessor,4,8,token1 };
 *
 *					RKH_CREATE_BASIC_STATE( S111, set_x_1,
 *											NULL, &S11, preprocessor );
 *					RKH_CREATE_BASIC_STATE( S22, set_x_4,
 *											NULL, &S2,
 *											(RKH_PPRO_T*)&option );
 *					\endcode
 *
 *	\sa
 *	RKH_SBSC_T structure definition for more information.
 *
 *  \ingroup apiSM
 */
#define RKH_CREATE_BASIC_STATE(name, en, ex, parent, prepro) \
                                                             \
    extern RKHROM RKH_TR_T name##_trtbl[]; \
                                             \
    RKHROM RKH_SBSC_T name = \
    { \
        {MKBASE(RKH_BASIC, name), MKST(en,ex,parent)}, \
        MKBASIC(name,prepro)  \
    }

/**
 *  \brief
 *	This macro creates a conditional pseudostate (a.k.a. junction).
 *
 *  The conditional pseudostates are semantic-free vertices that are used to
 *  chain together multiple transitions. They are used to construct compound
 *  transition paths between states. For example, a junction can be used to
 *  converge multiple incoming transitions into a single outgoing transition
 *  representing a shared transition path (this is known as a merge).
 *  Conversely, they can be used to split an incoming transition into multiple
 *  outgoing transition segments with different guard conditions. This
 *  realizes a static conditional branch. (In the latter case, outgoing
 *  transitions whose guard conditions evaluate to false are disabled. A
 *  predefined guard denoted "ELSE" may be defined for at most one outgoing
 *  transition. This transition is enabled if all the guards labeling the
 *  other transitions are false.) Static conditional branches are distinct
 *  from dynamic conditional branches that are realized by choice vertices.
 *
 *  \param[in] name     pseudostate name. Represents a conditional pseudostate
 *                      structure.
 *
 *	\sa
 *	RKH_SCOND_T structure definition for more information.
 *
 *  \ingroup apiSM
 */
#define RKH_CREATE_COND_STATE(name) \
                                    \
    extern RKHROM RKH_TR_T name##_trtbl[]; \
                                             \
    RKHROM RKH_SCOND_T name = \
    { \
        MKBASE(RKH_CONDITIONAL, name), \
        name##_trtbl \
    }

/**
 *  \brief
 *	This macro creates a choice pseudostate.
 *
 *  Choice pseudostate which, when reached, result in the dynamic evaluation
 *  of the guards of its outgoing transitions. This realizes a dynamic
 *  conditional branch. It allows splitting of transitions into multiple
 *  outgoing paths such that the decision on which path to take may be a
 *  function of the results of prior actions performed in the same
 *  run-to-completion step. If more than one of the guards evaluates to true,
 *  an arbitrary one is selected. If none of the guards evaluates to true,
 *  then the model is considered ill-formed. To avoid this, it is recommended
 *  to define one outgoing transition with the predefined "ELSE" guard for
 *  every choice pseudostate.
 *	Also, branches cannot contain triggers, but in addition to a guard they
 *	may contain actions. A branch can enter another choice connector, thus
 *	providing for the nesting of branches.
 *
 *  \param[in] name		pseudostate name. Represents a choice pseudostate
 *                      structure.
 *
 *	\sa
 *	RKH_SCHOICE_T structure definition for more information.
 *
 *  \ingroup apiSM
 */
#define RKH_CREATE_CHOICE_STATE(name) \
                                      \
    extern RKHROM RKH_TR_T name##_trtbl[]; \
                                           \
    RKHROM RKH_SCHOICE_T name = \
    { \
        MKBASE(RKH_CHOICE, name), \
        name##_trtbl \
    }

/**
 * 	\deprecated 
 * 	This macro creates a deep history pseudostate. Instead use: 
 *	RKH_CREATE_COMP_REGION_STATE() macro.
 *
 *	Deep history applies downwards to all levels of nesting.
 *
 *	\note
 *	At this framework version, it is not recommended to instantiate the 
 *	same history object more than once using this macro, since it uses a 
 *	static memory to store the last visited state (memory of history 
 *	pseudostate).
 *
 *	\sa
 *	RKH_SHIST_T structure definition for more information.
 *
 * 	\param[in] name		pseudostate name. Represents a deep history 
 *                      pseudostate structure.
 * 	\param[in] parent	pointer to parent state.
 *  \param[in] dftTrnGuard	
 *                      pointer to guard function. This argument is
 *			   	        optional, thus it could be declared as NULL.
 *  \param[in] dftTrnAction
 *                      pointer to action function. This argument is
 *			   	        optional, thus it could be declared as NULL.
 *  \param[in] dftTarget
 *                      pointer to target state. If a default history 
 *                      Transition is defined (the target parameter is not 
 *                      NULL) originating from the History Pseudostate, it 
 *                      will be taken. Otherwise, default State entry is 
 *                      applied. 
 *
 *  \ingroup apiSM
 */
#define RKH_CREATE_DEEP_HISTORY_STATE(name, parent, dftTrnGuard, \
                                      dftTrnAction, dftTarget) \
                                                               \
    static RKHROM RKH_ST_T *ram##name; \
    MKHISTORY(name, parent, RKH_DHISTORY, dftTrnGuard, dftTrnAction, \
              dftTarget, &ram##name)

/**
 * 	\deprecated 
 *	This macro creates a shallow history pseudostate. Instead use: 
 *	RKH_CREATE_COMP_REGION_STATE() macro.
 *
 *	Shallow history means that history applies to the current nesting context 
 *	only states nested more deeply are not affected by the presence of a 
 *	history pseudostates in a higher context.
 *	
 *	\note
 *	At this framework version, it is not recommended to instantiate the 
 *	same history object more than once using this macro, since it uses a 
 *	static memory to store the last visited state (memory of history 
 *	pseudostate).
 *
 *	\sa
 *	RKH_SHIST_T structure definition for more information.
 *
 * 	\param[in] name     pseudostate name. Represents a shallow history 
 *                      pseudostate structure.
 * 	\param[in] parent   pointer to parent state.
 *  \param[in] dftTrnGuard	
 *                      pointer to guard function. This argument is
 *					    optional, thus it could be declared as NULL.
 *  \param[in] dftTrnAction
 *                      pointer to action function. This argument is
 *					    optional, thus it could be declared as NULL.
 *  \param[in] dftTarget
 *                      pointer to target state. If a default history 
 *                      Transition is defined (the target parameter is not 
 *                      NULL) originating from the History Pseudostate, it 
 *                      will be taken. Otherwise, default State entry is 
 *                      applied. 
 *
 *  \ingroup apiSM
 */
#define RKH_CREATE_SHALLOW_HISTORY_STATE(name, parent, dftTrnGuard, \
                                         dftTrnAction, dftTarget) \
                                                                  \
    static RKHROM RKH_ST_T *ram##name; \
    MKHISTORY(name, parent, RKH_SHISTORY, dftTrnGuard, dftTrnAction, \
              dftTarget, &ram##name)

/**
 *  \brief
 *	This macro creates a submachine state.
 *
 *  A submachine state is a kind of a state that actually refers to
 *  another defined state machine.
 *  A submachine state is logically equivalent to the insertion of the
 *  referenced state machine as a composite state in the place of
 *  the submachine state. Consequently, every entrance to a submachine
 *  state is equivalent to the corresponding entrance to the inserted
 *  (referenced) composite state. In particular, it can be entered
 *  thruough its initial pseudostate (as any other composite state), or
 *  through one of its entry points.
 *
 *  Similary, every exit from a submachine state is equivalent to the
 *  corresponding exit from the inserted composite state. It can be exited
 *  through one of its exit points. When it is exited through its exit point
 *  the effect of the transition targeting the exit point is executed first,
 *  followed by the exit behavior of the composite state.
 *
 *  The purpose od defining submachine states is to decompose and localize
 *  repetitive parts because the same state machine can be referenced from
 *  more than one submachine state.
 *
 *  \param[in] name     submachine state name. Represents a submachine state
 *                      structure.
 *  \param[in] en		pointer to state entry action. This argument is
 *                      optional, thus it could be declared as NULL.
 *                      The RKH implementation preserves the transition sequence
 *                      imposed by Harel's Statechart and UML.
 *  \param[in] ex       pointer to state exit action. This argument is
 *                      optional, thus it could be declared as NULL.
 *                      The RKH implementation preserves the transition sequence
 *                      imposed by Harel's Statechart and UML.
 *  \param[in] parent	pointer to parent state.
 *  \param[in] sbm		pointer to referenced submachine state machine.
 *
 *	\sa
 *	RKH_SSBM_T structure definition for more information.
 *
 *	\code
 *	RKH_CREATE_SUBMACHINE_STATE(    adquire,		// state name
 *									start_adquire,  // entry action
 *									stop_adquire,   // exit action
 *									&processing,    // parent state
 *									&herror );		// referenced submachine
 *	\endcode
 *
 *  \ingroup apiSM
 */
#define RKH_CREATE_SUBMACHINE_STATE(name, en, ex, parent, sbm) \
                                                               \
    extern RKHROM RKH_EXPCN_T name##_exptbl[]; \
    extern RKHROM RKH_TR_T name##_trtbl[]; \
                                             \
    RKHROM RKH_SSBM_T name = \
    { \
        {MKBASE(RKH_SUBMACHINE, name), MKST(en,ex,parent)}, \
        MKSBM(name,sbm) \
    }

/**
 *  \brief
 *	This macro creates a exit point connection point reference table.
 *
 *  \param[in] name		submachine state name
 *
 *	\note
 *	This macro is not terminated with the semicolon.
 *
 *	\usage
 *	This table have the general structure shown below:
 *	\code
 *	RKH_CREATE_EX_CNNPNT_TABLE( S2 )
 *		RKH_EX_CNNPNT( EX1S2, &EXPNT1, NULL, &S1 ),
 *		RKH_EX_CNNPNT( EX2S2, &EXPNT2, NULL, &S3 ),
 *	RKH_END_EX_CNNPNT_TABLE
 *	\endcode
 *  Each exit point connection reference table always begins with the macro
 *  RKH_CREATE_EX_CNNPNT_TABLE() and ends with the macro
 *  RKH_END_EX_CNNPNT_TABLE().
 *	As noted above, sandwiched between these macros are the exit point
 *	macros, RKH_EX_CNNPNT().
 *
 *  \ingroup apiSM
 */
#define RKH_CREATE_EX_CNNPNT_TABLE(name) \
    RKHROM RKH_EXPCN_T name##_exptbl[]= \
    {

/**
 *  \brief
 *	This macro creates an exit point connection point reference.
 *
 *	Connection point references are sources/targets of transitions implying
 *	exits out of/entries into the submachine state machine referenced by a
 *	submachine state.
 *
 *	An exit point connection point reference as the source of a transition
 *	implies that the source of the transition is the exit point pseudostate
 *	as defined in the submachine of the submachine state that has the exit
 *	point connection point defined.
 *
 *	A connection point reference to an exit point has the same notation as
 *	an exit point pseudostate.
 *
 *  \param[in] name     exit point connection point reference name.
 *  \param[in] expnt	referenced exit point.
 *  \param[in] act		pointer to transition action function. This argument is
 *					    optional, thus it could be declared as NULL.
 *  \param[in] ts		pointer to target state.
 *
 *	\sa
 *	RKH_EXPCN_T structure definition for more information.
 *
 *  \usage
 *  \code
 *	// --- exit point pseudostates of SB submachine ---
 *	RKH_CREATE_REF_EXPNT(   EXPNT1,
 *							0,		// index of exit point connection table
 *							&SB );
 *	RKH_CREATE_REF_EXPNT(   EXPNT2,
 *							1,      // index of exit point connection table
 *							&SB );
 *							...
 *	// --- exit point connection references of S12 submachine state ---
 *	RKH_CREATE_EX_CNNPNT_TABLE( S12 )
 *		RKH_EX_CNNPNT( EX1S12, &EXPNT1, ... ), // table index = 0 (EXPNT1)
 *		RKH_EX_CNNPNT( EX2S12, &EXPNT2, ... ), // table index = 1 (EXPNT2)
 *	RKH_END_EX_CNNPNT_TABLE
 *	\endcode
 */
#define RKH_EX_CNNPNT(name, expnt, act, ts) \
    {(RKH_TRN_ACT_T)act, (RKHROM struct RKH_ST_T *)ts}

/**
 *  \brief
 *	This macro is used to terminate a exit point connection reference table.
 *
 *	\note
 *	This macro is not terminated with the semicolon.
 *
 *	\usage
 *	This table have the general structure shown below:
 *	\code
 *	RKH_CREATE_EX_CNNPNT_TABLE( S2 )
 *		RKH_EX_CNNPNT( EX1S2, &EXPNT1, NULL, &S1 ),
 *		RKH_EX_CNNPNT( EX2S2, &EXPNT2, NULL, &S3 ),
 *	RKH_END_EX_CNNPNT_TABLE
 *	\endcode
 *  Each exit point table always begins with the macro
 *  RKH_CREATE_EX_CNNPNT_TABLE() and ends with the macro
 *  RKH_END_EX_CNNPNT_TABLE().
 *	As noted above, sandwiched between these macros are the exit point
 *	macros, RKH_EX_CNNPNT().
 */
#define RKH_END_EX_CNNPNT_TABLE     };

/**
 *  \brief
 *	This macro creates an entry point connection point reference.
 *
 *	Connection point references are sources/targets of transitions implying
 *	exits out of/entries into the submachine state machine referenced by a
 *	submachine state.
 *
 *	An entry point connection point reference as the target of a transition
 *	implies that the target of the transition is the entry point pseudostate
 *	as defined in the submachine of the submachine state.
 *
 *	A connection point reference to an entry point has the same notation as
 *	an entry point pseudostate.
 *
 *  \param[in] name		entry point connection point reference name.
 *  \param[in] enpnt	referenced entry point.
 *  \param[in] subm		pointer to submachine state.
 *
 *	\sa
 *	RKH_SENP_T structure definition for more information.
 */
#define RKH_EN_CNNPNT(name, enpnt, subm) \
                                         \
    RKHROM RKH_SENP_T name = \
    { \
        MKBASE(RKH_ENPOINT, name), \
        MKENP(enpnt,subm) \
    }

/**
 *  \brief
 *	This macro creates a submachine state machine, which is to be
 *	inserted in place of the (submachine) state.
 *
 *  \param[in] name		submachine name. Represents a submachine structure.
 *  \param[in] defchild	pointer to default child state.
 *  \param[in] iact		pointer to initialization action (optional). The
 *                      function prototype is defined as RKH_TRN_ACT_T. This
 *                      argument is optional, thus it could be declared as
 *                      NULL.
 *
 *	\sa
 *	RKH_RSM_T structure definition for more information.
 *
 *  \code
 *  RKH_CREATE_REF_SUBMACHINE(  adquire,
 *                              &wait,
 *                              init_adquire );
 *  \endcode
 *
 *  \ingroup apiSM
 */
#define RKH_CREATE_REF_SUBMACHINE(name, defchild, iact) \
                                                        \
    static RKHROM RKH_ST_T * rdyp_##name; \
                                            \
    RKHROM RKH_RSM_T name = \
    { \
        MKBASE(RKH_REF_SUBMACHINE, name), \
        MKMCH(defchild,iact,name) \
    }

/**
 *  \brief
 *	This macro creates an exit point.
 *
 *  An exit pseudostate is used to join an internal transition terminating on
 *  that exit point to an external transition emanating from that exit point.
 *  The main purpose of such entry and exit points is to execute the state
 *  entry and exit actions respectively in between the actions that are
 *  associated with the joined transitions.
 *
 *  \param[in] name		entry point name.
 *  \param[in] ix		index of exit point connection table. Note that each 
 *                      row number matches with the index number of the exit 
 *                      point pseudostate that it represent.
 *  \param[in] subm		pointer to submachine state machine.
 *
 *  \code
 *  RKH_CREATE_REF_EXPNT(handled,
 *                       0,
 *                       &handle_error);
 *  \endcode
 *
 *	\sa
 *	RKH_SEXP_T structure definition for more information.
 *
 *  \usage
 *	\code
 *	// --- exit point pseudostates of SB submachine ---
 *	RKH_CREATE_REF_EXPNT(   EXPNT1,
 *							0,		// index of exit point connection table
 *							&SB );
 *	RKH_CREATE_REF_EXPNT(   EXPNT2,
 *							1,      // index of exit point connection table
 *							&SB );
 *							...
 *	// --- exit point connection references of S12 submachine state ---
 *	RKH_CREATE_EX_CNNPNT_TABLE( S12 )
 *		RKH_EX_CNNPNT( EX1S12, &EXPNT1, ... ), // table index = 0 (EXPNT1)
 *		RKH_EX_CNNPNT( EX2S12, &EXPNT2, ... ), // table index = 1 (EXPNT2)
 *	RKH_END_EX_CNNPNT_TABLE
 *	\endcode
 *
 *  \ingroup apiSM
 */
#define RKH_CREATE_REF_EXPNT(name, ix, subm) \
                                             \
    RKHROM RKH_SEXP_T name = \
    { \
        MKBASE(RKH_EXPOINT, name), \
        ix, subm \
    }

/**
 *  \brief
 *	This macro creates a entry point.
 *
 *  An entry pseudostate is used to join an external transition terminating
 *  on that entry point to an internal transition emanating from that entry
 *  point.
 *  The main purpose of such entry and exit points is to execute the state
 *  entry and exit actions respectively in between the actions that are
 *  associated with the joined transitions.
 *
 *  \param[in] name     entry point name.
 *  \param[in] act		pointer to transition action function. This argument is
 *					    optional, thus it could be declared as NULL.
 *  \param[in] ts		pointer to target state.
 *  \param[in] subm		pointer to submachine state machine.
 *
 *	\sa
 *	RKH_ENPCN_T structure definition for more information.
 *
 *  \code
 *  RKH_CREATE_REF_ENPNT(   show,
 *                          2,
 *                          &S2,
 *                          &handle_error );
 *  \endcode
 *
 *  \ingroup apiSM
 */
#define RKH_CREATE_REF_ENPNT(name, act, ts, subm) \
                                                  \
    RKHROM RKH_ENPCN_T name = \
    { \
        (RKH_TRN_ACT_T)act, (RKHROM struct RKH_ST_T *)ts \
    }

/**
 *  \brief
 *	This macro creates a state transition table. 
 *  
 *  \param[in] name		state name.
 *
 *	\note
 *	This macro is not terminated with the semicolon.
 *  
 *  \usage
 *  This table have the general structure shown below:
 *	\code
 *	RKH_CREATE_TRANS_TABLE( state_name )		// transition table begin
 *		RKH_TRxx( ... )							// transition
 *		RKH_TRxx( ... )							// transition
 *		...
 *	RKH_END_TRANS_TABLE							// transition table end
 *	\endcode
 *  Each transition table always begins with the macro RKH_CREATE_TRANS_TABLE()
 *  and ends with the macro RKH_END_TRANS_TABLE().
 *	As noted above, sandwiched between these macros are the transitions macros
 *	that actually represent behavior of state.
 *
 *  \ingroup apiSM
 */
#define RKH_CREATE_TRANS_TABLE(name) \
                                     \
    RKHROM RKH_TR_T name##_trtbl[]= \
    {

/**
 *  \brief
 *	This macro defines a regular state transition.
 *
 *	The general syntax of an expression labelling a transition in a
 *	statechart is \e "i[c]/a" where \e i is the input that triggers the
 *	transition, \e c is a condition that guards the transition from being
 *	taken unless it is true when \e i occurs, and \e a is an action that
 *	is carried out if and when the transition is taken. All of these parts
 *	are optional.
 *
 *  \param[in] evt_	    triggering event.
 *  \param[in] guard_	pointer to guard function. This argument is
 *					    optional, thus it could be declared as NULL.
 *  \param[in] effect_	pointer to action function. This argument is
 *					    optional, thus it could be declared as NULL.
 *  \param[in] target_	pointer to target state.
 *
 *	\sa
 *	RKH_TR_T structure definition for more information.
 *
 *  \usage
 *	\code
 *	RKH_TRREG(  TOUT0,              // triggering event
 *				is_full,            // guard function
 *				drop_frame,         // action function
 *				&WAIT_SYNC )		// next state
 *	\endcode
 */
#define RKH_TRREG(evt_, guard_, effect_, target_) \
            MKTRN(evt_, guard_, effect_, target_)

/**
 *  \brief
 *	This macro defines an internal state transition.
 *	Internal transitions are simple reactions to events that never lead
 *	to change of state and consequently never cause execution of exit
 *	actions, entry actions, or initial transitions.
 *
 *  \param[in] e    triggering event.
 *  \param[in] g	pointer to guard function.
 *  \param[in] a	pointer to action function.
 *
 *	\sa
 *	RKH_TR_T structure definition for more information.
 *
 *  \usage
 *	\code
 *	RKH_TRINT(  RCV_DATA,           // triggering event
 *				is_sync,            // guard function
 *				store_data )        // action function
 *	\endcode
 */
#define RKH_TRINT(e, g, a)    {e, (RKH_GUARD_T)g, (RKH_TRN_ACT_T)a, NULL}

/**
 *  \brief
 *	This macro defines a completion transition.
 *
 *  A special kind of transition is a completion transition, which has an 
 *  implicit trigger. The event that enables this trigger is called a 
 *  completion event and it signifies that all behaviors associated with the 
 *  source state of the completion transition have completed execution. 
 *  In case of simple states, a completion event is generated when the 
 *  associated entry behavior have completed executing. If no such behaviors 
 *  are defined, the completion event is generated upon entry into the state. 
 *  For composite state, a completion event is generated under the following 
 *  circumstances:
 *  - All internal activities (e.g., entry behavior) have completed execution, 
 *  and
 *  - its region have reached a finalState.
 *
 *  \param[in] guard_	pointer to guard function. This argument is
 *					    optional, thus it could be declared as NULL.
 *  \param[in] effect_	pointer to effect function. This argument is
 *					    optional, thus it could be declared as NULL.
 *  \param[in] target_	pointer to target vertex (state or pseudostate).
 *
 *	\sa
 *	RKH_TR_T structure definition for more information.
 *
 *  \usage
 *	\code
 *	RKH_TRCOMPLETION(isFull,        // guard function
 *				     dropFrame,     // effect function
 *				     &waitSync),    // next state
 *	\endcode
 */
#define RKH_TRCOMPLETION(guard_, effect_, target_) \
    {RKH_COMPLETION_EVENT, \
     (RKH_GUARD_T)guard_, (RKH_TRN_ACT_T)effect_, target_}

/**
 *  \brief
 *	This macro is used to terminate a state transition table.
 *	This table have the general structure shown below:
 *	\code
 *	RKH_CREATE_TRANS_TABLE( state_name )		// transition table begin
 *		RKH_TRxx( ... )							// transition
 *		RKH_TRxx( ... )							// transition
 *		...
 *	RKH_END_TRANS_TABLE							// transition table end
 *	\endcode
 *
 *  Each transition table always begins with the macro RKH_CREATE_TRANS_TABLE()
 *  and ends with the macro RKH_END_TRANS_TABLE().
 *	As noted above, sandwiched between these macros are the transitions macros
 *	that actually represent behavior of state.
 *
 *	\note
 *	This macro is not terminated with the semicolon.
 */
#define RKH_END_TRANS_TABLE     {RKH_ANY, NULL, NULL, NULL}};

/**
 *	This macro creates a branch table. 
 *
 *  \param[in] name		conditional pseudostate name.
 *
 *	\sa
 *	This macro is not terminated with the semicolon.
 *	Use rkh_sm_else() when if all the guards on the other branches are false.
 *
 *  \usage
 *	This table have the general structure shown below:
 *	\code
 *	RKH_CREATE_BRANCH_TABLE( C2 )
 *		RKH_BRANCH( is_power_ok,    enable_process,	&power		),
 *		RKH_BRANCH( is_switch_off,  turnoff,		&wait		),
 *		RKH_BRANCH( ELSE,           abort,			&aborted	),
 *	RKH_END_BRANCH_TABLE
 *	\endcode
 *  Each branch table always begins with the macro RKH_CREATE_BRANCH_TABLE()
 *  and ends with the macro RKH_END_BRANCH_TABLE().
 *  In RKH branches are defined by the macro RKH_BRANCH().
 *
 *  \ingroup apiSM
 */
#define RKH_CREATE_BRANCH_TABLE(name) \
                                      \
    RKH_CREATE_TRANS_TABLE(name)

/**
 *  \brief
 *	This macro defines a branch in the branch table.
 *
 *	Each condition connector can have one special branch with a guard
 *	labeled rkh_sm_else, which is taken if all the guards on the other
 *	branches are false.
 *	The general syntax of an expression labelling a branch in a statechart is
 *	\e "[c]/a" where \e c is a condition that guards the transition from
 *	being taken unless it is true, and \e a is an action that is carried out
 *	if and when the transition is taken. All of these parts are optional.
 *
 *  \param[in] guard_	branch guard function. Branches are labeled with guards
 *                      that determine which one is to be actually taken. Use
 *                      ELSE macro when if all the guards on the other branches
 *                      are false.
 *  \param[in] effect_	pointer to transition action. This argument is 
 *                      optional, thus it could be declared as NULL.
 *  \param[in] target_	pointer to target state.
 *
 *	\sa
 *	RKH_TR_T structure definition for more information.
 *
 *  \usage
 *	\code
 *	RKH_CREATE_BRANCH_TABLE( C2 )
 *		RKH_BRANCH( is_power_ok,    enable_process,	&power		),
 *		RKH_BRANCH( is_switch_off,  turnoff,		&wait		),
 *		RKH_BRANCH( ELSE,           abort,			&aborted	),
 *	RKH_END_BRANCH_TABLE
 *	\endcode
 */
#define RKH_BRANCH(guard_, effect_, target_) \
            MKTRN(0, guard_, effect_, target_)

/**
 *  \brief
 *  Define a RAM storage to mantain the last visited state of a composite 
 *  state (history pseudostate).
 *
 *  Frequently, it's used before of RKH_CREATE_COMP_REGION_STATE() macro to 
 *  define the dynamic storage of a history pseudostate.
 *
 *  \param[in] compStateName    pointer to the composite state that contains 
 *                              this history pseudostate.
 *
 *  \ingroup apiSM
 */
#define RKH_CREATE_HISTORY_STORAGE(compStateName) \
    static RKHROM RKH_ST_T *ramHist_##compStateName

/**
 *  \brief
 *  Return the history storage address of a composite state. 
 *
 *  Frequently, it's used in the hRamMem parameter of 
 *  RKH_CREATE_COMP_REGION_STATE() macro to define the dynamic storage of a 
 *  history pseudostate.
 *
 *  \param[in] compStateName    pointer to the composite state that contains 
 *                              this history pseudostate.
 */
#define RKH_GET_HISTORY_STORAGE(compStateName) \
    &ramHist_##compStateName

/*
 *  This macro is internal to RKH and the user application should
 *  not call it.
 */
#define RKH_EBTBL               {RKH_ANY, NULL, NULL, NULL}

/**
 *  \brief
 *	This macro is used to terminate a state transition table.
 *	This table have the general structure shown below:
 *	\code
 *	RKH_CREATE_BRANCH_TABLE( C2 )
 *		RKH_BRANCH( is_power_ok,    enable_process,	&power		),
 *		RKH_BRANCH( is_switch_off,  turnoff,		&wait		),
 *		RKH_BRANCH( ELSE,           abort,			&aborted	),
 *	RKH_END_BRANCH_TABLE
 *	\endcode
 *
 *  Each branch table always begins with the macro RKH_CREATE_BRANCH_TABLE()
 *  and ends with the macro RKH_END_BRANCH_TABLE().
 *
 *	\sa
 *	This macro is not terminated with the semicolon.
 */
#define RKH_END_BRANCH_TABLE    RKH_EBTBL};

/**
 *  \brief
 *	This macro indicates the root state of a state machine.
 */
#define RKH_ROOT                RKH_NULL

/**
 *  \brief
 *  This macro declares an opaque pointer to previously created state machine
 *  to be used as a global object.
 *
 *  This global pointer represent the state machine in the application.
 *  The state machine pointers are "opaque" because they cannot access the
 *  whole state machine structure, but only the part inherited from the
 *  RKH_SM_T structure. The power of an "opaque" pointer is that it allows
 *  to completely hide the definition of the state machine structure and make
 *  it inaccessible to the rest of the application.
 *
 *  \param[in] me_	Pointer to previously created state machine.
 *
 *	\note
 *  Generally, this macro is used in the SMA's header file.
 *
 *  \sa
 *  RKH_SMA_CREATE(), RKH_SM_CREATE().
 *
 *  \usage
 *  \code
 *  // ColorRegion.h: state machine's header file
 *
 *  RKH_SM_DCLR_TYPE(ColorRegion, color);
 *  \endcode
 */
#define RKH_SM_DCLR(me_) \
            RKH_DCLR_PTR_TYPE(RKH_SM_T, me_)

/**
 *  \brief
 *  This macro declares a typed pointer to previously created state machine
 *  to be used as a global and public object.
 *
 *  \param[in] type_
 *                  Data type of the state machine, derived from RKH_SM_T. 
 *  \param[in] me_	Pointer to previously created state machine.
 *
 *	\note
 *  Generally, this macro is used in the SMA's header file.
 *
 *  \sa
 *  RKH_SMA_CREATE(), RKH_SM_CREATE().
 *
 *  \usage
 *  \code
 *  // ColorRegion.h: state machine's header file
 *
 *  RKH_SM_DCLR_TYPE(ColorRegion, color);
 *  \endcode
 */
#define RKH_SM_DCLR_TYPE(type_, me_) \
            RKH_DCLR_PTR_TYPE(type_, me_)

/**@{
 *
 *  \brief
 *  Declares a previously created state/pseudostate to be used
 *  as a global object.
 *
 *  \note
 *  Generally, this macro is used in the state-machine's header file.
 */
#define RKH_DCLR_COMP_STATE     extern RKHROM RKH_SCMP_T
#define RKH_DCLR_BASIC_STATE    extern RKHROM RKH_SBSC_T
#define RKH_DCLR_FINAL_STATE    extern RKHROM RKH_FINAL_T
#define RKH_DCLR_COND_STATE     extern RKHROM RKH_SCOND_T
#define RKH_DCLR_CHOICE_STATE   extern RKHROM RKH_SCHOICE_T
#define RKH_DCLR_DHIST_STATE    extern RKHROM RKH_SHIST_T
#define RKH_DCLR_SHIST_STATE    extern RKHROM RKH_SHIST_T
#define RKH_DCLR_SUBM_STATE     extern RKHROM RKH_SSBM_T
#define RKH_DCLR_REF_SUBM       extern RKHROM RKH_RSM_T
#define RKH_DCLR_ENPNT          extern RKHROM RKH_SENP_T
#define RKH_DCLR_REF_EXPNT      extern RKHROM RKH_SEXP_T
#define RKH_DCLR_REF_ENPNT      extern RKHROM RKH_ENPCN_T
/**@}*/

/**
 *  \brief
 *  Declares a state transition table.
 *
 *  \param[in] name			name of state (basic or composite) object.
 */
#define RKH_DECLARE_TR_TBL(name) \
    extern RKHROM RKH_TR_T name##_trtbl[]

/**
 *  \brief
 *  Declares a previously created SM to be used as a global object.
 *
 *  \param[in] sm_t		    data type of SM.
 *  \param[in] sm			name of previously created SM.
 *  \param[in] gob			name of global object.
 *
 *  \note
 *  Generally, this macro is used in the state-machine's module.
 *
 *	\sa
 *	RKH_SM_T structure definition for more information. Also,
 *	\link RKH_EVT_T single inheritance in C \endlink, and
 *	\link RKH_CREATE_BASIC_STATE another example \endlink.
 *
 *  \usage
 *	\code
 *	// syncRegion: global pointer to sync state machine of 
 *  // type Sync.
 *	RKH_DCLR_SM_GLOBAL(Sync, sync, syncRegion);
 *	\endcode
 */
#define RKH_DCLR_SM_GLOBAL(sm_t, sm, gob) \
    sm_t * const gob = &s_##sm;

/**
 *  \brief
 *  Declares a previously created constant part (in ROM) of a state machine 
 *  to be used as a global object.
 *
 *  \param[in] smName			name of previously created SM.
 *
 *  \note
 *  Generally, this macro is used in the state-machine's module.
 *
 *	\sa
 *	RKH_SM_T structure definition for more information. Also,
 *	\link RKH_EVT_T single inheritance in C \endlink, and
 *	\link RKH_CREATE_BASIC_STATE another example \endlink.
 */
#define RKH_DCLR_SM_CONST_GLOBAL(smName) \
    extern RKHROM RKH_ROM_T RKH_SM_CONST_NAME(smName)

/**
 *  \brief
 *	Initializes the parameters (attibutes) of a basic state object.
 *
 *	By means of single inheritance in C it could be used to associate data
 *	to a basic state. Moreover, implementing the single inheritance in C
 *	is very simply by literally embedding the base type, RKH_SBSC_T in this
 *	case, as the first member of the derived structure. The following
 *	listing shows an example:
 *
 *	\sa
 *	See #RKH_CREATE_BASIC_STATE() macro for more information.
 *
 *  \usage
 *	\code
 *	(1) typedef struct MENU_ST_T
 *		{
 *	(2)		RKHROM RKH_SBSC_T base;
 *	(3)		const char *title;
 *		} MENU_ST_T;
 *
 *	...
 *
 *	(4) #define MENU_BASIC_STATE( type_t, name, en, ex, parent, title ) \
 *	(5)				RKH_DECLARE_TR_TBL( name ); \
 *					\
 *	(6)				RKHROM type_t name = \
 *					{ \
 *	(7)					RKH_INIT_BASIC_STATE( name, en, ex, parent, NULL ), \
 *	(8)					title \
 *					}
 *
 *	(9) MENU_BASIC_STATE( MENU_ST_T, menu_cfg,  NULL, NULL, RKH_ROOT,
 *												"Menu 1" );
 *		RKH_CREATE_TRANS_TABLE( menu_cfg )
 *			RKH_TRREG( TIMEOUT, NULL, mc_title, &menu_cfg_resume ),
 *			...
 *		RKH_END_TRANS_TABLE
 *
 *  ...
 *
 *
 *	(10) void
 *		 mc_title( const struct RKH_SMA_T *me )
 *		 {
 *		    ...
 *			lcd_print("%s\n", 
 *			          ((MENU_ST_T *)(((RKH_SM_T *)me)->state))->title);
 *		 }
 *	\endcode
 *
 *	(1)  Defines MENU_ST_T as a derived structure from the basic state
 *		 structure. It could be used to associate data to a basic state
 *		 objects. \n
 *	(2)	 Implementing the single inheritance in C is very simply by literally
 *		 embedding the base type, RKH_SBSC_T in this case, as the first
 *		 member of the derived structure.  \n
 *	(3)  Private members of MENU_ST_T derived structure. \n
 *	(4)  Defines a simple macro to instantiate the derived state object.
 *		 It is not strictly necessary.  \n
 *	(4)	 Declares the transtion table by means of RKH_DECLARE_TR_TBL() macro
 *		 provided by the framework. \n
 *	(6)  Instantiates the derived state structure. \n
 *	(7)  Initializes the base structure, basic state in this case, using the
 *		 RKH_INIT_BASIC_STATE() macro provided by the framework. \n
 *	(8)  Initializes the private member of derived state. \n
 *	(9)  Invokes the BLINKY_BASIC_STATE() macro to instantiate the
 *		 derived state object, menu_cfg. Note that the "Menu 1" string is
 *		 associated to the state machine context, menu_cfg state in this
 *		 case. \n
 *	(10) Lastly, on the transtion action mc_title(), or any other action, is
 *		 used the state data after casting the active object state to
 *		 derived state object.
 *
 *  The following listing shows another example for derived state structure,
 *  which access to RAM from ROM object (state structure).
 *
 *	\code
 *	    typedef struct XY_T
 *		{
 *			int x;
 *			int y;
 *		} XY_T;
 *
 *	(1) typedef struct NUM_STATE_T
 *		{
 *	(2)		RKHROM RKH_SBSC_T base;
 *	(3)     XY_T *p_ram_xy;
 *		} NUM_STATE_T;
 *
 *	(4) #define NUM_BASIC_STATE( name, en, ex, parent, p_xy ) \
 *	                RKH_DECLARE_TR_TBL( name ); \
 *					\
 *	                RKHROM NUM_STATE_T name = \
 *					{ \
 *	                    RKH_INIT_BASIC_STATE( name, en, ex, parent, NULL ), \
 *	(5)					p_xy \
 *					}
 *
 *		static XY_T xy;
 *	(6) NUM_BASIC_STATE( num, NULL, NULL, RKH_ROOT, &xy );
 *
 *	    void
 *		num_inc( const struct RKH_SMA_T *me )
 *		{
 *	(7)		((NUM_STATE_T *)(((RKH_SM_T *)me)->state))->p_ram_xy++;
 *			...
 *		}
 *	\endcode
 *
 *	(1)  Defines NUM_STATE_T as a derived structure from the basic state
 *		 structure. \n
 *	(2)	 Implementing the single inheritance in C is very simply by literally
 *		 embedding the base type, RKH_SBSC_T in this case, as the first
 *		 member of the derived structure. \n
 *	(3)  Private members of NUM_STATE_T derived structure. It allows to
 *		 acccess to RAM locations from this object (NUM_STATE_ST) allocated
 *		 in ROM. \n
 *	(4)  Defines a simple macro to instantiate the derived state object. \n
 *	(5)  Initializes the private member of derived state. \n
 *	(6,7)Invokes the NUM_BASIC_STATE() macro to instantiate the derived
 *		 state object, num. Note that the variable xy is allocated in RAM
 *		 but it is access to read and write from ROM object. \n
 */
#define RKH_INIT_BASIC_STATE(name, en, ex, parent, prepro) \
    { \
        { \
            MKBASE(RKH_BASIC, name),    /* RKH_BASE_T */ \
            MKST(en, ex, parent) \
        },                              /* RKH_ST_T */ \
        MKBASIC(name, prepro) \
    } /* RKH_SBSC_T */ \


/**
 *  \brief
 *	Initializes the parameters (attibutes) of a composite state object.
 *
 *	By means of single inheritance in C it could be used to associate data
 *	to a basic state. Moreover, implementing the single inheritance in C
 *	is very simply by literally embedding the base type, RKH_SCMP_T in this
 *	case, as the first member of the derived structure.
 *
 *	\note
 *	See RKH_INIT_BASIC_STATE() and RKH_CREATE_COMP_STATE() macros to more
 *	information.
 */
#define RKH_INIT_COMPOSITE_STATE(name, en, ex, parent, \
                                 defchild, history) \
    { \
        { \
            MKBASE(RKH_COMPOSITE, name),    /* RKH_BASE_T */ \
            MKST(en, ex, parent) \
        },                                  /* RKH_ST_T */ \
        MKCOMP(name, defchild, history) \
    }                                       /* RKH_SCMP_T */ \

/**
 *  \brief
 *  This values are retrived by guard functions.
 *
 *  A guard is a boolean condition that returns a true or false value that
 *  controls whether or not a transition is taken following the receipt of
 *  a triggering event.
 */
#define RKH_GFALSE      RKH_FALSE   /**< False condition */
#define RKH_GTRUE       RKH_TRUE    /**< True condition */

/**
 *  \brief
 *  Use it to allocate a state machine regardless of an active object.
 *
 *  \param[in] type 	Data type of the SMA. Could be derived from RKH_SMA_T.
 *  \param[in] name		Name of state machine application. Also, it represents 
 *                      the top state of state diagram.
 *  \param[in] prio		State machine application priority. A unique priority
 *                      number must be assigned to each SMA from 0 to
 *                      RKH_LOWEST_PRIO. The lower the number, the higher the
 *                      priority.
 *  \param[in] ppty		State machine properties. The available properties are
 *                      enumerated in RKH_HPPTY_T enumeration in the rkh.h
 *                      file.
 *  \param[in] initialState
 *                      Pointer to initial state. This state could be defined
 *                      either composite or basic (not pseudo-state).
 *  \param[in] initialAction
 *                      Pointer to initialization action (optional). The
 *                      function prototype is defined as RKH_TRN_ACT_T. This
 *                      argument is optional, thus it could be declared as
 *                      NULL.
 *  \param[in] initialEvt
 *                      Pointer to an event that will be passed to state
 *                      machine application when it starts. Could be used to
 *                      pass arguments to the state machine like an argc/argv.
 *                      This argument is optional, thus it could be declared
 *                      as NULL or eliminated in compile-time with
 *                      RKH_CFG_SMA_INIT_EVT_EN = 0.
 *
 *  \sa RKH_SM_CONST_CREATE, RKH_SM_GET_CONST, RKH_SM_GET_CONST
 */
#if RKH_CFG_SMA_SM_CONST_EN == RKH_ENABLED
    #define RKH_SM_CREATE(type, name, prio, ppty, initialState, \
                          initialAction, initialEvt) \
        static RKH_SM_CONST_CREATE(name, prio, ppty, initialState, \
                                   initialAction, initialEvt); \
        static type s_##name = {MKSM(&RKH_SM_CONST_NAME(name), initialState)}
#else
    #define RKH_SM_CREATE(type, name, prio, ppty, initialState, \
                          initialAction, initialEvt) \
        static type RKH_SMA_NAME(name) = {MKSM(name, prio, ppty, initialState, \
                                               initialAction, initialEvt)}
#endif

/**
 *  \brief
 *  Initialize (at runtime) a previously created state machine object.
 *
 *  \param[in] me_		Name of state machine. Also, it represents the top 
 *                      state of state diagram.
 *  \param[in] nameSMConst_  
 *                      Describe it 
 *  \param[in] prio_	State machine application priority. A unique priority
 *                      number must be assigned to each SMA from 0 to
 *                      RKH_LOWEST_PRIO. The lower the number, the higher the
 *                      priority.
 *  \param[in] ppty_	State machine properties. The available properties are
 *                      enumerated in RKH_HPPTY_T enumeration in the rkh.h
 *                      file.
 *  \param[in] initialState_ 
 *                      Pointer to initial state. This state could be defined
 *                      either composite or basic (not pseudo-state).
 *  \param[in] initialAction_ 
 *                      Pointer to initialization action (optional). The
 *                      function prototype is defined as RKH_TRN_ACT_T. This
 *                      argument is optional, thus it could be declared as
 *                      NULL.
 *  \param[in] initialEvt_ 
 *                      Pointer to an event that will be passed to state
 *                      machine application when it starts. Could be used to
 *                      pass arguments to the state machine like an argc/argv.
 *                      This argument is optional, thus it could be declared
 *                      as NULL or eliminated in compile-time with
 *                      RKH_CFG_SMA_INIT_EVT_EN = 0.
 */
#if RKH_CFG_SMA_SM_CONST_EN == RKH_ENABLED
    #define RKH_SM_INIT(me_, nameSMConst_, prio_, ppty_, initialState_, \
                        initialAction_, initialEvt_) \
        ((RKH_SM_T *)me_)->romrkh = \
            (RKHROM RKH_ROM_T *)(RKH_SM_GET_CONST_OBJ(nameSMConst_)); \
        ((RKH_SM_T *)me_)->state = \
            (RKHROM struct RKH_ST_T *) \
                ((RKH_SM_GET_CONST_OBJ(nameSMConst_))->istate)
#else
    #define RKH_SM_INIT(me_, nameSMConst_, prio_, ppty_, initialState_, \
                        initialAction_, initialEvt_) \
        MKRT_SM(me_, nameSMConst_, prio_, ppty_, initialState_, \
                initialAction_, initialEvt_)
#endif

/**
 *  \brief
 *  Allocates and then initializes the constant part (in ROM) of the state 
 *  machine.
 *
 *  \param[in] name		Name of state machine application. Also, it represents 
 *                      the top state of state diagram.
 *  \param[in] prio		State machine application priority. A unique priority
 *                      number must be assigned to each SMA from 0 to
 *                      RKH_LOWEST_PRIO. The lower the number, the higher the
 *                      priority.
 *  \param[in] ppty		State machine properties. The available properties are
 *                      enumerated in RKH_HPPTY_T enumeration in the rkh.h
 *                      file.
 *  \param[in] initialState
 *                      Pointer to initial state. This state could be defined
 *                      either composite or basic (not pseudo-state).
 *  \param[in] initialAction
 *                      Pointer to initialization action (optional). The
 *                      function prototype is defined as RKH_TRN_ACT_T. This
 *                      argument is optional, thus it could be declared as
 *                      NULL.
 *  \param[in] initialEvt
 *                      Pointer to an event that will be passed to state
 *                      machine application when it starts. Could be used to
 *                      pass arguments to the state machine like an argc/argv.
 *                      This argument is optional, thus it could be declared
 *                      as NULL or eliminated in compile-time with
 *                      RKH_CFG_SMA_INIT_EVT_EN = 0.
 */
#if RKH_CFG_SMA_SM_CONST_EN == RKH_ENABLED
    #define RKH_SM_CONST_CREATE(name, prio, ppty, initialState, \
                                initialAction, initialEvt) \
        RKHROM RKH_ROM_T RKH_SM_CONST_NAME(name) = \
                                                MKRRKH(name, \
                                                       prio, \
                                                       ppty, \
                                                       initialState, \
                                                       initialAction, \
                                                       initialEvt)
#else
    #define RKH_SM_CONST_CREATE(name, prio, ppty, initialState, \
                                initialAction, initialEvt)
#endif

/**
 *  \brief
 *  Return the address of the constant part from a previously created and 
 *  initializated state machine.
 *
 *  \param[in] sm   Name of state machine.
 */
#if RKH_CFG_SMA_SM_CONST_EN == RKH_ENABLED
    #define RKH_SM_GET_CONST(sm) \
        ((RKH_SM_T *)sm)->romrkh
#else
    #define RKH_SM_GET_CONST(sm) \
        ((RKH_SM_T *)sm)
#endif

/**
 *  \brief
 *  Return the address of the constant part of a state machine.
 *
 *  \param[in] sm   Name of state machine.
 */
#if RKH_CFG_SMA_SM_CONST_EN == RKH_ENABLED
    #define RKH_SM_GET_CONST_OBJ(sm) \
        &RKH_SM_CONST_NAME(sm)
#else
    #define RKH_SM_GET_CONST_OBJ(sm) \
        ((RKH_SM_T *)sm)
#endif

#if RKH_CFG_SMA_SM_CONST_EN == RKH_ENABLED
    #define RKH_SM_GET_OBJ(type, sm)
#else
    #define RKH_SM_GET_OBJ(type, sm) \
        (type *)&RKH_SM_NAME(sm)
#endif

/**
 *  \brief
 *  Declare an opaque pointer pointing to a previously created state 
 *  machine.
 *
 *  \param[in] sm   Name of state machine.
 *
 *  \sa RKH_SMA_DCLR()
 */
#define RKH_SM_DEF_PTR(sm) \
    RKH_SM_T *const sm = (RKH_SM_T *)&RKH_SM_NAME(sm)

/**
 *  \brief
 *  Declare a pointer of specified type pointing to a previously created 
 *  state machine.
 *  
 *  The pointer could be used to hide (opaque) or to publish the internals of 
 *  the class of the state machine. 
 *
 *  \param[in] type Data type of the state machine. Could be derived from 
 *                  RKH_SM_T.
 *  \param[in] sm   Name of state machine.
 *
 *  \sa RKH_SMA_DCLR()
 */
#define RKH_SM_DEF_PTR_TYPE(type, sm) \
    type *const sm = (type *)&RKH_SM_NAME(sm)

/**
 *  \brief
 *  This macro retrieves the state ID of SMA.
 *
 *  \param[in] me	pointer to previously created state machine application.
 *
 *  \return
 *  Id of current state.
 */
#define RKH_GET_CSTATE_ID(me) \
    ((RKH_BASE_T *)(((RKH_SM_T *)me)->state))->id

#if R_TRC_AO_NAME_EN == RKH_ENABLED
    /**
     *  \brief
     *  This macro retrieves the name of a vertex.
     *
     *  \param[in] vx		pointer to previously created vertex object.
     *
     *  \return
     *  Name of vertex object.
     */
    #define RKH_GET_VERTEX_NAME(vx) \
        ((struct RKH_BASE_T *)(vx))->name
#else
    #define RKH_GET_VERTEX_NAME(vx)       noname
#endif

/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
/**
 *  \brief
 *  Return codes from rkh_sm_dispatch() function.
 *
 *  \ingroup apiSM
 */
typedef enum RKH_RCODE_T
{
    /**
     *  The arrived event was succesfully processed and HSM
     *  resides in an allowed state.
     */
    RKH_EVT_PROC,

    /**
     *  The arrived event was't founded in the transition table.
     */
    RKH_EVT_NFOUND,

    /**
     *  The branch function returned a value not founded
     *  in the branch table.
     */
    RKH_CND_NFOUND,

    /**
     *  The transition was cancelled by guard function.
     */
    RKH_GRD_FALSE,

    /**
     *  Unknown state.
     */
    RKH_UNKN_STATE,

    /**
     *  The transition exceeded the allowed hierarchical level.
     */
    RKH_EX_HLEVEL,

    /**
     *  The transition exceeded the allowed number of segments
     *  within a compound transtion.
     */
    RKH_EX_TSEG,
    RKH_NUM_CODES
} RKH_RCODE_T;

/**
 *  \brief
 *  State machine properties.
 *
 *  \ingroup apiSM
 */
typedef enum
{
    /**
     *  Used as state machine property.
     *  This macro enables state nesting in a particular state machine.
     */
    HCAL,

    /**
     *  Used as state machine property.
     *  This macro disables state nesting in a particular state machine.
     *  When FLAT is used in RKH_SMA_CREATE() macro some important features of
     *  RKH are	not included: state nesting, composite state, history
     *  (shallow and deep) pseudostate, entry action, and exit action.
     */
    FLAT,

    /** Number of state machines properties */
    RKH_NUM_HPPTY
} RKH_HPPTY_T;

/**
 *  \brief
 *  Entry action.
 *
 *  The actions that are always executed when a state is entered
 *  should be specified as entry actions. UML statecharts provide optional
 *  entry and exit actions, which are associated with states rather than
 *  transitions, as in a Moore automaton.
 *  An entry function takes the state machine pointer as argument.
 *  This argument is optional, thus it could be eliminated in
 *  compile-time by means of RKH_CFG_SMA_ENT_ARG_SMA_EN.
 *
 *  The RKH implementation preserves the transition sequence imposed by
 *  Harel's Statechart and UML. Specifically, the implemented transition
 *  sequence is as follows:
 *
 *  - Execute exit actions of the source state.
 *  - Execute the transition actions.
 *  - Execute entry actions of the target state.
 *
 *  \note
 *  This callback is referenced from RKH_CREATE_COMP_STATE() and
 *  RKH_CREATE_BASIC_STATE() macros.
 *  \b RKH_CFG_SMA_ENT_ARG_SMA_EN macro.
 */

#if RKH_CFG_SMA_ENT_ARG_SMA_EN == RKH_ENABLED
    #if RKH_CFG_SMA_ENT_ARG_STATE_EN == RKH_ENABLED
    typedef void (*RKH_ENT_ACT_T)(const RKH_SM_T *me,
                                  const RKH_ST_T *state);
    #else
    typedef void (*RKH_ENT_ACT_T)(const RKH_SM_T *me);
    #endif
#else
    #if RKH_CFG_SMA_ENT_ARG_STATE_EN == RKH_ENABLED
    typedef void (*RKH_ENT_ACT_T)(const RKH_ST_T *state);
    #else
    typedef void (*RKH_ENT_ACT_T)(void);
    #endif
#endif

/**
 *  \brief
 *  Exit action.
 *
 *  The actions that are always execute when a state is exited should be
 *  exit actions. UML statecharts provide optional entry and exit actions,
 *  which are associated with states rather than transitions, as in a
 *  Moore automaton.
 *  An exit function takes the state machine pointer as argument.
 *  This argument is optional, thus it could be eliminated in
 *  compile-time by means of RKH_CFG_SMA_EXT_ARG_SMA_EN.
 *
 *  The RKH implementation preserves the transition sequence imposed by
 *  Harel's Statechart and UML. Specifically, the implemented transition
 *  sequence is as follows:
 *
 *  - Execute exit actions of the source state.
 *  - Execute the transition actions.
 *  - Execute entry actions of the target state.
 *
 *  \note
 *  This callback is referenced from RKH_CREATE_COMP_STATE() and
 *  RKH_CREATE_BASIC_STATE() macros.
 */

#if RKH_CFG_SMA_EXT_ARG_SMA_EN == RKH_ENABLED
    #if RKH_CFG_SMA_ENT_ARG_STATE_EN == RKH_ENABLED
    typedef void (*RKH_EXT_ACT_T)(const RKH_SM_T *me,
                                  const RKH_ST_T *state);
    #else
    typedef void (*RKH_EXT_ACT_T)(const RKH_SM_T *me);
    #endif
#else
    #if RKH_CFG_SMA_ENT_ARG_STATE_EN == RKH_ENABLED
    typedef void (*RKH_EXT_ACT_T)(const RKH_ST_T *state);
    #else
    typedef void (*RKH_EXT_ACT_T)(void);
    #endif
#endif

/**
 *  \brief
 *  Event preprocessor.
 *
 *  Before sending the arrived event to state machine, it can be previously
 *  processed using the	event preprocessor function.
 *  The RKH framework provides optional event preprocessor
 *  action, which are associated with states rather than transitions,
 *  as in a Moore automaton.
 *  This action takes the state machine pointer and the event
 *  pointer as arguments.
 *  This argument is optional, thus it could be eliminated in
 *  compile-time by means of RKH_CFG_SMA_PPRO_ARG_SMA_EN.
 *
 *  \note
 *  This callback is referenced from RKH_CREATE_COMP_STATE() and
 *  RKH_CREATE_BASIC_STATE() macros.
 */
#if RKH_CFG_SMA_PPRO_ARG_SMA_EN == RKH_ENABLED
    typedef RKH_SIG_T (*RKH_PPRO_T)(const RKH_SM_T *sma,
                                    RKH_EVT_T *pe);
#else
    typedef RKH_SIG_T (*RKH_PPRO_T)(RKH_EVT_T *pe);
#endif

/**
 *  \brief
 *  Transition action.
 *
 *  Transition actions are small atomic behaviors executed at specified points
 *  in a state machine. This actions are assumed to take an insignificant
 *  amount of time to execute and are noninterruptible. UML statecharts
 *  are extended state machines with characteristics of both Mealy and
 *  Moore automata. In statecharts, actions generally depend on both
 *  the state of the system and the triggering event, as in a Mealy
 *  automaton. Additionally, UML statecharts provide optional entry and
 *  exit actions, which are associated with states rather than transitions,
 *  as in a Moore automaton.
 *  An action function takes the state machine pointer and the event
 *  pointer as arguments.
 *  These arguments are optional, thus they could be eliminated in
 *  compile-time by means of RKH_CFG_SMA_ACT_ARG_EVT_EN and \b
 *  RKH_CFG_SMA_ACT_ARG_SMA_EN.
 *
 *  The RKH implementation preserves the transition sequence imposed by
 *  Harel's Statechart and UML. Specifically, the implemented transition
 *  sequence is as follows:
 *
 *  - Execute exit actions of the source state.
 *  - Execute the transition actions.
 *  - Execute entry actions of the target state.
 *
 *  \note
 *  This callback is referenced from RKH_TRREG() and RKH_TRINT()macro.
 */

#if (RKH_CFG_SMA_ACT_ARG_EVT_EN == RKH_ENABLED && \
     RKH_CFG_SMA_ACT_ARG_SMA_EN == RKH_ENABLED)
    typedef void (*RKH_TRN_ACT_T)(const RKH_SM_T *me,
                                  RKH_EVT_T *pe);
#elif (RKH_CFG_SMA_ACT_ARG_EVT_EN == RKH_ENABLED && \
       RKH_CFG_SMA_ACT_ARG_SMA_EN == RKH_DISABLED)
    typedef void (*RKH_TRN_ACT_T)(RKH_EVT_T *pe);
#elif (RKH_CFG_SMA_ACT_ARG_EVT_EN == RKH_DISABLED && \
       RKH_CFG_SMA_ACT_ARG_SMA_EN == RKH_ENABLED)
    typedef void (*RKH_TRN_ACT_T)(const RKH_SM_T *me);
#else
    typedef void (*RKH_TRN_ACT_T)(void);
#endif

/**
 *  \brief
 *  Guard.
 *
 *	A guard is a boolean condition that returns a true (RKH_GTRUE) or
 *	false (RKH_GFALSE) value that controls whether or not a transition
 *	is taken following the receipt of a triggering event. A transition
 *	with a guard is only take if the triggering event occurs and the
 *	guard evaluates to true. As long as the guard evaluated to false,
 *	the triggering event would be discarded and the transition would
 *	not be taken.
 *	Each condition connector can have one special branch with a guard
 *	labeled rkh_sm_else, which is taken if all the guards on the other
 *	branches are false.
 *  A guard function takes the state machine pointer and the event
 *  pointer as arguments.
 *  These arguments are optional, thus they could be eliminated in
 *  compile-time by means of \b RKH_CFG_SMA_GRD_ARG_EVT_EN and
 *  \b RKH_CFG_SMA_GRD_ARG_SMA_EN.
 *
 *  \note
 *  This callback is referenced from RKH_BRANCH() macro.
 */

#if (RKH_CFG_SMA_GRD_ARG_EVT_EN == RKH_ENABLED && \
     RKH_CFG_SMA_GRD_ARG_SMA_EN == RKH_ENABLED)

    typedef rbool_t (*RKH_GUARD_T)(const RKH_SM_T *me,
                                   RKH_EVT_T *pe);
#elif (RKH_CFG_SMA_GRD_ARG_EVT_EN == RKH_ENABLED && \
       RKH_CFG_SMA_GRD_ARG_SMA_EN == RKH_DISABLED)
    typedef rbool_t (*RKH_GUARD_T)(RKH_EVT_T *pe);
#elif (RKH_CFG_SMA_GRD_ARG_EVT_EN == RKH_DISABLED && \
       RKH_CFG_SMA_GRD_ARG_SMA_EN == RKH_ENABLED)
    typedef rbool_t (*RKH_GUARD_T)(const RKH_SM_T *me);
#else
    typedef rbool_t (*RKH_GUARD_T)(void);
#endif

#if RKH_CFG_SMA_SM_CONST_EN == RKH_ENABLED
/**
 *  \brief
 *  Constant parameters of state machine.
 *
 *	The constant key parameters of a state machine are allocated within.
 *
 *	\sa
 *	RKH_SMA_T structure definition for more information. Also,
 *	\link RKH_EVT_T single inheritance in C \endlink, and
 *	\link RKH_CREATE_BASIC_STATE another example \endlink.
 */
struct RKH_ROM_T
{
    /**
     *  \brief
     *  SMA (a.k.a Active Object) priority.
     *
     *  A unique priority number must be assigned to each SMA from 0 to
     *  RKH_LOWEST_PRIO. The lower the number, the higher the priority.
     */
    rui8_t prio;

    /**
     *  \brief
     *  State machine properties.
     *
     *  The available properties are enumerated in RKH_HPPTY_T enumeration in
     *  the rkh.h file.
     */
    rui8_t ppty;

    /**
     *  \brief
     *  Name of State Machine Application (a.k.a Active Object).
     *
     *  Pointer to an ASCII string (NULL terminated) to assign a name to the
     *	State Machine Application (a.k.a Active Object). The name can be
     *	displayed by debuggers or by Trazer.
     */
#if R_TRC_AO_NAME_EN == RKH_ENABLED
    const char *name;
#endif

    /**
     *  \brief
     *  Points to initial state.
     *
     *  This state could be defined either composite or basic
     *  (not pseudo-state).
     */
    RKHROM RKH_ST_T *istate;

    /**
     *  \brief
     *  Points to initializing action (optional).
     *
     *  The function prototype is defined as RKH_TRN_ACT_T. This argument is
     *  optional, thus it could be declared as NULL.
     */
    RKH_TRN_ACT_T iaction;

    /**
     *  \brief
     *	Pointer to an event that will be passed to state machine application
     *	when it starts. Could be used to pass arguments to the state machine
     *	like an argc/argv. This argument is optional, thus it could be
     *	declared as NULL or eliminated in compile-time with
     *	RKH_CFG_SMA_INIT_EVT_EN = 0.
     */
#if RKH_CFG_SMA_INIT_EVT_EN == RKH_ENABLED
    const RKH_EVT_T *ievent;
#endif
};
#endif

/**
 *  \brief
 *  Describes the state machine.
 *
 *  RKH_SM_T is not intended to be instantiated directly, but rather
 *  serves as the base structure for derivation of state machines in the
 *  application code. Also, is the base structure of active object structure 
 *  RKH_SMA_T.
 *
 *  \ingroup apiSM
 */
#if RKH_CFG_SMA_SM_CONST_EN == RKH_ENABLED
struct RKH_SM_T
{
    /**
     *  \brief
     *  Points to constant parameters of state machine.
     */
    RKHROM RKH_ROM_T *romrkh;

    /**
     *  \brief
     *  Points to current stable state (simple or final state).
     */
    RKHROM RKH_ST_T *state;

    /**
     *  \brief
     *  Points to the propagated event.
     */
    RKH_EVT_T* propagatedEvent;
};
#else
struct RKH_SM_T
{
    /**
     *  \brief
     *  SMA (a.k.a Active Object) priority.
     *
     *  A unique priority number must be assigned to each SMA from 0 to
     *  RKH_LOWEST_PRIO. The lower the number, the higher the priority.
     */
    rui8_t prio;

    /**
     *  \brief
     *  State machine properties.
     *
     *  The available properties are enumerated in RKH_HPPTY_T enumeration in
     *  the rkh.h file.
     */
    rui8_t ppty;

    /**
     *  \brief
     *  Name of State Machine Application (a.k.a Active Object).
     *
     *  Pointer to an ASCII string (NULL terminated) to assign a name to the
     *	State Machine Application (a.k.a Active Object). The name can be
     *	displayed by debuggers or by Trazer.
     */
#if R_TRC_AO_NAME_EN == RKH_ENABLED
    const char *name;
#endif

    /**
     *  \brief
     *  Points to initial state.
     *
     *  This state could be defined either composite or basic
     *  (not pseudo-state).
     */
    RKHROM RKH_ST_T *istate;

    /**
     *  \brief
     *  Points to initializing action (optional).
     *
     *  The function prototype is defined as RKH_TRN_ACT_T. This argument is
     *  optional, thus it could be declared as NULL.
     */
    RKH_TRN_ACT_T iaction;

    /**
     *  \brief
     *	Pointer to an event that will be passed to state machine application
     *	when it starts. Could be used to pass arguments to the state machine
     *	like an argc/argv. This argument is optional, thus it could be
     *	declared as NULL or eliminated in compile-time with
     *	RKH_CFG_SMA_INIT_EVT_EN = 0.
     */
#if RKH_CFG_SMA_INIT_EVT_EN == RKH_ENABLED
    const RKH_EVT_T *ievent;
#endif

    /**
     *  \brief
     *  Points to current stable state (simple or final state).
     */
    RKHROM RKH_ST_T *state;

    /**
     *  \brief
     *  Points to the propagated event.
     */
    RKH_EVT_T* propagatedEvent;
};
#endif

/**
 *  \brief
 *  Maintains the basic information of a state.
 */
struct RKH_BASE_T
{
    /**
     *  \brief
     *  State type.
     *
     *	Contains the type of a particular state and can have
     *	the following values:
     *
     *	- \b RKH_COMPOSITE:     composite state.
     *	- \b RKH_BASIC:         basic state.
     *	- \b RKH_CHOICE:        choice pseudostate.
     *	- \b RKH_CONDITIONAL:   conditional pseudostate.
     *	- \b RKH_SHISTORY:      shadow history pseudostate.
     *	- \b RKH_DHISTORY:      deep history pseudostate.
     */
    ruint type;

    /**
     *  \brief
     *  Name of state or pseudostate.
     *
     *  Pointer to an ASCII string (NULL terminated) to assign a name to the
     *	state object or pseudostate object. The name can be displayed by
     *	debuggers or by Trazer.
     */
#if R_TRC_AO_NAME_EN == RKH_ENABLED
    const char *name;
#endif
};

/**
 *  \brief
 *  Describes the state transition.
 *
 *  Transitions represent the response of a state machine to events.
 *  Any event that is not explicitly listed as causing an event to occur
 *  in a given state is quietly discarded should it occur.
 */
struct RKH_TR_T
{
    /**
     *  \brief
     *  Triggering event.
     */
    RKH_SIG_T event;

    /**
     *  \brief
     *	Points to guard function.
     */
    RKH_GUARD_T guard;

    /**
     *  \brief
     *  Points to transition action.
     */
    RKH_TRN_ACT_T action;

    /**
     *  \brief
     *  Points to target state.
     */
    RKHROM void *target;
};

/**
 *	\brief
 *  Describes the common properties of regular states (basic, composite,
 *  and submachine).
 */
struct RKH_ST_T
{
    /**
     *  \brief
     *	Maintains the basic information of state.
     */
    RKH_BASE_T base;

#if RKH_CFG_SMA_HCAL_EN == RKH_ENABLED
    /**
     *  \brief
     *	Points to entry action.
     */
    RKH_ENT_ACT_T enter;

    /**
     *  \brief
     *	Points to exit action.
     */
    RKH_EXT_ACT_T exit;

    /**
     *  \brief
     *	Points to state's parent.
     */
    RKHROM RKH_ST_T *parent;
#endif
};

/**
 *	\brief
 *  Describes a basic state.
 */
struct RKH_SBSC_T
{
    RKH_ST_T st;

    /**
     *  \brief
     *	Points to state transition table.
     */
    RKHROM RKH_TR_T *trtbl;

    /**
     *  \brief
     *	Points to event preprocessor.
     *
     *	Aditionally, by means of single inheritance in C it could be
     *	used as state's abstract data.
     *	Aditionally, implementing the single inheritance in C is very
     *	simply by literally embedding the base type, RKH_PPRO_T in
     *	this case, as the first member of the derived structure.
     *
     *	This argument is optional, thus it could be declared as NULL.
     *
     *	Example:
     *
     *  \code
     *	static
     *	RKH_SIG_T
     *	preprocessor(RKH_EVT_T *pe)
     *	{
     *		...
     *	}
     *
     *	typedef struct
     *	{
     *		RKH_PPRO_T prepro;  // extend the RKH_PPRO_T class
     *		unsigned min:4;
     *		unsigned max:4;
     *		char *buff;
     *	} SDATA_T;
     *
     *	static const SDATA_T option = {preprocessor, 4, 8, token1};
     *
     *	RKH_CREATE_BASIC_STATE(S111, set_x_1,
     *						   NULL, &S11, preprocessor);
     *	RKH_CREATE_BASIC_STATE(S22, set_x_4,
     *						   NULL, &S2, (RKH_PPRO_T*)&option);
     * \endcode
     */
#if RKH_CFG_SMA_PPRO_EN == RKH_ENABLED
    RKH_PPRO_T prepro;
#endif
};

/**
 *	\brief
 *  Describes a composite state.
 */
struct RKH_SCMP_T
{
    RKH_ST_T st;

    /**
     *  \brief
     *	Points to state transition table.
     */
    RKHROM RKH_TR_T *trtbl;

    /**
     *  \brief
     *	Points to event preprocessor.
     *
     *	Aditionally, by means of single inheritance in C it could be
     *	used as state's abstract data.
     *	Aditionally, implementing the single inheritance in C is very
     *	simply by literally embedding the base type, RKH_PPRO_T in
     *	this case, as the first member of the derived structure.
     *
     *	This argument is optional, thus it could be declared as NULL.
     *
     *	Example:
     *
     *  \code
     *	static
     *	RKH_SIG_T
     *	preprocessor( RKH_EVT_T *pe )
     *	{
     *		...
     *	}
     *
     *	typedef struct
     *	{
     *		RKH_PPRO_T prepro;  // extend the RKH_PPRO_T class
     *		unsigned min:4;
     *		unsigned max:4;
     *		char *buff;
     *	} SDATA_T;
     *
     *	static const SDATA_T option = { preprocessor, 4, 8, token1 };
     *
     *	RKH_CREATE_BASIC_STATE( S111, set_x_1,
     *							NULL, &S11, preprocessor );
     *	RKH_CREATE_BASIC_STATE( S22, set_x_4,
     *							NULL, &S2, (RKH_PPRO_T*)&option );
     * \endcode
     */
#if RKH_CFG_SMA_PPRO_EN == RKH_ENABLED
    RKH_PPRO_T prepro;
#endif

#if RKH_CFG_SMA_HCAL_EN == RKH_ENABLED
    /**
     *  \brief
     *	Points to state's default child.
     */
    RKHROM void *defchild;

    /**
     *  \brief
     *  Points to state's initial action.
     */
    RKH_TRN_ACT_T initialAction;

    /**
     *  \brief
     *	Points to state's history.
     */
#if defined(RKH_HISTORY_ENABLED)
    RKHROM RKH_SHIST_T *history;
#endif
#endif
};

/**
 *	\brief
 *  Describes a final state.
 */
struct RKH_FINAL_T
{
    /**
     *  \note
     *  A final state has neither exit behavior nor entry behavior.
     */
    RKH_ST_T st;

    /**
     *  \note
     *	A final state cannot have any outgoing transitions.
     */
    RKHROM RKH_TR_T *trtbl;

    /**
     *  \note
     *	A final state cannot have any outgoing transitions.
     */
#if RKH_CFG_SMA_PPRO_EN == RKH_ENABLED
    RKH_PPRO_T prepro;
#endif
};

/**
 *  \brief
 *  Describes the exit point connection.
 */
#if defined(RKH_SUBMACHINE_ENABLED)
struct RKH_EXPCN_T
{
    /**
     *  \brief
     *  Points to transition action.
     */
    RKH_TRN_ACT_T action;

     /**
     *  \brief
     *  Points to target state.
     */
    RKHROM void *target;
};

/**
 *  \brief
 *  Describes the entry point connection.
 */
struct RKH_ENPCN_T
{
    /**
     *  \brief
     *  Points to transition action.
     */
    RKH_TRN_ACT_T action;

    /**
     *  \brief
     *  Points to target state.
     */
    RKHROM void *target;
};

/**
 *	\brief
 *  Describes a submachine state.
 *
 *  A submachine state is a kind of a state that actually refers to
 *  another defined state machine.
 *  A submachine state is logically equivalent to the insertion of the
 *  referenced state machine as a composite state in the place of
 *  the submachine state. Consequently, every entrance to a submachine
 *  state is equivalent to the corresponding entrance to the inserted
 *  (referenced) composite state. In particular, it can be entered
 *  thruough its initial pseudostate (as any other composite state), or
 *  through one of its entry points.
 *
 *  Similary, every exit from a submachine state is equivalent to the
 *  corresponding exit from the inserted composite state. It can be exited
 *  through one of its exit points. When it is exited through its exit point
 *  the effect of the transition targeting the exit point is executed first,
 *  followed by the exit behavior of the composite state.
 *
 *  The entry, exit, and behavior actions and internal transitions are
 *  defined as part of the state. Submachine state is a decomposition
 *  mechanism that allows factoring of common behaviors and their reuse.
 *
 *  The purpose od defining submachine states is to decompose and localize
 *  repetitive parts because the same state machine can be referenced from
 *  more than one submachine state.
 *
 *	The diagram in following figure shows a fragment from a state machine
 *	diagram in which a submachine state (the \c SB) is referenced.
 *
 *	\anchor fig_sbm1
 *	\image html sbm1.png "Submachine state"
 *
 *	In the above example, the transition triggered by event \c TWO will
 *	terminate on entry point \c ENS12 of the \c SB state machine.
 *	The \c ONE transition implies taking of the default transition of the
 *	\c SB and executes the \c act5() action. The transition emanating from
 *	the \c EX1S12 exit point of the submachine will execute the \c act1()
 *	behavior in addition to what is executed within the \c SB state machine.
 *	Idem transition emanating from the \c EX2S12.
 *	This transition must have been triggered within the \c SB state machine.
 *	Finally, the transition emanating from the edge of the submachine state
 *	is triggered by event \c THREE.
 *
 *	The following figure is an example of a state machine \c SB defined with
 *	two exit points, \c EXPNT1 and \c EXPNT2, and one entry point \c ENPNT.
 *
 *	\anchor fig_sbm2
 *	\image html sbm2.png "State machine with two exit points and one entry
 *  point"
 *
 *	In the following figure the state machine shown above is referenced twice in
 *	a submachine state \c S12 and \c S2.
 *
 *	\anchor fig_sbm3
 *	\image html sbm3.png "Submachine state with usage of exit and entry points"
 */
struct RKH_SSBM_T
{
    RKH_ST_T st;

    /**
     *  \brief
     *	Points to state transition table.
     */
    RKHROM RKH_TR_T *trtbl;

    /**
     *  \brief
     *	Points to state transition table.
     */
    RKHROM RKH_EXPCN_T *exptbl;

    /**
     *  \brief
     *	Points to submachine object.
     */
    RKHROM RKH_RSM_T *sbm;
};

/**
 *	\brief
 *  Describes a (referenced) submachine state machine.
 */
struct RKH_RSM_T
{
    /**
     *  \brief
     *	Maintains the basic information of state.
     */
    RKH_BASE_T base;

    /**
     *  \brief
     *	Points to state's default child.
     */
    RKHROM void *defchild;

    /**
     *  \brief
     *  Points to initializing action (optional).
     *
     *  The function prototype is defined as RKH_TRN_ACT_T. This argument is
     *  optional, thus it could be declared as NULL.
     */
    RKH_TRN_ACT_T iaction;

    /**
     *  \brief
     *	Points to RAM memory location which stores
     *	the dynamic parent.
     */
    RKHROM RKH_ST_T * *dyp;
};

/**
 *  \brief
 *  Describes the entry point pseudostate.
 *
 *  An entry pseudostate is used to join an external transition terminating
 *  on that entry point to an internal transition emanating from that entry
 *  point.
 *  The main purpose of such entry and exit points is to execute the state
 *  entry and exit actions respectively in between the actions that are
 *  associated with the joined transitions.
 */
struct RKH_SENP_T
{
    /**
     *  \brief
     *	Maintains the basic information of state.
     */
    RKH_BASE_T base;

    /**
     *  \brief
     *	Points to entry point connection.
     */
    RKHROM RKH_ENPCN_T *enpcn;

    /**
     *  \brief
     *	Points to state's parent (submachine state).
     */
    RKHROM RKH_ST_T *parent;
};

/**
 *  \brief
 *  Describes the exit point pseudostate.
 *
 *  An exit pseudostate is used to join an internal transition terminating on
 *  that exit point to an external transition emanating from that exit point.
 *  The main purpose of such entry and exit points is to execute the state
 *  entry and exit actions respectively in between the actions that are
 *  associated with the joined transitions.
 */
struct RKH_SEXP_T
{
    /**
     *  \brief
     *	Maintains the basic information of state.
     */
    RKH_BASE_T base;

    /**
     *  \brief
     *	Index of exit point table.
     */
    rui8_t ix;

    /**
     *  \brief
     *	Points to state's parent (referenced submachine).
     */
    RKHROM RKH_RSM_T *parent;
};
#endif

/**
 *  \brief
 *  Describes the conditional pseudostate.
 */
#if defined(RKH_CHOICE_OR_CONDITIONAL_ENABLED)
struct RKH_SCOND_T
{
    /**
     *  \brief
     *	Maintains the basic information of state.
     */
    RKH_BASE_T base;

    /**
     *  \brief
     *	Points to branch table.
     */
    RKHROM RKH_TR_T *trtbl;
};
#endif

/**
 *  \brief
 *  Describes the choice pseudostate.
 */
#if defined(RKH_CHOICE_ENABLED)
struct RKH_SCHOICE_T
{
    /**
     *  \brief
     *	Maintains the basic information of state.
     */
    RKH_BASE_T base;

    /**
     *  \brief
     *	Points to branch table.
     */
    RKHROM RKH_TR_T *trtbl;
};
#endif

/**
 *  \brief
 *  Describes the history pseudostate
 *
 *  It can be either be shallow or deep.
 */
struct RKH_SHIST_T
{
    /**
     *  \brief
     *	Maintains the basic information of state.
     */
    RKH_BASE_T base;

    /**
     *  \brief
     *	Points to state's parent.
     */
    RKHROM RKH_ST_T *parent;

    /**
     *  \brief
     *	Points to RAM memory location which stores
     *	the state's history.
     */
    RKHROM RKH_ST_T **target;
    
    /**
     *  \brief
     *  History default transition.
     */
    RKHROM RKH_TR_T trn;
};

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
/**
 *  \brief
 *  Inits a previously created state machine calling its initializing action.
 *
 *  \param[in] me  pointer to previously created state machine.
 *
 *  \ingroup apiSM
 */
void rkh_sm_init(RKH_SM_T *me);

/**
 *  \brief
 *	Executes a state machine in a run-to-completation (RTC) model.
 *
 *	In this model, before the system handles a new event it can store it
 *	until the previous event has completed processing. This model is
 *	called run to completion or RTC. Thus, the system processes events in
 *	discrete, indivisible RTC steps. An RTC step is the period of time in
 *	which events are accepted and acted upon. Processing an event always
 *	completes within a single model step, including exiting the source
 *	state, executing any associated actions, and entering the target state.
 *  The RKH implementation preserves the transition sequence imposed by
 *  Harel's Statechart and UML. Specifically, the implemented transition
 *  sequence is as follows:
 *
 *  - Execute exit actions of the source state.
 *  - Execute the transition actions.
 *  - Execute entry actions of the target state.
 *
 *  Here now are the details of the main part of this algorithm
 *  (rkh_sm_dispatch()), in which an event is processed by the statechart.
 *
 *  \li	Determine the compound transition (CT) that will fire in response
 *      to the event: traverse the states in the active configuration from
 *      lowest states in the hierarchy upwards. A CT is enabled if its
 *      trigger is the dispatched event, and the guard evaluates to true.
 *      Once an enabled transition is found with a given source state stop
 *      traversing the states that are higher than this state in the
 *      hierarchy.
 *  \li	Perform the CT that we found should fire.
 *      For each transition do:
 *      -	Update histories of exited states.
 *      -   Perform the exit actions of the exited states according to the
 *          order states are exited, from low state to high state.
 *      -	Perform the actions on the CT sequentially according to the
 *          order in which they are written on the transition, from the
 *          action closest to source state to the action closest to target
 *          state.
 *      -	Perform the entry actions of the entered states according to
 *          the order states are entered, from high state to low state.
 *      -	For lowest level states that were entered, which are not basic
 *          states, perform default transitions (recursively) until the
 *          statechart reaches basic states.
 *      -	Update the active configuration.
 *  \li	Wrap up: once a stable configuration is reached, the reaction to the
 *      event is completed, control returns to the dispatcher and new events
 *      can be dispatched.
 *
 *  \param[in] me   pointer to previously created state machine application.
 *	\param[in] e	pointer to arrived event. It's used as state-machine's
 *					input alphabet.
 *
 *	\return         Result #RKH_RCODE_T code.
 *
 *  \ingroup apiSM
 */
ruint rkh_sm_dispatch(RKH_SM_T *me, RKH_EVT_T *e);

/**
 *  \brief
 *  Initializes the attributes of the state machine instance.
 *
 *  \param[in] me  pointer to previously created state machine.
 *
 *  \note
 *  The initializer assumes that memory has previously been allocated for the 
 *  object (either statically or dynamically).
 *
 *  \ingroup apiSM
 */
void rkh_sm_ctor(RKH_SM_T *me);

/**
 *  \brief
 *  Erase the history of a state. It can be a shallow or deep history.
 *
 *  \param[in] h    pointer to history pseudostate.
 *
 *  \ingroup apiSM
 */
void rkh_sm_clear_history(RKHROM RKH_SHIST_T *h);

#if (RKH_CFG_SMA_GRD_ARG_EVT_EN == RKH_ENABLED && \
     RKH_CFG_SMA_GRD_ARG_SMA_EN == RKH_ENABLED)
rbool_t rkh_sm_else(const RKH_SM_T *sma, RKH_EVT_T *pe);
#elif (RKH_CFG_SMA_GRD_ARG_EVT_EN == RKH_ENABLED && \
       RKH_CFG_SMA_GRD_ARG_SMA_EN == RKH_DISABLED)
rbool_t rkh_sm_else(RKH_EVT_T *pe);
#elif (RKH_CFG_SMA_GRD_ARG_EVT_EN == RKH_DISABLED && \
       RKH_CFG_SMA_GRD_ARG_SMA_EN == RKH_ENABLED)
rbool_t rkh_sm_else(const RKH_SM_T *sma);
#else
rbool_t rkh_sm_else(void);
#endif

/**
 *  \brief
 *  Propagate an event to self. This event will be processed subsequently,
 *  without exiting from the current execution of rkh_sma_dispatch().
 *  This feature has the same purpose as posting the interesting event to
 *  this active object, but it ensures that the event is processed without
 *  competing with other active objects that are ready to run.
 *
 *  \param[in] me   pointer to previously created state machine.
 *	\param[in] e    pointer to arrived event. It's used as state-machine's
 *					input alphabet.
 *
 *  \note
 *  This function was intented to be called from a state machine action.
 *
 *  \ingroup apiSM
 */
void rkh_sm_propagate(RKH_SM_T *me, RKH_EVT_T* e);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
