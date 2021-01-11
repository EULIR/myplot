#include <fstream>
#include <iostream>
#include <string>
#include<cmath>

#pragma pack(2)
using namespace std;

int zero[5][5] = {{0, 1, 1, 1, 0},
                  {0, 1, 0, 1, 0},
                  {0, 1, 0, 1, 0},
                  {0, 1, 0, 1, 0},
                  {0, 1, 1, 1, 0}};
int one[5][5] = {{0, 0, 1, 1, 0},
                 {0, 0, 0, 1, 0},
                 {0, 0, 0, 1, 0},
                 {0, 0, 0, 1, 0},
                 {0, 0, 0, 1, 0}};
int two[5][5] = {{0, 1, 1, 1, 0},
                 {0, 0, 0, 1, 0},
                 {0, 1, 1, 1, 0},
                 {0, 1, 0, 0, 0},
                 {0, 1, 1, 1, 0}};
int three[5][5] = {{0, 1, 1, 1, 0},
                   {0, 0, 0, 1, 0},
                   {0, 1, 1, 1, 0},
                   {0, 0, 0, 1, 0},
                   {0, 1, 1, 1, 0}};
int four[5][5] = {{0, 1, 0, 1, 0},
                  {0, 1, 0, 1, 0},
                  {0, 1, 1, 1, 1},
                  {0, 0, 0, 1, 0},
                  {0, 0, 0, 1, 0}};
int five[5][5] = {{0, 1, 1, 1, 0},
                  {0, 1, 0, 0, 0},
                  {0, 1, 1, 1, 0},
                  {0, 0, 0, 1, 0},
                  {0, 1, 1, 1, 0}};
int six[5][5] = {{0, 1, 1, 1, 0},
                 {0, 1, 0, 0, 0},
                 {0, 1, 1, 1, 0},
                 {0, 1, 0, 1, 0},
                 {0, 1, 1, 1, 0}};
int seven[5][5] = {{0, 1, 1, 1, 0},
                   {0, 0, 0, 1, 0},
                   {0, 0, 0, 1, 0},
                   {0, 0, 0, 1, 0},
                   {0, 0, 0, 1, 0}};
int eight[5][5] = {{0, 1, 1, 1, 0},
                   {0, 1, 0, 1, 0},
                   {0, 1, 1, 1, 0},
                   {0, 1, 0, 1, 0},
                   {0, 1, 1, 1, 0}};
int nine[5][5] = {{0, 1, 1, 1, 0},
                  {0, 1, 0, 1, 0},
                  {0, 1, 1, 1, 0},
                  {0, 0, 0, 1, 0},
                  {0, 1, 1, 1, 0}};

/*
    Settings:
    WIDTH: width of pic
    HEIGHT: height of pic
*/
const int MAX_WIDTH = 800;
const int MAX_HEIGHT = 600;


/*
    strcture canvas:
    pixel: pixels of pic,
            including HEIGHT * WIDTH * 4(B, G, R, Alpha for transparency)s, NOT RGBA!!!
            one-byte(0-255) data
*/
struct canvas {
    int width, height;
    char pixel[MAX_HEIGHT][MAX_WIDTH][4];// width, height, channel(BGRAlpha)
    int origX, origY;
    int parameter[6];
    int ratio;

    canvas(int w, int h, int x, int y, int rratio, int rxx, int rxy, int ryx, int ryy, int rzx,
           int rzy) {
        width = w;
        height = h;
        origX = x;
        origY = y;
        ratio = rratio;
        parameter[0] = rxx;
        parameter[1] = rxy;
        parameter[2] = ryx;
        parameter[3] = ryy;
        parameter[4] = rzx;
        parameter[5] = rzy;
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                pixel[i][j][0] = (char) 0xff; // initialize the pixel matrix to all white
                pixel[i][j][1] = (char) 0xff;
                pixel[i][j][2] = (char) 0xff;
                pixel[i][j][3] = 0;
            }
        }
    }  // initialization
};

/*
    structure BMPFileHeader:
    file header of BMP
    bfSize: sizeof(file header) + sizeof(infoHeader) + sizeof(canvas)
    bfOffBits: numOfBits before the bitstream of canvas starts
*/
struct BMPFileHeader {
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;

    //initialization
    BMPFileHeader() {
        bfType = 0x4D42;// 'BM'
        bfSize = 0;
        bfReserved1 = 0;
        bfReserved2 = 0;
        bfOffBits = 0;
    }
};


/*
    structure BMPInfoHeader:
    biSize: sizeof(infoHeader)
    biWidth, biHeight: width and height of pic
*/
struct BMPInfoHeader {
    unsigned int biSize;
    int biWidth;
    int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;

