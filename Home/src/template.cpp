#include <stdio.h> //printf, scanf, etc
#include <stdlib.h> //getch, etc
#include <conio.h> 
#include <graphics.h>
int main()
{
    int gm, gd = DETECT;   
    char data[] = "C:\\MinGW\\lib\\libbgi.a"; //static file

    initgraph(&gd, &gm, data);
    cleardevice();


    
    getch();
    closegraph();

    return 0;
}