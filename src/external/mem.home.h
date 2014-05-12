#ifndef _MEM_HOME_H_
#define _MEM_HOME_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
//#include "mem.h"

// ------------- For Home -------------------------------

// check whether lnk_cnt < 0
// #define _LNK_CNT_CHECKING 1

// HOME PAGE MANAGEMNET
// Group-based Page Meta Management
// must defined for space support.
// TODO: add space support for non-GPMM
#define GPMM 1

/// Page table for HOME

typedef struct _page_copy{
	vpc_id_t	vpc_id;
	uint8_t		attrib[2];
    uint16_t	link_cnt;
}page_copy_t;
/*
attrib[1] (8 bit)
	7:1		(7 bit)		reserved (IF GPMM defined, see below).
	0:0		(1 bit)		current version? 0: current, 1: non-current
*/

#define NO_OF_COPY	2
typedef struct _page_entry{
	page_copy_t	copy[NO_OF_COPY];
}page_entry_t;

/*
 Note: big endian on x86-64
 IFDEF GPMM
     copy[0].attrib[1:0] (16 bits)
     15:1  (15 bit)    group count (# of following pages in the group if this 
                       is the master; back index to reach the master page if
                       this is not the master)
     0:0   (1 bit)     current version? 
     copy[1].attrib[1:0] (16 bits)
     15:14  (14 bit)   reserved
     1:1   (1 bit)     whether master? 0: is, 1: not
     0:0   (1 bit)     current version? 
 ENDIF
*/

#define HOME_PAGE_ENTRY_ADDR	0x400000000

// bit for current copy
#define CURRENT_COPY	0

// max group count value: 15bit
#define GROUP_CNT_MAX_VALUE ( (0x1 << 15) - 1)
// #define GROUP_CNT_MAX_VALUE ( (0x1 << 8) - 1)

// MASTER BIT
#define MASTER_BIT_MASTER 0
#define MASTER_BIT_NON_MASTER 1


// the page entry on MEM home
#define PAGE_ENTRY(x)	(*((page_entry_t*)(HOME_PAGE_ENTRY_ADDR+sizeof(page_entry_t)*(x))))
// pointer to the page entry on MEM home
#define PAGE_ENTRY_P(x)	((page_entry_t*)(HOME_PAGE_ENTRY_ADDR+sizeof(page_entry_t)*(x)))
// cal page no by the address of page entry
#define PAGE_NO_BY_ENTRY_P(pe) (((uint64_t)(pe) - HOME_PAGE_ENTRY_ADDR)/sizeof(page_entry_t))

#ifdef GPMM // GPMM

// support to single page operations
#define GET_CURRENT_COPY(p) gpmm_get_cur_copy((p))
#define SET_CURRENT_COPY(p,c) gpmm_set_cur_copy((p), 1, (c))

#else // no GPMM

#define GET_CURRENT_COPY(p) ((((*((page_entry_t*)(HOME_PAGE_ENTRY_ADDR+sizeof(page_entry_t)*(p)))).copy[0].attrib[0])&0x01)==CURRENT_COPY?0:1)
#define SET_CURRENT_COPY(p,c)\
	do{\
		PAGE_ENTRY(p).copy[((c)&0x01)].attrib[0]=PAGE_ENTRY(p).copy[((c)&0x01)].attrib[0]&0xFE;\
		PAGE_ENTRY(p).copy[((~(c))&0x01)].attrib[0]=PAGE_ENTRY(p).copy[((~(c))&0x01)].attrib[0]|0x01;\
	}while(0)

#endif // GPMM

// GPMM

// only appliable to master
// pe is a pointer to the entry

// get current copy of a page whose entry is p.
// pe must be a master
#define GET_MASTER_CUR_COPY(pe) ( (((pe)->copy[0].attrib[0])&0x01) == CURRENT_COPY ? 0 : 1 )
#define SET_MASTER_CUR_COPY(pe, c) \
 	do {\
		(pe)->copy[((c)&0x01)].attrib[0] = (pe)->copy[((c)&0x01)].attrib[0]&0xFE;\
		(pe)->copy[((~(c))&0x01)].attrib[0] = (pe)->copy[((~(c))&0x01)].attrib[0]|0x01;\
	} while(0)

// TODO: to debug
// pe must be a master
#define GET_MASTER_LNK_CNT(pe, v) ((pe)->copy[(v)].link_cnt)
#define SET_MASTER_LNK_CNT(pe, v, l)            \
    do {                                        \
        (pe)->copy[(v)].link_cnt = (l);         \
    } while(0)
#define GET_MASTER_VMCID(pe, v) ((pe)->copy[(v)].vpc_id)
#define SET_MASTER_VMCID(pe, v, id)             \
    do {                                        \
        (pe)->copy[(v)].vpc_id = (id);          \
    } while(0)

// pe need not to be a master
#define GET_MASTER_BIT(pe) ( (((pe)->copy[1].attrib[0])&0x2) >> 1)

// Note: this should be atomic. This byte is in a cache line. It works
// on x86-64.
// This guarantee ensure that splitting is safe when there are readings
// together with that other bits are set before setting the master bit.
#define SET_MASTER_BIT(pe, b) \
    do {\
        (pe)->copy[1].attrib[0] = ((pe)->copy[1].attrib[0] & (~0x2)) | (((b) & 0x1) << 1); \
    } while (0)

