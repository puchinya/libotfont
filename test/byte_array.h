//
// Created by 鍋島雅貴 on 2016/06/04.
//

#ifndef PROJECT_BYTE_ARRAY_H
#define PROJECT_BYTE_ARRAY_H

#include <OpenType.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    otf_source_t base;
    void *data;
    uint32_t pos;
    uint32_t data_size;
} byte_array_source_t;

void byte_array_source_init(byte_array_source_t *self, void *data, uint32_t data_size);


#ifdef __cplusplus
};
#endif

#endif //PROJECT_BYTE_ARRAY_H
