#pragma once

#define DEMOX 0x32
#define DEMOY 0x32

// 地图元素的类型枚举常量
enum MAPELEMENT {
	e_none = 0, // 空白

	e_wall = 1,
	e_wall_a,   // 墙a
	e_wall_b,	 // 墙b
	e_wall_c,	 // 墙c
	e_wall_d,	 // 墙d

	e_tank = 100, // 坦克
	e_tank_body,	// 坦克外壳
	e_tank_center,	// 坦克中心
	e_tank_gun_up,	// 坦克炮管
	e_tank_gun_down,
	e_tank_gun_left,
	e_tank_gun_right,


	e_bullset = 150, // 炮弹
	e_bullset_a,      // 炮弹a
	e_bullset_b,      // 炮弹b
	e_bullset_c,      // 炮弹c

	e_other = 200, // 其它东西

				   // 最大值
				   e_max = 256
};



// 抽象地图类
class Map {

protected:
	unsigned char m_map[DEMOX][DEMOY];
	int  m_nWidht;
	int  m_nHeight;
public:
	Map();

	// 将一个点设置到地图的指定坐标上
	bool setPoint(int x, int y, MAPELEMENT nValue);

	// 判断指定的点上是否能够通行
	bool isPass(int x, int y);

	// 地图拷贝
	bool mapCopy(const char* pMap, int widht, int height);

	// 纯虚函数, 一个显示接口.
	virtual void show(int x, int y) = 0;
};



#define		COLOR_BLUE_FOR			0x0001	//蓝色 前
#define		COLOR_GREEN_FOR			0x0002	//绿色 前
#define		COLOR_BLACK_FOR			0x0000	//黑色 前
#define		COLOR_RED_FOR			0x0004	//红色 前
#define		COLOR_PURPLE_FOR		0x0005	//紫色 前
#define		COLOR_YELLOW_FOR		0x0006	//黄色 前
#define		COLOR_WHITE_FOR			0x0007	//白色 前
#define		COLOR_HIGHTLIGHT_FOR	0x0008  //高亮 前

// 控制台版的地图
class ConsoleMap : public Map
{

public:
	// 实现父类的纯虚函数
	virtual void show(int x, int y);

	const char*  getElement(MAPELEMENT element);
	void show(int x, int y, const char* pStr, int color);

};



