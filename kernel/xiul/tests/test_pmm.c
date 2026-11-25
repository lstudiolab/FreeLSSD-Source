/* This file has been modified by L,Studio. Copyright (c) 2026 L,Studio. All rights reserved. */
#include <assert.h>
#include <stdio.h>
#include "xiul/pmm.h"
void xiul_pmm_test_reset(void);
int main(void){
 const xiul_memory_region_t map[]={{0x1003,0x9000,XIUL_MEMORY_USABLE,0},{0x20000,0x4000,2,0},{0x30000,0x8000,XIUL_MEMORY_USABLE,0}};
 uint64_t a=0,b=0,initial;xiul_pmm_test_reset();
 assert(xiul_pmm_initialize(map,3)==XIUL_OK);initial=xiul_pmm_free_pages();assert(initial==16);
 assert(xiul_pmm_allocate(2,4,&a)==XIUL_OK);assert(a%(4*XIUL_PMM_PAGE_SIZE)==0);
 assert(xiul_pmm_free_pages()==initial-2);assert(xiul_pmm_allocate(3,1,&b)==XIUL_OK);
 assert(xiul_pmm_release(a,2)==XIUL_OK);assert(xiul_pmm_release(a,2)==XIUL_E_NOT_FOUND);
 assert(xiul_pmm_release(b,2)==XIUL_E_NOT_FOUND);assert(xiul_pmm_release(b,3)==XIUL_OK);
 assert(xiul_pmm_free_pages()==initial);assert(xiul_pmm_allocate(100,1,&a)==XIUL_E_NOMEM);
 assert(xiul_pmm_allocate(1,3,&a)==XIUL_E_INVALID);assert(xiul_pmm_allocate(0,1,&a)==XIUL_E_INVALID);
 assert(xiul_pmm_initialize(map,3)==XIUL_E_INVALID);
 puts("XIUL physical memory allocator tests: PASS");return 0;
}
