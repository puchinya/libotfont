//
//  OpenType.c
//
//  Created by 鍋島雅貴 on 2016/05/21.
//  Copyright © 2016年 鍋島雅貴. All rights reserved.
//

#include "OpenType.h"
#include <string.h>

#define OTF_TRAP(r) if(OTF_FAILED(err = (r))) { goto error_exit; }

void otf_finalize(otf_font_t *self);

void *otf_malloc(otf_heap_t *self, size_t size)
{
    return self->alloc(self, size);
}

void otf_free(otf_heap_t *self, void *p)
{
    self->free(self, p);
}

otf_result_t otf_seek(otf_source_t *self, uint32_t pos)
{
    return self->seek(self, pos);
}

otf_result_t otf_read(otf_source_t *self, void *buf, uint32_t size)
{
    return self->read(self, buf, size);
}

void otf_close(otf_source_t *self)
{
    self->close(self);
}

otf_result_t otf_read_u8(otf_source_t *self, uint8_t *data)
{
    uint8_t buf[1];

    if(OTF_FAILED(otf_read(self, buf, 1))) {
        return OTF_E_READ;
    }

    *data = buf[0];

    return OTF_S_OK;
}

otf_result_t otf_read_u16b(otf_source_t *self, uint16_t *data)
{
    uint8_t buf[2];

    if(OTF_FAILED(otf_read(self, buf, 2))) {
        return OTF_E_READ;
    }

    *data = (uint16_t)buf[1] | ((uint16_t)buf[0] << 8);

    return OTF_S_OK;
}

otf_result_t otf_read_u32b(otf_source_t *self, uint32_t *data)
{
    uint8_t buf[4];
    if(OTF_FAILED(otf_read(self, buf, 4))) {
        return OTF_E_READ;
    }

    *data = (uint32_t)buf[3] | ((uint32_t)buf[2] << 8) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[0] << 24);

    return OTF_S_OK;
}

otf_result_t otf_read_fixed(otf_source_t *self, otf_fixed_t *data)
{
    return otf_read_u32b(self, (uint32_t *)data);
}

otf_result_t otf_read_datetime(otf_source_t *self, otf_datetime_t *data)
{
    return otf_read(self, data, sizeof(otf_datetime_t));
}

otf_result_t otf_read_u16b_n(otf_source_t *self, uint16_t *buf, uint32_t count)
{
    uint32_t i;

    for(i = 0; i < count; i++) {
        if(OTF_FAILED(otf_read_u16b(self, &buf[i]))) {
            return OTF_E_READ;
        }
    }

    return OTF_S_OK;
}

otf_result_t otf_read_u32b_n(otf_source_t *self, uint32_t *buf, uint32_t count)
{
    uint32_t i;

    for(i = 0; i < count; i++) {
        if(OTF_FAILED(otf_read_u32b(self, &buf[i]))) {
            return OTF_E_READ;
        }
    }

    return OTF_S_OK;
}

otf_result_t otf_init(otf_font_t *self, otf_heap_t *heap)
{
    if(self == NULL || heap == NULL) {
        return OTF_E_PARAM;
    }

    self->heap = heap;
    self->src = NULL;

    return OTF_S_OK;
}

void otf_cleanup(otf_font_t *self)
{
    if(self->src != NULL) {
        if(self->flags & OTF_FONT_FLAG_AUTO_CLOSE_SOURCE) {
            otf_close(self->src);
            self->src = NULL;
        }
    }
}

otf_result_t otf_read_cmap_table(otf_source_t *src, otf_cmap_table_t *cmap)
{
    /* TODO: */
    return OTF_S_OK;
}

otf_result_t otf_validate_head_table(otf_head_table_t *table)
{
    if(table->magic_number != 0x5F0F3CF5) {
        return OTF_E_FILE_FORMAT;
    }
    if(table->uints_per_em < 16 || table->uints_per_em > 16384) {
        return OTF_E_FILE_FORMAT;
    }
    switch (table->index_to_loc_format) {
        case OTF_INDEX_TO_LOC_FORMAT_SHORT:
        case OTF_INDEX_TO_LOC_FORMAT_LONG:
            break;
        default:
            return OTF_E_FILE_FORMAT;
    }
    if(table->glyph_data_format != 0) {
        return OTF_E_FILE_FORMAT;
    }

    return OTF_S_OK;
}

