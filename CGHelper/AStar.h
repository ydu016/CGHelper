/**
* A���㷨
* ��ҪĿ��:
*	�������յ����������֮���ҵ�һ����������ͨ�е�·��.
*
* ��Ҫ�߼�:
*	1. �ж��Ƿ񵽴��յ�
*		1.1 ������յ�,���close�����ҵ������յ���������������ĵ�(�������).
*	2. �ж�һ�����Ƿ�����Ч�ĵ�
*		2.1 �����ϰ���
*		2.2 ����open����
*		2.3 ����close����
*	3. Ϊ��ͼ�ϵĵ��ʼ��������ֵ.
*	4. Ϊһ���������������Χ�ĵ�.
*
*  �·���3����(�ṹ��)
*	Node�� ���ڱ�ʾ��ͼ��һ�����Ϣ(��������,���H,G,Fֵ��)
*
*	Point�ṹ�� ���ڱ�ʾһ����,������е�ǰ�����Ϣ(��Node��ʾ), �е�
*	��õ����һ�������Ϣ(��Node��ʾ).
*
*	AStart�� ����ʵ��A*�㷨����·�����߼�, �õ���Ҫ�߼���getPathʵ��.
*	���ຯ��ּ�����getPath��������ȡ������ж���Ϣ.
**
***/


#pragma once
#include <math.h>
#include <vector>
using std::vector;


#include "Map.h"
// ��ͼ����. �����ж�һ�����ڵ�ͼ���Ƿ����ϰ���.
//extern ConsoleMap g_gameMap;


class Node {

public:
	int f;	// Fֵ , f = g + h
	int	x;	// x����
	int y;	// y����

	int g;	// Gֵ , �ƶ����,ÿ��һ������1, �������ʱgΪ0
	int h;	// Hֵ , ��ǰ����յ��ֱ�߾���, һ���ǵ�ǰ���x,y������յ��x,y�������֮��

	int d;	// ����: 0(up),1(down),2(left),3(right)


public:
	// ���캯��
	Node() :x(), y(), f(), g(), h(), d() {
	}

	Node(int x, int y, int g, int d)
		:x(x), y(y), d(d), g(g), f(0) {
	}

	// ���õ�ǰ�㵽�յ�ľ���Hֵ, Ҳ˳������fֵ
	inline void setH(int nEndX, int nEndY) {
		h = abs(x - nEndX) + abs(y - nEndY);
		f = g + h;
	}

	// ����==�����,���ڱȽ��������Ƿ����
	friend bool operator==(const Node& rLeft, const Node& rRight) {
		return (rLeft.x == rRight.x) && (rLeft.y == rRight.y);
	}
};

typedef struct Point {

	Node current; // ��ǰ��
	Node previous;// ��һ����
}Point;



class AStart {
	ConsoleMap		g_gameMap;

	vector<Point>	m_vecOpen; // open�� , �����ͼ����Щû���߹��ĵ�
	vector<Point>	m_vecClose;// close��, �����Ѿ��߹��ĵ�.

	Node			m_startNode; // ���
	Node			m_endNode; // �յ�

public:
	// ���ÿ�ʼ����
	void setStartPos(int x, int y);

	// ���ý�������
	void setEndPos(int x, int y);

	// ���ó�ʼ����
	void setStartDirection(int direction);

	ConsoleMap* getMap();

	// ��ȡ·��, ������ȡ����·�����浽vecPath��
	bool getPath(vector<Node>& vecPath);

	// ��ȡ·��, ������ȡ����·�����浽vecPath��
	bool getPath(int direction,
		int nStartX,
		int nStartY,
		int nEndX,
		int nEndY,
		vector<Node>& vecPath);


protected:

	// ��close����������һ�������յ������·��
	bool searchPath(Point endNode, vector<Node>& vecPath);

	// �ж�һ�����Ƿ����յ�
	bool isEndPos(const Node& node);

	// �ж�һ�����Ƿ�����Ч�ĵ�
	bool isValidNode(const Node& node);

	// �����ĵ���Χ������4����.
	void createNode(const Node& center,
		Point* pArray);

	// ��open�����ҵ�һ����Сֵ�ýڵ�
	int getOpenMinNode();
};