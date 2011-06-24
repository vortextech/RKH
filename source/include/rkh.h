/*
 *	file: rkh.h
 *	Last updated for version: 1.0.00
 *	Date of the last update:  May 28, 2010
 *
 * 	Copyright (C) 2010 Leandro Francucci. All rights reserved.
 *
 * 	RKH is free software: you can redistribute it and/or modify
 * 	it under the terms of the GNU General Public License as published by
 * 	the Free Software Foundation, either version 3 of the License, or
 * 	(at your option) any later version.
 *
 *  RKH is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with RKH, see copying.txt file.
 *
 * Contact information:
 * RKH web site:	http://
 * e-mail:			francuccilea@gmail.com
 */

/**
 * 	\file rkh.h
 *	\brief
 *	
 * 	RKH platform-independent interface.
 *
 * 	This header file must be included in all modules 
 * 	(*.c files) that use RKH.
 */


#ifndef __RKH_H__
#define __RKH_H__


#include <stdlib.h>
#include "rkhitl.h"


/** 	
 *  Return codes from rkh_engine() function.
 */

typedef enum
{
	/**
	 * 	The arrived event was succesfully processed and HSM 
	 * 	resides in a allowed state.
	 */

	RKH_OK,

	/**
	 * 	The arrived event was't founded in the transition table. 
	 */

	RKH_INPUT_NOT_FOUND,

	/**
	 * 	The branch function returned a value not founded 
	 * 	in the branch table.
	 */

	RKH_CONDITION_NOT_FOUND,

	/**
	 * 	The transition was cancelled by guard function.
	 */

	RKH_GUARD_FALSE,

	/**
	 * 	Unknown state. 
	 */

	RKH_UNKNOWN_STATE,

	/**
	 * 	The transition exceeded the allowed hierarchical level.
	 */

	RKH_EXCEED_HCAL_LEVEL,

	/**
	 * 	The transition exceeded the allowed number of segments 
	 * 	within a compound transtion.
	 */

	RKH_EXCEED_TR_SEGS,

	/** Number of returned codes */
	RKH_NUM_CODES
} RKH_RCODE_T;


/**
 * 	Each condition connector can have one special branch with a guard 
 *	labeled ELSE, which is taken if all the guards on the other 
 *	branches are false.
 *
 * 	A guard function takes the state machine pointer and the event 
 * 	pointer as arguments. These parameters are optional in compile-time
 * 	according to RKH_EN_GRD_EVT_ARG and RKH_EN_GRD_HSM_ARG.
 */

#define ELSE		rkh_else


/** 	
 *  State machine properties.
 */

typedef enum
{
	/**
	 * 	Used as state machine property.
	 * 	This macro enables state nesting in a particular state-machine.
	 */

	HCAL,


	/**
	 * 	Used as state machine property.
	 * 	This macro disables state nesting in a particular state-machine.
	 * 	When FLAT is used in RKH_CREATE_HSM() macro some important features of 
	 * 	RKH are	not included: state nesting, composite state, history 
	 * 	(shallow and deep) pseudostate, entry action, and exit action.
	 */

	FLAT,

	/** Number of state machines properties */
	RKH_NUM_HPPTY
} RKH_HPPTY_T;


/**
 *	This macro creates a HSM control block. 
 *
 *	\note
 *	
 *	See RKH_T structure definition for more information.
 *
 * 	\param name		state-machine (top-state) name. Represents a HSM structure.
 * 	\param id		the value of HSM ID.
 * 	\param ppty		state machine properties. The available properties are
 * 					enumerated in RKH_HPPTY_T enumeration in the rkh.h
 * 					file.
 * 	\param is		pointer to regular initial state. This state could be 
 * 					defined either composite or basic.
 * 	\param ia		pointer to initialization action. The function prototype 
 * 					is defined as RKHINIT_T. This argument is optional, thus
 * 					it could be declared as NULL.
 * 	\param hd		pointer to HSM's abstract data. This argument is optional, 
 * 					thus it could be declared as NULL.
 */

