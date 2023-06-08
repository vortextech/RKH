/* ------------------------------ File header ------------------------------ */
/*  Use file_header.txt file                                                 */

/* -------------------------- Development history -------------------------- */
/*
 *  2020.06.10  CaMa  v1.0.0  Initial version
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

/* ----------------------------- Include files ----------------------------- */
/* (1) #include <string.h>                                                   */
/* (2) #include <other system headers>                                       */
/* (3) #include �user header files�                                          */
#include "rkh.h"
#include "rkhport.h"
#include "bsp.h"
#include "server.h"
#include "client.h"
#include "shared.h"
#include "board.h"
#include "board_ext.h"
#include "switch.h"

/* ----------------------------- Local macros ------------------------------ */
/* (1) Function macro defines */

#ifdef DEBUG_SEMIHOSTING
#define PRINTF(...)	printf( __VA_ARGS__ )
#define PUTCHAR(ch)				putchar(ch)
#endif

#if SERIAL_TRACE == 1
	#define SERIAL_TRACE_OPEN()		bsp_uart_init()
	#define SERIAL_TRACE_CLOSE() 	bsp_uart_deinit()
	#define SERIAL_TRACE_SEND_BLOCK( buf_, len_ )	\
									bsp_uart_send_block( buf_, len_ )
#else
	#define SERIAL_TRACE_OPEN()						(void)0
	#define SERIAL_TRACE_CLOSE()					(void)0
	#define SERIAL_TRACE_SEND( d )					(void)0
	#define SERIAL_TRACE_SEND_BLOCK( buf_, len_ )	(void)0
#endif


/* ------------------------------- Constants ------------------------------- */
/* (1) Macro defines                                                         */
/* (2) Enumerations                                                          */
/* (3) Local const modifier                                                  */
/* (4) Global (extern or non-static external const modifier)                 */

#define UART_USB_LPC				LPC_USART2

#define SIZEOF_EP0STO				32
#define SIZEOF_EP0_BLOCK			sizeof( RKH_EVT_T )
#define SIZEOF_EP1STO				16
#define SIZEOF_EP1_BLOCK			sizeof( ReqEvt )
#define SVR_NAME					"Server    -"
#define CLI_NAME					"Client"

/* ---------------------------- Local data types --------------------------- */
/* (1) typedefs                                                              */
typedef struct
{
	bool led0_onState;
	bool led1_onState;
	bool led2_onState;
	bool led3_onState;
} LEDS_ST;


static LEDS_ST led_combinations[] = {	{ false, false, false, false },
										{ true, true, true, true },
										{ true, false, false, false },
										{ false, true, false, false },
										{ false, false, true, false },
										{ false, false, false, true }
									};

typedef enum
{
	NONE,
	ALL,
	ONLY_LED0,
	ONLY_LED1,
	ONLY_LED2,
	ONLY_LED3
} COMBINATION_IDX;

static COMBINATION_IDX clients_leds[] = {
										NONE,
										/*
										 * Client 1-4 according
										 * to clino arg
										 */
										ONLY_LED0,
										ONLY_LED1,
										ONLY_LED2,
										ONLY_LED3
};

/* ---------------------------- Global variables --------------------------- */
/* (1) Extern declarations of variables defined in other files.              */
/* (2) Non-static external definitions used in this file (and optionally     */
/*     in others if they are declared in those files using extern).          */

/* ---------------------------- Local variables ---------------------------- */
/* (1) Static external definitions used only in this file.                   */

#if defined( RKH_USE_TRC_SENDER )
static rui8_t l_isr_kbd;
#endif

#if RKH_CFG_TRC_EN == RKH_ENABLED
static RKH_TS_T tstamp;
#endif

static rui32_t l_rnd;				/* random seed */

static rui8_t ep0sto[ SIZEOF_EP0STO ], ep1sto[ SIZEOF_EP1STO ];

/* ----------------------- Local function prototypes ----------------------- */
void bsp_uart_init( void );
void bsp_uart_send_block(rui8_t *blk, TRCQTY_T nbytes );
void bsp_uart_deinit( void );

/* ---------------------------- Local functions ---------------------------- */

