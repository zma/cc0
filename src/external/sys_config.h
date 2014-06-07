#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

#include "mem.h"

// *************************
// VPC ID

// if defined, the prefix is 13, otherwise
// the prefix is 8
// #define VPCID_IP_PREFIX_13 1
#define VPCID_IP_PREFIX_8 1

// *************************
// Scheduler

// Whether aborts activate depending tasks
// Disabled this -- this cause potentially many retries
#define noABORTS_ACTIVATE_DEPENDING_TASKS

// only one watcher instance in runner Q
#define ACTIVATE_ONLY_ONE_WATCHER_INSTANCE

// scheduling algorithm number: configuration in the config file
// Must choose one from the followings
#define DETERMINISTIC_SCHEDULING 0
#define BELL_SCHEDULING 1

#define DETERMINISTIC_SCHEDULING_STR "deterministic_scheduling"
#define BELL_SCHEDULING_STR "bell_scheduling"

// scheduling wait time; in usec
#define SCHEDULING_TIME_SLICE (10)

// grenade explosion time
#define GRENADE_EXPLOSION_TIME (7200)

// number of tries to find best ones before selecting any ones
// during scheduling:
// SCHEDULING_MAX_TRY_BEFORE_ANY * SCHEDULING_TIME_SLICE is the maximum
// allowed time that a task in the Q and is not dispatched.
// #define SCHEDULING_MAX_TRY_BEFORE_ANY (1000000)

// before used
// 1MB
#define CONFIG_MIN_LEN_FOR_LOCALITY (0x100000)
// 1GB
// #define CONFIG_MIN_LEN_FOR_LOCALITY (0x40000000)
// 512MB
// #define CONFIG_MIN_LEN_FOR_LOCALITY (0x20000000)

// very large -- disabled it
// #define CONFIG_MIN_LEN_FOR_LOCALITY (0x7000000000000000)

#define SCHEDULER_RCMB_MAX_SIZE 2048
#define SCHEDULER_RUNNER_Q_MAX_SIZE 4096
// #define SCHEDULER_LOCALITY_INFO_SIZE 256
#define SCHEDULER_LOCALITY_INFO_SIZE 2048

// persistent memory configuration
#ifdef PPM_12TB

// Note: depends on the PPM_12TB here
// exit code after the stack range
// #define RUNNER_ID_ALLOCATE_BASE   (0x10000000000)
#define RUNNER_ID_ALLOCATE_BASE   (0x3d000000000)
// Note: n (1 here) pages
#define RUNNER_ID_ALLOCATE_INC    (0x1000)

// reserved # of IDs for special runners
#define RUNNER_ID_RESERVE_N       (0x10)
#define RUNNER_ID_LOADER          (0x3d000000000)
#define RUNNER_ID_IDLE_HDLR       (0x3d000001000)


// shalloc area allocated  range
// #define SHALLOC_AREA_ALLOC_BASE       (0x20000000000)
// #define SHALLOC_AREA_ALLOC_MAX_LEN    (0x10000000000)
// For KV+
// #define SHALLOC_AREA_ALLOC_BASE       (0x20000000000)
#define SHALLOC_AREA_ALLOC_BASE       (AMR_OFFSET_END + 1 + PAGE_SIZE)
#define SHALLOC_AREA_RAW_MAX_LEN         (0x2fb00000000)
// for debug: make it easily exhaused in one space:
// #define SHALLOC_AREA_RAW_MAX_LEN         (0x3400000000)
// #define SHALLOC_AREA_RAW_MAX_LEN            (0x3000000000)

// handle shalloc space management with replication supported
#ifdef MM_ENABLE_REPLICATION
#define SHALLOC_AREA_ALLOC_MAX_LEN  (SHALLOC_AREA_RAW_MAX_LEN / MM_REPLICATION_FACTOR)
#else
#define SHALLOC_AREA_ALLOC_MAX_LEN  (SHALLOC_AREA_RAW_MAX_LEN)
#endif // MM_ENABLE_REPLICATION

