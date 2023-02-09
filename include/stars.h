/*
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 */

#ifndef __STARS_H__
#define __STARS_H__

#define MAX_STARS 256
#define COLORS 256

#include "common.h"


typedef struct _star_data
{
  float x;
  float y;
  uint32_t color;
  unsigned char plane;
} starData;

/* used to pass data between functions */
typedef struct _stars_data
{
  u32 width;
  u32 height;
  u32 direction;
  starData star[MAX_STARS];
} starsData;

#include "rsxutil.h"

void starsClearData(starsData *s);
void starsInitialize(starsData *s);
void starsDrawRightRsx(starsData *s, rsxBuffer *buffer);
void starsDrawLeftRsx(starsData *s, rsxBuffer *buffer);

#endif /* __STARS_H__ */

