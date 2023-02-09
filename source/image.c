#line 1 "image.c"
/*
 * image.c
 *
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 *
 */

#include <stdio.h>
#include "common.h"
#include "image.h"

#include "buttons_png.h"          /* png in memory */
#include "payloader3_png.h"       /* png in memory */
#include "bar_png.h"              /* png in memory */
#include "truck_png.h"            /* png in memory */
#include "pl3_png.h"              /* png in memory */
#include "transformer_png.h"      /* png in memory */


/* initialize images */
inline void
imageInitialization ( textureData *tdata )
{
  dbgprintf ( "initializing" ) ;

  /* png bitmaps */
  pngLoadFromBuffer ( ( void * ) buttons_png, buttons_png_size, &tdata->texture[0].data ) ; /* buttons */
  pngLoadFromBuffer ( ( void * ) payloader3_png, payloader3_png_size, &tdata->texture[1].data ) ; /* logo */
  pngLoadFromBuffer ( ( void * ) bar_png, bar_png_size, &tdata->texture[2].data ) ; /* bar */
  pngLoadFromBuffer ( ( void * ) truck_png, truck_png_size, &tdata->texture[3].data ) ; /* truck */
  pngLoadFromBuffer ( ( void * ) pl3_png, pl3_png_size, &tdata->texture[4].data ) ; /* PL3 logo */
  pngLoadFromBuffer ( ( void * ) transformer_png, transformer_png_size, &tdata->texture[5].data ) ; /* transformer */
}


/* png image to buffer */
inline void
imageDrawPngTxt ( int from, int x, int y, txtBuffer *tbuf, pngData *pdata )
{
  static int i ;
  static int px ;
  static int py ;
  u32 *buffer = tbuf->ptr ;
  u32 *pngbmp = ( void * ) pdata->bmp_out ;
  u32 width = pdata->width ;
  u32 height = pdata->height ;

  /* location to put png in buffer */
  if ( from == 0 ) /* left */
  {
    px = x ;
  }
  else if ( from == 1 ) /* center */
  {
    px = ( tbuf->width / 2 ) - ( width / 2 ) ;
  }
  else if ( from == 2 ) /* right */
  {
    px = tbuf->width - width - x ;
  }
  py = y ;

  if ( pngbmp )
  {
    for ( int y = py ; y < ( py + height ) ; y++ )
    {
      if ( y >= tbuf->height ) break ;
      i = tbuf->width * y ;
      for ( int x = px ; x < ( px + width ) ; x++ )
      {
        i+=x ;
        if ( x <= tbuf->width )
        {
          *( buffer + i ) = *pngbmp ;
          pngbmp++ ;
        }
        i-=x ;
      }
    }
  }
}

/* png image to buffer */
inline void
imageDrawPngRsx ( int from, int x, int y, rsxBuffer *rbuf, pngData *pdata )
{
  static int i ;
  static int px ;
  static int py ;
  u32 *buffer = rbuf->ptr ;
  u32 *pngbmp = ( void * ) pdata->bmp_out ;
  u32 width = pdata->width ;
  u32 height = pdata->height ;

  /* location to put png in buffer */
  if ( from == 0 ) /* left */
  {
    px = x ;
  }
  else if ( from == 1 ) /* center */
  {
    px = ( rbuf->width / 2 ) - ( width / 2 ) ;
  }
  else if ( from == 2 ) /* right */
  {
    px = rbuf->width - width - x ;
  }
  py = y ; 

  if ( pngbmp )
  {
    for ( int y = py ; y < ( py + height ) ; y++ )
    {
      if ( y >= rbuf->height ) break ;
      i = rbuf->width * y ;
      for ( int x = px ; x < ( px + width ) ; x++ )
      {
        i+=x ;
        if ( x <= rbuf->width )
        {
          *( buffer + i ) = *pngbmp ;
          pngbmp++ ;
        }
        i-=x ;
      }
    }
  }
}

