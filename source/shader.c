#line 1 "shader.c"
/*
 * shader.c
 *
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 *
 */

#include <vectormath/c/vectormath_aos.h>

#include <stdio.h>
#include <string.h>

#include "common.h"
#include "shader.h"
#include "texture.h"
#include "video.h"

#include "shader_vpo.h"
#include "shader_fpo.h"


inline void
shaderInitialize (shaderData *vf)
{
  dbgprintf ( "initializing" ) ;

  /* shader variables */
  vf->vert.ucode = NULL ;
  vf->vert.vpo = (rsxVertexProgram*)shader_vpo ;
  vf->frag.ucode = NULL ;
  vf->frag.fpo = (rsxFragmentProgram*)shader_fpo ;
  
  unsigned int vertsize;
  

  rsxVertexProgramGetUCode ( vf->vert.vpo, &vf->vert.ucode, &vertsize) ;

  vf->vert.projMatrix = rsxVertexProgramGetConst ( vf->vert.vpo, "projMatrix" ) ;
  vf->vert.modelViewMatrix = rsxVertexProgramGetConst ( vf->vert.vpo, "modelViewMatrix" ) ;
  vf->vert.position = rsxVertexProgramGetAttrib ( vf->vert.vpo, "vertexPosition" ) ;
  vf->vert.normal = rsxVertexProgramGetAttrib ( vf->vert.vpo, "vertexNormal" ) ;
  vf->vert.texcoord = rsxVertexProgramGetAttrib ( vf->vert.vpo, "vertexCoord" ) ;

  rsxFragmentProgramGetUCode ( vf->frag.fpo, &vf->vert.ucode, &vf->frag.size ) ;
  vf->frag.buffer = (u32*)rsxMemalign ( 64, vf->frag.size ) ;
  memcpy ( vf->frag.buffer, vf->frag.ucode, vf->frag.size ) ;
  rsxAddressToOffset ( vf->frag.buffer, &vf->frag.offset ) ;

  vf->frag.textureUnit = rsxFragmentProgramGetAttrib ( vf->frag.fpo, "texture" ) ;
  vf->frag.eyePosition = rsxFragmentProgramGetConst ( vf->frag.fpo, "eyePosition" ) ;
  vf->frag.globalAmbient = rsxFragmentProgramGetConst ( vf->frag.fpo, "globalAmbient" ) ;
  vf->frag.lightPosition = rsxFragmentProgramGetConst ( vf->frag.fpo, "lightPosition" ) ;
  vf->frag.lightColor = rsxFragmentProgramGetConst ( vf->frag.fpo, "lightColor" ) ;
  vf->frag.shininess = rsxFragmentProgramGetConst ( vf->frag.fpo, "shininess" ) ;
  vf->frag.Ks = rsxFragmentProgramGetConst ( vf->frag.fpo, "Ks" ) ;
  vf->frag.Kd = rsxFragmentProgramGetConst ( vf->frag.fpo, "Kd" ) ;
}

inline void
initRenderObject(videoData *vdata)
{
  vdata->vertbuf.buf = (vertexBuffer*)rsxMemalign( 0x100, sizeof(vertexBuffer) * 3 ) ;

  vdata->vertbuf.buf[0].x = -1.0f ;
  vdata->vertbuf.buf[0].y = -1.0f ;
  vdata->vertbuf.buf[0].z = -1.0f ;
  vdata->vertbuf.buf[0].rgba = 0x00ff0000 ;

  vdata->vertbuf.buf[1].x =  1.0f ;
  vdata->vertbuf.buf[1].y = -1.0f ;
  vdata->vertbuf.buf[1].z = -1.0f ;
  vdata->vertbuf.buf[1].rgba = 0x0000ff00 ;

  vdata->vertbuf.buf[2].x = -1.0f ;
  vdata->vertbuf.buf[2].y =  1.0f ;
  vdata->vertbuf.buf[2].z = -1.0f ;
  vdata->vertbuf.buf[2].rgba = 0xff000000 ;
}

