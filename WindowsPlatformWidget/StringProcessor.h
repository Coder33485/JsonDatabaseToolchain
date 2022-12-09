#pragma once

#ifndef STRINGPROC_H
#define STRINGPROC_H

#include <string>
#include <Windows.h>

_declspec(dllexport) bool WstringToString(const std::wstring Wstring, std::string& String);
_declspec(dllexport) bool StringToWstring(const std::string String, std::wstring& Wstring);

#endif // !STRINGPROC_H
