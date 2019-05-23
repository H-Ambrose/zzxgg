// 实验读文件.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
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
#define Wheelbase 12.39    //轴距暂定15厘米
#define WheelDia 10     //车轮直径(Wheel Diameter)暂定10厘米
#define threshold 1     //阈值，转速低于阈值则认为轮子不转
#define delta  0.5      //阈值，左右转速差低于Δ则认为走直线
#define x0 1000          //使处于画布中间
#define y0 300
#define interval 0.5    //每次测量的时间间隔
#define M_PI 3.14159265358979323846
#define t 0.1
using namespace std;
struct node {
	double x = 0;
	double y = 0;
	node(int _x, int _y) :x(_x), y(_y) {}
	node() { x = 0;y = 0; }
}single_dot;
vector<node>route;       //保存路径用，不知道之后返回的时候是不是有用
double startx = 0, starty = 0;
double left_speed, right_speed, angle = 0;
int save_wayofmove;
double save_dis;
void gotoxy(int x, int y)
{
	COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
node get_coordinate(int way, double dis) {
	int d = 0;
	if (way == 2) {
		//if (save_wayofmove == 2)
		//	d = dis - save_dis;
		//else d = dis;
		d = dis;
	}
	else if (way == 4) {//左转
		if (save_wayofmove == 4)
			d = dis - save_dis;
		else d = dis;
		angle += d / Wheelbase;
	}
	else if (way == 6) {//左转
		if (save_wayofmove == 6)
			d = dis - save_dis;
		else d = dis;
		angle -= d / Wheelbase;
	}
	startx += d * sin(angle);
	starty += d * cos(angle);
	startx *= 100;
	startx = round(startx);
	startx = (double)startx / (double)100;
	starty *= 100;
	starty = round(starty);
	starty = (double)starty / (double)100;//去掉多余的小数位
	single_dot.x = startx;
	single_dot.y = starty;
	return single_dot;
}

void draw(int way, int dis) {

	node tmp=get_coordinate(way, dis);//当前位置
	route.push_back(tmp);
	vector<node>::iterator it = route.end() - 2;
	single_dot = (*it);

	//保存移动方式
	save_wayofmove = way;
	save_dis = dis;

	//画线
	//设置画笔&画布
	HWND hWnd = GetConsoleWindow();
	HDC hdc = GetWindowDC(hWnd);
	//重新定位
	gotoxy(0, 0);

	HPEN hpen1 = CreatePen(PS_DASH, 6, RGB(255, 230, 230));///实线 | 宽度 | 颜色
	HPEN hpen_old = (HPEN)SelectObject(hdc, hpen1);///保存原来的笔刷
	MoveToEx(hdc, (single_dot.x * t) + x0, (single_dot.y * t) + y0, NULL);
	LineTo(hdc, (tmp.x * t) + x0, (tmp.y * t) + y0);

	HPEN hpen2 = CreatePen(PS_DASH, 4, RGB(255, 179, 179));///实线 | 宽度 | 颜色
	SelectObject(hdc, hpen2);
	MoveToEx(hdc, (single_dot.x * t) + x0, (single_dot.y * t) + y0, NULL);
	LineTo(hdc, (tmp.x * t) + x0, (tmp.y * t) + y0);

	//画线段
	HPEN hpen3 = CreatePen(PS_DASH, 2, RGB(255, 0, 0));
	SelectObject(hdc, hpen3);
	MoveToEx(hdc, (single_dot.x * t) + x0, (single_dot.y * t) + y0, NULL);
	LineTo(hdc, (tmp.x * t) + x0, (tmp.y * t) + y0);

	SelectObject(hdc, hpen_old);
}

void go_home_after_catching_the_princess() {
	vector<node>::iterator it = route.end() - 1;
	single_dot = (*it);
	//angle保存着当前角度
	//4是👈 6是👉
	double sub_angle = atan2(single_dot.y, single_dot.x);
	while (angle >= M_PI) {
		angle -= 2 * M_PI;
	}
	double turn_angle = M_PI + angle - sub_angle;//计算👉转的情况
	
	while (turn_angle >= 2 * M_PI){
		turn_angle -= 2 * M_PI;
	}
	while (turn_angle < 0) {
		turn_angle += 2 * M_PI;
	}//处理大小

	if (turn_angle >= 0 && turn_angle <= M_PI) {
		cout << "6 " << turn_angle * Wheelbase << endl;
		return;
	}
	else {
		if (turn_angle > M_PI)
			cout << "4 " << (2 * M_PI - turn_angle)*Wheelbase << endl;
		return;
	}
}

int main(){
	double dis = 0;
	int way_of_move = 0;
	fstream File("C://Users//朱子仪//Desktop//新建文本文档.txt", ios::in);
	string str;
	route.clear();
	single_dot.x = 0, single_dot.y = 0;
	route.push_back(single_dot);
	while (!File.eof()) {
		Sleep(50);
		getline(File, str);           //按行读入
		if (str[0] == 'x') { 
			go_home_after_catching_the_princess(); 
		}
		else {
			int indexie = 0;
			indexie = str.find(' ');
			way_of_move = atoi(str.substr(0, indexie).c_str());
			str.erase(0, indexie + 1);
			dis = atof(str.c_str());
			draw(way_of_move, dis);
		}
	}
	File.close();
	return 0;
}

/*
             |
	     |
             |
———————·——————→ x
           0 |
             |
           ↓
	      y
*/

