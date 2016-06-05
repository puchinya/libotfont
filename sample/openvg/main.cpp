//
// Created by 鍋島雅貴 on 2016/06/04.
//

#include <glut/glut.h>
#include <MonkVG/openvg.h>
#include <MonkVG/vgu.h>
#include <MonkVG/vgext.h>
#include <OpenType.h>
#include "malloc_heap.h"
#include "file_source.h"

VGPaint _paint;
VGPath _path;
VGPath s_font_path;

int screenWidth = 320;
int screenHeight = 480;

otf_result_t add_path_seg_cb(void *user_data,
                                              uint8_t seg, int16_t p0, int16_t p1, int16_t p2, int16_t p3)
{
    VGubyte segs[1];
    VGfloat params[4];
    p0 = p0 / 16;
    p1 = p1 / 16;
    p2 = p2 / 16;
    p3 = p3 / 16;
    switch(seg) {
        case OTF_PATH_MOVE_TO:
            segs[0] = VG_MOVE_TO_ABS;
            params[0] = p0;
            params[1] = p1;
            printf("M %d %d\n", p0, p1);
            break;
        case OTF_PATH_LINE_TO:
            segs[0] = VG_LINE_TO_ABS;
            params[0] = p0;
            params[1] = p1;
            printf("L %d %d\n", p0, p1);
            break;
        case OTF_PATH_QUAD_TO:
            segs[0] = VG_QUAD_TO_ABS;
            params[0] = p0;
            params[1] = p1;
            params[2] = p2;
            params[3] = p3;
            printf("Q %d %d %d %d\n", p0, p1, p2, p3);
            break;
        case OTF_PATH_CLOSE_PATH:
            segs[0] = VG_CLOSE_PATH;
            printf("Z\n");
            break;
        default:
            return OTF_S_OK;
    }

    vgAppendPathData(s_font_path, 1, segs, params);

    return OTF_S_OK;
}

void load_font()
{
    otf_font_t *font;
    cfile_source_t src;

    cfile_source_open(&src, "OpenSans-Regular.ttf");

    otf_load_font(malloc_heap_instance(), (otf_source_t *)&src, OTF_FONT_FLAG_AUTO_CLOSE_SOURCE, &font);
    otf_glyf_table_t *glyf_table;

    otf_get_glyf_table(font, 35, &glyf_table);

    s_font_path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
    otf_make_outline(font, glyf_table, add_path_seg_cb, 0);

    otf_free_glyf_table(font, glyf_table);

    otf_free_font(font);

}

void init() {
    // setup the OpenVG context
    vgCreateContextMNK( 320, 480, VG_RENDERING_BACKEND_TYPE_OPENGLES11 );

    // create a paint
    _paint = vgCreatePaint();
    vgSetPaint(_paint, VG_FILL_PATH );
    VGfloat color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
    vgSetParameterfv(_paint, VG_PAINT_COLOR, 4, &color[0]);

    // create a box path
    _path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F,1,0,0,0, VG_PATH_CAPABILITY_ALL);
    vguRect( _path, 50.0f, 50.0f, 90.0f, 50.0f );

    // Load font
    load_font();
}

void draw() {

    glClear(GL_COLOR_BUFFER_BIT);

    /// draw the basic path
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
    vgLoadIdentity();
    vgTranslate( screenWidth/2, screenHeight/2 );
    vgSetPaint( _paint, VG_FILL_PATH );
    //vgDrawPath( _path, VG_FILL_PATH );
    vgDrawPath(s_font_path, VG_FILL_PATH);

    glFlush();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("vgsample");
    glutInitDisplayMode(GLUT_RGBA);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    init();
    glutDisplayFunc(draw);
    glutMainLoop();

    return 0;
}