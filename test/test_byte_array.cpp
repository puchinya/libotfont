//
// Created by 鍋島雅貴 on 2016/06/04.
//

#include <gtest/gtest.h>
#include <OpenType.h>
#include "byte_array.h"

extern "C" otf_result_t byte_array_source_read(void *self, void *buf, uint32_t size);
extern "C" otf_result_t byte_array_source_seek(void *self, uint32_t pos);
extern "C" void byte_array_source_close(void *self);

TEST(byte_array_source_init, Test1)
{
    byte_array_source_t s;
    uint8_t d[4];

    byte_array_source_init(&s, d, 4);
    GTEST_ASSERT_EQ((void *)s.base.read, (void *)byte_array_source_read);
    GTEST_ASSERT_EQ((void *)s.base.seek, (void *)byte_array_source_seek);
    GTEST_ASSERT_EQ((void *)s.base.close, (void *)byte_array_source_close);
    GTEST_ASSERT_EQ(s.pos, 0);
    GTEST_ASSERT_EQ(s.data_size, 4);
    GTEST_ASSERT_EQ(s.data, d);
}

TEST(byte_array_source_read, Test1)
{
    byte_array_source_t s;
    uint8_t d[4] = { 0x11, 0x22, 0x33, 0x44};
    uint8_t b[4];

    byte_array_source_init(&s, d, 4);

    memset(b, 0, sizeof(b));
    otf_result_t r = byte_array_source_read(&s, b, 2);
    GTEST_ASSERT_EQ(r, OTF_S_OK);
    GTEST_ASSERT_EQ(b[0], 0x11);
    GTEST_ASSERT_EQ(b[1], 0x22);
    GTEST_ASSERT_EQ(b[2], 0x00);
    GTEST_ASSERT_EQ(b[3], 0x00);
    GTEST_ASSERT_EQ(s.pos, 2);

    memset(b, 0, sizeof(b));
    r = byte_array_source_read(&s, b, 1);
    GTEST_ASSERT_EQ(r, OTF_S_OK);
    GTEST_ASSERT_EQ(b[0], 0x33);
    GTEST_ASSERT_EQ(b[1], 0x00);
    GTEST_ASSERT_EQ(b[2], 0x00);
    GTEST_ASSERT_EQ(b[3], 0x00);
    GTEST_ASSERT_EQ(s.pos, 3);

    memset(b, 0, sizeof(b));
    r = byte_array_source_read(&s, b, 2);
    GTEST_ASSERT_EQ(r, OTF_E_READ);
    GTEST_ASSERT_EQ(b[0], 0x00);
    GTEST_ASSERT_EQ(b[1], 0x00);
    GTEST_ASSERT_EQ(b[2], 0x00);
    GTEST_ASSERT_EQ(b[3], 0x00);
    GTEST_ASSERT_EQ(s.pos, 3);

    memset(b, 0, sizeof(b));
    r = byte_array_source_read(&s, b, 0);
    GTEST_ASSERT_EQ(r, OTF_S_OK);
    GTEST_ASSERT_EQ(b[0], 0x00);
    GTEST_ASSERT_EQ(b[1], 0x00);
    GTEST_ASSERT_EQ(b[2], 0x00);
    GTEST_ASSERT_EQ(b[3], 0x00);
    GTEST_ASSERT_EQ(s.pos, 3);
}