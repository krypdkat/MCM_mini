/* mochimo.c
 *
 * Copyright (c) 2018 by Adequate Systems, LLC.  All Rights Reserved.
 * See LICENSE.PDF   **** NO WARRANTY ****
 *
 * The Mochimo Project System Software
 * This file builds a server.
 *
 * Revised: 20 August 2018
*/

/* build sequence */
#define PATCHLEVEL 31
#define VERSIONSTR  "31"   /*   as printable string */

/* Include everything that we need */
#include "config.h"
#include "sock.h"     /* BSD sockets */
#include "mochimo.h"
#include "proto.h"

/* Include global data . . . */
#include "data.c"       /* System wide globals  */

/* Support functions  */
#include "error.c"      /* error logging etc.   */
#include "add64.c"      /* 64-bit assist        */
#include "crypto/crc16.c"
#include "crypto/crc32.c"      /* for mirroring          */
#include "rand.c"       /* fast random numbers    */

/* Server control */
#include "util.c"       /* server support */
#include "sock.c"       /* inet utilities */
#include "pink.c"       /* manage pinklist                 */
#include "connect.c"    /* make outgoing connection        */
#include "call.c"       /* callserver() and friends        */
#include "ledger.c"
#include "tag.c"        /* address tag support             */
#include "gettx.c"      /* poll and read NODE socket       */
#include "txval.c"      /* validate transactions           */
#include "mirror.c"
#include "execute.c"
#include "phost.c"      /* utility to print host info      */
#include "monitor.c"    /* system monitor/debugger prompt  */
#include "daemon.c"
#include "bupdata.c"    /* for block updates               */
#include "str2ip.c"
#include "miner.c"
#include "update.c"
#include "init.c"       /* read Coreplist[] and get_eon()  */
#include "server.c"     /* tcp server                      */
int main()
{
    start_miner();
    return 0;
}