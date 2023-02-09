/*
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 */

#ifndef __TOOLS_H__
#define __TOOLS_H__

#include "common.h"

#define MAX_TOOLS 5


typedef struct _tool
{
  char *name;
  char *file;
} Tool;

/* used to pass data between functions */
typedef struct _tool_data
{
  int idx;
  char *path;
  char file[256];
  Tool tool[MAX_TOOLS];
} toolData;


void toolsInitialize(toolData *t);

#endif /* __TOOLS_H__ */

