/*
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 */

#ifndef __AUDIO_H__
#define __AUDIO_H__

#include <sys/mutex.h>
#include <sys/cond.h>
#include <audio/audio.h>

#include "common.h"


/* used to pass data between functions */
typedef struct _audio_data
{
  int *exitapp;
  u64 snd_key;
  sys_event_queue_t snd_queue;
  f32 buf;
  audioPortParam params;
  audioPortConfig config;
  u32 portNum;

  sys_mutex_t mutex;
  sys_mutex_attr_t mutex_attr;

  sys_cond_t cond;
  sys_cond_attr_t cond_attr;
} audioData;


void audioInitialize(audioData *adata);
void audioFinish(audioData *adata);
void audioFillBuffer(f32 *buf);
void audioPlayBlock(audioData *adata);
void audioWorker(void *arg);

#endif /* __AUDIO_H__ */

