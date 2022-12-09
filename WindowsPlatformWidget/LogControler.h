#pragma once

#ifndef MYERROR_H
#define MYERROR_H

#include <string>
#include <fstream>

typedef enum LogType
{
	ALL_LOGS,
	ONLY_ERR,
	NO_LOG
} LogType;

typedef struct LogFileHandle
{
	std::fstream LogStream;
	LogType LogType;
	bool NoError;
} LogFileHandle;

_declspec(dllexport) LogFileHandle OpenLog(std::string filename, LogType type = ALL_LOGS);

_declspec(dllexport) void CloseLog(LogFileHandle& LogPtr);

_declspec(dllexport) bool WriteLog(LogFileHandle& LogPtr, const std::string& ErrorMessage, bool error = false);

#endif // !ERROR_H
