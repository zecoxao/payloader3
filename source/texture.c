#line 1 "texture.c"
/*
 * texture.c
 *
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 *
 */

#include <stdio.h>

#include "shader.h"
#include "texture.h"
#include "rsxutil.h"
#include "video.h"
#include "common.h"


inline void
textureInitialize (gcmContextData *c, textureData *t, int idx)
{
  dbgprintf ( "initializing" ) ;

  u32 offset;

  t->texture[idx].width =  t->texture[idx].data.width ;
  t->texture[idx].height = t->texture[idx].data.height ;
  t->texture[idx].depth = sizeof(u32) ;
  t->texture[idx].pitch = t->texture[idx].data.pitch ;
  t->texture[idx].size = t->texture[idx].width * t->texture[idx].height * t->texture[idx].depth ;

  t->texture[idx].buffer = (u32*)rsxMemalign(128, t->texture[idx].size);
  if ( !t->texture[idx].buffer )
  {
    errprintf ( "ERROR" ) ;
    goto error;
  }

  rsxAddressToOffset(t->texture[idx].buffer, &t->texture[idx].offset);
  rsxAddressToOffset(t->texture[idx].data.bmp_out, &offset);

  rsxSetTransferData ( c,
                       GCM_TRANSFER_MAIN_TO_LOCAL,
                       t->texture[idx].offset,
                       t->texture[idx].pitch,
                       offset,
                       t->texture[idx].data.pitch,
                       t->texture[idx].data.width * sizeof(u32),
                       t->texture[idx].data.height ) ;

error:
  if ( t->texture[idx].buffer != NULL )
  {
    rsxFree(t->texture[idx].buffer);
  }
}

inline void
textureSet (gcmContextData *c, textureData *t, int idx, shaderData *vf)
{
  if (!t->texture[idx].buffer)
  {
    errprintf ( "ERROR" ) ;
    goto error;
  }

  rsxInvalidateTextureCache ( c, GCM_INVALIDATE_TEXTURE ) ;

  t->texture[idx].texture.format = (GCM_TEXTURE_FORMAT_A8R8G8B8 | GCM_TEXTURE_FORMAT_LIN) ;
  t->texture[idx].texture.mipmap = GCM_TRUE ;
  t->texture[idx].texture.dimension = GCM_TEXTURE_DIMS_2D ;
  t->texture[idx].texture.cubemap = GCM_FALSE ;
  t->texture[idx].texture.remap = ( (GCM_TEXTURE_REMAP_TYPE_REMAP << GCM_TEXTURE_REMAP_TYPE_B_SHIFT) |
                                    (GCM_TEXTURE_REMAP_TYPE_REMAP << GCM_TEXTURE_REMAP_TYPE_G_SHIFT) |
                                    (GCM_TEXTURE_REMAP_TYPE_REMAP << GCM_TEXTURE_REMAP_TYPE_R_SHIFT) |
                                    (GCM_TEXTURE_REMAP_TYPE_REMAP << GCM_TEXTURE_REMAP_TYPE_A_SHIFT) |
                                    (GCM_TEXTURE_REMAP_COLOR_B << GCM_TEXTURE_REMAP_COLOR_B_SHIFT) |
                                    (GCM_TEXTURE_REMAP_COLOR_G << GCM_TEXTURE_REMAP_COLOR_G_SHIFT) |
                                    (GCM_TEXTURE_REMAP_COLOR_R << GCM_TEXTURE_REMAP_COLOR_R_SHIFT) |
                                    (GCM_TEXTURE_REMAP_COLOR_A << GCM_TEXTURE_REMAP_COLOR_A_SHIFT) ) ;
  t->texture[idx].texture.width = t->texture[idx].width ;
  t->texture[idx].texture.height = t->texture[idx].height ;
  t->texture[idx].texture.depth = t->texture[idx].depth ;
  t->texture[idx].texture.location = GCM_LOCATION_RSX ;
  t->texture[idx].texture.pitch = t->texture[idx].pitch ;
  t->texture[idx].texture.offset = t->texture[idx].offset ;

  rsxLoadTexture ( c,
                   vf->frag.textureUnit,
                   &t->texture[idx].texture ) ;

  rsxTextureControl ( c,
                      vf->frag.textureUnit,
                      GCM_TRUE,
                      0<<8,
                      12<<8,
                      GCM_TEXTURE_MAX_ANISO_1 ) ;

  rsxTextureFilter ( c,
					 idx,
                     vf->frag.textureUnit,
                     GCM_TEXTURE_LINEAR,
                     GCM_TEXTURE_LINEAR,
                     GCM_TEXTURE_CONVOLUTION_QUINCUNX ) ;

  rsxTextureWrapMode ( c,
                       vf->frag.textureUnit,
                       GCM_TEXTURE_CLAMP_TO_EDGE,
                       GCM_TEXTURE_CLAMP_TO_EDGE,
                       GCM_TEXTURE_CLAMP_TO_EDGE,
                       0,
                       GCM_TEXTURE_ZFUNC_LESS,
                       0 ) ;

error:
  errprintf ( "ERROR" ) ;
}

inline void
textureSetDrawEnv (gcmContextData *c, textureData *t, int idx, videoData *vdata)
{
  rsxSetColorMask ( c,
                    GCM_COLOR_MASK_B |
                    GCM_COLOR_MASK_G |
                    GCM_COLOR_MASK_R |
                    GCM_COLOR_MASK_A ) ;

  rsxSetColorMaskMrt ( c, GCM_FALSE ) ;

  u16 x, y, w, h;
  f32 min, max;
  f32 scale[4], offset[4];

  x = 0;
  y = 0;
  w = vdata->width;
  h = vdata->height;
  min = 0.0f;
  max = 1.0f;
  scale[0] = w*0.5f;
  scale[1] = h*-0.5f;
  scale[2] = (max - min)*0.5f;
  scale[3] = 0.0f;
  offset[0] = x + w*0.5f;
  offset[1] = y + h*0.5f;
  offset[2] = (max + min)*0.5f;
  offset[3] = 0.0f;

  rsxSetViewport(c, x, y, w, h, min, max, scale, offset);
  rsxSetScissor(c, x, y, w, h);

  rsxSetDepthTestEnable(c, GCM_TRUE);
  rsxSetDepthFunc(c, GCM_LESS);
  rsxSetShadeModel(c, GCM_SHADE_MODEL_SMOOTH);
  rsxSetDepthWriteEnable(c, GCM_TRUE);
  rsxSetFrontFace(c, GCM_FRONTFACE_CCW);
}

