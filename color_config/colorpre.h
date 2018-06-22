#ifndef COLORPRE_H
#define COLORPRE_H

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

typedef uchar UINT8;
const int WIDTH = 640;
const int HEIGHT = 480;
using UINT8 = unsigned char;
using UINT16 = unsigned short;

int get_hsv(const UINT8 *yuv, UINT8 *hsv[3] );
#endif // COLORPRE_H
