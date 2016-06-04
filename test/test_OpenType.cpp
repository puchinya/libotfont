//
// Created by 鍋島雅貴 on 2016/06/04.
//

#include <gtest/gtest.h>
#include <OpenType.h>
#include "byte_array.h"

extern "C" otf_result_t otf_read_u8(otf_source_t *self, uint8_t *data);

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
