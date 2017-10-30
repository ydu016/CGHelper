#include "stdafx.h"
#include "Map.h"
#include <string.h>
#include <windows.h>


Map::Map() : m_nHeight(DEMOY), m_nWidht(DEMOX)
{
	memset(m_map, 0, sizeof(m_map));
}

//-----------------------------------
// ������: setPoint
// ��  ��: ��һ���㱣�浽��ͼ��, �����ڲ��������ʾ�ķ����������ú��������ʾ����
// ����ֵ: bool 
// ��  ��: int x		����
// ��  ��: int y
// ��  ��: int nValue Ҫ���õ�ֵ
//-----------------------------------
bool Map::setPoint(int x, int y, MAPELEMENT nValue) {

	m_map[y][x] = nValue;

	show(x, y);

	return true;
}

//-----------------------------------
// ������: isPass
// ��  ��: �ж�ָ�������Ƿ��ܹ�ͨ��
// ����ֵ: bool 
// ��  ��: int x
// ��  ��: int y
//-----------------------------------
bool Map::isPass(int x, int y) {

	if (x < 0 || x >= m_nWidht || y < 0 || y >= m_nHeight)
		return false;

	return m_map[y][x] == 0;
}

bool Map::mapCopy(const char* pMap, int widht, int height) {

	if (widht != m_nWidht || height != m_nHeight)
		return false;


	memcpy(m_map, pMap, widht * height);

	return true;
}


const char* ConsoleMap::getElement(MAPELEMENT element) {

	switch (element) {
	case e_none: return "  ";
		break;

	case e_wall_a: return "��";
		break;
	case e_wall_b: return "��";
		break;
	case e_wall_c: return "��";
		break;
	case e_wall_d:return "��";
		break;

	case e_tank_body: return "��";
		break;
	case e_tank_center:return "��";
		break;
	case e_tank_gun_up:	return "��";
		break;
	case e_tank_gun_down:	return "��";
		break;
	case e_tank_gun_left:	return "��";
		break;
	case e_tank_gun_right:	return "��";
		break;

	default: return "  ";
		break;
	}
}

void ConsoleMap::show(int x, int y) {

	MAPELEMENT element = (MAPELEMENT)m_map[y][x];

	int color = 0;

	// ���ݵ�ͼԪ��������Ҫ��ʾ����ɫ
	switch (element) {

	case e_wall_a:
		color = COLOR_WHITE_FOR;
		break;
	case e_wall_b:
		color = COLOR_RED_FOR;
		break;
	case e_wall_c:
		color = COLOR_YELLOW_FOR | COLOR_HIGHTLIGHT_FOR;
		break;
	case e_wall_d:
		color = COLOR_GREEN_FOR;
		break;

	case e_tank_body:
		color = COLOR_GREEN_FOR | COLOR_HIGHTLIGHT_FOR;
		break;
	case e_tank_center:
		color = COLOR_RED_FOR | COLOR_HIGHTLIGHT_FOR;
		break;

	case e_tank_gun_up:
	case e_tank_gun_down:
	case e_tank_gun_left:
	case e_tank_gun_right:
		color = COLOR_YELLOW_FOR | COLOR_HIGHTLIGHT_FOR;
		break;

	case e_other: color = COLOR_WHITE_FOR;
		break;
	}

	// ��ʾ������̨��
	show(x, y, getElement(element), color);

}

// ���ַ�����ӡ������̨��ָ�����괦
void ConsoleMap::show(int x, int y, const char* pStr, int color) {

	//HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	//COORD  pos = { x * 2 , y };
	//SetConsoleCursorPosition(hStdOut, pos);
	//SetConsoleTextAttribute(hStdOut, color);
	//printf(pStr);
}
