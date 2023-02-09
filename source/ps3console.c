#line 1 "ps3console.c"
/*
 *  ps3console.c
 *  2011 an0nym0us
 *
 *  based on sconsole by Scognito (scognito@gmail.com)
 */

#include "common.h"
#include "font.h"
#include "video.h"
#include "buffers.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "MonkirtaPursuitNC_ttf.h"


/* initialize ps3console */
void
ps3consoleInitialize (int bgColor, int fgColor, int fontSize, int width, int height)
{
  dbgprintf ( "initializing" ) ;

  ps3console.bgColor = bgColor ;
  ps3console.fgColor = fgColor ;
  ps3console.fontSize = fontSize ;
  ps3console.fontPath = "/dev_hdd0/game/PayLoader/USRDIR/fonts" ;
  ps3console.fontName = "MonkirtaPursuitNC.ttf" ;
  ps3console.fontPtr = (FT_Byte*)MonkirtaPursuitNC_ttf ;
  ps3console.fontLen = MonkirtaPursuitNC_ttf_size ;
  ps3console.x = 0 ;
  ps3console.y = 0 ;
  ps3console.width = width ;
  ps3console.height = height ;
  ps3console.chromakey = CHROMAKEY ;

  dbgprintf ( "fontInitialize()" ) ;
  fontInitialize () ;

  dbgprintf ( "fontLoadTTF()" ) ;
  fontLoadTTF () ;
}

/* clear text buffer */
inline void
txtBufferClear ( txtBuffer *tbuf )
{
  memset ( tbuf->ptr, 0x00, ( size_t )( tbuf->width * tbuf->height * sizeof ( uint32_t ) ) ) ;
}

/* initialize txtBuffer */
inline void
txtBufferInitialize (videoData *vdata)
{
  txtBuffer *tbuf = vdata->txt_buffer ;

  tbuf->chromakey = ps3console.chromakey ;
  tbuf->width = ps3console.width ;
  tbuf->height = ps3console.height ;
  tbuf->color_depth = sizeof(u32) ;
  tbuf->color_pitch = tbuf->width * tbuf->color_depth ;
  tbuf->size = tbuf->width * tbuf->height * tbuf->color_depth ;

  tbuf->ptr = (uint32_t*) rsxMemalign (64, tbuf->size) ;
#ifdef DEBUG
  printf ( "\033[0;33m%s:%s():#%d :: rsxMemalign( 64, 0x%x ) = 0x%llx\033[0m\n", __FILE__, __func__, __LINE__, tbuf->size, (long long unsigned int)tbuf->ptr ) ;
#endif
  rsxAddressToOffset ( tbuf->ptr, &tbuf->color_offset ) ;
#ifdef DEBUG
  printf ( "\033[0;33m%s:%s():#%d :: rsxAddressToOffset ( 0x%llx, 0x%x )\033[0m\n", __FILE__, __func__, __LINE__, (long long unsigned int)tbuf->ptr, (unsigned int)tbuf->color_offset ) ;
#endif
  txtBufferClear( tbuf ) ;

  /* initialize semaphore attributes */
  tbuf->sem_attr.key            = 0x02 ;
  tbuf->sem_attr.attr_protocol  = SYS_SEM_ATTR_PROTOCOL ;
  tbuf->sem_attr.attr_pshared   = SYS_SEM_ATTR_PSHARED ;

  /* create semaphore */
  sysSemCreate ( &tbuf->sem, &tbuf->sem_attr, 1, SEM_CONSUMERS ) ; 
}

inline void
txtBufferFinish(txtBuffer *tbuf)
{
  dbgprintf ( "finishing" ) ;

  sysSemDestroy ( tbuf->sem ) ;

  /* free freetype resources */
  fontFinish () ;

  /* free txtBuffer */
  rsxFree ( tbuf->ptr ) ;
}