// idle handler heap
#define IDLE_HANDLE_RUNNER_HEAP_BEGIN (0x3f800000000)
#define IDLE_HANDLE_RUNNER_HEAP_LEN   (0x1000)

// I0 Code meta
// I0 Code meta in SR (AMR)
#define I0_CODE_META_BEGIN            (0x3f000000000)
#define I0_CODE_META_LEN              (0x2000)
#define I0_CODE_META_END              (I0_CODE_META_BEGIN + I0_CODE_META_LEN - 1)

#else // not PPM_12TB

#define SPAWN_ALLOCATED_STACK_BASE   (0x70000000000)
#define SPAWN_ALLOCATED_STACK_LEN    (0x1000)
#define SPAWN_ALLOCATED_STACK_END    (0x7efffffffff)

//shalloc area allocated  range
#define SHALLOC_AREA_ALLOC_BASE (0x50000000000)
#define SHALLOC_AREA_ALLOC_MAX_LEN (0x20000000000)

// idle handler heap
#define IDLE_HANDLE_RUNNER_HEAP_BEGIN 0x7f800000000
#define IDLE_HANDLE_RUNNER_HEAP_LEN 0x1000

// I0 Code meta in SR (AMR)
#define I0_CODE_META_BEGIN   (0x7f000000000)
#define I0_CODE_META_LEN   (0x2000)

#endif // PPM_12TB

// support exit code
#define EC_SUPPORT 1

// exit code range

// exit code range length
#define EC_RANGE  (RUNNER_ID_ALLOCATE_INC)

// interface to MEM
#define __TEST_IF_PERF__

#define IF_BETWEEN_RETRY_USLEEP 100000
// #define IF_BETWEEN_RETRY_USLEEP 10000000
#define IF_MAX_RETRY_NUM 100
// creating ss check whether ~cur copy are ready
// not always true. Need retry.
#define IF_MAX_RETRY_NUM_CS 100000
#define IF_MAX_RETRY_NUM_COMMIT 1
#define IF_MAX_RETRY_NUM_COMMIT_DELETE 1

// *************************
// VPC and scheduler

#define noMM_ENABLE_PRNT_MMAP 1

#define PR_RUNNER_SB 0x100000100;
#define PR_RUNNER_SS 0x100000108;
#define PR_RUNNER_ID 0x100000120;

// auto init area in PR
#define PR_AUTO_INIT_AREA_BASE (0x200100000)
#define PR_AUTO_INIT_AREA_MAX_LEN (0x1000)

// #define DEFAULT_STACK_START  (1UL << 40) // + (0xfff);
// #define DEFAULT_STACK_SIZE (1UL << 12) // (1UL << 22);

// // LIR_ADDRESS is the starting address of LIR
// #define LIR_ADDRESS  (0x100004000)
// #define LIRS_MAX_LEN (0xc000)
//
// // LIRS is for users
// #define LIRS ((char*)LIR_ADDRESS))

// *************************
// Interface

#define NEW_RUNNER_BACK_FILL   (0x100000400)
#define NEW_RUNNER_ID_BACK_FILL   (0x100000410)

// system call
#define SYSCALL_ID_ADDR (0x100000420)
#define SYSCALL_COMM_AREA_ADDR (0x100001000)
#define SYSCALL_COMM_AREA_LEN (0x1000)

#define SYSCALL_ID_MALLOC  (1)
#define SYSCALL_ID_MALLOC_EXT  (6)

#define SYSCALL_ID_USLEEP  (2)
#define SYSCALL_ID_GETTIME (7)

// streaming I/O
#define SYSCALL_ID_SREAD   (3)
#define SYSCALL_ID_SWRITE  (4)
#define SYSCALL_ID_SLISTEN (5)

// shalloc_ext option
#define SHALLOC_1WAY 1
#define SHALLOC_3WAY 3



