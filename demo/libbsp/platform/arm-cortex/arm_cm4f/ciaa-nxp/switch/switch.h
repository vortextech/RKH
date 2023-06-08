/*
 * switch.h
 *
 * Minimal debouncing switch handler
 *
 */

#ifndef __SWITCH_H__
#define __SWITCH_H__

typedef struct
{
	unsigned char state;
	uint8_t debsw;
}SWITCH_ST;

enum
{
	SW_RELEASED,
	SW_PRESSED
};
void switch_init( void );
void switch_tick( void );
uint8_t get_switch_state( uint32_t who );

#endif