// pe need not to be a master
#define IS_MASTER(pe) (GET_MASTER_BIT((pe)) == MASTER_BIT_MASTER)

// pe need not to be a master
#define GET_GROUP_CNT(pe) ( ((*(uint16_t*)(&((pe)->copy[0].attrib[0]))) & 0xFFFE) >> 1 )
#define SET_GROUP_CNT(pe, c) \
    do {\
        (*(uint16_t*)(&((pe)->copy[0].attrib[0]))) = ((*(uint16_t*)(&((pe)->copy[0].attrib[0]))) & 0x1) | ((uint16_t)(c) << 1) ; \
    } while (0)

// GPMM

// p is the page number, c is the copy number

// ------------- For VPC -------------------------------

/// Page Table in VPC

typedef uint64_t pt_page_info_t;
typedef struct _pt_entry {
	pt_page_info_t	pt_page_info;
	vpc_id_t		vpc_id;

    // For commit only:
    // number of following pages
    uint32_t        page_cnt; // number of following pages
}pt_entry_t;

/*
pt_page_info (64 bit)
	63:12	(52 bit)	page number
	11:11	(1 bit)		Local=0/Remote=1
	10:10	(1 bit)		Dirty=1/Not=0
	9:9		(1 bit)		VPC Loaded=1/ Not =0
	8:8		(1 bit)		copy number
#ifdef GPMM
    7:0     (8 bit)     page_cnt, number of following pages
#else
	7:0		(8 bit)		reserved
#endif
*/

#define PAGE_LOCAL	0
#define PAGE_REMOTE 1
#define PAGE_DIRTY 1
#define PAGE_CLEAN 0
#define PAGE_VPC_LOADED	1
#define PAGE_VPC_UNKNOWN	0

#define PT_GET_PAGE_NO(pt,index)		((((pt)->pt_entry[(index)].pt_page_info)&0xFFFFFFFFFFFFF000)>>12)
#define PT_ENTRY_GET_PAGE_NO(pt_entry)		(((pt_entry->pt_page_info)&0xFFFFFFFFFFFFF000)>>12)
#define PT_GET_COPY_NO(pt,index)		((((pt)->pt_entry[(index)].pt_page_info)&0x100)>>8)
#define PT_ENTRY_GET_COPY_NO(pt_entry)		((((pt_entry)->pt_page_info)&0x100)>>8)
#define PT_SET_PAGE_NO(pt,index,no)		((pt)->pt_entry[(index)].pt_page_info=(((no)<<12)|(((pt)->pt_entry[(index)].pt_page_info)&0xFFF)))
#define PT_SET_COPY_NO(pt,index,no)		((pt)->pt_entry[(index)].pt_page_info=((((no)&0x1)<<8)|(((pt)->pt_entry[(index)].pt_page_info)&0xFFFFFFFFFFFFFEFF)))
#define PT_ENTRY_SET_COPY_NO(pt_entry,no)		((pt_entry)->pt_page_info=((((no)&0x1)<<8)|(((pt_entry)->pt_page_info)&0xFFFFFFFFFFFFFEFF)))
#define PT_IS_LOCAL(pt,index)			((((pt)->pt_entry[(index)].pt_page_info)&0x800)>>11)
#define PT_ENTRY_IS_LOCAL(pt_entry)			((((pt_entry)->pt_page_info)&0x800)>>11)
#define PT_IS_DIRTY(pt,index)			((((pt)->pt_entry[(index)].pt_page_info)&0x400)>>10)
#define PT_ENTRY_IS_DIRTY(pt_entry)			((((pt_entry)->pt_page_info)&0x400)>>10)
#define PT_IS_VPC(pt,index)			((((pt)->pt_entry[(index)].pt_page_info)&0x200)>>9)
#define PT_ENTRY_IS_VPC(pt_entry)			((((pt_entry)->pt_page_info)&0x200)>>9)
#define PT_SET_LOCAL(pt,index,val)		((pt)->pt_entry[(index)].pt_page_info=((((pt)->pt_entry[(index)].pt_page_info)&0xFFFFFFFFFFFFF7FF)|((val&0x1)<<11)))
#define PT_ENTRY_SET_LOCAL(pt_entry,val)		((pt_entry)->pt_page_info=((((pt_entry)->pt_page_info)&0xFFFFFFFFFFFFF7FF)|((val&0x1)<<11)))
#define PT_SET_DIRTY(pt,index,val)		((pt)->pt_entry[(index)].pt_page_info=((((pt)->pt_entry[(index)].pt_page_info)&0xFFFFFFFFFFFFFBFF)|((val&0x1)<<10)))
#define PT_ENTRY_SET_DIRTY(pt_entry,val)		((pt_entry)->pt_page_info=((((pt_entry)->pt_page_info)&0xFFFFFFFFFFFFFBFF)|((val&0x1)<<10)))
#define PT_SET_VPC(pt,index,val)		((pt)->pt_entry[(index)].pt_page_info=((((pt)->pt_entry[(index)].pt_page_info)&0xFFFFFFFFFFFFFDFF)|((val&0x1)<<9)))
#define PT_SIZE(pt)						(sizeof(pt_t)+sizeof(pt_entry_t)*((pt)->pt_entry_cnt))

/// Misc
#define MAX_BUF (1024*1024)

#endif