#define RKH_CREATE_HSM( name, id, ppty, is, ia, hd )					\
																		\
								RKH_T name = CHSM( id,ppty,name,is,ia,hd )


/**
 *	This macro creates a composite state.
 *
 *	\note
 *
 *	See RKHSREG_T structure definition for more information.
 *
 * 	\param name		state name. Represents a composite state structure.
 * 	\param id		the value of state ID.	
 * 	\param en		pointer to state entry action. This argument is 
 *					optional, thus it could be declared as NULL.
 * 	\param ex		pointer to state exit action. This argument is 
 *					optional, thus it could be declared as NULL.
 * 	\param parent	pointer to parent state.
 * 	\param defchild	pointer to default child state or pseudostate.
 * 	\param history	pointer history pseudostate. This argument is 
 *					optional, thus it could be declared as NULL.
 */

#define RKH_CREATE_COMP_STATE( name,id,en,ex,parent,defchild,history )	\
																		\
								extern rkhrom RKHTR_T name##_trtbl[];	\
																		\
								rkhrom RKHSREG_T name =					\
								{										\
									mkbase(RKH_COMPOSITE,id,name),		\
									mkcomp(en,ex,parent,name,			\
												defchild,history)		\
								}


/**
 *	This macro creates a basic state.
 *
 *	\note
 *
 *	See RKHSREG_T structure definition for more information.
 *
 * 	\param name		state name. Represents a basic state structure.
 * 	\param id		the value of state ID.	
 * 	\param en		pointer to state entry action. This argument is 
 *					optional, thus it could be declared as NULL.
 * 	\param ex		pointer to state exit action. This argument is 
 *					optional, thus it could be declared as NULL.
 * 	\param parent	pointer to parent state.
 * 	\param prepro	pointer to input preprocessor function. This argument is 
 *					optional, thus it could be declared as NULL.
 *					Aditionally, by means of single inheritance in C it 
 *					could be used as state's abstract data. 
 *					Aditionally, implementing the single inheritance in C 
 *					is very simply by literally embedding the base type, 
 *					RKHPPRO_T in this case, as the first member of the 
 *					derived structure. See member "prepro" of RKHSREG_T 
 *					structure for more information.
 */

#define RKH_CREATE_BASIC_STATE( name,id,en,ex,parent,prepro )			\
																		\
								extern rkhrom RKHTR_T name##_trtbl[];	\
																		\
								rkhrom RKHSREG_T name =					\
								{										\
									mkbase(RKH_BASIC,id,name),			\
									mkbasic(en,ex,parent,name,prepro)	\
								}

/**
 *	This macro creates a conditional pseudostate. A condition connector has 
 *	one incoming transition and can have several outgoing transition segments 
 *	called branches. Branches are labeled with guards that determine which 
 *	one is to be actually taken. Since the condition connector is an OR 
 *	connector, only one of the branches can be taken. Each 
 *	condition connector can have one special branch with a guard labeled 
 *	rkh_else, which is taken if all the guards on the other branches are 
 *	false. 
 *	Branches cannot contain triggers, but in addition to a guard they may 
 *	contain actions. A branch can enter another condition connector, thus 
 *	providing for the nesting of branches.
 *	
 *	\note
 *
 *	See RKHSCOND_T structure definition for more information.
 *
 * 	\param name		pseudostate name. Represents a conditional pseudostate 
 * 					structure.
 * 	\param id		the value of state ID.	
 */

#define RKH_CREATE_COND_STATE( name,id )								\
																		\
								extern rkhrom RKHTR_T name##_trtbl[];	\
																		\
								rkhrom RKHSCOND_T name =				\
								{										\
									mkbase(RKH_CONDITIONAL,id,name),	\
									name##_trtbl 						\
								}


/**
 *	This macro creates a junction pseudostate.
 *	Transitions arrows can be joined using junction pseudostate. 
 *	Multiple entrances and exits may be attached to a junction.
 *
 *	\note
 *
 *	See RKHSJUNC_T structure definition for more information.
 *
 * 	\param name		pseudostate name. Represents a junction pseudostate 
 * 					structure.
 * 	\param id		the value of state ID.	
 * 	\param action	pointer to transition action. This argument is optional, 
 * 					thus it could be declared as NULL.
 * 	\param target	pointer to target state.
 */

