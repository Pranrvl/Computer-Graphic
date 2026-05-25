#include <stdio.h> //input dan output 
#include <stdlib.h> 
#include <conio.h> //getch()
#include <graphics.h>

int x_mid, y_mid;
int koor[20][2];

void dda_line(int xa, int ya, int xb, int yb, int color)
{
    int delta_x, delta_y,dda;
    float x_inc, y_inc, x, y, i;

    delta_x = xb - xa;
    delta_y = yb - ya;

    if (abs(delta_y) < abs(delta_x)) dda = abs(delta_x);
    else dda = abs(delta_y);
    x_inc = delta_x / (float)dda;
    y_inc = delta_y / (float)dda;

    x = xa, y = ya;
    putpixel(x + x_mid, y_mid - y , color); 
    i = 0;
    do
    {
        x = x + x_inc;
        y = y + y_inc;
        putpixel(x + x_mid, y_mid - y, color);
        i++;
    }
    while(i<=dda);
}

void square_four(int xa, int ya, int xb, int yb, int color)
{
    dda_line(xa, ya, xb, ya, color);
    dda_line(xa, ya, xa, yb, color);
    dda_line(xb, ya, xb, yb, color);
    dda_line(xa, yb, xb, yb, color);
}

void pointClip(int koor[][2], int n, int xmin, int ymin, int xmax, int ymax)
{
    for(int i = 0;i < n; i++)
    {
        if((koor[i][0] >= xmin) && (koor[i][0] <= xmax))
            if((koor[i][1] >= ymin) && (koor[i][1] <= ymax))
                putpixel(koor[i][0]+ x_mid,y_mid - koor[i][1],2);
    }    
}

int main()
{
    int xmax, ymax, gm, gd = DETECT;
    int n;
    char data[] = "C:\\MinGW\\lib\\libbgi.a"; //static file
    
    initgraph(&gd, &gm, data);
    cleardevice();
    
    xmax = getmaxx(); // to get the co-ordinates i.e. x & y
    ymax = getmaxy();
    x_mid = xmax/2;
    y_mid = ymax/2;
    //coding graphics

    int wx1,wx2, wy1,wy2;

    printf("Masukkan Jumlah titik yang akan ada di layar: ");
    scanf("%d",&n);

    for (int i = 0; i < n; i++)
    {
        printf("Masukkan Nilai Koordinat Titik ke-%d: ", i+1);
        scanf("%d %d",&koor[i][0], &koor[i][1]);
    }

    printf("Masukkan Nilai Koordinat Window Clipping: ");
    scanf("%d %d %d %d",&wx1,&wy1,&wx2,&wy2);

    for (int i = 0; i < n; i++)
    {
        putpixel(koor[i][0]+ x_mid,y_mid - koor[i][1],15);
    }

    delay(5000);
    square_four(wx1,wy1,wx2,wy2,14);
    pointClip(koor, n, wx1,wy1,wx2,wy2);

    getch();
    closegraph();

    return 0;
}
