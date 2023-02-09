/*
 * Copyright (C) Youness Alaoui (KaKaRoTo)
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 */

#ifndef __RSXUTIL_H__
#define __RSXUTIL_H__

#include <sysutil/video.h> // videoState, videoResolution, videoGetState, videoGetResolution
#include <rsx/rsx.h> // gcmGetFlipStatus, gcmResetFlipStatus, rsxMemalign, rsxAddressToOffset, gcmSetDisplayBuffer, rsxFree

#include "common.h"
#include "video.h"
#include "buffers.h"

#define CB_SIZE                        0x100000
#define HOST_SIZE                      (32*1024*1024)
#define VIDEO_PRIMARY                  0
#define VIDEO_RESOLUTION_UNDEFINED     0
#define GCM_LABEL_INDEX                255


typedef enum _videoResolutions
{
  RESOLUTION_UNDEFINED  = VIDEO_RESOLUTION_UNDEFINED,
  RESOLUTION_1080       = VIDEO_RESOLUTION_1080,
  RESOLUTION_720        = VIDEO_RESOLUTION_720,
  RESOLUTION_480        = VIDEO_RESOLUTION_480,
  RESOLUTION_576        = VIDEO_RESOLUTION_576
} videoResolutions;

typedef enum _videoAspects
{
  ASPECT_AUTO		= VIDEO_ASPECT_AUTO,
  ASPECT_4_3		= VIDEO_ASPECT_4_3,
  ASPECT_16_9		= VIDEO_ASPECT_16_9 
} videoAspects;

typedef struct _videoResolutionList
{
  videoResolutions resolution;
  videoAspects aspect;
  int available;
} videoResolutionList;


/* initialize rsx */
void rsxInitialize (videoData *vdata);
/* Initilize the RSX properly. Returns NULL on error */
gcmContextData *rsxScreenInitialize (videoData *vdata, u32 size);
/* Create a buffer to draw into and assign it to @id. Returns NULL on error */
int rsxMakeBuffer (rsxBuffer * buffer, int id);
/* Get current screen resolution. returns TRUE on success */
int rsxGetResolution (u16 *width, u16 *height);
/* wait for RSX to be idle */
void rsxWaitIdle(gcmContextData *context);
/* clear rsx buffer */
void rsxClearBuffer (gcmContextData *context);
/* rsx blending */
void rsxBlend(gcmContextData *context);
/* Block the PPU thread untill the previous flip operation has finished. */
void rsxWaitFlip (void);
/* Flip a buffer onto the screen. Returns TRUE on success */
int rsxFlip (videoData *vdata);
/* Sets the target buffer to render to */
void rsxSetRenderTarget(videoData *vdata);

/* lazy psl1ght maintainers left this function out */
extern s32 videoGetResolutionAvailability(u32 videoOut, u32 resolutionId, u32 aspect, u32 option);


#endif /* __RSXUTIL_H__ */

