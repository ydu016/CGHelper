#pragma once

#define DEMOX 0x32
#define DEMOY 0x32

// ��ͼԪ�ص�����ö�ٳ���
enum MAPELEMENT {
	e_none = 0, // �հ�

	e_wall = 1,
	e_wall_a,   // ǽa
	e_wall_b,	 // ǽb
	e_wall_c,	 // ǽc
	e_wall_d,	 // ǽd

	e_tank = 100, // ̹��
	e_tank_body,	// ̹�����
	e_tank_center,	// ̹������
	e_tank_gun_up,	// ̹���ڹ�
	e_tank_gun_down,
	e_tank_gun_left,
	e_tank_gun_right,


	e_bullset = 150, // �ڵ�
	e_bullset_a,      // �ڵ�a
	e_bullset_b,      // �ڵ�b
	e_bullset_c,      // �ڵ�c

	e_other = 200, // ��������

				   // ���ֵ
				   e_max = 256
};



// �����ͼ��
class Map {

protected:
	unsigned char m_map[DEMOX][DEMOY];
	int  m_nWidht;
	int  m_nHeight;
public:
	Map();

	// ��һ�������õ���ͼ��ָ��������
	bool setPoint(int x, int y, MAPELEMENT nValue);

	// �ж�ָ���ĵ����Ƿ��ܹ�ͨ��
	bool isPass(int x, int y);

	// ��ͼ����
	bool mapCopy(const char* pMap, int widht, int height);

	// ���麯��, һ����ʾ�ӿ�.
	virtual void show(int x, int y) = 0;
};



#define		COLOR_BLUE_FOR			0x0001	//��ɫ ǰ
#define		COLOR_GREEN_FOR			0x0002	//��ɫ ǰ
#define		COLOR_BLACK_FOR			0x0000	//��ɫ ǰ
#define		COLOR_RED_FOR			0x0004	//��ɫ ǰ
#define		COLOR_PURPLE_FOR		0x0005	//��ɫ ǰ
#define		COLOR_YELLOW_FOR		0x0006	//��ɫ ǰ
#define		COLOR_WHITE_FOR			0x0007	//��ɫ ǰ
#define		COLOR_HIGHTLIGHT_FOR	0x0008  //���� ǰ

// ����̨��ĵ�ͼ
class ConsoleMap : public Map
{

public:
	// ʵ�ָ���Ĵ��麯��
	virtual void show(int x, int y);

	const char*  getElement(MAPELEMENT element);
	void show(int x, int y, const char* pStr, int color);

};



