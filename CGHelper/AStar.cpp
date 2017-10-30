#include "stdafx.h"
#include "AStar.h"




//-----------------------------------
// ������: setStartPos
// ��  ��: �����������
// ����ֵ: void 
// ��  ��: int x	
// ��  ��: int y
//-----------------------------------
void AStart::setStartPos(int x, int y) {
	m_startNode.x = x;
	m_startNode.y = y;
}

//-----------------------------------
// ������: setEndPos
// ��  ��: �����յ�����
// ����ֵ: void 
// ��  ��: int x
// ��  ��: int y
//-----------------------------------
void AStart::setEndPos(int x, int y) {
	m_endNode.x = x;
	m_endNode.y = y;
}

//-----------------------------------
// ������: setStartDirection
// ��  ��: �������ĳ�ʼ����
// ����ֵ: void 
// ��  ��: int direction
//-----------------------------------
void AStart::setStartDirection(int direction) {
	m_startNode.d = direction;
}

ConsoleMap* AStart::getMap() {
	return &g_gameMap;
}

//-----------------------------------
// ������: getPath
// ��  ��: ��ȡ��㵽�յ��һ������ͨ�е�·��
// ����ֵ: bool 
// ��  ��: int direction	 ��㴦�ĳ�ʼ����
// ��  ��: int nStartX	 �������X
// ��  ��: int nStartY	 �������Y
// ��  ��: int nEndX		 �յ�����X
// ��  ��: int nEndY		 �յ�����Y
// ��  ��: vector<Node> & vecPath
//-----------------------------------
bool AStart::getPath(int direction,
	int nStartX,
	int nStartY,
	int nEndX,
	int nEndY,
	vector<Node>& vecPath) {

	// �����������
	setStartPos(nStartX, nStartY);
	// �����յ�����
	setEndPos(nEndX, nEndY);

	// ��������ʼ����
	setStartDirection(direction);

	// ��ȡ·��
	return getPath(vecPath);
}



//-----------------------------------
// ������: getPath
// ��  ��: ��ȡ��㵽�յ��һ������ͨ�е�·��,������ȡ����·�����浽vecPath��
// ����ֵ: bool 
// ��  ��: vector<Node> & vecPath
//-----------------------------------
bool AStart::getPath(vector<Node>& vecPath) {

	/*
	* ��ȡ·���ķ���:
	* 1. �������뵽open����.
	* 2. ���ϵش�open����ȡ��һ��Fֵ��С�ĵ�. ���û����һ��,���൱�ڽ�ȫ��ͼ�ĵ㶼�ж�һ��.
	* 3. ��Fֵ��С�ĵ��������ܵ�4����, �����4�����Ƿ����յ�, ����Ǿ��ܹ��ҵ�·��,���뿪ѭ��.
	* 4. ���4������û��һ�������յ�, ��4��������Ч�ĵ���뵽open����,��Ч�ĵ�ָ����:�㲻
	*    ��open���У�Ҳ����close����, Ҳ������һ������ͨ�еĵ㡣
	* 5. ���Ǹ���open����ȡ����Fֵ��С�ĵ����close����,����open����ɾ����.����һ
	*	 ��, open��ĵ�Ż�һ��һ���ؼ���, ѭ�����ܹ��˳���
	* 6. ѭ����2��
	**
	***/

	// �ж�����Ƿ����յ�
	if (isEndPos(m_startNode)) {
		return false;
	}

	// ������ű�
	m_vecOpen.clear();
	m_vecClose.clear();

	Point point = { m_startNode };
	// �������뵽open��
	m_vecOpen.push_back(point);


	Point	stcArray[4];

	//1. ѭ����open�����ҵ����еĵ�(���open����û�е�,���˳�ѭ��)
	while (!m_vecOpen.empty()) {

		//2. ��open�����ҵ�һ��Fֵ��С�ĵ�
		int  nMinIndex = getOpenMinNode();

		//3. �ӵõ��ĵ��������������Χ4����
		createNode(m_vecOpen[nMinIndex].current, stcArray);


		//=====================================================================================
		//============================ ���´������ڵ��� ==========================================
		// g_map ��һ��ȫ�ֶ���, ��main���������ļ��ж���.
#ifdef _DEBUG
		// ��ͣ,�ȴ��û�����
		//getchar( );
		// ����ǰ���ӡ����Ļ��
		g_gameMap.show(m_vecOpen[nMinIndex].current.x,
			m_vecOpen[nMinIndex].current.y,
			"��",
			COLOR_YELLOW_FOR | COLOR_HIGHTLIGHT_FOR);
#endif
		//============================ ���ϴ������ڵ��� ==========================================


		//3. ��ֵ��С�ĵ��open���뵽close��,����open��ɾ�������.
		m_vecClose.push_back(m_vecOpen[nMinIndex]);
		m_vecOpen.erase(m_vecOpen.begin() + nMinIndex);

		for (int i = 0; i < 4; ++i) {

			// 4.1 �ж���4������,�Ƿ���һ�������յ�, �����,��ֱ�ӻ�ȡ·��,Ȼ�󷵻�.
			if (isEndPos(stcArray[i].current)) {

				// �ҳ����������յ��·��
				return searchPath(stcArray[i], vecPath);
			}
			// 4.2 ��4��������Ч�ĵ����open����.
			else if (isValidNode(stcArray[i].current)) {

				g_gameMap.show(stcArray[i].current.x, stcArray[i].current.y, "��", COLOR_GREEN_FOR);

				// ����Ч�ĵ���뵽open����
				m_vecOpen.push_back(stcArray[i]);
			}
		}
	}

	return false;
}

