//
// Created by 鍋島雅貴 on 2016/06/04.
//

#include "file_source.h"

otf_result_t cfile_source_open(cfile_source_t *self, const char *fn)
{
    self->base.close = cfile_source_close;
    self->base.read = cfile_source_read;
    self->base.seek = cfile_source_seek;
    self->fp = NULL;

    FILE *fp;
    fp = fopen(fn, "rb");
    if(fp == NULL) {
        return OTF_E_READ;
    }

    self->fp = fp;

    return OTF_S_OK;
}

otf_result_t cfile_source_read(void *self, void *buf, uint32_t size)
{
    cfile_source_t *f = (cfile_source_t *)self;
    if(f->fp == NULL) {
        return OTF_E_READ;
    }

    size_t rd = fread(buf, size, 1, f->fp);
    if(rd != 1) {
        return OTF_E_READ;
    }
    return OTF_S_OK;
}


otf_result_t cfile_source_seek(void *self, uint32_t pos)
{

    cfile_source_t *f = (cfile_source_t *)self;
    if(f->fp == NULL) {
        return OTF_E_READ;
    }

    if(fseek(f->fp, (long)pos, SEEK_SET) != 0) {
        return OTF_E_READ;
    }
    return OTF_S_OK;
}

void cfile_source_close(void *self)
{
    cfile_source_t *f = (cfile_source_t *)self;
    if(f->fp == NULL) {
        return;
    }
    fclose(f->fp);
    f->fp = NULL;
}
