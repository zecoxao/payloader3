/*
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 */

#ifndef __VIDEO_H__
#define __VIDEO_H__

#include <sys/sem.h>
#include <sys/mutex.h>
#include <sys/cond.h>
#include <rsx/rsx.h>
#include <rsx/resc.h>
#include <sysutil/video.h>

#include "common.h"
#include "buffers.h"

#define MAX_BUFFERS                     2


/* used to pass data between functions */
typedef struct _video_data
{
  gcmContextData *context;
  videoState state;
  videoConfiguration vconfig;
  videoResolution res;

  rescInitConfig rconfig;
  rescSrc src;
  rescDsts dsts;
  u32 bufMode;

  colorBuffer cbuf;
  vertexArrayBuffer vertbuf;
  fragmentShaderBuffer fragbuf;

  rsxBuffer rsx_buffers[MAX_BUFFERS] ;
  void *host_addr;
  u16 currentBuffer;

  u16 width;
  u16 height;

  u32 color_pitch;

  u32 depth_pitch;
  u32 depth_offset;
  u32 *depth_buffer;

  uint32_t chromakey;

  txtBuffer *txt_buffer;

  int *exitapp;

  sys_sem_t sem;
  sys_sem_attr_t sem_attr;

  sys_mutex_t mutex;
  sys_mutex_attr_t mutex_attr;

  sys_cond_t cond;
  sys_cond_attr_t cond_attr;
} videoData;

#include "rsxutil.h"
#include "rescutil.h"
#include "stars.h"


void videoInitialize(videoData *vdata);
void videoFinish(videoData *vdata);
void videoDrawFrame(starsData *sdata, videoData *vdata);
void videoWorker(void *arg);

#endif /* __VIDEO_H__ */

