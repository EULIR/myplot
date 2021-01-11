#include <iostream>

using namespace std;

void splitPixel(char matrix[], int pixel);

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

void connect(canvas *cvs, int x1, int y1, int x2, int y2, int color) {
    if (x1 > x2) {
        int temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
    }
    int startX = min(x1, x2);
    int endX = max(x1, x2);
    int startY = min(y1, y2);
    int endY = max(y1, y2);
    int width = endX - startX;
    int height = endY - startY;
    if (width == 0) {
        for (int i = startY; i <= endY; ++i) {
            if (i < 0 || i >= cvs->height) continue;
            splitPixel(cvs->pixel[i][startX], color);
        }
        return;
    }
    if (height == 0) {
        for (int i = startX; i <= endX; ++i) {
            if (i < 0 || i >= cvs->width) continue;
            splitPixel(cvs->pixel[startY][i], color);
        }
        return;
    }
    if (y1 <= y2) {
        if (width >= height) {
            int ratio = width / height;
            int index = startX;
            for (int i = startY; i <= endY; ++i) {
                for (int j = 0; j < ratio + 1; ++j) {
                    if (i < 0 || i >= cvs->height) continue;
                    if (index + j < 0 || index + j >= cvs->width) continue;
                    splitPixel(cvs->pixel[i][index + j], color);
                }
                index++;
            }
        } else {
            int ratio = height / width;
            int index = startY;
            for (int i = startX; i <= endX; ++i) {
                for (int j = 0; j < ratio + 1; ++j) {
                    if (i < 0 || i >= cvs->width) continue;
                    if (index + j < 0 || index + j >= cvs->height) continue;
                    splitPixel(cvs->pixel[index + j][i], color);
                }
                index++;
            }
        }
    } else {
        if (width >= height) {
            int ratio = width / height;
            int index = startX;
            for (int i = endY; i >= startY; --i) {
                for (int j = 0; j < ratio + 1; ++j) {
                    if (i < 0 || i >= cvs->height) continue;
                    if (index + j < 0 || index + j >= cvs->width) continue;
                    splitPixel(cvs->pixel[i][index + j], color);
                }
                index++;
            }
        } else {
            int ratio = height / width;
            int index = startY;
            for (int i = endX; i >= startX; --i) {
                for (int j = 0; j < ratio + 1; ++j) {
                    if (i < 0 || i >= cvs->width) continue;
                    if (index + j < 0 || index + j >= cvs->height) continue;
                    splitPixel(cvs->pixel[index + j][i], color);
                }
                index++;
            }
        }
    }
}

void plot2D(canvas *cvs, double (*func)(double), double minX, double maxX, int color) {
    int prevX = (int) (cvs->origX + cvs->ratio * minX);
    int prevY = int(((*func)((double(prevX) - cvs->origX) / (double) cvs->ratio)) * (cvs->ratio) + cvs->origY);
    for (int i = (int) (cvs->origX + cvs->ratio * minX); i <= cvs->origX + cvs->ratio * maxX; i++) {
        int y = int(((*func)((double(i) - cvs->origX) / (double) cvs->ratio)) * (cvs->ratio) + cvs->origY);
        if (y < 0 || y >= cvs->height) continue;
        if (i < 0 || i >= cvs->width) continue;
        connect(cvs, prevX, prevY, i, y, color);
        prevX = i;
        prevY = y;
        splitPixel(cvs->pixel[y][i], color);
    }
}


void plotPara(canvas *cvs, double (*funcX)(double), double (*funcY)(double), double minT, double maxT, int color) {
    int init = (int) (cvs->ratio * minT);
    int prevX = int(((*funcX)(double(init) / (double) cvs->ratio) * (cvs->ratio) + cvs->origX));
    int prevY = int(((*funcY)(double(init) / (double) cvs->ratio)) * (cvs->ratio) + cvs->origY);
    for (int i = (int) (cvs->ratio * minT); i <= cvs->ratio * maxT; i++) {
        int x = int(((*funcX)(double(i) / (double) cvs->ratio) * (cvs->ratio) + cvs->origX));
        int y = int(((*funcY)(double(i) / (double) cvs->ratio)) * (cvs->ratio) + cvs->origY);
        if (y < 0 || y >= cvs->height) continue;
        if (x < 0 || x >= cvs->width) continue;
        connect(cvs, prevX, prevY, x, y, color);
        prevX = x;
        prevY = y;
        splitPixel(cvs->pixel[y][x], color);
    }
}

void plot3D(canvas *cvs, double (*func)(double, double), double minX, double maxX, double minY, double maxY) {
    double rxx = (double) cvs->parameter[0] / 10000;
    double rxy = (double) cvs->parameter[1] / 10000;
    double ryx = (double) cvs->parameter[2] / 10000;
    double ryy = (double) cvs->parameter[3] / 10000;
    double rzx = (double) cvs->parameter[4] / 10000;
    double rzy = (double) cvs->parameter[5] / 10000;

//    double z = func(minX, minY);
//    double imgX = 0 + rxx * minX + ryx * minY + rzx * z;
//    double imgY = 0 + rxy * minX + ryy * minY + rzy * z;
//    int prevX = (int) (imgX + cvs->origX);
//    int prevY = (int) (imgY + cvs->origY);
    for (double i = minX; i < maxX; i += 0.1) {
        for (double j = minY; j < maxY; j += 0.1) {
            double z = func(i, j);
            // double imgX = 0 + 24.8438 * i + -16.175 * j + 0 * z;
            double imgX = 0 + rxx * i + ryx * j + rzx * z;
            // double imgY = 0 + -5.375 * i + -5.3 * j + -145 * z;
            double imgY = 0 + rxy * i + ryy * j + rzy * z;
            int x = (int) (imgX + cvs->origX);
            int y = (int) (imgY + cvs->origY);
            if (y < 0 || y >= cvs->height) continue;
            if (x < 0 || x >= cvs->width) continue;
//            connect(cvs, prevX, prevY, x, y, 0x00FF00);
//            prevX = x;
//            prevY = y;
            splitPixel(cvs->pixel[y][x], 0x00ff00);
        }
    }
}
