/**
* A星算法
* 主要目的:
*	在起点和终点两个坐标点之间找到一条可以连续通行的路径.
*
* 主要逻辑:
*	1. 判断是否到达终点
*		1.1 如果是终点,则从close表中找到连接终点和起点的所有连续的点(逆序查找).
*	2. 判断一个点是否是有效的点
*		2.1 不是障碍点
*		2.2 不在open表中
*		2.3 不在close表中
*	3. 为地图上的点初始化其属性值.
*	4. 为一个坐标点生成其周围的点.
*
*  下方有3个类(结构体)
*	Node类 用于表示地图上一点的信息(如点的坐标,点的H,G,F值等)
*
*	Point结构体 用于表示一个点,这个点有当前点的信息(用Node表示), 有到
*	达该点的上一个点的信息(用Node表示).
*
*	AStart类 用于实现A*算法查找路径的逻辑, 该的主要逻辑由getPath实现.
*	其余函数旨在配合getPath函数来获取额外的判断信息.
**
***/


#pragma once
#include <math.h>
#include <vector>
using std::vector;


#include "Map.h"
// 地图对象. 用于判断一个点在地图上是否是障碍物.
//extern ConsoleMap g_gameMap;


class Node {

public:
	int f;	// F值 , f = g + h
	int	x;	// x坐标
	int y;	// y坐标

	int g;	// G值 , 移动损耗,每走一步增加1, 处于起点时g为0
	int h;	// H值 , 当前点和终点的直线距离, 一般是当前点的x,y坐标和终点的x,y坐标相加之和

	int d;	// 方向: 0(up),1(down),2(left),3(right)


public:
	// 构造函数
	Node() :x(), y(), f(), g(), h(), d() {
	}

	Node(int x, int y, int g, int d)
		:x(x), y(y), d(d), g(g), f(0) {
	}

	// 设置当前点到终点的距离H值, 也顺便设置f值
	inline void setH(int nEndX, int nEndY) {
		h = abs(x - nEndX) + abs(y - nEndY);
		f = g + h;
	}

	// 重载==运算符,用于比较两个点是否相等
	friend bool operator==(const Node& rLeft, const Node& rRight) {
		return (rLeft.x == rRight.x) && (rLeft.y == rRight.y);
	}
};

typedef struct Point {

	Node current; // 当前点
	Node previous;// 上一个点
}Point;



class AStart {
	ConsoleMap		g_gameMap;

	vector<Point>	m_vecOpen; // open表 , 保存地图上那些没有走过的点
	vector<Point>	m_vecClose;// close表, 保存已经走过的点.

	Node			m_startNode; // 起点
	Node			m_endNode; // 终点

public:
	// 设置开始坐标
	void setStartPos(int x, int y);

	// 设置结束坐标
	void setEndPos(int x, int y);

	// 设置初始方向
	void setStartDirection(int direction);

	ConsoleMap* getMap();

	// 获取路径, 并将获取到的路径保存到vecPath中
	bool getPath(vector<Node>& vecPath);

	// 获取路径, 并将获取到的路径保存到vecPath中
	bool getPath(int direction,
		int nStartX,
		int nStartY,
		int nEndX,
		int nEndY,
		vector<Node>& vecPath);


protected:

	// 在close表中搜索出一条连接终点和起点的路径
	bool searchPath(Point endNode, vector<Node>& vecPath);

	// 判断一个点是否是终点
	bool isEndPos(const Node& node);

	// 判断一个点是否是有效的点
	bool isValidNode(const Node& node);

	// 在中心点周围创建出4个点.
	void createNode(const Node& center,
		Point* pArray);

	// 从open表中找到一个最小值得节点
	int getOpenMinNode();
};