/*
 *  ps3console - 2011 an0nym0us
 *
 *  based on sconsole by Scognito (scognito@gmail.com)
 */

#ifndef _PS3CONSOLE_H
#define _PS3CONSOLE_H

#include <ppu-lv2.h>
#include <stdio.h> // snprintf
#include <string.h> // strlen

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_SYNTHESIS_H
#include FT_STROKER_H

#include "common.h"
#include "video.h"
#include "buffers.h"


typedef struct _ps3_console
{
  int x;
  int y;
  int fgColor;
  int bgColor;
  int fontSize;
  int width;
  int height;
  u32 chromakey;
  char *fontPath;
  char *fontName;
  char *fontFile;
  s32 fontLen;
  bool fontKerning;
  FT_Byte *fontPtr;
  FT_Library library;
  FT_Glyph glyph;
  FT_Stroker stroker;
  FT_Face face;
  FT_Bitmap *bitmap;
  FT_Vector pen;
} ps3_console;

ps3_console ps3console;


void ps3consoleInitialize(int bgColor, int fgColor, int fontSize, int width, int height);
void txtBufferInitialize(videoData *vdata);
void txtBufferFinish(txtBuffer *tbuf);
void txtBufferClear(txtBuffer *tbuf);
void txtBufferDraw(gcmContextData *context, txtBuffer *tbuf, rsxBuffer *rbuf);
void print(int x, int y, char* text, txtBuffer *tbuf);

#endif

