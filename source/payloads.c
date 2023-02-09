#line 1 "payloads.c"
/*
 * payloads.c
 *
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 *
 */

#include <stdio.h>
#include <inttypes.h> /* for PRIx64 */
#include "common.h"
#include "payloads.h"
#include "pad.h"
#include "syscall.h"


/* initialize payloads struct values */
inline void
payloadsInitialize ( payloadData *p )
{
  dbgprintf ( "initializing" ) ;

  /* define path to files on PS3 */
  p->path = "/dev_hdd0/game/payloader/USRDIR/payloads" ;

  /*
   * list of payload files presented to user
   *
   * name = text to be displayed to user
   * file = filename found at the path defined above
   */
  #ifdef FIRMWARE_3_41
    p->payload[PAD_TRIANGLE].name = "PL3 3.41 CEX (app_home/ethernet tty)" ;
    p->payload[PAD_TRIANGLE].file = "payload_dev_3_41.bin" ;
    p->payload[PAD_TRIANGLE].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_TRIANGLE].type = 0 ;
/*
    p->payload[PAD_CIRCLE].name = "PL3 3.41 (app_home/no unauth syscall)" ;
    p->payload[PAD_CIRCLE].file = "payload_no_unauth_syscall_3_41.bin" ;
    p->payload[PAD_CIRCLE].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_CIRCLE].type = 0 ;
*/
    p->payload[PAD_CIRCLE].name = "cobrav2 3.41 CEX" ;
    p->payload[PAD_CIRCLE].file = "payload_groove_cobrav2.bin" ;
    p->payload[PAD_CIRCLE].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_CIRCLE].type = 2 ;
    p->payload[PAD_CROSS].name = "PL3 3.41 CEX (app_home)" ;
    p->payload[PAD_CROSS].file = "default_payload_3_41.bin" ;
    p->payload[PAD_CROSS].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_CROSS].type = 0 ;
    p->payload[PAD_SQUARE].name = "psgroove 3.41 CEX" ;
    p->payload[PAD_SQUARE].file = "payload_groove_hermes.bin" ;
    p->payload[PAD_SQUARE].descriptor = 0x00 ;
    p->payload[PAD_SQUARE].type = 1 ;
  #elif defined ( FIRMWARE_3_41_KIOSK )
    p->payload[PAD_TRIANGLE].name = "PL3 3.41 KIOSK (app_home/ethernet tty)" ;
    p->payload[PAD_TRIANGLE].file = "payload_dev_3_41_kiosk.bin" ;
    p->payload[PAD_TRIANGLE].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_TRIANGLE].type = 0 ;
    p->payload[PAD_CIRCLE].name = "PL3 3.41 KIOSK (app_home/no unauth syscall)" ;
    p->payload[PAD_CIRCLE].file = "payload_no_unauth_syscall_3_41_kiosk.bin" ;
    p->payload[PAD_CIRCLE].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_CIRCLE].type = 0 ;
    p->payload[PAD_CROSS].name = "PL3 3.41 KIOSK (app_home)" ;
    p->payload[PAD_CROSS].file = "default_payload_3_41_kiosk.bin" ;
    p->payload[PAD_CROSS].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_CROSS].type = 0 ;
    p->payload[PAD_SQUARE].name = "PL3 dump lv2" ;
    p->payload[PAD_SQUARE].file = "dump_lv2.bin" ;
    p->payload[PAD_SQUARE].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_SQUARE].type = 0 ;
  #elif defined ( FIRMWARE_3_41_DEX )
    p->payload[PAD_TRIANGLE].name = "PL3 3.41 DEX (app_home/ethernet tty)" ;
    p->payload[PAD_TRIANGLE].file = "payload_dev_3_41_dex.bin" ;
    p->payload[PAD_TRIANGLE].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_TRIANGLE].type = 0 ;
    p->payload[PAD_CIRCLE].name = "PL3 3.41 DEX (app_home/no unauth syscall)" ;
    p->payload[PAD_CIRCLE].file = "payload_no_unauth_syscall_3_41_dex.bin" ;
    p->payload[PAD_CIRCLE].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_CIRCLE].type = 0 ;
    p->payload[PAD_CROSS].name = "PL3 3.41 DEX (app_home)" ;
    p->payload[PAD_CROSS].file = "default_payload_3_41_dex.bin" ;
    p->payload[PAD_CROSS].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_CROSS].type = 0 ;
    p->payload[PAD_SQUARE].name = "PL3 dump lv2" ;
    p->payload[PAD_SQUARE].file = "dump_lv2.bin" ;
    p->payload[PAD_SQUARE].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_SQUARE].type = 0 ;
  #elif defined ( FIRMWARE_3_40 )
    p->payload[PAD_TRIANGLE].name = "PL3 3.40 CEX (app_home/ethernet tty)" ;
    p->payload[PAD_TRIANGLE].file = "payload_dev_3_40.bin" ;
    p->payload[PAD_TRIANGLE].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_TRIANGLE].type = 0 ;
    p->payload[PAD_CIRCLE].name = "PL3 3.40 CEX (app_home/no unauth syscall)" ;
    p->payload[PAD_CIRCLE].file = "payload_no_unauth_syscall_3_40.bin" ;
    p->payload[PAD_CIRCLE].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_CIRCLE].type = 0 ;
    p->payload[PAD_CROSS].name = "PL3 3.40 CEX (app_home)" ;
    p->payload[PAD_CROSS].file = "default_payload_3_40.bin" ;
    p->payload[PAD_CROSS].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_CROSS].type = 0 ;
    p->payload[PAD_SQUARE].name = "PL3 dump lv2" ;
    p->payload[PAD_SQUARE].file = "dump_lv2.bin" ;
    p->payload[PAD_SQUARE].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_SQUARE].type = 0 ;
  #elif defined ( FIRMWARE_3_30 )
    p->payload[PAD_TRIANGLE].name = "PL3 3.30 CEX (app_home/ethernet tty)" ;
    p->payload[PAD_TRIANGLE].file = "payload_dev_3_30.bin" ;
    p->payload[PAD_TRIANGLE].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_TRIANGLE].type = 0 ;
    p->payload[PAD_CIRCLE].name = "PL3 3.30 CEX (app_home/no unauth syscall)" ;
    p->payload[PAD_CIRCLE].file = "payload_no_unauth_syscall_3_30.bin" ;
    p->payload[PAD_CIRCLE].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_CIRCLE].type = 0 ;
    p->payload[PAD_CROSS].name = "PL3 3.30 CEX (app_home)" ;
    p->payload[PAD_CROSS].file = "default_payload_3_30.bin" ;
    p->payload[PAD_CROSS].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_CROSS].type = 0 ;
    p->payload[PAD_SQUARE].name = "PL3 dump lv2" ;
    p->payload[PAD_SQUARE].file = "dump_lv2.bin" ;
    p->payload[PAD_SQUARE].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_SQUARE].type = 0 ;
  #elif defined ( FIRMWARE_3_21 )
    p->payload[PAD_TRIANGLE].name = "PL3 3.21 CEX (app_home/ethernet tty)" ;
    p->payload[PAD_TRIANGLE].file = "payload_dev_3_21.bin" ;
    p->payload[PAD_TRIANGLE].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_TRIANGLE].type = 0 ;
    p->payload[PAD_CIRCLE].name = "PL3 3.21 CEX (app_home/no unauth syscall)" ;
    p->payload[PAD_CIRCLE].file = "payload_no_unauth_syscall_3_21.bin" ;
    p->payload[PAD_CIRCLE].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_CIRCLE].type = 0 ;
    p->payload[PAD_CROSS].name = "PL3 3.21 CEX (app_home)" ;
    p->payload[PAD_CROSS].file = "default_payload_3_21.bin" ;
    p->payload[PAD_CROSS].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_CROSS].type = 0 ;
    p->payload[PAD_SQUARE].name = "PL3 dump lv2" ;
    p->payload[PAD_SQUARE].file = "dump_lv2.bin" ;
    p->payload[PAD_SQUARE].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_SQUARE].type = 0 ;
  #elif defined ( FIRMWARE_3_15 )
    p->payload[PAD_TRIANGLE].name = "PL3 3.15 CEX (app_home/ethernet tty)" ;
    p->payload[PAD_TRIANGLE].file = "payload_dev_3_15.bin" ;
    p->payload[PAD_TRIANGLE].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_TRIANGLE].type = 0 ;
    p->payload[PAD_CIRCLE].name = "PL3 3.15 CEX (app_home/no unauth syscall)" ;
    p->payload[PAD_CIRCLE].file = "payload_no_unauth_syscall_3_15.bin" ;
    p->payload[PAD_CIRCLE].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_CIRCLE].type = 0 ;
    p->payload[PAD_CROSS].name = "PL3 3.15 CEX (app_home)" ;
    p->payload[PAD_CROSS].file = "default_payload_3_15.bin" ;
    p->payload[PAD_CROSS].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_CROSS].type = 0 ;
    p->payload[PAD_SQUARE].name = "PL3 dump lv2" ;
    p->payload[PAD_SQUARE].file = "dump_lv2.bin" ;
    p->payload[PAD_SQUARE].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_SQUARE].type = 0 ;
  #elif defined ( FIRMWARE_3_10 )
    p->payload[PAD_TRIANGLE].name = "PL3 3.10 CEX (app_home/ethernet tty)" ;
    p->payload[PAD_TRIANGLE].file = "payload_dev_3_10.bin" ;
    p->payload[PAD_TRIANGLE].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_TRIANGLE].type = 0 ;
    p->payload[PAD_CIRCLE].name = "PL3 3.10 CEX (app_home/no unauth syscall)" ;
    p->payload[PAD_CIRCLE].file = "payload_no_unauth_syscall_3_10.bin" ;
    p->payload[PAD_CIRCLE].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_CIRCLE].type = 0 ;
    p->payload[PAD_CROSS].name = "PL3 3.10 CEX (app_home)" ;
    p->payload[PAD_CROSS].file = "default_payload_3_10.bin" ;
    p->payload[PAD_CROSS].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_CROSS].type = 0 ;
    p->payload[PAD_SQUARE].name = "PL3 dump lv2" ;
    p->payload[PAD_SQUARE].file = "dump_lv2.bin" ;
    p->payload[PAD_SQUARE].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_SQUARE].type = 0 ;
  #elif defined ( FIRMWARE_3_01 )
    p->payload[PAD_TRIANGLE].name = "PL3 3.01 CEX (app_home/ethernet tty)" ;
    p->payload[PAD_TRIANGLE].file = "payload_dev_3_01.bin" ;
    p->payload[PAD_TRIANGLE].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_TRIANGLE].type = 0 ;
    p->payload[PAD_CIRCLE].name = "PL3 3.01 CEX (app_home/no unauth syscall)" ;
    p->payload[PAD_CIRCLE].file = "payload_no_unauth_syscall_3_01.bin" ;
    p->payload[PAD_CIRCLE].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_CIRCLE].type = 0 ;
    p->payload[PAD_CROSS].name = "PL3 3.01 CEX (app_home)" ;
    p->payload[PAD_CROSS].file = "default_payload_3_01.bin" ;
    p->payload[PAD_CROSS].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_CROSS].type = 0 ;
    p->payload[PAD_SQUARE].name = "PL3 dump lv2" ;
    p->payload[PAD_SQUARE].file = "dump_lv2.bin" ;
    p->payload[PAD_SQUARE].descriptor = USB_DESCRIPTOR ;
    p->payload[PAD_SQUARE].type = 0 ;
  #endif
}

