#line 1 "rsxutil.c"
/*
 * rsxutil.c
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 */

#include <stdio.h>
#include <malloc.h> // free()
#include <string.h> // memset()
#include <unistd.h> // usleep()

#include "common.h"
#include "video.h"
#include "rsxutil.h"
#include "buffers.h"


/* initialize RSX */
inline void
rsxInitialize (videoData *vdata)
{
  dbgprintf ( "initializing" ) ;

  static int i, ret ;

  vdata->chromakey = CHROMAKEY ;
  vdata->host_addr = memalign ( 1024*1024, HOST_SIZE ) ;

  vdata->context = rsxScreenInitialize ( vdata, HOST_SIZE ) ;
  if ( vdata->context == NULL )
  {
    errprintf ( "ERROR" ) ;
    goto error;
  }

  ret = rsxGetResolution ( &vdata->width, &vdata->height ) ;
  if ( ret != 0 )
  {
    retprintf ( "ERROR" ) ;
    goto error;
  }

  for ( i = 0; i < MAX_BUFFERS; i++ )
  {
    vdata->rsx_buffers[i].width = vdata->width ;
    vdata->rsx_buffers[i].height = vdata->height ;
    rsxMakeBuffer ( &vdata->rsx_buffers[i], i ) ;
  }

  rsxFlip ( vdata ) ;
  rsxSetRenderTarget ( vdata ) ;

  return ;

  error:
  {
    if ( vdata->context )
    {
      rsxFinish ( vdata->context, 0 ) ;
    }

    if ( vdata->host_addr )
    {
      free ( vdata->host_addr ) ;
    }

    errprintf ( "FATAL ERROR EXITING" ) ;
    exit ( EXIT_FAILURE ) ;
  }
}

gcmContextData *
rsxScreenInitialize (videoData *vdata, u32 size)
{
  static int ret;
  static u16 i;
  gcmContextData *context = NULL; /* Context to keep track of the RSX buffer. */
  videoResolution res[12]; /* Screen Resolutions */

  /* screen resolutions / aspects */
  videoResolutionList reslist[] = {
    { RESOLUTION_UNDEFINED, ASPECT_AUTO, 0 },
    { RESOLUTION_1080, ASPECT_AUTO, 0 },
    { RESOLUTION_720, ASPECT_AUTO, 0 },
    { RESOLUTION_576, ASPECT_AUTO, 0 },
    { RESOLUTION_480, ASPECT_AUTO, 0 },

    { RESOLUTION_1080, ASPECT_16_9, 0 },
    { RESOLUTION_720, ASPECT_16_9, 0 },
    { RESOLUTION_576, ASPECT_16_9, 0 },
    { RESOLUTION_480, ASPECT_16_9, 0 },

    { RESOLUTION_576, ASPECT_4_3, 0 },
    { RESOLUTION_480, ASPECT_4_3, 0 },
  };

  /* Initialize RSX context
   * which sets up the command buffer and shared IO memory
   */
  context = rsxInit (CB_SIZE, size, vdata->host_addr);
  if (context == NULL)
  {
    errprintf ( "ERROR" ) ;
    goto error;
  }

  /* find working resolutions */
  for ( i = 1; i <= 10; i++ )
  {
    reslist[i].available = videoGetResolutionAvailability(VIDEO_PRIMARY, reslist[i].resolution, reslist[i].aspect, 0);
#ifdef DEBUG
    printf ("\033[0;33m%s:%s():#%d :: %d:%d :: resolution: %d :: aspect: %d\033[0m\n", __FILE__, __func__, __LINE__, i, reslist[i].available, reslist[i].resolution, reslist[i].aspect);
#endif
  }

  /* Get the state of the display */
  ret = videoGetState (VIDEO_PRIMARY, 0, &vdata->state) ;
  if ( ret != 0 )
  {
    retprintf ( "ERROR" ) ;
    goto error;
  }

  /* verify display state */
  if (vdata->state.state != 0)
  {
    errprintf ( "ERROR" ) ;
    goto error;
  }
  else
  {
    /* define the current state / resolution / aspect */
    reslist[0].available = VIDEO_STATE_ENABLED;
    reslist[0].resolution = vdata->state.displayMode.resolution;
    reslist[0].aspect = vdata->state.displayMode.aspect;
  }

  /* fill res[] array */
  for ( i = 0; i <= 10; i++ )
  {
    if (reslist[i].available == VIDEO_STATE_ENABLED)
    {
      ret = videoGetResolution (reslist[i].resolution, &res[i]) ;
      if ( ret != 0 )
      {
        retprintf ( "ERROR" ) ;
        goto error;
      }
    }
  }

  /* clear video configuration */
  memset (&vdata->vconfig, 0, (size_t)sizeof(videoConfiguration));

  /* Configure the buffer format to xRGB */
  vdata->vconfig.format = VIDEO_BUFFER_FORMAT_XRGB;

  /* 720 */
  if (reslist[2].available != VIDEO_STATE_DISABLED)
  {
    vdata->vconfig.resolution = reslist[2].resolution;
    vdata->vconfig.aspect = reslist[2].aspect;
    vdata->vconfig.pitch = res[2].width * sizeof(u32);
  }
  /* 576 */
  else if (reslist[4].available != VIDEO_STATE_DISABLED)
  {
    vdata->vconfig.resolution = reslist[4].resolution;
    vdata->vconfig.aspect = reslist[4].aspect;
    vdata->vconfig.pitch = res[4].width * sizeof(u32);
  }
  /* 480 */
  else if (reslist[3].available != VIDEO_STATE_DISABLED)
  {
    vdata->vconfig.resolution = reslist[3].resolution;
    vdata->vconfig.aspect = reslist[3].aspect;
    vdata->vconfig.pitch = res[3].width * sizeof(u32);
  }
  /* default is current settings */
  else
  {
    vdata->vconfig.resolution = reslist[0].resolution;
    vdata->vconfig.aspect = reslist[0].aspect;
    vdata->vconfig.pitch = res[0].width * sizeof(u32);
  }

  /* wait for RSX command buffer to be idle to continue */
  rsxWaitIdle(context);

  /* configure video */
  ret = videoConfigure (VIDEO_PRIMARY, &vdata->vconfig, NULL, 0) ;
  if ( ret != 0 )
  {
    retprintf ( "ERROR" ) ;
    goto error;
  }

  /* verify video state */
  ret = videoGetState (VIDEO_PRIMARY, 0, &vdata->state) ;
  if ( ret != 0 )
  {
    retprintf ( "ERROR" ) ;
    goto error;
  }

  /* Wait for VSYNC to flip */
  gcmSetFlipMode (GCM_FLIP_VSYNC);

  /* get current width / height */
  ret = rsxGetResolution ( &vdata->width, &vdata->height );
  if ( ret != 0 )
  {
    retprintf ( "ERROR" ) ;
    goto error;
  }

  vdata->color_pitch = vdata->width * sizeof(u32);
  vdata->depth_pitch = vdata->width * sizeof(u32);

  vdata->depth_buffer = (u32 *) rsxMemalign (64, (vdata->height * vdata->depth_pitch) * 2);
  if (vdata->depth_buffer == NULL)
  {
    errprintf ( "ERROR" ) ;
    goto error;
  }
#ifdef DEBUG
  else
  {
    printf ( "\033[0;33m%s:%s():#%d :: [depth_buffer] rsxMemalign ( 64, 0x%x ) = 0x%llx\033[0m\n", __FILE__, __func__, __LINE__, (vdata->height * vdata->depth_pitch) * 2, (long long unsigned int)vdata->depth_buffer ) ;
  }
#endif

  ret = rsxAddressToOffset (vdata->depth_buffer, &vdata->depth_offset);
  if ( ret != 0 )
  {
    retprintf ( "ERROR" ) ;
    goto error;
  }
#ifdef DEBUG
  else
  {
    printf ( "\033[0;33m%s:%s():#%d :: [depth_offset] rsxAddressToOffset ( 0x%llx, 0x%x )\033[0m\n", __FILE__, __func__, __LINE__, (long long unsigned int)vdata->depth_buffer, (unsigned int)vdata->depth_offset ) ;
  }
#endif

  gcmResetFlipStatus();

  return context;

  error:
  {
    if (context)
    {
      rsxFinish (context, 0);
    }
  
    if (vdata->host_addr)
    {
      free (vdata->host_addr);
    }

    return NULL;
  }
}

