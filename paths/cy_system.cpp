#include "stdafx.h"
#include <tchar.h>
#ifndef PSAPI_VERSION
#define PSAPI_VERSION  1
#endif
#include<set>
#include <windows.h>
#include <Psapi.h>
#include <tlhelp32.h>
#include "cy_system.h"
#pragma comment (lib,"Psapi.lib")
#include <atlconv.h>
#include "cy_stringconvert.h"
using namespace cy::system;
using namespace cy::convert;
using namespace std;
#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

BOOL System::DosPathToNtPath(char* pszDosPath, char* pszNtPath)
{
	char            szDriveStr[500];
	char            szDrive[3];
	char            szDevName[100];
	int                cchDevName;
	int                i;

	//������
	if (!pszDosPath || !pszNtPath)
		return FALSE;

	//��ȡ���ش����ַ���
	if (GetLogicalDriveStringsA(sizeof(szDriveStr), szDriveStr))
	{
		for (i = 0; szDriveStr[i]; i += 4)
		{
			if (!lstrcmpiA(&(szDriveStr[i]), ("A:\\")) || !lstrcmpiA(&(szDriveStr[i]), ("B:\\")))
				continue;

			szDrive[0] = szDriveStr[i];
			szDrive[1] = szDriveStr[i + 1];
			szDrive[2] = '\0';
			if (!QueryDosDeviceA(szDrive, szDevName, 100))//��ѯ Dos �豸��
				return FALSE;

			cchDevName = lstrlenA(szDevName);
			if (_tcsnicmp(pszDosPath, szDevName, cchDevName) == 0)//����
			{
				lstrcpyA(pszNtPath, szDrive);//����������
				lstrcatA(pszNtPath, pszDosPath + cchDevName);//����·��

				return TRUE;
			}
		}
	}

	lstrcpyA(pszNtPath, pszDosPath);

	return FALSE;
};
BOOL System::GetProcessFullPath(DWORD dwPID, char pszFullPath[MAX_PATH], set<string> &s)
{
	char        szImagePath[MAX_PATH];
	HANDLE        hProcess;
	if (!pszFullPath)
		return FALSE;

	pszFullPath[0] = '\0';
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, 0, dwPID);
	if (!hProcess)
		return FALSE;

	if (!GetProcessImageFileNameA(hProcess, szImagePath, MAX_PATH))
	{
		CloseHandle(hProcess);
		return FALSE;
	}

	if (!DosPathToNtPath(szImagePath, pszFullPath))
	{
		CloseHandle(hProcess);
		return FALSE;
	}

	CloseHandle(hProcess);

	//_tprintf(_T("%s \r\n"), pszFullPath);
	if (pszFullPath[0] != 0)
	{
		//		printf("%d --- %s\n", i++, shortpath);
		s.insert(pszFullPath);
	}
	return TRUE;
}

BOOL System::get_system_process_path(set<string> &s)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot)
	{
		return NULL;
	}
	PROCESSENTRY32 pe = { 0 };
	pe.dwSize = sizeof(PROCESSENTRY32);

	BOOL fOk;
	for (fOk = Process32First(hSnapshot, &pe); fOk; fOk = Process32Next(hSnapshot, &pe))
	{
		char szProcessName[MAX_PATH] = { 0 };
		GetProcessFullPath(pe.th32ProcessID, szProcessName, s);
	}
	return FALSE;
}

