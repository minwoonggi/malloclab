
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>

#include "memlib.h"
#include "config.h"

static char* mem_start_brk;  
static char* mem_brk;       
static char* mem_max_addr;   


void mem_init(void)
{
    if ((mem_start_brk = (char*)malloc(MAX_HEAP)) == NULL) {
        fprintf(stderr, "mem_init_vm: malloc error\n");
        exit(1);
    }

    mem_max_addr = mem_start_brk + MAX_HEAP;  
    mem_brk = mem_start_brk;                  
}

void mem_deinit(void)
{
    free(mem_start_brk);
}


void mem_reset_brk()
{
    mem_brk = mem_start_brk;
}


void* mem_sbrk(intptr_t incr)
{
    char* old_brk = mem_brk;

    if ((incr < 0) || ((mem_brk + incr) > mem_max_addr)) {
        errno = ENOMEM;
        fprintf(stderr, "ERROR: mem_sbrk failed. Ran out of memory...\n");
        return (void*)-1;
    }
    mem_brk += incr;
    return (void*)old_brk;
}


void* mem_heap_lo()
{
    return (void*)mem_start_brk;
}


void* mem_heap_hi()
{
    return (void*)(mem_brk - 1);
}


size_t mem_heapsize()
{
    return (size_t)(mem_brk - mem_start_brk);
}


size_t mem_pagesize()
{
    return (size_t)getpagesize();
}
