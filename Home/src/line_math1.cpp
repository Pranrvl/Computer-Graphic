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
    int xa  = 100, ya = 100, xb = 500, yb = 400;
    float m,c,x,y;
 
    m = ((float)yb - (float)ya) / ((float)xb - (float)xa);
    c = (float)ya - (m * (float)xa); 

    x = xa, y = ya;
    putpixel(x, y, WHITE); 
    do
    {
        x = x + 1;
        y = (m * x) + c;
        putpixel(x,y, WHITE);
    }
    while(x<=xb);
    
    getch();
    closegraph();

    return 0;
}