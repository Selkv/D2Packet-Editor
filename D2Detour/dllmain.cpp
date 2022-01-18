// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Patchmemory.h"
#define _CRT_SECURE_NO_DEPRECATE

static HANDLE hD2Thread = INVALID_HANDLE_VALUE;

DWORD WINAPI D2Thread(LPVOID lpParam)
{

	InstallPatches();

    return TRUE;
}

int Startup()
{

	if ((hD2Thread = CreateThread(NULL, NULL, D2Thread, NULL, NULL, NULL)) == NULL)
		return FALSE;

}
BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID lpReserved)
{



	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
	{


		DisableThreadLibraryCalls(hDll);
		if (!Startup())
			return FALSE;

	}
	return TRUE;

	}
}
