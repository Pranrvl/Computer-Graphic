#include <stdio.h> //input dan output 
#include <stdlib.h> 
#include <conio.h> //getch()
#include <graphics.h>
#include <windows.h>
#include <math.h> //pakai PI

int xc,yc, r;
float theta_s, theta_e;

void circleTripoint(int xc, int yc, int r, float theta_s, float theta_e)
{
    float theta_inc = 1 / float(r);
    float theta, x, y;
    theta_s = theta_s * M_PI / 180;
    theta_e = theta_e * M_PI / 180;
    printf("%.2f, %.2f\n", theta_s,theta_e);
    if (theta_e < theta_s)
    {
        theta_e = 2 * M_PI + theta_e;
    }
    theta = theta_s;
    do 
    {
        x = float(r) * cos(theta);
        y = float(r) * sin(theta);
        putpixel(round(xc + x),round(yc + y),WHITE);
        theta = theta + theta_inc;
    }while (theta <= theta_e);
}

int main()
{
    int gm, gd = DETECT;
    char data[] = "C:\\MinGW\\lib\\libbgi.a"; //static file

    initgraph(&gd, &gm, data);
    cleardevice();
    
    //coding graphics
    printf("Masukkan Nilai Koordinat Titik Tengah Lingkaran:");
    scanf("%d %d",&xc, &yc);
    printf("Masukkan Nilai Radius Lingkaran:");
    scanf("%d",&r);
    printf("Masukkan Nilai sudut awal Lingkaran:");
    scanf("%f",&theta_s);
    printf("Masukkan Nilai sudut akhir Lingkaran:");
    scanf("%f",&theta_e);

    circleTripoint(xc,yc,r,theta_s, theta_e);
    
    system("pause");
    getch();
    closegraph();
    return 0;
}