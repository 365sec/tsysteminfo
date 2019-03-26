// paths.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <iostream>
#include <set>
using namespace std;
#include "cy_system.h"
using namespace cy::system;
int main()
{
	set<string> paths;
	//System::get_system_process_path(paths);
	//System::get_system_service_path(paths);
	System::get_system_start_path(paths);
	set<string>::iterator it;
	for (it = paths.begin(); it != paths.end(); it++)
	{
		//printf("%d  %s\n", ss++, it->c_str());
		cout << it->c_str() << endl;
	}
	getchar();
}