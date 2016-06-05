//
// Created by 鍋島雅貴 on 2016/06/04.
//

#ifndef PROJECT_FILE_SOURCE_H
#define PROJECT_FILE_SOURCE_H

#include <OpenType.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    otf_source_t base;
    FILE *fp;
} cfile_source_t;

otf_result_t cfile_source_open(cfile_source_t *self, const char *fn);
otf_result_t cfile_source_read(void *self, void *buf, uint32_t size);
otf_result_t cfile_source_seek(void *self, uint32_t pos);
void cfile_source_close(void *self);

#ifdef __cplusplus
};
#endif

#endif //PROJECT_FILE_SOURCE_H