#define RKH_CREATE_JUNCTION_STATE( name,id,action,target )				\
																		\
								rkhrom RKHSJUNC_T name =				\
								{										\
									mkbase(RKH_JUNCTION,id,name),		\
									action,	target 						\
								}


/**
 *	This macro creates a deep history pseudostate. Deep history 
 *	applies downwards to all levels of nesting.
 *
 *	\note
 *
 *	See RKHSHIST_T structure definition for more information.
 *
 *	Arguments:
 *
 * 	\param name		pseudostate name. Represents a deep history 
 * 					pseudostate structure.
 * 	\param id		the value of state ID.	
 * 	\param parent	pointer to parent state.
 */

#define RKH_CREATE_DEEP_HISTORY_STATE( name,id,parent )					\
																		\
								static rkhrom RKHSREG_T *ram##name;		\
																		\
								rkhrom RKHSHIST_T name =				\
								{										\
									mkbase(RKH_DHISTORY,id,name),		\
									parent,&ram##name 					\
								}


/**
 *	This macro creates a shallow history pseudostate. Shallow 
 *	history means that history applies to the current nesting context 
 *	only � states nested more deeply are not affected by the 
 *	presence of a history pseudostates in a higher context.
 *	
 *	\note
 *
 *	See RKHSHIST_T structure definition for more information.
 *
 * 	\param name		pseudostate name. Represents a shallow history pseudostate 
 * 					structure.
 * 	\param id		the value of state ID.	
 * 	\param parent	pointer to parent state.
 */

#define RKH_CREATE_SHALLOW_HISTORY_STATE( name,id,parent )				\
																		\
								static rkhrom RKHSREG_T *ram##name;		\
																		\
								rkhrom RKHSHIST_T name =				\
								{										\
									mkbase(RKH_SHISTORY,id,name),		\
									parent,&ram##name 					\
								}


/**
 *	This macro creates a state transition table. Use the 
 *	'RKH_END_TRANS_TABLE' macro to terminate the transition table.
 *
 * 	\param name		state name.
 */

#define RKH_CREATE_TRANS_TABLE( name )									\
																		\
								static rkhrom RKHTR_T name##_trtbl[]={


/**
 *	This macro defines a regular state transition.
 *
 *	\note
 *
 *	See RKHTR_T structure definition for more information.
 *
 * 	\param e		triggering event.
 * 	\param g		pointer to guard function. This argument is 
 *					optional, thus it could be declared as NULL.
 * 	\param a		pointer to action function. This argument is 
 *					optional, thus it could be declared as NULL.
 * 	\param t		pointer to target state.
 */

#define RKH_TRREG( e, g, a, t )	{ e, g, a, t }


/**
 *	This macro defines an internal state transition.
 *
 *	\note
 *
 *	See RKHTR_T structure definition for more information.
 *
 * 	\param e		triggering event.
 * 	\param g		pointer to guard function.	
 * 	\param a		pointer to action function.
 */

#define RKH_TRINT( e, g, a )	{ e, g, a, NULL }


/*
 * 	This macro is internal to RKH and the user application should 
 * 	not call it.
 */

#define RKH_ETRTBL				{ RKH_ANY, NULL, NULL, NULL }


/**
 *	This macro is used to terminate a state transition table.
 */

#define RKH_END_TRANS_TABLE		RKH_ETRTBL};


/**
 *	This macro creates a branch table. Use the 
 *	'RKH_END_BRANCH_TABLE' macro to terminate the branch table.
 *	Use rkh_else when if all the guards on the other branches are false.
 *
 * 	\param name		conditional pseudostate name.
 */

#define RKH_CREATE_BRANCH_TABLE( name )								\
																	\
								RKH_CREATE_TRANS_TABLE( name )


