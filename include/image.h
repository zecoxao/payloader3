/*
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 */

#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <pngdec/pngdec.h>

#include "common.h"
#include "texture.h"
#include "buffers.h"


void imageInitialization(textureData *tdata);
void imageDrawPngTxt(int from, int x, int y, txtBuffer *tbuf, pngData *pdata);
void imageDrawPngRsx(int from, int x, int y, rsxBuffer *rbuf, pngData *pdata);

#endif /* __IMAGE_H__ */

