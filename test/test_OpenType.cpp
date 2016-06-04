//
// Created by 鍋島雅貴 on 2016/06/04.
//

#include <gtest/gtest.h>
#include <OpenType.hpp>

extern otf_result_t otf_read_u8(otf_source_t *self, uint8_t *data);

typedef struct {
    otf_source_t base;
    void *data;
    uint32_t pos;
    uint32_t data_size;
} byte_array_source_t;

otf_result_t byte_array_source_read(void * const self, void * const buf, const uint32_t size)
{
    byte_array_source_t * const b = (byte_array_source_t *)self;
    const uint32_t readable_size = b->data_size - b->pos;

    if(readable_size == 0) {
        return 0;
    }

    const uint32_t read_size = size > readable_size ? readable_size : size;

    memcpy(buf, (uint8_t *)b->data + b->pos, read_size);
    b->pos += read_size;

    return read_size;
}

otf_result_t byte_array_source_seek(void * const self, const uint32_t pos)
{
    byte_array_source_t * const b = (byte_array_source_t *)self;

    if(pos > b->data_size) return OTF_E_READ;

    b->pos = pos;

    return OTF_S_OK;
}

void byte_array_source_close(void * const self)
{
}

void byte_array_source_init(byte_array_source_t * const self, void * const data,
                            const uint32_t data_size)
{
    self->base.seek = byte_array_source_seek;
    self->base.read = byte_array_source_read;
    self->base.close = byte_array_source_close;
    self->data_size = data_size;
    self->pos = 0;
    self->data = data;
}

TEST(otf_read_u8, test1)
{
    uint8_t data[4];
    data[0] = 0x11;
    data[1] = 0x22;
    data[2] = 0x33;
    data[3] = 0x44;

    byte_array_source_t s;
    byte_array_source_init(&s, data, 4);

    uint8_t d = 0xFF;
    otf_result_t r = otf_read_u8((otf_source_t *)&s, &d);
    GTEST_ASSERT_EQ(r, OTF_S_OK);
    GTEST_ASSERT_EQ(d, 0x11);

    d = 0xFF;
    r = otf_read_u8((otf_source_t *)&s, &d);
    GTEST_ASSERT_EQ(r, OTF_S_OK);
    GTEST_ASSERT_EQ(d, 0x22);

    d = 0xFF;
    r = otf_read_u8((otf_source_t *)&s, &d);
    GTEST_ASSERT_EQ(r, OTF_S_OK);
    GTEST_ASSERT_EQ(d, 0x33);

    d = 0xFF;
    r = otf_read_u8((otf_source_t *)&s, &d);
    GTEST_ASSERT_EQ(r, OTF_S_OK);
    GTEST_ASSERT_EQ(d, 0x44);

    d = 0xFF;
    r = otf_read_u8((otf_source_t *)&s, &d);
    GTEST_ASSERT_EQ(r, OTF_E_READ);

}