static otf_result_t otf_read_head_table(otf_source_t *src, otf_head_table_t *head)
{
    otf_result_t err;

    OTF_TRAP(otf_read_fixed(src, &head->version));
    OTF_TRAP(otf_read_fixed(src, &head->font_revision));
    OTF_TRAP(otf_read_u32b(src, &head->check_sum_adjustment));
    OTF_TRAP(otf_read_u32b(src, &head->magic_number));
    OTF_TRAP(otf_read_u16b(src, &head->flags));
    OTF_TRAP(otf_read_u16b(src, &head->uints_per_em));
    OTF_TRAP(otf_read_datetime(src, &head->created));
    OTF_TRAP(otf_read_datetime(src, &head->modified));
    OTF_TRAP(otf_read_u16b(src, (uint16_t *)&head->x_min));
    OTF_TRAP(otf_read_u16b(src, (uint16_t *)&head->y_min));
    OTF_TRAP(otf_read_u16b(src, (uint16_t *)&head->x_max));
    OTF_TRAP(otf_read_u16b(src, (uint16_t *)&head->y_max));
    OTF_TRAP(otf_read_u16b(src, &head->mac_style));
    OTF_TRAP(otf_read_u16b(src, &head->lowest_rec_ppem));
    OTF_TRAP(otf_read_u16b(src, (uint16_t *)&head->font_direction_hint));
    OTF_TRAP(otf_read_u16b(src, (uint16_t *)&head->index_to_loc_format));
    OTF_TRAP(otf_read_u16b(src, (uint16_t *)&head->glyph_data_format));

    return otf_validate_head_table(head);

error_exit:
    return err;
}

static otf_result_t otf_read_maxp_table(otf_source_t *src, otf_maxp_table_t *maxp)
{
    otf_result_t err;

    OTF_TRAP(otf_read_fixed(src, &maxp->version));
    OTF_TRAP(otf_read_u16b(src, &maxp->num_glyphs));
    if(maxp->version >= 0x10000) {
        OTF_TRAP(otf_read_u16b_n(src, &maxp->max_points, 13));
    }
    return OTF_S_OK;

error_exit:
    return err;
}

static otf_result_t otf_read_loca_table(otf_font_t *self, otf_loca_table_t *loca)
{
    otf_result_t err;
    uint16_t *short_offsets = NULL;
    uint32_t *long_offsets = NULL;
    uint32_t n;

    n = self->maxp_table.num_glyphs + 1;

    otf_source_t *src = self->src;
    if(self->head_table.index_to_loc_format == OTF_INDEX_TO_LOC_FORMAT_LONG) {
        long_offsets = (uint32_t *)otf_malloc(self->heap, sizeof(uint32_t) * n);
        if(long_offsets == NULL) {
            err = OTF_E_MEM;
            goto error_exit;
        }
        OTF_TRAP(otf_read_u32b_n(src, long_offsets, n));
        self->loca_table.u.long_offsets = long_offsets;
    } else {
        short_offsets = (uint16_t *)otf_malloc(self->heap, sizeof(uint16_t) * n);
        if(short_offsets == NULL) {
            err = OTF_E_MEM;
            goto error_exit;
        }
        OTF_TRAP(otf_read_u16b_n(src, short_offsets, n));
        self->loca_table.u.short_offsets = short_offsets;
    }
    return OTF_S_OK;

error_exit:
    if(short_offsets != NULL) otf_free(self->heap, short_offsets);
    if(long_offsets != NULL) otf_free(self->heap, long_offsets);
    return err;
}


#define OTF_READ_TAG_CMAP   (1 << 0)
#define OTF_READ_TAG_HEAD   (1 << 1)
#define OTF_READ_TAG_MAXP   (1 << 2)
#define OTF_READ_TAG_GLYF   (1 << 8)
#define OTF_READ_TAG_LOCA   (1 << 9)

