#include "PipeControler.h"

std::string GetFullPipeName(const std::string PipeName)
{
	return "\\\\.\\pipe\\WPW_Coder33485\\" + PipeName;
}

std::wstring GetFullPipeName(const std::wstring PipeName)
{
	return L"\\\\.\\pipe\\WPW_Coder33485\\" + PipeName;
}

bool IsPipeExist(const std::string FullPipeName)
{
	if (WaitNamedPipeA(FullPipeName.c_str(), NMPWAIT_NOWAIT))
	{
		return true;
	}
	return false;
}

bool IsPipeExist(const std::wstring FullPipeName)
{
	if (WaitNamedPipeW(FullPipeName.c_str(), NMPWAIT_NOWAIT))
	{
		return true;
	}
	return false;
}

HANDLE CreatePipe(const std::string FullPipeName)
{
	HANDLE hPipe;
	hPipe = CreateNamedPipeA(FullPipeName.c_str(), PIPE_ACCESS_DUPLEX | FILE_FLAG_FIRST_PIPE_INSTANCE, PIPE_WAIT | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE, 1, 1024, 1024, 0, nullptr);
	if (hPipe != INVALID_HANDLE_VALUE)
	{
		return hPipe;
	}
	return nullptr;
}

HANDLE CreatePipe(const std::wstring FullPipeName)
{
	HANDLE hPipe;
	hPipe = CreateNamedPipeW(FullPipeName.c_str(), PIPE_ACCESS_DUPLEX | FILE_FLAG_FIRST_PIPE_INSTANCE, PIPE_WAIT | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE, 1, 1024, 1024, 0, nullptr);
	if (hPipe != INVALID_HANDLE_VALUE)
	{
		return hPipe;
	}
	return nullptr;
}

bool OpenPipe(HANDLE hPipe)
{
	if (ConnectNamedPipe(hPipe, nullptr))
	{
		return true;
	}
	return false;
}

bool ClosePipe(HANDLE hPipe)
{
	if (DisconnectNamedPipe(hPipe))
	{
		return true;
	}
	return false;
}

HANDLE ConnectPipe(const std::string FullPipeName)
{
	HANDLE hPipe = CreateFileA(FullPipeName.c_str(), GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (hPipe != INVALID_HANDLE_VALUE)
	{
		return hPipe;
	}
	return nullptr;
}

HANDLE ConnectPipe(const std::wstring FullPipeName)
{
	HANDLE hPipe = CreateFileW(FullPipeName.c_str(), GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (hPipe != INVALID_HANDLE_VALUE)
	{
		return hPipe;
	}
	return nullptr;
}

bool DestroyPipe(HANDLE hPipe)
{
	if (CloseHandle(hPipe))
	{
		return true;
	}
	return false;
}

DWORD WritePipe(HANDLE hPipe, const std::string Message)
{
	if (Message.size() > PIPE_BUFFER_SIZE)
	{
		return 0;
	}
	DWORD Size = 0;
	if (WriteFile(hPipe, Message.c_str(), Message.size(), &Size, nullptr))
	{
		return Size;
	}
	return 0;
}

DWORD ReadPipe(HANDLE hPipe, std::string& Message)
{
	DWORD Size = 0;
	char Buffer[PIPE_BUFFER_SIZE] = { 0 };
	if (ReadFile(hPipe, Buffer, PIPE_BUFFER_SIZE, &Size, nullptr))
	{
		Message.clear();
		Message = Buffer;
		return Size;
	}
	return 0;
}
