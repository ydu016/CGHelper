#include "stdafx.h"
#include "AStar.h"




//-----------------------------------
// 函数名: setStartPos
// 功  能: 设置起点坐标
// 返回值: void 
// 参  数: int x	
// 参  数: int y
//-----------------------------------
void AStart::setStartPos(int x, int y) {
	m_startNode.x = x;
	m_startNode.y = y;
}

//-----------------------------------
// 函数名: setEndPos
// 功  能: 设置终点坐标
// 返回值: void 
// 参  数: int x
// 参  数: int y
//-----------------------------------
void AStart::setEndPos(int x, int y) {
	m_endNode.x = x;
	m_endNode.y = y;
}

//-----------------------------------
// 函数名: setStartDirection
// 功  能: 设置起点的初始方向
// 返回值: void 
// 参  数: int direction
//-----------------------------------
void AStart::setStartDirection(int direction) {
	m_startNode.d = direction;
}

ConsoleMap* AStart::getMap() {
	return &g_gameMap;
}

//-----------------------------------
// 函数名: getPath
// 功  能: 获取起点到终点间一条连续通行的路径
// 返回值: bool 
// 参  数: int direction	 起点处的初始方向
// 参  数: int nStartX	 起点坐标X
// 参  数: int nStartY	 起点坐标Y
// 参  数: int nEndX		 终点坐标X
// 参  数: int nEndY		 终点坐标Y
// 参  数: vector<Node> & vecPath
//-----------------------------------
bool AStart::getPath(int direction,
	int nStartX,
	int nStartY,
	int nEndX,
	int nEndY,
	vector<Node>& vecPath) {

	// 设置起点坐标
	setStartPos(nStartX, nStartY);
	// 设置终点坐标
	setEndPos(nEndX, nEndY);

	// 设置起点初始方向
	setStartDirection(direction);

	// 获取路径
	return getPath(vecPath);
}



//-----------------------------------
// 函数名: getPath
// 功  能: 获取起点到终点间一条连续通行的路径,并将获取到的路径保存到vecPath中
// 返回值: bool 
// 参  数: vector<Node> & vecPath
//-----------------------------------
bool AStart::getPath(vector<Node>& vecPath) {

	/*
	* 获取路径的方法:
	* 1. 将起点放入到open表中.
	* 2. 不断地从open表中取出一个F值最小的点. 如果没有这一步,就相当于将全地图的点都判断一遍.
	* 3. 由F值最小的点生成四周的4个点, 检查这4个点是否是终点, 如果是就能够找到路径,并离开循环.
	* 4. 如果4个点中没有一个点是终点, 则将4个点中有效的点放入到open表中,有效的点指的是:点不
	*    在open表中，也不在close表中, 也不能是一个不可通行的点。
	* 5. 将那个从open表中取出的F值最小的点放入close表中,并从open表中删除它.这样一
	*	 来, open表的点才会一个一个地减少, 循环才能够退出。
	* 6. 循环第2步
	**
	***/

	// 判断起点是否是终点
	if (isEndPos(m_startNode)) {
		return false;
	}

	// 清空两张表
	m_vecOpen.clear();
	m_vecClose.clear();

	Point point = { m_startNode };
	// 将起点加入到open表
	m_vecOpen.push_back(point);


	Point	stcArray[4];

	//1. 循环在open表中找到所有的点(如果open表中没有点,则退出循环)
	while (!m_vecOpen.empty()) {

		//2. 在open表中找到一个F值最小的点
		int  nMinIndex = getOpenMinNode();

		//3. 从得到的点的坐标中生成周围4个点
		createNode(m_vecOpen[nMinIndex].current, stcArray);


		//=====================================================================================
		//============================ 以下代码用于调试 ==========================================
		// g_map 是一个全局对象, 在main函数所在文件中定义.
#ifdef _DEBUG
		// 暂停,等待用户按键
		//getchar( );
		// 将当前点打印到屏幕上
		g_gameMap.show(m_vecOpen[nMinIndex].current.x,
			m_vecOpen[nMinIndex].current.y,
			"⊙",
			COLOR_YELLOW_FOR | COLOR_HIGHTLIGHT_FOR);
#endif
		//============================ 以上代码用于调试 ==========================================


		//3. 将值最小的点从open放入到close表,并从open中删除这个点.
		m_vecClose.push_back(m_vecOpen[nMinIndex]);
		m_vecOpen.erase(m_vecOpen.begin() + nMinIndex);

		for (int i = 0; i < 4; ++i) {

			// 4.1 判断这4个点中,是否有一个点是终点, 如果是,则直接获取路径,然后返回.
			if (isEndPos(stcArray[i].current)) {

				// 找出连接起点和终点的路径
				return searchPath(stcArray[i], vecPath);
			}
			// 4.2 将4个点中有效的点存入open表中.
			else if (isValidNode(stcArray[i].current)) {

				g_gameMap.show(stcArray[i].current.x, stcArray[i].current.y, "◆", COLOR_GREEN_FOR);

				// 将有效的点放入到open表中
				m_vecOpen.push_back(stcArray[i]);
			}
		}
	}

	return false;
}

