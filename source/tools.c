#line 1 "tools.c"
/*
 * tools.c
 *
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 *
 */

#include <stdio.h>

#include "common.h"
#include "pad.h"
#include "tools.h"


/* initialize tools struct values */
inline void
toolsInitialize ( toolData *t )
{
  dbgprintf ( "initializing" ) ;

  /* define path to files on PS3 */
  t->path = "/dev_hdd0/game/payloader/USRDIR/tools" ;

  /*
   * list of tool files presented to user
   *
   * name = text to be displayed to user
   * file = filename found at the path defined above
   */
  #ifdef FIRMWARE_3_41
    t->tool[PAD_TRIANGLE].name = "ps3load" ;
    t->tool[PAD_TRIANGLE].file = "ps3load.self" ;
    t->tool[PAD_CIRCLE].name = "ethdebug" ;
    t->tool[PAD_CIRCLE].file = "EthernetDebugLoader-v2.self" ;
    t->tool[PAD_CROSS].name = "return to XMB" ;
    t->tool[PAD_CROSS].file = "vsh.self" ;
    t->tool[PAD_SQUARE].name = "ethdebug/ps3load" ;
    t->tool[PAD_SQUARE].file = "EthernetDebugLoader-v2_PS3load.self" ;
  #elif defined ( FIRMWARE_3_41_KIOSK )
    t->tool[PAD_TRIANGLE].name = "ps3load" ;
    t->tool[PAD_TRIANGLE].file = "ps3load.self" ;
    t->tool[PAD_CIRCLE].name = "ethdebug" ;
    t->tool[PAD_CIRCLE].file = "EthernetDebugLoader-v2.self" ;
    t->tool[PAD_CROSS].name = "return to XMB" ;
    t->tool[PAD_CROSS].file = "vsh.self" ;
    t->tool[PAD_SQUARE].name = "ethdebug/ps3load" ;
    t->tool[PAD_SQUARE].file = "EthernetDebugLoader-v2_PS3load.self" ;
  #elif defined ( FIRMWARE_3_40 )
    t->tool[PAD_TRIANGLE].name = "ps3load" ;
    t->tool[PAD_TRIANGLE].file = "ps3load.self" ;
    t->tool[PAD_CIRCLE].name = "ps3load" ;
    t->tool[PAD_CIRCLE].file = "ps3load.self" ;
    t->tool[PAD_CROSS].name = "return to XMB" ;
    t->tool[PAD_CROSS].file = "vsh.self" ;
    t->tool[PAD_SQUARE].name = "ps3load" ;
    t->tool[PAD_SQUARE].file = "ps3load.self" ;
  #elif defined ( FIRMWARE_3_30 )
    t->tool[PAD_TRIANGLE].name = "ps3load" ;
    t->tool[PAD_TRIANGLE].file = "ps3load.self" ;
    t->tool[PAD_CIRCLE].name = "ps3load" ;
    t->tool[PAD_CIRCLE].file = "ps3load.self" ;
    t->tool[PAD_CROSS].name = "return to XMB" ;
    t->tool[PAD_CROSS].file = "vsh.self" ;
    t->tool[PAD_SQUARE].name = "ps3load" ;
    t->tool[PAD_SQUARE].file = "ps3load.self" ;
  #elif defined ( FIRMWARE_3_21 )
    t->tool[PAD_TRIANGLE].name = "ps3load" ;
    t->tool[PAD_TRIANGLE].file = "ps3load.self" ;
    t->tool[PAD_CIRCLE].name = "ps3load" ;
    t->tool[PAD_CIRCLE].file = "ps3load.self" ;
    t->tool[PAD_CROSS].name = "return to XMB" ;
    t->tool[PAD_CROSS].file = "vsh.self" ;
    t->tool[PAD_SQUARE].name = "ps3load" ;
    t->tool[PAD_SQUARE].file = "ps3load.self" ;
  #elif defined ( FIRMWARE_3_15 )
    t->tool[PAD_TRIANGLE].name = "ps3load" ;
    t->tool[PAD_TRIANGLE].file = "ps3load.self" ;
    t->tool[PAD_CIRCLE].name = "ps3load" ;
    t->tool[PAD_CIRCLE].file = "ps3load.self" ;
    t->tool[PAD_CROSS].name = "return to XMB" ;
    t->tool[PAD_CROSS].file = "vsh.self" ;
    t->tool[PAD_SQUARE].name = "ps3load" ;
    t->tool[PAD_SQUARE].file = "ps3load.self" ;
  #elif defined ( FIRMWARE_3_10 )
    t->tool[PAD_TRIANGLE].name = "ps3load" ;
    t->tool[PAD_TRIANGLE].file = "ps3load.self" ;
    t->tool[PAD_CIRCLE].name = "ps3load" ;
    t->tool[PAD_CIRCLE].file = "ps3load.self" ;
    t->tool[PAD_CROSS].name = "return to XMB" ;
    t->tool[PAD_CROSS].file = "vsh.self" ;
    t->tool[PAD_SQUARE].name = "ps3load" ;
    t->tool[PAD_SQUARE].file = "ps3load.self" ;
  #elif defined ( FIRMWARE_3_01 )
    t->tool[PAD_TRIANGLE].name = "ps3load" ;
    t->tool[PAD_TRIANGLE].file = "ps3load.self" ;
    t->tool[PAD_CIRCLE].name = "ps3load" ;
    t->tool[PAD_CIRCLE].file = "ps3load.self" ;
    t->tool[PAD_CROSS].name = "return to XMB" ;
    t->tool[PAD_CROSS].file = "vsh.self" ;
    t->tool[PAD_SQUARE].name = "ps3load" ;
    t->tool[PAD_SQUARE].file = "ps3load.self" ;
  #endif
}
