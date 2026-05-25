#include <stdio.h>
#include <algorithm>
#include <graphics.h>
#include <cmath>

struct MyCoords {
    int x;
    int y;
};

class myLib {
    const int maxX = getmaxx(), maxY = getmaxy(), midX = maxX / 2, midY = maxY / 2;

    void fill (int x, int y, colors startingCoordColour, colors fillColour) {

        if(startingCoordColour == intToColors(getpixel(x, y))){
            putpixel(x, y, fillColour);

            fill(x, y+1, startingCoordColour, fillColour);
            fill(x, y-1, startingCoordColour, fillColour);
            fill(x+1, y, startingCoordColour, fillColour);
            fill(x-1, y, startingCoordColour, fillColour);
        }
    }

    void printCoords(MyCoords coords) {
        printf("(x | y): %d | %d", coords.x, coords.y);
    }
    
    void bresenham(MyCoords start, MyCoords end, colors colour) {
        // Variables for function
        int major, deltaMajor, slopeMajor, minor, deltaMinor, slopeMinor, e = 0, *x, *y;

        // calculations
        if (abs(end.x - start.x) > abs(end.y - start.y)) {
            major = start.x;
            x = &major;
            minor = start.y;
            y = &minor;
            deltaMajor = end.x - start.x;
            deltaMinor = end.y - start.y;
        } else {
            major = start.y;
            y = &major;
            minor = start.x;
            x = &minor;
            deltaMajor = end.y - start.y;
            deltaMinor = end.x - start.x;
        }

        slopeMajor = (deltaMajor > 0) ? 1 : -1;
        deltaMajor = abs(deltaMajor);
        slopeMinor = (deltaMinor > 0) ? 1 : -1;
        deltaMinor = abs(deltaMinor);

        int n = deltaMajor;

        // std::cout << "[DEBUG INFO]\nmajor: " << major << "\nminor: " << minor << "\nstart: " << start.x << " " << start.y << "\nstop: " << end.x << " " << end.y << "\n";
        // std::cout << (end.x - start.x) << " > " << (end.y-start.y) << "\n";
        for (int i = 0; i <= n; i++) {
            // std::cout << "Major: " << major << " | minor: " << minor << "\n";
            putpixel(*x + midX, midY - *y, colour);

            // Handle changes in major axis
            major += slopeMajor;

            // Check if we need to increase minor
            e += deltaMinor;
            if ((e * 2) >= deltaMajor) {
                minor += slopeMinor;
                e -= deltaMajor;
            }
        }
    }

    void plotCircle(int centreX, int centreY, int x, int y, colors colour){
        putpixel(centreX + x, centreY + y, colour);
        putpixel(centreX + x, centreY - y, colour);
        putpixel(centreX - x, centreY + y, colour);
        putpixel(centreX - x, centreY - y, colour);

        putpixel(centreX + y, centreY + x, colour);
        putpixel(centreX + y, centreY - x, colour);
        putpixel(centreX - y, centreY + x, colour);
        putpixel(centreX - y, centreY - x, colour);
    }
public:
    myLib() {
    }

    void drawLine(int startX, int startY, int endX, int endY, colors colour) {
        MyCoords start, end;
        start.x = startX;
        start.y = startY;

        end.x = endX;
        end.y = endY;
        bresenham(start, end, colour);
    }

    void drawRectangle(int startX, int startY, int width, int height, colors colour) {
        int endX = startX + width, endY = startY + height;
        MyCoords start, end;

        // Left
        start.x = startX, start.y = startY;
        end.x = startX, end.y = endY;

        bresenham(start, end, colour);

        // Right
        start.x = endX, start.y = startY;
        end.x = endX, end.y = endY;

        bresenham(start, end, colour);

        // Top
        start.x = startX, start.y = endY;
        end.x = endX, end.y = endY;

        bresenham(start, end, colour);

        // Bot
        start.x = startX, start.y = startY;
        end.x = endX, end.y = startY;

        bresenham(start, end, colour);
    }

