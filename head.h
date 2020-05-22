#pragma once
class Block;//方块类的前项引用
class Game {//游戏类
public:
	int Timecount;
	bool Choose();//游戏模式选择(单人还是双人模式)
	void Begin();//开始游戏
	int Timework();//计时工作
	void Getblock();//随机获得方块
	void Getp(Block &);//通过友元关系获取方块的位置(get position)
	void End();//结束游戏
};
class Block {//方块类
private:
	int shape;//方块的形状，一共七种，分别用1到7表示
	int X, Y;//标记方块在大地图上的位置，作为方块的中心坐标(也将作为下面pos数组的中心)
	int pos[3][3];//pos数组（3*3）用来绘制方块(position)
public:
	void Initial();//初始化方块
	void DropB();//方块自然下落
	void Cmove();//控制方块运动(control move)
	void Spin();//方块的旋转(spin)
	void Tran();//方块的平移(tranlation)
	friend void Game::Getp();//Game类由此获取方块坐标
};
class Render {//渲染类
private:
	int Map[10][25];//游戏界面大地图(宽10*高25)
	int Scount[4][4];//计分板(暂定)(score count)
public:
	bool Judge();//检测地图底部是否可以消行
	void Newmap();//更新大地图
	void NewS();//更新计分板
	//以及一些能让大地图以图形界面的形式显示出来的函数(暂定)
	void gameinterface();//显示游戏界面//用户模式选择界面
};
class Player {//玩家类
private://是否换成public会好一些？
	int score;//玩家的当前积分
	int top;//当前最高行数
public:
	int Getscore();//获取玩家积分
	int GetT();//检查玩家最高行是否为地图顶行
	void Key();//玩家击键指令检测(检测玩家是否发出指令以及该指令能否执行)
};
