#pragma once
class Block;//�������ǰ������
class Game {//��Ϸ��
public:
	int Timecount;
	bool Choose();//��Ϸģʽѡ��(���˻���˫��ģʽ)
	void Begin();//��ʼ��Ϸ
	int Timework();//��ʱ����
	void Getblock();//�����÷���
	void Getp(Block &);//ͨ����Ԫ��ϵ��ȡ�����λ��(get position)
	void End();//������Ϸ
};
class Block {//������
private:
	int shape;//�������״��һ�����֣��ֱ���1��7��ʾ
	int X, Y;//��Ƿ����ڴ��ͼ�ϵ�λ�ã���Ϊ�������������(Ҳ����Ϊ����pos���������)
	int pos[3][3];//pos���飨3*3���������Ʒ���(position)
public:
	void Initial();//��ʼ������
	void DropB();//������Ȼ����
	void Cmove();//���Ʒ����˶�(control move)
	void Spin();//�������ת(spin)
	void Tran();//�����ƽ��(tranlation)
	friend void Game::Getp();//Game���ɴ˻�ȡ��������
};
class Render {//��Ⱦ��
private:
	int Map[10][25];//��Ϸ������ͼ(��10*��25)
	int Scount[4][4];//�Ʒְ�(�ݶ�)(score count)
public:
	bool Judge();//����ͼ�ײ��Ƿ��������
	void Newmap();//���´��ͼ
	void NewS();//���¼Ʒְ�
	//�Լ�һЩ���ô��ͼ��ͼ�ν������ʽ��ʾ�����ĺ���(�ݶ�)
	void gameinterface();//��ʾ��Ϸ����//�û�ģʽѡ�����
};
class Player {//�����
private://�Ƿ񻻳�public���һЩ��
	int score;//��ҵĵ�ǰ����
	int top;//��ǰ�������
public:
	int Getscore();//��ȡ��һ���
	int GetT();//������������Ƿ�Ϊ��ͼ����
	void Key();//��һ���ָ����(�������Ƿ񷢳�ָ���Լ���ָ���ܷ�ִ��)
};
