#pragma once
#include<windows.h>
#include<iostream>
#define ROWS 20//背景数组的行数
#define COLS 10//背景数组的列数
#define WIDTH 4//方块形状数组的列数
#define HEIGH 2//方块形状数组的行数
using namespace std;
class Block;//向前引用
class Render
{//把Render类声明为Block类的友元类之后，Render类的所有成员函数都是Block类的友元函数，使用时记得要带上一个Block&(Block类的引用)
private:
	int color;//颜色色号
	int hposition;
	int zposition;
	char g_bg[ROWS][COLS];//背景数组
public:
	Render(int h, int z, int C = -1);
	void paint(HDC hdc, Block& b);
	void paintsquare_1(HDC hdc);
	void paintsquare_2(HDC hdc);
	void paintsquare_3(HDC hdc);
	void copyblock(Block& b);
	void copybg(Block& b);//拷贝Block类的假背景地图到Render类的真背景地图
	bool judgeGameover();
};
class Player;//向前引用
class Block
{
private:
	char g_square[HEIGH][WIDTH];//方块形状数组
	int line;//方块位置行数
	int list;//方块位置列数
	int shape;//方块的形状编号
	char faker_bg[ROWS][COLS];//假背景数组
public:
	friend Render;//将Render类声明为Block的友元类
	Block();//初始化
	void changelist(int n);
	void changeline(int n);
	void change1into2();
	bool judgebottom_1();//判断方块是否应该停下的第一种情况:方块是否直接落到最底行
	bool judgebottom_2();//判断方块是否应该停下的第二种情况:方块是否叠在其他方块上了
	bool judgeleft_1();
	bool judgeleft_2();
	bool judgeright_1();
	bool judgeright_2();
	int createrandomsquare();
	void squaredown();
	void squareleft();
	void squareright();
	void squarerotate();
	void normalrotate();
	void linerotate();
	bool judgenormal();
	bool judgeline();
	void deleteline(Player& p);
};
class Game
{
private:
	static char Pause;//用于判断是否暂停
	int timer;
public:
	Game(int t);
	static void changePause();
	static char getPause();
	void oncreate(Render& r, Block& b);
	void onreturn();
	void ontimer(HWND hwnd, Render& r, Block& b, Player& p);
	void onleft(Render& r, Block& b, Player& p);
	void onright(Render& r, Block& b, Player& p);//响应右移操作
	void ondown(Render& r, Block& b, Player& p);//响应加速操作
	void onrotate(Render& r, Block& b, Player& p);//响应旋转操作
	void onpaint(HDC hdc, Render& r, Block& b, Player& p);
};
class Player {
private:
	int score;
	int chance;
	static char judgevictory;
	string ID;
	char str[30];
public:
	Player(string id);
	char* getstr();
	int dispscore();
	int dispchance();
	void changeScore(int n);
	void changeChance(int n);
	void changeJ();
	char getJ();
	friend Player getWin(Player& p1, Player& p2);
};