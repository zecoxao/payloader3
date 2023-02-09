#line 1 "video.c"
/*
 * video.c
 *
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 *
 */

#include <stdio.h>
#include <time.h>
#include <sys/sem.h>
#include <sys/mutex.h>
#include <sys/cond.h>
#include <sys/thread.h>           /* for threads */

#include "common.h"
#include "video.h"
#include "rsxutil.h"
#include "buffers.h"
#include "ps3console.h"


void
videoInitialize ( videoData *vdata )
{
  dbgprintf ( "initializing" ) ;

  /* initialize semaphore attributes */
  vdata->sem_attr.key            = 0x01 ;
  vdata->sem_attr.attr_protocol  = SYS_SEM_ATTR_PROTOCOL ;
  vdata->sem_attr.attr_pshared   = SYS_SEM_ATTR_PSHARED ;

  /* initialize mutex attributes */
  vdata->mutex_attr.key              = 0x01 ;
  vdata->mutex_attr.attr_protocol    = SYS_MUTEX_PROTOCOL_FIFO ;
  vdata->mutex_attr.attr_pshared     = SYS_MUTEX_ATTR_NOT_PSHARED ;
  vdata->mutex_attr.attr_recursive   = SYS_MUTEX_ATTR_RECURSIVE ;
  vdata->mutex_attr.attr_adaptive    = SYS_MUTEX_ATTR_ADAPTIVE ;

  /* initialize condition attributes */
  vdata->cond_attr.key               = 0x01 ;
  vdata->cond_attr.attr_pshared      = SYS_COND_ATTR_PSHARED ;

  /* create semaphore */
  sysSemCreate ( &vdata->sem, &vdata->sem_attr, 1, SEM_CONSUMERS ) ;

  /* create mutex */
  sysMutexCreate ( &vdata->mutex, &vdata->mutex_attr ) ;

  /* create cond */
  sysCondCreate ( &vdata->cond, vdata->mutex, &vdata->cond_attr ) ;

  /* initialize rsx context and buffers */
  rsxInitialize ( vdata ) ;
}

void
videoFinish ( videoData *vdata )
{
  dbgprintf ( "video finishing" ) ;

  static int i ;
  /* make sure no flip is in progress */
  gcmSetWaitFlip ( vdata->context ) ;

  /* free rsx buffers */
  for ( i = 0; i < MAX_BUFFERS; i++ )
  {
    rsxFree ( vdata->rsx_buffers[i].ptr ) ;
  }

  /* finish rsx context */
  rsxFinish ( vdata->context, 1 ) ;

  /* free host_addr memory */
  free ( vdata->host_addr ) ;

  /* destroy semaphore */
  sysSemDestroy ( vdata->sem ) ;

  /* destroy cond */
  sysCondDestroy ( vdata->cond ) ;

  /* destroy mutex */
  sysMutexDestroy ( vdata->mutex ) ;
}

inline void
videoDrawFrame ( starsData *sdata, videoData *vdata )
{
  /* wait vsync */
  rsxWaitFlip () ;

  /* update stars */
  starsDrawRightRsx ( sdata, &vdata->rsx_buffers[vdata->currentBuffer] ) ;

  /* update text */
  sysSemWait ( vdata->txt_buffer->sem, NO_TIMEOUT ) ;
  txtBufferDraw ( vdata->context, vdata->txt_buffer, &vdata->rsx_buffers[vdata->currentBuffer] ) ;
  sysSemPost ( vdata->txt_buffer->sem, 1 ) ;

  /* flip buffer to screen */
  rsxFlip ( vdata ) ;
  //rescFlip ( vdata ) ;
}

/* video worker thread */
void
videoWorker ( void *arg )
{
  static int frames ;
  static time_t starttime ;
  dbgprintf ( "video thread starting" ) ;

  starsData sdata ;

  /* cast the void *arg to rsxData */
  videoData* vdata = ( videoData* )arg;

  /* initialize randomly generated stars */
  sysSemWait ( vdata->sem, NO_TIMEOUT ) ;
  sdata.width = vdata->width ;
  sdata.height = vdata->height ;
  sysSemPost ( vdata->sem, 1 ) ;

  starsInitialize ( &sdata ) ;

  /* signal main thread is ready */
  sysCondSignal ( vdata->cond ) ;

  /* lock mutex */
  sysMutexLock ( vdata->mutex, NO_TIMEOUT ) ;

  dbgprintf ( "video thread waiting" ) ;

  /* wait for main to be ready */
  sysCondWait ( vdata->cond, NO_TIMEOUT ) ;

  /* release lock */
  sysMutexUnlock ( vdata->mutex ) ;

  starttime = time( NULL ) ;

  dbgprintf ( "video thread entering loop" ) ;

  /* render frames until exit */
  while ( *vdata->exitapp )
  {
    /* render frame */
    videoDrawFrame ( &sdata, vdata ) ;
    frames++;
  }

  dbgprintf ( "video thread left loop" ) ;

  /* flip black screen */
  rsxWaitFlip () ;
  rsxClearBuffer ( vdata->context ) ;
  rsxFlip ( vdata ) ;

  /* lock mutex */
  sysMutexLock ( vdata->mutex, NO_TIMEOUT ) ;

  /* signal main before exit */
  sysCondSignal ( vdata->cond ) ;

  /* release lock */
  sysMutexUnlock ( vdata->mutex ) ;

#ifdef DEBUG
  printf ( "\033[0;33m%s:%s():#%d :: frame rate: %g frames/sec\033[0m\n", __FILE__, __func__, __LINE__, ( ( double ) frames ) / difftime ( time ( NULL ) , starttime ) ) ;
  dbgprintf ( "video thread exiting" ) ;
#endif

  /* exit thread */
  sysThreadExit ( 0 ) ;
}

