/**
 * \cond
 *  --------------------------------------------------------------------------
 *
 *                                Framework RKH
 *                                -------------
 *
 * 	          State-machine framework for reactive embedded systems            
 * 	        
 * 	                    Copyright (C) 2010 Leandro Francucci.
 * 	        All rights reserved. Protected by international copyright laws.
 *
 *
 * 	RKH is free software: you can redistribute it and/or modify it under the 
 * 	terms of the GNU General Public License as published by the Free Software 
 * 	Foundation, either version 3 of the License, or (at your option) any 
 * 	later version.
 *
 *  RKH is distributed in the hope that it will be useful, but WITHOUT ANY 
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for 
 *  more details.
 *
 *  You should have received a copy of the GNU General Public License along 
 *  with RKH, see copying.txt file.
 *
 * 	Contact information:
 * 	RKH web site:	http://sourceforge.net/projects/rkh-reactivesys/
 * 	e-mail:			francuccilea@gmail.com
 *
 *  --------------------------------------------------------------------------
 *  File                     : rkhtrc.c
 *	Last updated for version : v2.4.04
 *	By                       : LF
 *  --------------------------------------------------------------------------
 *  \endcond
 *
 * 	\file
 * 	\ingroup 	trc
 *
 * 	\brief 		Implements the RKH trace facility.
 */


#include "rkhassert.h"
#include "rkhtrc.h"
#include "rkh.h"
#include "rkhrdy.h"


#if RKH_TRC_EN == RKH_ENABLED

/* This macro is needed only if the module requires to check 	... */
/* ... expressions that ought to be true as long as the program ... */
/* ... is running. 													*/

/* RKH_MODULE_NAME( rkhtrc ) */

#define GETGRP( e )				(rkhui8_t)(((e) & 0xE0) >> 5)
#define GETEVT( e )				(rkhui8_t)((e) & 0x1F)


#if RKH_TRC_RUNTIME_FILTER == RKH_ENABLED

/**
 * 	\brief
 * 	Filter table of trace events.
 *
 * 	The trace filter management is similar to the native priority scheme.
 * 	In this case, each trace event is assigned a unique number 
 * 	(#RKH_TRC_EVENTS). When a event is ready to record a trace its 
 * 	corresponding bit in the filter table must be clear. The size of 
 * 	trceftbl[] depends on #RKH_TOT_NUM_TRC_EVTS.
 *
 * 	Trace event number = | 0 | Y | Y | Y | Y | X | X | X |\n
 *
 * 	Y's:	index into trceftbl[ #RKH_TRC_MAX_EVENTS_IN_BYTES ] table.\n
 * 	X's:	bit position in trceftbl[ Y's ].\n
 *
 * 	The lower 3 bits (X's) of the trace event number are used to determine 
 * 	the bit position in trceftbl[], while the next four most significant bits 
 * 	(Y's) are used to determine the index into trceftbl[].
 */

static rkhui8_t trceftbl[ RKH_TRC_MAX_EVENTS_IN_BYTES ];

/**
 * 	\brief
 * 	Filter table of trace groups.
 *
 * 	Each bit in #trcgfilter is used to indicate whenever any trace group 
 * 	is filtered out its events. See #RKH_TRC_GROUPS.
 *
 * 	\code
 *  bit position =   7   6   5   4   3   2   1   0   -- Groups   
 * 	trcgfilter   = | Y | Y | Y | Y | Y | Y | Y | Y |
 * 				   		     |		   	     |   |___ RKH_TG_MP
 *						     |			     |_______ RKH_TG_RQ
 * 						     |				  		  ...
 * 				             |_______________________ RKH_TG_FWK
 *	\endcode
 */

static rkhui8_t trcgfilter;

/**
 * 	\brief
 * 	Filter table of trace points associated with the SMA (AO).
 *
 * 	The trace filter management is similar to the native priority scheme.
 * 	In this case, each SMA is assigned a unique priority number. When a SMA 
 * 	is ready to record a trace its corresponding bit in the filter table 
 * 	must be clear. The size of #trcsmaftbl[] depends on 
 * 	#RKH_MAX_SMA (see rkhcfg.h).
 *
 * 	SMA priority number = | Y | Y | Y | Y | Y | X | X | X |\n
 *
 * 	Y's:	index into trcsmaftbl[ #RKH_TRC_MAX_SMA ] table.\n
 * 	X's:	bit position in trcsmaftbl[ Y's ].\n
 *
 * 	The lower 3 bits (X's) of the SMA priority number are used to determine 
 * 	the bit position in trcsmaftbl[], while the next five most significant bits 
 * 	(Y's) are used to determine the index into trcsmaftbl[].
 */

