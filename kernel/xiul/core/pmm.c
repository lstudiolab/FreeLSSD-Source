/* This file has been modified by L,Studio. Copyright (c) 2026 L,Studio. All rights reserved. */
#include "xiul/pmm.h"
typedef struct { uint64_t base, pages; } range_t;
typedef struct { uint64_t base, pages; } allocation_t;
static range_t free_ranges[XIUL_PMM_MAX_RANGES];
static allocation_t allocations[XIUL_PMM_MAX_ALLOCATIONS];
static size_t range_count, allocation_count;
static uint64_t free_count;
static int initialized;
static int add_overflow(uint64_t a,uint64_t b,uint64_t *out){*out=a+b;return *out<a;}
static uint64_t page_ceil(uint64_t n){uint64_t r=n%XIUL_PMM_PAGE_SIZE;if(!r)return n;if(n>UINT64_MAX-(XIUL_PMM_PAGE_SIZE-r))return 0;return n+XIUL_PMM_PAGE_SIZE-r;}
static void erase_range(size_t n){size_t i;for(i=n+1;i<range_count;i++)free_ranges[i-1]=free_ranges[i];--range_count;}
static xiul_status_t insert_range(uint64_t base,uint64_t pages){
 uint64_t end;size_t i,pos=0;
 if(!pages||base%XIUL_PMM_PAGE_SIZE||pages>UINT64_MAX/XIUL_PMM_PAGE_SIZE||add_overflow(base,pages*XIUL_PMM_PAGE_SIZE,&end))return XIUL_E_INVALID;
 while(pos<range_count&&free_ranges[pos].base<base)++pos;
 if(pos&&free_ranges[pos-1].base+free_ranges[pos-1].pages*XIUL_PMM_PAGE_SIZE>base)return XIUL_E_INVALID;
 if(pos<range_count&&end>free_ranges[pos].base)return XIUL_E_INVALID;
 if(range_count==XIUL_PMM_MAX_RANGES)return XIUL_E_CAPACITY;
 for(i=range_count;i>pos;i--)free_ranges[i]=free_ranges[i-1];
 free_ranges[pos]=(range_t){base,pages};++range_count;
 if(pos&&free_ranges[pos-1].base+free_ranges[pos-1].pages*XIUL_PMM_PAGE_SIZE==free_ranges[pos].base){free_ranges[pos-1].pages+=free_ranges[pos].pages;erase_range(pos);--pos;}
 if(pos+1<range_count&&free_ranges[pos].base+free_ranges[pos].pages*XIUL_PMM_PAGE_SIZE==free_ranges[pos+1].base){free_ranges[pos].pages+=free_ranges[pos+1].pages;erase_range(pos+1);}
 return XIUL_OK;
}
xiul_status_t xiul_pmm_initialize(const xiul_memory_region_t *map,size_t count){
 size_t i;if(initialized||(count&&!map))return XIUL_E_INVALID;
 range_count=allocation_count=0;free_count=0;
 for(i=0;i<count;i++){uint64_t raw_end,start,end,pages;xiul_status_t s;
  if(!map[i].length||add_overflow(map[i].base,map[i].length,&raw_end))return XIUL_E_INVALID;
  if(map[i].type!=XIUL_MEMORY_USABLE)continue;
  start=page_ceil(map[i].base);if(!start&&map[i].base)return XIUL_E_INVALID;
  end=raw_end-raw_end%XIUL_PMM_PAGE_SIZE;if(!start)start=XIUL_PMM_PAGE_SIZE;
  if(end<=start)continue;pages=(end-start)/XIUL_PMM_PAGE_SIZE;
  if(UINT64_MAX-free_count<pages)return XIUL_E_INVALID;
  s=insert_range(start,pages);if(s!=XIUL_OK)return s;free_count+=pages;
 }
 initialized=1;return XIUL_OK;
}
xiul_status_t xiul_pmm_allocate(size_t pages,size_t align_pages,uint64_t *out){
 size_t i;uint64_t alignment;
 if(!initialized||!out||!pages)return XIUL_E_INVALID;
 if(allocation_count==XIUL_PMM_MAX_ALLOCATIONS)return XIUL_E_CAPACITY;
 if(!align_pages)align_pages=1;
 if((align_pages&(align_pages-1))||(uint64_t)align_pages>UINT64_MAX/XIUL_PMM_PAGE_SIZE||(uint64_t)pages>UINT64_MAX/XIUL_PMM_PAGE_SIZE)return XIUL_E_INVALID;
 alignment=(uint64_t)align_pages*XIUL_PMM_PAGE_SIZE;
 for(i=0;i<range_count;i++){uint64_t base=free_ranges[i].base,rem=base%alignment,aligned=base,skip,available,after;
  if(rem){uint64_t delta=alignment-rem;if(base>UINT64_MAX-delta)continue;aligned=base+delta;}
  skip=(aligned-base)/XIUL_PMM_PAGE_SIZE;if(skip>free_ranges[i].pages)continue;
  available=free_ranges[i].pages-skip;if((uint64_t)pages>available)continue;after=available-pages;
  if(skip&&after&&range_count==XIUL_PMM_MAX_RANGES)return XIUL_E_CAPACITY;
  if(skip&&after){size_t j;for(j=range_count;j>i+1;j--)free_ranges[j]=free_ranges[j-1];++range_count;free_ranges[i].pages=skip;free_ranges[i+1]=(range_t){aligned+(uint64_t)pages*XIUL_PMM_PAGE_SIZE,after};}
  else if(skip)free_ranges[i].pages=skip;
  else if(after){free_ranges[i].base=aligned+(uint64_t)pages*XIUL_PMM_PAGE_SIZE;free_ranges[i].pages=after;}
  else erase_range(i);
  allocations[allocation_count++]=(allocation_t){aligned,pages};free_count-=pages;*out=aligned;return XIUL_OK;
 }return XIUL_E_NOMEM;
}
xiul_status_t xiul_pmm_release(uint64_t address,size_t pages){
 size_t i;xiul_status_t s;if(!initialized||!pages||address%XIUL_PMM_PAGE_SIZE)return XIUL_E_INVALID;
 for(i=0;i<allocation_count;i++)if(allocations[i].base==address&&allocations[i].pages==pages){
  s=insert_range(address,pages);if(s!=XIUL_OK)return s;free_count+=pages;
  allocations[i]=allocations[--allocation_count];return XIUL_OK;
 }return XIUL_E_NOT_FOUND;
}
uint64_t xiul_pmm_free_pages(void){return initialized?free_count:0;}
int xiul_pmm_is_initialized(void){return initialized;}
#ifdef XIUL_TESTING
void xiul_pmm_test_reset(void){range_count=allocation_count=0;free_count=0;initialized=0;}
#endif
