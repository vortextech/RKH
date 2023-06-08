/**
 *  \file       main.c
 *  \brief      Example application
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2016.03.17  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "bsp.h"
#include "server.h"
#include "client.h"

/* ----------------------------- Local macros ------------------------------ */
#define QSTO_SIZE           4

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
#if RKH_CFGPORT_SMA_QSTO_EN == RKH_ENABLED
/* Defines the event queue storage for active object 'server' */
static RKH_EVT_T *svr_qsto[QSTO_SIZE];

/* Defines the event queue storage for active object 'client' */
static RKH_EVT_T *cli_qsto[NUM_CLIENTS][QSTO_SIZE];

#else
    #define svr_qsto
    #define cli_qsto
#endif

#if RKH_CFGPORT_SMA_STK_EN == RKH_ENABLED
/* Defines the task's stack for active object 'server' */
static RKH_THREAD_STK_TYPE svr_stk[SVR_STK_SIZE];

/* Defines the task's stack for active object 'client' */
static RKH_THREAD_STK_TYPE cli_stk[NUM_CLIENTS][CLI_STK_SIZE];

#else
    #define svr_stk
    #define cli_stk
#endif

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
int
main(int argc, char *argv[])
{
    rInt cn;

    bsp_init(argc, argv);

    RKH_SMA_ACTIVATE(server, svr_qsto, QSTO_SIZE, svr_stk, SVR_STK_SIZE);
    for (cn = 0; cn < NUM_CLIENTS; ++cn)
    {
        RKH_SMA_ACTIVATE(CLI(cn), cli_qsto[cn], QSTO_SIZE, cli_stk[cn],
                         CLI_STK_SIZE);
    }

    rkh_fwk_enter();
    RKH_TRC_CLOSE();

    return 0;
}

/* ------------------------------ End of file ------------------------------ */
