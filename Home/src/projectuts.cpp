#include <graphics.h>
#include <conio.h>
#include <cmath>

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int cx = getmaxx() / 2;
    int cy = getmaxy() / 2;

    // --- ukuran segitiga sama sisi ---
    int side = 400;
    int height = (int)(sqrt(3) / 2 * side);   // tinggi segitiga
    int R = height / 3;                       // jari-jari lingkaran dalam segitiga
    int yShift = 40;

    // --- Koordinat Batas ---
    int left_bar = cx - 250;
    int top_bar = cy - 250;
    int right_bar = cx + 250;
    int bottom_bar = cy + 250;
    
    // --- Koordinat Segitiga ---
    int tri[] = {
        cx, cy - height / 2 - yShift,         
        cx - side / 2, cy + height / 2 - yShift, 
        cx + side / 2, cy + height / 2 - yShift  
    };

    // --- Pusat Lingkaran ---
    int cyb = cy + height / 6 - yShift;   


    // ----------------------------------------------------------------------------------
    // 1. Persegi (Latar) - MAGENTA & Garis Hitam
    // ----------------------------------------------------------------------------------
    setcolor(MAGENTA);
    setfillstyle(SOLID_FILL, MAGENTA);
    bar(left_bar, top_bar, right_bar, bottom_bar);
    setcolor(BLACK);
    rectangle(left_bar, top_bar, right_bar, bottom_bar);


    // ----------------------------------------------------------------------------------
    // 2. Segitiga - CYAN & Garis Hitam
    // ----------------------------------------------------------------------------------
    setcolor(CYAN);
    setfillstyle(SOLID_FILL, CYAN);
    fillpoly(3, tri);
    setcolor(BLACK);
    drawpoly(3, tri);


    // ----------------------------------------------------------------------------------
    // 3. Lingkaran - LIGHTBLUE & Garis Hitam (dengan Smoothening)
    // ----------------------------------------------------------------------------------
    setcolor(LIGHTBLUE);
    setfillstyle(SOLID_FILL, LIGHTBLUE);
    fillellipse(cx, cyb, R, R);
    
    // Trik Smoothening 
    setcolor(MAGENTA); 
    fillellipse(cx, cyb, R - 2, R - 2); 

    setcolor(LIGHTBLUE);
    setfillstyle(SOLID_FILL, LIGHTBLUE);
    fillellipse(cx, cyb, R - 2, R - 2); 

    // Garis luar hitam untuk lingkaran
    setcolor(BLACK);
    circle(cx, cyb, R);

    // ----------------------------------------------------------------------------------
    // 4. Bentuk Elips Horizontal Terisi KUNING dengan Garis Hitam
    // ----------------------------------------------------------------------------------
    int elips_radius_x = R;
    int elips_radius_y = R / 3; 
    
    // 4a. Isi elips dengan warna KUNING (YELLOW)
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    fillellipse(cx, cyb, elips_radius_x, elips_radius_y);
    
    // 4b. Gambar garis tepi elips menjadi HITAM
    setcolor(BLACK); // PERUBAHAN DI SINI! (Sebelumnya WHITE)
    ellipse(cx, cyb, 0, 360, elips_radius_x, elips_radius_y);


    // ----------------------------------------------------------------------------------
    // 5. Garis Vertikal (Hanya di dalam Elips)
    // ----------------------------------------------------------------------------------
    
    // --- Garis Vertikal (LIGHTRED) yang dibatasi di elips ---
    // Tinggi elips adalah 2 * elips_radius_y. Kita gunakan batas y ini.
    setcolor(LIGHTRED); 
    // Batas bawah dan atas garis hanya sejauh radius_y dari pusat cyb
    line(cx, cyb - elips_radius_y, cx, cyb + elips_radius_y); 
    // PERUBAHAN DI SINI! (Sebelumnya line(cx, cyb - R, cx, cyb + R);)


    getch();
    closegraph();
    return 0;
}