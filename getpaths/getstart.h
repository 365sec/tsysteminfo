#pragma once
#include "stdafx.h"
#include<windows.h>
#include<stdio.h>
#include<string.h>
#include "StdAfx.h"
#include<set>
#include<string>
#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

BOOL IsWow64()  //64?
{
	typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(_T("kernel32")), "IsWow64Process");
	BOOL bIsWow64 = FALSE;
	if (NULL != fnIsWow64Process)
	{
		fnIsWow64Process(GetCurrentProcess(), &bIsWow64);
	}


	return bIsWow64;

}

void QueryKey(HKEY hKey, set<string> &s)
{
	//TCHAR    achKey[MAX_KEY_LENGTH];   // 存储子键名称
	//DWORD    cbName;                   // 名字字符串的长度
	TCHAR    achClass[MAX_PATH] = TEXT("");  // 指定一个字串，用于装载这个注册表项的类名 
	DWORD    cchClassName = MAX_PATH;  // 指定一个变量，用于装载lpClass缓冲区的长度。一旦返回，它会设为实际装载到缓冲区的字节数量 
	DWORD    cSubKeys = 0;               // 子键的数目 
	DWORD    cbMaxSubKey;              // 设置最大子键长度 
	DWORD    cchMaxClass;              // 指定一个变量，用于装载这个项之子项的最长一个类名的长度 
	DWORD    cValues;              // 用于装载这个项的设置值数量的一个变量
	DWORD    cchMaxValue;          // value的最长名字
	DWORD    cbMaxValueData;       // value的最长数据
	DWORD    cbSecurityDescriptor; // 安全描述符的大小 
	FILETIME ftLastWriteTime;      // 上次写入的时间 

	DWORD i, retCode;
	DWORD dwType = REG_SZ;
	DWORD dwSize;

	TCHAR *wStr = new TCHAR[MAX_VALUE_NAME];
	TCHAR  achValue[MAX_VALUE_NAME];
	TCHAR  data[MAX_VALUE_NAME];
	DWORD cchValue = MAX_VALUE_NAME;

	// Get the class name and the value count. 
	retCode = RegQueryInfoKey(
		hKey,                    // 主键句柄 
		achClass,                // 指定一个字串，用于装载这个注册表项的类名
		&cchClassName,           // 指定一个变量，用于装载lpClass缓冲区的长度。一旦返回，它会设为实际装载到缓冲区的字节数量
		NULL,                    // reserved 
		&cSubKeys,               // 用于装载（保存）这个项的子项数量的一个变量
		&cbMaxSubKey,            // 指定一个变量，用于装载这个项最长一个子项的长度。注意这个长度不包括空中止字符
		&cchMaxClass,            // 指定一个变量，用于装载这个项之子项的最长一个类名的长度
		&cValues,                // 用于装载这个项的设置值数量的一个变量
		&cchMaxValue,            // 指定一个变量，用于装载这个项之子项的最长一个值名的长度
		&cbMaxValueData,         // 指定一个变量，用于装载容下这个项最长一个值数据所需的缓冲区长度
		&cbSecurityDescriptor,   // 装载值安全描述符长度的一个变量 
		&ftLastWriteTime);       // 指定一个结构，用于容纳该项的上一次修改时间

	if (cValues)
	{
		//printf( "\nNumber of values: %d\n", cValues);

		for (i = 0, retCode = ERROR_SUCCESS; i<cValues; i++)
		{
			cchValue = MAX_VALUE_NAME;
			dwSize = MAX_VALUE_NAME;
			dwType = REG_SZ;
			achValue[0] = '\0';
			data[0] = '\0';
			retCode = RegEnumValue(hKey, i,
				wStr,
				&cchValue,
				NULL,
				NULL,
				NULL,
				NULL);
			RegQueryValueEx(hKey, wStr,
				NULL,
				&dwType,
				(LPBYTE)data,
				&dwSize);

			if (retCode == ERROR_SUCCESS)
			{
				int nArgs = 0;
				LPWSTR *szArglist = NULL;
				szArglist = CommandLineToArgvW(cy::convert::StringConvert::AnsiToWide(data).c_str(), &nArgs);

				if (NULL != szArglist) {

					s.insert(cy::convert::StringConvert::WideToAnsi(szArglist[0]).c_str());
					//	printf("%d %s\n",ssss++, cy::convert::StringConvert::WideToAnsi(szArglist[0]).c_str());
					LocalFree(szArglist);
				}
				//s.insert(data);
			}
		}
	}
	delete[]wStr;
	RegCloseKey(hKey);
}
void getstart(set<string> &s)
{

		HKEY hTestKey;
		HKEY aTestKey = NULL;
		HKEY sTestKey =NULL;
		
			if (RegOpenKeyEx(HKEY_CURRENT_USER,
				TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"),
				0,
				KEY_ALL_ACCESS,
				&hTestKey) == ERROR_SUCCESS
				)

			{
				QueryKey(hTestKey, s);
			}
			RegCloseKey(hTestKey);
		
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
				TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"),
				0,
				KEY_ALL_ACCESS,
				&aTestKey) == ERROR_SUCCESS
				)
			{
				QueryKey(aTestKey, s);
			}
			RegCloseKey(aTestKey);
			if (IsWow64())
			{
				if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
					TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"),
					0,
					KEY_ALL_ACCESS | KEY_WOW64_64KEY,
					&sTestKey) == ERROR_SUCCESS
					)
				{
					QueryKey(sTestKey, s);
				}
				RegCloseKey(sTestKey);
			}
			
		
}
