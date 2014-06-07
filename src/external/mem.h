#ifndef _MEM_H_
#define _MEM_H_
#include <stdint.h>
#include "types.h"
#include "mem.home.h"

// whether enaable replication support
#define MM_ENABLE_REPLICATION 1

// eneable checkpointing of the ppm log
#define ENABLE_CHECKPOINTING 1

// Note1: the libi0 should be aware of this configuration
// Note2: Exit code is only implemented under this configuration
// otherwise 8TB
#define PPM_12TB 1

// #define MSYNC_PPM 1

// MEM configuration
// should not large or equal than 4G
#define DEFAULT_PAGE_CAPACITY (1024L*1024L*1024L*2L)

// maximum number of concurrent connection for UDT
#define UDT_MAX_CONN 32

// network: UDP select write timeout wait second
#define NW_UDP_SELECT_W_SEC (0)
// network: UDP select write timeout wait usecond
// #define NW_UDP_SELECT_W_USEC (10)
// for 12TB pmem exp.
// #define NW_UDP_SELECT_W_USEC (100)
// for 1Gbps Network
// #define NW_UDP_SELECT_W_USEC (10000)
// Note: it is nano seconds actually
#define NW_UDP_PSELECT_W_NSEC (400000000)

// network: UDP select read timeout wait second
#define NW_UDP_SELECT_R_SEC (0)
// network: UDP select read timeout wait usecond
#define NW_UDP_SELECT_R_USEC (10)

// network: UDP receive buffer size
#define NW_UDP_RECEIVE_BUFFER_SIZE 65535

// network: home concurrent connection allowed
#define NW_HOME_CONCURR_CONN_NUM 2048
#define NW_VPC_CONCURR_CONN_NUM 2048

// network: VPC UDP server
// maximum retry times to send response
#define NW_RC_UDP_SVR_SEND_RETRY 1
// wait time between to retries of responses
#define NW_RC_UDP_SVR_SEND_RETRY_TIME 10

// usec
#define NW_MEM_RECONN_WAIT_TIME 100

#define NW_MEM_RECONN_RETRY_NUM 100

// read/write wait time interval
//mem_sleep_usec_num
#define MEM_SLEEP_USEC_NUM 100

// #define NW_HOME_MSG_BUFFER_SIZE (1024*1024*1024*5L)
// #define HOME_SS_BUFFER_SIZE (1024*1024*1024*5L)

#define NW_HOME_MSG_BUFFER_SIZE (1024*1024*1024*2L)
#define HOME_SS_BUFFER_SIZE (1024*1024*1024*2L)

// commit_delete retry and wait time
#define MEM_COMMIT_DELETE_BETWEEN_RETRY_USLEEP 100

#define MEM_COMMIT_DELETE_MAX_RETRY_NUM 100

// vpc prefetch PT from home
#define MEM_RC_PREFETCH_PT_NUM 10000
// #define MEM_RC_PREFETCH_PT_NUM 1000000
// buffer for request to home for prefetching PT
// pkt_size (<92) + PT_SIZE (8 + 16*PREFETCH_PT_NUM)
#define MEM_RC_PT_REQ_BUF_SIZE (16*MEM_RC_PREFETCH_PT_NUM+100)
// #define MEM_RC_PT_REQ_BUF_SIZE 32000000

// page prefetching
// must be defined for support to space
#define MEM_ENABLE_PAGE_PREFETCHING 1

// number of prefetching pages
// Note: The practical limit for the UDP datagram length which is
// imposed by the underlying IPv4 protocol is 65,507 bytes (65,535 − 8
// byte UDP header − 20 byte IP header).
// In L0: mem_pkt header 40. 8 bytes for number of pages.
#define MEM_RC_PREFETCH_PAGE_NUM 15

// End of MEM configuration

// number of spaces
#define N_SPACES    64


#define L0_SPACE0           (0x000000000000)
#define L0_SPACE_INC        (0x400000000000)

// offset mask (46 bits http://baijia.info/showthread.php?tid=1404 )
#define OFFSET_MASK 0x00003FFFFFFFFFFF

// space mask (64 - 46 = 18 bits)
#define SPACE_MASK  0xFFFFC00000000000
// zeros on the right
#define SPACE_RIGHT_ZEROS 46