inline void
shaderDraw (gcmContextData *c, shaderData *vf)
{
/*
  VmathVector4 objEyePos, objLightPos;
  VmathMatrix4 viewMatrix, modelMatrix, modelMatrixIT, modelViewMatrix ;
  VmathPoint3 lightPos;
  lightPos.vec128 = (vec_float4) {250.0f,150.0f,150.0f};
  f32 globalAmbientColor[3] = {0.1f,0.1f,0.1f};
  f32 lightColor[3] = {0.95f,0.95f,0.95f};
  f32 materialColorDiffuse[3] = {0.5f,0.0f,0.0f};
  f32 materialColorSpecular[3] = {0.7f,0.6f,0.6f};
  f32 shininess = 17.8954f;


  rsxLoadVertexProgram ( c, vf->vert.vpo, vf->vert.ucode ) ;
  //rsxSetVertexProgramParameter ( c, vf->vert.vpo, vf->vert.projMatrix, (float*)& ) ;
  rsxSetVertexProgramParameter ( c, vf->vert.vpo, vf->vert.modelViewMatrix, (float*)&modelViewMatrix ) ;

  rsxSetFragmentProgramParameter ( c, vf->frag.fpo, vf->frag.eyePosition, (float*)&objEyePos, vf->frag.offset ) ;
  rsxSetFragmentProgramParameter ( c, vf->frag.fpo, vf->frag.globalAmbient, globalAmbientColor, vf->frag.offset ) ;
  rsxSetFragmentProgramParameter ( c, vf->frag.fpo, vf->frag.lightPosition, (float*)&objLightPos, vf->frag.offset ) ;
  rsxSetFragmentProgramParameter ( c, vf->frag.fpo, vf->frag.lightColor, lightColor, vf->frag.offset ) ;
  rsxSetFragmentProgramParameter ( c, vf->frag.fpo, vf->frag.shininess, &shininess, vf->frag.offset ) ;

  rsxSetFragmentProgramParameter ( c, vf->frag.fpo, vf->frag.Kd, materialColorDiffuse, vf->frag.offset ) ;
  rsxSetFragmentProgramParameter ( c, vf->frag.fpo, vf->frag.Ks, materialColorSpecular, vf->frag.offset ) ;
*/
}

void
buildProjection(float *M, float top, float bottom, float left, float right, float near, float far)
{
  memset(M, 0, 16*sizeof(float)); 

  M[0*4+0] = (2.0f*near) / (right - left);
  M[1*4+1] = (2.0f*near) / (bottom - top);

  float A = (right + left) / (right - left);
  float B = (top + bottom) / (top - bottom);
  float C = -(far + near) / (far - near);
  float D = -(2.0f*far*near) / (far - near);

  M[0*4 + 2] = A;
  M[1*4 + 2] = B;
  M[2*4 + 2] = C;
  M[3*4 + 2] = -1.0f;
  M[2*4 + 3] = D;
}

void
matrixMul(float *Dest, float *A, float *B)
{
  for (int i=0; i < 4; i++)
  {
    for (int j=0; j < 4; j++)
    {
      Dest[i*4+j] = A[i*4+0]*B[0*4+j] 
                  + A[i*4+1]*B[1*4+j] 
                  + A[i*4+2]*B[2*4+j] 
                  + A[i*4+3]*B[3*4+j];
    }
  }
}

void
matrixTranslate(float *M, float x, float y, float z)
{
  memset(M, 0, sizeof(float)*16);
  M[0*4+3] = x;
  M[1*4+3] = y;
  M[2*4+3] = z;

  M[0*4+0] = 1.0f;
  M[1*4+1] = 1.0f;
  M[2*4+2] = 1.0f;
  M[3*4+3] = 1.0f;
}

void
matrixUnit(float *M)
{
  M[0*4+0] = 1.0f;
  M[0*4+1] = 0.0f;
  M[0*4+2] = 0.0f;
  M[0*4+3] = 0.0f;

  M[1*4+0] = 0.0f;
  M[1*4+1] = 1.0f;
  M[1*4+2] = 0.0f;
  M[1*4+3] = 0.0f;

  M[2*4+0] = 0.0f;
  M[2*4+1] = 0.0f;
  M[2*4+2] = 1.0f;
  M[2*4+3] = 0.0f;

  M[3*4+0] = 0.0f;
  M[3*4+1] = 0.0f;
  M[3*4+2] = 0.0f;
  M[3*4+3] = 1.0f;
}

