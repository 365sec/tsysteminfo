#pragma once

#include <stdio.h>
#include <windows.h>
#include<conio.h>
#include<psapi.h>
#include<iostream>
#include "StdAfx.h"
#include<set>
#include<string>
using namespace std;

int process(set<string> &s) {
	TCHAR chBuf[1024];
	DWORD  lpidProcess[1024 * 2];
	DWORD  cbNeeded;
	//获取所有进程pid
	if (!EnumProcesses(lpidProcess, sizeof(lpidProcess), &cbNeeded)) {
		wsprintf(chBuf, __TEXT("EnumProcesses failed (%d)./n"), GetLastError());
		OutputDebugString(chBuf);
		return 0;
	}
	DWORD dwProcCount = cbNeeded / sizeof(DWORD);
	char strFilePath[256];
	char strProcessName[256];
	//遍历继承pid
	for (int i = 0; i < dwProcCount; i++) {
		HANDLE nHwndForExe = OpenProcess(PROCESS_ALL_ACCESS, FALSE, lpidProcess[i]);
		//获取路径
		GetModuleFileNameExA(nHwndForExe, NULL, strFilePath, 256);
//	printf("进程路径:%s\n", strFilePath);
		s.insert(strFilePath);
	}
	//_getch();
}