/*
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 */

#ifndef __SHADER_H__
#define __SHADER_H__

#include <rsx/rsx.h>

#include "common.h"
#include "video.h"


/* vertex shader */
typedef struct _vertex_shader
{
  s32 projMatrix;
  s32 modelViewMatrix;
  s32 position;
  s32 normal;
  s32 texcoord;

  void *ucode;
  u32 ucode_size;
  rsxVertexProgram *vpo;
} vertexShader;

/* fragment shader */
typedef struct _fragment_shader
{
  u32 *buffer;
  u32 size;
  u32 offset;

  s32 textureUnit;
  s32 eyePosition;
  s32 globalAmbient;
  s32 lightPosition;
  s32 lightColor;
  s32 shininess;
  s32 Ks;
  s32 Kd;

  void *ucode;
  u32 ucode_size;
  rsxFragmentProgram *fpo;
} fragmentShader;

/* used to pass data between functions */
typedef struct _shaders
{
  vertexShader vert;
  fragmentShader frag;
} shaderData;


/* initialize shader */
void shaderInitialize (shaderData *vf);
void buildProjection(float *M, float top, float bottom, float left, float right, float near, float far);
void matrixMul(float *Dest, float *A, float *B);
void matrixTranslate(float *M, float x, float y, float z);
void unitMatrix(float *M);
void initRenderObject(videoData *vdata);

#endif /* __SHADER_H__ */