/**
 *	This macro defines a branch in the branch table. Each condition
 *	connector can have one special branch with a guard labeled rkh_else, 
 *	which is taken if all the guards on the other branches are false.
 *	
 *
 *	\note
 *
 *	See RKHTR_T structure definition for more information.
 *
 * 	\param g		branch guard. Branches are labeled with guards that 
 * 					determine which one is to be actually taken. Use rkh_else
 * 					when if all the guards on the other branches are false.
 * 	\param a		pointer to action function. This argument is 
 *					optional, thus it could be declared as NULL.
 * 	\param t		pointer to target state.
 */

#define RKH_BRANCH( g, a, t )	{ 0, g, a, t }


/*
 * 	This macro is internal to RKH and the user application should 
 * 	not call it.
 */

#define RKH_EBTBL				{ RKH_ANY, NULL, NULL, NULL }


/**
 *	This macro is used to terminate a state transition table.
 */

#define RKH_END_BRANCH_TABLE	RKH_EBTBL};


/**
 *	This macro indicates the root state of a HSM.
 */

#define RKH_ROOT				NULL


/**
 * 	This macro declares a previously created HSM to be used 
 * 	as a global object. Generally, the HSMs are declared in HSM 
 * 	dependent header file.
 *
 * 	\param h		HSM name.
 */

#define RKH_DCLR_HSM( h )		extern RKH_T h


/**@{
 *
 * 	Declares a previously created state/pseudostate to be used 
 * 	as a global object.  
 *
 * 	\note
 *
 * 	Generally, the states are declared in HSM dependent header file.
 */

#define RKH_DCLR_COMP_STATE		extern rkhrom RKHSREG_T
#define RKH_DCLR_BASIC_STATE	extern rkhrom RKHSREG_T
#define RKH_DCLR_COND_STATE		extern rkhrom RKHSCOND_T
#define RKH_DCLR_JUNC_STATE		extern rkhrom RKHSJUNC_T
#define RKH_DCLR_DHIST_STATE	extern rkhrom RKHSHIST_T 
#define RKH_DCLR_SHIST_STATE	extern rkhrom RKHSHIST_T

/*@}*/


/**
 * 	This values are retrived by guard functions. A guard is a boolean 
 * 	condition that returns a TRUE or FALSE value that controls
 * 	whether or not a transition is taken following the receipt of 
 * 	a triggering event.
 */

enum
{
	RKH_GFALSE,	/**< False condition */
	RKH_GTRUE	/**< True condition */
};


/**
 *	Posts an event directly to the event queue \a qd using the LIFO policy.
 *
 * 	\note 
 *
 *	For memory efficiency and best performance the deferred event queue, 
 *	STORE ONLY POINTERS to events, not the whole event objects.
 *
 * 	The LIFO policy should be used only with great caution because it
 * 	alters order of events in the queue.
 *	At this time, this functions are required only when the user 
 *	application is used dynamic event (RKH_EN_DYNAMIC_EVENT == 1).
 *
 * 	\param qd		event queue descriptor.
 * 	\param evt		pointer to event.
 *
 * 	\returns
 *
 * 	Zero (0) if the event was successfully inserted, 
 *	otherwise error code. 	
 */

HUInt rkh_put_fifo( HUInt qd, RKHEVT_T *evt );


/**
 *	Posts an event directly to the event queue \a qd using the FIFO policy.
 *
 * 	\note
 *
 *	For memory efficiency and best performance the deferred event queue, 
 *	STORE ONLY POINTERS to events, not the whole event objects.
 *
 *	At this time, this functions are required only when the user 
 *	application is used dynamic event (RKH_EN_DYNAMIC_EVENT == 1).
 *
 * 	\param qd		event queue descriptor.
 * 	\param evt		pointer to event.
 *
 * 	\returns
 *
 * 	Zero (0) if the event was successfully inserted, 
 *	otherwise error code. 	
 */

HUInt rkh_put_lifo( HUInt qd, RKHEVT_T *evt );


