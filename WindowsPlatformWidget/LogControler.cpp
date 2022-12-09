#include "LogControler.h"

LogFileHandle OpenLog(std::string filename, LogType type)
{
	LogFileHandle LogPtr;
	if (type != ALL_LOGS && type != ONLY_ERR && type != NO_LOG)
		return LogPtr;
	if (type == NO_LOG)
		LogPtr.LogType = type; return LogPtr;
	LogPtr.LogStream.open(filename, std::ios::out | std::ios::binary | std::ios::app);
	if (LogPtr.LogStream.is_open())
	{
		LogPtr.LogType = type;
		LogPtr.NoError = true;
		LogPtr.LogStream.write("\n-----The location where the log was last opened-----\n", 54);
		LogPtr.LogStream.flush();
		return LogPtr;
	}
	LogPtr.LogType = NO_LOG;
	return LogPtr;
}

void CloseLog(LogFileHandle& LogPtr)
{
	if (LogPtr.LogStream.is_open())
	{
		if (LogPtr.NoError)
		{
			LogPtr.LogStream.write("[REPORT] No errors occurred during this use.\n", 45);
			LogPtr.LogStream.flush();
		}
		LogPtr.LogStream.close();
		LogPtr.LogType = NO_LOG;
	}
}

bool WriteLog(LogFileHandle& LogPtr, const std::string& ErrorMessage, bool error)
{
	switch (LogPtr.LogType)
	{
	case ALL_LOGS:
		break;
	case ONLY_ERR:
		if (!error)
			return false;
		break;
	case NO_LOG:
		return false;
		break;
	default:
		return false;
		break;
	}
	if (LogPtr.LogStream.is_open())
	{
		if (error)
		{
			LogPtr.NoError = false;
			LogPtr.LogStream.write("[ERROR] ", 8);
		}
		else
			LogPtr.LogStream.write("[INFO] ", 7);
		LogPtr.LogStream.write(ErrorMessage.c_str(), ErrorMessage.length());
		LogPtr.LogStream.write("\n", 1);
		LogPtr.LogStream.flush();
		return true;
	}
	return false;
}