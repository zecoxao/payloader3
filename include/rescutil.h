/*
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 */

#ifndef __RESCUTIL_H__
#define __RESCUTIL_H__

#include <rsx/resc.h>
#include <sysutil/video.h> // videoState, videoResolution, videoGetState, videoGetResolution

#include "video.h"
#include "common.h"

/* initialize rsx */
void rescInitialize (videoData *vdata);
void rescFlipCallback(u32 head);
/* Block the PPU thread untill the previous flip operation has finished. */
void rescWaitFlip (void);
/* Flip a buffer onto the screen. Returns TRUE on success */
int rescFlip (videoData *vdata);
/* Sets the target buffer to render to */
void rescSetRenderTarget(videoData *vdata);


#endif /* __RESCUTIL_H__ */

