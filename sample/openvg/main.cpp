//
// Created by 鍋島雅貴 on 2016/06/04.
//

#include <glut/glut.h>
#include <MonkVG/openvg.h>
#include <MonkVG/vgu.h>
#include <MonkVG/vgext.h>

VGPaint _paint;
VGPath _path;

int screenWidth = 320;
int screenHeight = 480;

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

}

void draw() {

    glClear(GL_COLOR_BUFFER_BIT);

    /// draw the basic path
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
    vgLoadIdentity();
    vgTranslate( screenWidth/2, screenHeight/2 );
    vgSetPaint( _paint, VG_FILL_PATH );
    vgDrawPath( _path, VG_FILL_PATH );

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