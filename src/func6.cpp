#include <iostream>

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

void plot3D(canvas *cvs, double (*func)(double, double), double minX, double maxX, double minY, double maxY);

canvas *createCanvas3D(int width, int height, int origX, int origY,
                       double rxx, double rxy, double ryx, double ryy, double rzx, double rzy);

double func6(double x, double y) {
    double r = sqrt(x * x + y * y);
    if (r == 0.0) return 1.0;
    return sin(r) / r;
}

int plotFunc6() {
    canvas cvs = *createCanvas3D(800, 600, 400, 300, 248438, -53750, -161750, -53000, 0, -1450000);
    plot3D(&cvs, func6, -8, 8, -8, 8);
    saveCanvas(&cvs, "./func6.bmp");
    return 0;
}