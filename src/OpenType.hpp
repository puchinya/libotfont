//
//  OpenType.hpp
//  OpengGLTest
//
//  Created by 鍋島雅貴 on 2016/05/21.
//  Copyright © 2016年 鍋島雅貴. All rights reserved.
//

#ifndef OpenType_hpp
#define OpenType_hpp

#include <stdio.h>
#include <stdint.h>

typedef int otf_result_t;

#define OTF_S_OK    0
#define OTF_S_TRUE  1
#define OTF_S_FALSE 0
#define OTF_E_PARAM -1
#define OTF_E_STATE -2
#define OTF_E_MEM   -3
#define OTF_E_READ  -4
#define OTF_E_FILE_FORMAT   -5

#define OTF_SUCCEED(r)  ((r) >= 0)
#define OTF_FAILED(r)  ((r) < 0)

#define OTF_HEAD_FLAG_BASELINE_0            (1 << 0)
#define OTF_HEAD_FLAG_LEFT_SIDEBEARING_0    (1 << 1)
#define OTF_HEAD_FLAG_INSTRUCTIONS_PTSIZE   (1 << 2)

#define OTF_UNITS_PER_EM_MIN           16
#define OTF_UNITS_PER_EM_MAX           16384

#define OTF_INDEX_TO_LOC_FORMAT_SHORT   0
#define OTF_INDEX_TO_LOC_FORMAT_LONG    1


typedef uint32_t otf_fixed_t;
typedef int16_t otf_fword_t;
typedef uint16_t otf_ufword_t;
typedef int16_t otf_f2dot14;

typedef struct {
    uint8_t data[8];
} otf_datetime_t;

typedef struct
{
    otf_result_t (*seek)(void *self, uint32_t pos);

    /**
     * Read data
     * @param self pointer of top of otf_source_t
     *
     * @retval >0 Read bytes
     * @retval 0 End of stream
     * @retval <0 Exception is occurred
     */
    otf_result_t (*read)(void *self, void *buf, uint32_t read_size);
    void (*close)(void *self);
} otf_source_t;

typedef struct
{
    void *(*alloc)(void *self, size_t size);
    void (*free)(void *self, void *p);
} otf_heap_t;

typedef struct
{
    uint16_t    num_tables;
    uint16_t    search_range;
    uint16_t    entry_selector;
    uint16_t    range_shift;
} otf_offset_table_t;

typedef struct
{
    uint32_t    tag;
    uint32_t    check_sum;
    uint32_t    offset;
    uint32_t    length;
} otf_table_dir_entry_t;

typedef struct {
    uint16_t    platform_id;
    uint16_t    encoding_id;
    uint32_t    offset;
} otf_cmap_encoding_table_t;

typedef struct {
    uint16_t    format;
    uint16_t    length;
    uint16_t    language;
    uint8_t     glyph_id_array[256];
} otf_cmap_format0_encoding_table_t;

typedef struct {
    uint16_t    first_code;
    uint16_t    entry_count;
    int16_t     id_delta;
    uint16_t    id_range_offset;
} otf_cmap_format2_sub_header_t;

typedef struct {
    uint16_t    format;
    uint16_t    length;
    uint16_t    version;
    uint16_t    sub_header_keys[256];
    otf_cmap_format2_sub_header_t    *sub_headers;
    uint16_t    *glyph_index_array;
} otf_cmap_format2_encoding_table_t;

typedef struct {
    uint16_t    format;
    uint16_t    length;
    uint16_t    language;
    uint16_t    sub_header_keys[256];
    uint8_t     *sub_headers;
    uint16_t    *glyph_index_array;
} otf_cmap_high_byte_mapping_through_table_t;

typedef struct {
    uint16_t                version;
    uint16_t                num_tables;
    otf_cmap_encoding_table_t  *encoding_tables;
} otf_cmap_table_t;

typedef struct {
    otf_fixed_t version;   /* 0x00010000 for version 1.0. */
    otf_fixed_t font_revision;
    uint32_t check_sum_adjustment;
    uint32_t magic_number;
    uint16_t flags;
    uint16_t uints_per_em;  /* from 16 to 16384 */
    otf_datetime_t created;
    otf_datetime_t modified;
    int16_t x_min;  /* For all glyph bouding boxes */
    int16_t y_min;
    int16_t x_max;
    int16_t y_max;
    uint16_t mac_style;
    uint16_t lowest_rec_ppem;
    int16_t font_direction_hint;
    int16_t index_to_loc_format;    /* 0 for short offsets, 1 for long. */
    int16_t glyph_data_format;      /* 0 for current format. */
} otf_head_table_t;

