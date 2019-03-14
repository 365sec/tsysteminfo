#include "stdafx.h"
#include <vector>
#include <algorithm>
#include <string>
#include <sstream> 
#include<tchar.h>
#include <atlconv.h>
#include "cy_stringconvert.h"

using namespace cy::convert;
using namespace std;

string StringConvert::IntToAnsi(int value){
	char str[100];
	sprintf_s(str,sizeof(str),"%d",value);
	return str;
}

wstring StringConvert::IntToWide(int value){
	wchar_t str[100];
	swprintf_s(str,sizeof(str),L"%d",value);
	return str;
}

string  StringConvert::DoubleToAnsi(double value){
	char str[100];
	sprintf_s(str,sizeof(str),"%.2lf",value);
	return str;
}

wstring  StringConvert::DoubleToWide(double value){
	wchar_t str[100];
	memset(str,0,sizeof(str));
	//_stprintf_s(str,sizeof(str)/2,L"%.2lf",value);
	return str;
}

bool StringConvert::WideToAnsi(const wstring& wideString, string& ansiString)
{
    if (wideString.empty())
    {
        return false;
    }

    // 使用-1后 返回的长度包含最后的NULL字符
    int nLen = WideCharToMultiByte(CP_ACP, 0, wideString.c_str(), -1, NULL, 0, NULL, NULL);

    if (nLen<= 0) 
    {
        ansiString = "";
        return false;
    }

    vector<char> resultstring(nLen);

    WideCharToMultiByte(CP_ACP, 0, wideString.c_str(), -1, &resultstring[0], nLen, NULL, NULL);

   // ansiString = string(string(&resultstring[0]));  
	ansiString = string(&resultstring[0]);  

    return true;
}


string StringConvert::WideToAnsi( const wstring& wstr )
{
    string ansiStr;
    if (wstr.empty())
    {
        return ansiStr;
    }

    if (!WideToAnsi(wstr, ansiStr))
    {
        ansiStr = "";
    }

    return ansiStr;
}

bool StringConvert::AnsiToWide(const string& str, wstring& wideString)
{
    if (str.empty())
    {
        return false;
    }

    int nLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

    if (nLen<= 0) 
    {
        wideString = L"";
        return false;
    }

    vector<wchar_t> resultstring(nLen);

    ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &resultstring[0], nLen);
    
    wideString = wstring(&resultstring[0]);

    return true;
}


std::wstring StringConvert::AnsiToWide( const string& str )
{
    wstring wideStr;

    if (str.empty())
    {
        return wideStr;
    }

    if (!AnsiToWide(str, wideStr))
    {
        wideStr = L"";
    }

    return wideStr;

}
// 这里使用的是Windows API转换，
// 还有一种方法是使用Unicode组织提供的C代码：
//  http://www.unicode.org/Public/PROGRAMS/CVTUTF/ConvertUTF.h
//  http://www.unicode.org/Public/PROGRAMS/CVTUTF/ConvertUTF.c
// 该代码提供了UTF-32 UTF-16 UTF-8 三者间的互相转换
// CodeProject  提供了包装类：http://www.codeproject.com/KB/string/UtfConverter.aspx
bool StringConvert::Utf8ToWide(const string& utf8string, wstring& wideString)
{
    if (utf8string.empty())
    {
        wideString = L"";
        return true;
    }

    int widesize = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, NULL, 0);

    // Invalid UTF-8 sequence.
    if (ERROR_NO_UNICODE_TRANSLATION == widesize ||
        0 == widesize)
    {
        wideString = L"";
        return false;
    }

    vector<wchar_t> resultstring(widesize);

    int convresult = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, &resultstring[0], widesize);

    // Error in convert.
    if (convresult != widesize)
    {
        wideString = L"";
        return false;
    }

    wideString = std::wstring(&resultstring[0]);
    return true;
}

std::wstring StringConvert::Utf8ToWide( const string& utf8string )
{
    wstring wideStr;
    if (utf8string.empty())
    {
        return wideStr;
    }

    if (!Utf8ToWide(utf8string, wideStr))
    {
        wideStr = L"";
    }

    return wideStr;
}

bool StringConvert::WideToUtf8( const wstring& widestring, string& utf8String )
{
    if (widestring.empty())
    {
        utf8String = "";
        return true;
    }

    int utf8size = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, NULL, 0, NULL, NULL);
    if (utf8size == 0)
    {
        utf8String = "";
        return false;
    }

    vector<char> resultstring(utf8size);

    int convresult = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, &resultstring[0], utf8size, NULL, NULL);

    if (convresult != utf8size)
    {
        utf8String = "";
        return false;
    }

    utf8String = string(&resultstring[0]);
    return true;
}


std::string StringConvert::WideToUtf8( const wstring& widestring )
{
    if (widestring.empty())
    {
        return string("");
    }

    string str;
    if (!WideToUtf8(widestring, str))
    {
        str = "";
    }

    return str;
}

