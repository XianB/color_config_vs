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
	UINT16 search_region_type;    //����������״
	INT8   tool_name[32];         //��������

								  /* ��������������� */
	UINT16 search_rect_x0;        //�����������ϵ������
	UINT16 search_rect_y0;        //�����������ϵ�������
	UINT16 search_rect_x1;        //�����������µ������
	UINT16 search_rect_y1;        //�����������µ�������
	UINT16 search_rect_x2;        //�����������µ������
	UINT16 search_rect_y2;        //�����������µ�������
	UINT16 search_rect_x3;        //�����������ϵ������
	UINT16 search_rect_y3;        //�����������ϵ�������
	UINT16 search_rect_w;        //���������
	UINT16 search_rect_h;        //���������

								 /* Բ������������� */
	UINT16 search_circ_x;        //����Բ�ĺ�����
	UINT16 search_circ_y;        //����Բ��������
	UINT16 search_circ_radius;   //����Բ�뾶

								 /* ����������Ӿ��β��� */
	UINT16 ext_rect_x;	         //����������Ӿ�����ʼ�������
	UINT16 ext_rect_y;           //����������Ӿ�����ʼ��������
	UINT16 ext_rect_width;       //����������Ӿ��ο��
	UINT16 ext_rect_height;      //����������Ӿ��θ߶�

	UINT16 color_x;              //�����ɫ����ĺ�����
	UINT16 color_y;              //�����ɫ�����������
	UINT16 color_range;          //�����ɫ������ȡֵ��Χ

	INT32  pix_count;            //���ظ���

	UINT16  hsv_min[3];           //hsv����
	UINT16  hsv_max[3];           //hsv����

	UINT16 top_threshold;        //��������ֵ����
	UINT16 bot_threshold;        //��������ֵ����
} Koyo_Tool_Color_Parameter;

int get_hsv(const UINT8 *yuv, UINT16 *hsv[3]);
// �Զ�����hsv����������������ֵ��ȶ�
int auto_adapt_hsv(const UINT8 *yuv, Koyo_Tool_Color_Parameter *parameter);

#endif // COLORPRE_H