otf_result_t otf_load_impl(otf_font_t *self, otf_heap_t *heap, otf_source_t *src, uint32_t flags)
{
    otf_result_t err;
    uint32_t i;
    uint32_t read_flags;
    otf_fixed_t sfnt_version;

    self->heap = heap;
    self->src = src;
    self->flags = flags;

    /* read version */
    OTF_TRAP(otf_read_fixed(self->src, &sfnt_version));

    /* read offset table */
    OTF_TRAP(otf_read_u16b_n(self->src, (uint16_t *)&self->offset_table, 4));

    /* read table directory entries */
    self->table_dir_entries = otf_malloc(heap, sizeof(otf_table_dir_entry_t) * self->offset_table.num_tables);
    if(self->table_dir_entries == NULL) {
        err = OTF_E_MEM;
        goto error_exit;
    }

    OTF_TRAP(otf_read_u32b_n(self->src, (uint32_t *)self->table_dir_entries, self->offset_table.num_tables * 4));

    /* read required tables */
    read_flags = 0;
    for(i = 0; i < self->offset_table.num_tables; i++) {
        otf_table_dir_entry_t *entry = &self->table_dir_entries[i];
        OTF_TRAP(otf_seek(self->src, entry->offset));
        switch (entry->tag) {
            case OTF_TAG_CMAP:
                if(read_flags & OTF_READ_TAG_CMAP) {
                    err = OTF_E_FILE_FORMAT;
                    goto error_exit;
                }
                err = otf_read_cmap_table(self->src, &self->cmap_table);
                read_flags |= OTF_READ_TAG_CMAP;
                break;
            case OTF_TAG_HEAD:
                if(read_flags & OTF_READ_TAG_HEAD) {
                    err = OTF_E_FILE_FORMAT;
                    goto error_exit;
                }
                err = otf_read_head_table(self->src, &self->head_table);
                read_flags |= OTF_READ_TAG_HEAD;
                break;
            case OTF_TAG_MAXP:
                if(read_flags & OTF_READ_TAG_MAXP) {
                    err = OTF_E_FILE_FORMAT;
                    goto error_exit;
                }
                err = otf_read_maxp_table(self->src, &self->maxp_table);
                read_flags |= OTF_READ_TAG_MAXP;
                break;
            default:
                continue;
        }
        if(OTF_FAILED(err)) {
            err = OTF_E_READ;
            goto error_exit;
        }
        if(read_flags == (OTF_READ_TAG_CMAP | OTF_READ_TAG_HEAD | OTF_READ_TAG_MAXP)) {
            break;
        }
    }

    /* read outlines table */
    read_flags = 0;
    for(i = 0; i < self->offset_table.num_tables; i++) {
        otf_table_dir_entry_t *entry = &self->table_dir_entries[i];
        OTF_TRAP(otf_seek(self->src, entry->offset));
        switch (entry->tag) {
            case OTF_TAG_LOCA:
                if(read_flags & OTF_READ_TAG_LOCA) {
                    err = OTF_E_FILE_FORMAT;
                    goto error_exit;
                }
                err = otf_read_loca_table(self, &self->loca_table);
                read_flags |= OTF_READ_TAG_LOCA;
                break;
            case OTF_TAG_GLYF:
                if(read_flags & OTF_READ_TAG_GLYF) {
                    err = OTF_E_FILE_FORMAT;
                    goto error_exit;
                }
                self->glyf_table_offset = entry->offset;
                read_flags |= OTF_READ_TAG_GLYF;
            default:
                continue;
        }
        if(OTF_FAILED(err)) {
            err = OTF_E_READ;
            goto error_exit;
        }
    }

    return OTF_S_OK;

error_exit:
    otf_finalize(self);

    return err;
}


otf_result_t otf_load_font(otf_heap_t *heap, otf_source_t *src, uint32_t flags, otf_font_t **font)
{
    otf_font_t *f;
    otf_result_t r;

    f = (otf_font_t *)otf_malloc(heap, sizeof(otf_font_t));
    if(f == NULL) {
        return OTF_E_MEM;
    }

    // zero clear
    memset(f, 0, sizeof(otf_font_t));

    r = otf_load_impl(f, heap, src, flags);
    if(OTF_FAILED(r)) {
        otf_free(heap, f);
        return r;
    }

    *font = f;

    return OTF_S_OK;

}

void otf_finalize(otf_font_t *self)
{
    otf_heap_t *heap = self->heap;
    if(self->table_dir_entries != NULL) {
        otf_free(heap, self->table_dir_entries);
        self->table_dir_entries = NULL;
    }
    if(self->loca_table.u.long_offsets != NULL) {
        otf_free(heap, self->loca_table.u.long_offsets);
        self->loca_table.u.long_offsets = NULL;
    }

    if(self->src != NULL && (self->flags & OTF_FONT_FLAG_AUTO_CLOSE_SOURCE) != 0) {
        otf_close(self->src);
        self->src = NULL;
    }
}

