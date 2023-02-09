#line 1 "main.c"
/*
 * payloader3 loader
 *
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 *
 */

#include <stdio.h>                /* */
#include <malloc.h>               /* */
#include <string.h>               /* */
#include <assert.h>               /* */
#include <time.h>                 /* */
#include <unistd.h>               /* */

#include <sys/process.h>          /* for ps3load / ethdebug */
#include <sys/cond.h>             /* for conditionals */
#include <sys/sem.h>              /* for semaphores */
#include <sys/mutex.h>            /* for mutexes */
#include <sys/thread.h>           /* for threads */
#include <sys/types.h>            /* */

#include <sysutil/sysutil.h>      /* sysUtilRegisterCallback, sysUtilCheckCallback */
#include <sysmodule/sysmodule.h>  /*  */

#include "common.h"
#include "video.h"
#include "buffers.h"
#include "audio.h"
#include "ps3console.h"
#include "image.h"
#include "buttons.h"
#include "shader.h"
#include "texture.h"
#include "stars.h"
#include "pad.h"
#include "payloads.h"
#include "tools.h"
#include "syscall.h"


#define PAYLOADER_VERSION         "v0.05"
#define PAYLOADER_TITLE           "payloader3"

u32 module_flag ;

SYS_PROCESS_PARAM( 1001, 0x100000 ) ;

/* unload sysmodules at exit */
void
unload_modules () 
{
  if ( module_flag & 7 )
  {
    sysModuleUnload ( SYSMODULE_FS ) ;
  }

  if ( module_flag & 6 )
  {
    sysModuleUnload ( SYSMODULE_IO ) ;
  }

  if ( module_flag & 5 ) 
  {
    sysModuleUnload ( SYSMODULE_AUDIO ) ;
  }

  if ( module_flag & 4 ) 
  {
    sysModuleUnload ( SYSMODULE_GCM_SYS ) ;
  }

  if ( module_flag & 3 ) 
  {
    sysModuleUnload ( SYSMODULE_RESC ) ;
  }
/*
  if ( module_flag & 2 ) 
  {
    sysModuleUnload ( SYSMODULE_FREETYPE ) ;
  }
*/
  if ( module_flag & 1 ) 
  {
    sysModuleUnload ( SYSMODULE_PNGDEC ) ;
  }
}

/*
 *
 * MAIN
 *
 */
