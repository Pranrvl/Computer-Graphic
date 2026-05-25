#include <stdio.h> //input dan output 
#include <stdlib.h> 
#include <conio.h> //getch()
#include <graphics.h>

int x_mid, y_mid;
int koor[20][2];
float cx, cy;

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

void bre_line(int xa, int ya, int xb, int yb)
{
    int delta_x, delta_y, p;
    int xtemp, ytemp;
    int x, y, i;
    delta_x = xb - xa;
    delta_y = yb - ya;

    /* if (delta_x >= 0) xtemp = 1;
    else xtemp = -1;

    if (delta_y >= 0) ytemp = 1; 
    else ytemp = -1; */
    setcolor(RED);
   
    x = xa, y = ya;
    putpixel(x + x_mid, y_mid - y, WHITE);
    i = 0;
    if (abs(delta_y) < abs(delta_x))
    {
        p = (2 * delta_y) - (delta_x);
        do
        {
            if (p < 0)
            {
                x = x + 1;
                p = p + 2 * delta_y * ytemp;
                putpixel(x + x_mid, y_mid - y, WHITE);
            }
            else
            {
                x = x + 1;
                y = y + 1;
                p = p + 2 * delta_y - 2 * delta_x;
                putpixel(x + x_mid, y_mid - y, WHITE);
            }
            i++;
        }while(i<=delta_x);
    }
    else
    {
        p = (2 * delta_x ) - (delta_y);
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
        }while(i<=delta_y);
    }
}

void scanlineFill(int n, int color)
{
    float slope[20];
    int delta_x, delta_y,k,i,j,xi[20],yi[20],temp,y;
     for(i = 0; i<n;i++)
    {
        delta_x = koor[i+1][0] - koor[i][0];
        delta_y = koor[i+1][1] - koor[i][1];
        if (delta_y == 0) slope[i] = 1.0;
        if (delta_x == 0) slope[i] = 0.0;
        if ((delta_y != 0) && (delta_x != 0))
            slope[i] = (float) delta_x/delta_y;
    }

    for(y = 0; y<480; y++) // ambil nilai terendah dan tertinggi yang ada
    {
        k = 0;
        for(i = 0;i<n;i++)
        {
            if(((koor[i][1]<= y)&&(koor[i+1][1]>y))||((koor[i][1]>y)&&(koor[i+1][1]<= y)))
            {
                xi[k] = (int) (koor[i][0] + slope[i] * (y-koor[i][1]));
                k++;
            }
        }
    
        for(j = 0;j<k;j++)
        {
            for(i = 0;i<k;i++)
            {
                if(xi[i]>xi[i+1])
                {
                    temp = xi[i];
                    xi[i] = xi[i+1];
                    xi[i+1]= temp;
                }
            }
        }

        for(i=0;i<k;i+=2)
        {
            printf("%d,%d - %d,%d\n",xi[i],y,xi[i+1],y);
            //dda_line(xi[i],y,xi[i+1]+1,y, color);
        }
            
        /* for(i=0;i<k;i+=2)
            for(j = 0;j<100;j++)
            {
                dda_line(xi[i]+1,y+(float)j/100,xi[i+1]+1,y+(float)j/100, color);
            } */
    }
}

void coordCenter(int n)
{
    float a, t;
    int i;

    a = 0.0;
    for (i=0; i<n; i++) {
        a += koor[i][0] * koor[i+1][1] - koor[i+1][0] * koor[i][1];
    }
    a *= 0.5;

    cx = cy = 0.0;
    for (i=0; i<n; i++) {
        t = koor[i][0] * koor[i+1][1] - koor[i+1][0] * koor[i][1];
        cx += (koor[i][0]+koor[i+1][0]) * t;
        cy += (koor[i][1]+koor[i+1][1]) * t;
    }
    cx = cx / (6.0 * a);
    cy = cy / (6.0 * a);
}

void boundaryFill(int x, int y, int fore, int back)
{
    if(getpixel(x,y) != back && getpixel(x,y) != fore)
    {
        putpixel(x,y, fore);
        boundaryFill(x+1,y,fore,back); //kanan
        boundaryFill(x,y+1,fore,back); //atas
        boundaryFill(x-1,y,fore,back); //kiri
        boundaryFill(x,y-1,fore,back); //bawah 
    }
}

void floodFill(int x, int y, int newC, int oldC)
{
    if(getpixel(x,y) == oldC)
    {
        putpixel(x,y, newC);
        floodFill(x+1,y,newC,oldC); //kanan
        floodFill(x,y+1,newC,oldC); //atas
        floodFill(x-1,y,newC,oldC); //kiri
        floodFill(x,y-1,newC,oldC); //bawah 
    }
}
        
void square_four(int xa, int ya, int xb, int yb)
{
    bre_line(xa, ya, xb, ya);
    bre_line(xa, ya, xa, yb);
    bre_line(xb, ya, xb, yb);
    bre_line(xa, yb, xb, yb);
}

void polygon(int n, int color)
{
    int i, j ;
    for (i = 0; i < n; i++)
    {
        printf("Masukkan Nilai Koordinat Titik ke-%d:", i+1);
        scanf("%d %d",&koor[i][0], &koor[i][1]);
    }

    for (j= 0; j < n; j++)
    {
        if (j < n-1)
            dda_line(koor[j][0], koor[j][1], koor[j+1][0], koor[j+1][1],color);
        else
            dda_line(koor[0][0], koor[0][1], koor[j][0], koor[j][1],color);
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
    
    printf("Masukkan Jumlah Sisi dari Polygon:");
    scanf("%d",&n);
    
    //square_four(x1,y1, x2,y2);
    polygon(n,15);
    scanlineFill(n,4);
    
    coordCenter(n);
    //boundaryFill(int(cx)+x_mid,y_mid-int(cy),4,15);

    int oldC = getpixel(int(cx)+x_mid,y_mid-int(cy));
    floodFill(int(cx)+x_mid,y_mid-int(cy),4,oldC);

    getch();
    closegraph();

    return 0;
}
