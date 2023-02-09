/*
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 */

#ifndef __BUTTONS_H__
#define __BUTTONS_H__

#include <ppu-types.h>
#include <pngdec/pngdec.h>

#include "ps3console.h"

#define MAX_BUTTONS       4

#define BUTTON_TRIANGLE   0
#define BUTTON_SQUARE     1
#define BUTTON_CIRCLE     2
#define BUTTON_CROSS      3


typedef struct _button_png_data
{
  unsigned char button;
  u32 minx;
  u32 miny;
  u32 maxx;
  u32 maxy;
  u32 width;
  u32 height;
  u32 length;
  u32 chromakey;
  u32 *ptr;
} buttonPngData;

/* used to pass data between functions */
typedef struct _buttons_data
{
  u32 width;
  u32 height;
  u32 length;
  u32 chromakey;
  buttonPngData button[5];
} buttonsData;


void buttonsInitialize(pngData *p, buttonsData *b);
void buttonsFinish(buttonsData *b);
void buttonsDataInitialize(buttonsData *b);
void buttonsLoadPng(pngData *p, buttonsData *b, unsigned char button);
void buttonDraw(int px, int py, txtBuffer *t, buttonsData *b, unsigned char button);

#endif /* __BUTTONS_H__ */

