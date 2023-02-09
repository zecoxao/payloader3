#line 1 "buttons.c"
/*
 * buttons.c
 *
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 *
 */

#include "common.h"
#include "buttons.h"


/* initialize buttons for use */
void
buttonsInitialize (pngData *p, buttonsData *b)
{
  dbgprintf ( "initializing" ) ;

  int i;

  /* initialize image data for buttons */
  buttonsDataInitialize ( b ) ;

  for ( i = 0 ; i <= MAX_BUTTONS ; i++ )
  {
    /* calculate buffer size */
    b->button[i].length = ( b->button[i].width + 1 ) * ( b->button[i].height + 1 ) * ( sizeof ( uint32_t ) ) ;

    /* allocate buffers for button bitmaps */
    b->button[i].ptr = malloc( b->button[i].length ) ;

    /* clear buffer */
    memset ( b->button[i].ptr, 0x00, ( size_t )b->button[i].length ) ;

    /* add bytes required for each button */
    b->length += b->button[i].length ;

    /* load button bitmaps from png images */
    buttonsLoadPng( p, b, ( uchar )i ) ;
  }
}

/* cleanup memory */
void
buttonsFinish(buttonsData *b)
{
  dbgprintf ( "finishing" ) ;

  int i ;

  for ( i = 0 ; i <= MAX_BUTTONS ; i++ )
  {
    free ( b->button[i].ptr ) ;
  }
}

/* load button from png bitmap */
void
buttonsLoadPng (pngData *p, buttonsData *b, uchar button)
{
  int i = 0, x, y ;
  int gx = 0 ;
  int gy = 0 ;
  u32 *buffer = b->button[button].ptr ;
  u32 *pngbmp = (void *)p->bmp_out ;

  if (pngbmp)
  {
    for ( y = b->button[button].miny; y < b->button[button].maxy + 1; y++)
    {
      if ( y >= p->height) break ;
      i = p->width * y ;
      for ( x = b->button[button].minx; x < b->button[button].maxx + 1; x++)
      {
        i += x ;
        if (gx <= b->button[button].width && gy <= b->button[button].height)
        {
          *buffer++ = *( pngbmp + i ) ;
        }
        else
        {
          break ;
        }
        gx++ ;
        i -= x ;
      }
      gx = 0 ;
      gy++ ;
    }
  }
}

/* draw button into buffer */
inline void
buttonDraw (int px, int py, txtBuffer *t, buttonsData *b, uchar button)
{
  int i = 0, x, y ;
  u32 *buffer = t->ptr ;
  u32 *glyphbmp = (void *)b->button[button].ptr ;
  int width = b->button[button].width ;
  int height = b->button[button].height ;

  if (glyphbmp)
  {
    for ( y = py ; y < ( py + height ) ; y++ )
    {
      if ( y >= t->height ) break ;
      i = t->width * y ;
      for ( x = ( t->width - width - px ) ; x < ( t->width - px ) ; x++ )
      {
        i += x ;
        if ( x <= t->width )
        {
          *( buffer + i ) = *( glyphbmp++ ) ;
        }
        i -= x ;
      }
    }
  }
}

