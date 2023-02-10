#line 1 "event.c"
/*
 * event.c
 *
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 *
 */

#include <stdio.h>
#include <sys/sem.h>
#include <sys/mutex.h>
#include <sys/cond.h>
#include <sysutil/sysutil.h>      // sysUtilRegisterCallback, sysUtilCheckCallback

#include "common.h"
#include "event.h"


/* init event handler */
void
eventInitialization ( eventData *edata )
{
  dbgprintf ( "initializing" ) ;

  /* initialize variables */
  edata->menu = 1 ;
  edata->exitapp = 1 ;
  edata->xmbopen = 0 ;

  /* initialize semaphore attributes */
  edata->sem_attr.key            = 0x03 ;
  edata->sem_attr.attr_protocol  = SYS_SEM_ATTR_PROTOCOL ;
  edata->sem_attr.attr_pshared   = SYS_SEM_ATTR_PSHARED ;

  /* initialize mutex attributes */
  edata->mutex_attr.key              = 0x03 ;
  edata->mutex_attr.attr_protocol    = SYS_MUTEX_PROTOCOL_FIFO ;
  edata->mutex_attr.attr_pshared     = SYS_MUTEX_ATTR_PSHARED ;
  edata->mutex_attr.attr_recursive   = SYS_MUTEX_ATTR_RECURSIVE ;
  edata->mutex_attr.attr_adaptive    = SYS_MUTEX_ATTR_ADAPTIVE ;

  /* initialize condition attributes */
  edata->cond_attr.key               = 0x03 ;
  edata->cond_attr.attr_pshared      = SYS_COND_ATTR_PSHARED ;

  /* create semaphore */
  sysSemCreate ( &edata->sem, &edata->sem_attr, 1, SEM_CONSUMERS ) ;

  /* create mutex */
  sysMutexCreate ( &edata->mutex, &edata->mutex_attr ) ;

  /* create cond */
  sysCondCreate ( &edata->cond, edata->mutex, &edata->cond_attr ) ;

  /* register eventHandler */
  sysUtilRegisterCallback ( SYSUTIL_EVENT_SLOT0, eventHandler, &edata ) ;
}

/* event handler for in-game XMB */
inline void
eventHandler ( u64 status, u64 param, void *userdata )
{
  eventData* event = (eventData*)userdata;

  switch ( status )
  {
    case SYSUTIL_EXIT_GAME:
      event->exitapp = 0 ;
      break ;
    case SYSUTIL_MENU_OPEN:
      event->xmbopen = 1 ;
      break ;
    case SYSUTIL_MENU_CLOSE:
      event->xmbopen = 0 ;
      break ;
  }
}

