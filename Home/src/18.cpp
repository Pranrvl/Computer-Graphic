#include <GL/freeglut.h>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <GL/freeglut.h>

#define PI 3.14159265
#define N 40.0

static int isWire = 0;
static long font = (long)GLUT_BITMAP_8_BY_13;

void writeBitmapString(void *font, const char *string) {
    const char *c;
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void drawDisc(float R, float X, float Y, float Z) {
    float t;
    int i;
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(X, Y, Z); // center of disc
    for (int i = 0; i <= N; i++) {
        t = 2 * PI * i / N;
        glVertex3f(X + cos(t) * R, Y + sin(t) * R, Z);
    }
    glEnd();
}

void drawScene(void) {
    float angle;
    int i;
    /*
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.75, 0.8);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPointSize(5.0);
    glBegin(GL_TRIANGLE_STRIP); //GL_POLYGON, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP
    glVertex3f(10.0, 90.0, 0.0);
    glVertex3f(10.0, 10.0, 0.0);
    glVertex3f(35.0, 75.0, 0.0);
    glVertex3f(30.0, 20.0, 0.0);
    glVertex3f(90.0, 90.0, 0.0);
    glVertex3f(80.0, 40.0, 0.0);
    // glVertex3f(20.0, 20.0, 0.0);
    // glVertex3f(80.0, 20.0, 0.0);
    // glVertex3f(80.0, 80.0, 0.0);
    // glVertex3f(20.0, 80.0, 0.0);
    glEnd();
    glutSwapBuffers();
    */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT, GL_FILL);
    glColor3f(1.0, 0.0, 0.0);
    drawDisc(20.0, 25.0, 75.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    drawDisc(10.0, 25.0, 75.0, 0.0);

    glEnable(GL_DEPTH_TEST);
    glColor3f(1.0, 0.0, 0.0);
    drawDisc(20.0, 75.0, 75.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    drawDisc(10.0, 75.0, 75.0, 0.5);
    glDisable(GL_DEPTH_TEST);

    if (isWire) {
        glPolygonMode(GL_FRONT, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT, GL_FILL);
    }

    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= N; i++) {
        angle = 2 * PI * i / N;
        glVertex3f(50 + cos(angle) * 10.0, 30.0 + sin(angle) * 10.0, 0.0);
        glVertex3f(50 + cos(angle) * 20.0, 30.0 + sin(angle) * 20.0, 0.0);
    }
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    glRasterPos3f(15.0, 51.0, 0.0);
    writeBitmapString((void *)font, "Overwritten");
    glRasterPos3f(69.0, 51.0, 0.0);
    writeBitmapString((void *)font, "Floating");
    glRasterPos3f(38.0, 6.0, 0.0);
    writeBitmapString((void *)font, "The Real Deal");

    glutSwapBuffers();
    

    //glFlush();
}

void setUp(void) {
    glClearColor(1.0, 1.0, 1.0, 0.0);
}

void resize(int w, int h) {
    //if ()
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
    // glOrtho(0.0, 200.0, 0.0, 200.0, -1.0, 1.0);
    // glOrtho(20.0, 80.0, 20.0, 90.0, -1.0, 1.0);
    glOrtho(0.0, 100.0, 0.0, 100.0, -2.0, 5.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyInput(unsigned char key, int x, int y) {
    switch (key) {
        case ' ':
            if (isWire == 0) {
                isWire = 1;
            } else {
                isWire = 0;
            }
            glutPostRedisplay();
            break;
        case 27: // Escape key
            exit(0);
            break;
        default:
            break;
    }
}

void printInteraction() {
    std::cout << "Interaction:" << std::endl;
    std::cout << "Press the space bar to toggle between wireframe and filled for lower" << std::endl;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.0f, 0.5f);
    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    // glutInit(&argc, argv);
    // glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    // glutInitWindowSize(800, 600);
    // glutCreateWindow("OpenGL GLUT Example");
    // glutDisplayFunc(display);
    // glutMainLoop();
    // return 0;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    //glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Drawing a Square");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    setUp();
    glutMainLoop();
}