/* load payload file */
int
payloadLoadFile ( payloadData *p )
{
  char file[256];

  sprintf ( file, "%s/%s", p->path, p->payload[p->idx].file ) ;
  FILE *payload = fopen ( file, "rb" ) ;
  if ( payload == NULL )
  {
    return 0 ;
  }

  fseek ( payload, 0, SEEK_END ) ;
  size_t size = ftell ( payload ) ;

  if ( size + p->payload[p->idx].descriptor > PAGE_SIZE - 1 )
  {
    printf ( "%s too big: 0x%04x > 0x%04x\n",
              file,
              ( uint ) size + p->payload[p->idx].descriptor,
              PAGE_SIZE - 1 ) ;

    fclose ( payload ) ;

    return 0 ;
  }

  printf ( "%s size: 0x%04x ( %d ) \n",
            p->payload[p->idx].file,
            ( uint ) size,
            ( uint ) size ) ;

  fseek ( payload, 0, SEEK_SET ) ;
  fread ( p->data, 1, size, payload ) ;
  fclose ( payload ) ;

  return size ;
}

/* execute payload file */
int payloadExecPL3 ( payloadData *p, int size )
{
  int i, alloc_size ;
  static u64 ptr ;

  printf ( "payloadExecPL3:    0x%04x ( %d ) bytes\n", size, size ) ;

  /* Make sure we allocate a full page. */
  if ( size + p->payload[p->idx].descriptor < PAGE_SIZE - 1 )
  {
    alloc_size = PAGE_SIZE ;
    ptr = lv2_alloc ( alloc_size ) ;
  }
  else
  {
    printf ( "\npayload too big: 0x%04x > 0x%04x\n",
              size + p->payload[p->idx].descriptor,
              PAGE_SIZE - 1 ) ;
    return 1 ;
  }

  printf ( "poke payload:      0x%016" PRIx64 "\n", ptr ) ;

  /* poke payload into memory ptr */
  for ( i = 0 ; i < size ; i+=8 )
  {
    syscall_poke ( ptr + i + p->payload[p->idx].descriptor,
                 ( ( u64 * ) ( p->data + i ) ) [0] ) ;
    __asm__ volatile ( "sync" ) ;
  }

  /* execute payload */
  printf ( "execute payload:   0x%016" PRIx64 "\n", ptr + p->payload[p->idx].descriptor ) ;
  syscall_run ( ptr + p->payload[p->idx].descriptor ) ;

  /* free memory allocated */
  lv2_free ( ptr ) ;

  return 0 ;
}

