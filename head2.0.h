#pragma once
#include<windows.h>
#include<iostream>
#define ROWS 20//�������������
#define COLS 10//�������������
#define WIDTH 4//������״���������
#define HEIGH 2//������״���������
using namespace std;
class Block;//��ǰ����
class Render
{//��Render������ΪBlock�����Ԫ��֮��Render������г�Ա��������Block�����Ԫ������ʹ��ʱ�ǵ�Ҫ����һ��Block&(Block�������)
private:
	int color;//��ɫɫ��
	int hposition;
	int zposition;
	char g_bg[ROWS][COLS];//��������
public:
	Render(int h, int z, int C = -1);
	void paint(HDC hdc, Block& b);
	void paintsquare_1(HDC hdc);
	void paintsquare_2(HDC hdc);
	void paintsquare_3(HDC hdc);
	void copyblock(Block& b);
	void copybg(Block& b);//����Block��ļٱ�����ͼ��Render����汳����ͼ
	bool judgeGameover();
};
class Player;//��ǰ����
class Block
{
private:
	char g_square[HEIGH][WIDTH];//������״����
	int line;//����λ������
	int list;//����λ������
	int shape;//�������״���
	char faker_bg[ROWS][COLS];//�ٱ�������
public:
	friend Render;//��Render������ΪBlock����Ԫ��
	Block();//��ʼ��
	void changelist(int n);
	void changeline(int n);
	void change1into2();
	bool judgebottom_1();//�жϷ����Ƿ�Ӧ��ͣ�µĵ�һ�����:�����Ƿ�ֱ���䵽�����
	bool judgebottom_2();//�жϷ����Ƿ�Ӧ��ͣ�µĵڶ������:�����Ƿ����������������
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
	static char Pause;//�����ж��Ƿ���ͣ
	int timer;
public:
	Game(int t);
	static void changePause();
	static char getPause();
	void oncreate(Render& r, Block& b);
	void onreturn();
	void ontimer(HWND hwnd, Render& r, Block& b, Player& p);
	void onleft(Render& r, Block& b, Player& p);
	void onright(Render& r, Block& b, Player& p);//��Ӧ���Ʋ���
	void ondown(Render& r, Block& b, Player& p);//��Ӧ���ٲ���
	void onrotate(Render& r, Block& b, Player& p);//��Ӧ��ת����
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