static
void
print_banner( void )
{
#ifdef PRINTF
	PRINTF(	"\"Shared\" example\n\n" );
	PRINTF(	"RKH version      = %s\n", RKH_RELEASE );
	PRINTF(	"Port version     = %s\n", rkhport_get_version() );
	PRINTF(	"Port description = %s\n\n", rkhport_get_desc() );
	PRINTF(	"Description: \n\n" );
	PRINTF(	"This application deals with the shared resource problem \n" );
	PRINTF(	"in active object systems. Showing one of the biggest \n" );
	PRINTF(	"benefit of using active objects: resource encapsulation. \n" );
	PRINTF(	"The encapsulation naturally designates the owner of the \n" );
	PRINTF(	"resource as the ultimate arbiter in resolving any contention \n" );
	PRINTF(	"and potential conflicts for the resource. \n" );
	PRINTF(	"The SHD application is relatively simple and can be tested \n" );
	PRINTF(	"only with a couple of LEDs on your target board. \n" );
	PRINTF(	"Still, SHD contains five (5) concurrent active objects \n" );
	PRINTF(	"that exchange events via direct event posting mechanism. \n" );
	PRINTF(	"The application uses four timers, as well as dynamic  \n" );
	PRINTF(	"and static events. \n" );
	PRINTF(	"On the other hand, this application could be used in either \n" );
	PRINTF(	"preemptive or cooperative environment. \n" );
	PRINTF(	"Additionally, the SHD could be used to verify a new RKH port. \n" );
	PRINTF(	"\n\n\n" );
#if BOARD==ciaa_nxp
	PRINTF( "1.- Each Client have its own LED/DOUT, Client 1-4:\n" );
	PRINTF( "	   	DOUT4, DOUT5, DOUT6, DOUT7\n" );
	PRINTF( "2.- Activate DIN0 to PAUSE.\n" );
	PRINTF( "3.- Paused state is shown with all LEDs/DOUTs activated.\n" );
#elif BOARD==edu_ciaa_nxp
	PRINTF( "1.- Each Client have its own LED, Client 1-4:\n" );
	PRINTF( "	   	LEDB, LED1, LED2, LED3\n" );
	PRINTF( "2.- Press TEC1 to PAUSE.\n" );
	PRINTF( "3.- Paused state is shown with all LEDs lit.\n" );
#endif
	PRINTF(	"\n\n" );
#endif
}

void
set_leds( COMBINATION_IDX idx )
{
	Board_LED_Set(LED0, led_combinations[idx].led0_onState);
	Board_LED_Set(LED1, led_combinations[idx].led1_onState);
	Board_LED_Set(LED2, led_combinations[idx].led2_onState);
	Board_LED_Set(LED3, led_combinations[idx].led3_onState);
}

/* ---------------------------- Global functions --------------------------- */

void
bsp_init(int argc, char *argv[])
{
	rInt cn;

	(void)argc;
	(void)argv;

	SystemCoreClockUpdate();

	Board_Init();
	switch_init();

	bsp_srand( 1234U );
	print_banner();
	rkh_fwk_init();

	RKH_FILTER_OFF_SMA( server );
	for( cn = 0; cn < NUM_CLIENTS; ++cn )
		RKH_FILTER_OFF_SMA( CLI(cn) );

	RKH_FILTER_OFF_EVENT( RKH_TE_SMA_FIFO );
	RKH_FILTER_OFF_EVENT( RKH_TE_SMA_LIFO );
	RKH_FILTER_OFF_EVENT( RKH_TE_SM_DCH );
	RKH_FILTER_OFF_EVENT( RKH_TE_SM_STATE );
	/*RKH_FILTER_OFF_ALL_SIGNALS();*/
	RKH_FILTER_OFF_SIGNAL( REQ );
	RKH_FILTER_OFF_SIGNAL( START );

}

void
bsp_keyParser(int c)
{

}

rui32_t
bsp_rand( void )
{
	/*
	 * A very cheap pseudo-random-number generator.
	 * "Super-Duper" Linear Congruential Generator (LCG)
	 * LCG(2^32, 3*7*11*13*23, 0, seed) [MS]
	 */
	l_rnd = l_rnd * (3*7*11*13*23);
	return l_rnd >> 8;
}

void
bsp_srand( rui32_t seed )
{
    l_rnd = seed;
}

void
bsp_cli_req( rui8_t clino )
{
#ifdef PRINTF
	PRINTF( "Client[%d] - Send request to server...\n", CLI_ID(clino) );
#endif
}

void
bsp_cli_wait_req( rui8_t clino, RKH_TNT_T req_time )
{
#ifdef PRINTF
	PRINTF( "Client[%d] - Waiting for send request to server (%d seg)\n",
									CLI_ID(clino), req_time );
#endif
}

void
bsp_cli_using( rui8_t clino, RKH_TNT_T using_time )
{
#ifdef PRINTF
	PRINTF( "Client[%d] - Using server for %d [seg]\n",
									CLI_ID(clino), using_time );
#endif
	set_leds(clients_leds[clino]);
}

void
bsp_cli_paused( rui8_t clino )
{
#ifdef PRINTF
	PRINTF( "Client[%d] - Paused\n", CLI_ID(clino) );
#endif
	set_leds(ALL);
}

