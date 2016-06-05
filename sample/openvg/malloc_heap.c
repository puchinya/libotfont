//
// Created by 鍋島雅貴 on 2016/06/04.
//

#include "malloc_heap.h"
#include <stdlib.h>

void *malloc_heap_alloc(void *self, size_t size);
void malloc_heap_free(void *self, void *p);

otf_heap_t s_malloc_heap_inst = {
        malloc_heap_alloc,
        malloc_heap_free
};

otf_heap_t *malloc_heap_instance(void)
{
    return &s_malloc_heap_inst;
}

void *malloc_heap_alloc(void *self, size_t size)
{
    return malloc(size);
}

void malloc_heap_free(void *self, void *p)
{
    free(p);
}
