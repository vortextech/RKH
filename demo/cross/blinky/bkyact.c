/*
 *	bkyact.c
 */


#include "rkh.h"
#include "bky.h"
#include "bkyact.h"
#include "bsp.h"


#define CBKY( sm_ )		(( BKYSM_T* )(sm_))


/*
 * 	Declare and allocate the 'e_tout' event.
 * 	The 'e_tout' event with TIMEOUT signal never changes, so it can be 
 * 	statically allocated just once by means of RKH_DCLR_STATIC_EVENT() macro.
 */

static RKH_DCLR_STATIC_EVENT( e_tout, TIMEOUT );


/*
 * 	Declare and allocate the 'blktim' timer.
 */

static RKHT_T bkytim;


/*
 *	Defines HSM init function
 */

void 
blinky_init( const struct rkhsma_t *sma )
{
	rkh_tim_init( &bkytim, &e_tout, NULL );
	blinky_led_on( sma, NULL  );	
}


/*
 *	Defines transition action functions
 */

void
blinky_led_on( const struct rkhsma_t *sma, RKHEVT_T *pe )
{
	(void)pe;

	rkh_tim_oneshot( &bkytim, blinky, LED_ON_TIME );
	bsp_led_on();
	++CBKY(sma)->cnt;
}


void
blinky_led_off( const struct rkhsma_t *sma, RKHEVT_T *pe )
{
	(void)sma;
	(void)pe;

	rkh_tim_oneshot( &bkytim, blinky, LED_OFF_TIME );
	bsp_led_off();
}