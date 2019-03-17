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
			SC_HANDLE SCMan = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS); //打开系统服务控制器
			if (SCMan == NULL)
			{
				printf("OpenSCManager Eorror\n");
				return 0;
			}
			LPENUM_SERVICE_STATUS service_status; //保存系统服务的结构
			DWORD cbBytesNeeded = NULL;
			DWORD ServicesReturned = NULL;
			DWORD ResumeHandle = NULL;

			service_status = (LPENUM_SERVICE_STATUS)LocalAlloc(LPTR, 1024 * 64);//分配内存，注意大小


																				//获取系统服务的简单信息
			bool ESS = EnumServicesStatus(SCMan, //系统服务句柄
				SERVICE_WIN32, //服务的类型
				SERVICE_STATE_ALL,  //服务的状态
				(LPENUM_SERVICE_STATUS)service_status,  //输出参数，系统服务的结构
				1024 * 64,  // 结构的大小
				&cbBytesNeeded, //输出参数，接收返回所需的服务
				&ServicesReturned, //输出参数，接收返回服务的数量
				&ResumeHandle); //输入输出参数，第一次调用必须为0，返回为0代表成功
			if (ESS == NULL)
			{
				printf("EnumServicesStatus Eorror\n");
				return 0;
			}
			printf("当前服务数量为:%d\n", ServicesReturned);
			for (int i = 0; i<int(ServicesReturned); i++)
			{
				//printf("服务名: %s ", service_status[i].lpDisplayName);
				LPQUERY_SERVICE_CONFIG lpServiceConfig = NULL; //服务详细信息结构
				SC_HANDLE service_curren = NULL; //当前的服务句柄
				service_curren = OpenService(SCMan, service_status[i].lpServiceName, SERVICE_QUERY_CONFIG); //打开当前服务
				lpServiceConfig = (LPQUERY_SERVICE_CONFIG)LocalAlloc(LPTR, 8 * 1024);      //分配内存， 最大为8kb 

				if (NULL == QueryServiceConfig(service_curren, lpServiceConfig, 8 * 1024, &ResumeHandle))
				{
					printf("QueryServiceConfig Error\n");
					return 0;
				}
				//printf("路径: %s ", lpServiceConfig->lpBinaryPathName);//服务的路径
				s.insert(lpServiceConfig->lpBinaryPathName);
//				printf("\n");
				CloseServiceHandle(service_curren);//关闭当前服务的句柄
			}
			CloseServiceHandle(SCMan);//关闭服务管理器的句柄
		}
		return 0;
}