void
bsp_cli_resumed( rui8_t clino )
{
#ifdef PRINTF
	PRINTF( "Client[%d] - Resumed\n", CLI_ID(clino) );
#endif
}

void
bsp_cli_done( rui8_t clino )
{
#ifdef PRINTF
	PRINTF( "Client[%d] - Done\n", CLI_ID(clino) );
#endif
	set_leds(NONE);
}

void
bsp_svr_recall( rui8_t clino )
{
#ifdef PRINTF
	PRINTF( "%s Recall a deferred request from client[%d]\n",
									SVR_NAME, CLI_ID(clino) );
#endif
}

void
bsp_svr_paused(rui32_t ntot, rui32_t *ncr)
{
#ifdef PRINTF
	rInt cn;
    rui32_t *pNcr;
	PRINTF( "Server paused | ");
	PRINTF( "ntot = %d |", ntot );

	for(pNcr = ncr, cn = 0; cn < NUM_CLIENTS; ++cn, ++pNcr )
		PRINTF( " cli%d=%d |", cn, *pNcr );

	PUTCHAR('\n');
#endif
}

void
bsp_svr_start( rui8_t clino )
{
	(void)0;
}

void
bsp_svr_end( void )
{

}

void
bsp_svr_resume( void )
{

}

#if RKH_CFG_TRC_EN == RKH_ENABLED

void
rkh_trc_open( void )
{
	rkh_trc_init();

	SERIAL_TRACE_OPEN();
	RKH_TRC_SEND_CFG( RKH_CFG_FWK_TICK_RATE_HZ );
}

void
rkh_trc_close( void )
{
	SERIAL_TRACE_CLOSE();
}

RKH_TS_T
rkh_trc_getts(void)
{
    return tstamp;
}


void
rkh_trc_flush( void )
{
	rui8_t *blk;
	TRCQTY_T nbytes;
	RKH_SR_ALLOC();

	FOREVER
	{
		nbytes = (TRCQTY_T)1024;

		RKH_ENTER_CRITICAL_();
		blk = rkh_trc_get_block( &nbytes );
		RKH_EXIT_CRITICAL_();

		if((blk != (rui8_t *)0))
		{
			SERIAL_TRACE_SEND_BLOCK( blk, nbytes );
		}
		else
			break;
	}
}

#endif

void
bsp_timeTick(void)
{
#if RKH_CFG_TRC_EN == RKH_ENABLED
    ++tstamp;
#endif
    switch_tick();
}


void
bsp_uart_init( void )
{
	Chip_UART_Init(UART_USB_LPC);
	Chip_UART_SetBaud(UART_USB_LPC, 19200);  /* Set Baud rate */
	Chip_UART_SetupFIFOS(UART_USB_LPC, UART_FCR_FIFO_EN | UART_FCR_TRG_LEV0); /* Modify FCR (FIFO Control Register)*/
	Chip_UART_TXEnable(UART_USB_LPC); /* Enable UART Transmission */
	Chip_SCU_PinMux(7, 1, MD_PDN, FUNC6);              /* P7_1,FUNC6: UART2_TXD */
	Chip_SCU_PinMux(7, 2, MD_PLN|MD_EZI|MD_ZI, FUNC6); /* P7_2,FUNC6: UART2_RXD */
}

void
bsp_uart_send_block(rui8_t *blk, TRCQTY_T nbytes )
{
	while (nbytes--)
	{
		while ((Chip_UART_ReadLineStatus(UART_USB_LPC) & UART_LSR_THRE) == 0) {}   // Wait for space in FIFO
		      Chip_UART_SendByte(UART_USB_LPC, *blk++);
	}
}

void
bsp_uart_deinit( void )
{

}

void
bsp_publish( const RKH_EVT_T *e )
{
	rInt cn;

	RKH_SMA_POST_FIFO( server, e, &l_isr_kbd );			/* to server */

	for( cn = 0; cn < NUM_CLIENTS; ++cn )				/* to clients */
		RKH_SMA_POST_FIFO( CLI(cn), e, &l_isr_kbd );
}

void
rkh_hook_timetick( void )
{
    bsp_timeTick();
}

void
rkh_hook_start( void )
{
	rkh_fwk_registerEvtPool( ep0sto, SIZEOF_EP0STO, SIZEOF_EP0_BLOCK  );
	rkh_fwk_registerEvtPool( ep1sto, SIZEOF_EP1STO, SIZEOF_EP1_BLOCK  );
}

void
rkh_hook_idle(void)             /* called within critical section */
{
    RKH_ENA_INTERRUPT();
    RKH_TRC_FLUSH();
}

void
rkh_hook_exit( void )
{
	RKH_TRC_FLUSH();
}

/* ------------------------------ File footer ------------------------------ */
/*  Use file_footer.txt file                                                 */
