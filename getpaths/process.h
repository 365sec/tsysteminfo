#pragma once

#include <stdio.h>
#include <windows.h>
#include<conio.h>
#include<psapi.h>
#include<iostream>
#include "StdAfx.h"
#include<set>
#include<string>
#include"cy_debug.h"
#include"cy_stringconvert.h"""
using namespace std;

int process(set<string> &s) {
	char chBuf[1024];
	DWORD  lpidProcess[1024 * 2];
	DWORD  cbNeeded;
	//��ȡ���н���pid
	if (!EnumProcesses(lpidProcess, sizeof(lpidProcess), &cbNeeded)) {
		wsprintf(chBuf, __TEXT("EnumProcesses failed (%d)./n"), GetLastError());
		OutputDebugStringA(chBuf);
		return 0;
	}

	DWORD dwProcCount = cbNeeded / sizeof(DWORD);
	wchar_t strFilePath[256];
	char strProcessName[256];
	//�����̳�pid
	for (int i = 0; i < dwProcCount; i++) {
		HANDLE nHwndForExe = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, lpidProcess[i]);
		//��ȡ·��
		memset(strFilePath, 0, 256);
		GetModuleFileNameExW(nHwndForExe, NULL, strFilePath, 256);

		cy_outputdebugstring(L"-->%s\n", strFilePath);

		//printf("--��%s \n", strFilePath);
		
//	printf("����·��:%s\n", strFilePath);
		//s.insert(strFilePath);
	}
	//_getch();
}