#include "stdafx.h"
#include "colorpre.h"
#include <iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

int cmpint(const void *a, const void *b)
{
	return *(int*)a - *(int*)b;
}

line_struct get_line_equation(IPoint p1, IPoint p2) {
	line_struct line;
	line.k = 1.0f * (p1.y - p2.y) / (p1.x - p2.x);
	line.b = p1.y - p1.x * line.k;
	return line;
}

// 高度的范围是从点中y最小到y最大

void get_intersections(line_struct lines[], unsigned short intersection[][2], IPoint minPoint, IPoint maxPoint)
{
	// 首先判断四条直线中是否有垂直的x轴的，如果有垂直x轴的就直接使用minPoint和maxPoint来计算即可
	int isvertical = false;
	for (int i = 0; i < 4; ++i) {
		if (isinf(lines[i].k)) {
			isvertical = true;
		}
	}
	if (isvertical) {
		for (int i = minPoint.y; i < maxPoint.y; ++i) {
			intersection[i][0] = minPoint.x;
			intersection[i][1] = maxPoint.x;
		}
	}
	else {
		for (int i = minPoint.y; i < maxPoint.y; ++i) {
			// 获取在该处四条直线的交点，之保留在外界矩形内的交点
			int x[4];
			x[0] = 1.0f * (i - lines[0].b) / lines[0].k;
			x[1] = 1.0f * (i - lines[1].b) / lines[1].k;
			x[2] = 1.0f * (i - lines[2].b) / lines[2].k;
			x[3] = 1.0f * (i - lines[3].b) / lines[3].k;
			qsort(x, 4, sizeof(x[0]), cmpint);
			intersection[i][0] = x[1];
			intersection[i][1] = x[2];
		}
	}
}


void get_intersections_circle(circle_struct circle, unsigned short intersection[][2], IPoint minPoint, IPoint maxPoint)
{
	UINT16 radius = circle.radius;
	UINT16 cenx = circle.center.x;
	UINT16 ceny = circle.center.y;
	for (int i = minPoint.y; i < maxPoint.y; ++i) {
		int x = sqrt(radius * radius - (ceny - i) * (ceny - i));
		intersection[i][0] = cenx - x;
		intersection[i][1] = cenx + x;
	}
}

int get_point_count_with_region(UINT16 *hsv[3], Koyo_Tool_Color_Parameter *parameter, int *hsv_min, int *hsv_max, unsigned short intersection[][2]) {
	int extw = parameter->ext_rect_width;
	int exth = parameter->ext_rect_height;

	UINT8 hmax = hsv_max[0], smax = hsv_max[1], vmax = hsv_max[2];
	UINT8 hmin = hsv_min[0], smin = hsv_min[1], vmin = hsv_min[2];
 	int imax = exth;
	int color_step_skip_factor = 1;

	int pix_count = 0;
	for (int i = 0; i < imax; i += color_step_skip_factor) {
		//write(fd,hsv_row_start_u8c3p,len)
		if (!(intersection[i][0] == 0 && intersection[i][1] == 0)) {
			for (int j = intersection[i][0]; j < intersection[i][1]; j += color_step_skip_factor) {
				int y = i;
				int x = j;
				//std::cout << "x: " << x << "y:" << y << std::endl;
				
				UINT16 h_u16 = hsv[0][y * extw + x];
				UINT16 s_u16 = hsv[1][y * extw + x];
				UINT16 v_u16 = hsv[2][y * extw + x];
				if (hmax >= 256 && (h_u16 <= (hmax - 256))) h_u16 += 256;
				if (h_u16 >= hmin && h_u16 <= hmax
					&& s_u16 >= smin && s_u16 <= smax
					&& v_u16 >= vmin && v_u16 <= vmax) {
					pix_count++;
				}
				else {
				}
			}
		}
	}
	return pix_count;
}