s32 main ( s32 argc, const char* argv[] )
{
  static int loaded, ret, size ;

  /* ps3console variables */
  int line = 140 ;
  char ts[256] ;

  /* thread variables */
  u64 priority = 1500 ;
  size_t stacksize = 0x100000 ;
  sys_ppu_thread_t aud_thread_id ;
  sys_ppu_thread_t vid_thread_id ;

  /* data structs */
  payloadData payloads ;
  toolData tools ;
  buttonsData btn_data ;
  eventData evt_data ;
  padBtnData pad_data ;
  shaderData shd_data ;
  textureData tex_data ;
  audioData aud_data ;
  videoData vid_data ;
  txtBuffer txt_buffer ;

  /*
   * initialization begin
   */
  dbgprintf ( "main initializing" ) ;

  /* register unload_modules for execution at type exit */
  if ( atexit ( unload_modules ) != 0 )
  {
    errprintf ( "cannot set exit function" ) ;
    exit ( EXIT_FAILURE ) ;
  }

  /* load pngdec sysmodule */
  if ( sysModuleLoad ( SYSMODULE_PNGDEC ) != 0 )
  {
    errprintf ( "cannot load SYSMODULE_PNGDEC" ) ;
    exit ( EXIT_FAILURE ) ;
  }
  else
  {
    module_flag |= 1 ;
  }

  /* load freetype sysmodule */
/*
  if ( sysModuleLoad ( SYSMODULE_FREETYPE) != 0 )
  {
    printf ( "cannot load SYSMODULE_FREETYPE\n" ) ;
    exit ( EXIT_FAILURE ) ;
  }
  else
  {
    module_flag |= 2 ;
  }
*/
  /* load resc sysmodule */
  if ( sysModuleLoad ( SYSMODULE_RESC ) != 0 )
  {
    errprintf ( "cannot load SYSMODULE_RESC" ) ;
    exit ( EXIT_FAILURE ) ;
  }
  else
  {
    module_flag |= 3 ;
  }

  /* load gcm_sys sysmodule */
  if ( sysModuleLoad ( SYSMODULE_GCM_SYS ) !=0 ) 
  {
    errprintf ( "cannot load SYSMODULE_GCM_SYS" ) ;
    exit ( EXIT_FAILURE ) ;
  }
  else
  {
    module_flag |= 4 ;
  }

  /* load audio sysmodule */
  if ( sysModuleLoad ( SYSMODULE_AUDIO ) != 0 ) 
  {
    errprintf ( "cannot load SYSMODULE_AUDIO" ) ;
    exit ( EXIT_FAILURE ) ;
  }
  else
  {
    module_flag |= 5 ;
  }

  /* load io sysmodule */
  if ( sysModuleLoad ( SYSMODULE_IO ) != 0 ) 
  {
    errprintf ( "cannot load SYSMODULE_IO" ) ;
    exit ( EXIT_FAILURE ) ;
  }
  else
  {
    module_flag |= 6 ;
  }

  /* load fs sysmodule */
  if ( sysModuleLoad ( SYSMODULE_FS ) != 0 )   
  {
    errprintf ( "cannot load SYSMODULE_FS" ) ;
    exit ( EXIT_FAILURE ) ;
  }
  else
  {
    module_flag |= 7 ;
  }

  /* register eventHandler */
  eventInitialization ( &evt_data ) ;

  /* initialize audio */
  audioInitialize ( &aud_data ) ;

  /* lock audio mutex */
  sysMutexLock ( aud_data.mutex, NO_TIMEOUT ) ;

  /* create audio player thread */
  sysThreadCreate ( &aud_thread_id, audioWorker, (void *)&aud_data,
                    priority + 10, stacksize, THREAD_JOINABLE, "audioWorker" ) ;

  /* wait for audio thread to be ready */
  sysCondWait ( aud_data.cond, NO_TIMEOUT ) ;

  /* release audio lock */
  sysMutexUnlock ( aud_data.mutex ) ;

  /* initialize video */
  videoInitialize ( &vid_data ) ;

  /* lock video mutex */
  sysMutexLock ( vid_data.mutex, NO_TIMEOUT ) ;

  /* create video thread */
  sysThreadCreate ( &vid_thread_id, videoWorker, (void *)&vid_data,
                    priority + 20, stacksize, THREAD_JOINABLE, "videoWorker" ) ;

  /* wait for video thread to be ready */
  sysCondWait ( vid_data.cond, NO_TIMEOUT ) ;

  /* release lock */
  sysMutexUnlock ( vid_data.mutex ) ;

  /* initialize ps3console settings */
  sysSemWait ( vid_data.sem, TIMEOUT ) ;
  ps3consoleInitialize ( FONT_COLOR_BG, FONT_COLOR_FG, FONT_SIZE, vid_data.width, vid_data.height ) ;
  sysSemPost ( vid_data.sem, 1 ) ;

  /* pointer to txt_buffer for video thread */
  sysSemWait ( txt_buffer.sem, TIMEOUT ) ;
  sysSemWait ( vid_data.sem, TIMEOUT ) ;
  vid_data.txt_buffer = &txt_buffer ;
  sysSemPost ( vid_data.sem, 1 ) ;
  sysSemPost ( txt_buffer.sem, 1 ) ;

  /* width / height for txt_buffer */
  sysSemWait ( txt_buffer.sem, TIMEOUT ) ;
  sysSemWait ( vid_data.sem, TIMEOUT ) ;
  txt_buffer.width = vid_data.width ;
  txt_buffer.height = vid_data.height ;
  sysSemPost ( vid_data.sem, 1 ) ;
  sysSemPost ( txt_buffer.sem, 1 ) ;

  /* initialize txt_buffer */
  txtBufferInitialize ( &vid_data ) ;

  /* initialize payloads */
  payloadsInitialize ( &payloads ) ;

  /* initialize tools */
  toolsInitialize ( &tools ) ;

  /* initialize controller io */
  padInitialize ( &pad_data ) ;

  /* initialize shader */
  shaderInitialize ( &shd_data ) ;

  /* initialize resc */
  rescInitialize ( &vid_data ) ;

  /* initialize textures */
  textureInitialize ( vid_data.context, &tex_data, 0 ) ;
  textureInitialize ( vid_data.context, &tex_data, 1 ) ;
  textureInitialize ( vid_data.context, &tex_data, 2 ) ;
  textureInitialize ( vid_data.context, &tex_data, 3 ) ;
  textureInitialize ( vid_data.context, &tex_data, 4 ) ;
  textureInitialize ( vid_data.context, &tex_data, 5 ) ;

  /* png bitmaps */
  imageInitialization ( &tex_data ) ;

  /* initialize button images */
  buttonsInitialize ( &tex_data.texture[0].data, &btn_data ) ;

  /* initialize exitapp pointers */
  aud_data.exitapp = &evt_data.exitapp ;

  sysSemWait ( vid_data.sem, TIMEOUT ) ;
  vid_data.exitapp = &evt_data.exitapp ;
  sysSemPost ( vid_data.sem, 1 ) ;

  /*
   *  initialization complete
   */

  /* signal audio thread to start */
  sysMutexLock ( aud_data.mutex, NO_TIMEOUT ) ;
  dbgprintf ( "signalling audio" ) ;
  sysCondSignal ( aud_data.cond ) ;
  sysMutexUnlock ( aud_data.mutex ) ;

  /* signal video thread to start */
  sysMutexLock ( vid_data.mutex, NO_TIMEOUT ) ;
  dbgprintf ( "signalling video" ) ;
  sysCondSignal ( vid_data.cond ) ;
  sysMutexUnlock ( vid_data.mutex ) ;

  /*
   *  start rendering to payloads screen
   */

  /* draw images into buffer */
  sysSemWait ( txt_buffer.sem, TIMEOUT ) ;
  imageDrawPngTxt ( 1, 0, 25, &txt_buffer, &tex_data.texture[1].data ) ;   /* logo */
  imageDrawPngTxt ( 1, 0, 119, &txt_buffer, &tex_data.texture[2].data ) ;  /* bar */
  imageDrawPngTxt ( 0, 50, vid_data.height - 135, &txt_buffer, &tex_data.texture[3].data ) ; /* truck */
  imageDrawPngTxt ( 1, 0, vid_data.height - 130, &txt_buffer, &tex_data.texture[4].data ) ; /* PL3 logo */
  imageDrawPngTxt ( 2, 75, vid_data.height - 200, &txt_buffer, &tex_data.texture[5].data ) ;  /* transformer */
  sysSemPost ( txt_buffer.sem, 1 ) ;

  /* header text */
  dbgprintf ( "["PAYLOADER_TITLE" "PAYLOADER_VERSION"]::" ) ;
  sprintf ( ts, "::[%s %s]::", PAYLOADER_TITLE, PAYLOADER_VERSION ) ;
  sysSemWait ( txt_buffer.sem, TIMEOUT ) ;
  print ( LINE_INDENT, line, ts, &txt_buffer ) ;
  sysSemPost ( txt_buffer.sem, 1 ) ;

  /* payload selections */
  sprintf ( ts, ": %s (%s)", payloads.payload[PAD_TRIANGLE].name, payloads.payload[PAD_TRIANGLE].file ) ;
  buttonDraw ( vid_data.width - LINE_INDENT - 13, line += LINE_SIZE - 13, &txt_buffer, &btn_data, BUTTON_TRIANGLE ) ;
  print ( LINE_INDENT + 15, line += 13, ts, &txt_buffer ) ;

  sprintf ( ts, ": %s (%s)", payloads.payload[PAD_CIRCLE].name, payloads.payload[PAD_CIRCLE].file ) ;
  buttonDraw ( vid_data.width - LINE_INDENT - 13, line += LINE_SIZE - 13, &txt_buffer, &btn_data, BUTTON_CIRCLE ) ;
  print ( LINE_INDENT + 15, line += 13, ts, &txt_buffer ) ;

  sprintf ( ts, ": %s (%s)", payloads.payload[PAD_CROSS].name, payloads.payload[PAD_CROSS].file ) ;
  buttonDraw( vid_data.width - LINE_INDENT - 13, line += LINE_SIZE - 13, &txt_buffer, &btn_data, BUTTON_CROSS ) ;
  print ( LINE_INDENT + 15, line += 13, ts, &txt_buffer ) ;

  sprintf ( ts, ": %s (%s)", payloads.payload[PAD_SQUARE].name, payloads.payload[PAD_SQUARE].file ) ;
  buttonDraw ( vid_data.width - LINE_INDENT - 13, line += LINE_SIZE - 13, &txt_buffer, &btn_data, BUTTON_SQUARE ) ;
  print ( LINE_INDENT + 15, line += 13, ts, &txt_buffer ) ;

  /* wait at first screen for user input */
  while ( evt_data.menu && evt_data.exitapp )
  {
    /* check controller input */
    padCheckPayloads ( &payloads, &evt_data, &pad_data ) ;

    /* check if XMB */
    sysUtilCheckCallback () ;
  }

  /* exit if user quit via xmb */
  if ( evt_data.exitapp == 0 )
  {
    goto exit;
  }

  /*
   *  start rendering payload deploy and tools
   */

  /* clear screen */
  sysSemWait ( txt_buffer.sem, TIMEOUT ) ;
  txtBufferClear ( &txt_buffer ) ;
  sysSemPost ( txt_buffer.sem, 1 ) ;

  /* reset line count */
  line = 140 ;

  /* draw images into buffer */
  sysSemWait ( txt_buffer.sem, TIMEOUT ) ;
  imageDrawPngTxt ( 1, 0, 25, &txt_buffer, &tex_data.texture[1].data ) ;   /* logo */
  imageDrawPngTxt ( 1, 0, 119, &txt_buffer, &tex_data.texture[2].data ) ;  /* bar */
  imageDrawPngTxt ( 0, 50, vid_data.height - 135, &txt_buffer, &tex_data.texture[3].data ) ; /* truck */
  imageDrawPngTxt ( 1, 0, vid_data.height - 130, &txt_buffer, &tex_data.texture[4].data ) ; /* PL3 */
  imageDrawPngTxt ( 2, 75, vid_data.height - 200, &txt_buffer, &tex_data.texture[5].data ) ;  /* transformer */
  sysSemPost ( txt_buffer.sem, 1 ) ;

  /* header text */
  dbgprintf ( "["PAYLOADER_TITLE" "PAYLOADER_VERSION"]::" ) ;
  sprintf ( ts, "::[%s %s]::", PAYLOADER_TITLE, PAYLOADER_VERSION ) ;
  sysSemWait ( txt_buffer.sem, TIMEOUT ) ;
  print ( LINE_INDENT, line, ts, &txt_buffer ) ;
  sysSemPost ( txt_buffer.sem, 1 ) ;

  /* begin payload deploy code */
  if ( payloads.doPayload == 1 )
  {
    printf ( "\033[0;33mLoading payload: %s\033[0m\n", payloads.payload[payloads.idx].file ) ;
    sprintf ( ts, "Loading payload: %s", payloads.payload[payloads.idx].file ) ;
    print ( LINE_INDENT, line += LINE_SIZE, ts, &txt_buffer ) ;

    size = payloadLoadFile ( &payloads ) ;
    if ( size > 0 )
    {
      printf ( "\033[0;32mSUCCESS: Loaded %s\033[0m\n", payloads.payload[payloads.idx].file ) ;
      sprintf ( ts, "SUCCESS: Loaded %s", payloads.payload[payloads.idx].file ) ;
      print ( LINE_INDENT, line += LINE_SIZE, ts, &txt_buffer ) ;
      loaded = 1 ;
    } else {
      printf ( "\033[0;31m%s:%s():#%d :: ERROR: Loading %s\033[0m\n", __FILE__, __func__, __LINE__, payloads.payload[payloads.idx].file ) ;
      sprintf ( ts, "ERROR: Loading %s", payloads.payload[payloads.idx].name ) ;
      print ( LINE_INDENT, line += LINE_SIZE, ts, &txt_buffer ) ;
      loaded = 0 ;
    }

    if ( size + payloads.payload[payloads.idx].descriptor < PAGE_SIZE - 1 && loaded == 1 )
    {
      printf ( "\033[0;33mPoking 0x%04x ( %d ) bytes into memory\033[0m\n", size, size ) ;
      sprintf ( ts, "Poking 0x%04x ( %d ) bytes into memory", size, size ) ;
      print ( LINE_INDENT, line += LINE_SIZE, ts, &txt_buffer ) ;

      switch ( payloads.payload[payloads.idx].type )
      {
        case 0:
          ret = payloadExecPL3 ( &payloads, size ) ;
          break ;
        case 1:
          ret = payloadExecGroove ( &payloads, size ) ;
          break ;
        case 2:
          ret = payloadExecCobra ( &payloads, size ) ;
          break ;
        default:
          ret = 1 ;
          break;
      }

      if ( ret == 0 )
      {
        printf ( "\033[0;32mSUCCESS: Poked %s\033[0m\n", payloads.payload[payloads.idx].file ) ;
        sprintf ( ts, "SUCCESS: Poked %s", payloads.payload[payloads.idx].name ) ;
        print ( LINE_INDENT, line += LINE_SIZE, ts, &txt_buffer ) ;

        dbgprintf ( "Payload now resident" ) ;
        sprintf ( ts, "Payload now resident" ) ;
        print ( LINE_INDENT, line += LINE_SIZE, ts, &txt_buffer ) ;
      } else {
        printf ( "\033[0;31m%s:%s():#%d :: ERROR: Executing %s\033[0m\n", __FILE__, __func__, __LINE__, payloads.payload[payloads.idx].file ) ;
        sprintf ( ts, "ERROR: Executing %s", payloads.payload[payloads.idx].name ) ;
        print ( LINE_INDENT, line += LINE_SIZE, ts, &txt_buffer ) ;
      }
    }
  }
  else
  {
    dbgprintf ( "Payload skipped" ) ;
    sprintf ( ts, "Payload skipped..." ) ;
    print ( LINE_INDENT, line += LINE_SIZE, ts, &txt_buffer ) ;
  }
  /* end payload deploy code */

  /*
   *  start rendering tools
   */

  /* tools selections */
  sprintf ( ts, ": %s (%s)", tools.tool[PAD_TRIANGLE].name, tools.tool[PAD_TRIANGLE].file ) ;
  buttonDraw( vid_data.width - LINE_INDENT - 13, line += LINE_SIZE - 13, &txt_buffer, &btn_data, BUTTON_TRIANGLE ) ;
  print ( LINE_INDENT + 15, line += 13, ts, &txt_buffer ) ;

  sprintf ( ts, ": %s (%s)", tools.tool[PAD_CIRCLE].name, tools.tool[PAD_CIRCLE].file ) ;
  buttonDraw( vid_data.width - LINE_INDENT - 13, line += LINE_SIZE - 13, &txt_buffer, &btn_data, BUTTON_CIRCLE ) ;
  print ( LINE_INDENT + 15, line += 13, ts, &txt_buffer ) ;

  sprintf ( ts, ": %s (%s)", tools.tool[PAD_CROSS].name, tools.tool[PAD_CROSS].file ) ;
  buttonDraw( vid_data.width - LINE_INDENT - 13, line += LINE_SIZE - 13, &txt_buffer, &btn_data, BUTTON_CROSS ) ;
  print ( LINE_INDENT + 15, line += 13, ts, &txt_buffer ) ;

  sprintf ( ts, ": %s (%s)", tools.tool[PAD_SQUARE].name, tools.tool[PAD_SQUARE].file ) ;
  buttonDraw( vid_data.width - LINE_INDENT - 13, line += LINE_SIZE - 13, &txt_buffer, &btn_data, BUTTON_SQUARE ) ;
  print ( LINE_INDENT + 15, line += 13, ts, &txt_buffer ) ;

  /* reset menu variable */
  evt_data.menu = 1 ;

  /* wait at second screen for user input */
  while ( evt_data.menu && evt_data.exitapp )
  {
    /* check controller input */
    padCheckTools ( &tools, &evt_data, &pad_data ) ;

    /* check if XMB */
    sysUtilCheckCallback () ;
  }

  /*
   *  finished close threads and exit
   */

  exit:
  {
    dbgprintf ( "main stopping" ) ;

    /* toggle exitapp so threads quit */
    evt_data.exitapp = 0 ;

    /* lock mutex */
    sysMutexLock ( vid_data.mutex, NO_TIMEOUT ) ;

    /* wait for video to exit */
    sysCondWait ( vid_data.cond, NO_TIMEOUT ) ;

    /* release lock */
    sysMutexUnlock ( vid_data.mutex ) ;

    /* free buttons image data */
    buttonsFinish( &btn_data ) ;

    /* free txt_buffer resources */
    txtBufferFinish ( &txt_buffer ) ;

    /* close pads */
    padFinish () ;

    /* free audio resources */
    audioFinish ( &aud_data ) ;

    /* free video resources */
    videoFinish ( &vid_data );

    /* spawn user selected tool */
    if ( tools.idx == PAD_TRIANGLE || tools.idx == PAD_CIRCLE || tools.idx == PAD_SQUARE )
    {
      printf ( "\033[0;33m\nspawning %s\033[0m\n", tools.file ) ;
      sysProcessExitSpawn2 ( tools.file,
                             NULL, NULL, NULL, 0, 1001,
                             SYS_PROCESS_SPAWN_STACK_SIZE_1M ) ;
    }

#ifdef DEBUG
    sysProcessExitSpawn2 ( "/dev_hdd0/game/PSL145310/ps3load.self",
                            NULL, NULL, NULL, 0, 1001,
                            SYS_PROCESS_SPAWN_STACK_SIZE_1M ) ;
#endif

    dbgprintf ( "main exiting" ) ;

    exit ( EXIT_SUCCESS ) ;
  }
}

