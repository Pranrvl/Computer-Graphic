#include <graphics.h>
#include <conio.h>
#include <cmath>

void drawEllipse(int xc, int yc, int rx, int ry, int color) {
    float dx, dy, d1, d2, x, y;
    x = 0;
    y = ry;
    putpixel(xc + x, yc + y, color);
    putpixel(xc - x, yc + y, color);
    putpixel(xc + x, yc - y, color);
    putpixel(xc - x, yc - y, color);
    d1 = (ry * ry) - (rx * rx * ry) + (0.25 * rx * rx);
    dx = 2 * ry * ry * x;
    dy = 2 * rx * rx * y;
    while (dx < dy) {
        if (d1 < 0) {
            x++;
            dx = dx + (2 * ry * ry);
            d1 = d1 + dx + (ry * ry);
        } else {
            x++;
            y--;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d1 = d1 + dx - dy + (ry * ry);
        }
        putpixel(xc + x, yc + y, color);
        putpixel(xc - x, yc + y, color);
        putpixel(xc + x, yc - y, color);
        putpixel(xc - x, yc - y, color);
    }
    d2 = ((ry * ry) * ((x + 0.5) * (x + 0.5))) + ((rx * rx) * ((y - 1) * (y - 1))) - (rx * rx * ry * ry);
    while (y >= 0) {
        if (d2 > 0) {
            y--;
            dy = dy - (2 * rx * rx);
            d2 = d2 + (rx * rx) - dy;
        } else {
            y--;
            x++;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d2 = d2 + dx - dy + (rx * rx);
        }
        putpixel(xc + x, yc + y, color);
        putpixel(xc - x, yc + y, color);
        putpixel(xc + x, yc - y, color);
        putpixel(xc - x, yc - y, color);
    }
}

void boundaryFill4(int x, int y, int fillColor, int boundaryColor) {
    int current = getpixel(x, y);
    if (current != fillColor && current != boundaryColor) {
        putpixel(x, y, fillColor);
        boundaryFill4(x + 1, y, fillColor, boundaryColor);
        boundaryFill4(x - 1, y, fillColor, boundaryColor);
        boundaryFill4(x, y + 1, fillColor, boundaryColor);
        boundaryFill4(x, y - 1, fillColor, boundaryColor);
    }
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");
    int cx = getmaxx() / 2;
    int cy = getmaxy() / 2;
    int side = 400;
    int height = (int)(sqrt(3) / 2 * side);
    int R = height / 3;
    int yShift = 40;
    int x1 = cx;
    int y1 = cy - height / 2 - yShift;
    int x2 = cx - side / 2;
    int y2 = cy + height / 2 - yShift;
    int x3 = cx + side / 2;
    int y3 = cy + height / 2 - yShift;
    int cyb = cy + height / 6 - yShift;
    setcolor(BLUE);
    for (int y = cy - 250; y <= cy + 250; y++)
        for (int x = cx - 250; x <= cx + 250; x++)
            putpixel(x, y, BLUE);
    setcolor(BLACK);
    rectangle(cx - 250, cy - 250, cx + 250, cy + 250);
    setcolor(MAGENTA);
    line(x1, y1, x2, y2);
    line(x2, y2, x3, y3);
    line(x3, y3, x1, y1);
    boundaryFill4(cx, cy, MAGENTA, BLACK);
    setcolor(BLACK);
    line(x1, y1, x2, y2);
    line(x2, y2, x3, y3);
    line(x3, y3, x1, y1);
    setcolor(CYAN);
    circle(cx, cyb, R);
    boundaryFill4(cx, cyb, CYAN, BLACK);
    setcolor(BLACK);
    circle(cx, cyb, R);
    setcolor(YELLOW);
    drawEllipse(cx, cyb, R, R / 3, YELLOW);
    boundaryFill4(cx, cyb, YELLOW, BLACK);
    setcolor(BLACK);
    drawEllipse(cx, cyb, R, R / 3, BLACK);
    setcolor(RED);
    line(cx, cyb - R / 3, cx, cyb + R / 3);
    getch();
    closegraph();
    return 0;
}
