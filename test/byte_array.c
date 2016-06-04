//
// Created by 鍋島雅貴 on 2016/06/04.
//

#include "byte_array.h"
#include <string.h>

otf_result_t byte_array_source_read(void *self, void *buf, uint32_t size)
{
    byte_array_source_t * const b = (byte_array_source_t *)self;
    const uint32_t readable_size = b->data_size - b->pos;

    if(readable_size < size) {
        return OTF_E_READ;
    }

    memcpy(buf, (uint8_t *)b->data + b->pos, size);
    b->pos += size;

    return OTF_S_OK;
}

otf_result_t byte_array_source_seek(void *self, uint32_t pos)
{
    byte_array_source_t * const b = (byte_array_source_t *)self;

    if(pos > b->data_size) return OTF_E_READ;

    b->pos = pos;

    return OTF_S_OK;
}

void byte_array_source_close(void *self)
{
}

void byte_array_source_init(byte_array_source_t *self, void *data, uint32_t data_size)
{
    self->base.seek = byte_array_source_seek;
    self->base.read = byte_array_source_read;
    self->base.close = byte_array_source_close;
    self->data_size = data_size;
    self->pos = 0;
    self->data = data;
}
