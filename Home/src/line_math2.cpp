#include <stdio.h> //input dan output 
#include <stdlib.h> 
#include <conio.h> //getch()
#include <graphics.h>
int main()
{
    int gm, gd = DETECT;
    char data[] = "C:\\MinGW\\lib\\libbgi.a"; //static file

    initgraph(&gd, &gm, data);
    cleardevice();
    
    //coding graphics
    int xa  = 500, ya = 200, xb = 600, yb = 400;
    float m,c,x,y;
 
    m = ((float)yb - (float)ya) / ((float)xb - (float)xa);
    c = (float)ya - (m * (float)xa); 

    x = xa, y = ya;
    putpixel(x, y, WHITE); 
    do
    {
        y = y + 1;
        x = (y - c) / m;
        putpixel(x,y, WHITE);
    }
    while(y<=yb);
    
    getch();
    closegraph();

    return 0;
}