// 读速度（不是读坐标）.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>
#include <cmath>
#include <math.h>
#include <Windows.h>
#define Wheelbase 15    //轴距暂定15厘米
#define WheelDia 10     //车轮直径(Wheel Diameter)暂定10厘米
#define threshold 1    //阈值，转速低于阈值则认为轮子不转
#define delta  0.5        //阈值，左右转速差低于Δ则认为走直线
#define x0 400
#define y0 400
#define interval 0.5      //每次测量的时间间隔
#define M_PI 3.14159265358979323846
using namespace std;
struct  node {
	double x;
	double y;
	node(int _x, int _y) :x(_x), y(_y) {}
	node() {}
}single_dot;
vector<node>route;        //保存路径用，不知道之后返回的时候是不是有用
void draw(double &startx, double &starty, int left_speed, int right_speed, double angle) {
	if (abs(left_speed - right_speed) > delta) {
		//抛出异常，小车重新调速
	}
	node tmp(startx, starty);
	route.push_back(tmp);
	double center_speed = (left_speed + right_speed) / 2;//速度瞬心
	startx += center_speed * interval*cos(angle);
	starty += center_speed * interval*sin(angle);
	startx *= 100;
	startx = round(startx);
	startx = (double)startx / (double)100;
	starty *= 100;
	starty = round(starty);
	starty = (double)starty / (double)100;//去掉多余的小数位
	single_dot.x = startx;
	single_dot.y = starty;
	route.push_back(single_dot);

	//画线
	HWND hWnd = GetConsoleWindow();
	HDC hdc = GetWindowDC(hWnd);
	HPEN hpen2 = CreatePen(PS_DASH, 5, RGB(0, 255, 0));
	HPEN hpen_old = (HPEN)SelectObject(hdc, hpen2);
	MoveToEx(hdc, (tmp.x * 10) + x0, (tmp.y * 10) + y0, NULL);
	LineTo(hdc, (single_dot.x * 10) + x0, (single_dot.y * 10) + y0);
	SelectObject(hdc, hpen_old);

}
int main(int argc, char **argv) {
	long long i = 0;
	double startx = 0, starty = 0;
	double left_speed, right_speed, angle = 0;
	int Type;
	fstream readFile("in.txt", ios::in);
	string str;
	while (!readFile.eof()) {
		Sleep(3000);     //每次间隔0.5秒
		getline(readFile, str);   //获得速度,是否
								  //假装这样可以按顺序读取:)不准打人
		//cout << left_speed << ' ' << right_speed << endl;
		int indexie = 0;
		indexie = str.find(' ');
		left_speed = atof(str.substr(0, indexie).c_str());
		str.erase(0, indexie + 1);
		indexie = str.find(' ');
		right_speed = atof(str.substr(0, indexie).c_str());
		str.erase(0, indexie + 1);
		Type= atoi(str.c_str());

		if (Type == 0) {//直走
			draw(startx, starty, left_speed, right_speed, angle);
		}
		else if (Type == 1) {//转弯
			int dire;
			double speed;
			if (left_speed < threshold) { dire = 1;speed = right_speed; }
			else { dire = -1;speed = left_speed; }
			/*
			double tmp = (M_PI - speed * interval / Wheelbase * dire) / 2;
			angle = M_PI / 2 - speed * interval / Wheelbase * dire;
			draw(startx, starty, left_speed, right_speed, tmp);//写这里的时候太困了不知道是不是对的
			//好吧就是写错了，你个沙雕，想不想睡觉了？
			*/

			//重新写
			angle = (M_PI / 2 - speed * interval / Wheelbase) * dire;
			double tmp_angle;
			if (speed * interval / Wheelbase > M_PI) {
				tmp_angle = (speed * interval / Wheelbase * dire) / 2;
			}
			else tmp_angle = (M_PI - speed * interval / Wheelbase) / 2 * dire;
			draw(startx, starty, left_speed, right_speed, tmp_angle);
			//woc怎么感觉还是错的？
		}
		++i;
		//readFile.seekg(i);
		//readFile.get(left_speed);
		//	cout << "c:" << c << endl;
		//readFile.seekp(i++);
		//readFile.put('Y');
		//readFile.seekp(i++);
		//readFile.put('Y');
		//Delete the rest of the file
	}

	//我要是想返回……
	vector<node>::iterator it = route.end() - 1;
	//算偏角，确定要转过的角度
	double thet = atan2((*it).y, (*it).x);
	//算了算了，睡觉了
	return 0;
}

//abcdefg  执行后：
//abcYYfg