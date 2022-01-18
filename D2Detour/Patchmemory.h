#include "pch.h"

PatchHook* RetrievePatchHooks(PINT pBuffer);
void PatchBytes(DWORD dwAddr, DWORD dwValue, DWORD dwLen);
void PatchJmp(DWORD dwAddr, DWORD dwFunc, DWORD dwLen);
void PatchCall(DWORD dwAddr, DWORD dwFunc, DWORD dwLen);
void InterceptLocalCode(BYTE bInst, DWORD pAddr, DWORD pFunc, DWORD dwLen);
void FillBytes(void* pAddr, BYTE bFill, DWORD dwLen);
BOOL WriteBytes(void* pAddr, void* pData, DWORD dwLen);
void InstallPatches();