// the starting address of PR except the VPC/VMC impl. used ones
// 4GB
#define PR_BEGIN    (1UL << 32)
// the starting address of SR
// #define SR_BEGIN	0x400000000L
#define SR_BEGIN	0x800000000L
// the ending address of SR
#define SR_END		0x100000000000
// the starting address of Cache
#define CACHE_BEGIN	0x200000000000
// the end of the Cache
#define CACHE_END	0x400000000000

#define MM_END		0x400000000000


#define PR_START_ADDR  (PR_BEGIN)
#define PR_LENGTH  ((SR_BEGIN - PR_BEGIN) - 1024)


// obsolete
// // persistent memory range
// #define PMM_BEGIN   0x100000000000
// // #define PMM_SIZE   0xF00000000
// // 1TB
// #define PMM_SIZE   0x10000000000
// // maximum end
// #define PMM_END     0x200000000000

// if defined PPM_BACKEND_PHYSICAL_MEMORY,
// the persistent memory is not backed by disks.
// it it backed by physical memory instead.
#define noPPM_BACKEND_PHYSICAL_MEMORY 1

#ifdef PPM_12TB

// planetary memory range (12TB)
#define PPM_BEGIN  0x40000000000
#define PPM_SIZE   0xc0000000000
#define PPM_END    0xfffffffffff


// further partitioning the PPM (persistent memory)

// normal SR (in physical memory) size
// 2TB
#define SR_N_SIZE  0x20000000000

// normal SR begin and end
#define SR_N_BEGIN (SR_BEGIN)
#define SR_N_END (SR_BEGIN + SR_N_SIZE - 1)

// normal AMR (in physical memory) size
// 2TB
#define AMR_N_OFFSET_SIZE 0x20000000000

#define AMR_N_OFFSET_BEGIN (SR_N_END + 1)
#define AMR_N_OFFSET_END (AMR_N_OFFSET_BEGIN + AMR_N_OFFSET_SIZE - 1)

// persistent AMR size
// 6TB
#define AMR_P_OFFSET_SIZE 0x60000000000


#define AMR_P_OFFSET_BEGIN (AMR_N_OFFSET_END + 1)
#define AMR_P_OFFSET_END (AMR_P_OFFSET_BEGIN + AMR_P_OFFSET_SIZE - 1)


// normal (not in AMR) persistent memory size
// 6TB
#define PMEM_N_SIZE       0x60000000000


#define PMEM_N_BEGIN (AMR_P_OFFSET_END + 1)
#define PMEM_N_END (PMEM_N_BEGIN + PMEM_N_SIZE - 1)


// Note: in total persistent memory size is PPM_SIZE

// Note: libi0 and KV+ need to be changed accordingly

// AMR ranges
// Option 1: all the SR except PPM
// #define AMR_OFFSET_BEGIN SR_BEGIN
#define AMR_OFFSET_BEGIN AMR_N_OFFSET_BEGIN
// #define AMR_OFFSET_END   PPM_BEGIN
// for KV+
// #define AMR_OFFSET_END   0x54000000000
#define AMR_OFFSET_END   AMR_P_OFFSET_END


#define SPAWN_ALLOCATED_STACK_BASE    (0x30000000000)
#define SPAWN_ALLOCATED_STACK_LEN     (0x2000)
// some for exit code
// #define SPAWN_ALLOCATED_STACK_END     (0x3efffffffff)
#define SPAWN_ALLOCATED_STACK_END     (0x3cfffffffff)


// replication factor
#define MM_REPLICATION_FACTOR 3

// number of replication regions
#define N_REP_REGION 3

// optimization: concurrently push replications
#define CONCURRENT_PUSH_REPLICA 1

// optimization: push replicas in bundles
#define PUSH_BUNDLED_REPLICAS 1

// Note: if defined PUSH_BUNDLED_REPLICAS
// push replica processing on home and mem.vpc are seperated
// for long run, we may migrate the page fetching to use UDT too

#else // default 8TB


// planetary memory range (8TB)
#define PPM_BEGIN  0x80000000000
#define PPM_SIZE   0x80000000000
#define PPM_END    0xfffffffffff

// TODO: partitioning the PPM range



#endif



