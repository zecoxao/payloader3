/*
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 */

#ifndef __BUFFERS_H__
#define __BUFFERS_H__

#include <sys/sem.h>
#include <rsx/rsx.h>


typedef struct _rsx_buffer
{
  //gcmContextData *context;
  int id;
  u16 width;
  u16 height;
  u32 size;
  u32 chromakey;
  u32 *ptr;
  // Internal stuff
  u32 color_depth;
  u32 color_pitch;
  u32 color_offset;
} rsxBuffer;

typedef struct _color_buffer
{
  int id;
  int size;
  void *ptr;
  u32 offset;
} colorBuffer;

typedef struct _vertex_buffer
{
  float x;
  float y;
  float z;
  u32 rgba;
} vertexBuffer;

typedef struct _vertex_array_buffer
{
  int id;
  int size;
  void *ptr;
  u32 offset;
  vertexBuffer *buf;
} vertexArrayBuffer;

typedef struct _fragment_shader_buffer
{
  int id;
  int size;
  void *ptr;
  u32 offset;
} fragmentShaderBuffer;

typedef struct _txt_buffer
{
  int id;
  u16 width;
  u16 height;
  u32 size;
  u32 length;
  u32 chromakey;
  u32 *ptr;
  /* */
  u32 color_depth;
  u32 color_pitch;
  u32 color_offset;
  /* */
  sys_sem_t sem;
  sys_sem_attr_t sem_attr;
} txtBuffer;

#endif /* __BUFFERS_H__ */

