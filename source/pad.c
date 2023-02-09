#line 1 "pad.c"
/*
 * pad.c
 *
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 *
 */

#include <stdio.h>
#include <io/pad.h> // ioPadInit, ioPadCheck, ioPadEnd
#include "common.h"
#include "pad.h"


/* initialize controller */
inline void
padInitialize (padBtnData *pdata )
{
  pdata->last = 0 ;
  dbgprintf ( "initializing" ) ;
  ioPadInit ( 7 ) ;
}

/* release controller */
inline void
padFinish ()
{
  dbgprintf ( "finishing" ) ;
  ioPadEnd () ;
}

/* check controller for input */
inline int
padCheck ( padBtnData *b )
{
  static int i ;
  padInfo padinfo ;
  padData paddata ;

  b->btn = 0 ;

  ioPadGetInfo ( &padinfo ) ;
  for ( i = 0 ; i < MAX_PADS ; i++ )
  {
    if ( !padinfo.status[i] )
    {
      continue ;
    }
    else
    {
      ioPadGetData ( i, &paddata ) ;
      if ( paddata.BTN_TRIANGLE )
      {
        b->btn= PAD_TRIANGLE ;
      }
      else if ( paddata.BTN_CIRCLE )
      {
        b->btn = PAD_CIRCLE ;
      }
      else if ( paddata.BTN_CROSS )
      {
        b->btn = PAD_CROSS ;
      }
      else if ( paddata.BTN_SQUARE )
      {
        b->btn = PAD_SQUARE ;
      }
      else if ( paddata.BTN_SELECT )
      {
        b->btn = PAD_SELECT ;
      }
      else if ( paddata.BTN_START )
      {
        b->btn = PAD_START ;
      }
      else if ( paddata.BTN_UP )
      {
        b->btn = PAD_UP ;
      }
      else if ( paddata.BTN_RIGHT )
      {
        b->btn = PAD_RIGHT ;
      }
      else if ( paddata.BTN_DOWN )
      {
        b->btn = PAD_DOWN ;
      }
      else if ( paddata.BTN_LEFT )
      {
        b->btn = PAD_LEFT ;
      }
      else
      {
        b->btn = 0 ;
      }
    }
  }

  b->now = b->btn & (~b->last) ;
  b->last = b->btn ;

  return b->now ;
}

/* check controller buttons */
inline int
padCheckPayloads ( payloadData *p, eventData *e, padBtnData *b )
{
  switch ( padCheck ( b ) )
  {
    case 0:
      e->exitapp = 1;
      e->menu = 1;
      break ;
    case PAD_TRIANGLE:
      dbgprintf ( "PAD_TRIANGLE" ) ;
      e->exitapp = 1;
      e->menu = 0;
      p->doPayload = 1;
      p->idx = PAD_TRIANGLE;
      break;
/*
    case PAD_CIRCLE:
      dbgprintf ( "PAD_CIRCLE" ) ;
      e->exitapp = 1;
      e->menu = 0;
      p->doPayload = 1;
      p->idx = PAD_CIRCLE;
      break ;
*/
    case PAD_CIRCLE:
      dbgprintf ( "PAD_CIRCLE" ) ;
      e->exitapp = 1;
      e->menu = 0;
      p->doPayload = 1;
      p->idx = PAD_CIRCLE;
      break ;
    case PAD_CROSS:
      dbgprintf ( "PAD_CROSS" ) ;
      e->exitapp = 1;
      e->menu = 0;
      p->doPayload = 1;
      p->idx = PAD_CROSS;
      break ;
    case PAD_SQUARE:
      dbgprintf ( "PAD_SQUARE" ) ;
      e->exitapp = 1;
      e->menu = 0;
      p->doPayload = 1;
      p->idx = PAD_SQUARE;
      break;
    case PAD_SELECT:
      dbgprintf ( "PAD_SELECT" ) ;
      e->exitapp = 1;
      e->menu = 0;
      p->doPayload = 0;
      break;
    case PAD_START:
      dbgprintf ( "PAD_START" ) ;
      e->exitapp = 0;
      e->menu = 0;
      p->doPayload = 0;
      break;
    default:
      dbgprintf ( "default" ) ;
      e->exitapp = 1;
      e->menu = 1;
      break ;
  }
  return 0 ;
}

/* check controller buttons */
inline int
padCheckTools ( toolData *t, eventData *e, padBtnData *b )
{
  switch ( padCheck ( b ) )
  {
    case 0:
      e->exitapp = 1;
      e->menu = 1;
      break ;
    case PAD_TRIANGLE:
      dbgprintf ( "PAD_TRIANGLE" ) ;
      e->exitapp = 0;
      e->menu = 0;
      t->idx = PAD_TRIANGLE;
      sprintf ( t->file, "%s/%s", t->path, t->tool[t->idx].file ) ;
      break;
    case PAD_CIRCLE:
      dbgprintf ( "PAD_CIRCLE" ) ;
      e->exitapp = 0;
      e->menu = 0;
      t->idx = PAD_CIRCLE;
      sprintf ( t->file, "%s/%s", t->path, t->tool[t->idx].file ) ;
      break ;
    case PAD_CROSS:
      dbgprintf ( "PAD_CROSS" ) ;
      e->exitapp = 0;
      e->menu = 0;
      t->idx = PAD_CROSS;
      break ;
    case PAD_SQUARE:
      dbgprintf ( "PAD_SQUARE" ) ;
      e->exitapp = 0;
      e->menu = 0;
      t->idx = PAD_SQUARE;
      sprintf ( t->file, "%s/%s", t->path, t->tool[t->idx].file ) ;
      break;
    case PAD_SELECT:
      dbgprintf ( "PAD_SELECT" ) ;
      e->exitapp = 0;
      e->menu = 0;
      break;
    case PAD_START:
      dbgprintf ( "PAD_START" ) ;
      e->exitapp = 0;
      e->menu = 0;
      break;
    default:
      dbgprintf ( "default" ) ;
      e->exitapp = 1;
      e->menu = 1;
      break ;
  }
  return 0 ;
}

