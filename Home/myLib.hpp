#include <stdio.h>
#include "./myLib.hpp" // Memanggil file header yang sudah kita buat

colors colour;

colors chooseColour(){
    int colorChoice;
    printf("\nSilakan pilih warna:\n"
           "[0] Hitam\t[5] Magenta\t[10] Hijau Terang\t[15] Putih\n"
           "[1] Biru\t[6] Coklat\t[11] Cyan Terang\n"
           "[2] Hijau\t[7] Abu Terang\t[12] Merah Terang\n"
           "[3] Cyan\t[8] Abu Gelap\t[13] Magenta Terang\n"
           "[4] Merah\t[9] Biru Terang\t[14] Kuning\n"
           "> ");
    scanf("%d", &colorChoice);
    return myLib::intToColors(colorChoice);
}

int main() {
    int gm, gd = DETECT;
    // Hapus path "C:\\..." karena seringkali bermasalah.
    // Gunakan (char*)"" agar linker bisa menemukannya secara otomatis.
    initgraph(&gd, &gm, (char*)"");

    myLib ml = myLib();
    int choice;

    // Bagian Midterm yang akan digambar pertama kali
    ml.drawRectangle(-100, -100, 200, 200, WHITE);
    ml.myFill(MyCoords{-99, -99}, RED);
    ml.drawIsoscelesTriangle(-90, -90, 180, 180, YELLOW);
    ml.myFill(MyCoords{0, 0}, LIGHTGREEN); // Mengisi segitiga, bukan -89,-89 agar lebih pasti di tengah
    ml.drawCircle(0, 34, 28, MAGENTA); // Parameter ke-3 adalah radius, bukan diameter
    ml.myFill(MyCoords{0, 34}, LIGHTGRAY); // Mengisi lingkaran
    ml.drawElipse(0, 34, 56, 28, RED);
    ml.myFill(MyCoords{0, 35}, LIGHTBLUE);
    ml.drawLine(0, 62, 0, 6, LIGHTGRAY); // Memperbaiki koordinat garis

    while (true) {
        printf("\nApa yang ingin Anda gambar?\n"
               "[1] Garis\t\t\t[6] Segitiga Siku-siku\n"
               "[2] Persegi/Persegi Panjang\t[7] Lingkaran\n"
               "[3] Segitiga Sama Sisi\t\t[8] Poligon\n"
               "[4] Segitiga Sama Kaki\t\t[9] Elips\n"
               "[5] Segitiga Sembarang\n\n"
               "[0] Keluar\t\t\t[10] Isi Warna\n"
               "> ");
        scanf("%d", &choice);

        switch (choice) {
            case 0:
                closegraph();
                return 0;

            case 1: {
                int startX, startY, endX, endY;
                printf("Masukkan koordinat (startX startY endX endY): ");
                scanf("%d %d %d %d", &startX, &startY, &endX, &endY);
                colour = chooseColour();
                ml.drawLine(startX, startY, endX, endY, colour);
                break;
            }
            case 2: {
                int startX, startY, width, height;
                printf("Masukkan koordinat awal (startX startY): ");
                scanf("%d %d", &startX, &startY);
                printf("Masukkan dimensi (lebar tinggi): ");
                scanf("%d %d", &width, &height);
                colour = chooseColour();
                ml.drawRectangle(startX, startY, width, height, colour);
                break;
            }
            case 3: {
                int startX, startY, length;
                printf("Masukkan koordinat awal (startX startY): ");
                scanf("%d %d", &startX, &startY);
                printf("Masukkan panjang sisi: ");
                scanf("%d", &length);
                colour = chooseColour();
                ml.drawEquilateralTriangle(startX, startY, length, colour);
                break;
            }
            case 4: {
                int startX, startY, width, height;
                printf("Masukkan koordinat awal (startX startY): ");
                scanf("%d %d", &startX, &startY);
                printf("Masukkan dimensi (lebar alas tinggi): ");
                scanf("%d %d", &width, &height);
                colour = chooseColour();
                ml.drawIsoscelesTriangle(startX, startY, width, height, colour);
                break;
            }
            case 5: {
                // DIPERBAIKI: Ukuran array harus 3
                MyCoords points[3];
                printf("Masukkan 3 titik koordinat (x1 y1 x2 y2 x3 y3): ");
                for (int i = 0; i < 3; i++) {
                    scanf("%d %d", &points[i].x, &points[i].y);
                }
                colour = chooseColour();
                ml.drawScaleneTriangle(points, colour);
                break;
            }
            case 6: {
                int startX, startY, width, height;
                printf("Masukkan koordinat awal (startX startY): ");
                scanf("%d %d", &startX, &startY);
                printf("Masukkan dimensi (lebar alas tinggi): ");
                scanf("%d %d", &width, &height);
                colour = chooseColour();
                ml.drawRightTriangle(startX, startY, width, height, colour);
                break;
            }
            case 7: {
                int startX, startY, radius;
                printf("Masukkan koordinat pusat (centreX centreY): ");
                scanf("%d %d", &startX, &startY);
                printf("Masukkan radius lingkaran: ");
                scanf("%d", &radius);
                colour = chooseColour();
                ml.drawCircle(startX, startY, radius, colour);
                break;
            }
            case 8: {
                int polyAmount;
                printf("Masukkan jumlah sisi (minimal 3): ");
                scanf("%d", &polyAmount);
                if (polyAmount < 3) break;
                
                // DIPERBAIKI: Ukuran array harus sesuai jumlah sisi
                MyCoords polyPoints[polyAmount];
                for (int i = 0; i < polyAmount; i++) {
                    printf("Masukkan koordinat titik #%d (x y): ", i + 1);
                    scanf("%d %d", &polyPoints[i].x, &polyPoints[i].y);
                }
                colour = chooseColour();
                ml.drawPoly(polyPoints, polyAmount, colour);
                break;
            }
            case 9: {
                int startX, startY, radiusX, radiusY;
                printf("Masukkan koordinat pusat (centreX centreY): ");
                scanf("%d %d", &startX, &startY);
                printf("Masukkan radius (radiusX radiusY): ");
                scanf("%d %d", &radiusX, &radiusY);
                colour = chooseColour();
                ml.drawElipse(startX, startY, radiusX, radiusY, colour);
                break;
            }
            case 10: {
                MyCoords coord;
                printf("Masukkan koordinat untuk diisi warna (x y): ");
                scanf("%d %d", &coord.x, &coord.y);
                colors fillColour = chooseColour();
                ml.myFill(coord, fillColour);
                break;
            }
        }
    }
    return 0;
}