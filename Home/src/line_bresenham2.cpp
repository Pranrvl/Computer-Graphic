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
    int xa  = 50, ya = 20, xb = 100, yb = 60;
    int delta_x, delta_y, p;
    int x_mid = xmax/2;
    int y_mid = ymax/2;
    int x, y, i;
 
    setcolor(RED);
    line(0, y_mid, xmax, y_mid);
    line(x_mid, 0, x_mid, ymax);
    
    delta_x = xb - xa;
    delta_y = yb - ya;
    

    x = xa, y = ya;
    putpixel(x + x_mid, y_mid - y , WHITE); 
    i = 0;
    do
    {
        if (p < 0)
        {
            y = y + 1;
            p = p + 2 * delta_x;
            putpixel(x + x_mid, y_mid - y, WHITE);
        }
        else
        {
            x = x + 1;
            y = y + 1;
            p = p + 2 * delta_x - 2 * delta_y;
            putpixel(x + x_mid, y_mid - y, WHITE);
        }
        i++;
    }
    while(i<=delta_y);
    
    getch();
    closegraph();

    return 0;
}