/* create an RSX buffer */
int
rsxMakeBuffer (rsxBuffer *buf, int id)
{
  static int ret ;

  /* populate buffer variables */
  buf->id = id ;
  buf->color_depth = sizeof(u32) ;
  buf->color_pitch = buf->width * buf->color_depth ;
  buf->size = buf->width * buf->height * buf->color_depth ;
  buf->chromakey = CHROMAKEY ;

  buf->ptr = (uint32_t*) rsxMemalign (64, buf->size);
  if (buf->ptr == NULL)
  {
    errprintf ( "ERROR" ) ;
    goto error;
  }
#ifdef DEBUG
  else
  {
    printf ( "\033[0;33m%s:%s():#%d :: buffer[%d] rsxMemalign ( 64, 0x%x ) = 0x%llx\033[0m\n", __FILE__, __func__, __LINE__, id, buf->size, (long long unsigned int)buf->ptr ) ;
  }
#endif

  ret = rsxAddressToOffset (buf->ptr, &buf->color_offset) ;
  if ( ret != 0 )
  {
    retprintf ( "ERROR" ) ;
    goto error;
  }
#ifdef DEBUG
  else
  {
    printf ( "\033[0;33m%s:%s():#%d :: buffer[%d] rsxAddressToOffset ( 0x%llx, 0x%x )\033[0m\n", __FILE__, __func__, __LINE__, id, (long long unsigned int)buf->ptr, (unsigned int)buf->color_offset ) ;
  }
#endif

  /* Register the display buffer with the RSX */
  ret = gcmSetDisplayBuffer (id, buf->color_offset, buf->color_pitch, buf->width, buf->height) ;
  if ( ret != 0 )
  {
    retprintf ( "ERROR" ) ;
    goto error;
  }

  return TRUE;

  error:
  {
    if (buf->ptr != NULL)
    {
      rsxFree (buf->ptr);
    }

    return FALSE;
  }
}

