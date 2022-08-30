/*
 * Goal：帧差法实现移动物体检测
 * 当监控场景中出现异常物体运动时，帧与帧之间会出现较为明显的差别，两帧相减，得到两帧图像亮度差的绝对值
 * 判断它是否大于阈值来分析视频或图像序列的运动特性，确定图像序列中有无物体运动
*/
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat MoveCheck(Mat& frontMat, Mat& afterMat)
{
	// 创建图像
	Mat frontGray, afterGray, diffGray;
	Mat resframe = afterMat.clone();

	// 灰度处理
	cvtColor(frontMat, frontGray, COLOR_BGR2GRAY);
	cvtColor(afterMat, afterGray, COLOR_BGR2GRAY);
	// imshow("Gray", frontGray);

	// 进行帧差处理，找到在帧与帧之间运动的物体差异
	// 缺点：会把不需要的运动物体也算进来
	absdiff(frontGray, afterGray, diffGray);
	// imshow("absdiff", diffGray);

	// 二值化，黑白分明。但是会产生大量的白色噪点
	threshold(diffGray, diffGray, 27, 255, THRESH_BINARY);
	// imshow("diff", diffGray);

	// 腐蚀处理，去除二值化后的白色噪点。但噪点不可能完全清除，同时一些主要物体也会被腐蚀从而变得不明显
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	erode(diffGray, diffGray, element);
	// imshow("erode", diffGray);

	// 膨胀处理，将腐蚀处理后的白色区域变大(明显)
	Mat element_2 = getStructuringElement(MORPH_RECT, Size(20, 20));
	dilate(diffGray, diffGray, element_2);
	// imshow("dilate", diffGray);

	// 动态物体标记
	vector<vector<Point>> contours;
	findContours(diffGray, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

	// 提取关键点
	vector<vector<Point>> contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	
	int x, y, w, h;
	int num = contours.size();

	for (int i = 0; i < num; i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));

		x = boundRect[i].x;
		y = boundRect[i].y;
		w = boundRect[i].width;
		h = boundRect[i].height;

		// 绘制
		rectangle(resframe, Point(x, y), Point(x + w, y + h), Scalar(0, 255, 0), 4);
	}

	return resframe;
}

// 加载视频
int main(int argc, char *argv[])
{
	Mat frame;
	Mat tempframe;
	Mat res;
	int count = 0;

	VideoCapture cap("C:\\Users\\64855\\Pictures\\视频\\buff_3.mp4");
	while (cap.read(frame))
	{
		count++;
		if (count == 1)
			res = MoveCheck(frame, frame);
		else
			res = MoveCheck(tempframe, frame);

		tempframe = frame.clone();
		imshow("frame", frame);		// 原始图像
		imshow("res", res);			// 款选后的视频帧

		waitKey(25);
	}

	return 0;
}