// page size (same in every nodes)
#define PAGE_SIZE	4096
// log of pages size
#define LG_PAGE_SIZE 12
// page num mask
#define PAGE_NO_MASK 0xFFFFFFFFFFFFF000

// total number of pages in SR, used by the home only
// #define NUM_OF_PAGES	((SR_END-SR_BEGIN)/PAGE_SIZE)
// including multiple spaces
// #define NUM_OF_PAGES	(SR_END/PAGE_SIZE*N_SPACES)
#define NUM_OF_PAGES	((SR_END + L0_SPACE_INC * N_SPACES) / PAGE_SIZE)

// The current layout of the memory on VPC
//
//  ---------------------------------------------------------------------------
//  |    PR   |        SR     |   Reserved    | Page Cache(0) | Page Cache(1) |
//  ---------------------------------------------------------------------------
//  0       16GiB           16TiB           32TiB          48TiB            64TiB
//  0    0x4000000000   0x100000000000   0x200000000000  0x300000000000  0x400000000000
//  0        2^34            2^44           2^45            3*2^44           2^46
//
//  Linux supports only 47 bit address space (128TiB) for process
//
//  Page 0x400000, copy 1 == 300400000000

typedef uint64_t page_info_t;

/*
   page_info (64 bit)
63:12	(52 bit)	page number
11:9	(3 bit)		reserved
8:8		(1 bit)		copy number
7:0		(8 bit)		link count
*/


// The changes to type definitions shouldn't be make before everyone
// agrees

typedef struct _range{
    uint64_t addr;
    uint64_t size;
}range_t;

typedef struct _range_list{
    uint64_t range_cnt;
    range_t range[0];
}range_list_t;

int range2page(range_t *r, uint64_t *begin_page, uint64_t *end_page);

// get total page count from
uint64_t get_range_page_cnt(range_t *r);

// calculate range_list len from range_cnt
uint64_t calc_range_list_len(uint64_t range_cnt);

// whole len of a range_list data struct
uint64_t get_range_list_len(range_list_t *rl);

long rl_merge_overlap_range(range_list_t *rl);

// snapshot range
typedef struct _ss_range{
    uint64_t addr;
    uint64_t size;
    uint64_t copy_no_pos; // position of copy # in data[] of ss_t
}ss_range_t;

int ss_range2page(ss_range_t *r, uint64_t *begin_page, uint64_t *end_page);

// get total page count from a ss
uint64_t get_ss_range_page_cnt(ss_range_t *r);

// snapshot
// data: ss_ranges + copy_no strings
typedef struct _ss_t{
    uint64_t	range_cnt;
    uint64_t	data_len;
    char        data[];
}ss_t;

// calculat ss_t len
uint64_t calc_ss_data_len(range_list_t *rl);
uint64_t calc_ss_len_from_data_len(uint64_t data_len);
uint64_t calc_ss_len(range_list_t *rl);
// copy no begin pos from range list that generates the ss
uint64_t calc_ss_copy_no_data_begin_pos(range_list_t *rl);


// copy_num_len: bit copy# string, len in Bytes
// size: range size
uint64_t calc_copy_num_len_addr_size(uint64_t addr, uint64_t size);
uint64_t calc_copy_num_len_range(range_t *r);
uint64_t calc_copy_num_len_ss_range(ss_range_t *r);

// get_range: get n th range
ss_range_t* get_ss_range(ss_t *ss, uint64_t n);

// copy copy no str. return len of bytes copied.
// the ss_range_dst's field should already be ready
uint64_t copy_no_str_cpy(ss_t *ss_dst, ss_range_t *ss_range_dst, ss_t *ss_src, ss_range_t *ss_range_src);

// causion: used for a formed snapshot

// get total page count from a ss
uint64_t get_ss_page_cnt(ss_t *ss);

// get an already formed ss_t len
uint64_t get_ss_len(ss_t *ss);

// get_copy_number string
uint8_t *get_copy_no_data_ss_range(ss_t *ss, ss_range_t* ss_range);

// get i th page's copy number from copy number string
uint64_t get_copy_no(char *str, uint64_t i);

// deprecated snapshot
typedef struct _snapshot_t{
    uint64_t	page_cnt;
    page_info_t	page_info[];
}snapshot_t;