//-----------------------------------
// 函数名: isEndPos
// 功  能: 判断一个节点是否是终点
// 返回值: bool 
// 参  数: const Node & node 要判断的点
//-----------------------------------
bool AStart::isEndPos(const Node& node) {

	return m_endNode == node;
}

//-----------------------------------
// 函数名: isValidNode
// 功  能: 判断一个点是否有效的点
// 返回值: bool 
// 参  数: const Node & node
//-----------------------------------
bool AStart::isValidNode(const Node& node) {

	// 1. 查找open表,判断点是否在open表中
	for (unsigned int i = 0; i < m_vecOpen.size(); ++i) {
		if (node == m_vecOpen[i].current)
			return false;
	}

	// 2. 查找close表,判断点是否在close表中
	for (unsigned int i = 0; i < m_vecClose.size(); ++i) {
		if (node == m_vecClose[i].current)
			return false;
	}

	// 3. 判断点在地图上是否是一个有效的点(该点不是障碍物就是有效的点)
	if (g_gameMap.isPass(node.x, node.y) == false)
		return false;

	return true;
}

//-----------------------------------
// 函数名: createNode
// 功  能: 用一个中心点生成它周围的4个点
// 返回值: void 
// 参  数: const Node & center 中心点
// 参  数: Node * pArray		  数组,用于保存生成出来4个点
//-----------------------------------
void AStart::createNode(const Node& center, Point* pArray) {

	/*
	*
	* 		      [up:0]
	*		        ↑
	* [left:2] ← [center] → [right:3]
	*	            ↓
	*	         [down:1]
	**/
	//>>>>>>>>>>>>>>>> up,down,left,right
	int nStepX[4] = { 0 , 0 , -1 , 1 };
	int nStepY[4] = { -1 , 1 , 0 , 0 };

	for (int i = 0; i < 4; ++i)
	{
		// 将中心点设置为新点的上一个点
		pArray[i].previous = center;

		// 设置新点当前点的属性
		// 设置方向
		pArray[i].current.d = i;

		// 设置移动损耗,每移动一步,损耗加1
		pArray[i].current.g = center.g + 1;

		// 设置x,y坐标
		pArray[i].current.x = center.x + nStepX[i];
		pArray[i].current.y = center.y + nStepY[i];

		// 设置当前点到终点的距离: H值
		pArray[i].current.setH(m_endNode.x, m_endNode.y);
	}

}

//-----------------------------------
// 函数名: getOpenMinNode
// 功  能: 获取open表中F值最小的元素
// 返回值: vector<Node>::iterator 
//-----------------------------------
int AStart::getOpenMinNode() {

	// 遍历vector,找到一个F值最小的元素
	int minIndex = 0;

	int F = m_vecOpen.begin()->current.f;

	for (unsigned int i = 0; i < m_vecOpen.size(); ++i) {

		if (F >= m_vecOpen[i].current.f) {

			// 保存最小值
			F = m_vecOpen[i].current.f;

			// 记录最小值得下标
			minIndex = i;
		}
	}

	return minIndex;
}

//-----------------------------------
// 函数名: findPath
// 功  能: 在close表中查找路径
// 返回值: bool 
// 参  数: const Node & endNode
// 参  数: vector<Node> & vecPath
//-----------------------------------
bool AStart::searchPath(Point endNode, vector<Node>& vecPath) {

	/**
	* 在close表中查找一条通往起点的连续的路径.
	* 查找依据:
	*  1. 在close表中, 每个点都记录着它前一个点在哪里.
	*  2. 已知最后一个点是endNode , 这个点保存着上一个点.
	*  3. 通过endNode可以找到它的上一个点, 通过它的上一个点可以找到上上个点.
	*  4. 依此类推,能够找到起点.
	*/

	// 将终点存入到保存路径的vector中.

	// 在close表中循环查找上一个点
	for (unsigned int i = 0; i < m_vecClose.size(); ++i) {

		// 如果close表中的一个点的当前点和终点的上一个点一样, 说明
		// 该点就是终点的前一个点.
		if (m_vecClose[i].current == endNode.previous) {


			//=====================================================================================
			//============================ 以下代码用于调试 ==========================================
			// g_map 是一个全局对象, 在main函数所在文件中定义.
#ifdef _DEBUG
			// 等待用户按键
			//getchar( );

			// 根据当前点的方向在屏幕上画出箭头.
			switch (m_vecClose[i].current.d) {
			case 0: g_gameMap.show(m_vecClose[i].current.x, m_vecClose[i].current.y, "↑", 10);
				break;
			case 1: g_gameMap.show(m_vecClose[i].current.x, m_vecClose[i].current.y, "↓", 10);
				break;
			case 2: g_gameMap.show(m_vecClose[i].current.x, m_vecClose[i].current.y, "←", 10);
				break;
			case 3: g_gameMap.show(m_vecClose[i].current.x, m_vecClose[i].current.y, "→", 10);
				break;
			}
#endif
			//============================ 以上代码用于调试 ==========================================
			//=====================================================================================


			// 将这个点存入到保存路径的vector中
			vecPath.push_back(m_vecClose[i].current);

			// 这个点设置为新的终点,以找到该点的上一个点
			endNode = m_vecClose[i];

			// 将i置零从头开始查找.
			i = 0;
		}
	}

	return !vecPath.empty();
}