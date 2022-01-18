#define _DEFINE_VARS
#define __D2PTRS_ONCE_
#include <Windows.h>
#include "pch.h"
#include "Patchmemory.h"
#undef __D2PTRS_H__
#define __D2PTRS_LIST_
#define INST_INT3	0xCC
#define INST_CALL	0xE8
#define INST_NOP	0x90
#define INST_JMP	0xE9
#define INST_RET	0xC3
#ifndef ArraySize
#define ArraySize(x) (sizeof((x)) / sizeof((x)[0]))
#endif


PatchHook Patches[] = {

	{PatchCall,	0x47C89D,	(DWORD)GameInput_Intercept,				5},//1.14d for message ingame
	{PatchCall,	0x45F802,	(DWORD)GamePacketRec,	5},//Updated 1.14d 
	{PatchJmp,  0x52AE5A,  (DWORD)GamePacketSe,	5},//Updated 1.14d 
};



void InstallPatches()
{


	for (int x = 0; x < ArraySize(Patches); x++)
	{
		Patches[x].bOldCode = new BYTE[Patches[x].dwLen];
		::ReadProcessMemory(GetCurrentProcess(), (void*)Patches[x].dwAddr, Patches[x].bOldCode, Patches[x].dwLen, NULL);
		Patches[x].pFunc(Patches[x].dwAddr, Patches[x].dwFunc, Patches[x].dwLen);
	}

}


BOOL WriteBytes(void* pAddr, void* pData, DWORD dwLen)
{
	DWORD dwOld;

	if (!VirtualProtect(pAddr, dwLen, PAGE_READWRITE, &dwOld))
		return FALSE;

	::memcpy(pAddr, pData, dwLen);
	return VirtualProtect(pAddr, dwLen, dwOld, &dwOld);
}

void FillBytes(void* pAddr, BYTE bFill, DWORD dwLen)
{
	BYTE* bCode = new BYTE[dwLen];
	::memset(bCode, bFill, dwLen);

	WriteBytes(pAddr, bCode, dwLen);

	delete[] bCode;
}

void InterceptLocalCode(BYTE bInst, DWORD pAddr, DWORD pFunc, DWORD dwLen)
{
	BYTE* bCode = new BYTE[dwLen];
	::memset(bCode, 0x90, dwLen);
	DWORD dwFunc = pFunc - (pAddr + 5);

	bCode[0] = bInst;
	*(DWORD*)&bCode[1] = dwFunc;
	WriteBytes((void*)pAddr, bCode, dwLen);

	delete[] bCode;
}

void PatchCall(DWORD dwAddr, DWORD dwFunc, DWORD dwLen)
{
	InterceptLocalCode(INST_CALL, dwAddr, dwFunc, dwLen);
}

void PatchJmp(DWORD dwAddr, DWORD dwFunc, DWORD dwLen)
{

	InterceptLocalCode(INST_JMP, dwAddr, dwFunc, dwLen);
}

void PatchBytes(DWORD dwAddr, DWORD dwValue, DWORD dwLen)
{
	BYTE* bCode = new BYTE[dwLen];
	::memset(bCode, (BYTE)dwValue, dwLen);

	WriteBytes((LPVOID)dwAddr, bCode, dwLen);

	delete[] bCode;
}

PatchHook* RetrievePatchHooks(PINT pBuffer)
{
	*pBuffer = ArraySize(Patches);
	return &Patches[0];
}