    void drawEquilateralTriangle(int startX, int startY, int length, colors colour) {
        MyCoords left, right, top;

        left.x = startX, left.y = startY;
        right.x = startX + length, right.y = startY;

        bresenham(left, right, colour);

        int height = round((sqrt(3) * length) / 2);
        top.x = startX + round(length / 2), top.y = startY + height;

        bresenham(left, top, colour);
        bresenham(top, right, colour);
    }

    void drawIsoscelesTriangle(int startX, int startY, int width, int height, colors colour) {
        MyCoords left, right, top;

        left.x = startX, left.y = startY;
        right.x = startX + width, right.y = startY;
        top.x = startX + round((float)width / 2.0), top.y = startY + height;

        bresenham(left, right, colour);
        bresenham(left, top, colour);
        bresenham(top, right, colour);
    }

    void drawScaleneTriangle(MyCoords points[2], colors colour) {
        bresenham(points[0], points[1], colour);
        bresenham(points[0], points[2], colour);
        bresenham(points[1], points[2], colour);
    }

    void drawRightTriangle(int startX, int startY, int width, int height, colors colour) {
        MyCoords left, right, top;

        left.x = startX, left.y = startY;
        right.x = startX + width, right.y = startY;
        top.x = startX, top.y = startY + height;

        bresenham(left, right, colour);
        bresenham(left, top, colour);
        bresenham(top, right, colour);
    }

    void drawCircle(int startX, int startY, int diameter, colors colour) {
        startX += midX;
        startY += midY;

        int x = 0;
        int y = diameter;
        int d = 3 - 2 * diameter;

        plotCircle(startX, startY, x, y, colour);

        while(y >= x){
            if(d > 0){
                y--;
                d += 4 * (x-y) + 10;
            } else {
                d += 4 * x + 6;
            }

            x++;
            plotCircle(startX, startY, x, y, colour);
        }
    }

    void drawPoly(MyCoords points[], int polyAmount, colors colour) {
        for (int i = 0; i < polyAmount - 1; i++) {
            bresenham(points[i], points[(i + 1)], colour);
        }
        bresenham(points[polyAmount - 1], points[0], colour);
    }

    void drawElipse(int centreX, int centreY, int radiusX, int radiusY, colors colour){
        centreX += midX;
        centreY += midY;

        double deltaX, deltaY, deltaA, deltaB, x, y;
        x = 0;
        y = radiusY;

        deltaA = pow(radiusY, 2) - pow(radiusX, 2) * radiusY + (0.25 * pow(radiusX, 2));
        deltaX = 2 * x * pow(radiusY, 2);
        deltaY = 2 * y * pow(radiusX, 2);

        while (deltaX < deltaY){
            putpixel(centreX + x, centreY + y, colour);
            putpixel(centreX + x, centreY - y, colour);
            putpixel(centreX - x, centreY + y, colour);
            putpixel(centreX - x, centreY - y, colour);
            
            if(deltaA < 0){
                x++;
                deltaX += 2 * pow(radiusY, 2);
                deltaA += deltaX + pow(radiusY, 2);
            } else {
                x++;
                y--;
                deltaX += 2 * pow(radiusX, 2);
                deltaY -= 2 * pow(radiusY, 2);
                deltaA += deltaX - deltaY + pow(radiusY, 2);
            }
        }

        deltaB =  (pow(radiusY, 2) * ((x + 0.5) * (x + 0.5))) + 
         (pow(radiusX, 2) * ((y - 1) * (y - 1))) -
          (pow(radiusX, 2) * pow(radiusY, 2));

        while (y >= 0){
            putpixel(centreX + x, centreY + y, colour);
            putpixel(centreX + x, centreY - y, colour);
            putpixel(centreX - x, centreY + y, colour);
            putpixel(centreX - x, centreY - y, colour);

            if(deltaB >= 0){
                y--;
                deltaY -= pow(radiusX, 2) * 2;
                deltaB += pow(radiusX, 2) - deltaY;
            } else {
                y--;
                x++;
                deltaX += pow(radiusY, 2);
                deltaY -= pow(radiusX, 2);
                deltaB += deltaX - deltaY + pow(radiusX, 2); 
            }
        }
    }

