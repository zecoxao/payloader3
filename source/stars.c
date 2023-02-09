#line 1 "stars.c"
/*
 * stars.c
 *
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 *
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ppu-types.h>

#include <time.h>
#include "common.h"
#include "stars.h"


/* clear star buffer */
inline void
starsClearData ( starsData *s )
{
  memset( s, 0x00, (size_t)( sizeof(starsData) * MAX_STARS ) );
}

/* initialize stars */
void
starsInitialize ( starsData *s )
{
  dbgprintf ( "initializing" ) ;

  int color = 0 ;

  /* prime pseudo-random generator */
  srand ( time ( NULL ) ) ;

  /* randomly generate stars */
  for (int i = 0; i < MAX_STARS; i++)
  {
    s->star[i].x     = rand() % s->width ;
    s->star[i].y     = rand() % s->height ;
    s->star[i].plane = rand() % 16 ; // star color 0 - COLORS
    color = 128 + s->star[i].plane * 8;
    s->star[i].color = COLOR_TO_RGB(color, color, color);
  }
}

/* draw stars to buffer */
inline void
starsDrawRightRsx ( starsData *s, rsxBuffer *buffer )
{
  for (int i = 0; i < MAX_STARS; i++)
  {
    /* move star right, speed based on plane */
    s->star[i].x += (1 + (float)s->star[i].plane) * 0.15;
    /* right edge of screen? */
    if (s->star[i].x > buffer->width)
    {
      /* reset to left edge */
      s->star[i].x = 0;
      /* random y position */
      s->star[i].y = rand() % buffer->height;
    }
    /* draw star to buffer */
    *(buffer->ptr + (int)(buffer->width * s->star[i].y) + (int)s->star[i].x) = s->star[i].color;
  }
}

/* draw stars to buffer */
inline void
starsDrawLeftRsx ( starsData *s, rsxBuffer *buffer )
{
  for (int i = 0; i < MAX_STARS; i++)
  {
    /* move star left, speed based on plane */
    s->star[i].x -= (1 + (float)s->star[i].plane) * 0.15;
    /* left edge of screen? */
    if (s->star[i].x < 0)
    {
      /* reset to right edge */
      s->star[i].x = buffer->width;
      /* random y position */
      s->star[i].y = rand() % buffer->height;
    }
    /* draw star to buffer */
    *(buffer->ptr + (int)(buffer->width * s->star[i].y) + (int)s->star[i].x) = s->star[i].color;
  }
}

