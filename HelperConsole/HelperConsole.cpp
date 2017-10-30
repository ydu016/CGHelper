// InjectDll.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "tchar.h"
#include <windows.h>
#include "tlhelp32.h"

DWORD FindProcessID(LPCTSTR szProcessName)
{
	DWORD dwPID = 0xFFFFFFFF;
	HANDLE hSnapShot = INVALID_HANDLE_VALUE;
	PROCESSENTRY32 pe;

	pe.dwSize = sizeof(PROCESSENTRY32);
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);

	Process32First(hSnapShot, &pe);
	do
	{
		if (!_tcsicmp(szProcessName, (LPCTSTR)pe.szExeFile))
		{
			dwPID = pe.th32ProcessID;
			break;
		}
	} while (Process32Next(hSnapShot, &pe));

	CloseHandle(hSnapShot);

	return dwPID;
}

bool InjectDll(DWORD dwPID, LPCTSTR szDllPath)
{
	HANDLE hProcess = NULL, hThread = NULL;
	HMODULE hMod = NULL;
	LPVOID pRemoteBuf = NULL;
	DWORD dwBufSize = (DWORD)(_tcslen(szDllPath) + 1) * sizeof(TCHAR);
	LPTHREAD_START_ROUTINE pThreadProc;

	if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID)))
	{
		_tprintf(L"failed to open process %d\n", GetLastError());
		return FALSE;
	}
	else
		printf("process opened\n");

	pRemoteBuf = VirtualAllocEx(hProcess, NULL, dwBufSize, MEM_COMMIT, PAGE_READWRITE);
	printf("nbyte allocated %p\n", pRemoteBuf);

	SIZE_T nbyte;
	WriteProcessMemory(hProcess, pRemoteBuf, (LPVOID)szDllPath, dwBufSize, &nbyte);
	printf("nbyte wrote %d\n", nbyte);

	hMod = GetModuleHandle(L"kernel32.dll");
	pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hMod, "LoadLibraryW");

	if (pThreadProc == NULL)
		return FALSE;

	hThread = CreateRemoteThread(hProcess, NULL, 0, pThreadProc, pRemoteBuf, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	CloseHandle(hProcess);
	return TRUE;
}

BOOL EjectDll(DWORD dwPID, LPCTSTR szDllName)
{
	BOOL bMore = FALSE, bFound = FALSE;
	HANDLE hSnapshot, hProcess, hThread;
	HMODULE hModule = NULL;
	MODULEENTRY32 me = { sizeof(me) };
	LPTHREAD_START_ROUTINE pThreadProc;

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);

	bMore = Module32First(hSnapshot, &me);
	for (; bMore; bMore = Module32Next(hSnapshot, &me))
	{
		if (!_tcsicmp((LPCTSTR)me.szModule, szDllName) ||
			!_tcsicmp((LPCTSTR)me.szExePath, szDllName))
		{
			bFound = TRUE;
			break;
		}
	}

	if (!bFound)
	{
		CloseHandle(hSnapshot);
		return FALSE;
	}

	if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID)))
		return FALSE;

	hModule = GetModuleHandle(L"kernel32.dll");
	pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hModule, "FreeLibrary");
	hThread = CreateRemoteThread(hProcess, NULL, 0, pThreadProc, me.modBaseAddr, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	CloseHandle(hProcess);
	CloseHandle(hSnapshot);

	return TRUE;
}

typedef struct _THREAD_PARAM
{
	FARPROC pFunc[2];
	char szBuf[2][128];
	int crood[2];
}THREAD_PARAM, *PTHREAD_PARAM;

typedef HMODULE(WINAPI *PFLOADLIBRARYA)(LPCSTR lpLibFileName);
typedef FARPROC(WINAPI *PFGETPROCEADDRESS)(HMODULE hModule, LPCSTR lpProcName);
typedef BOOL(*AUTONAV)(int x, int y);

DWORD WINAPI ThreadProc(LPVOID lParam)
{
	PTHREAD_PARAM	pParam = (PTHREAD_PARAM)lParam;
	HMODULE			hMod = NULL;
	FARPROC			pFunc = NULL;

	hMod = ((PFLOADLIBRARYA)pParam->pFunc[0])(pParam->szBuf[0]);
	pFunc = (FARPROC)((PFGETPROCEADDRESS)pParam->pFunc[1])(hMod, pParam->szBuf[1]);
	((AUTONAV)pFunc)(pParam->crood[0], pParam->crood[1]);

	return 0;
}

BOOL InjectCode(DWORD dwPID, int x, int y)
{
	HMODULE			hMod			= NULL;
	THREAD_PARAM	param			= { 0, };
	HANDLE			hProcess		= NULL;
	HANDLE			hThread			= NULL;
	LPVOID			pRemoteBuf[2]	= { 0, };
	DWORD			dwSize			= 0;

	hMod = GetModuleHandleA("kernel32.dll");

	param.pFunc[0] = GetProcAddress(hMod, "LoadLibraryA");
	param.pFunc[1] = GetProcAddress(hMod, "GetProcAddress");
	strcpy_s(param.szBuf[0], "CGHelper.dll");
	strcpy_s(param.szBuf[1], "AutoNavigate");

	param.crood[0] = x;
	param.crood[1] = y;

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);

	dwSize = sizeof(THREAD_PARAM);
	pRemoteBuf[0] = VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(hProcess, pRemoteBuf[0], (LPVOID)&param, dwSize, NULL);

	dwSize = (DWORD)InjectCode - (DWORD)ThreadProc;
	pRemoteBuf[1] = VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	WriteProcessMemory(hProcess, pRemoteBuf[1], (LPVOID)ThreadProc, dwSize, NULL);
	hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pRemoteBuf[1],
		pRemoteBuf[0], 0, NULL);

	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	CloseHandle(hProcess);

	return TRUE;
}

int main()
{
	DWORD dwPID = FindProcessID(L"cg_item_6000.exe");
	while (1)
	{
		char ch = getchar();
		switch (ch)
		{
		case 'i':
			if (InjectDll(dwPID, L"C:\\Users\\CEG2136\\Documents\\Visual Studio 2017\\Projects\\CGHelper\\Release\\CGHelper.dll"))
				printf("注入成功\n");
			break;
		case 'r':
			InjectCode(dwPID, 23, 23);
			break;
		case 'e':
			if (EjectDll(dwPID, L"CGHelper.dll"))
				printf("卸载成功\n");
			break;
		default:
			break;
		}
		fflush(stdin);
	}
	return 0;
}