/* hermes mess */
int payloadExecGroove ( payloadData *p, int size )
{
  int i ;
  static u64 ptr = 0x80000000007e0000ULL ;

  printf ( "payloadExecGroove: 0x%04x ( %d ) bytes\n", size, size ) ;

  /* Make sure we allocate a full page. */
  if ( size > PAGE_SIZE - 1 )
  {
    printf ( "\npayload too big: 0x%04x > 0x%04x\n", size, PAGE_SIZE - 1 ) ;
    return 1 ;
  }

  printf ( "poke payload:      0x%016" PRIx64 "\n", ptr ) ;

  /* poke payload into memory ptr */
  for(i = 0; i < size; i += 8)
  {
    syscall_poke (ptr + i, ( ( u64 * ) ( p->data + i ) ) [0] );
    __asm__ volatile ("sync");
  }

  /* syscall 9 enable */
  syscall_poke (0x8000000000017CE0ULL, 0x7C6903A64E800420ULL);
  __asm__ volatile ("sync");

  /* execute payload */
  printf ( "execute payload:   0x%016" PRIx64 "\n", ptr ) ;
  lv2syscall8(9, (u64)ptr, 0,0,0,0,0,0,0);

  return 0;
}

/* hermes mess */
int payloadExecCobra ( payloadData *p, int size )
{
  int i ;
  static u64 ptr = 0x80000000007e0000ULL ;

  printf ( "payloadExecCobra:  0x%04x ( %d ) bytes\n", size, size ) ;

  /* Make sure we allocate a full page. */
  if ( size > PAGE_SIZE - 1 )
  {
    printf ( "\npayload too big: 0x%04x > 0x%04x\n", size, PAGE_SIZE - 1 ) ;
    return 1 ;
  }

  printf ( "poke payload:      0x%016" PRIx64 "\n", ptr ) ;

  /* poke payload into memory ptr */
  for(i = 0; i < size; i += 8)
  {
    syscall_poke (ptr + i, ( ( u64 * ) ( p->data + i ) ) [0] );
    __asm__ volatile ("sync");
  }

  /* syscall 9 enable */
  syscall_poke (0x8000000000017CE0ULL, 0x7C6903A64E800420ULL);
  __asm__ volatile ("sync");

  /* execute payload */
  printf ( "execute payload:   0x%016" PRIx64 "\n", ptr + p->payload[p->idx].descriptor ) ;
  lv2syscall8(9, (u64)ptr + p->payload[p->idx].descriptor, 0,0,0,0,0,0,0);

  return 0;
}