void otf_free_font(otf_font_t *font)
{
    otf_finalize(font);
    otf_free(font->heap, font);
}

#define TTF_COORDINATE_FLAG_ON_CURVE                    0x01
#define TTF_COORDINATE_FLAG_X_SHORT_VECTOR              0x02
#define TTF_COORDINATE_FLAG_Y_SHORT_VECTOR              0x04
#define TTF_COORDINATE_FLAG_REPEAT                      0x08
#define TTF_COORDINATE_FLAG_X_IS_SAME_OR_POSITIVE       0x10
#define TTF_COORDINATE_FLAG_Y_IS_SAME_OR_POSITIVE       0x20

otf_result_t otf_read_glyph_header(otf_source_t *src, otf_glyph_header_t *header)
{
    int16_t number_of_contours;
    int16_t x_min, y_min, x_max, y_max;

    if(OTF_FAILED(otf_read_u16b(src, (uint16_t *)&number_of_contours))) {
        return OTF_E_READ;
    }
    if(OTF_FAILED(otf_read_u16b(src, (uint16_t *)&x_min))) {
        return OTF_E_READ;
    }
    if(OTF_FAILED(otf_read_u16b(src, (uint16_t *)&y_min))) {
        return OTF_E_READ;
    }
    if(OTF_FAILED(otf_read_u16b(src, (uint16_t *)&x_max))) {
        return OTF_E_READ;
    }
    if(OTF_FAILED(otf_read_u16b(src, (uint16_t *)&y_max))) {
        return OTF_E_READ;
    }

    header->number_of_contours = number_of_contours;
    header->x_min = x_min;
    header->y_min = y_min;
    header->x_max = x_max;
    header->y_max = y_max;

    return OTF_S_OK;
}

otf_result_t otf_read_simpl_glyph_desc(otf_heap_t *heap, otf_source_t *src, otf_glyf_table_t *glyf)
{
    otf_result_t err;
    uint16_t *end_pts_of_contours = NULL;
    uint16_t instructions_length;
    uint8_t *instructions = NULL;
    uint8_t *flags = NULL;
    int16_t *x_coordinates = NULL;
    int16_t *y_coordinates = NULL;
    uint32_t i;
    uint32_t flags_length;

    end_pts_of_contours = (uint16_t *)otf_malloc(heap, sizeof(uint16_t) * glyf->header.number_of_contours);
    if(end_pts_of_contours == NULL) {
        err = OTF_E_MEM;
        goto error_exit;
    }

    OTF_TRAP(otf_read_u16b_n(src, end_pts_of_contours, glyf->header.number_of_contours));

    OTF_TRAP(otf_read_u16b(src, &instructions_length));

    instructions = (uint8_t *)otf_malloc(heap, instructions_length);
    if(instructions == NULL) {
        err = OTF_E_MEM;
        goto error_exit;
    }

    OTF_TRAP(otf_read(src, instructions, instructions_length));

    flags_length = end_pts_of_contours[glyf->header.number_of_contours - 1] + 1;
    flags = (uint8_t *)otf_malloc(heap, flags_length);
    if(flags == NULL) {
        err = OTF_E_MEM;
        goto error_exit;
    }

    OTF_TRAP(otf_read(src, flags, flags_length));

    x_coordinates = (int16_t *)otf_malloc(heap, sizeof(int16_t) * flags_length);
    if(x_coordinates == NULL) {
        err = OTF_E_MEM;
        goto error_exit;
    }

    y_coordinates = (int16_t *)otf_malloc(heap, sizeof(int16_t) * flags_length);
    if(y_coordinates == NULL) {
        err = OTF_E_MEM;
        goto error_exit;
    }

    /* read x coordinates */
    {
        int32_t prev_x;
        for(i = 0; i < flags_length; i++) {
            uint8_t flag = flags[i];
            int16_t dx;
            if((flag & TTF_COORDINATE_FLAG_X_SHORT_VECTOR) != 0) {
                uint8_t data;
                OTF_TRAP(otf_read_u8(src, &data));
                dx = data;
                if((flag & TTF_COORDINATE_FLAG_X_IS_SAME_OR_POSITIVE) == 0) {
                    dx = -dx;
                }
            } else {
                if((flag & TTF_COORDINATE_FLAG_X_IS_SAME_OR_POSITIVE) != 0) {
                    dx = 0;
                } else {
                    OTF_TRAP(otf_read_u16b(src, (uint16_t *)&dx));
                }
            }
            prev_x += dx;
            x_coordinates[i] = (int16_t)prev_x;
        }
    }

    /* read y coordinates */
    {
        int32_t prev_y = 0;
        for(i = 0; i < flags_length; i++) {
            uint8_t flag = flags[i];
            int16_t dy;
            if((flag & TTF_COORDINATE_FLAG_Y_SHORT_VECTOR) != 0) {
                uint8_t data;
                OTF_TRAP(otf_read_u8(src, &data));
                dy = data;
                if((flag & TTF_COORDINATE_FLAG_Y_IS_SAME_OR_POSITIVE) == 0) {
                    dy = -dy;
                }
            } else {
                if((flag & TTF_COORDINATE_FLAG_Y_IS_SAME_OR_POSITIVE) != 0) {
                    dy = 0;
                } else {
                    OTF_TRAP(otf_read_u16b(src, (uint16_t *)&dy));
                }
            }
            prev_y += dy;
            y_coordinates[i] = (int16_t)prev_y;
        }
    }

    glyf->u.simple.end_pts_of_contours = end_pts_of_contours;
    glyf->u.simple.instructions_length = instructions_length;
    glyf->u.simple.instructions = instructions;
    glyf->u.simple.flags = flags;
    glyf->u.simple.x_coordinates = x_coordinates;
    glyf->u.simple.y_coordinates = y_coordinates;

    return OTF_S_OK;

error_exit:

    if(end_pts_of_contours != NULL) {
        otf_free(heap, end_pts_of_contours);
    }
    if(instructions != NULL) {
        otf_free(heap, instructions);
    }
    if(flags != NULL) {
        otf_free(heap, flags);
    }
    if(x_coordinates != NULL) {
        otf_free(heap, x_coordinates);
    }
    if(y_coordinates != NULL) {
        otf_free(heap, y_coordinates);
    }

    return err;
}