#define REG_FILE_BEGIN         (0x200000000)
#define REG_FILE_END           (0x200000040)

#define IO_STDIN_Q              (0x100000200)
#define IO_STDOUT_Q             (0x100000208)


// *************************
// binary translator

// #define _CONV_V0_

// Note: highest 64-bit possible memory address (byte): 0x7fffffffefff
// Note: 64-bit : 0x7fffffffeff8

// In SR (AMR)
// #define I0_CODE_BEGIN   (0x400000000)
#define I0_CODE_BEGIN   (SR_BEGIN)
// #define I0_CODE_MAX_LEN   (0x20000)
#define I0_CODE_MAX_LEN   (0x80000)
#define I0_CODE_END   (I0_CODE_BEGIN + I0_CODE_MAX_LEN - 1)

// maximum translating block size for each time
#define I0_TRANSLATE_BLOCK_LEN   (0x10000000)

// special FIs
#define LOADER_FI (0)
#define IDLE_HANDLER_FI (8)

/*
#define I0_TRANSLATED_BLOCK_TABLE (0xffe00010000)
#define I0_TRANSLATED_BLOCK_TABLE_MAX_LEN   (0x10000)

#define NATIVE_CODE_BEGIN (0xe1000000000)
#define NATIVE_CODE_BEGIN_MAX_LEN   (0x10000)

#define NATIVE_TRAMPOLINE (0xffe10000000)
#define NATIVE_TRAMPOLINE_MAX_LEN   (0x10000)
*/

#define I0_TRANSLATED_BLOCK_TABLE (0x110000000)
// #define I0_TRANSLATED_BLOCK_TABLE_MAX_LEN   (0x20000)
#define I0_TRANSLATED_BLOCK_TABLE_MAX_LEN   (0x40000)


#define NATIVE_CODE_BEGIN (0x110040000)
#define NATIVE_CODE_BEGIN_MAX_LEN   (0x200000)

#define NATIVE_TRAMPOLINE (0x120000000)
#define NATIVE_TRAMPOLINE_MAX_LEN   (0x100000)

// the INDIR_JUMP configs determines the hash function
// in sys_indirect_jump
#define NATIVE_INDIR_JUMP (0x130000000)
#define NATIVE_INDIR_JUMP_MAX_LEN   (0x100000)
#define NATIVE_INDIR_JUMP_ENTRY_SIZE (0x80)

// in PR:
#define SYS_CALL_TABLE (0x100000200)
#define SYS_CALL_TABLE_MAX_LEN   (0x200)

#define SYS_TRAMPOLINE_HANDLER  (0x100000200)
#define SYS_RUNNER_WRAPPER      (0x100000208)
#define SYS_BACK_RUNNER_WRAPPER (0x100000210)
#define SYS_NEW_RUNNER          (0x100000218)
#define SYS_INDIRECT_JUMP       (0x100000220)
#define SYS_STDIN_Q             (0x100000228)
#define SYS_STDOUT_Q            (0x100000230)
#define SYS_INDIR_JUMP_HANDLER  (0x100000238)
#define SYS_SPAWNX              (0x100000240)
// spawnx affiliation 64-bit integer
#define SPAWNX_AFF_SPACE_ADDR   (0x100000248)


// Profiling counter: last
#define PROFILLING_LAST         (0x140000000)

/*
#define SYS_CALL_TABLE (0xffe00000000)
#define SYS_CALL_TABLE_MAX_LEN   (0x10000)

#define SYS_TRAMPOLINE_HANDLER  (0xffe00000000)
#define SYS_RUNNER_WRAPPER      (0xffe00000008)
#define SYS_BACK_RUNNER_WRAPPER (0xffe00000010)
#define SYS_NEW_RUNNER          (0xffe00000018)
*/

// Interrupt descriptor table
#define IDT_BASE       (0x100000800)

#endif // _SYS_CONFIG_H_
