#include "StringProcessor.h"

bool WstringToString(const std::wstring Wstring, std::string& String)
{
	if (Wstring.empty())
		return false;
	int len = WideCharToMultiByte(CP_UTF8, 0, Wstring.c_str(), Wstring.size(), nullptr, 0, nullptr, nullptr);
	char* buffer = new char[len + 1];
	memset(buffer, 0, sizeof(char) * (len + 1));
	WideCharToMultiByte(CP_UTF8, 0, Wstring.c_str(), Wstring.size(), buffer, len, nullptr, nullptr);
	String = buffer;
	delete[] buffer;
	return true;
}

bool StringToWstring(const std::string String, std::wstring& Wstring)
{
	if (String.empty())
		return false;
	DWORD len = MultiByteToWideChar(CP_ACP, 0, String.c_str(), -1, nullptr, 0);
	wchar_t* wbuffer = new wchar_t[len + 1];
	memset(wbuffer, 0, sizeof(wchar_t) * (len + 1));
	MultiByteToWideChar(CP_UTF8, 0, String.c_str(), -1, wbuffer, len);
	Wstring = wbuffer;
	delete[] wbuffer;
	return true;
}