void printHSV(cv::Mat hsvimage) {
    std::ofstream of;
    of.open("csc.txt");
    for (int i = 0; i < hsvimage.rows; ++i) {
        for (int j = 0; j < hsvimage.cols; ++j) {
            auto hsvv =hsvimage.at<cv::Vec3b>(i,j);
            //printf("(%d,%d)H= %d,S = %d, V = %d\n",i,j,hsvv[0],hsvv[1],hsvv[2]);
            of<< "H: " << (int)hsvv[0] << " " << "S: " <<(int)hsvv[1] << " " << "V: " <<(int)hsvv[2] << " i: " << i << " " << "j: " << j << " " << std::endl;;
        }
    }
    of.close();
}
int get_hsv(const UINT8 *yuv, UINT16 *hsv[3] ) {
	std::cout << "yanse start" << std::endl;
    cv::Mat imgRGB, imgHSV, img_h, img_s, img_v;

    cv::Mat imgYUV(480 + 480 / 2, 640, CV_8UC1, (void*)yuv);
    // imgRGB.create(480, 640, CV_8UC3);
    cv::cvtColor(imgYUV, imgRGB, CV_YUV420sp2BGR, 3);
    cv::cvtColor(imgRGB, imgHSV, CV_BGR2HSV_FULL, 3);

    cv::Mat hsv_vec[3];
    cv::split(imgHSV, hsv_vec);
    img_h = hsv_vec[0];
    img_s = hsv_vec[1];
    img_v = hsv_vec[2];

    if (!yuv) {
        std::cout << "No allocate memory!\n" << std::endl;
        return -1;
    }

    // cv::imshow("imgRGB", imgRGB);
    // cvWaitKey(0);
    // cv::imshow("imgHSV_H", img_h);
    // cvWaitKey(0);
    // cv::imshow("imgHSV_S", img_s);
    // cvWaitKey(0);
    // cv::imshow("imgHSV_V", img_v);
    // cvWaitKey(0);

    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            hsv[0][i * WIDTH + j] = img_h.at<uchar>(i, j);
            hsv[1][i * WIDTH + j] = img_s.at<uchar>(i, j);
            hsv[2][i * WIDTH + j] = img_v.at<uchar>(i, j);
        }
    }

    printHSV(imgHSV);
    return 0;
}

int get_hsv_with_region(const UINT8 *yuv, UINT16 *hsv[3], int extw, int exth, int extx, int exty) {
	std::cout << "yanse start" << std::endl;
	cv::Mat imgRGB, imgHSV, img_h, img_s, img_v;

	cv::Mat imgYUV(480 + 480 / 2, 640, CV_8UC1, (void*)yuv);
	// imgRGB.create(480, 640, CV_8UC3);
	cv::cvtColor(imgYUV, imgRGB, CV_YUV420sp2BGR, 3);

	// 裁剪图片
	cv::Rect rect(extx, exty, extw, exth);
	cv::Mat imgCut(imgRGB, rect);
	cv::Mat imgCopy = imgCut.clone();
	//cv::imshow("imgCopy", imgCopy);
	//cv::waitKey(0);

	cv::cvtColor(imgCopy, imgHSV, CV_BGR2HSV_FULL, 3);

	cv::Mat hsv_vec[3];
	cv::split(imgHSV, hsv_vec);
	img_h = hsv_vec[0];
	img_s = hsv_vec[1];
	img_v = hsv_vec[2];

	if (!yuv) {
		std::cout << "No allocate memory!\n" << std::endl;
		return -1;
	}

	// cv::imshow("imgRGB", imgRGB);
	// cvWaitKey(0);
	// cv::imshow("imgHSV_H", img_h);
	// cvWaitKey(0);
	// cv::imshow("imgHSV_S", img_s);
	// cvWaitKey(0);
	// cv::imshow("imgHSV_V", img_v);
	// cvWaitKey(0);

	for (int i = 0; i < exth; ++i) {
		for (int j = 0; j < extw; ++j) {
			hsv[0][i * extw + j] = img_h.at<uchar>(i, j);
			hsv[1][i * extw + j] = img_s.at<uchar>(i, j);
			hsv[2][i * extw + j] = img_v.at<uchar>(i, j);
		}
	}

	printHSV(imgHSV);
	return 0;
}

