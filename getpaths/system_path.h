#pragma once
#include "StdAfx.h"
#include<set>
#include<string>
#include"process.h"
using namespace std;
/*
void system_deleteparm(std::set<string> &path)
{
	std::set<string>::iterator it;
	for (it = path.begin(); it != path.end(); it++)
	{
		printf("%s", *it);
	}
	return;
}
*/
int system_showpath(std::set<string> &path)
{
	std::set<string>::iterator it;
	for (it = path.begin(); it != path.end(); it++)
		printf("%s\n", it->c_str());
	return 0;
}
int get_system_start_path(std::set<string> &path) {
	getstart(path);
	system_showpath(path);
	return 0;
}

int get_system_process_path(std::set<string> &path) {
//	set<string> *p;
	allprecess(path);
	system_showpath(path);
	return 0;
}

int get_system_service_path(std::set<string> &path) {
	serverecho(path);
	system_showpath(path);
	return 0;
}
