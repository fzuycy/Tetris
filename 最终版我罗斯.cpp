#include<windows.h>
#include<iostream>
#include<string>
#include<time.h>
#define ROWS 20//�������������
#define COLS 10//�������������
#define WIDTH 4//������״���������
#define HEIGH 2//������״���������
#define TIMER1 1234
#define SPEED 1000
#define TSUB 200//ʱ�������
#define SSUB -3//�����ۻ�����е���һ��ֻᱻ�۳�SSUB
using namespace std;
int Base1 = 2;//���ٱ�׼������Base���ܻ��һ�ε������������ٶȵĻ��ᣬBase�ĳ�ʼֵ��2
int Base2 = 2;
int Begin = 0;
int Over = 0;
int NowSpeed = SPEED;
HINSTANCE g_hinstance;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HWND hwnd;

class Block;//��ǰ����
class Render
{//��Render������ΪBlock�����Ԫ��֮��Render������г�Ա��������Block�����Ԫ������ʹ��ʱ�ǵ�Ҫ����һ��Block&(Block�������)
private:
	int color;//��ɫɫ��
	int hposition;
	int zposition;
	char g_bg[ROWS][COLS];//��������
public:
	Render(int h, int z,int C=-1) { hposition = h, zposition = z,color=C; }
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
	void changelist(int n) { list += n; }//���ڸı�list��nΪ��Ҫ�ı����
	void changeline(int n) { line += n; }//���ڸı�line��nΪ��Ҫ�ı����
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
	Game(int t) { timer = t; }
	static void changePause() { Pause = !Pause; }
	static char getPause() { return Pause; }
	void oncreate(Render& r, Block& b);
	void onreturn();
	void ontimer(HWND hwnd, Render& r, Block& b, Player& p);
	void onleft(Render& r, Block& b, Player& p);
	void onright(Render& r, Block& b, Player& p);//��Ӧ���Ʋ���
	void ondown(Render& r, Block& b, Player& p);//��Ӧ���ٲ���
	void onrotate(Render& r, Block& b, Player& p);//��Ӧ��ת����
	void onpaint(HDC hdc, Render& r, Block& b, Player& p);
};
char Game::Pause = 0;
class Player {
private:
	int score;
	int chance;
	static char judgevictory;
	string ID;
	char str[30];
public:
	Player(string id) 
	{ 
		ID = id, score = 0,chance=0; 
	}
	char* getstr();
	int dispscore() { return score; }
	int dispchance() { return chance; }
	void changeScore(int n) { score+=n,chance=score/2; }//�Ի��ֽ����ۼ�
	void changeChance(int n) { chance += n; }
	void changeJ() { judgevictory = !judgevictory; }
	char getJ() { return judgevictory; }
	friend Player getWin(Player& p1, Player& p2);
};
char Player::judgevictory = 1;
Player getWin(Player& p1, Player& p2)
{
	if (p1.dispscore() > p2.dispscore())return p1;
	else if (p1.dispscore() == p2.dispscore())p1.changeJ();
	return p2;
}
Block b1, b2;
Render r1(0, 0), r2(500, 0);
Game g1(1), g2(2);
Player p1("P1"), p2("P2");
int main()
{
	HINSTANCE hinstance = GetModuleHandle(NULL);
	g_hinstance = hinstance;
	TCHAR szAppName[] = TEXT("Tetris");
	MSG msg;
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//wc.hbrBackground = CreateSolidBrush(RGB(255, 0, 0));
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szAppName;
	if (!RegisterClass(&wc))
	{
		cout << "RegisterClass occurs errors!";
		return 0;
	}
	hwnd = CreateWindow(szAppName,TEXT("els"),WS_OVERLAPPEDWINDOW,100, 50,1000, 650,NULL, NULL,hinstance,NULL);
	ShowWindow(hwnd, SW_SHOWNORMAL);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
Block::Block()//��ʼ��
{
	line = -1; list = -1; shape = -1;
	for (int i = 0; i < HEIGH; i++)
	{
		for (int j = 0; j < WIDTH; j++)
			g_square[i][j] = 0;
	}
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
			faker_bg[i][j] = 0;
	}
}
void Block::change1into2()//ֱ�ӶԼٱ�������faker_bg���в�����֮��������Render��ĳ�Ա����copy_bg()ֱ�Ӱѷ�������ļٱ������鸴�Ƶ�Render����汳��������
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (faker_bg[i][j] == 1)
				faker_bg[i][j] = 2;
		}
	}
}
bool Block::judgebottom_1()//ֱ�Ӽ��ٱ������飬�Ϸ��㣬�����ٱ���������汳������һģһ��
{
	for (int i = 0; i < COLS; i++)
	{
		if (faker_bg[ROWS - 1][i] == 1)
		{
			return 0;
		}
	}
	return 1;
}
bool Block::judgebottom_2()//Ҳ��ֱ�Ӽ��ٱ������飬�Ϸ��㣬�����ٱ���������汳������һģһ��
{
	for (int i = ROWS-1; i >= 1; i--)
	{
		for (int j = 0;j < COLS; j++)
		{
			if (faker_bg[i-1][j] == 1)
			{
				if (faker_bg[i][j] == 2)
				{
					return 0;
				}
			}
		}
	}
	return 1;
}
bool Block::judgeleft_1()
{
	for (int i = 0; i < ROWS; i++) {
		if (1 == faker_bg[i][0]) return 0;
	}
	return 1;
}
bool Block::judgeleft_2()
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 1; j < COLS; j++)
		{
			if (1 == faker_bg[i][j] && 2 == faker_bg[i][j - 1]) return 0;
		}
	}
	return 1;
}
bool Block::judgeright_1()
{
	for (int i = 0; i < ROWS; i++) {
		if (1 == faker_bg[i][COLS - 1]) return 0;
	}
	return 1;
}
bool Block::judgeright_2()
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = COLS-2 ; j >= 0; j--) {
			if (1 == faker_bg[i][j] && 2 == faker_bg[i][j + 1]) return 0;
		}
	}
	return 1;
}
int Block::createrandomsquare()
{
	int n = rand() % 7;
	switch (n)
	{
	case 0: {
		g_square[0][0] = 1; g_square[0][1] = 1; g_square[0][2] = 0; g_square[0][3] = 0;
		g_square[1][0] = 0; g_square[1][1] = 1; g_square[1][2] = 1; g_square[1][3] = 0;
		line = 0;
		list = 3;
		break;
	}
	case 1: {
		g_square[0][0] = 0; g_square[0][1] = 1; g_square[0][2] = 0; g_square[0][3] = 0;
		g_square[1][0] = 1; g_square[1][1] = 1; g_square[1][2] = 1; g_square[1][3] = 0;
		line = 0;
		list = 3;
		break;
	}
	case 2: {
		g_square[0][0] = 0; g_square[0][1] = 1; g_square[0][2] = 1; g_square[0][3] = 0;
		g_square[1][0] = 1; g_square[1][1] = 1; g_square[1][2] = 0; g_square[1][3] = 0;
		line = 0;
		list = 3;
		break;
	}
	case 3: {
		g_square[0][0] = 1; g_square[0][1] = 0; g_square[0][2] = 0; g_square[0][3] = 0;
		g_square[1][0] = 1; g_square[1][1] = 1; g_square[1][2] = 1; g_square[1][3] = 0;
		line = 0;
		list = 3;
		break;
	}
	case 4: {
		g_square[0][0] = 0; g_square[0][1] = 0; g_square[0][2] = 1; g_square[0][3] = 0;
		g_square[1][0] = 1; g_square[1][1] = 1; g_square[1][2] = 1; g_square[1][3] = 0;
		line = 0;
		list = 3;
		break;
	}
	case 5: {
		g_square[0][0] = 0; g_square[0][1] = 1; g_square[0][2] = 1; g_square[0][3] = 0;
		g_square[1][0] = 0; g_square[1][1] = 1; g_square[1][2] = 1; g_square[1][3] = 0;
		break;
	}
	case 6: {
		g_square[0][0] = 1; g_square[0][1] = 1; g_square[0][2] = 1; g_square[0][3] = 1;
		g_square[1][0] = 0; g_square[1][1] = 0; g_square[1][2] = 0; g_square[1][3] = 0;
		line = 0;
		list = 4;
		break;
	}
	}
	shape = n;
	return n;
}
void Block::squaredown()//�����Զ����亯��
{
	for (int i = ROWS - 2; i >= 0; i--)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (faker_bg[i][j] == 1)
			{
				faker_bg[i + 1][j] = 1;//�ȶԼٱ�����������ֵ����
				faker_bg[i][j] = 0;
			}//֮��������Render��ĳ�Ա����copy_bg()ֱ�Ӱѷ�������ļٱ������鸴�Ƶ�Render����汳��������
		}
	}
}
void Block::squareleft()
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 1; j < COLS; j++) {
			if (1 == faker_bg[i][j]) {
				faker_bg[i][j - 1] = faker_bg[i][j];
				faker_bg[i][j] = 0;
			}
		}
	}
}
void Block::squareright()
{
	for (int i = 0; i < ROWS; i++) {
		for (int j = COLS - 2; j >= 0; j--) {
			if (1 == faker_bg[i][j])
			{
				faker_bg[i][j + 1] = faker_bg[i][j];
				faker_bg[i][j] = 0;
			}
		}
	}
}
bool Block::judgenormal()//�ж��Ƿ�����ת
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (faker_bg[line + i][list + j] == 2)
			{
				return 0;
			}
		}
	}
	if (list < 0)
	{
		list = 0;
	}
	else if (list + 2 > 9)
	{
		list = 7;
	}
	return 1;
}
bool Block::judgeline()//�ж��Ƿ�����ת
{
	int i = 1, j = 1;
	for (i = 1; i < 4; i++)
	{
		if (faker_bg[line][list + i] == 2 || line + i > 9)
		{
			break;
		}
	}
	for (j = 1; j < 4; j++)
	{
		if (faker_bg[line][list - j] == 2 || list - j < 0)
		{
			break;
		}
	}
	if ((i - 1 + j - 1) < 3)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
void Block::squarerotate()
{
	if (shape >= 0 && shape <= 4)
	{
		if (judgenormal() == 1)
			normalrotate();
		else
			return;
	}
	else if (shape == 5)
	{
		return;
	}
	else
	{
		if (line != 0 && judgeline() == 1)
		{
			linerotate();
		}
	}
}
void Block::normalrotate()
{
	int num = 2;
	char temp[3][3];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			temp[i][j] = faker_bg[line + i][list + j];
		}
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			faker_bg[line + i][list + j] = temp[2 - j][i];
		}
	}
}
void Block::linerotate()
{
	if (faker_bg[line][list - 1] == 1)//�����
	{
		faker_bg[line][list - 1] = 0;
		faker_bg[line][list + 1] = 0;
		faker_bg[line][list + 2] = 0;
		if (faker_bg[line + 1][list] == 2)
		{
			faker_bg[line - 1][list] = 1;
			faker_bg[line - 2][list] = 1;
			faker_bg[line - 3][list] = 1;
		}
		else if (faker_bg[line + 2][list] == 2)
		{
			faker_bg[line + 1][list] = 1;
			faker_bg[line - 1][list] = 1;
			faker_bg[line - 2][list] = 1;
		}
		else
		{
			faker_bg[line - 1][list] = 1;
			faker_bg[line + 1][list] = 1;
			faker_bg[line + 2][list] = 1;
		}
	}
	else//�����
	{
		faker_bg[line - 1][list] = 0;
		faker_bg[line + 1][list] = 0;
		faker_bg[line + 2][list] = 0;
		if (faker_bg[line][list + 1] == 2 || list == 9)
		{
			faker_bg[line][list - 1] = 1;
			faker_bg[line][list - 2] = 1;
			faker_bg[line][list - 3] = 1;
			list = list - 2;
		}
		else if (faker_bg[line][list + 2] == 2 || list == 8)
		{
			faker_bg[line][list + 1] = 1;
			faker_bg[line][list - 1] = 1;
			faker_bg[line][list - 2] = 1;
			list = list - 2;
		}
		else if (faker_bg[line][list - 1] == 2 || list == 0)
		{
			faker_bg[line][list + 1] = 1;
			faker_bg[line][list + 2] = 1;
			faker_bg[line][list + 3] = 1;
			list = list + 1;
		}
		else
		{
			faker_bg[line][list - 1] = 1;
			faker_bg[line][list + 1] = 1;
			faker_bg[line][list + 2] = 1;
		}

	}
}
void Block::deleteline(Player& p)
{
	int sum = 0;
	for (int i = ROWS - 1; i >= 0; i--)
	{
		for (int j = 0; j < COLS; j++)
		{
			sum = sum + faker_bg[i][j];
		}
		if (sum == 20)
		{
			for (int k = i - 1; k >= 0; k--)
			{
				for (int q = 0; q < 10; q++)
				{
					faker_bg[k + 1][q] = faker_bg[k][q];

				}
			}
			p.changeScore(1);//score++;
			i = 20;
		}
		sum = 0;
	}
}
void Render::paint(HDC hdc, Block& b)
{
	copyblock(b);
	paintsquare_1(hdc);
	paintsquare_2(hdc);
}
void Render::paintsquare_1(HDC hdc)
{
	Rectangle(hdc, hposition, zposition, hposition + 300, zposition + +600);
	int R = 0, G = 0, B = 0;
	switch (color)
	{
	case 0: R = 238, G = 248, B = 173; break;//����
	case 1: R = 149; G = 240; B = 172; break;//��
	case 2: R = 245; G = 171; B = 158; break;//ǳ��
	case 3: R = 125; G = 190; B = 255; break;//����
	case 4: R = 250; G = 224; B = 165; break;//����
	case 5: R = 255; G = 159; B = 207; break;//����
	case 6: R = 208; G = 162; B = 255; break;//����
	}
	HBRUSH hNewBrush = CreateSolidBrush(RGB(R, G, B));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hNewBrush);
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{

			if (g_bg[i][j] == 1)
			{
				Rectangle(hdc, hposition + j * 30 + 1, i * 30 + 1, hposition + j * 30 + 30 - 1, i * 30 + 30 - 1);
			}
		}
	}
	hNewBrush = (HBRUSH)SelectObject(hdc, hOldBrush);
	DeleteObject(hNewBrush);
}
void Render::paintsquare_2(HDC hdc)
{
	HBRUSH hOldBrush;
	HBRUSH hNewBrush = CreateSolidBrush(RGB(99, 80, 205));
	hOldBrush = (HBRUSH)SelectObject(hdc, hNewBrush);
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{

			if (g_bg[i][j] == 2)
			{
				Rectangle(hdc, hposition + j * 30 + 1, i * 30 + 1, hposition + j * 30 + 30 - 1, i * 30 + 30 - 1);
			}
		}
	}
	hNewBrush = (HBRUSH)SelectObject(hdc, hOldBrush);
	DeleteObject(hNewBrush);
}
void Render::paintsquare_3(HDC hdc)
{
	Rectangle(hdc, hposition + 340, zposition + 250, hposition + 450, zposition + 350);
}
void Render::copyblock(Block& b)
{
	for (int i = 0; i < HEIGH; i++)
	{
		if (b.shape == 6) {
			if(i==1) continue;
			else 
				for (int i = 0; i < WIDTH; i++)
			{
				b.faker_bg[0][i + 3] = b.g_square[0][i];
				g_bg[0][i + 3] = b.g_square[0][i];
			}
		}
		else
		{
			if ((g_bg[1][3] == 2 && b.g_square[1][0] == 1)|| (g_bg[1][4] == 2 && b.g_square[1][1] == 1) || (g_bg[1][5] == 2 && b.g_square[1][2] == 1) || (g_bg[1][6] == 2 && b.g_square[1][3] == 1))
			{
				for (int i = 0; i < WIDTH; i++)
				{
					b.faker_bg[0][i + 3] = b.g_square[1][i];
					g_bg[0][i + 3] = b.g_square[1][i];
				}
				break;
			}
			else
			{
				for (int j = 0; j < WIDTH; j++)
				{//�ѳ�ʼ����ͬʱ������ٱ�����
					b.faker_bg[i][j + 3] = b.g_square[i][j];
					g_bg[i][j + 3] = b.g_square[i][j];
				}
			}
		}
	}
	color = b.shape;//��������и�ֵ������������ɫ�仯�űȽ�����
}
void Render::copybg(Block& b) //�Ѽٱ������Ƶ��汳����
{
	for (int i = ROWS - 1; i >= 0; i--)//�����汳������
	{
		for (int j = 0; j < COLS; j++)
		{
			g_bg[i][j] = b.faker_bg[i][j];//�Ѽٱ�������faker_bg��ֵ�����汳������g_bg
		}
	}
}
bool Render::judgeGameover()
{
	for (int i = 0; i < COLS; i++)
	{
		if (g_bg[0][i] == 2) {
			return true;
		}
	}
	return false;
}
void Game::onpaint(HDC hdc, Render& r, Block& b, Player& p)//˫�����ͼ
{
	int h = 0, z = 0;
	char nscore_1[10], nscore_2[10];
	HDC hMemDC = CreateCompatibleDC(hdc);//����������DC(һ��ֽ��ID)
	HBITMAP hBitmapBack = CreateCompatibleBitmap(hdc, 1000, 650);////����һ��ֽ������������ָ���Ǻ�̨��ֽ�Ŀ�͸�
	SelectObject(hMemDC, hBitmapBack);//��������
	r.paintsquare_1(hMemDC);//��ʾ����
	r.paintsquare_2(hMemDC);
	if (timer == 1) BitBlt(hdc, 0, 0, 300, 600, hMemDC, 0, 0, SRCCOPY);//���ݣ�����ֽ̨�ϻ��ƵĻ�ֱ��һ�δ�����Ļ��
	else BitBlt(hdc, 500, 0, 300, 600, hMemDC, 500, 0, SRCCOPY);
	r.paintsquare_3(hdc);
	int s1= p.dispscore();
	int s2 = p.dispchance();
	_itoa_s(s1, nscore_1, 10);
	_itoa_s(s2, nscore_2, 10);
	int count = 0,temp=s1;
	do{
		temp /= 10;
		count++;
	} while (temp);
	nscore_1[count] = '\0';
	count = 0;
	do {
		temp /= 10;
		count++;
	} while (temp);
	nscore_2[count] = '\0';
	if (timer == 1)//��ߴ���
	{
		TextOut(hdc, 360, 270, "PLAYER 1", 8);
		TextOut(hdc, 340, 70, "W ��ת", 6);
		TextOut(hdc, 340, 100, "A ����", 6);
		TextOut(hdc, 340, 160, "D ����", 6);
		TextOut(hdc, 340, 130, "S ����", 6);
		TextOut(hdc, 340, 190, "G ������Ϸ�����ٶ�", 18);
		TextOut(hdc, 340, 220, "�ո� Pause", 10);
		switch (s1)
		{
		case 0:case 1: {TextOut(hdc, 360, 300, "Ӣ�»�ͭ", 8); break; }
		case 2: case 3: {TextOut(hdc, 360, 300, "��������", 8); break; }
		case 4: case 5: {TextOut(hdc, 360, 300, "��ҫ�ƽ�", 8); break; }
		case 6: case 7: {TextOut(hdc, 360, 300, "���󲬽�", 8); break; }
		case 8: case 9: {TextOut(hdc, 360, 300, "����ʯ", 8); break; }
		case 11: case 12:case 13:case 14: {TextOut(hdc, 360, 300, "������ʦ", 8); break; }
		case 15: case 16:case 17:case 18:case 19: {TextOut(hdc, 360, 300, "��ǿ����", 8); break; }
		default: {TextOut(hdc, 360, 300, "��Զ����", 8); break; }
		}
		TextOut(hdc, 360, 330, "Grade: ", 7);
		TextOut(hdc, 415, 330, nscore_1, strlen(nscore_1));
		TextOut(hdc, 340, 360, "G�����ô���: ", 13);
		TextOut(hdc, 440, 360, nscore_2, strlen(nscore_2));
	}
	else
	{
		TextOut(hdc, 858, 270, "PLAYER 2", 8);
		TextOut(hdc, 840, 70, "�� ��ת", 7);
		TextOut(hdc, 840, 100, "�� ����", 7);
		TextOut(hdc, 840, 160, "�� ����", 7);
		TextOut(hdc, 840, 130, "�� ����", 7);
		TextOut(hdc, 840, 190, "L ������Ϸ�����ٶ�", 18);
		TextOut(hdc, 840, 220, "�ո� Pause", 10);
		switch (s1)
		{
		case 0:case 1:{TextOut(hdc, 860, 300, "Ӣ�»�ͭ", 8); break; }
		case 2: case 3:{TextOut(hdc, 860, 300, "��������", 8); break; }
		case 4: case 5:{TextOut(hdc, 860, 300, "��ҫ�ƽ�", 8); break; }
		case 6: case 7:{TextOut(hdc, 860, 300, "���󲬽�", 8); break; }
		case 8: case 9:{TextOut(hdc, 860, 300, "����ʯ", 8); break; }
		case 11: case 12:case 13:case 14: {TextOut(hdc, 860, 300, "������ʦ", 8); break; }
		case 15: case 16:case 17:case 18:case 19: {TextOut(hdc, 860, 300, "��ǿ����", 8); break; }
		default: {TextOut(hdc, 860, 300, "��Զ����", 8); break; }
		}
		TextOut(hdc, 860, 330, "Grade: ", 7);
		TextOut(hdc, 915, 330, nscore_1, strlen(nscore_1));
		TextOut(hdc, 840, 360, "L�����ô���: ", 13);
		TextOut(hdc, 940, 360, nscore_2, strlen(nscore_2));
	}
	DeleteObject(hBitmapBack);
	DeleteDC(hMemDC);
}
void Game::oncreate(Render& r, Block& b)
{
	HDC hdc = GetDC(hwnd);
	b.createrandomsquare();
	r.copyblock(b);
}
void Game::onreturn()
{
	Begin = 1;
	SetTimer(hwnd, TIMER1, SPEED, NULL);
}
void Game::ontimer(HWND hwnd, Render& r, Block& b, Player& p)
{
	HDC hdc = GetDC(hwnd);
	if (b.judgebottom_1() == 1 && b.judgebottom_2() == 1)
	{
		b.squaredown();//��������
		b.changeline(1);
		r.copybg(b);//�Ѽٱ������Ƶ��汳����
	}
	else
	{
		b.change1into2();//�Ѽٱ����ϵ�1���2
		b.deleteline(p);
		r.copybg(b);//�Ѽٱ������Ƹ��汳��
		if (r.judgeGameover())
		{
			Over = 1;
			p.changeScore(SSUB);
			KillTimer(hwnd, TIMER1);
			onpaint(hdc, r, b, p);
			ReleaseDC(hwnd, hdc);
			p = getWin(p1, p2);
			if(!p.getJ()) MessageBox(NULL, "No Winner! Game Over!", "��ʾ", MB_OK);
			else MessageBox(NULL, p.getstr(), "��ʾ", MB_OK);
			return;
		}
		b.createrandomsquare();
		r.copyblock(b);//���²����ķ������ڼٱ������汳����
	}
	onpaint(hdc, r, b, p);
	ReleaseDC(hwnd, hdc);
}
void Game::onleft(Render& r, Block& b, Player& p)
{
	if (b.judgeleft_1() == 1 && b.judgeleft_2() == 1)
	{
		HDC hdc = GetDC(hwnd);
		b.squareleft();//��������
		r.copybg(b);
		b.changelist(-1);
		//��ʾ����
		onpaint(hdc, r, b, p);
		ReleaseDC(hwnd, hdc);
	}
}
void Game::onright(Render& r, Block& b, Player& p)
{
	if (b.judgeright_1() == 1 && b.judgeright_2() == 1)
	{
		HDC hdc = GetDC(hwnd);
		b.squareright();//��������
		r.copybg(b);
		b.changelist(1);
		//��ʾ����
		onpaint(hdc, r, b, p);
		ReleaseDC(hwnd, hdc);
	}
}
void Game::onrotate(Render& r, Block& b, Player& p)
{
	HDC hdc = GetDC(hwnd);
	b.squarerotate();
	r.copybg(b);
	onpaint(hdc, r, b, p);
	ReleaseDC(hwnd, hdc);
}
void Game::ondown(Render& r, Block& b, Player& p)
{
	ontimer(hwnd, r, b, p);
}
char* Player::getstr()
{
	const char ch[18] = " Win!  Game Over!";
	int j=0;
	for (int i = 0; i < (int)ID.length(); i++) str[i] = ID[i];
	for (int i = ID.length();ch[j]; i++) str[i] = ch[j++];
	return str;
}
//�ص�����
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT pt;
	HDC hdc;
	switch (msg)
	{
	case WM_CREATE:
	{
		srand((unsigned int)time(NULL));
		g1.oncreate(r1, b1);//��r1��b1������������g1�ĳ�Ա������ʵ��������֮����Ϣ�Ĵ���
		g2.oncreate(r2, b2);
		break;
	}
	case WM_TIMER:
	{
		g1.ontimer(hwnd, r1, b1, p1);
		g2.ontimer(hwnd, r2, b2, p2);
		break;
	}
	case WM_KEYDOWN:
	{
		switch (wParam)
		{   //ͳһ���������Ҽ������ұߴ���(����Ϊb2,g2,r2)
			//ͳһ��WSAD��������ߴ���(����Ϊb1,g1,r1)
		case VK_RETURN:
		{
			g1.onreturn();
			g2.onreturn();
			break;
		}
		case VK_UP:
		{
			if (Begin && !g1.getPause() && !Over) g2.onrotate(r2, b2, p2);
			break;
		}
		case VK_LEFT:
		{
			if (Begin && !g1.getPause()&&!Over) g2.onleft(r2, b2, p2);
			break;
		}
		case VK_RIGHT:
		{
			if (Begin && !g1.getPause() && !Over) g2.onright(r2, b2, p2);
			break;
		}
		case VK_DOWN:
		{
			if (Begin && !g1.getPause() && !Over) g2.ondown(r2, b2, p2);
			break;
		}
		case VK_SPACE:
		{
			if (Begin) Game::changePause();
			if (Game::getPause())
			{
				KillTimer(hwnd, TIMER1);
			}
			else
			{
				SetTimer(hwnd, TIMER1, NowSpeed, NULL);
			}
			break;
		}
		case 'L':
		{
			//�ﵽBase��׼���Ի��һ�λ������������ٶȣ�ÿʹ��һ�λ��ᣬBase����Ӷ�
			if (Begin && !g1.getPause() && !Over)
				if (p2.dispscore() >= Base2) {//Ҳ����˵��ÿ������������һ�μ��ٻ���
				KillTimer(hwnd, TIMER1);
				NowSpeed = SPEED - TSUB * Base2 / 2;
				SetTimer(hwnd, TIMER1, NowSpeed, NULL);
				p2.changeChance(-1);
				Base2 += 2;
			    }
			break;
		}
		case 'G':
		{
			if (Begin && !g1.getPause() && !Over)
				if (p1.dispscore() >= Base1) {//�ﵽBase��׼���Ի��һ�λ������������ٶȣ�ÿʹ��һ�λ��ᣬBase����Ӷ�
				//Ҳ����˵��ÿ������������һ�μ��ٻ���
				KillTimer(hwnd, TIMER1);//���趨ʱ��
				NowSpeed = SPEED - TSUB * Base1 / 2;
				SetTimer(hwnd, TIMER1,NowSpeed , NULL);
				p1.changeChance(-1);
				Base1 += 2;
			    }
			break;
		}
		case 'A':
		{
			if (Begin && !g1.getPause() && !Over) g1.onleft(r1, b1, p1);
			break;
		}
		case 'S':
		{
			if (Begin && !g1.getPause() && !Over) g1.ondown(r1, b1, p1);//д��ondown�����������һЩ�޸ģ��ں�������ֱ�ӵ���һ��ontimer����
			break;
		}
		case 'D':
		{
			if (Begin && !g1.getPause() && !Over) g1.onright(r1, b1, p1);
			break;
		}
		case 'W':
		{
			if (Begin && !g1.getPause() && !Over) g1.onrotate(r1, b1, p1);
			break;
		}
		}
		break;
	}
	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &pt);
		g1.onpaint(hdc, r1, b1, p1);
		g2.onpaint(hdc, r2, b2, p2);
		break;
	}
	case WM_CLOSE://�رմ���
	{
		DestroyWindow(hwnd);
		break;
	}
	case WM_DESTROY://�رմ��ڣ��ں�̨Ҳ�ر�
	{
		KillTimer(hwnd, TIMER1);
		PostQuitMessage(0);
		break;
	}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}