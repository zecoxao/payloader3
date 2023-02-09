/*
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 */

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <pngdec/pngdec.h>
#include <rsx/rsx.h>

#define MAX_TEXTURES         6


/* texture */
typedef struct _rsx_texture
{
  u32 *buffer;
  u32 offset;

  u32 size;
  u32 width;
  u32 height;
  u32 pitch;
  u32 depth;
  pngData data;
  gcmTexture texture;
} rsxTexture;

/* used to pass data between functions */
typedef struct _rsx_textures
{
  gcmContextData *context;
  rsxTexture texture[MAX_TEXTURES];
} textureData;


/* initialize texture */
void textureInitialize (gcmContextData *c, textureData *t, int idx);

#endif /* __TEXTURE_H__ */