void otf_free_glyf_table(otf_font_t *self, otf_glyf_table_t *table)
{
    otf_heap_t *heap = self->heap;

    if(table->header.number_of_contours >= 0) {
        if(table->u.simple.end_pts_of_contours != NULL) {
            otf_free(heap, table->u.simple.end_pts_of_contours);
        }
        if(table->u.simple.instructions != NULL) {
            otf_free(heap, table->u.simple.instructions);
        }
        if(table->u.simple.x_coordinates != NULL) {
            otf_free(heap, table->u.simple.x_coordinates);
        }
        if(table->u.simple.y_coordinates != NULL) {
            otf_free(heap, table->u.simple.y_coordinates);
        }
    }

    otf_free(heap, table);
}

uint32_t otf_get_glyf_table_offset(otf_font_t *self, uint32_t index)
{
    if(self->head_table.index_to_loc_format == OTF_INDEX_TO_LOC_FORMAT_SHORT) {
        return self->loca_table.u.short_offsets[index] * 2;
    } else {
        return self->loca_table.u.long_offsets[index];
    }
}

/* 指定したインデックスのグリフを取得する */
otf_result_t otf_get_glyf_table(otf_font_t *self, uint32_t index, otf_glyf_table_t **table)
{
    otf_result_t err;
    otf_source_t *src;
    uint32_t glyf_table_offset;
    otf_glyf_table_t *glyf_table = NULL;

    glyf_table_offset = self->glyf_table_offset + otf_get_glyf_table_offset(self, index);
    src = self->src;

    glyf_table = (otf_glyf_table_t *)otf_malloc(self->heap, sizeof(otf_glyf_table_t));
    if(glyf_table == NULL) {
        err = OTF_E_MEM;
        goto error_exit;
    }

    memset(glyf_table, 0, sizeof(otf_glyf_table_t));

    OTF_TRAP(otf_seek(src, glyf_table_offset));
    OTF_TRAP(otf_read_glyph_header(src, &glyf_table->header));
    OTF_TRAP(otf_read_simpl_glyph_desc(self->heap, self->src, glyf_table));

    *table = glyf_table;

    return OTF_S_OK;

error_exit:
    if(glyf_table != NULL) {
        otf_free(self->heap, glyf_table);
    }

    return err;
}

