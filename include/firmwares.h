/*
 * Copyright (C) an0nym0us
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 */

// for FIRMWARE_3_55
#ifdef FIRMWARE_3_55
#define ALLOC 			0x8000000000060B78ULL
#define FREE 			0x8000000000060FB4ULL
#define SYSCALL_TABLE		0x8000000000346570ULL
#define MEM_BASE		0x800000000000EF48ULL
#endif

// for FIRMWARE_3_41
#ifdef FIRMWARE_3_41
#define ALLOC 			0x8000000000062088ULL
#define FREE 			0x80000000000624c8ULL
#define SYSCALL_TABLE 		0x80000000002eb128ULL
#define MEM_BASE		0x8000000000050B3CULL
#endif

// for FIRMWARE_3_41_dex
#ifdef FIRMWARE_3_41_dex
#define ALLOC			0x8800000000065974ULL
#define FREE			0x8000000000065DB4ULL
#define SYSCALL_TABLE		0x8000000000303130ULL
#define MEM_BASE		0x8000000000054400ULL
#endif

// for FIRMWARE_3_41_kiosk
#ifdef FIRMWARE_3_41_kiosk
#define ALLOC			0x8000000000062084ULL
#define FREE			0x80000000000624c4ULL
#define SYSCALL_TABLE		0x80000000002eb128ULL
#define MEM_BASE		0x8000000000050B3CULL
#endif

// for FIRMWARE_3_40
#ifdef FIRMWARE_3_40
#define ALLOC			0x8000000000062084ULL
#define FREE			0x80000000000624c4ULL
#define SYSCALL_TABLE		0x80000000002eb128ULL
#define MEM_BASE		0x8000000000050b38ULL
#endif

// for FIRMWARE_3_30
#ifdef FIRMWARE_3_30
#define ALLOC			0x8000000000062080ULL
#define FREE			0x80000000000624c0ULL
#define SYSCALL_TABLE		0x80000000002ea728ULL
#endif

// for FIRMWARE_3_21
#ifdef FIRMWARE_3_21
#define ALLOC			0x8000000000061cf0ULL
#define FREE			0x8000000000062138ULL
#define SYSCALL_TABLE		0x80000000002ea8a0ULL
#endif

// for FIRMWARE_3.15
#ifdef FIRMWARE_3_15
#define ALLOC			0x8000000000061cf0ULL
#define FREE			0x8000000000062138ULL
#define SYSCALL_TABLE		0x80000000002ea820ULL
#define MEM_BASE		0x8000000000050608ULL
#endif

// for FIRMWARE_3_10
#ifdef FIRMWARE_3_10
#define ALLOC			0x8000000000061CECULL
#define FREE			0x8000000000062134ULL
#define SYSCALL_TABLE		0x80000000002EA820ULL
#endif

// for FIRMWARE_3_01
#ifdef FIRMWARE_3_01
#define ALLOC			0x800000000005DF4CULL
#define FREE			0x800000000005E38CULL
#define SYSCALL_TABLE		0x80000000002CFB40ULL
#endif

// for FIRMWARE_2_85
#ifdef FIRMWARE_2_85
#define ALLOC			0x800000000005DAF4ULL
#define FREE			0x800000000005DF34ULL //same in PL3, correct?
#define SYSCALL_TABLE		0x80000000002CBEC0ULL
#endif

// for FIRMWARE_2_76
#ifdef FIRMWARE_2_76
#define ALLOC			0x8000000000059D54ULL
#define FREE			0x800000000005A194ULL
#define SYSCALL_TABLE		0x80000000002C4318ULL
#endif

// for FIRMWARE_2_70
#ifdef FIRMWARE_2_70
#define ALLOC			0x8000000000059d50ULL
#define FREE			0x800000000005a190ULL
#define SYSCALL_TABLE		0x80000000002c4318ULL
#endif


// for FIRMWARE_2_60
#ifdef FIRMWARE_2_60
#define ALLOC			0x8000000000059040ULL
#define FREE			0x800000000005944cULL
#define SYSCALL_TABLE		0x80000000002b67c0ULL
#endif

// for FIRMWARE_2_53
#ifdef FIRMWARE_2_53
#define ALLOC			0x800000000005903cULL
#define FREE			0x8000000000059448ULL
#define SYSCALL_TABLE		0x80000000002b5bb8ULL
#endif

// for FIRMWARE_2_43
#ifdef FIRMWARE_2_43
#define ALLOC			0x800000000006c2e0ULL
#define FREE			0x800000000006c708ULL
#define SYSCALL_TABLE		0x80000000002d2b18ULL
#endif

