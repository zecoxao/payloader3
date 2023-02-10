#line 1 "audio.c"
/*
 * audio.c
 *
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 *
 */

#include <stdio.h>

#include <sys/mutex.h>
#include <sys/cond.h>
#include <sys/thread.h>           /* for threads */

#include "common.h"
#include "audio.h"

#include "register_echo_raw.h"


/* initialize audio */
inline void 
audioInitialize ( audioData *adata )
{
  dbgprintf ( "audio initializing" ) ;

  /* initialize mutex attributes */
  adata->mutex_attr.key            = 0x02 ;
  adata->mutex_attr.attr_protocol  = SYS_MUTEX_PROTOCOL_FIFO ;
  adata->mutex_attr.attr_pshared   = SYS_MUTEX_ATTR_PSHARED ;
  adata->mutex_attr.attr_recursive = SYS_MUTEX_ATTR_RECURSIVE ;
  adata->mutex_attr.attr_adaptive  = SYS_MUTEX_ATTR_ADAPTIVE ;

  /* initialize condition attributes */
  adata->cond_attr.key           = 0x02 ;   
  adata->cond_attr.attr_pshared  = SYS_COND_ATTR_PSHARED ;

  /* create mutex */
  sysMutexCreate ( &adata->mutex, &adata->mutex_attr ) ;

  /* create cond */
  sysCondCreate ( &adata->cond, adata->mutex, &adata->cond_attr ) ;

  /* initialize audio data */
  adata->params.numChannels = AUDIO_PORT_2CH;
  adata->params.numBlocks = AUDIO_BLOCK_8;
  adata->params.attrib = AUDIO_PORT_INITLEVEL;
  adata->params.level = 1.0f;

  /* initialize audio */
  audioInit ();
  audioPortOpen ( &adata->params, &adata->portNum );
  audioGetPortConfig ( adata->portNum, &adata->config );
  audioCreateNotifyEventQueue ( &adata->snd_queue, &adata->snd_key );
  audioSetNotifyEventQueue ( adata->snd_key );
  sysEventQueueDrain ( adata->snd_queue );
  audioPortStart ( adata->portNum );
}

/* stop audio */
inline void
audioFinish ( audioData *adata )
{
  dbgprintf ( "audio finishing" ) ;

  /* stop audio */
  audioPortStop ( adata->portNum );
  audioRemoveNotifyEventQueue ( adata->snd_key );
  audioPortClose ( adata->portNum );
  sysEventQueueDestroy ( adata->snd_queue, 0 );
  audioQuit ();

  /* destroy cond */
  sysCondDestroy ( adata->cond ) ;

  /* destroy mutex */
  sysMutexDestroy ( adata->mutex ) ;
}

/* play block of audio data from buffer */
void
audioPlay ( audioData *adata )
{
  static u32 i ;
  static u32 pos ;
  static s32 ret ;
  f32 *buf;
  sys_event_t event;

  u64 current_block = *( u64* )( ( u64 )adata->config.readIndex );

  f32 *dataStart = ( f32* )( ( u64 )adata->config.audioDataStart );

  u32 audio_block_index = ( current_block + 1 ) % adata->config.numBlocks;

  ret = sysEventQueueReceive ( adata->snd_queue, &event, 20 * 1000 );

  buf = dataStart + adata->config.channelCount * AUDIO_BLOCK_SAMPLES * audio_block_index;

  for ( i = 0 ; i < AUDIO_BLOCK_SAMPLES ; i++ )
  {
    buf[ i * 2 + 0 ] = ( f32 )*( ( s16* )&register_echo_raw[ pos ] ) / 32768.0f;
    buf[ i * 2 + 1 ] = ( f32 )*( ( s16* )&register_echo_raw[ pos + 2 ] ) / 32768.0f;

    pos += 4;
    if ( pos >= register_echo_raw_size ) pos = 0;
  }
}

/* audio worker thread */
void
audioWorker ( void *arg )
{
  dbgprintf ( "audio thread starting" ) ;

  /* cast the void *arg to modulesData */
  audioData* adata = ( audioData* )arg;

  /* signal main thread is ready */
  sysCondSignal ( adata->cond ) ;

  /* lock mutex */
  sysMutexLock ( adata->mutex, NO_TIMEOUT ) ;

  dbgprintf ( "audio thread waiting" ) ;

  /* wait for main to be ready */
  sysCondWait ( adata->cond, NO_TIMEOUT ) ;

  /* release lock */
  sysMutexUnlock ( adata->mutex ) ;

  dbgprintf ( "audio thread entering loop" ) ;

  /* play audio until exit */
  while( *adata->exitapp )
  {
    /* play audio */
    audioPlay ( adata );
  }

  dbgprintf ( "audio thread left loop" ) ;

  /* lock mutex */
  sysMutexLock ( adata->mutex, NO_TIMEOUT ) ;

  /* signal main before exit */
  sysCondSignal ( adata->cond ) ;

  /* release lock */
  sysMutexUnlock ( adata->mutex ) ;

  dbgprintf ( "audio thread exiting" ) ;

  sysThreadExit ( 0 ) ;
}