/**
 *	Defer an event to a given separate event queue.
 *
 * 	This function is part of the event deferral support. An active object
 * 	uses this function to defer an event \a evt to the event queue \a qd. 
 * 	RKH correctly accounts for another outstanding reference to the event 
 * 	and will not recycle the event at the end of the RTC step. 
 * 	Later, the active object might recall one event at a time from the 
 * 	event queue.
 *	
 *	\note
 *
 *	For memory efficiency and best performance the deferred event queue, 
 *	STORE ONLY POINTERS to events, not the whole event objects.
 *  An active object can use multiple event queues to defer events of
 *  different kinds.
 *
 * 	\param qd		event queue descriptor.
 * 	\param evt		pointer to event.
 *
 * 	\returns
 *
 * 	Zero (0) if the event was successfully deferred, 
 *	otherwise error code. 	
 */

HUInt rkh_defer( HUInt qd, RKHEVT_T *evt );


/**
 * 	Recall a deferred event from a given event queue.
 *
 * 	This function is part of the event deferral support. An active object
 * 	uses this function to recall a deferred event from a given event queue. 
 * 	Recalling an event means that it is removed from the deferred event 
 * 	queue \a qds and posted (LIFO) to the event queue of the active object
 * 	\a qdd.
 *
 * 	\note
 *
 *	For memory efficiency and best performance the deferred event queue, 
 *	STORE ONLY POINTERS to events, not the whole event objects.
 *
 * 	\returns 
 *
 * 	The pointer to the recalled event to the caller, or NULL if no 
 * 	event has been recalled.
 */

RKHEVT_T *rkh_recall( HUInt qdd, HUInt qds );


/**
 *	This macro dynamically creates a new event of type 'et' with the signal.
 *	The basic policy is to allocate the event from the first pool that has 
 *	a block size big enough to fit the requested event size.
 *	RKH can manage up to three event pools (e.g., small, medium, and 
 *	large events, like shirt sizes). Thus, ensure the 
 *
 *	It returns a pointer to the event already cast to the event type 
 *	(et*). Here is an example of dynamic event allocation with the macro 
 *	rkh_alloc_event():
 *
 *	\code
 *	MYEVT_T *mye = rkh_alloc_event( MYEVT_T, DATA );
 *	mye->y = mye->x = 0;
 *	...
 *	\endcode
 *
 * 	\note
 *	The assertions inside rkh_ae() function guarantee that the pointer is 
 *	valid, so you don't need to check the pointer returned from rkh_ae(), 
 *	unlike the value returned from malloc(), which you should check.
 *
 * 	\param et		type of event.
 * 	\param e		event signal.
 */

#define rkh_alloc_event( et, e )										\
																		\
								(et*)rkh_ae(sizeof(et),(RKHE_T)(e))


/**
 * 	Recycle a dynamic event.
 *
 * 	This function implements a simple garbage collector for the dynamic 
 * 	events.	Only dynamic events are candidates for recycling. (A dynamic 
 * 	event is one that is allocated from an event pool, which is determined 
 * 	as non-zero	e->dynamic_ attribute.) Next, the function decrements the 
 * 	reference counter of the event, and recycles the event only if the 
 * 	counter drops to zero (meaning that no more references are outstanding 
 * 	for this event). The dynamic event is recycled by returning it to the 
 * 	pool from which	it was originally allocated. The pool-of-origin 
 * 	information is stored in the upper 2-MSBs of the e->dynamic_ attribute 
 * 	[MS]. Note that the data member dynamic_ of a dynamic event cannot be 
 * 	zero because the two most significant bits of the byte hold the pool 
 * 	ID, with valid values of 1, 2, or 3.
 * 	
 * 	\note 
 *
 * 	The garbage collector must be explicitly invoked at all appropriate 
 * 	contexts, when an event can become garbage (automatic garbage collection).
 *
 * 	\param evt		pointer to event to be potentially recycled.
 */

void rkh_gc( RKHEVT_T *evt );