std::wstring StringConvert::Replace( const wstring& originalStr, const wstring& oldStr, const wstring& newStr )
{
    if (originalStr.empty())
    {
        return wstring(L"");
    }

    size_t pos = 0; 
    wstring tempStr = originalStr; 
    wstring::size_type newStrLen = newStr.length(); 
    wstring::size_type oldStrLen = oldStr.length(); 
    while(true) 
    { 
        pos = tempStr.find(oldStr, pos); 
        if (pos == wstring::npos) 
        {
            break;
        }

        tempStr.replace(pos, oldStrLen, newStr);         
        pos += newStrLen;
    }

    return tempStr;
}

std::wstring StringConvert::ToUpper( const wstring& orignalStr )
{
    if (orignalStr.empty())
    {
        return wstring(L"");
    }

    wstring convertedStr = orignalStr;
    transform(convertedStr.begin(), convertedStr.end(), convertedStr.begin(), towupper);

    return convertedStr;
}

std::wstring StringConvert::ToLower( const wstring& orignalStr )
{
    if (orignalStr.empty())
    {
        return wstring(L"");
    }

    wstring convertedStr = orignalStr;
    transform(convertedStr.begin(), convertedStr.end(), convertedStr.begin(), towlower);

    return convertedStr;
}


std::wstring StringConvert::BoolToWString( bool value )
{
    if (value)
    {
        return L"True";
    }
    else
    {
        return L"False";
    }
}

bool StringConvert::WStringToBool( const wstring& wstr )
{
    wstring tempStr = StringConvert::ToLower(wstr);
    if (tempStr == L"true")
    {
        return true;
    }
    else
    {
        return false;
    }
}

wstring StringConvert::Format( wstring format, ... )
{
	if (format.empty())
	{
		return wstring(L"");
	}

	wstring retStr(L"");

	va_list marker = NULL; 

	// initialize variable arguments
	va_start(marker, format); 

	// Get formatted string length adding one for NULL
	size_t len = _vscwprintf(format.c_str(), marker) + 1;

	// Create a wchar vector to hold the formatted string.
	vector<wchar_t> buffer(len, '\0');
	int nWritten = _vsnwprintf_s(&buffer[0], buffer.size(), len, format.c_str(), marker);    

	if (nWritten > 0)
	{
		retStr = &buffer[0];
	}

	// Reset variable arguments
	va_end(marker); 

	return retStr; 
}

BSTR StringConvert::WStringToBSTR(const wstring& text)
{
	BSTR bsText = NULL;
	if (!text.empty())
    {
		bsText = ::SysAllocString(text.c_str());
    }

	return bsText;
}

wstring StringConvert::BSTRToWString(const BSTR bsText)
{
	USES_CONVERSION;
	wstring value = L"";
	if (::SysStringLen(bsText) > 0)
    {
		value = OLE2W(bsText);
    }

	return value;
}

vector<wstring> StringConvert::Split(const wstring& str, const wstring& delimiter /*= L""*/, int maxSplit /*= -1*/)
{
	vector<wstring> strVector;

    if (str.empty())
    {
        return strVector;
    }

	size_t offsize = 0;
	size_t count = 0;

	while (offsize < str.size())
	{
		size_t startIndex = str.find(delimiter, offsize);

		if (startIndex == std::wstring::npos)
		{
			break;
		}

		size_t subStrCount = startIndex - offsize;
		wstring subStr = str.substr(offsize, subStrCount);
		
		subStr = StringConvert::Trim(subStr);

		offsize = offsize + subStrCount + delimiter.size();

		if (!subStr.empty())
		{
			strVector.push_back(subStr);
			count++;
			if (count == static_cast<size_t>(maxSplit - 1))
			{
				break;
			}
		}
	}

	wstring leftStr = str.substr(offsize);
	leftStr = StringConvert::Trim(leftStr);
	if (!leftStr.empty())
		strVector.push_back(leftStr);

	return strVector;
}

wstring StringConvert::Trim(const wstring& str)
{
	if (str.empty())
	{
		return wstring(L"");
	}

	wstring strResult = TrimLeft(str);

	strResult = TrimRight(strResult);

	return strResult;
	
}

wstring StringConvert::TrimLeft( const wstring& str )
{
	if (str.empty())
	{
		return wstring(L"");
	}

	wstring strResult = str;

	bool endLeftTrim = false;

	while (!endLeftTrim)
	{
		wstring subStr = strResult.substr(0, 1);
		if (subStr == L" ")
		{
			strResult = strResult.substr(1);
		}
		else
		{
			endLeftTrim = true;
		}
	}
	return strResult;
}

