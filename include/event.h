/*
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 */

#ifndef __EVENT_H__
#define __EVENT_H__

#include <sys/sem.h>
#include <sys/mutex.h>
#include <sys/cond.h>
#include "common.h"


/* used to pass data between functions */
typedef struct _event_data
{
  int exitapp;
  int xmbopen;
  int menu;

  sys_sem_t sem;
  sys_sem_attr_t sem_attr;

  sys_mutex_t mutex;
  sys_mutex_attr_t mutex_attr;

  sys_cond_t cond;
  sys_cond_attr_t cond_attr;
} eventData;


void eventInitialization(eventData *edata);
void eventHandler (u64 status, u64 param, void *userdata);

#endif /* __EVENT_H__ */

