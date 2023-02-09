#line 1 "syscall.c"
/*
 * syscall.c
 *
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 *
 */

#include <stdio.h>    // printf ()
#include <ppu-lv2.h>  // lv2syscalln ()
#include <inttypes.h> // PRIx64

#include "common.h"
#include "syscall.h"
#include "firmwares.h"


u64 syscall_peek ( u64 addr )
{
  lv2syscall1 ( SYSCALL_PEEK, addr ) ;
#ifdef DEBUG
  printf ( "syscall_peek:    %d 0x%016" PRIx64 " %016" PRIx64 "\n",
           SYSCALL_PEEK,
           addr,
           p1 ) ;
#endif
  return_to_user_prog ( u64 ) ;
}

void syscall_poke ( u64 addr, u64 value )
{
#ifdef DEBUG
  printf ( "syscall_poke:    %d 0x%016" PRIx64 " %016" PRIx64 "\n",
           SYSCALL_POKE,
           addr,
           value ) ;
#endif
  lv2syscall2 ( SYSCALL_POKE, addr, value ) ;
}

u64 set_syscall ( int syscall, u64 addr )
{
#ifdef DEBUG
  printf ( "set_syscall:   %d 0x%016" PRIx64 "\n",
           syscall,
           addr ) ;
#endif
  u64 syscall_ptr = syscall_peek ( SYSCALL_TABLE + 8 * syscall ) ;
  u64 old_syscall = syscall_peek ( syscall_ptr ) ;

  syscall_poke ( syscall_ptr, addr ) ;

  return old_syscall ;
}

u64 syscall_alloc ( u64 size )
{
#ifdef DEBUG
  printf ( "syscall_alloc:     0x%016" PRIx64 " bytes\n", size ) ;
#endif
  lv2syscall2 ( SYSCALL_ALLOC, size, MEMORY_POOL ) ;
  return_to_user_prog ( u64 ) ;
}

void syscall_free ( u64 addr )
{
#ifdef DEBUG
  printf ( "syscall_free:      0x%016" PRIx64 "\n", addr ) ;
#endif
  lv2syscall2 ( SYSCALL_FREE, addr, MEMORY_POOL ) ;
}

u64 lv2_alloc ( u64 size )
{
#ifdef DEBUG
  printf ( "lv2_alloc:         0x%016" PRIx64 " bytes\n", size ) ;
#endif
  u64 prev = set_syscall ( SYSCALL_ALLOC, ALLOC ) ;
  u64 ret = syscall_alloc ( size ) ;

  set_syscall ( SYSCALL_ALLOC, prev ) ;
  return ret ;
}

void lv2_free ( u64 addr )
{
#ifdef DEBUG
  printf ( "lv2_free:          0x%016" PRIx64 "\n", addr ) ;
#endif
  u64 prev = set_syscall ( SYSCALL_FREE, FREE ) ;

  syscall_free ( addr ) ;

  set_syscall ( SYSCALL_FREE, prev ) ;
}

void syscall_run ( u64 addr )
{
#ifdef DEBUG
  printf ( "syscall_run:       0x%016" PRIx64 "\n", addr ) ;
#endif
  u64 prev = set_syscall ( SYSCALL_RUN_PAYLOAD, addr ) ;

  lv2syscall0 ( SYSCALL_RUN_PAYLOAD ) ;

  set_syscall ( SYSCALL_RUN_PAYLOAD, prev ) ;
}