//#define GET_PAGE_NO(p)		(((p)&0xFFFFFFFFFFFFF000)>>12)
#define SS_GET_PAGE_NO(ss,index)	((((ss)->page_info[(index)])&0xFFFFFFFFFFFFF000)>>12)
//#define SET_PAGE_NO(p,n)	((p)=((n)<<12))
#define SS_SET_PAGE_NO(ss,index,no)	(((ss)->page_info[(index)])=((no)<<12)|(((ss)->page_info[(index)])&(0x0FFF)))
#define SS_SET_PAGE_ADDR(ss,index,addr)	(((ss)->page_info[(index)])=(((addr)&(0x0FFFFFFFFFFFFF000))|(((ss)->page_info[(index)])&(0x0FFF))))
#define SS_GET_COPY_NO(ss,index)		((((ss)->page_info[(index)])&0x100)>>8)
#define SS_SET_COPY_NO(ss,index,no)	(((ss)->page_info[(index)])=(((ss)->page_info[(index)])&0xFFFFFFFFFFFFFEFF)|(((no)<<8)&0x100))
#define SS_GET_LINK_CNT(ss,index)		(((ss)->page_info[(index)])&0xFF)
#define SS_SET_LINK_CNT(ss,index,cnt)	(((ss)->page_info[(index)])=(((ss)->page_info[(index)])&0xFFFFFFFFFFFFFF00)|((cnt)&0xFF))
// p is of type page_info_t

// deprecated
#define SS_SIZE(ss)	(((ss)->page_cnt)*sizeof(page_info_t)+sizeof(snapshot_t))

// page based - should not be used
// #define N_SS_SIZE(ss)	(((ss)->range_cnt)*sizeof(ss_range_t)+sizeof(ss_t))
// Get the size of a snapshot
// SNAPSHOT_SIZE(snapshot_t* s)


#define RL_SIZE(rl)	(((rl)->range_cnt)*sizeof(range_t)+sizeof(range_list_t))
// Get the size of a range list

enum {
    NOOP = 1,				// no op.
    MEM_RETURN =2,
    MEM_CREATE_SNAPSHOT =3,
    MEM_DELETE_SNAPSHOT = 4,
    MEM_COMMIT_RANGES = 5,
    MEM_FIND_PAGECOPY = 6,
    MEM_INIT_VPC = 7,
    MEM_UPDATE_VPC = 8,
    MEM_RETRIEVE_PAGE = 9,
    MEM_DELETE_REPLICA = 10,
    MEM_SET_WRITECOPY = 11,
    MEM_SET_WRITECOPY_BUNDLE = 24,
    MEM_UNSET_WRITECOPY = 22,
    MEM_PUSH_REPLICA = 12,
    MEM_PUSH_REPLICA_BUNDLE = 23,
    MEM_COMMIT_DELETE = 13,
    MEM_CHECK_COMMIT = 21,
    MEM_ACK = 14,
    MEM_ACK_FAIL = 15,
    MEM_RC_PRE_COMMIT = 16,
    MEM_RC_DO_COMMIT = 17,
    MEM_RC_DO_COMMIT_ABORT = 18,
    MEM_RETRIEVE_PAGE2 = 19,
    MEM_RETRIEVE_PAGE3 = 21,
    MEM_COMMIT_TX_DONE = 20,
};

/** MEM Interface **/
/** For Scheduler **/
snapshot_t* mem_create_snapshot(range_list_t* range_list);
// Create a snapshot on target ranges.
//  If failed, return NULL. Need to retry.
//  If success, return a snapshot_t pointer.

snapshot_t* mem_merge_snapshot(snapshot_t* snapshot1, snapshot_t* snapshot2);
// Merger two snapshots to form a new one.
//  If failed, return NULL. This is PANIC.
//  If success, return a new pointer to the new snapshot
//              The two old snapshots are deleted

l0_reval_t mem_update_vpc(vpc_id_t vpc, snapshot_t* snapshot);
// Update the page table on a VPC
//  If snapshot!=NULL, update the page table, all dirty pages on VPC remain untouched
//                     This is used for updating snapshot while runner runing
//  If snapshot==NULL, cleanup the page table. Call before starting create snapshot for a runner.
//   If failed, return FAIL. This is PANIC.
//   If success, return SUCCESS.

