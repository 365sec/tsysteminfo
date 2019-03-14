#pragma once
#include "StdAfx.h"
#include<set>
#include<string>
#include<iostream>
using namespace std;

int system_showpath(std::set<std::string> &path)
{
	std::set<std::string>::iterator it;
	for (it = path.begin(); it != path.end(); it++)
		cout << *it << endl;
	return 0;
}
int get_system_start_path(std::set<std::string> &path) {
	getstart(path);
	system_showpath(path);
	return 0;
}

int get_system_process_path(std::set<std::string> &path) {
//	set<string> *p;
	process(path);
	system_showpath(path);
	return 0;
}

int get_system_service_path(std::set<std::string> &path) {
	serverecho(path);
	system_showpath(path);
	return 0;
}