int auto_adapt_hsv(const UINT8 *yuv, Koyo_Tool_Color_Parameter *parameter) {

	int extw = parameter->ext_rect_width;
	int exth = parameter->ext_rect_height;

	// 计算交集
	UINT16 intersection[500][2];
	IPoint maxPoint, minPoint;
	minPoint.x = 0;
	minPoint.y = 0;

	maxPoint.x = parameter->ext_rect_width;
	maxPoint.y = parameter->ext_rect_height;

	if (parameter->search_region_type == COLOR_DETECTION_REGION_RECT) {

		IPoint p0, p1, p2, p3;
		p0.x = parameter->search_rect_x0 - parameter->ext_rect_x;
		p0.y = parameter->search_rect_y0 - parameter->ext_rect_y;
		p1.x = parameter->search_rect_x1 - parameter->ext_rect_x;
		p1.y = parameter->search_rect_y1 - parameter->ext_rect_y;
		p2.x = parameter->search_rect_x2 - parameter->ext_rect_x;
		p2.y = parameter->search_rect_y2 - parameter->ext_rect_y;
		p3.x = parameter->search_rect_x3 - parameter->ext_rect_x;
		p3.y = parameter->search_rect_y3 - parameter->ext_rect_y;

		line_struct lines[4];
		lines[0] = get_line_equation(p0, p1);
		lines[1] = get_line_equation(p1, p2);
		lines[2] = get_line_equation(p2, p3);
		lines[3] = get_line_equation(p3, p0);

		get_intersections(lines, intersection, minPoint, maxPoint);
	}
	else if (parameter->search_region_type == COLOR_DETECTION_REGION_CIRCLE) {
		circle_struct circle;
		circle.radius = parameter->search_circ_radius;
		circle.center.x = parameter->search_circ_x - parameter->ext_rect_x;
		circle.center.y = parameter->search_circ_y - parameter->ext_rect_y;

		minPoint.x = 0;
		minPoint.y = 0;

		maxPoint.x = parameter->ext_rect_width;
		maxPoint.y = parameter->ext_rect_height;

		get_intersections_circle(circle, intersection, minPoint, maxPoint);
	}

	std::cout << "start" << std::endl;
	// 计算原图中符合要求的点的个数
	UINT16 *hsv[3];
	hsv[0] = new UINT16[extw * exth];
	hsv[1] = new UINT16[extw * exth];
	hsv[2] = new UINT16[extw * exth];
	get_hsv_with_region(yuv, hsv, extw, exth, parameter->ext_rect_x, parameter->ext_rect_y);

	int hsv_min_end[3], hsv_max_end[3];
	for (int i = 0; i < 3; ++i) {
		hsv_min_end[i] = parameter->hsv_min[i];
		hsv_max_end[i] = parameter->hsv_max[i];
	}
	int start_count = get_point_count_with_region(hsv, parameter, hsv_min_end, hsv_max_end, intersection);
	std::cout << "pix_raw_count: " << start_count << std::endl;
	int end_count = start_count * 1.04;
	std::cout << "pix_end_count: " << end_count << std::endl;

	// 循环增加h,s,v的值，若都超过预定的值，结束循环
	UINT16 step = 1;  // 预设增加步长是1
	bool h_flag = true, s_flag = true, v_flag = true;
	while (h_flag || s_flag || v_flag) {
		// v
		if (v_flag) {
			hsv_min_end[2] = hsv_min_end[2] - step;
			hsv_max_end[2] = hsv_max_end[2] + step;
			if (hsv_min_end[2] < 0 ||
				hsv_max_end[2] > 255) {
				hsv_min_end[2] = hsv_min_end[2] + step;
				hsv_max_end[2] = hsv_max_end[2] - step;
				v_flag = false;
			}
			if (get_point_count_with_region(hsv, parameter, hsv_min_end, hsv_max_end, intersection) < end_count) {

				//std::cout << "V" << std::endl;
			}
			else {
				hsv_min_end[2] = hsv_min_end[2] + step;
				hsv_max_end[2] = hsv_max_end[2] - step;
				v_flag = false;
			}
		}
		// h
		if (h_flag) {
			hsv_min_end[0] = hsv_min_end[0] - step;
			hsv_max_end[0] = hsv_max_end[0] + step;
			// 判断如果越界
			if (hsv_min_end[0] < 0 ||
				hsv_max_end[0] > 255) {
				hsv_min_end[0] = hsv_min_end[0] + step;
				hsv_max_end[0] = hsv_max_end[0] - step;
				h_flag = false;
			}
			if (get_point_count_with_region(hsv, parameter, hsv_min_end, hsv_max_end, intersection) < end_count) {

				//std::cout << "H" << std::endl;
			}
			else {
				hsv_min_end[0] = hsv_min_end[0] + step;
				hsv_max_end[0] = hsv_max_end[0] - step;
				h_flag = false;
			}
		}
		// s
		if (s_flag) {
			hsv_min_end[1] = hsv_min_end[1] - step;
			hsv_max_end[1] = hsv_max_end[1] + step;
			// 判断如果越界
			if (hsv_min_end[1] < 0 ||
				hsv_max_end[1] > 255) {
				hsv_min_end[1] = hsv_min_end[1] + step;
				hsv_max_end[1] = hsv_max_end[1] - step;
				s_flag = false;
			}
			if (get_point_count_with_region(hsv, parameter, hsv_min_end, hsv_max_end, intersection) < end_count) {

				//std::cout << "S" << std::endl;
			}
			else {
				hsv_min_end[1] = hsv_min_end[1] + step;
				hsv_max_end[1] = hsv_max_end[1] - step;
				s_flag = false;
			}
		}
		
	}
	std::cout << "最开始的hsv值为h_min: " << parameter->hsv_min[0] << " s_min: " << parameter->hsv_min[1] << " v_min: " << parameter->hsv_min[2] << std::endl;
	std::cout << "最开始的hsv值为h_max: " << parameter->hsv_max[0] << " s_max: " << parameter->hsv_max[1] << " v_max: " << parameter->hsv_max[2] << std::endl;

	std::cout << "最后的hsv值为h_min: " << (int)hsv_min_end[0] << " s_min: " << (int)hsv_min_end[1] << " v_min: " << (int)hsv_min_end[2] << std::endl;
	std::cout << "最后的hsv值为h_max: " << (int)hsv_max_end[0] << " s_max: " << (int)hsv_max_end[1] << " v_max: " << (int)hsv_max_end[2] << std::endl;

	// 取中值作为最终值，赋值到原数据结构中
	for (int i = 0; i < 3; ++i) {
		parameter->hsv_min[i] = (parameter->hsv_min[i] + hsv_min_end[i]) / 2;
		parameter->hsv_max[i] = (parameter->hsv_max[i] + hsv_max_end[i]) / 2;
	}

	std::cout << "取中值后的hsv值为h_min: " << parameter->hsv_min[0] << " s_min: " << parameter->hsv_min[1] << " v_min: " << parameter->hsv_min[2] << std::endl;
	std::cout << "取中值后的hsv值为h_max: " << parameter->hsv_max[0] << " s_max: " << parameter->hsv_max[1] << " v_max: " << parameter->hsv_max[2] << std::endl;

	// free memory
	for (int i = 0; i < 3; ++i) {
		delete[] hsv[i];
	}

	return 0;
}

//int main(int argc, char ** argv) {
    
//    std::string filename(argv[1]);
//    unsigned char *buf = nullptr;
//    uchar *hsv[3];
//    hsv[0] = new uchar[640*480];
//    hsv[1] = new uchar[640*480];
//    hsv[2] = new uchar[640*480];

//    if (filename.substr(filename.size() - 3, 3) == "yuv") {
//        FILE *yuv_file = fopen(filename.c_str(), "rb+");

//        buf = new unsigned char[640 * 480 + 320 * 240 * 2];
//        fread(buf, 640 * 480 + 320 * 240 * 2, 1, yuv_file);
//        fclose(yuv_file);

        
//        get_hsv(buf, hsv);
//        std::cout << std::endl;
//    }

//    return 0;
//}
