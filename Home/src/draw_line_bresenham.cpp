#include <stdio.h>
#include <stdlib.h> // Diperlukan untuk fungsi abs()
#include <conio.h>
#include <graphics.h>

int main()
{
    int gm, gd = DETECT;
    char data[] = "C:\\MinGW\\lib\\libbgi.a";
    initgraph(&gd, &gm, data);
    cleardevice();
    int delta_x, delta_y, p;
    int xa, ya, xb, yb;
    int xtemp, ytemp;
    int x, y, i;
    int x_mid, y_mid, xmax, ymax;

    xmax = getmaxx();
    ymax = getmaxy();
    x_mid = xmax/2;
    y_mid = ymax/2;
    
    printf("Masukkan Nilai Koordinat Awal:");
    scanf("%d %d",&xa, &ya);
    printf("Masukkan Nilai Koordinat Akhir:");
    scanf("%d %d",&xb, &yb);

    delta_x = xb - xa;
    delta_y = yb - ya;

    // Menentukan arah pergerakan (+1 atau -1)
    if (delta_x >= 0) xtemp = 1; else xtemp = -1;
    if (delta_y >= 0) ytemp = 1; else ytemp = -1;
    
    setcolor(RED);
    line(0, y_mid, xmax, y_mid);
    line(x_mid, 0, x_mid, ymax);

    x = xa;
    y = ya;
    putpixel(x + x_mid, y_mid - y, WHITE);
    i = 0;

    // KASUS 1: Garis lebih datar/landai (perubahan x lebih besar)
    if (abs(delta_y) < abs(delta_x))
    {
        // Rumus 'p' untuk kasus landai. Perhatikan urutan delta_y lalu delta_x
        p = (2 * abs(delta_y)) - abs(delta_x);
        do
        {
            if (p < 0)
            {
                x = x + xtemp;
                p = p + 2 * abs(delta_y);
                putpixel(x + x_mid, y_mid - y, WHITE);
            }
            else
            {
                x = x + xtemp;
                y = y + ytemp;
                p = p + 2 * abs(delta_y) - 2 * abs(delta_x);
                putpixel(x + x_mid, y_mid - y, WHITE);
            }
            i++;
        } while(i <= abs(delta_x));
    }
    // KASUS 2: Garis lebih tegak/curam (perubahan y lebih besar atau sama)
    else
    {
        // Rumus 'p' untuk kasus curam. Perhatikan urutan delta_x lalu delta_y (TERBALIK dari kasus 1)
        p = (2 * abs(delta_x)) - abs(delta_y);
        do
        {
            if (p < 0)
            {
                y = y + ytemp;
                p = p + 2 * abs(delta_x);
                putpixel(x + x_mid, y_mid - y, WHITE);
            }
            else
            {
                x = x + xtemp;
                y = y + ytemp;
                p = p + 2 * abs(delta_x) - 2 * abs(delta_y);
                putpixel(x + x_mid, y_mid - y, WHITE);
            }
            i++;
        } while(i <= abs(delta_y));
    }

    getch();
    closegraph();

    return 0;
}