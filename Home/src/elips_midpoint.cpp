#include <stdio.h> //input dan output 
#include <stdlib.h> 
#include <conio.h> //getch()
#include <graphics.h>
#include <windows.h>

int xc,yc, rx, ry;

void elipsPlotpoint(int xc, int yc, int x, int y)
{
    putpixel(xc + x,yc + y,WHITE);
    putpixel(xc - x,yc + y,WHITE);
    putpixel(xc + x,yc - y,WHITE);
    putpixel(xc - x,yc - y,WHITE);
}

void elipsMidpoint(int xc, int yc, int rx, int ry)
{
    int rx2 = rx * rx;
    int ry2 = ry * ry;
    
    //region 1
    int x1 = 0;
    int y1 = ry;
    int p1 = ry2 - ry * rx2;
    //Gambar Elips utk 4 area
    elipsPlotpoint(xc,yc,x1,y1);
    do
    {
        if(p1 <=0)
        {    
            x1 = x1 + 1;
            p1 = p1 + (2 * x1 + 3) * ry2;
        }
        else
        {
            x1 = x1 + 1;
            y1 = y1 - 1; 
            p1 = p1 + (2 * x1 + 3) * ry2 + (-2 * y1 + 2) * rx2;
        }
        //Gambar Elips utk 4 area
        elipsPlotpoint(xc,yc,x1,y1);
    }while(ry2*x1<=rx2*y1);

    //region 2
    int x2 = rx;
    int y2 = 0;
    int p2 = rx2 - rx * ry2;
    //Gambar Elips utk 4 area
    elipsPlotpoint(xc,yc,x2,y2);
    do
    {
        if(p2 <=0)
        {    
            y2 = y2 + 1;
            p2 = p2 + (2 * y2 + 3) * rx2;
        }
        else
        {
            x2 = x2 - 1;
            y2 = y2 + 1; 
            p2 = p2 + (2 * y2 + 3) * rx2 + (-2 * x2 + 2) * ry2;
        }
        //Gambar Elips utk 4 area
        elipsPlotpoint(xc,yc,x2,y2);
    }while(rx2*y2<=ry2*x2);
}

int main()
{
    int gm, gd = DETECT;
    char data[] = "C:\\MinGW\\lib\\libbgi.a"; //static file

    initgraph(&gd, &gm, data);
    cleardevice();
    
    //coding graphics
    printf("Masukkan Nilai Koordinat Titik Tengah Ellips:");
    scanf("%d %d",&xc, &yc);
    printf("Masukkan Nilai Radius Ellips (X dan Y):");
    scanf("%d %d",&rx,&ry);

    elipsMidpoint(xc,yc,rx,ry);
    
    system("pause");
    getch();
    closegraph();
    return 0;
}