#include <stdio.h>
    #include <stdlib.h>
    #include <conio.h>
    #include <graphics.h>
    #include <math.h> // For abs() and round()

    int x_mid, y_mid;
    int koor[20][2];
    int cx,cy;

    struct coordinate9
    {
        int x;
        int y;
    } coor_st, coor_to;

    void draw_bresenham(int xa, int ya, int xb, int yb, int color)
    {
        int delta_x = xb - xa;
        int delta_y = yb - ya;
        int xtemp, ytemp;
        int xmax, ymax;
        int abs_dx = abs(delta_x);
        int abs_dy = abs(delta_y);
        int p;

        if (delta_x >= 0) xtemp = 1;
        else xtemp = -1;

        if (delta_y >= 0) ytemp = 1; 
        else ytemp = -1;
        
        setcolor(RED);
        line(0, y_mid, xmax, y_mid);
        line(x_mid, 0, x_mid, ymax);

        int x = xa, y = ya;
        putpixel(x + x_mid, y_mid - y, color);
        int i = 0;
        if (abs(delta_y) < abs(delta_x))
        {
            p = (2 * abs_dy) - (abs_dx);
            do
            {
                if (p < 0)
                {
                    x = x + xtemp;
                    p = p + 2 * abs_dy;
                    putpixel(x + x_mid, y_mid - y, color);
                }
                else
                {
                    x = x + xtemp;
                    y = y + ytemp;
                    p = p + 2 * abs_dy - 2 * abs_dx;
                    putpixel(x + x_mid, y_mid - y, color);
                }
                i++;
            }while(i<=abs_dx);
        }
        else
        {
            p = (2 * abs_dx ) - (abs_dy);
            do
            {
                if (p < 0)
                {
                    y = y + ytemp;
                    p = p + 2 * abs_dx;
                    putpixel(x + x_mid, y_mid - y, color);
                }
                else
                {
                    x = x + xtemp;
                    y = y + ytemp;
                    p = p + 2 * abs_dx - 2 * abs_dy;
                    putpixel(x + x_mid, y_mid - y, color);
                }
                i++;
            }while(i<=abs_dy);
        }
    }

    void draw_dda(int x_st, int y_st, int x_to, int y_to)
    {
        int delta_x, delta_y, dda, i;
        float x_inc, y_inc, x, y;

        delta_x = x_to - x_st;
        delta_y = y_to - y_st;

        if (abs(delta_y) < abs(delta_x))
            dda = abs(delta_x);
        else
            dda = abs(delta_y);

        x_inc = delta_x / (float)dda;
        y_inc = delta_y / (float)dda;

        x = x_st;
        y = y_st;
        putpixel(round(x) + x_mid, y_mid - round(y), WHITE);

        for (i = 0; i < dda; i++)
        {
            x = x + x_inc;
            y = y + y_inc;
            putpixel(round(x) + x_mid, y_mid - round(y), WHITE);
        }
    }

    void draw_equation(int xa, int ya, int xb, int yb)
    {
        float m, c;
        int x, y;

        if (xa == xb) {
            int start_y = (ya < yb) ? ya : yb;
            int end_y = (ya < yb) ? yb : ya;
            for (y = start_y; y <= end_y; y++) {
                putpixel(xa + x_mid, y_mid - y, WHITE);
            }
            return;
        }
        
        m = ((float)yb - (float)ya) / ((float)xb - (float)xa);
        c = (float)ya - (m * (float)xa);

        if (fabs(m) < 1) 
        {
            int start_x = (xa < xb) ? xa : xb;
            int end_x = (xa < xb) ? xb : xa;
            for (x = start_x; x <= end_x; x++)
            {
                y = round((m * x) + c);
                putpixel(x + x_mid, y_mid - y, WHITE);
            }
        }
        else 
        {
            int start_y = (ya < yb) ? ya : yb;
            int end_y = (ya < yb) ? yb : ya;
            for (y = start_y; y <= end_y; y++)
            {
                x = round((y - c) / m);
                putpixel(x + x_mid, y_mid - y, WHITE);
            }
        }
    }

    void draw_combo(int xa, int ya, int xb, int yb)
    {
        {
            int delta_x = xb - xa;
            int delta_y = yb - ya;
            int xtemp, ytemp;
            int abs_dx = abs(delta_x);
            int abs_dy = abs(delta_y);
            int p;
            int x = xa;
            int y = ya;

            int xquar = x_mid / 2;
            int yquar = y_mid / 2;
            if (delta_x >= 0) xtemp = 1;
            else xtemp = -1;

            if (delta_y >= 0) ytemp = 1; 
            else ytemp = -1;
            putpixel(x + xquar, yquar - y, WHITE);

            if (abs_dx > abs_dy)
            {
                p = 2 * abs_dy - abs_dx;
                for (int i = 0; i < abs_dx; i++)
                {
                    x = x + xtemp;
                    if (p < 0) {
                        p = p + 2 * abs_dy;
                    } else {
                        y = y + ytemp;
                        p = p + 2 * abs_dy - 2 * abs_dx;
                    }
                    putpixel(x + xquar, yquar - y, WHITE);
                }
            }
            else
            {
                p = 2 * abs_dx - abs_dy;
                for (int i = 0; i < abs_dy; i++)
                {
                    y = y + ytemp;
                    if (p < 0) {
                        p = p + 2 * abs_dx;
                    } else {
                        x = x + xtemp;
                        p = p + 2 * abs_dx - 2 * abs_dy;
                    }
                    putpixel(x + xquar, yquar - y, WHITE);
                }
            }
        }


        {
            int dx = xb - xa;
            int dy = yb - ya;
            int steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);
            float x_inc = dx / (float)steps;
            float y_inc = dy / (float)steps;
            float x = xa;
            float y = ya;

            int xthird = x_mid + (x_mid / 2);
            int ythird = y_mid + (y_mid / 2);

            putpixel(round(x) + xthird, ythird - round(y), WHITE);

            for (int i = 0; i < steps; i++)
            {
                x = x + x_inc;
                y = y + y_inc;
                putpixel(round(x) + xthird, ythird - round(y), WHITE);
            }
        }
    }

    float degressToRadians(float angle)
    {
        return angle * M_PI / 180;
    }

    void triangle(int xa, int ya, int angle, int sideLen)
    {
        int xb = sideLen * cos(degressToRadians(angle - 30)) + xa;
        int yb = sideLen * sin(degressToRadians(angle - 30)) + ya;

        int xc = sideLen * cos(degressToRadians(angle + 30)) + xa;
        int yc = sideLen * sin(degressToRadians(angle + 30)) + ya;

        printf("xa: %d, ya: %d\n", xa, ya);
        printf("xb: %d, yb: %d\n", xb, yb);
        printf("xc: %d, yc: %d\n", xc, yc);

        draw_bresenham(xa, ya, xb, yb,WHITE);
        draw_bresenham(xc, yc, xb, yb,WHITE);
        draw_bresenham(xa, ya, xc, yc, WHITE); //musihnya a ke c
    }

    void square_four(int xa, int ya, int xb, int yb)
    {
        draw_bresenham(xa, ya, xb, ya, WHITE);
        draw_bresenham(xa, ya, xa, yb, WHITE);
        draw_bresenham(xb, ya, xb, yb, WHITE);
        draw_bresenham(xa, yb, xb, yb, WHITE);
    }

    void polygon(int n, int color)
    {
        int i, j;
        for (i = 0; i < n; i++)
        {
            printf("Masukkan Nilai Koordinat Titik ke -%d", i+i);
            scanf("%d %d", &koor[i][0], &koor[i][1]);
        }
        for(j = 0; j < n; j++)
        {
            if(j < n-1)
                draw_bresenham(koor[j][0], koor[j][1], koor[j+1][0], koor[j+1][1], color);
            else
                draw_bresenham(koor[0][0], koor[0][1], koor[j][0], koor[j][1], color);
        }
    }

    int main()
    {
        int gm, gd = DETECT;
        char data[] = "C:\\MinGW\\lib\\libbgi.a";
        initgraph(&gd, &gm, data);
        cleardevice();
        int xmax = getmaxx(), ymax = getmaxy();

        x_mid = xmax / 2;
        y_mid = ymax / 2;

        setcolor(RED);
        line(0, y_mid, xmax, y_mid);
        line(x_mid, 0, x_mid, ymax);

        int xquar = x_mid + (x_mid / 2);
        int yquar = y_mid / 2;
        setcolor(BLUE);
        line(x_mid, yquar, xmax, yquar);   
        line(xquar, 0, xquar, y_mid);      
        int xthird = x_mid / 2;
        int ythird = y_mid + (y_mid / 2);
        setcolor(GREEN);
        line(0, ythird, x_mid, ythird); 
        line(xthird, y_mid, xthird, ymax);

        int e;
        printf("Metode: \n");
        printf("1. DDA\n");
        printf("2. Bresenham\n");
        printf("3. EQ\n");
        printf("4. Combo\n");
        printf("5. Square\n");
        printf("6. Triangle\n");
        printf("Pilihan: ");
        scanf("%d", &e);

        printf("Enter start coordinate: ");
        scanf("%d %d", &coor_st.x, &coor_st.y); // 10 10
        printf("Enter destination coordinate: ");
        scanf("%d %d", &coor_to.x, &coor_to.y); // 0 0

        switch (e)
        {
            case 1:           
                draw_dda(coor_st.x, coor_st.y, coor_to.x, coor_to.y);
                break;
            case 2:
                draw_bresenham(coor_st.x, coor_st.y, coor_to.x, coor_to.y, WHITE);
                break;
            case 3:
                draw_equation(coor_st.x, coor_st.y, coor_to.x, coor_to.y);
                break;
            case 4:
                draw_combo(coor_st.x, coor_st.y, coor_to.x, coor_to.y);
                break;
            case 5:
                square_four(coor_st.x, coor_st.y, coor_to.x, coor_to.y);
                break;
            case 6:
            {
                int angle, sideLen;
                printf("Enter angle (in degrees): ");
                scanf("%d", &angle); //-30
                printf("Enter side length: ");
                scanf("%d", &sideLen); //100
                triangle(coor_st.x, coor_st.y, angle, sideLen);            
            }
            break;
            case 7:
            {
                int n;
                printf("Jumlah sisi Polygon: ");
                scanf("%d", &n);
                polygon(n, WHITE);
            }
        }

        getch();
        closegraph();
        return 0;
    }