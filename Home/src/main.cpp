#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h> 

// NOTES: Program visualisasi bumi dengan kontrol lampu

GLuint texEarth;
GLuint texSun;
GLUquadric* quad;

// [FITUR X] Variabel global untuk status lampu
bool lampuMatahariNyala = true; 

// Variabel rotasi & revolusi
float rotasiMatahari = 0.0f;
float rotasiBumi = 0.0f;
float revolusiBumi = 0.0f;
float kecepatanRevolusi = 0.5f;

GLuint loadTexture(const char* filename)
{
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);

    if (!data) {
        printf("Gagal load texture: %s\n", filename);
        return 0; 
    }

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexImage2D(GL_TEXTURE_2D, 0, format,
                 width, height, 0,
                 format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(data);
    return texID;
}

void drawScene()
{
    glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    texEarth = loadTexture("earth.jpg");
    texSun = loadTexture("sun.jpg");

    quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluQuadricNormals(quad, GLU_SMOOTH);
}

void reShape(int w, int h)
{
    if (h == 0) h = 1;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / h, 0.1, 300.0);
    glMatrixMode(GL_MODELVIEW);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0, 20.0, 70.0, 
              0.0, 0.0, 0.0, 
              0.0, 1.0, 0.0); 

    glEnable(GL_LIGHTING);

    // [FITUR X] Logika Toggle: Cek status variabel lalu nyalakan/matikan GL_LIGHT0
    if (lampuMatahariNyala) {
        glEnable(GL_LIGHT0);
    } else {
        glDisable(GL_LIGHT0);
    }

    glEnable(GL_LIGHT1);

    GLfloat lightPos[]  = { 0.0f, 0.0f, 0.0f, 1.0f }; 
    GLfloat lightAmb[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat lightDiff[] = { 1.0f, 1.0f, 0.9f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiff);

    GLfloat light1_pos[]   = { -4.0f, -2.0f, -3.0f, 1.0f };  
    GLfloat light1_amb[]   = { 0.1f,  0.1f,  0.2f, 1.0f };  
    GLfloat light1_diff[]  = { 0.6f,  0.6f,  1.0f, 1.0f };   

    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
    glLightfv(GL_LIGHT1, GL_AMBIENT,  light1_amb);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light1_diff);

    GLfloat sunDiffuse[]  = { 1.0f, 0.9f, 0.3f, 1.0f }; 
    GLfloat sunEmission[] = { 1.0f, 0.9f, 0.3f, 1.0f }; 
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE,  sunDiffuse); 
    glMaterialfv(GL_FRONT, GL_EMISSION, sunEmission); 

    glPushMatrix();
        glRotatef(rotasiMatahari, 0.0f, 1.0f, 0.0f);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texSun);
        glScalef(5.0f, 5.0f, 5.0f); 
        gluSphere(quad, 1.0, 50, 50); 
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    GLfloat noEmi[] = {0,0,0,1};
    glMaterialfv(GL_FRONT, GL_EMISSION, noEmi);

    float radiusRevolusi = 25.0f;
    const float DEGkeRAD = 3.141592f / 180.0f;

    float earthX = cos(revolusiBumi * DEGkeRAD) * radiusRevolusi; 
    float earthZ = sin(revolusiBumi * DEGkeRAD) * radiusRevolusi; 

    glPushMatrix();
        glTranslatef(earthX, 0.0f, earthZ);
        glRotatef(rotasiBumi, 0.0f, 1.0f, 0.0f);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texEarth);

        glScalef(2.0f, 2.0f, 2.0f);
        gluSphere(quad, 1.0, 50, 50); 

        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glutSwapBuffers();
}

void timer(int value)
{
    rotasiMatahari += 0.5f;
    if (rotasiMatahari > 360) rotasiMatahari -= 360;
    
    rotasiBumi += 1.0f;
    if (rotasiBumi > 360) rotasiBumi -= 360;

    revolusiBumi += kecepatanRevolusi;
    if (revolusiBumi > 360) revolusiBumi -= 360;
    if (revolusiBumi < 0) revolusiBumi += 360;

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void specialKeys(int key, int x, int y)
{
    if (key == GLUT_KEY_LEFT)
        kecepatanRevolusi = +0.5f; 

    if (key == GLUT_KEY_RIGHT)
        kecepatanRevolusi = -0.5f; 
}

// [FITUR X] Fungsi Keyboard untuk mendeteksi input tombol X
void keyboard(unsigned char key, int x, int y)
{
    if (key == 'x' || key == 'X') 
    {
        lampuMatahariNyala = !lampuMatahariNyala; // Ubah true jadi false, dan sebaliknya
        if(lampuMatahariNyala) printf("Lampu: ON\n");
        else printf("Lampu: OFF\n");
    }

    if (key == 27) exit(0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 700);
    glutCreateWindow("Earth Orbiting Sun - Tekan 'X' untuk Lampu");

    drawScene();
    glutDisplayFunc(display);
    glutReshapeFunc(reShape);
    
    glutKeyboardFunc(keyboard); // [FITUR X] Panggil fungsi keyboard
    glutSpecialFunc(specialKeys);
    
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}