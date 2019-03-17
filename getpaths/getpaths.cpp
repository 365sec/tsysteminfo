// GetSysService.cpp : Defines the entry point for the console application.
//

#include "StdAfx.h"
#include <windows.h>
#include <iostream>
#include<stdio.h>
#include<stdio.h>
#include "server.h"
#include "getstart.h"
#include<set>
#include<string>
#include"system_path.h"
#include "getprocess.h"

using namespace std;

void main()
{
	int s = 1;
	while (s)
	{
		set<string> path;
		path.clear();
		printf("1服务 2进程 3启动项 \n");
		cin >> s;
		switch (s)
		{
		case 1:
		{
			get_system_service_path(path);
			break;
		}
		case 2:
		{
			get_system_process_path(path);
			break;
		}case 3:
		{
			get_system_start_path(path);
			break;
		}
		default:
			return;
		}
	}

}


