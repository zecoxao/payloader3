#line 1 "PANIC.c"
/*
 * PANIC.c
 *
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 *
 */

#include <ppu-lv2.h>
#include <stdbool.h>
#include "firmwares.h"
#include "syscall.h"


/*
 * lv1_panic(true|false) ;
 * true  = reboot panic
 * false = normal panic
 */
static inline void
lv1_panic(bool reboot)
{
  uint64_t syscall_ptr = syscall_peek(SYSCALL_TABLE + 8 * SYSCALL_RUN_PAYLOAD) ;
  syscall_poke (MEM_BASE, 0x38600000396000ffULL | ((unsigned long)reboot << 32)) ;
  syscall_poke (MEM_BASE + 8, 0x4400002200000000ULL) ;

  syscall_poke(syscall_ptr, MEM_BASE) ;
  lv2syscall0 (SYSCALL_RUN_PAYLOAD) ;
}