/* get current screen resolution */
int
rsxGetResolution (u16 *width, u16 *height)
{
  videoState state;
  videoResolution res;

  /* Get the state of the display and resolution */
  if ( videoGetState ( 0, 0, &state ) == 0 &&
       videoGetResolution ( state.displayMode.resolution, &res ) == 0 )
  {
    if ( width )
    {
      *width = res.width;
    }
    if ( height )
    {
      *height = res.height;
    }
    return 0 ;
  }
  return -1 ;
}

/* wait for last flip to complete */
inline void 
rsxWaitFlip () 
{ 
  while (gcmGetFlipStatus () != 0)
    usleep (100);  /* Sleep, to not stress the cpu. */
  gcmResetFlipStatus ();
}     
      
/* flip a buffer to screen */
inline int
rsxFlip ( videoData *vdata )
{
  if (gcmSetFlip (vdata->context, vdata->rsx_buffers[vdata->currentBuffer].id ) == 0)
  {
    rsxFlushBuffer (vdata->context);
    /* Prevent the RSX from continuing until the flip has finished. */
    gcmSetWaitFlip (vdata->context);

    /* switch buffers */
    vdata->currentBuffer = !vdata->currentBuffer ;

    /* set new buffer as target */
    rsxSetRenderTarget ( vdata ) ;

    return TRUE;
  }
  return FALSE;
}
  
/* wait for the RSX command buffer to be idle */
inline void
rsxWaitIdle(gcmContextData *context)
{       
  static u32 sLabelVal = 1;

  rsxSetWriteBackendLabel (context, GCM_LABEL_INDEX, sLabelVal);
  rsxSetWaitLabel (context, GCM_LABEL_INDEX, sLabelVal);

  sLabelVal++;

  rsxSetWriteBackendLabel (context, GCM_LABEL_INDEX, sLabelVal);

  rsxFlushBuffer (context);

  while(*(vu32 *) gcmGetLabelAddress (GCM_LABEL_INDEX) != sLabelVal)
  {
    usleep(30);
  }
}

/* clear an RSX buffer */
inline void
rsxClearBuffer ( gcmContextData *context )
{
  rsxSetClearColor ( context, 0x00000000 ) ;
  rsxSetClearDepthValue ( context, 0x00000000 ) ;
  rsxClearSurface ( context, GCM_CLEAR_Z|GCM_CLEAR_R|GCM_CLEAR_G|GCM_CLEAR_B|GCM_CLEAR_A ) ;
}

/* rsx blending */
inline void
rsxBlend ( gcmContextData *context )
{
  rsxSetBlendFunc ( context, GCM_SRC_ALPHA, GCM_ONE_MINUS_SRC_ALPHA, GCM_SRC_ALPHA, GCM_ONE_MINUS_SRC_ALPHA ) ;
  rsxSetBlendEquation ( context, GCM_FUNC_ADD, GCM_FUNC_ADD ) ;
  rsxSetBlendEnable ( context, GCM_TRUE ) ;
}

/* set the current render target */
inline void
rsxSetRenderTarget(videoData *vdata )
{
  gcmSurface sf;

  /* clear surface struct */
  memset ( &sf, 0, sizeof ( gcmSurface ) ) ;

  sf.colorFormat      = GCM_TF_COLOR_A8R8G8B8;
  sf.colorTarget      = GCM_TF_TARGET_0;
  sf.colorLocation[0] = GCM_LOCATION_RSX;
  sf.colorOffset[0]   = vdata->rsx_buffers[vdata->currentBuffer].color_offset;
  sf.colorPitch[0]    = vdata->color_pitch;

  sf.colorLocation[1] = GCM_LOCATION_RSX;
  sf.colorOffset[1]   = 0;
  sf.colorPitch[1]    = 64;

  sf.colorLocation[2] = GCM_LOCATION_RSX;
  sf.colorOffset[2]   = 0;
  sf.colorPitch[2]    = 64;

  sf.colorLocation[3] = GCM_LOCATION_RSX;
  sf.colorOffset[3]   = 0;
  sf.colorPitch[3]    = 64;

  sf.depthFormat      = GCM_TF_ZETA_Z16;
  sf.depthLocation    = GCM_LOCATION_RSX;
  sf.depthOffset      = vdata->depth_offset;
  sf.depthPitch       = vdata->depth_pitch;

  sf.type             = GCM_TF_TYPE_LINEAR;
  sf.antiAlias 	      = GCM_TF_CENTER_1;

  sf.width            = vdata->rsx_buffers[vdata->currentBuffer].width;
  sf.height           = vdata->rsx_buffers[vdata->currentBuffer].height;
  sf.x                = 0;
  sf.y                = 0;

  /* set buffer surface */
  rsxSetSurface (vdata->context, &sf);

  /* clear new buffer */
  rsxClearBuffer ( vdata->context ) ;

  /* set pixel blending */
  rsxBlend ( vdata->context ) ;

  /* wait for command buffer to be idle */
  rsxWaitIdle ( vdata->context ) ;
}
