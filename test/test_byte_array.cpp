//
// Created by 鍋島雅貴 on 2016/06/04.
//

#include <gtest/gtest.h>
#include <OpenType.h>
#include "byte_array.h"

extern "C" otf_result_t byte_array_source_read(void *self, void *buf, uint32_t size);
extern "C" otf_result_t byte_array_source_seek(void *self, uint32_t pos);
extern "C" void byte_array_source_close(void *self);

TEST(byte_array_init, Test1)
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