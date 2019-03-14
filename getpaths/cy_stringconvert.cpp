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

    // ʹ��-1�� ���صĳ��Ȱ�������NULL�ַ�
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
// ����ʹ�õ���Windows APIת����
// ����һ�ַ�����ʹ��Unicode��֯�ṩ��C���룺
//  http://www.unicode.org/Public/PROGRAMS/CVTUTF/ConvertUTF.h
//  http://www.unicode.org/Public/PROGRAMS/CVTUTF/ConvertUTF.c
// �ô����ṩ��UTF-32 UTF-16 UTF-8 ���߼�Ļ���ת��
// CodeProject  �ṩ�˰�װ�ࣺhttp://www.codeproject.com/KB/string/UtfConverter.aspx
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

//�˴���Ҫ����ͳ��'\n'����, ����ÿ���ַ���������
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
		++ idx; //�ַ��±�
		++ nChar;
		if(nChar > 100) //�ٹ�ÿ������ַ���������һ������
		{
			strTmp.insert(idx, L"\n", 1);
			++ count;
            nChar = 0;
		}

		if(*p == ch)
		{
			++ count; //�����ǻ��з�+1 
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

	/*���ͣ� 
	sin>>t��ʾ��sinת����double�ı�������ʵ����int��float�͵Ķ�����գ������ת���ɹ�����ֵΪ��0�����ת�����ɹ��ͷ���Ϊ0 
	*/  
	if(!(sin >> t))  
		return false; 

	/*���ͣ��˲������ڼ����������У����ּ��ַ�����������ʽ�����磺34.f����������ĵĲ��֣�sin>>t���Ѿ����ղ�ת������������ֲ��֣���stringstream����ӦҲ�����һ���ָ�����������ʱ�����ַ��������ּ��ַ�����������ʽ����˲��ֿ���ʶ�𲢽����ַ����֣�����������˵�ģ����յ���.f�ⲿ�֣�������������������false;���ʣ�µĲ��ֲ����ַ�����ô��sin>>p��Ϊ0,����е���һ��else���� 
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
	HANDLE hGlobalMemory = GlobalAlloc(GHND, dwLength + 1); // �����ڴ�
	LPBYTE lpGlobalMemory = (LPBYTE)GlobalLock(hGlobalMemory); // �����ڴ�
	if ( NULL != lpGlobalMemory )
	{
		strcpy_s((char*)lpGlobalMemory, dwLength, srcString);
		::GlobalUnlock(hGlobalMemory); // �����ڴ�����
		::OpenClipboard(NULL); // �򿪼�����
		::EmptyClipboard(); // ��ռ�����
		::SetClipboardData(CF_TEXT, hGlobalMemory); // ���ڴ��е����ݷ��õ�������
		::CloseClipboard();

		bResult = TRUE;
	}
	return bResult;
}