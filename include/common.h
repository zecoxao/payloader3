/*
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 */

#ifndef __COMMON_H__
#define __COMMON_H__

typedef unsigned char uchar;

#define PAGE_SIZE          0x1000

#ifdef NORMAL
#define USB_DESCRIPTOR     0x20
#endif

#ifdef STANDALONE
#define USB_DESCRIPTOR     0x00
#endif

#define LINE_INDENT        40
#define LINE_SIZE          20

#define CHROMAKEY          0x00ffffff

#define SEM_CONSUMERS      2
#define TIMEOUT            10000
#define NO_TIMEOUT         0x00

#define FONT_SIZE 16
#define MAX_STRING 256

#define FONT_COLOR_NONE  -1

#define FONT_COLOR_BLACK  0xff000000
#define FONT_COLOR_GREY   0xff444444
#define FONT_COLOR_WHITE  0xffffffff

#define FONT_COLOR_RED    0xfffa001c
#define FONT_COLOR_ORANGE 0xfffa8500
#define FONT_COLOR_YELLOW 0xfffae900
#define FONT_COLOR_GREEN  0xff3fd711
#define FONT_COLOR_BLUE   0xff1775ca

#define FONT_COLOR_FG FONT_COLOR_YELLOW
#define FONT_COLOR_BG FONT_COLOR_BLACK

#define dbgprintf(message) printf ( "\033[0;33m%s:%s():#%d :: "message"\033[0m\n", __FILE__, __func__, __LINE__ ) ;
#define errprintf(message) printf ( "\031[0;31m%s:%s():#%d :: "message"\033[0m\n", __FILE__, __func__, __LINE__ ) ;
#define retprintf(message) printf ( "\031[0;31m%s:%s():#%d :: "message"[0x%x]\033[0m\n", __FILE__, __func__, __LINE__, ret ) ;

#define COLOR_TO_ARGB(alpha, red, green, blue) (((alpha) << 24) | ((red) << 16) | ((green) << 8) | (blue))
#define COLOR_TO_RGB(red, green, blue) (((red) << 16) | ((green) << 8) | (blue))

#define VALUE_TO_ARGB(value) (((value) << 24) | ((value) << 16) | ((value) << 8) | (value))
#define VALUE_TO_RGB(value) (((value) << 16) | ((value) << 8) | (value))

#define A(argb)  (((argb) >> 24) & 0xFF)
#define R(argb)  (((argb) >> 16) & 0xFF)
#define G(argb)  (((argb) >>  8) & 0xFF)
#define B(argb)  ( (argb)        & 0xFF)


#endif /* __COMMON_H__ */

