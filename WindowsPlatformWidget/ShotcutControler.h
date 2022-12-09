#pragma once

#ifndef SHOTCUT_H
#define SHOTCUT_H

#include <Windows.h>
#include <ShlObj.h>

_declspec(dllexport) BOOL CreateShotCut(LPCWSTR lpFilePath, LPCWSTR lpShotcutPath, LPCWSTR lpWorkingDir, LPCWSTR lpIconPath, LPCWSTR lpDescription);
_declspec(dllexport) BOOL CreateShotCut(LPCSTR lpFilePath, LPCSTR lpShotcutPath, LPCSTR lpWorkingDir, LPCSTR lpIconPath, LPCSTR lpDescription);

#endif // !SHOTCUT_H
