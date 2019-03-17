#pragma once
#include "StdAfx.h"
#include <windows.h>
#include <iostream>
#include<stdio.h>
#include<set>
#include<string>
using namespace std;

int serverecho(set<string> &s)
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
				return 0;
			}
			printf("��ǰ��������Ϊ:%d\n", ServicesReturned);
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
				//printf("·��: %s ", lpServiceConfig->lpBinaryPathName);//�����·��
				s.insert(lpServiceConfig->lpBinaryPathName);
//				printf("\n");
				CloseServiceHandle(service_curren);//�رյ�ǰ����ľ��
			}
			CloseServiceHandle(SCMan);//�رշ���������ľ��
		}
		return 0;
}