l0_reval_t mem_commit_ranges(vpc_id_t vpc_id, snapshot_t* snapshot, range_list_t* range_list);
// Commit the changed ranges of a runner in VPC (vpc_id).
//   If failed. return FAIL. Need to be handled.
//   If success, return SUCCESS.

l0_reval_t mem_delete_snapshot(snapshot_t* snapshot);
// Delete the snapshot
//   If success, return SUCCESS.
//   If failed, return FAIL. This is PANIC.

// l0_reval_t	mem_peek(snapshot_t* snapshot, uint64_t addr, uint64_t len, void* buf);
// For the scheduler to peek into the memory content

// commit and delete the snapshot
//    IF succeed, return SUCCESS
//    IF failed to commit, return FAIL. The snapshot is not deleted.
l0_reval_t mem_commit_delete(vpc_id_t vpc_id, snapshot_t* b_snapshot, range_list_t* range_list);

/** For VPC **/
l0_reval_t mem_vpc_init(vpc_id_t vpc_id);
// Call this when VPC starts
//   If success, return SUCCESS.
//   If failed, return FAIL. This is PANIC.

/** For Scheduler **/
l0_reval_t mem_scheduler_init();
// Call this when scheduler starts
//   If success, return SUCCESS.
//   If failed, return FAIL. This is PANIC.

typedef struct _pt{
	uint64_t pt_entry_cnt;
	pt_entry_t	pt_entry[];
}pt_t;

/// MEM helper function
int ss_page_no_compare_func(void* a, void* b, int item_size);
int pt_page_no_compare_func(void* a, void* b, int item_size);
int rl_range_compare_func(void* a, void* b, int item_size);
l0_reval_t init_rpc(uint32_t recv_type, uint32_t recv_value, int* sockfd);
//l0_reval_t mem_notify_del_replica(vpc_id_t vpc_id, page_info_t page_info);
int print_ss(ss_t *ss);
int debug_print_snapshot(snapshot_t* snapshot);
int debug_print_pt(pt_t* pt_h);
int debug_print_rl(range_list_t* range_list);

/// Packet
typedef struct _mem_pkt{
	uint64_t	pkt_id;
	vpc_id_t	vpc_id;
	uint32_t	action;
	uint64_t	value;
	uint64_t	page;
    // len of the data field
	uint64_t	len;
	char		data[];
}mem_pkt_t;


int mem_send_ack(int fd);
int mem_send_ack_fail(int fd);
int mem_read_ack(int fd);

int32_t mem_get_msg_meta(int fd, mem_pkt_t *pkt);

// int32_t mem_send_msg(int fd, mem_pkt_t *ppkt);
// mem_pkt_t *mem_get_msg(int fd);

typedef struct _rc_pt_range_t
{
    range_t range;
    pt_t *pt; // pt_t in this range. continuous.
} rc_pt_range_t;

typedef struct _rc_pt_rl_t{
    uint64_t range_cnt;
    rc_pt_range_t range[];
}rc_pt_rl_t;


// obsolete
// network: UDP select timeout wait second
#define NW_UDP_SELECT_TO_SEC (0)
// network: UDP select timeout wait usecond
// #define NW_UDP_SELECT_TO_USEC (20000)
#define NW_UDP_SELECT_TO_USEC (20)
// network: UDP select respond timeout wait second
#define NW_UDP_SELECT_RS_TO_SEC (0)
// network: UDP select timeout wait usecond
#define NW_UDP_SELECT_RS_TO_USEC (20)

// network: UDP buffer size
#define NW_UDP_BUFFER_SIZE 8192

// end obsolete

int is_range_in_amr(range_t *r);
int is_page_in_amr(uint64_t pno);
void normalize_page_no_in_amr(uint64_t *pno);
l0_reval_t init_rpc_persist(uint32_t recv_type, uint32_t recv_value, int* sockfd);

int is_page_in_ppm(uint64_t pno);
long extend_range_to_page_margin(range_list_t *rl);
l0_reval_t init_rpc0(uint32_t recv_type, uint32_t recv_value, int* sockfd);
int debug_print_rc_pt_rl(rc_pt_rl_t *rc_pt);

#endif // _MEM_H_