otf_result_t otf_make_outline(otf_font_t *self, otf_glyf_table_t *table,
                              otf_add_path_seg_cb_t add_path_seg_cb,
                              void *user_data)
{
    otf_result_t err;
    int16_t number_of_contours;

    number_of_contours = table->header.number_of_contours;

    if(number_of_contours >= 0) {
        /* single glyph */

        uint32_t i, index, start_index, end_index;
        uint8_t *flags = table->u.simple.flags;
        int16_t *x_coordinates = table->u.simple.x_coordinates;
        int16_t *y_coordinates = table->u.simple.y_coordinates;
        uint16_t *end_pts_of_contours = table->u.simple.end_pts_of_contours;

        start_index = 0;
        for(i = 0; i < number_of_contours; i++) {
            int32_t x, y, flag, px, py, pflag;
            int32_t done_move = 0;

            end_index = end_pts_of_contours[i];

            if(start_index == end_index) {
                continue;
            }

            x = x_coordinates[start_index];
            y = y_coordinates[start_index];
            flag = flags[start_index];

            if((flag & TTF_COORDINATE_FLAG_ON_CURVE) != 0) {
                done_move = 1;
                add_path_seg_cb(user_data, OTF_PATH_MOVE_TO, x, y, 0, 0);
            }

            px = x;
            py = y;
            pflag = flag;

            for(index = start_index + 1; index <= end_index; ) {

                x = x_coordinates[index];
                y = y_coordinates[index];
                flag = flags[index];

                /* OFFカーブが連続したなら、ONカーブの中点を挿入する */
                if((flag & TTF_COORDINATE_FLAG_ON_CURVE) == 0 && (pflag & TTF_COORDINATE_FLAG_ON_CURVE) == 0) {
                    x = (x + px) / 2;
                    y = (y + py) / 2;
                    flag = TTF_COORDINATE_FLAG_ON_CURVE;
                } else {
                    index++;
                }

                if((flag & TTF_COORDINATE_FLAG_ON_CURVE) != 0) {
                    if((pflag & TTF_COORDINATE_FLAG_ON_CURVE) != 0) {
                        /* ON->ONなら、直線で結ぶ */
                        add_path_seg_cb(user_data, OTF_PATH_LINE_TO, x, y, 0, 0);
                    } else {
                        /* OFF->ON */
                        if(!done_move) {
                            add_path_seg_cb(user_data, OTF_PATH_MOVE_TO, x, y, 0, 0);
                            done_move = 1;
                        } else {
                            add_path_seg_cb(user_data, OTF_PATH_QUAD_TO, px, py, x, y);
                        }
                    }
                }

                pflag = flag;
                px = x;
                py = y;
            }

            /* 終了点から開始点へ繋ぐ */
            for(index = start_index; index <= end_index; ) {

                x = x_coordinates[index];
                y = y_coordinates[index];
                flag = flags[index];

                /* OFFカーブが連続したなら、ONカーブの中点を挿入する */
                if((flag & TTF_COORDINATE_FLAG_ON_CURVE) == 0 && (pflag & TTF_COORDINATE_FLAG_ON_CURVE) == 0) {
                    x = (x + px) / 2;
                    y = (y + py) / 2;
                    flag = TTF_COORDINATE_FLAG_ON_CURVE;
                } else {
                    index++;
                }

                if((flag & TTF_COORDINATE_FLAG_ON_CURVE) != 0) {
                    if((pflag & TTF_COORDINATE_FLAG_ON_CURVE) != 0) {
                        add_path_seg_cb(user_data, OTF_PATH_LINE_TO, x, y, 0, 0);
                    } else {
                        if(!done_move) {
                            add_path_seg_cb(user_data, OTF_PATH_MOVE_TO, x, y, 0, 0);
                            done_move = 1;
                        } else {
                            add_path_seg_cb(user_data, OTF_PATH_QUAD_TO, px, py, x, y);
                        }
                    }
                    /* 結べたら、終了 */
                    break;
                }

                pflag = flag;
                px = x;
                py = y;
            }

            add_path_seg_cb(user_data, OTF_PATH_CLOSE_PATH, 0, 0, 0, 0);

            start_index = end_index + 1;
        }
    }

    return OTF_S_OK;

error_exit:

    return err;
}