    //initialization
    BMPInfoHeader() {
        biSize = 0;
        biWidth = MAX_WIDTH, biHeight = MAX_HEIGHT;
        biPlanes = 1, biBitCount = 32, biCompression = 0, biSizeImage = 0;
        biXPelsPerMeter = 0, biYPelsPerMeter = 0;
        biClrUsed = 0, biClrImportant = 0;
    }
};

/*
    function void saveCanvas:
    function used to save a canvas as a BMP file
    input:
        canvas cvs: cvs to be saved
        string filename: desired filepath
    output:
        NONE
*/
void saveCanvas(canvas *cvs, string filename) {
    int width = cvs->width;
    int height = cvs->height;
    int size = width * height * 4;

    //file header
    BMPFileHeader fileHeader;
    fileHeader.bfSize = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + size;
    fileHeader.bfOffBits = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

    //info header
    BMPInfoHeader infoHeader;
    infoHeader.biWidth = width;
    infoHeader.biHeight = height;
    infoHeader.biSize = sizeof(BMPInfoHeader);

    //write BMP file
    ofstream fout;
    fout.open(filename, ios::binary);
    fout.write((char *) &fileHeader, sizeof(BMPFileHeader));
    fout.write((char *) &infoHeader, sizeof(BMPInfoHeader));

    fout.write((char *) cvs->pixel, size);

    fout.close();
}

// fill in the pixel array with an int in the form of 0xRRGGBBAA
void splitPixel(char matrix[], int pixel) {
    matrix[2] = (pixel >> 24) & 0xFF;
    matrix[1] = (pixel & 0xFF0000) >> 16;
    matrix[0] = (pixel & 0xFF00) >> 8;
    matrix[3] = pixel & 0xFF;
}

void sign(int i, int j, canvas *cvs) {
    // draw the "-" before the number x if x<0;\
	// length 2;start at(i,j);end at (i,j+2);
    splitPixel(cvs->pixel[j][i], 0xFF000000);
    splitPixel(cvs->pixel[j][i + 1], 0xFF000000);


}

void draw(int i, int j, int p, canvas *cvs) {//0<=p<=9
    // draw the digit p (5*5) with first (i,j)
    if (p == 1) {
        for (int k = 0; k <= 4; k++)
            for (int l = 0; l <= 4; l++)
                if (one[4 - l][k] == 1) splitPixel(cvs->pixel[j + l][i + k], 0xFF000000);

    }
    if (p == 2) {
        for (int k = 0; k <= 4; k++)
            for (int l = 0; l <= 4; l++)
                if (two[4 - l][k] == 1) splitPixel(cvs->pixel[j + l][i + k], 0xFF000000);

    }
    if (p == 3) {
        for (int k = 0; k <= 4; k++)
            for (int l = 0; l <= 4; l++)
                if (three[4 - l][k] == 1) splitPixel(cvs->pixel[j + l][i + k], 0xFF000000);

    }
    if (p == 4) {
        for (int k = 0; k <= 4; k++)
            for (int l = 0; l <= 4; l++)
                if (four[4 - l][k] == 1) splitPixel(cvs->pixel[j + l][i + k], 0xFF000000);

    }
    if (p == 5) {
        for (int k = 0; k <= 4; k++)
            for (int l = 0; l <= 4; l++)
                if (five[4 - l][k] == 1) splitPixel(cvs->pixel[j + l][i + k], 0xFF000000);

    }
    if (p == 6) {
        for (int k = 0; k <= 4; k++)
            for (int l = 0; l <= 4; l++)
                if (six[4 - l][k] == 1) splitPixel(cvs->pixel[j + l][i + k], 0xFF000000);

    }
    if (p == 7) {
        for (int k = 0; k <= 4; k++)
            for (int l = 0; l <= 4; l++)
                if (seven[4 - l][k] == 1) splitPixel(cvs->pixel[j + l][i + k], 0xFF000000);

    }
    if (p == 8) {
        for (int k = 0; k <= 4; k++)
            for (int l = 0; l <= 4; l++)
                if (eight[4 - l][k] == 1) splitPixel(cvs->pixel[j + l][i + k], 0xFF000000);

    }
    if (p == 9) {
        for (int k = 0; k <= 4; k++)
            for (int l = 0; l <= 4; l++)
                if (nine[4 - l][k] == 1) splitPixel(cvs->pixel[j + l][i + k], 0xFF000000);

    }
    if (p == 0) {
        for (int k = 0; k <= 4; k++)
            for (int l = 0; l <= 4; l++)
                if (zero[4 - l][k] == 1) splitPixel(cvs->pixel[j + l][i + k], 0xFF000000);

    }
}

