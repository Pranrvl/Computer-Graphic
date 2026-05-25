#include <graphics.h>
#include <conio.h>
#include <cmath>

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int cx = getmaxx() / 2;
    int cy = getmaxy() / 2;

    int side = 400;
    int height = (int)(sqrt(3) / 2 * side);
    int R = height/3;
    int yShift = 40;

    int left_bar = cx - 250;
    int top_bar = cy - 250;
    int right_bar = cx + 250;
    int bottom_bar = cy + 250;
    
    int tri[] = {
        cx, cy - height / 2 - yShift,
         cx - side / 2, cy + height / 2 - yShift,
            cx + side / 2, cy + height / 2 - yShift
    };

    int cyb = cy + height/6 - yShift;

    setcolor(BLUE);
     setfillstyle(SOLID_FILL, BLUE);
    bar(left_bar, top_bar, right_bar, bottom_bar);
    setcolor(BLACK); 
    rectangle(left_bar, top_bar, right_bar, bottom_bar);

    setcolor(MAGENTA);
    setfillstyle(SOLID_FILL, MAGENTA);
    fillpoly(3, tri);
     setcolor(BLACK); 
    drawpoly(3, tri);

    setcolor(CYAN);
    setfillstyle(SOLID_FILL, CYAN);
    fillellipse(cx, cyb, R, R);
    
    setcolor(MAGENTA); fillellipse(cx, cyb, R - 2, R - 2); 
    setcolor(CYAN);
    setfillstyle(SOLID_FILL, CYAN);
     fillellipse(cx, cyb, R - 2, R - 2); 

    setcolor(BLACK); 
    circle(cx, cyb, R);

    
    int elips_radius_x = R;
    int elips_radius_y = R / 3; 
    
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    fillellipse( cx, cyb, elips_radius_x, elips_radius_y );
    
    setcolor(BLACK); 
    ellipse(cx, cyb, 0, 360, elips_radius_x, elips_radius_y);

    
    setcolor(RED); 
    line(cx, cyb - elips_radius_y, cx, cyb + elips_radius_y); 

    getch();
    closegraph();
    return 0;
}