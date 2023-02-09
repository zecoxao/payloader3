#line 1 "buttons_data.c"
/*
 * buttons_data.c
 *
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 *
 */

#include "common.h"
#include "buttons.h"


/* png image data */
void
buttonsDataInitialize (buttonsData *b)
{
  dbgprintf ( "initializing" ) ;

  b->width = 64 ;
  b->height = 16 ;
  b->chromakey = CHROMAKEY ;

  /* △  triangle */
  b->button[0].minx = 0;
  b->button[0].miny = 0;
  b->button[0].maxx = 15;
  b->button[0].maxy = 15;
  b->button[0].width = 16;
  b->button[0].height = 16;
  b->button[0].ptr = NULL;

  /* ☐  square */
  b->button[1].minx = 16;
  b->button[1].miny = 0;
  b->button[1].maxx = 31;
  b->button[1].maxy = 15;
  b->button[1].width = 16;
  b->button[1].height = 16;
  b->button[1].ptr = NULL;

  /* ◯  circle */
  b->button[2].minx = 32;
  b->button[2].miny = 0;
  b->button[2].maxx = 47;
  b->button[2].maxy = 15;
  b->button[2].width = 16;
  b->button[2].height = 16;
  b->button[2].ptr = NULL;

  /* ✕  cross */
  b->button[3].minx = 48;
  b->button[3].miny = 0;
  b->button[3].maxx = 63;
  b->button[3].maxy = 15;
  b->button[3].width = 16;
  b->button[3].height = 16;
  b->button[3].ptr = NULL;
}

