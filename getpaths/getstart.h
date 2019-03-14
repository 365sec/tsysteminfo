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

/*BOOL IsWow64()  //64?
{
	typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(_T("kernel32")), "IsWow64Process");
	BOOL bIsWow64 = FALSE;
	if (NULL != fnIsWow64Process)
	{
		fnIsWow64Process(GetCurrentProcess(), &bIsWow64);
	}


	return bIsWow64;

}*/

void QueryKey(HKEY hKey, set<string> &s)
{
	//TCHAR    achKey[MAX_KEY_LENGTH];   // �洢�Ӽ�����
	//DWORD    cbName;                   // �����ַ����ĳ���
	TCHAR    achClass[MAX_PATH] = TEXT("");  // ָ��һ���ִ�������װ�����ע���������� 
	DWORD    cchClassName = MAX_PATH;  // ָ��һ������������װ��lpClass�������ĳ��ȡ�һ�����أ�������Ϊʵ��װ�ص����������ֽ����� 
	DWORD    cSubKeys = 0;               // �Ӽ�����Ŀ 
	DWORD    cbMaxSubKey;              // ��������Ӽ����� 
	DWORD    cchMaxClass;              // ָ��һ������������װ�������֮������һ�������ĳ��� 
	DWORD    cValues;              // ����װ������������ֵ������һ������
	DWORD    cchMaxValue;          // value�������
	DWORD    cbMaxValueData;       // value�������
	DWORD    cbSecurityDescriptor; // ��ȫ�������Ĵ�С 
	FILETIME ftLastWriteTime;      // �ϴ�д���ʱ�� 

	DWORD i, retCode;
	DWORD dwType = REG_SZ;
	DWORD dwSize;

	TCHAR *wStr = new TCHAR[MAX_VALUE_NAME];
	TCHAR  achValue[MAX_VALUE_NAME];
	TCHAR  data[MAX_VALUE_NAME];
	DWORD cchValue = MAX_VALUE_NAME;

	// Get the class name and the value count. 
	retCode = RegQueryInfoKey(
		hKey,                    // ������� 
		achClass,                // ָ��һ���ִ�������װ�����ע����������
		&cchClassName,           // ָ��һ������������װ��lpClass�������ĳ��ȡ�һ�����أ�������Ϊʵ��װ�ص����������ֽ�����
		NULL,                    // reserved 
		&cSubKeys,               // ����װ�أ����棩����������������һ������
		&cbMaxSubKey,            // ָ��һ������������װ��������һ������ĳ��ȡ�ע��������Ȳ���������ֹ�ַ�
		&cchMaxClass,            // ָ��һ������������װ�������֮������һ�������ĳ���
		&cValues,                // ����װ������������ֵ������һ������
		&cchMaxValue,            // ָ��һ������������װ�������֮������һ��ֵ���ĳ���
		&cbMaxValueData,         // ָ��һ������������װ������������һ��ֵ��������Ļ���������
		&cbSecurityDescriptor,   // װ��ֵ��ȫ���������ȵ�һ������ 
		&ftLastWriteTime);       // ָ��һ���ṹ���������ɸ������һ���޸�ʱ�� 

								 //ö������

								 /*if (cSubKeys)
								 {
								 //printf( "\nNumber of subkeys: %d\n", cSubKeys);

								 for (i=0; i<cSubKeys; i++)
								 {
								 cbName = MAX_KEY_LENGTH;
								 retCode = RegEnumKeyEx(hKey, i,
								 achKey,
								 &cbName,
								 NULL,
								 NULL,
								 NULL,
								 &ftLastWriteTime);
								 if (retCode == ERROR_SUCCESS)
								 {
								 _tprintf(TEXT("(%d) %s\n"), i+1, achKey);
								 }
								 }
								 } */

								 // ö�ټ�ֵ. 

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
				s.insert(data);
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
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
			TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"),
			0,
			KEY_READ | KEY_WOW64_64KEY,
			&hTestKey) == ERROR_SUCCESS
			)

		{
			QueryKey(hTestKey,s);
		}
		else
			printf("64error \n");
		RegCloseKey(hTestKey);

		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
			TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"),
			0,
			KEY_READ,
			&aTestKey) == ERROR_SUCCESS
			)
		{
			QueryKey(aTestKey,s);
		}
		RegCloseKey(aTestKey);
}