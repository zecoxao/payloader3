/*
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 */

#ifndef SYSCALL_H
#define SYSCALL_H

#include "common.h"

#define SYSCALL_PEEK		6
#define SYSCALL_POKE		7
#define SYSCALL_RUN_PAYLOAD	200
#define SYSCALL_ALLOC		201
#define SYSCALL_FREE		202
#define MEMORY_POOL		0x27


u64 syscall_peek (u64 addr);
void syscall_poke (u64 addr, u64 value);
u64 set_syscall (int syscall, u64 addr);
u64 syscall_alloc (u64 size);
void syscall_free (u64 addr);
u64 lv2_alloc (u64 size);
void lv2_free (u64 addr);
void syscall_run (u64 addr);

#endif

