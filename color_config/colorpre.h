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
using INT8 = char;
using INT32 = int;
using UINT8 = unsigned char;
using UINT16 = unsigned short;

const int COLOR_DETECTION_REGION_RECT = 1;
const int COLOR_DETECTION_REGION_CIRCLE = 0;

typedef struct IPoint_ {
	UINT16 x;
	UINT16 y;
}IPoint;

typedef struct _LINE_STRUCT_ {
	float k;
	float b;
} line_struct;

typedef struct _CIRCLE_STRUCT_ {
	IPoint center;
	UINT16 radius;
} circle_struct;

typedef struct KOYO_TOOL_COLOR_PARAMETER_
{
	UINT16 search_region_type;    //搜索区域形状
	INT8   tool_name[32];         //工具名称

								  /* 矩形搜索区域参数 */
	UINT16 search_rect_x0;        //搜索区域左上点横坐标
	UINT16 search_rect_y0;        //搜索区域左上点纵坐标
	UINT16 search_rect_x1;        //搜索区域左下点横坐标
	UINT16 search_rect_y1;        //搜索区域左下点纵坐标
	UINT16 search_rect_x2;        //搜索区域右下点横坐标
	UINT16 search_rect_y2;        //搜索区域右下点纵坐标
	UINT16 search_rect_x3;        //搜索区域右上点横坐标
	UINT16 search_rect_y3;        //搜索区域右上点纵坐标
	UINT16 search_rect_w;        //搜索区域宽
	UINT16 search_rect_h;        //搜索区域高

								 /* 圆形搜索区域参数 */
	UINT16 search_circ_x;        //搜索圆心横坐标
	UINT16 search_circ_y;        //搜索圆心纵坐标
	UINT16 search_circ_radius;   //搜索圆半径

								 /* 搜索区域外接矩形参数 */
	UINT16 ext_rect_x;	         //搜索区域外接矩形起始点横坐标
	UINT16 ext_rect_y;           //搜索区域外接矩形起始点纵坐标
	UINT16 ext_rect_width;       //搜索区域外接矩形宽度
	UINT16 ext_rect_height;      //搜索区域外接矩形高度

	UINT16 color_x;              //检测颜色坐标的横坐标
	UINT16 color_y;              //检测颜色坐标的纵坐标
	UINT16 color_range;          //检测颜色的上下取值范围

	INT32  pix_count;            //像素个数

	UINT16  hsv_min[3];           //hsv下限
	UINT16  hsv_max[3];           //hsv上限

	UINT16 top_threshold;        //处理结果阈值上限
	UINT16 bot_threshold;        //处理结果阈值下限
} Koyo_Tool_Color_Parameter;

int get_hsv(const UINT8 *yuv, UINT16 *hsv[3]);
// 自动调节hsv，增加冗余便于评分的稳定
int auto_adapt_hsv(const UINT8 *yuv, Koyo_Tool_Color_Parameter *parameter);

#endif // COLORPRE_H
