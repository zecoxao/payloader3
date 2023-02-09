#line 1 "font.c"
/*
 * font.c
 *
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 *
 */
#include "common.h"
#include "font.h"
#include "ps3console.h"

#include <ft2build.h>  
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_SYNTHESIS_H
#include FT_STROKER_H


/* initialize freetype */
void
fontInitialize ()
{
  dbgprintf ( "initializing freetype" ) ;

  static int ret ;

  ret = FT_Init_FreeType ( &ps3console.library );
  if ( ret )
  {
    retprintf ( "freetype init error" ) ;
    goto error;
  }
  return ;

error:
  FT_Done_FreeType ( ps3console.library ) ;
}

/* free resources */
void
fontFinish ()
{
  dbgprintf ( "finishing freetype" ) ;
  FT_Stroker_Done ( ps3console.stroker ) ;
  FT_Done_Glyph ( ps3console.glyph ) ;
  FT_Done_FreeType ( ps3console.library ) ;
}

/* load freetype font */
void
fontLoadTTF ()
{
#ifdef DEBUG
  printf ("%s:%s():#%d :: loading font: %s\033[0m\n", __FILE__, __func__, __LINE__, ps3console.fontName ) ;
#endif

  static int ret ;

  /* load font from memory */
  ret = FT_New_Memory_Face( ps3console.library, ps3console.fontPtr, ps3console.fontLen, 0, &ps3console.face ) ;
  if ( ret )
  {
    printf ( "\033[0;31m%s:%s():#%d :: unable to load font %s [%d]\033[0m\n", __FILE__, __func__, __LINE__, ps3console.fontName, ret ) ;
    goto error ;
  }

  /* stroker */
  ret = FT_Stroker_New ( ps3console.library, &ps3console.stroker ) ;
  if ( ret )
  {
    retprintf ( "unable to create stroker" ) ;
    goto error ;
  }
  FT_Stroker_Set ( ps3console.stroker, 40, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0 ) ;

  /* enable kerning */
  ps3console.fontKerning = FT_HAS_KERNING(ps3console.face) ;

  /* font size in pixels */
  ret = FT_Set_Pixel_Sizes ( ps3console.face, 0, ps3console.fontSize );
  if( ret ) {
    retprintf ( "SetPixelSize error" ) ;
    goto error ;
  }
  return ;

error:
  FT_Done_Face ( ps3console.face ) ;
  FT_Done_FreeType ( ps3console.library ) ;
}

/* draw font glyph into buffer */
void
fontDrawBitmap ( FT_Bitmap *bitmap, int offset, int top, txtBuffer *tbuf )
{
  static int color, a, r, g, b ;
  FT_Int x, y, i, j ;
  FT_Int x_max = offset + bitmap->width ;
  FT_Int y_max = top + bitmap->rows ;

  int fgA = A(ps3console.fgColor) ;
  int fgR = R(ps3console.fgColor) ;
  int fgG = G(ps3console.fgColor) ;
  int fgB = B(ps3console.fgColor) ;

  u32 width = tbuf->width ;
  u32 height = tbuf->height ;

  for ( x = offset, i = 0 ; x < x_max ; x++, i++ )
  {
    if ( x >= width ) break ;
    for ( y = top, j = 0 ; y < y_max ; y++, j++ )
    {
      if ( y >= height ) break ;

      /* grab the current glyph pixel value */
      color = bitmap->buffer[bitmap->width * j + i] ;

      /* calculate ARGB values for pixel color */
      a = ( ( fgA * color + 255 ) >> 8 ) ;
      r = ( ( fgR * color + 255 ) >> 8 ) ;
      g = ( ( fgG * color + 255 ) >> 8 ) ;
      b = ( ( fgB * color + 255 ) >> 8 ) ;

      /* fill buffer with pixel values */
      *( tbuf->ptr + tbuf->width * y + x ) = COLOR_TO_ARGB(a, r, g, b) ;
    }
  }
}

