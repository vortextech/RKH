/*
 * switch.c
 *
 * Minimal debouncing switch handler.
 * hardware access.
 *
 */

#include "rkh.h"
#include "switch.h"
#include "bsp.h"
#include "shared.h"
#include "board.h"
#include "board_ext.h"


static RKH_ROM_STATIC_EVENT( e_pause, PAUSE );
static SWITCH_ST switchs[SWITCHS_NUM];

void bsp_publish( const RKH_EVT_T *e );

#define SWITCH_EVT(s__, st__)					\
			do {								\
				if((st__) == SW_RELEASED)		\
					return;						\
				if((s__) == SW1_SWITCH)			\
					bsp_publish( &e_pause );	\
			} while(0)

void
switch_init ( void )
{
	uint32_t i;
	for (i = 0; i < SWITCHS_NUM; i++)
	{
		switchs[i].state = 0;
		switchs[i].debsw = SW_RELEASED;
	}
}
			
void
switch_tick( void )
{
	SWITCH_ST *p = switchs;
	uint32_t s;

	uint32_t btnStatus = Buttons_GetStatus();

	for (s = 0; s < SWITCHS_NUM; ++p, ++s)
	{
		p->state = (p->state<<1) | ((btnStatus >> s) & 0x01);
		if( (p->state == 0xFF) && (p->debsw != SW_PRESSED) )
		{
			p->debsw = SW_PRESSED;
			SWITCH_EVT( s, p->debsw );
		}
		else if( ( p->state == 0x00 ) && (p->debsw != SW_RELEASED) )
		{
			p->debsw = SW_RELEASED;
			SWITCH_EVT( s, p->debsw );
		}
	}
}


uint8_t
get_switch_state( uint32_t who )
{
	return switchs[who].debsw;
}