static rkhui8_t trcsmaftbl[ RKH_TRC_MAX_SMA ];

/**
 * 	\brief
 * 	Filter table of trace points associated with the event signals.
 *	Similar to trcsmaftbl[].
 *
 * 	Signal number = | Y | ... | Y | Y | Y | X | X | X |\n
 *
 * 	Y's:	index into trcsigftbl[ #RKH_TRC_MAX_SIGNALS ] table.\n
 * 	X's:	bit position in trcsigftbl[ Y's ].\n
 *
 * 	The lower 3 bits (X's) of the event signal are used to determine the bit 
 * 	position in trcsigftbl[], while the next most significant bits (Y's) are 
 * 	used to determine the index into trcsigftbl[].
 */

static rkhui8_t trcsigftbl[ RKH_TRC_MAX_SIGNALS ];

/**
 * 	\brief
 * 	The tables to filter trace events related to signal and active objects.
 */

const FIL_T fsig = { RKH_TRC_MAX_SIGNALS, 	trcsigftbl };
const FIL_T fsma = { RKH_TRC_MAX_SMA, 		trcsmaftbl };

/** Map (group << 4) + event to event index in trceftbl[] table. */
static RKHROM rkhui8_t trcgmtbl[] =
{
	/*	<offset>					| <range> [in bytes] */
	/*              				  1 byte -> 8 events */
	((RKH_MP_TTBL_OFFSET 	<< 4) 	| RKH_MP_TTBL_RANGE		),
	((RKH_RQ_TTBL_OFFSET 	<< 4) 	| RKH_RQ_TTBL_RANGE		),
	((RKH_SMA_TTBL_OFFSET 	<< 4) 	| RKH_SMA_TTBL_RANGE	),
	((RKH_SM_TTBL_OFFSET 	<< 4)	| RKH_SM_TTBL_RANGE		),
	((RKH_TIM_TTBL_OFFSET 	<< 4) 	| RKH_TIM_TTBL_RANGE	),
	((RKH_FWK_TTBL_OFFSET 	<< 4) 	| RKH_FWK_TTBL_RANGE	),
	((RKH_USR_TTBL_OFFSET 	<< 4) 	| RKH_USR_TTBL_RANGE	)
};
#endif

static RKH_TE_T trcstm[ RKH_TRC_SIZEOF_STREAM ];
static RKH_TE_T *trcin, *trcout, *trcend;
static rkhui8_t chk;
static rkhui8_t nseq;
static TRCQTY_T trcqty;


void
rkh_trc_init( void )
{
	trcin = trcout = trcstm;
	trcqty = 0;
	nseq = 0;
	trcend = &trcstm[ RKH_TRC_SIZEOF_STREAM ];
	RKH_TRC_U8_RAW( RKH_FLG );
}


void
rkh_trc_put( rkhui8_t b )
{
	*trcin++ = b;
	++trcqty;

	if( trcin == trcend )
		trcin = trcstm;

	if( trcqty >= RKH_TRC_SIZEOF_STREAM )
	{
		trcqty = RKH_TRC_SIZEOF_STREAM;
		trcout = trcin;
	}
}


rkhui8_t *
rkh_trc_get( void )
{
	rkhui8_t *tre = ( rkhui8_t* )0;

	if( trcqty == 0 )
		return tre;

	tre = trcout++;
	--trcqty;

	if( trcout >= trcend )
		trcout = trcstm;

	return tre;
}


rkhui8_t *
rkh_trc_get_block( TRCQTY_T *nget )
{
	rkhui8_t *tre = (rkhui8_t *)0;
	TRCQTY_T n;

	if( trcqty == (TRCQTY_T)0 )
	{
		*nget = (TRCQTY_T)0;
		return tre;
	}

	tre = trcout;

	/* Calculates the number of bytes to be retrieved */
	n = (TRCQTY_T)(trcend - trcout);	/* bytes until the end */
	if( n > trcqty )
		n = trcqty;
	if( n > *nget )
		n = *nget;

	*nget = n;
	trcout += n;
	trcqty -= n;

	if( trcout >= trcend )
		trcout = trcstm;

	return tre;
}


