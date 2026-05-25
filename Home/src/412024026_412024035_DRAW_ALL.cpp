#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <graphics.h>
#include <math.h>

void drawEquation(int xa, int ya, int xb, int yb, int x_mid, int y_mid) {
    float m, c, x, y;
    m = ((float)yb - (float)ya) / ((float)xb - (float)xa);
    c = (float)ya - (m * (float)xa);

    x = xa;
    y = ya;
    putpixel(x + x_mid, y_mid - y, WHITE);

    if (m < 1) {
        do {
            x = x + 1;
            y = (m * x) + c;
            putpixel(x + x_mid, y_mid - y, WHITE);
        } while (x <= xb);
    } else {
        do {
            y = y + 1;
            x = (y - c) / m;
            putpixel(x + x_mid, y_mid - y, WHITE);
        } while (y <= yb);
    }
}

void drawBresenham(int xa, int ya, int xb, int yb, int x_mid, int y_mid) {
    int delta_x, delta_y, p;
    int x, y, i;
    int xtemp, ytemp;

    delta_x = xb - xa;
    delta_y = yb - ya;

    if (delta_x >= 0) xtemp = 1;
    else xtemp = -1;

    if (delta_y >= 0) ytemp = 1;
    else ytemp = -1;

    delta_x = abs(delta_x);
    delta_y = abs(delta_y);

    x = xa;
    y = ya;
    putpixel(x + x_mid, y_mid - y, WHITE);
    i = 0;

    if (delta_y < delta_x) {
        p = (2 * delta_y) - delta_x;
        do {
            if (p < 0) {
                x = x + xtemp;
                p = p + 2 * delta_y;
            } else {
                x = x + xtemp;
                y = y + ytemp;
                p = p + 2 * delta_y - 2 * delta_x;
            }
            putpixel(x + x_mid, y_mid - y, WHITE);
            i++;
        } while (i <= delta_x);
    } else {
        p = (2 * delta_x) - delta_y;
        do {
            if (p < 0) {
                y = y + ytemp;
                p = p + 2 * delta_x;
            } else {
                x = x + xtemp;
                y = y + ytemp;
                p = p + 2 * delta_x - 2 * delta_y;
            }
            putpixel(x + x_mid, y_mid - y, WHITE);
            i++;
        } while (i <= delta_y);
    }
}

void drawDDA(int xa, int ya, int xb, int yb, int x_mid, int y_mid) {
    int delta_x, delta_y, dda;
    float x_inc, y_inc, x, y, i;

    delta_x = xb - xa;
    delta_y = yb - ya;

    if (abs(delta_y) < abs(delta_x))
        dda = abs(delta_x);
    else
        dda = abs(delta_y);

    x_inc = delta_x / (float)dda;
    y_inc = delta_y / (float)dda;

    x = xa, y = ya;
    putpixel(x + x_mid, y_mid - y, WHITE);
    i = 0;

    do {
        x = x + x_inc;
        y = y + y_inc;
        putpixel(x + x_mid, y_mid - y, WHITE);
        i++;
    } while (i <= dda);
}

void drawGrid(int x_mid, int y_mid, int left, int top, int right, int bottom) {
    setcolor(RED);
    line(left, y_mid, right, y_mid);
    line(x_mid, top, x_mid, bottom);
}

int main() {
    int gm, gd = DETECT;
    char data[] = "C:\\MinGW\\lib\\libbgi.a"; 

    initgraph(&gd, &gm, data);
    cleardevice();

    int choice;
    int xa, ya, xb, yb;
    int xmax = getmaxx(), ymax = getmaxy();

    printf("Menu:\n");
    printf("1. DDA\n");
    printf("2. Equation\n");
    printf("3. Bresenham\n");
    printf("4. DDA + Bresenham (Split)\n");
    printf("Pilih metode: ");
    scanf("%d", &choice);

    printf("Masukkan Nilai Koordinat Awal: ");
    scanf("%d %d", &xa, &ya);
    printf("Masukkan Nilai Koordinat Akhir: ");
    scanf("%d %d", &xb, &yb);

    if (choice == 1) {
        int x_mid = xmax / 2, y_mid = ymax / 2;
        drawGrid(x_mid, y_mid, 0, 0, xmax, ymax);
        drawDDA(xa, ya, xb, yb, x_mid, y_mid);
    } 
    else if (choice == 2) {
        int x_mid = xmax / 2, y_mid = ymax / 2;
        drawGrid(x_mid, y_mid, 0, 0, xmax, ymax);
        drawEquation(xa, ya, xb, yb, x_mid, y_mid);
    } 
    else if (choice == 3) {
        int x_mid = xmax / 2, y_mid = ymax / 2;
        drawGrid(x_mid, y_mid, 0, 0, xmax, ymax);
        drawBresenham(xa, ya, xb, yb, x_mid, y_mid);
    } 
    else if (choice == 4) {
        setcolor(RED);
        line(xmax / 2, 0, xmax / 2, ymax);
        line(0, ymax / 2, xmax, ymax / 2);

        int left1 = 0, top1 = 0, right1 = xmax / 2, bottom1 = ymax / 2;
        int x_mid1 = (left1 + right1) / 2;
        int y_mid1 = (top1 + bottom1) / 2;
        drawGrid(x_mid1, y_mid1, left1, top1, right1, bottom1);
        drawDDA(xa, ya, xb, yb, x_mid1, y_mid1);

        int left2 = xmax / 2, top2 = ymax / 2, right2 = xmax, bottom2 = ymax;
        int x_mid2 = (left2 + right2) / 2;
        int y_mid2 = (top2 + bottom2) / 2;
        drawGrid(x_mid2, y_mid2, left2, top2, right2, bottom2);
        drawBresenham(xa, ya, xb, yb, x_mid2, y_mid2);
    } 
    else {
        printf("Invalid choice!\n");
    }

    getch();
    closegraph();
    return 0;
}