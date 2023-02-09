/*
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 */

#ifndef __FONT_H__
#define __FONT_H__

#include "buffers.h"
#include "ps3console.h"

#include <ft2build.h>
#include FT_FREETYPE_H


void fontInitialize ();
void fontFinish ();
void fontLoadTTF ();
void fontDrawBitmap (FT_Bitmap *bitmap, int offset, int top, txtBuffer *tbuf);

#endif /* __FONT_H__ */

