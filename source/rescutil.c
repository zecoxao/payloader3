#line 1 "rescutil.c"
/*
 * rescutil.c
 *
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 *
 */

#include <stdio.h>
#include <malloc.h> // free()
#include <string.h> // memset()
#include <unistd.h> // usleep()

#include <rsx/resc.h>


#include "common.h"
#include "video.h"
#include "rescutil.h"
#include "buffers.h"


static bool flipped = true ;

/* initialize resc */
void
rescInitialize ( videoData *vdata )
{
  dbgprintf ( "initializing" ) ;

  int ret ;

  /* Get the state of the display */
  ret = videoGetState (VIDEO_PRIMARY, 0, &vdata->state) ;
  if (ret != 0)
  {
    retprintf( "ERROR" ) ;
    goto error;
  }

  /* Make sure display is enabled */
  if (vdata->state.state != VIDEO_STATE_DISABLED)
  {
    errprintf( "ERROR" ) ;
    goto error;
  }

  /* initialize resc configuration */
  memset (&vdata->rconfig, 0, (size_t)sizeof(rescInitConfig));

  vdata->rconfig.size = sizeof ( rescInitConfig ) ;
  vdata->rconfig.resourcePolicy = RESC_MINIMUM_GPU_LOAD | RESC_CONSTANT_VRAM ;
  vdata->rconfig.supportModes = RESC_1280x720 | RESC_720x480 ;
  vdata->rconfig.ratioMode = RESC_LETTERBOX ;
  vdata->rconfig.palTemporalMode = RESC_PAL_50 ;
  vdata->rconfig.interlaceMode = RESC_NORMAL_BILINEAR ;
  vdata->rconfig.flipMode = RESC_DISPLAY_VSYNC ;

  ret = rescInit ( &vdata->rconfig ) ;
  if ( ret != 0 ) ;
  {
    retprintf( "ERROR" ) ;
    goto error;
  }

  //rescVideoResolution2RescBufferMode ( vdata->state.displayMode.resolution, &vdata->bufMode ) ;
  //rescVideoResolution2RescBufferMode ( VIDEO_RESOLUTION_720, &vdata->bufMode ) ;
  vdata->bufMode = RESC_1280x720 ;

  vdata->dsts.format = RESC_SURFACE_A8R8G8B8 ;
  vdata->dsts.pitch = 0x1400 ;
  vdata->dsts.heightAlign = 64 ;

  ret = rescSetDsts ( vdata->bufMode, &vdata->dsts ) ;
  if ( ret != 0 ) ;
  {
    retprintf( "ERROR" ) ;
    goto error;
  }

  ret = rescSetDisplayMode ( vdata->bufMode ) ;
  if ( ret != 0 ) ;
  {
    retprintf( "ERROR" ) ;
    goto error;
  }

  ret = rescGetBufferSize ( &vdata->cbuf.size, &vdata->vertbuf.size, &vdata->fragbuf.size ) ;
  if ( ret != 0 ) ;
  {
    retprintf( "ERROR" ) ;
    goto error;
  }

  vdata->cbuf.ptr = (void*) rsxMemalign ( 0x10000, vdata->cbuf.size ) ;
  vdata->vertbuf.ptr = (void*) rsxMemalign ( 0x80, vdata->vertbuf.size ) ;
  vdata->fragbuf.ptr = (void*) rsxMemalign ( 0x80, vdata->fragbuf.size ) ;

  if ( vdata->cbuf.ptr == NULL || vdata->vertbuf.ptr == NULL || vdata->fragbuf.ptr == NULL ) ;
  {
    errprintf ( "ERROR" ) ;
    printf("Required size,  color:0x%x vertex:0x%x fragment:0x%x\n", vdata->cbuf.size, vdata->vertbuf.size, vdata->fragbuf.size);
    printf("Buf addr given, color:0x%p vertex:0x%p fragment:0x%p\n", vdata->cbuf.ptr, vdata->vertbuf.ptr, vdata->fragbuf.ptr);
    goto error;
  }

  ret = rescSetBufferAddress ( vdata->cbuf.ptr, vdata->vertbuf.ptr, vdata->fragbuf.ptr ) ;
  if ( ret != 0 ) ;
  {
    retprintf( "ERROR" ) ;
    goto error;
  }

  printf("Required size,  color:0x%x vertex:0x%x fragment:0x%x\n", vdata->cbuf.size, vdata->vertbuf.size, vdata->fragbuf.size);
  printf("Buf addr given, color:0x%p vertex:0x%p fragment:0x%p\n", vdata->cbuf.ptr, vdata->vertbuf.ptr, vdata->fragbuf.ptr);

  rescSetFlipHandler ( rescFlipCallback ) ;

  rescSetRenderTarget ( vdata ) ;
  
  error:
  {
    errprintf( "ERROR" ) ;
    return ;
  }
}

/* rescFlip callback */
void
rescFlipCallback ( u32 head )
{
  printf("rescFlipCallback()\n");
  //(void)head ;
  flipped = true ;
}

/* wait until flip */
void
rescWaitFlip ( void )
{
  printf("rescWaitFlip()\n");
  while (!flipped)
  {
    usleep ( 100 ) ;
  }
  flipped = false ;
}

/* flip a buffer to screen */
inline int
rescFlip ( videoData *vdata )
{
  //printf("rescFlip()\n");
  if ( rescSetConvertAndFlip ( vdata->context, vdata->rsx_buffers[vdata->currentBuffer].id ) == 0 )
  {
    rsxFlushBuffer ( vdata->context ) ;

    /* Prevent the RSX from continuing until the flip has finished. */
    rescSetWaitFlip ( vdata->context ) ;

    return TRUE;
  }
  return FALSE;
}
  
/* set the current render target */
inline void
rescSetRenderTarget(videoData *vdata )
{
  printf("rescSetRenderTarget()\n");
  gcmSurface sf;
  rescSrc src;

  sf.colorFormat = GCM_TF_COLOR_A8R8G8B8;
  sf.colorTarget = GCM_TF_TARGET_0;
  sf.colorLocation[0] = GCM_LOCATION_RSX;
  sf.colorOffset[0] = vdata->rsx_buffers[vdata->currentBuffer].color_offset;
  sf.colorPitch[0] = vdata->color_pitch;

  sf.colorLocation[1] = GCM_LOCATION_RSX;
  sf.colorLocation[2] = GCM_LOCATION_RSX;
  sf.colorLocation[3] = GCM_LOCATION_RSX;
  sf.colorOffset[1] = 0;
  sf.colorOffset[2] = 0;
  sf.colorOffset[3] = 0;
  sf.colorPitch[1] = 64;
  sf.colorPitch[2] = 64;
  sf.colorPitch[3] = 64;

  sf.depthFormat = GCM_TF_ZETA_Z16;
  sf.depthLocation = GCM_LOCATION_RSX;
  sf.depthOffset = vdata->depth_offset;
  sf.depthPitch = vdata->depth_pitch;

  sf.type = GCM_TF_TYPE_LINEAR;
  sf.antiAlias 	= GCM_TF_CENTER_1;

  sf.width = vdata->rsx_buffers[vdata->currentBuffer].width;
  sf.height = vdata->rsx_buffers[vdata->currentBuffer].height;
  sf.x = 0;
  sf.y = 0;

  rsxSetSurface ( vdata->context, &sf ) ;

  rescGcmSurface2RescSrc ( &sf, &src ) ;
  rescSetSrc ( vdata->currentBuffer, &src ) ;
}