#if RKH_TRC_RUNTIME_FILTER == RKH_ENABLED
HUInt
rkh_trc_isoff_( rkhui8_t e )
{
	rkhui8_t evt, grp;

	evt = GETEVT( e );
	grp = GETGRP( e );

	return 	(( trcgfilter & rkh_maptbl[ grp ] ) != 0 ) &&
			(( trceftbl[(rkhui8_t)((trcgmtbl[ grp ] >> 4) + (evt >> 3))] & 
			   rkh_maptbl[evt & 0x7]) != 0 );
}


void 
rkh_trc_filter_group_( rkhui8_t ctrl, rkhui8_t grp, rkhui8_t mode )
{
	rkhui8_t *p, ix, c, offset, range;

	if( grp == RKH_TRC_ALL_GROUPS )
	{
		trcgfilter = (rkhui8_t)((ctrl == FILTER_OFF) ? 0xFF : 0);
		return;
	}

	if( ctrl == FILTER_OFF )
		trcgfilter |= rkh_maptbl[ grp ];
	else
		trcgfilter &= ~rkh_maptbl[ grp ];

	if( mode == ECHANGE )
	{
		offset = (rkhui8_t)(trcgmtbl[ grp ] >> 4);
		range = (rkhui8_t)(trcgmtbl[ grp ] & 0x0F);
		for( 	p = &trceftbl[ offset ], 
				ix = 0, 
				c = (rkhui8_t)((ctrl == FILTER_OFF) ? 0xFF : 0); 
				ix < range; ++ix, ++p )
			*p = c;
	}
}


void 
rkh_trc_filter_event_( rkhui8_t ctrl, rkhui8_t evt )
{
	rkhui8_t *p, ix, c, grp, e, offset;

	if( evt == RKH_TRC_ALL_EVENTS )
	{
		for( 	p = trceftbl, 
				ix = 0, 
				c = (rkhui8_t)((ctrl == FILTER_OFF) ? 0xFF : 0); 
				ix < RKH_TRC_MAX_EVENTS_IN_BYTES; ++ix, ++p )
			*p = c;
		trcgfilter = (rkhui8_t)((ctrl == FILTER_OFF) ? 0xFF : 0);
		return;
	}

	e = GETEVT( evt );
	grp = GETGRP( evt );
	offset = (rkhui8_t)((trcgmtbl[ grp ] >> 4) + (e >> 3));

	if( ctrl == FILTER_OFF )
	{
		trceftbl[offset] |= rkh_maptbl[e & 0x7];
		trcgfilter |= rkh_maptbl[ grp ];
	}
	else
		trceftbl[offset] &= ~rkh_maptbl[e & 0x7];
}


HUInt
rkh_trc_simfil_isoff( const FIL_T *filter, TRCFS_T slot )
{
	rkhui8_t x, y;

	y = (rkhui8_t)(slot >> 3);
	x = (rkhui8_t)(slot & 0x07);

	return (*(filter->tbl + y) & rkh_maptbl[x]) != 0;
}


void 
rkh_trc_simfil( const FIL_T *filter, TRCFS_T slot, rkhui8_t mode )
{
	rkhui8_t x, y, onoff, *ft, c;
	TRCFS_T ix;

	onoff = mode & RKH_FILTER_MODE_MASK;
	if( mode & RKH_TRC_ALL_FILTERS )
	{
		for( 	ft = (rkhui8_t *)(filter->tbl), 
				ix = (TRCFS_T)0, 
				c = (rkhui8_t)((onoff == FILTER_OFF) ? 0xFF : 0);
				ix < filter->size; ++ix, ++ft )
			*ft = c;
		return;
	}

	y = (rkhui8_t)(slot >> 3);
	x = (rkhui8_t)(slot & 0x07);

	if( onoff == FILTER_OFF )
		*(filter->tbl + y) |= rkh_maptbl[ x ];
	else
		*(filter->tbl + y) &= ~rkh_maptbl[ x ];
}
#endif


