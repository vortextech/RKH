/* ------------------------------ File header ------------------------------ */
/*  Use file_header.txt file                                                 */

/* -------------------------- Development history -------------------------- */
/*
 *  2020.06.21  CaMa  v1.0.0  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  CaMa  Carlos Mancón  manconci@gmail.com
 */

/* --------------------------------- Notes --------------------------------- */
/*
 *  (1) These notes could be referenced by number.
 *      (1.1) Sub-note.
 *  (2) Another note.
 */

/* --------------------------------- Module -------------------------------- */
#ifndef __BOARD_EXT_H__
#define __BOARD_EXT_H__

/* ----------------------------- Include files ----------------------------- */
/* (1) #include <string.h>                                                   */
/* (2) #include <other system headers>                                       */
/* (3) #include "user header files"                                          */

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* (1) Function macro defines                                                */

/* -------------------------------- Constants ------------------------------ */
/* (1) Macro defines                                                         */
/* (2) Enumerations                                                          */
/* (3) Extern declaration of const objects defined in other files            */

/* ------------------------------- Data types ------------------------------ */
/* (1) typedefs                                                              */
typedef struct {
	uint8_t port;
	uint8_t pin;
} io_port_t;

#ifdef CIAA_NXP
	static const io_port_t gpioLEDBits[] = {{5,12},{5,13},{5,14},{1,8}};
	typedef enum
	{
		LED0,
		LED1,
		LED2,
		LED3
	} LEDS;

	enum
	{
		SW1_SWITCH,
		SW2_SWITCH,
		SW3_SWITCH,
		SW4_SWITCH,
		SW5_SWITCH,
		SW6_SWITCH,
		SW7_SWITCH,
		SW8_SWITCH,
		SWITCHS_NUM
	};

	static const io_port_t gpioBtnBits[] = {{2, 0},{2, 1},{2, 2},{2, 3},
											{3,11},{3,12},{3,13},{3,13}};

#define	NO_BUTTON_PRESSED	0b0
#define	DIN0_LOW			0b1
#define	DIN1_LOW			0b10
#define	DIN2_LOW			0b100
#define	DIN3_LOW			0b1000
#define	DIN4_LOW			0b10000
#define	DIN5_LOW			0b100000
#define	DIN6_LOW			0b1000000
#define	DIN7_LOW			0b10000000

#else
	static const io_port_t gpioLEDBits[] = {{5,0},{5,1},{5,2},{0,14},{1,11},{1,12}};
	typedef enum
		{
			LEDR,
			LEDG,
			LED0,
			LED1,
			LED2,
			LED3
		} LEDS;

		enum
		{
			SW1_SWITCH,
			SW2_SWITCH,
			SW3_SWITCH,
			SW4_SWITCH,
			SWITCHS_NUM
		};

	static const io_port_t gpioBtnBits[] = {{0,4},{0,8},{0,9},{1,9}};

#define	NO_BUTTON_PRESSED	0b0
#define	TEC1_PRESSED		0b0001
#define	TEC2_PRESSED		0b0010
#define	TEC3_PRESSED		0b0100
#define	TEC4_PRESSED		0b1000

#endif

/* -------------------------- External variables --------------------------- */
/* (1) Extern declarations of variables defined in other files               */

/* -------------------------- Function prototypes -------------------------- */
/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ File footer ------------------------------ */
/*  Use file_footer.txt file                                                 */
