#include <stdio.h> //printf, scanf, etc
#include <stdlib.h> //getch, etc
#include <conio.h> 
#include <graphics.h>
int main()
{
    int gm, gd = DETECT;   
    char data[] = "C:\\MinGW\\lib\\libbgi.a"; //static file
    int x_max, y_max;
    initgraph(&gd, &gm, data);
    cleardevice();

    x_max = getmaxx();
    y_max = getmaxy();
    setcolor(RED);
    line(0, y_max/2, x_max, y_max/2);
    setcolor(GREEN);
    line(x_max/2, 0, x_max/2, y_max);
    putpixel(10, 10, YELLOW); //x, y, color
    //enter script here
    printf("Max X: %d; Max Y: %d\n", x_max, y_max);


    getch();
    closegraph();

    return 0;
}