/**
 *	This macro set the event 'evt' with 'e' signal and establishes it as one 
 *	static event.
 *
 * 	\param evt		pointer to event structure derived from RKHEVT_T.
 * 	\param es		event signal. The RKH takes this value for triggering 
 * 					a state transition.
 *
 * 	\returns
 */

#define rkh_set_static_event( evt, es )			mksevt( evt, es )


/**
 * 	Inits a previously created HSM calling its initializing function.
 *
 * 	\param ph		pointer to HSM control block. Represents a previously 
 * 					created HSM structure.
 */

void rkh_init_hsm( RKH_T *ph );


/**
 *	Executes a HSM in a non-preemtive model. In this model, before the 
 *	system handles a new event it can store it until the previous event 
 *	has completed processing. This model is called run to completion or 
 *	RTC. Thus, the system processes events in discrete, indivisible RTC 
 *	steps.
 *
 * 	\param ph		pointer to HSM control block. Represents a previously 
 * 					created HSM structure.
 *	\param pevt		pointer to arrived event. It's used as HSM's input 
 *					alphabet.
 *
 *	\return
 *
 *	Result RKH_RCODE_T code.
 *	
 */

HUInt rkh_engine( RKH_T *ph, RKHEVT_T *pevt );


/**
 * 	This macro retrieves the state ID of HSM control block.
 *
 * 	\param ph 		pointer to HSM control block. Represents a previously 
 * 					created HSM structure.
 *
 * 	\return
 *
 * 	Id of current state.
 */

#define rkh_get_cstate_id( ph )											\
																		\
								((RKHBASE_T*)(ph->state))->id	


/**	
 * 	This macro retrieves the current state name of HSM control block.
 *
 * 	\param ph 		pointer to HSM control block. Represents a previously 
 * 					created HSM structure.
 *
 * 	\returns
 *
 * 	Name of current state.
 */

#define rkh_get_cstate_name( ph )										\
																		\
								((RKHBASE_T*)(ph->state))->name	

/**	
 * 	This macro retrieves the HSM's data.
 *
 * 	\param ph 		pointer to HSM control block. Represents a previously 
 * 					created HSM structure.
 *
 * 	\returns
 *
 * 	Pointer to state-machine's data.
 */

#define rkh_get_data( ph )												\
																		\
								((ph)->hdata)	


/**	
 * 	This macro retrieves the state's abstract data.
 *	Aditionally, by means of single inheritance in C it could be used 
 *	as state's abstract data. Aditionally, implementing the single 
 *	inheritance in C is very simply by literally embedding the base type, 
 *	RKHPPRO_T in this case, as the first member of the derived structure. 
 *	See member "prepro" of RKHSREG_T structure for more information.
 *
 * 	\param ph 		pointer to HSM control block. Represents a previously 
 * 					created HSM structure.
 *
 * 	\returns
 *
 * 	Pointer to state's abstract data.
 */

#define rkh_get_sdata( ph )												\
																		\
								((ph)->state->sdata)	


/**
 * 	Erase the history of a state. It can be a shallow or deep history.
 *
 * 	\param h 		pointer to history pseudostate.
 */

void rkh_clear_history( rkhrom RKHSHIST_T *h );


/**
 * 	Clear performance information for a particular HSM.
 *
 * 	Information is available during run-time for each HSM. This 
 * 	information can be useful in determining whether
 * 	the application is performing properly, as well as helping to
 * 	optimize the application.
 *
 *	\note
 *
 *	See RKH_INFO_T structure definition for more information.
 *
 * 	\param ph 		pointer to HSM control block. Represents a previously 
 * 					created HSM structure.
 */

void rkh_clear_info( RKH_T *ph );


/**
 * 	Retrieves performance information for a particular HSM.
 *
 *	Note:
 *
 *	See RKH_INFO_T structure definition for more information.
 *
 * 	\param ph 		pointer to HSM control block. Represents a previously 
 * 					created HSM structure.
 *	
 * 	\return
 *
 * 	A pointer to performance information structure.
 */

RKH_INFO_T *rkh_get_info( RKH_T *ph );


#endif