//-----------------------------------
// ������: isEndPos
// ��  ��: �ж�һ���ڵ��Ƿ����յ�
// ����ֵ: bool 
// ��  ��: const Node & node Ҫ�жϵĵ�
//-----------------------------------
bool AStart::isEndPos(const Node& node) {

	return m_endNode == node;
}

//-----------------------------------
// ������: isValidNode
// ��  ��: �ж�һ�����Ƿ���Ч�ĵ�
// ����ֵ: bool 
// ��  ��: const Node & node
//-----------------------------------
bool AStart::isValidNode(const Node& node) {

	// 1. ����open��,�жϵ��Ƿ���open����
	for (unsigned int i = 0; i < m_vecOpen.size(); ++i) {
		if (node == m_vecOpen[i].current)
			return false;
	}

	// 2. ����close��,�жϵ��Ƿ���close����
	for (unsigned int i = 0; i < m_vecClose.size(); ++i) {
		if (node == m_vecClose[i].current)
			return false;
	}

	// 3. �жϵ��ڵ�ͼ���Ƿ���һ����Ч�ĵ�(�õ㲻���ϰ��������Ч�ĵ�)
	if (g_gameMap.isPass(node.x, node.y) == false)
		return false;

	return true;
}

//-----------------------------------
// ������: createNode
// ��  ��: ��һ�����ĵ���������Χ��4����
// ����ֵ: void 
// ��  ��: const Node & center ���ĵ�
// ��  ��: Node * pArray		  ����,���ڱ������ɳ���4����
//-----------------------------------
void AStart::createNode(const Node& center, Point* pArray) {

	/*
	*
	* 		      [up:0]
	*		        ��
	* [left:2] �� [center] �� [right:3]
	*	            ��
	*	         [down:1]
	**/
	//>>>>>>>>>>>>>>>> up,down,left,right
	int nStepX[4] = { 0 , 0 , -1 , 1 };
	int nStepY[4] = { -1 , 1 , 0 , 0 };

	for (int i = 0; i < 4; ++i)
	{
		// �����ĵ�����Ϊ�µ����һ����
		pArray[i].previous = center;

		// �����µ㵱ǰ�������
		// ���÷���
		pArray[i].current.d = i;

		// �����ƶ����,ÿ�ƶ�һ��,��ļ�1
		pArray[i].current.g = center.g + 1;

		// ����x,y����
		pArray[i].current.x = center.x + nStepX[i];
		pArray[i].current.y = center.y + nStepY[i];

		// ���õ�ǰ�㵽�յ�ľ���: Hֵ
		pArray[i].current.setH(m_endNode.x, m_endNode.y);
	}

}

//-----------------------------------
// ������: getOpenMinNode
// ��  ��: ��ȡopen����Fֵ��С��Ԫ��
// ����ֵ: vector<Node>::iterator 
//-----------------------------------
int AStart::getOpenMinNode() {

	// ����vector,�ҵ�һ��Fֵ��С��Ԫ��
	int minIndex = 0;

	int F = m_vecOpen.begin()->current.f;

	for (unsigned int i = 0; i < m_vecOpen.size(); ++i) {

		if (F >= m_vecOpen[i].current.f) {

			// ������Сֵ
			F = m_vecOpen[i].current.f;

			// ��¼��Сֵ���±�
			minIndex = i;
		}
	}

	return minIndex;
}

//-----------------------------------
// ������: findPath
// ��  ��: ��close���в���·��
// ����ֵ: bool 
// ��  ��: const Node & endNode
// ��  ��: vector<Node> & vecPath
//-----------------------------------
bool AStart::searchPath(Point endNode, vector<Node>& vecPath) {

	/**
	* ��close���в���һ��ͨ������������·��.
	* ��������:
	*  1. ��close����, ÿ���㶼��¼����ǰһ����������.
	*  2. ��֪���һ������endNode , ����㱣������һ����.
	*  3. ͨ��endNode�����ҵ�������һ����, ͨ��������һ��������ҵ����ϸ���.
	*  4. ��������,�ܹ��ҵ����.
	*/

	// ���յ���뵽����·����vector��.

	// ��close����ѭ��������һ����
	for (unsigned int i = 0; i < m_vecClose.size(); ++i) {

		// ���close���е�һ����ĵ�ǰ����յ����һ����һ��, ˵��
		// �õ�����յ��ǰһ����.
		if (m_vecClose[i].current == endNode.previous) {


			//=====================================================================================
			//============================ ���´������ڵ��� ==========================================
			// g_map ��һ��ȫ�ֶ���, ��main���������ļ��ж���.
#ifdef _DEBUG
			// �ȴ��û�����
			//getchar( );

			// ���ݵ�ǰ��ķ�������Ļ�ϻ�����ͷ.
			switch (m_vecClose[i].current.d) {
			case 0: g_gameMap.show(m_vecClose[i].current.x, m_vecClose[i].current.y, "��", 10);
				break;
			case 1: g_gameMap.show(m_vecClose[i].current.x, m_vecClose[i].current.y, "��", 10);
				break;
			case 2: g_gameMap.show(m_vecClose[i].current.x, m_vecClose[i].current.y, "��", 10);
				break;
			case 3: g_gameMap.show(m_vecClose[i].current.x, m_vecClose[i].current.y, "��", 10);
				break;
			}
#endif
			//============================ ���ϴ������ڵ��� ==========================================
			//=====================================================================================


			// ���������뵽����·����vector��
			vecPath.push_back(m_vecClose[i].current);

			// ���������Ϊ�µ��յ�,���ҵ��õ����һ����
			endNode = m_vecClose[i];

			// ��i�����ͷ��ʼ����.
			i = 0;
		}
	}

	return !vecPath.empty();
}