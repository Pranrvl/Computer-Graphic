#include <stdio.h> //input dan output 
#include <stdlib.h> 
#include <conio.h> //getch()
#include <graphics.h>
#include <math.h>

int koor_tr[2][2];
int koor_sc[2][2];
int koor_rt[2][2];

void translate2(int koor[2][2], int translate[2][2])
{
    for(int i = 0; i < 2; ++i)
        for(int j = 0; j < 2; ++j)
            koor_tr[i][j] = koor[i][j] + translate[j][j];
}

void scaling2(int koor[2][2], int scaling[2][2])
{
    for(int i = 0; i < 2; ++i)
        for(int j = 0; j < 2; ++j)
        {
            koor_sc[i][j] = 0;
            for (int k = 0; k < 2; k++)
                koor_sc[i][j] += koor[i][k] * scaling[k][j];
        }            
}

void rotation2(int koor[2][2], int rotation[2][2])
{
   ..... // hampir mirip dengan scaling2
}

int main()
{
    int gm, x, y, gd = DETECT, i;
    
    char data[] = "C:\\MinGW\\lib\\libbgi.a"; //static file

    initgraph(&gd, &gm, data);
    x = getmaxx(); // to get the co-ordinates i.e. x & y
    y = getmaxy();
    cleardevice();
    
    int koor[2][2] = {{100,50},{200,50}};
    int translation[2][2] =  {{20,0},{0,20}};
    translate2(koor, translation);
    
    setcolor(RED);
    line(koor[0][0],koor[0][1],koor[1][0],koor[1][1]);
    setcolor(BLUE);
    line(koor_tr[0][0],koor_tr[0][1],koor_tr[1][0],koor_tr[1][1]);   

    int scaling[2][2] =  {{2,0},{0,2}};
    scaling2(koor, scaling);

    setcolor(YELLOW);
    line(koor_tr[0][0],koor_tr[0][1],koor_sc[1][0],koor_sc[1][1]);   

    float degree = ...;
    float radian = ...;
    float rotation = {{....,....},{....,...}};
    rotation2(koor, rotation);

    setcolor(GREEN);
    line(koor_tr[0][0],koor_tr[0][1],koor_rt[1][0],koor_rt[1][1]);

    getch();
    closegraph();

    return 0;
}