void
rkh_trc_begin( rkhui8_t eid )
{
	RKH_TRC_HDR( eid );
}


void
rkh_trc_end( void )
{
	RKH_TRC_CHK();
	RKH_TRC_FLG();
}


void
rkh_trc_clear_chk( void )
{
	chk = 0;
}


void 
rkh_trc_u8( rkhui8_t d )
{
	chk = (rkhui8_t)( chk + d );
	if( d == RKH_FLG || d == RKH_ESC )
	{
		rkh_trc_put( RKH_ESC );
		rkh_trc_put( (rkhui8_t)(d ^ RKH_XOR) );
	}
	else
		rkh_trc_put( d );
}


void 
rkh_trc_u16( rkhui16_t d )
{
	rkh_trc_u8( (rkhui8_t)d );
	d >>= 8;
	rkh_trc_u8( (rkhui8_t)d );
}


void 
rkh_trc_u32( rkhui32_t d )
{
	rkh_trc_u8( (rkhui8_t)d );
	d >>= 8;
	rkh_trc_u8( (rkhui8_t)d );
	d >>= 8;
	rkh_trc_u8( (rkhui8_t)d );
	d >>= 8;
	rkh_trc_u8( (rkhui8_t)d );
}


void 
rkh_trc_str( const char *s )
{
	while( *s != '\0' )
		rkh_trc_u8( (rkhui8_t)*s++ );
	rkh_trc_u8( '\0' );
}


void
rkh_trc_obj( rkhui8_t tre, rkhui8_t *obj, const char *obj_name )
{
	RKH_TRC_BEGIN_WOFIL( tre )
		RKH_TRC_SYM( obj );
		RKH_TRC_STR( obj_name );
	RKH_TRC_END_WOFIL()
	RKH_TRC_FLUSH();
}


void
rkh_trc_sig( RKHE_T sig, const char *sig_name )
{
	RKH_TRC_BEGIN_WOFIL( RKH_TE_FWK_SIG )
		RKH_TRC_SIG( sig );
		RKH_TRC_STR( sig_name );
	RKH_TRC_END_WOFIL()
	RKH_TRC_FLUSH();
}


void
rkh_trc_ao( struct rkhsma_t *ao )
{
	RKH_TRC_BEGIN_WOFIL( RKH_TE_FWK_AO )
		RKH_TRC_SYM( ao );
		RKH_TRC_STR( ao->romrkh->name );
	RKH_TRC_END_WOFIL()
	RKH_TRC_FLUSH();
}


void
rkh_trc_state( struct rkhsma_t *ao, rkhui8_t *state )
{
	RKH_TRC_BEGIN_WOFIL( RKH_TE_FWK_STATE )
		RKH_TRC_SYM( ao );
		RKH_TRC_SYM( state );
		RKH_TRC_STR( ((struct rkhbase_t *)state)->name );
	RKH_TRC_END_WOFIL()
	RKH_TRC_FLUSH();
}


#if RKH_TRC_EN_USER_TRACE == RKH_ENABLED
void 
rkh_trc_fmt_u8( rkhui8_t fmt, rkhui8_t d )
{
	rkh_trc_u8( fmt );
	rkh_trc_u8( d );
}


void 
rkh_trc_fmt_u16( rkhui8_t fmt, rkhui16_t d )
{
	rkh_trc_u8( fmt );
	rkh_trc_u16( d );
}


void 
rkh_trc_fmt_u32( rkhui8_t fmt, rkhui32_t d )
{
	rkh_trc_u8( fmt );
	rkh_trc_u32( d );
}


void 
rkh_trc_fmt_str( const char *s )
{
	rkh_trc_u8( (rkhui8_t)RKH_STR_T );
	while( *s != '\0' )
		rkh_trc_u8( (rkhui8_t)*s++ );
	rkh_trc_u8( '\0' );
}


void 
rkh_trc_fmt_mem( const rkhui8_t *mem, rkhui8_t size )
{
	rkh_trc_u8( (rkhui8_t)RKH_MEM_T );
	rkh_trc_u8( size );
    while( size != 0 )
	{
		rkh_trc_u8( (rkhui8_t)*mem++ );
        --size;
    }	
}
#endif


#endif
