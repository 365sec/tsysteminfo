#include<stdarg.h>
#include<stdio.h>
#include<Windows.h>

#include"cy_debug.h"
#include"cy_stringconvert.h"
//#include"log4cxxutils.h"

//update at 2016/11/10 by mj
void cy_outputdebugstring(const char*format,...)
{
	if (format == NULL)
	{
		return ;
	}

	string formattedStr("");

	va_list argsList = NULL; 

	// initialize variable arguments
	va_start(argsList, format); 

	// Get formatted string length adding one for NULL
	size_t formattedStrLen = _vscprintf(format, argsList) + 1;

	// Create a char vector to hold the formatted string.
	vector<char> buffer(formattedStrLen, '\0');

	int nWritten = _vsnprintf_s(&buffer[0], buffer.size(), formattedStrLen, format, argsList);    

	if (nWritten > 0)
	{
		formattedStr = &buffer[0];
	}

	// Reset variable arguments
	va_end(argsList); 

	::OutputDebugStringA(formattedStr.c_str());
	//LOG4CXX_DEBUG(log4cxx::Logger::getRootLogger(), formattedStr);

// 	va_list va;
// 	char stringbuffer[40960];
// 	va_start(va,format);
// 	vsprintf(stringbuffer,format,va);
// 	va_end(va);
// 	::OutputDebugStringA(stringbuffer);
// 	LOG4CXX_DEBUG(log4cxx::Logger::getRootLogger(),stringbuffer);
}

void cy_outputdebugstring(const wchar_t* format,...)
{
	if (format == NULL)
	{
		return ;
	}

	wstring retStr(L"");

	va_list marker = NULL; 

	// initialize variable arguments
	va_start(marker, format); 

	// Get formatted string length adding one for NULL
	size_t len = _vscwprintf(format, marker) + 1;

	// Create a wchar vector to hold the formatted string.
	vector<wchar_t> buffer(len, '\0');
	int nWritten = _vsnwprintf_s(&buffer[0], buffer.size(), len, format, marker);    

	if (nWritten > 0)
	{
		retStr = &buffer[0];
	}

	// Reset variable arguments
	va_end(marker); 

	::OutputDebugStringW(retStr.c_str());
	//LOG4CXX_DEBUG(log4cxx::Logger::getRootLogger(), cy::convert::StringConvert::WideToAnsi(retStr));

// 	va_list va;
// 	wchar_t stringbuffer[4096*10];
// 	va_start(va,format);
// 	wvsprintf(stringbuffer,format,va);
// 	va_end(va);
// 	::OutputDebugStringW(stringbuffer);
// 	LOG4CXX_DEBUG(log4cxx::Logger::getRootLogger(),cy::convert::StringConvert::WideToAnsi(stringbuffer).c_str());
}