BOOL System::get_system_service_path(set<string> &s)
{
	int menu = 1;
	if (menu == 1)
	{
		SC_HANDLE SCMan = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS); //��ϵͳ���������
		if (SCMan == NULL)
		{
			printf("OpenSCManager Eorror\n");
			return 0;
		}
		LPENUM_SERVICE_STATUS service_status; //����ϵͳ����Ľṹ
		DWORD cbBytesNeeded = NULL;
		DWORD ServicesReturned = NULL;
		DWORD ResumeHandle = NULL;

		service_status = (LPENUM_SERVICE_STATUS)LocalAlloc(LPTR, 1024 * 64);//�����ڴ棬ע���С


																			//��ȡϵͳ����ļ���Ϣ
		bool ESS = EnumServicesStatus(SCMan, //ϵͳ������
			SERVICE_WIN32, //���������
			SERVICE_STATE_ALL,  //�����״̬
			(LPENUM_SERVICE_STATUS)service_status,  //���������ϵͳ����Ľṹ
			1024 * 64,  // �ṹ�Ĵ�С
			&cbBytesNeeded, //������������շ�������ķ���
			&ServicesReturned, //������������շ��ط��������
			&ResumeHandle); //���������������һ�ε��ñ���Ϊ0������Ϊ0����ɹ�
		if (ESS == NULL)
		{
			printf("EnumServicesStatus Eorror\n");
			return FALSE;
		}
		for (int i = 0; i<int(ServicesReturned); i++)
		{
			//printf("������: %s ", service_status[i].lpDisplayName);
			LPQUERY_SERVICE_CONFIG lpServiceConfig = NULL; //������ϸ��Ϣ�ṹ
			SC_HANDLE service_curren = NULL; //��ǰ�ķ�����
			service_curren = OpenService(SCMan, service_status[i].lpServiceName, SERVICE_QUERY_CONFIG); //�򿪵�ǰ����
			lpServiceConfig = (LPQUERY_SERVICE_CONFIG)LocalAlloc(LPTR, 8 * 1024);      //�����ڴ棬 ���Ϊ8kb 

			if (NULL == QueryServiceConfig(service_curren, lpServiceConfig, 8 * 1024, &ResumeHandle))
			{
				printf("QueryServiceConfig Error\n");
				return 0;
			}
			int nArgs = 0;
			LPWSTR *szArglist = NULL;
			szArglist = CommandLineToArgvW(cy::convert::StringConvert::AnsiToWide(lpServiceConfig->lpBinaryPathName).c_str(), &nArgs);

			if (NULL != szArglist) {

				s.insert(cy::convert::StringConvert::WideToAnsi(szArglist[0]).c_str());
				//	printf("%d %s\n",ssss++, cy::convert::StringConvert::WideToAnsi(szArglist[0]).c_str());
				LocalFree(szArglist);
			}
			CloseServiceHandle(service_curren);//�رյ�ǰ����ľ��
		}
		CloseServiceHandle(SCMan);//�رշ���������ľ��
	}
	return 0;
}/*
BOOL System::IsWow64()  //64?
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

BOOL System::QueryKey(HKEY hKey, set<string> &s)
{
	{
		//TCHAR    achKey[MAX_KEY_LENGTH];   // �洢�Ӽ�����
		//DWORD    cbName;                   // �����ַ����ĳ���
		char    achClass[MAX_PATH] = TEXT("");  // ָ��һ���ִ�������װ�����ע���������� 
		unsigned long    cchClassName = MAX_PATH;  // ָ��һ������������װ��lpClass�������ĳ��ȡ�һ�����أ�������Ϊʵ��װ�ص����������ֽ����� 
		unsigned long    cSubKeys = 0;               // �Ӽ�����Ŀ 
		unsigned long    cbMaxSubKey;              // ��������Ӽ����� 
		unsigned long    cchMaxClass;              // ָ��һ������������װ�������֮������һ�������ĳ��� 
		unsigned long    cValues;              // ����װ������������ֵ������һ������
		unsigned long    cchMaxValue;          // value�������
		unsigned long    cbMaxValueData;       // value�������
		unsigned long    cbSecurityDescriptor; // ��ȫ�������Ĵ�С 
		FILETIME ftLastWriteTime;      // �ϴ�д���ʱ�� 

		unsigned long i, retCode;
		unsigned long dwType = REG_SZ;
		unsigned long dwSize;

		char *wStr = new char[MAX_VALUE_NAME];
		char  achValue[MAX_VALUE_NAME];
		char  data[MAX_VALUE_NAME];
		unsigned long cchValue = MAX_VALUE_NAME;

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

		if (cValues)
		{
			//printf( "\nNumber of values: %d\n", cValues);

			for (i = 0, retCode = ERROR_SUCCESS; i < cValues; i++)
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
					  //  printf("%s\n",cy::convert::StringConvert::WideToAnsi(szArglist[0]).c_str());
						LocalFree(szArglist);
					}
					//s.insert(data);
				}
			}
		}
		delete[]wStr;
		RegCloseKey(hKey);
	}
	return 0;
}
BOOL  System::get_system_start_path(set<string> &s)
{

	HKEY hTestKey;
	HKEY aTestKey;
	HKEY sTestKey = NULL;

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
	//printf("%d", IsWow64());
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
			TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"),
			0,
			KEY_ALL_ACCESS | KEY_WOW64_64KEY,
			&sTestKey) == ERROR_SUCCESS
			)
		{
		//	printf("%s\n", "dfaf");
			QueryKey(sTestKey, s);
		}
	return 0;
}