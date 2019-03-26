#pragma once

#include<set>
#include <windows.h>
using namespace std;


namespace cy
{
	namespace system
	{
		class System
		{
		public:
			static BOOL DosPathToNtPath(char* pszDosPath, char* pszNtPath);
			/*
			* @brief   ��ȡ������·��
			* @param   LPTSTR 
			* @param   LPTSTR
			* @return  �ɹ�����true����������false
			*/
			static BOOL GetProcessFullPath(DWORD dwPID, char pszFullPath[MAX_PATH], set<string> &s);
			/*
			* @brief   ��ȡ����ȫ·��
			* @param   DWORD
			* @param   TCHAR 
			* @param   set<string>
			* @return  �ɹ�����true����������false
			*/
			static BOOL get_system_process_path(set<string> &s);
			/*
			* @brief   ����GetProcessFullPath(),���ؽ���ȫ·��
			* @param   set<string>
			* @return  �ɹ�����true����������false
			*/
			static BOOL get_system_service_path(set<string> &s);
			/*
			* @brief   ���ط���·��
			* @param   set<string>
			* @return  �ɹ�����true����������false
			*/
			static BOOL IsWow64();
			/*
			* @brief   �ж��Ƿ�Ϊwow64
			* @return  �ɹ�����true����������false
			*/
			static BOOL QueryKey(HKEY hKey, set<string> &s);
			/*
			* @brief   ��ע�����ȡ������·��
			* @param   HKEY ����
			* @param   set<string>
			* @return  �ɹ�����true����������false
			*/
			static BOOL get_system_start_path(set<string> &s);
			/*
			* @brief   ����QueryKey()
			* @param   set<string>
			* @return  �ɹ�����true����������false
			*/
		}; 
	}
}