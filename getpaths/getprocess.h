#pragma once
#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>
#include "StdAfx.h"
#include<iostream>
#include<set>
#include<stdlib.h>
using namespace std;

int getprocess(set<string> &s)
{

	HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	HANDLE hModule;
	PROCESSENTRY32* pinfo = new PROCESSENTRY32;
	MODULEENTRY32* minfo = new MODULEENTRY32;
	pinfo->dwSize = sizeof(PROCESSENTRY32);
	minfo->dwSize = sizeof(MODULEENTRY32);
	BOOL report;
	char shortpath[MAX_PATH] = "";
	int i = 0;

	report = Process32First(hProcess, pinfo);
	while (report)
	{
	 
		hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pinfo->th32ProcessID);
		Module32First(hModule, minfo);
		memset(shortpath,0, sizeof(shortpath));
		GetShortPathName(minfo->szExePath, shortpath, 256);
		if (shortpath[0] != 0) 
		{
	//		printf("%d --- %s\n", i++, shortpath);
			s.insert(shortpath);
		}
		report = Process32Next(hProcess, pinfo);
	}
	CloseHandle(hProcess);
	CloseHandle(hModule);
	return 0;
}

