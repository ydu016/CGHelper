// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "AStar.h"
#include <windows.h>

struct map
{
	char	name[3];
	BYTE	uk;
	DWORD	uk0;
	DWORD	uk1;
	DWORD	width;
	DWORD	height;
};

typedef DWORD(__cdecl *MAPBINARIZATION)(int a, int b, int c, int d, WORD *arg5, WORD *arg6, WORD *arg7, WORD *arg8);

#define CENTERX 0x140
#define CENTERY 0x0F0
typedef DWORD(__cdecl *MOUSEMOVE)(int x, int y);
typedef DWORD(__cdecl *LBUTTONDOWN)(int x, int y);
typedef DWORD(__cdecl *LBUTTONUP)(int x, int y);

void Move(Node node)
{
	MOUSEMOVE mousemove = (MOUSEMOVE)(0x576B00);
	LBUTTONDOWN	lbuttondown = (LBUTTONDOWN)(0x576B70);
	LBUTTONUP	lbuttonup = (LBUTTONUP)(0x576C70);

	DWORD X, Y = 0;
	switch (node.d)
	{
	case 0:
		X = CENTERX - 0x30;
		Y = CENTERY - 0x20;
		break;
	case 1:
		X = CENTERX + 0x30;
		Y = CENTERY + 0x20;
		break;
	case 2:
		X = CENTERX - 0x30;
		Y = CENTERY + 0x20;
		break;
	case 3:
		X = CENTERX + 0x30;
		Y = CENTERY - 0x20;
		break;
	default:
		break;
	}
	mousemove(X, Y);
	Sleep(100);
	lbuttondown(X, Y);
	Sleep(100);
	lbuttonup(X, Y);
	Sleep(100);
}

void getCurrentIndex(Node &index)
{
	//float x = *(float *)(0x9D1134);
	//float y = *(float *)(0xC1B354);
	DWORD x, y;
	__asm
	{
		push eax;
		pushfd;
		movss xmm0, dword ptr ds : [0x9D1134];
		cvttss2si eax, xmm0
			mov dword ptr[x], eax
			movss xmm0, dword ptr ds : [0xC1B354];
		cvttss2si eax, xmm0
			mov dword ptr[y], eax
			popfd;
		pop eax;
	}
	index.x = x / 64;
	index.y = y / 64;
}

BOOL getDir(Node& next) {
	Node current;
	getCurrentIndex(current);

	int diffX = next.x - current.x;
	int diffY = next.y - current.y;

	if (diffX == 0 && diffY == 0) {
		//MessageBoxA(NULL, "乖乖♂站好", NULL, 0);
		return false;
	}
	if (diffX != 0 && diffY != 0) {
		//MessageBoxA(NULL, "asdf0", NULL, 0);
		return false;
	}
	if (abs(diffX) > 1 || abs(diffY) > 1) {
		//MessageBoxA(NULL, "asdf1", NULL, 0);
		return false;
	}

	if (diffY < 0)
		next.d = 0;
	else if (diffY > 0)
		next.d = 1;
	else if (diffX < 0)
		next.d = 2;
	else if (diffX > 0)
		next.d = 3;

	return true;
}

BOOL AutoNavigate(Node dest)
{
	char path[] = "C:\\Users\\CEG2136\\Downloads\\CrossGate\\map\\0\\1531.dat";

	HANDLE hFile = INVALID_HANDLE_VALUE;
	hFile = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD dwFileSize = 0;
	dwFileSize = GetFileSize(hFile, NULL);

	BYTE* pBuf = new BYTE[dwFileSize];

	DWORD dwRead = 0;
	ReadFile(hFile, pBuf, dwFileSize, &dwRead, NULL);
	CloseHandle(hFile);

	struct map *m = (struct map *)pBuf;
	WORD *arg0 = (WORD *)(pBuf + sizeof(struct map));
	WORD *arg1 = arg0 + m->height * m->width;
	WORD *arg2 = arg1 + m->height * m->width;
	WORD* arg3 = new WORD[m->height * m->width * sizeof(WORD)];

	MAPBINARIZATION mapBinarization = (MAPBINARIZATION)0x4975F0;
	mapBinarization(0, 0, m->height, m->width, arg0, arg1, arg2, arg3);
	delete pBuf;

	for (DWORD i = 0; i < m->height; i++) {
		for (DWORD j = 0; j < m->width; j++) {
			if (arg3[i * m->height + j] != 0x1)
				arg3[i * m->height + j] = 0x0;
		}
	}

	Node index;
	getCurrentIndex(index);

	AStart astart;
	vector<Node> vecPath;
	for (DWORD i = 0; i < m->height; i++) {
		for (DWORD j = 0; j < m->width; j++) {
			if (arg3[i * m->height + j] == 0x1)
				astart.getMap()->setPoint(j, i, e_wall_c);
		}
	}
	delete arg3;

	vecPath.push_back(dest);
	astart.getPath(0, index.x, index.y, dest.x, dest.y, vecPath);

	for (signed int i = vecPath.size() - 1; i >= 0; i--) {
		if (!getDir(vecPath[i]))
			return false;
		Move(vecPath[i]);
	}

	//for (DWORD i = 0; i < vecPath.size(); i++)
	//	arg3[vecPath[i].y * m->height + vecPath[i].x] = 0x0002;
	/**
	char path0[] = "C:\\Users\\CEG2136\\Downloads\\CrossGate\\map\\0\\__1531.dat";

	HANDLE hFilew = INVALID_HANDLE_VALUE;
	hFilew = CreateFileA(path0, GENERIC_ALL, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD dwWrite = 0;
	WriteFile(hFilew, arg3, m->height * m->width * sizeof(WORD), &dwWrite, NULL);
	CloseHandle(hFilew);
	delete arg3;
	**/
	return true;
}

__declspec(dllexport) void AutoNavigate(int x, int y) {
	Node dest;
	dest.x = x;
	dest.y = y;
	while (!AutoNavigate(dest))
		;
	MessageBoxA(NULL, "Arrived", NULL, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		MessageBoxA(NULL, "DLL attached", NULL, 0);
		//AutoNavigate(23, 23);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		MessageBoxA(NULL, "DLL dettached", NULL, 0);
		break;
	}
	return TRUE;
}
