#include "MyConvert.h"
#include "pch.h"
#include <string>

bool UTF8ToGBK(const std::string& strUTF8, std::wstring& strGBK)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
	TCHAR* wszGBK = new TCHAR[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, wszGBK, len);
	strGBK = std::wstring(wszGBK);
	delete[]wszGBK;
	return true;
}