void xdigit(int l, int i, int j, canvas *cvs) {
//write the number of k=(i,j) on the x axis under every ratio point
//ps: area under k:5*5 every digit;  assume 1<=|l|<=99;
    int k = abs(l);

    if ((k >= 1) && (k <= 9)) {
        //in this case, draw 5*5;
        draw(i - 2, j - 10, k, cvs);
        if (l < 0) sign(i - 5, j - 8, cvs);
    }
    if ((10 <= k) && (k <= 99)) {//draw two 5*5s
        int a = k / 10;
        int b = k % 10;  //let k=a*10+b;
        draw(i - 6, j - 10, a, cvs);
        draw(i, j - 10, b, cvs);
        if (l < 0) sign(i - 9, j - 8, cvs);
    }

}

void ydigit(int l, int i, int j, canvas *cvs) {

    //write the number of k=(i,j) on the y axis on tyhe left of every ratio point
//ps: area under k:5*5 every digit;  assume 1<=|l|<=99;
    int k = abs(l);
    if ((k >= 1) && (k <= 9)) {
        //in this case, draw 5*5;
        draw(i - 10, j - 2, k, cvs);
        if (l < 0) sign(i - 13, j, cvs);
    }
    if ((10 <= k) && (k <= 99)) {//draw two 5*5s
        int a = k / 10;
        int b = k % 10;  //let k=a*10+b;
        draw(i - 15, j - 2, a, cvs);
        draw(i - 10, j - 2, b, cvs);
        if (l < 0) sign(i - 18, j, cvs);
    }
}

canvas *createCanvas(int width, int height, int origX, int origY, int ratio, bool axis) {
    canvas cvs(width, height, origX, origY, ratio, 0, 0, 0, 0, 0, 0);

    // emphasis the origin
    for (int i = origY - 2; i <= origY + 2; ++i) {
        for (int j = origX - 2; j <= origX + 2; ++j) {
            splitPixel(cvs.pixel[i][j], 0xFF000000);
        }
    }

    // return the canvas immediately if no axis is needed
    if (!axis) return &cvs;

    // x-axis and y-axis
    for (int i = 0; i < width; ++i) {
        splitPixel(cvs.pixel[origY][i], 0x00000000);
    }
    for (int i = 0; i < height; ++i) {
        splitPixel(cvs.pixel[i][origX], 0x00000000);
    }

    // draw the scale on x-axis
    for (int i = origX; i < width; i += ratio) {
        for (int j = origY - 2; j <= origY + 2; ++j) {
            splitPixel(cvs.pixel[j][i], 0x00000000);
        }
    }
    for (int i = origX; i >= 0; i -= ratio) {
        for (int j = origY - 2; j <= origY + 2; ++j) {
            splitPixel(cvs.pixel[j][i], 0x00000000);
        }
    }

    for (int i = origX; i < width - 1; i += ratio) {
        xdigit((i - origX) / ratio, i, origY, &cvs);

    }
    for (int i = origX; i >= ratio; i -= ratio) {
        xdigit((i - origX) / ratio, i, origY, &cvs);
    }

    // draw the scale on y-axis
    for (int i = origY; i < height; i += ratio) {
        for (int j = origX - 2; j <= origX + 2; ++j) {
            splitPixel(cvs.pixel[i][j], 0x00000000);
        }
    }
    for (int i = origY; i >= 0; i -= ratio) {
        for (int j = origX - 2; j <= origX + 2; ++j) {
            splitPixel(cvs.pixel[i][j], 0x00000000);

        }
    }

    for (int i = origY; i < height - 1; i += ratio) {
        ydigit((i - origY) / ratio, origX, i, &cvs);
    }
    for (int i = origY; i >= ratio; i -= ratio) {
        ydigit((i - origY) / ratio, origX, i, &cvs);
    }

    return &cvs;
}

// all parameters rxx, rxy, ryx, ryy, rzx, rzy should be 10000 times of their initial value to
// avoid potential float-point problems
canvas *createCanvas3D(int width, int height, int origX, int origY,
                       double rxx, double rxy, double ryx, double ryy, double rzx, double rzy) {
    canvas cvs(width, height, origX, origY, 0, rxx, rxy, ryx, ryy, rzx, rzy);
    return &cvs;
}

/*
    function void demo:
    a demo of saving a all-purple canvas to ./output.bmp
*/
void demo() {
    cout << "Running demo, see output.bmp for results.";
    canvas cvs = *createCanvas(800, 600, 400, 300, 20, true);
//    canvas cvs(100, 600);
//    for (int i = 0; i < cvs.width; i++) {
//        for (int j = 0; j < cvs.height; j++) {
//            cvs.pixel[i][j][0] = 0;
//            cvs.pixel[i][j][1] = 0;
//            cvs.pixel[i][j][2] = 0;
//            cvs.pixel[i][j][3] = 0;
//        }
//    }
    saveCanvas(&cvs, "./demo.bmp");
}
