#include "StringProcessor.h"

void WstringToString(const std::wstring Wstring, std::string& String)
{
	String.clear();
	if (Wstring.empty())
		return;
	String.clear();
	int len = WideCharToMultiByte(CP_UTF8, 0, Wstring.c_str(), Wstring.size(), nullptr, 0, nullptr, nullptr);
	char* buffer = new char[len + 1];
	memset(buffer, 0, sizeof(char) * (len + 1));
	WideCharToMultiByte(CP_UTF8, 0, Wstring.c_str(), Wstring.size(), buffer, len, nullptr, nullptr);
	String = buffer;
	delete[] buffer;
	return;
}

void StringToWstring(const std::string String, std::wstring& Wstring)
{
	Wstring.clear();
	if (String.empty())
		return;
	DWORD len = MultiByteToWideChar(CP_ACP, 0, String.c_str(), -1, nullptr, 0);
	wchar_t* wbuffer = new wchar_t[len + 1];
	memset(wbuffer, 0, sizeof(wchar_t) * (len + 1));
	MultiByteToWideChar(CP_UTF8, 0, String.c_str(), -1, wbuffer, len);
	Wstring = wbuffer;
	delete[] wbuffer;
	return;
}
