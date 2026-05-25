#include <GL/freeglut.h>

void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.7, 0.0, 0.7);

    glPointSize(5.0);
    glBegin(GL_POLYGON); //GL_POLYGON, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_POINTS
        glVertex3f(20.0, 20.0, 0.0);
        glVertex3f(80.0, 20.0, 0.0);
        glVertex3f(90.0, 50.0, 0.0);
        glVertex3f(80.0, 80.0, 0.0);
        glVertex3f(20.0, 80.0, 0.0);
    glEnd();
    glFlush();
}

void setUp(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
}

void resize(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 100.0, 0.0, 100.0, -2.0, 5.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyInput(unsigned char key, int x, int y)
{
    switch(key){
        case 27:
            exit(0);
            break;
        default:
            break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(500, 580);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Drawing a Square");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    setUp();
    glutMainLoop();
}

//g++ main.cpp -lfreeglut -lopengl32 -lglu32 -o main.exe