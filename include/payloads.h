/*
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 */

#ifndef __PAYLOAD_H__
#define __PAYLOAD_H__

#include <ppu-types.h>
#include "common.h"

#define MAX_PAYLOADS 5


typedef struct _payload
{
  char *name;
  char *file;
  int type;
  int descriptor;
} Payload;

/* used to pass data between functions */
typedef struct _payloadData
{
  int idx;
  int doPayload;
  char *path;
  u8 data[PAGE_SIZE];
  Payload payload[MAX_PAYLOADS];
} payloadData;


void payloadsInitialize ( payloadData *p ) ;
int payloadLoadFile ( payloadData *p ) ;
int payloadExecPL3 ( payloadData *p, int size ) ;
int payloadExecGroove ( payloadData *p, int size ) ;
int payloadExecCobra ( payloadData *p, int size ) ;

#endif /* __PAYLOAD_H__ */

