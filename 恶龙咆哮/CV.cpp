// 实验读文件.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include<fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/types_c.h>


using namespace std;
using namespace cv;
Mat image, image_gray;
int h_min = 140;
int s_min = 100;
int v_min = 100;

int h_max = 180;
int s_max = 255;
int v_max = 255;

int test() {

	VideoCapture capture;
	Mat frame;

	//注意下面的连接部分，admin:123（账号密码），
	//@符号之后的是局域网ip地址(打开app后，点击下方“打开IP摄像头服务器”，会有显示局域网ip)
	//即：http://<USERNAME>:<PASSWORD>@<IP_ADDRESS>/<the value of src>
	capture.open(2);
	//"http://admin:admin@192.168.43.57:8081"
	//"http://admin:admin@192.168.43.57:8888"
	//"http://admin:admin@192.168.137.240:8081"
	//"http://admin:admin@10.0.155.223:8081"
	//capture.open(0);
	//循环显示每一帧
	while (1)
	{
		capture >> frame;//读取当前每一帧画面
		if (!capture.isOpened())return -1;
		cv::imshow("读取视频", frame);//显示当前帧
		cv::waitKey(30);//延时30ms
	}
	return 0;
}
void rlsb() {

	cvtColor(image, image_gray, CV_BGR2GRAY);//转为灰度图
	equalizeHist(image_gray, image_gray);//直方图均衡化，增加对比度方便处理

	CascadeClassifier eye_Classifier;  //载入分类器
	CascadeClassifier face_cascade;    //载入分类器

									   //加载分类训练器，OpenCv官方文档提供的xml文档，可以直接调用
									   //xml文档路径  opencv\sources\data\haarcascades 


	if (!face_cascade.load("C:\\Users\\19354\\Desktop\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml"))
	{
		cout << "Load haarcascade_frontalface_alt failed!" << endl;
		return;
	}

	//vector 是个类模板 需要提供明确的模板实参 vector<Rect>则是个确定的类 模板的实例化

	vector<Rect> faceRect;




	//检测关于脸部位置
	face_cascade.detectMultiScale(image_gray, faceRect, 1.1, 2, 0, Size(30, 30));//检测
	for (size_t i = 0; i < faceRect.size(); i++)
	{
		rectangle(image, faceRect[i], Scalar(0, 0, 255));      //用矩形画出检测到的位置
	}
	imshow("人脸识别图", image);         //显示当前帧
	char c = waitKey(1);         //延时30ms，即每秒播放33帧图像
	return;
}

int main()
{
	bool sus = 0;
	//test();
	ofstream fout;
	//test();
	//定义两个Mat变量，用于存储每一帧的图像
	VideoCapture capture;
	capture.open(0);

	while (1)                  //循环显示每一帧
	{
		fout.open("a.txt");

		Mat srcImage; capture >> srcImage;
		//imshow("src", srcImage);
		Mat tempImage = srcImage.clone();
		cvtColor(srcImage, srcImage, CV_BGR2HSV);
		Scalar hsv_min(h_min, s_min, v_min);
		Scalar hsv_max(h_max, s_max, v_max);
		Mat dstImage = Mat::zeros(srcImage.size(), CV_8U);
		inRange(srcImage, hsv_min, hsv_max, dstImage);
		Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
		morphologyEx(dstImage, dstImage, MORPH_ERODE, element, Point(-1, -1), 1);
		morphologyEx(dstImage, dstImage, MORPH_DILATE, element, Point(-1, -1), 4);
		//imshow("dst", dstImage);
		vector<vector<Point>>contours;
		findContours(dstImage, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
		vector<Rect>rect(contours.size());
		int mid_x = 0;
		int mid_y = 0;
		for (int i = 0; i < contours.size(); i++)
		{
			rect[i] = boundingRect(contours[i]);
			int x = rect[i].x;
			int y = rect[i].y;
			int width = rect[i].width;
			int height = rect[i].height;
			rectangle(tempImage, Point(x, y), Point(x + width, y + height), Scalar(0, 255, 0), 2);
			mid_x += (x + (width >> 1));
			mid_y += (y + (height >> 1));
		}
		if (contours.size() == 0) {
			//fout << 5 << endl;
			waitKey(300);
			imshow("result", tempImage);
			continue;
		}
		sus = 1;
		fout << 5 << endl;
		waitKey(100);
		Point mid(mid_x / contours.size(), mid_y / contours.size());
		rectangle(tempImage, mid - Point(5, 5), mid + Point(5, 5), Scalar(0, 255, 0), 2);
		//	printf("%d %d\n", mid.x, mid.y);
		//	printf("\n%d %d\n", srcImage.rows, srcImage.cols);
		int l = (srcImage.cols >> 1) - 0.10 * srcImage.cols;
		int r = (srcImage.cols >> 1) + 0.10 * srcImage.cols;
		if (mid.x < l)fout << 4 << endl;


		else if (mid.x > r)fout << 6 << endl;

		else fout << 5 << endl;

		imshow("result", tempImage);
		fout.close();
		waitKey(300);





	}
	return 0;
}