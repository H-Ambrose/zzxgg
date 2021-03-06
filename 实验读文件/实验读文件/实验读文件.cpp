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
#include <GdiPlus.h>
#define Wheelbase 12.39    //轴距暂定15厘米
#define WheelDia 10     //车轮直径(Wheel Diameter)暂定10厘米
#define threshold 1     //阈值，转速低于阈值则认为轮子不转
#define delta  0.5      //阈值，左右转速差低于Δ则认为走直线
#define x0 900          //使处于画布中间
#define y0 500
#define interval 0.5    //每次测量的时间间隔
#define M_PI 3.14159265358979323846
#define t 2
using namespace std;
using namespace Gdiplus;
#pragma comment(lib, "Gdiplus.lib")
struct node {
	double x = 0;
	double y = 0;
	node(int _x, int _y) :x(_x), y(_y) {}
	node() { x = 0;y = 0; }
}single_dot;
vector<node>route;       //保存路径用，不知道之后返回的时候是不是有用
double startx = 0, starty = 0;
double left_speed, right_speed, angle = 0;
int save_wayofmove = 2;
double save_dis;

void gotoxy(int x, int y)
{
	COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

/*void DrawShadow(Graphics &g, GraphicsPath ButtonPath)
{
	CDC *pDC = pView->GetDC();       // 定义一个画图对象
	Gdiplus::Graphics graphics(pDC->m_hDC);             // 获取视图客户区大小
	CRect rt;       pView->GetClientRect(&rt);  // 定义一个线性渐变画刷，按红黄蓝绿的顺序四种颜色渐变
	LinearGradientBrush linGrBrush(Point(100,0),Point(100,rt.Height()/2),Color(255,255,0,0),Color(255,0,0,255));
	Color colors[] = {
		Color(255, 255, 0, 0),   // red    
		Color(255, 255, 255, 0), //yellow  
		Color(255, 0, 0, 255),   // blue   
		Color(255, 0, 255, 0)};  // green   
	REAL positions[] = {  
		0.0f,          
		0.33f,          
		0.66f,          
		1.0f};      
	linGrBrush.SetInterpolationColors(colors, positions,4);  
	// 填充指定区域矩形     
	graphics.FillRectangle(&linGrBrush,rt.Width()/2,0,80,rt.Height()/2);   
	
	
	g.SetPageUnit(UnitPixel); //设置Graphics的坐标单位为像素  

	GraphicsPath &ShadowPath = *(ButtonPath.Clone());
	//拷贝一个按钮区域路径的副本，用来生成阴影区域路径  

	// 获得阴影区域  
	Matrix ShadowMatrix;
	ShadowMatrix.Translate(ShadowSize, ShadowSize);
	// 平移，ShadowSize即阴影延伸出来的像素数，这里是向右下方移动的，可以根据实际情况修改。  
	ShadowPath.Transform(&ShadowMatrix);
	// 应用矩阵  

	Region ButtonRegion(&ButtonPath);
	//利用按钮的路径建立按钮区域  
	Region ShadowRegion(&ShadowPath);
	//利用阴影路径建立阴影的区域  

	ShadowRegion.Exclude(&ButtonRegion);
	// 区域求差，这样就得出了纯粹的阴影区域，排除了阴影区域和按钮区域重合的部分。  

	// 初始化渐变画刷  
	PathGradientBrush brush(&ShadowPath);
	brush.SetCenterColor(ShadowColor);
	// 这里利用的是路径渐变画刷  
	Color colors[] = ...{Color(0, 0, 0, 0)};
	int count = 1;
	brush.SetSurroundColors(colors, &count);
	brush.SetFocusScales(0.75f, 0.75f);
	//对渐变效果进行调整，使其更加自然。这句的实际作用是对渐变效果进行缩放。  
	//参数是横纵两个坐标轴的缩放比例。  

	g.FillRegion(&brush, &ShadowRegion);

	delete &ShadowPath;
	//别忘了删除Clone出来的副本。 
	
}*/

node get_coordinate(int way, double dis) {
	int d = 0;
	if (way == 2) {
		if (save_wayofmove == 2)
			d = dis - save_dis;
		else d = dis;
	}
	else if (way == 4) {//左转
		d = dis;
		angle += d / Wheelbase;
	}
	else if (way == 6) {//左转
		d = dis;
		angle -= d / Wheelbase;
	}
	save_wayofmove = way;
	save_dis = dis;
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

	HPEN hpen0 = CreatePen(PS_DASH, 12, RGB(255, 255, 255));///实线 | 宽度 | 颜色
	HPEN hpen_old = (HPEN)SelectObject(hdc, hpen0);///保存原来的笔刷
	MoveToEx(hdc, (single_dot.x * t) + x0, (single_dot.y * t) + y0, NULL);
	LineTo(hdc, (tmp.x * t) + x0, (tmp.y * t) + y0);
	gotoxy(0, 0);

	HPEN hpen1 = CreatePen(PS_DASH, 11, RGB(255, 230, 230));///实线 | 宽度 | 颜色
	SelectObject(hdc, hpen1);
	MoveToEx(hdc, (single_dot.x * t) + x0, (single_dot.y * t) + y0, NULL);
	LineTo(hdc, (tmp.x * t) + x0, (tmp.y * t) + y0);
	gotoxy(0, 0);

	HPEN hpen2 = CreatePen(PS_DASH, 8, RGB(255, 172, 172));///实线 | 宽度 | 颜色
	SelectObject(hdc, hpen2);
	MoveToEx(hdc, (single_dot.x * t) + x0, (single_dot.y * t) + y0, NULL);
	LineTo(hdc, (tmp.x * t) + x0, (tmp.y * t) + y0);
	gotoxy(0, 0);

	HPEN hpen3 = CreatePen(PS_DASH, 5, RGB(255, 102, 102));///实线 | 宽度 | 颜色
	SelectObject(hdc, hpen3);
	MoveToEx(hdc, (single_dot.x * t) + x0, (single_dot.y * t) + y0, NULL);
	LineTo(hdc, (tmp.x * t) + x0, (tmp.y * t) + y0);
	gotoxy(0, 0);

	//画线段
	HPEN hpen4 = CreatePen(PS_DASH, 2, RGB(230, 0, 0));
	SelectObject(hdc, hpen4);
	MoveToEx(hdc, (single_dot.x * t) + x0, (single_dot.y * t) + y0, NULL);
	LineTo(hdc, (tmp.x * t) + x0, (tmp.y * t) + y0);

	SelectObject(hdc, hpen_old);
}

void go_home_after_catching_the_princess() {
	fstream File("C://Users//朱子仪//Desktop//恶龙咆哮//返回.txt", ios::out);

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
		File << "6 " << setprecision(6) << turn_angle * Wheelbase << endl;
		return;
	}
	else {
		if (turn_angle > M_PI)
			File << "4 " << setprecision(6) << (2 * M_PI - turn_angle)*Wheelbase << endl;
		return;
	}
	File.close();

}

int main(){
	system("mode con cols=200 lines=100");//改变宽高

	double dis = 0;
	int way_of_move = 0;
	fstream File("C://Users//朱子仪//Desktop//恶龙咆哮//新建文本文档.txt", ios::in);
	string str;
	route.clear();
	single_dot.x = 0, single_dot.y = 0;
	route.push_back(single_dot);
	while (!File.eof()) {
		Sleep(3000);
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

