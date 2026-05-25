#include <stdio.h> //input dan output 
#include <stdlib.h> 
#include <conio.h> //getch()
#include <graphics.h>
int main()
{
    int gm, x, y, gd = DETECT;
    char data[] = "C:\\MinGW\\lib\\libbgi.a"; //static file

    initgraph(&gd, &gm, data);
    x = getmaxx(); // to get the co-ordinates i.e. x & y
    y = getmaxy();
    cleardevice();
    
    //coding graphics
    int x_awal = x/2;
    while(x_awal<x)
    {    
        putpixel(x_awal++,y/2,WHITE);    
        printf("Max Koordinat X : %d\n",x);
    }
    x_awal = x/2;
    int y_awal = y/2 + 10;
    line(x_awal,y_awal,x,y_awal);
    printf("Max Koordinat X : %d\n",x);
    printf("Max Koordinat Y : %d\n",y);

    getch();
    closegraph();

    return 0;
}