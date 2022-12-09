#include "ShotcutControler.h"
#include "StringProcessor.h"

BOOL CreateShotCut(LPCWSTR lpFilePath, LPCWSTR lpShotcutPath, LPCWSTR lpWorkingDir, LPCWSTR lpIconPath, LPCWSTR lpDescription)
{
	CoInitialize(NULL);
	IShellLinkW* pShellLink = NULL;

	HRESULT hres;
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_ALL, IID_IShellLink, (void**)&pShellLink);

	if (SUCCEEDED(hres))
	{
		pShellLink->SetPath(lpFilePath);
		pShellLink->SetDescription(lpDescription);
		pShellLink->SetIconLocation(lpIconPath, 0);
		pShellLink->SetWorkingDirectory(lpWorkingDir);

		IPersistFile* pPersistFile;
		hres = pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile);

		if (SUCCEEDED(hres))
		{
			hres = pPersistFile->Save(lpShotcutPath, TRUE);
			pPersistFile->Release();
		}
		else
		{
			return FALSE;
		}

		pShellLink->Release();
	}
	else
	{
		return FALSE;
	}
	CoUninitialize();
	return TRUE;
}

BOOL CreateShotCut(LPCSTR lpFilePath, LPCSTR lpShotcutPath, LPCSTR lpWorkingDir, LPCSTR lpIconPath, LPCSTR lpDescription)
{
	std::wstring ShotcutPath;


	CoInitialize(NULL);
	IShellLinkA* pShellLink = NULL;

	HRESULT hres;
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_ALL, IID_IShellLink, (void**)&pShellLink);

	if (SUCCEEDED(hres))
	{
		pShellLink->SetPath(lpFilePath);
		pShellLink->SetDescription(lpDescription);
		pShellLink->SetIconLocation(lpIconPath, 0);
		pShellLink->SetWorkingDirectory(lpWorkingDir);

		IPersistFile* pPersistFile;
		hres = pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile);

		if (SUCCEEDED(hres))
		{
			hres = pPersistFile->Save(ShotcutPath.c_str(), TRUE);
			pPersistFile->Release();
		}
		else
		{
			return FALSE;
		}

		pShellLink->Release();
	}
	else
	{
		return FALSE;
	}
	CoUninitialize();
	return TRUE;
}