    static colors intToColors(int value) {
        switch (value) {
        case 0:
            return BLACK;

        case 1:
            return BLUE;

        case 2:
            return GREEN;

        case 3:
            return CYAN;

        case 4:
            return RED;

        case 5:
            return MAGENTA;

        case 6:
            return BROWN;

        case 7:
            return LIGHTGRAY;

        case 8:
            return DARKGRAY;

        case 9:
            return LIGHTBLUE;

        case 10:
            return LIGHTGREEN;

        case 11:
            return LIGHTCYAN;

        case 12:
            return LIGHTRED;

        case 13:
            return LIGHTMAGENTA;

        case 14:
            return YELLOW;

        case 15:
            return WHITE;
        }
    }

    static int colorsToInt(colors value) {
        switch (value) {
        case BLACK:
            return 0;

        case BLUE:
            return 1;

        case GREEN:
            return 2;

        case CYAN:
            return 3;

        case RED:
            return 4;

        case MAGENTA:
            return 5;

        case BROWN:
            return 6;

        case LIGHTGRAY:
            return 7;

        case DARKGRAY:
            return 8;

        case LIGHTBLUE:
            return 9;

        case LIGHTGREEN:
            return 10;

        case LIGHTCYAN:
            return 11;

        case LIGHTRED:
            return 12;

        case LIGHTMAGENTA:
            return 13;

        case YELLOW:
            return 41;

        case WHITE:
            return 15;
        }
    }

    void myFill(MyCoords startingCoord, colors fillColour) {
        startingCoord.x += midX;
        startingCoord.y += midY;
        
        colors startingCoordColour = intToColors(getpixel(startingCoord.x, startingCoord.y));

        // No need to fill paint the same colour
        if (startingCoordColour == fillColour){
            return;
        }


        fill(startingCoord.x, startingCoord.y, startingCoordColour, fillColour);
    }


};


colors colour;

colors chooseColour(){
    int colorChoice;

    printf("Please choose a colour!\n"
           "[0] Black\t[5] Magenta\t[10] Light Green\t[15] White\n"
           "[1] Blue\t[6] Brown\t[11] Light Cyan\n"
           "[2] Green\t[7] Light Gray\t[12] Light Red\n"
           "[3] Cyan\t[8] Dark Gray\t[13] Light Magenta\n"
           "[4] Red\t\t[9] Light Blue\t[14] Yellow"
           "\n> ");

    scanf("%d", &colorChoice);

    return myLib::intToColors(colorChoice);
}