/* draw buffer to rsx */
inline void
txtBufferDraw( gcmContextData *context, txtBuffer *tbuf, rsxBuffer *rbuf )
{
  gcmTransferScale xferscale ;
  memset ( &xferscale, 0, sizeof ( xferscale ) ) ;

  gcmTransferSurface xfersurface ;
  memset ( &xfersurface, 0, sizeof ( xfersurface ) ) ;

  /* configure transfer scale */
  xferscale.conversion = GCM_TRANSFER_CONVERSION_TRUNCATE ;
  xferscale.format = GCM_TRANSFER_SCALE_FORMAT_A8R8G8B8 ;
  xferscale.operation = GCM_TRANSFER_OPERATION_SRCCOPY_AND ;
  xferscale.clipX = 0 ;
  xferscale.clipY = 0 ;
  xferscale.clipW = rbuf->width ;
  xferscale.clipH = rbuf->height ;
  xferscale.outX = 0 ;
  xferscale.outY = 0 ;
  xferscale.outW = rbuf->width ;
  xferscale.outH = rbuf->height ;
  xferscale.inX = rsxGetFixedUint16 ( 1.0f ) ;
  xferscale.inY = rsxGetFixedUint16 ( 1.0f ) ;
  xferscale.inW = tbuf->width ;
  xferscale.inH = tbuf->height ;

  xferscale.ratioX = rsxGetFixedSint32 ( (float)xferscale.inW / (float)xferscale.outW ) ;
  xferscale.ratioY = rsxGetFixedSint32 ( (float)xferscale.inH / (float)xferscale.outH ) ;

  xferscale.pitch = tbuf->color_pitch ;
  xferscale.origin = GCM_TRANSFER_ORIGIN_CORNER ;
  xferscale.interp = GCM_TRANSFER_INTERPOLATOR_NEAREST ;
  xferscale.offset = tbuf->color_offset ;

  /* configure destination surface for transfer */
  xfersurface.format = GCM_TRANSFER_SURFACE_FORMAT_A8R8G8B8 ;
  xfersurface.pitch = rbuf->color_pitch ;
  xfersurface.offset = rbuf->color_offset ;

  /* blit txt buffer */
  rsxSetTransferScaleMode ( context, GCM_TRANSFER_LOCAL_TO_LOCAL, GCM_TRANSFER_SURFACE ) ;
  rsxSetTransferScaleSurface ( context, &xferscale, &xfersurface ) ;
}

/* print text to txtBuffer */
inline void
print(int x, int y, char* text, txtBuffer *tbuf)
{
  int i = 0 ;
  int error = 0 ;
  size_t textlen = strlen ( text ) ;

  if ( textlen > 0 )
  {
    ps3console.pen.x = 0 ;
    ps3console.pen.y = ps3console.fontSize ;

    FT_GlyphSlot slot = ps3console.face->glyph;
    FT_UInt glyph_index = 0 ;
    FT_UInt previous_glyph = 0 ;

    /* enable kerning */
    ps3console.fontKerning = FT_HAS_KERNING(ps3console.face) ;

    for(i = 0; i < textlen; i++)
    {
      /* retrieve glyph index from character code */
      glyph_index = FT_Get_Char_Index(ps3console.face, text[i]);

      /* grab kerning info */
      if ( ps3console.fontKerning && previous_glyph && glyph_index )
      {
        FT_Vector delta ;
        FT_Get_Kerning ( ps3console.face, previous_glyph, glyph_index, FT_KERNING_DEFAULT, &delta ) ;
        ps3console.pen.x += delta.x >> 6 ;
      }

      /* load glyph image into the slot (erase previous one) */
      error = FT_Load_Glyph ( ps3console.face, glyph_index, FT_LOAD_RENDER ) ;
      if ( error ) continue ;

      FT_Get_Glyph ( ps3console.face->glyph, &ps3console.glyph ) ;

      FT_Glyph_StrokeBorder ( &ps3console.glyph, ps3console.stroker, 0, 0 ) ;

      /* draw glyph to txtBuffer */
      fontDrawBitmap ( &slot->bitmap,
                       ps3console.pen.x + slot->bitmap_left + x,
                       (ps3console.pen.y - slot->bitmap_top + y - ps3console.fontSize),
                       tbuf ) ;

      /* store current glyph index in previous */
      previous_glyph = glyph_index ;

      /* increment pen position */
      ps3console.pen.x += slot->advance.x >> 6 ;
      ps3console.pen.y += slot->advance.y >> 6 ;
    }
  }
}

