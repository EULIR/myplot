#include <iostream>
#include <cmath>

using namespace std;

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

void saveCanvas(canvas *cvs, string filename);

canvas *createCanvas(int width, int height, int origX, int origY, int ratio, bool axis);

void plot2D(canvas *cvs, double (*func)(double), double minX, double maxX, int color);

void plotPara(canvas *cvs, double (*funcX)(double), double (*funcY)(double), double minT, double maxT, int color);

double func51(double x) {
    return sqrt(-x * x - 2 * x);
}

double func52(double x) {
    return sqrt(-x * x + 2 * x);
}

double func53(double x) {
    return asin(-x - 1) - 3.14159 / 2;
}

double func54(double x) {
    return asin(x - 1) - 3.14159 / 2;
}

double func55(double t) {
    return 2 * sqrt(2) * (cos(t) + 2 * sin(t));
}

double func56(double t) {
    return 2 * sqrt(2) * (cos(t) - 2 * sin(t));
}

double func57(double t) {
    return 2 * sqrt(2) * (cos(t) + 2 * sin(t));
}

double func58(double t) {
    return 2 * sqrt(2) * (-cos(t) + 2 * sin(t));
}

int plotFunc5() {
    canvas cvs = *createCanvas(800, 600, 400, 300, 30, true);
    plot2D(&cvs, func51, -2, 0, 0x6B8AA0);
    plot2D(&cvs, func53, -2, 0, 0x6B8AA0);
    plot2D(&cvs, func52, 0, 2, 0x6B8AA0);
    plot2D(&cvs, func54, 0, 2, 0x6B8AA0);
    plotPara(&cvs, func55, func56, 3.1416 - atan(0.5), 2 * 3.14159 - atan(0.5), 0xFF0000);
    plotPara(&cvs, func57, func58, -atan(0.5), 3.14159 - atan(0.5), 0xFF0000);
    saveCanvas(&cvs, "./func5.bmp");
    return 0;
}