int main()
{
    int gm, gd = DETECT;
    char data[] = "C:\\MinGW\\lib\\libbgi.a"; // static file

    int startX, startY, endX, endY, width, height, choice, polyAmount;

    initgraph(&gd, &gm, data);
    cleardevice();

    myLib ml = myLib();

    // Midterm
    ml.drawRectangle(-100, -100, 200, 200, WHITE);
    ml.myFill(MyCoords {-99, -99}, RED);
    ml.drawIsoscelesTriangle(-90, -90, 180, 180, YELLOW);
    ml.myFill(MyCoords {-89, -89}, LIGHTGREEN);
    ml.drawCircle(0, 34, 56, MAGENTA);
    ml.myFill(MyCoords {0, 0}, LIGHTGRAY);
    ml.drawElipse(0, 34, 56, 28, RED);
    ml.myFill(MyCoords {0, 35}, LIGHTBLUE);
    ml.drawLine(0, 0, 0, -90, LIGHTGRAY);

    while (true)
    {
        printf("What would you like to draw?\n"
               "[1] Line\t\t\t[6] Right Triangle\n"
               "[2] Rectangle / Square\t\t[7] Circle\n"
               "[3] Equilateral Triangle\t[8] Polygon\n"
               "[4] Isoceles Triangle\t\t[9] Elipse\n"
               "[5] Scalene Triangle\n\n"
               "[0] Quit\t\t\t[10] Fill\n"
               "> ");

        scanf("%d", &choice);

        switch (choice) {
        case 0:
            return 0;

        case 1:
            printf("Please enter coordinates (startX startY endX endY)\n> ");
            scanf("%d %d %d %d", &startX, &startY, &endX, &endY);

            colour = chooseColour();

            ml.drawLine(startX, startY, endX, endY, colour);
            break;

        case 2:
            printf("Please enter starting coordinates (startX startY)\n> ");
            scanf("%d %d", &startX, &startY);

            printf("Please enter rectangle dimensions (width height)\n> ");
            scanf("%d %d", &width, &height);

            colour = chooseColour();

            ml.drawRectangle(startX, startY, width, height, colour);
            break;

        case 3:
            printf("Please enter starting coordinates (startX startY)\n> ");
            scanf("%d %d", &startX, &startY);

            printf("Please enter the side / width of the triangle\n> ");
            scanf("%d", &width);

            colour = chooseColour();

            ml.drawEquilateralTriangle(startX, startY, width, colour);
            break;

        case 4:
            printf("Please enter starting coordinates (startX startY)\n> ");
            scanf("%d %d", &startX, &startY);

            printf("Please enter dimensions (width height)\n> ");
            scanf("%d %d", &width, &height);

            colour = chooseColour();

            ml.drawIsoscelesTriangle(startX, startY, width, height, colour);

            break;

        case 5:
            printf("Please enter starting coordinates (point1.x point1.y point2.x point2.y point3.x point3.y)\n> ");

            MyCoords points[2];
            for (int i = 0; i < 3; i++)
            {
                scanf("%d %d", &points[i].x, &points[i].y);
            }

            colour = chooseColour();

            ml.drawScaleneTriangle(points, colour);
            break;

        case 6:
            printf("Please enter starting coordinates (startX startY)\n> ");
            scanf("%d %d", &startX, &startY);

            printf("Please enter dimensions (width height)\n> ");
            scanf("%d %d", &width, &height);

            colour = chooseColour();

            ml.drawRightTriangle(startX, startY, width, height, colour);
            break;

        case 7:
            printf("Please enter starting coordinates (centreX centreY)\n> ");
            scanf("%d %d", &startX, &startY);

            printf("Please enter circle diameter\n> ");
            scanf("%d", &width);

            colour = chooseColour();

            ml.drawCircle(startX, startY, width, colour);
            break;

        case 8: {
            printf("Please enter amount of sides, with a minimal of 3\n> ");
            scanf("%d", &polyAmount);

            MyCoords polyPoints[polyAmount - 1];

            for (int i = 0; i < polyAmount; i++)
            {
                printf("Please enter polygon # %i  location (startX startY)\n> ", i);
                scanf("%d %d", &polyPoints[i].x, &polyPoints[i].y);
            }

            colour = chooseColour();

            ml.drawPoly(polyPoints, polyAmount, colour);
        }
            break;

        case 9:
            printf("Please enter starting coordinates (centreX centreY)\n> ");
            scanf("%d %d", &startX, &startY);

            printf("Please enter diameters (verticalD horizontalD)\n> ");
            scanf("%d %d", &height, &width);

            colour = chooseColour();

            ml.drawElipse(startX, startY, width, height, colour);
        break;

        case 10:
            colors fillColour = chooseColour();
            
            MyCoords coord;
            printf("Please enter a coordinate where you'd like to fill (x y)\n> ");
            scanf("%d %d", &coord.x, &coord.y);

            ml.myFill(coord, fillColour);
            break;
        }
    }
    getch();
    closegraph();
}