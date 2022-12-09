#pragma once

#ifndef PIPECTRL_H
#define PIPECTRL_H

#include <string>
#include <Windows.h>

#define PIPE_BUFFER_SIZE 2048

_declspec(dllexport) bool GetFullPipeName(const std::string PipeName, std::string& FullPipeName);
_declspec(dllexport) bool GetFullPipeName(const std::wstring PipeName, std::wstring& FullPipeName);

_declspec(dllexport) bool IsPipeExist(const std::string FullPipeName);
_declspec(dllexport) bool IsPipeExist(const std::wstring FullPipeName);

_declspec(dllexport) HANDLE CreatePipe(const std::string FullPipeName);
_declspec(dllexport) HANDLE CreatePipe(const std::wstring FullPipeName);
_declspec(dllexport) bool OpenPipe(HANDLE hPipe);
_declspec(dllexport) bool ClosePipe(HANDLE hPipe);
_declspec(dllexport) HANDLE ConnectPipe(const std::string FullPipeName);
_declspec(dllexport) HANDLE ConnectPipe(const std::wstring FullPipeName);
_declspec(dllexport) bool DestroyPipe(HANDLE hPipe);

_declspec(dllexport) DWORD WritePipe(HANDLE hPipe, const std::string Message);
_declspec(dllexport) DWORD ReadPipe(HANDLE hPipe, std::string& Message);

#endif // !PIPECTRL_H
