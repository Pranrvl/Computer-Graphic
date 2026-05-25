#include <stdio.h> //input dan output 
#include <stdlib.h> 
#include <conio.h> //getch()
#include <graphics.h>

int main()
{
    int xmax, ymax,gm, gd = DETECT;
    char data[] = "C:\\MinGW\\lib\\libbgi.a"; //static file

    initgraph(&gd, &gm, data);
    xmax = getmaxx(); // to get the co-ordinates i.e. x & y
    ymax = getmaxy();
    cleardevice();
    
    //coding graphics
    int xa  = 0, ya = 60, xb = -30, yb = 0;
    int delta_x, delta_y, dda;
    int x_mid = xmax/2;
    int y_mid = ymax/2;
    float x_inc, y_inc, x, y, i;
 
    setcolor(RED);
    line(0, y_mid, xmax, y_mid);
    line(x_mid, 0, x_mid, ymax);
    
    delta_x = xb - xa;
    delta_y = yb - ya;

    if (abs(delta_y) < abs(delta_x)) dda = abs(delta_x);
    else dda = abs(delta_y);
    x_inc = delta_x / (float)dda;
    y_inc = delta_y / (float)dda;

    x = xa, y = ya;
    putpixel(x + x_mid, y_mid - y , WHITE); 
    i = 0;
    do
    {
        x = x + x_inc;
        y = y + y_inc;
        putpixel(x + x_mid, y_mid - y, WHITE);
        i++;
    }
    while(i<=dda);
    
    getch();
    closegraph();

    return 0;
}