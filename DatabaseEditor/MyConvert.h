#pragma once

#ifndef MYCONVERT_H
#define MYCONVERT_H

#include <string>

bool UTF8ToGBK(const std::string& strUTF8, std::wstring& strGBK);

#endif // !MYCONVERT_H