wstring StringConvert::TrimRight( const wstring& str )
{
	if (str.empty())
	{
		return wstring(L"");
	}

	wstring strResult = str;

	bool endRightTrim = false;

	while (!endRightTrim)
	{
		wstring subStr = strResult.substr(strResult.size() - 1, 1);
		if (subStr == L" ")
		{
			strResult = strResult.substr(0, strResult.size() - 1);
		}
		else
		{
			endRightTrim = true;
		}
	}

	return strResult;
}

std::string StringConvert::Format( string format, ... )
{
    if (format.empty())
    {
        return string("");
    }

    string formattedStr("");

    va_list argsList = NULL; 

    // initialize variable arguments
    va_start(argsList, format); 

    // Get formatted string length adding one for NULL
    size_t formattedStrLen = _vscprintf(format.c_str(), argsList) + 1;

    // Create a char vector to hold the formatted string.
    vector<char> buffer(formattedStrLen, '\0');

    int nWritten = _vsnprintf_s(&buffer[0], buffer.size(), formattedStrLen, format.c_str(), argsList);    

    if (nWritten > 0)
    {
        formattedStr = &buffer[0];
    }

    // Reset variable arguments
    va_end(argsList); 

    return formattedStr; 
}

std::string StringConvert::Utf8ToAnsi( const string& utf8Str )
{
    wstring unicodeStr = Utf8ToWide(utf8Str);
    string ansistr = WideToAnsi(unicodeStr);

    return ansistr;
}
std::string StringConvert::AnsiToUtf8( const string& ansiStr )
{
    wstring wideStr = AnsiToWide(ansiStr);
    string utf8Str = WideToUtf8(wideStr);

    return utf8Str;
}

std::string StringConvert::ToUpper( const string& orignalStr )
{
    if (orignalStr.empty())
    {
        return "";
    }

    string convertedStr = orignalStr;
    transform(convertedStr.begin(), convertedStr.end(), convertedStr.begin(), toupper);

    return convertedStr;
}

std::string StringConvert::ToLower( const string& orignalStr )
{
    if (orignalStr.empty())
    {
        return "";
    }

    string convertedStr = orignalStr;
    transform(convertedStr.begin(), convertedStr.end(), convertedStr.begin(), tolower);

    return convertedStr;
}

//此处主要用于统计'\n'个数, 并且每行字符数有限制
int StringConvert::CountChar(wstring& str, const wchar_t& ch) 
{
	if (str.empty())
	{
		return 0;
	}

	str = Trim(str);
	if(str[str.length() - 1] != '\n')
	{
		str += L"\n";
	}

	wstring strTmp = str;
	int idx = 0;
	int nChar = 0;
	int count = 0;
	for(const wchar_t* p = str.c_str(); p && *p; p ++)
	{
		++ idx; //字符下标
		++ nChar;
		if(nChar > 100) //操过每行最大字符数，增加一个换行
		{
			strTmp.insert(idx, L"\n", 1);
			++ count;
            nChar = 0;
		}

		if(*p == ch)
		{
			++ count; //本身是换行符+1 
		}
	}

	str = strTmp;
	return count;
}

bool StringConvert::isNum(string s)
{
	stringstream sin(s);  
	int t;  
	char p;  

	/*解释： 
	sin>>t表示把sin转换成double的变量（其实对于int和float型的都会接收），如果转换成功，则值为非0，如果转换不成功就返回为0 
	*/  
	if(!(sin >> t))  
		return false; 

	/*解释：此部分用于检测错误输入中，数字加字符串的输入形式（例如：34.f），在上面的的部分（sin>>t）已经接收并转换了输入的数字部分，在stringstream中相应也会把那一部分给清除，如果此时传入字符串是数字加字符串的输入形式，则此部分可以识别并接收字符部分，例如上面所说的，接收的是.f这部分，所以条件成立，返回false;如果剩下的部分不是字符，那么则sin>>p就为0,则进行到下一步else里面 
		*/ 
	if(sin >> p)  
		return false;  
	else  
		return true;  
}

BOOL StringConvert::StringToClipBoard(const char* srcString)
{
	BOOL bResult = FALSE;
	DWORD dwLength = strlen(srcString);
	HANDLE hGlobalMemory = GlobalAlloc(GHND, dwLength + 1); // 分配内存
	LPBYTE lpGlobalMemory = (LPBYTE)GlobalLock(hGlobalMemory); // 锁定内存
	if ( NULL != lpGlobalMemory )
	{
		strcpy_s((char*)lpGlobalMemory, dwLength, srcString);
		::GlobalUnlock(hGlobalMemory); // 锁定内存块解锁
		::OpenClipboard(NULL); // 打开剪贴板
		::EmptyClipboard(); // 清空剪贴板
		::SetClipboardData(CF_TEXT, hGlobalMemory); // 将内存中的数据放置到剪贴板
		::CloseClipboard();

		bResult = TRUE;
	}
	return bResult;
}