typedef struct {
    otf_fixed_t version;        /* 0x00010000 for version 1.0. */
    uint16_t    num_glyphs;
    uint16_t    max_points;
    uint16_t    max_contours;
    uint16_t    max_composite_points;
    uint16_t    max_composite_contours;
    uint16_t    max_zones;
    uint16_t    max_twilight_points;
    uint16_t    max_storage;
    uint16_t    max_function_defs;
    uint16_t    max_instruction_defs;
    uint16_t    max_stack_elements;
    uint16_t    max_size_of_instructions;
    uint16_t    max_component_elements;
    uint16_t    max_component_depth;
} otf_maxp_table_t;

typedef struct {
    union {
        uint16_t    *short_offsets;
        uint32_t    *long_offsets;
    } u;
} otf_loca_table_t;

typedef struct {
    int16_t number_of_contours;
    int16_t x_min, y_min, x_max, y_max;
} otf_glyph_header_t;

typedef struct {
    uint16_t *end_pts_of_contours;
    uint16_t instructions_length;
    uint8_t *instructions;
    uint8_t *flags;
    int32_t *x_coordinates; /* abs */
    int32_t *y_coordinates; /* abs */
} otf_simple_glyph_desc_t;

typedef struct {
    uint16_t flags;
    uint16_t glyph_index;
    uint16_t argument_1;
    uint16_t argument_2;
} otf_composite_glyph_desc_t;

typedef struct {
    otf_glyph_header_t  header;
    union {
        otf_simple_glyph_desc_t     simple;
        otf_composite_glyph_desc_t  composite;
    } u;
} otf_glyf_table_t;

typedef struct
{
    uint32_t                flags;
    otf_source_t            *src;
    otf_heap_t              *heap;
    otf_offset_table_t      offset_table;
    otf_table_dir_entry_t   *table_dir_enties;
    otf_head_table_t        head_table;
    otf_cmap_table_t        cmap_table;
    otf_maxp_table_t        maxp_table;
    otf_loca_table_t        loca_table;
} otf_font_t;

#define OTF_PATH_CLOSE_PATH     (0 << 1)
#define OTF_PATH_MOVE_TO        (1 << 1)
#define OTF_PATH_LINE_TO        (2 << 1)
#define OTF_PATH_QUAD_TO        (5 << 1)

typedef struct
{
    otf_heap_t  *heap;
    uint8_t     *segments;
    int16_t     *data;
    uint32_t    segments_count;
    uint32_t    data_count;
    uint32_t    segments_capacity;
    uint32_t    data_capacity;
} otf_path_t;

typedef otf_result_t (*otf_add_path_seg_cb_t)(void *user_data,
                                              uint8_t seg, uint16_t p0, uint16_t p1, uint16_t p2, uint16_t p3);

otf_result_t otf_load_font(otf_heap_t *heap, otf_source_t *src, uint32_t flags, otf_font_t **font);
void otf_free_font(otf_font_t *font);
otf_result_t otf_get_glyph_outline(otf_font_t *font, uint32_t code, otf_path_t **outline);
otf_result_t otf_get_text_outline_utf8(otf_font_t *font, const uint8_t *text, uint32_t text_length,
                                       otf_path_t **outline);
otf_result_t otf_get_glyph_metrics(otf_font_t *font, uint32_t code);
otf_result_t otf_get_text_size_utf8(otf_font_t *font, const uint8_t *text, uint32_t text_length,
                                    uint16_t *width, uint16_t *height);
void otf_free_path(otf_path_t *path);

#define OTF_FONT_FLAG_USE_CACHE         1
#define OTF_FONT_FLAG_AUTO_CLOSE_SOURCE 2

#define OTF_TAG(c0, c1, c2, c3) ((uint32_t)c0 << 24)
#define OTF_TAG_CMAP OTF_TAG('c', 'm', 'a', 'p')
#define OTF_TAG_HEAD OTF_TAG('h', 'e', 'a', 'd')
#define OTF_TAG_HHEA OTF_TAG('h', 'h', 'e', 'a')
#define OTF_TAG_HMTX OTF_TAG('h', 'm', 't', 'x')
#define OTF_TAG_MAXP OTF_TAG('m', 'a', 'x', 'p')
#define OTF_TAG_NAME OTF_TAG('n', 'a', 'm', 'e')
#define OTF_TAG_OS2 OTF_TAG('O', 'S', '/', '2')
#define OTF_TAG_POST OTF_TAG('p', 'o', 's', 't')

#define OTF_TAG_CVT OTF_TAG('c', 'v', 't', '¥0')
#define OTF_TAG_GLYF OTF_TAG('g', 'l', 'y', 'f')
#define OTF_TAG_LOCA OTF_TAG('l', 'o', 'c', 'a')


#endif /* OpenType_hpp */
