// pch.cpp: source file corresponding to the pre-compiled header
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "Patchmemory.h"
#include <string>
//49E3A0
int blockrec;
int blocksend;
std::list<int> _blockPacketListRec;
std::list<int> _blockPacketListSent;

DWORD InputCall_I = 0x4787B0; // asm pointer

typedef void(__fastcall* GamePrint)(wchar_t* wMessage, int nColor);
GamePrint PrintIngame = (GamePrint)0x49E3A0; //func for printing ingame

typedef void(__stdcall* Net_Packet)(size_t aLen, DWORD arg1, BYTE* aPacket);
Net_Packet SendPacket_1 = (Net_Packet)0x52AE50; //for sending packet

typedef void(__stdcall* Net_Packet3)(size_t aLen, DWORD arg1, BYTE* aPacket);
Net_Packet3 SendPacket_2 = (Net_Packet3)0x52AE62; //for sending packet

typedef void(__fastcall* Net_Packet2)(BYTE* aPacket, DWORD aLen);
Net_Packet2 RecPacket_1 = (Net_Packet2)0x52AEB0; //for receiving pointer


typedef void(__fastcall* Net_Packet4)(BYTE* aPacket, DWORD aLen);
Net_Packet4 RecPacket_2 = (Net_Packet4)0x52B920;


int GetHexValue(char c)
{
	if (c >= '0' && c <= '9')
	{
		return c - '0';
	}

	if (c >= 'a' && c <= 'f')
	{
		return c - 'a' + 10;
	}

	if (c >= 'A' && c <= 'F')
	{
		return c - 'A' + 10;
	}

	return -1;
}
int GetHexSpan(const char* string)
{
	int i = 0;
	for (; string[i] != '\0' && GetHexValue(string[i]) >= 0; i++);
	return i;
}
int ConvertHexStringToBytes(const char* string, void* data, int length)
{
	unsigned char* bytes = static_cast<unsigned char*>(data);

	int span = GetHexSpan(string);
	int offset = span / 2 + (span & 1) - 1;
	int end = span - 1;

	if (offset >= length)
	{
		offset = length - 1;
		end = length * 2 - 1;
	}

	int count = offset + 1;

	for (int i = end, j = 0; i >= 0 && offset < length; i--, j++)
	{
		int value = GetHexValue(string[i]);

		if ((j & 1) != 0)
		{
			bytes[offset--] |= (value << 4);
		}

		else
		{
			bytes[offset] = value;
		}
	}

	return count;
}
void MyMultiByteToWideChar(UINT CodePage, DWORD dwFlags, LPCSTR lpMultiByteStr, INT cbMultiByte, LPWSTR lpWideCharStr, int cchWideChar)
{
	MultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);
	for (DWORD i = 0; i < wcslen(lpWideCharStr); i++)
	{
		if (lpWideCharStr[i] >= 0xFF && lpWideCharStr[i + 1] == L'c')
		{
			if (lpWideCharStr[i + 2] >= L'0' && lpWideCharStr[i + 2] <= L'9')
			{
				lpWideCharStr[i] = 0xFF;
			}
		}
	}
}
void PrintString(LPSTR lpFormat, ...)
{
	CHAR szOutput[0x400] = "";
	WCHAR wOutput[0x400] = L"";
	va_list Args;


	va_start(Args, lpFormat);
	vsprintf(szOutput + strlen(szOutput), lpFormat, Args);
	va_end(Args);

	MyMultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szOutput, (INT)strlen(szOutput), wOutput, (INT)strlen(szOutput));
	PrintIngame(wOutput, 0);
}
INT StringTokenize(CHAR* Input, CHAR Separator, LPSTR* TokenBuffer, INT MaxTokens)
{
	CHAR* P = Input;
	INT i = 0;

	do
	{
		TokenBuffer[i] = P;
		P = strchr(P, Separator);

		if (P)
			*(P++) = 0;
	}

	while (P && ++i < MaxTokens);
	return ++i;
}
BOOL __fastcall Input(WCHAR* Text)
{
	CHAR Buffer[100];
	CHAR Message[100];
	CHAR* Argument[532];
	WideCharToMultiByte(CP_ACP, 0, Text, -1, Buffer, sizeof(Buffer), 0, 0);
	strcpy(Message, Buffer);
	INT Arguments = StringTokenize(Buffer + 1, ' ', Argument, 532);
	if (Buffer[0] == '.')
	{
		if (!_stricmp(Argument[0], "Send"))
		{

			if (Argument[1])
			{


	
				unsigned char data[2048];
				int length = ConvertHexStringToBytes(Argument[1], data, sizeof(data));
				ConvertHexStringToBytes(Argument[1], data, length); 
				SendPacket_1(length, 1, data);
				PrintString(LPSTR("packet Sent"));
				PrintString(Argument[1]); // Print out Which packet we sent


			
			}
			return FALSE;

		}
			if (!_stricmp(Argument[0], "received"))
			{

				if (Argument[1])
				{

			
					unsigned char data[2048];
					int length = ConvertHexStringToBytes(Argument[1], data, sizeof(data));
					ConvertHexStringToBytes(Argument[1], data, length); // 
					PrintString(LPSTR("packet Received"));
					PrintString(Argument[1]); // Print out Which packet we sent
					RecPacket_1(data, length);
					
				}
				return FALSE;
			

			return FALSE;
		}

			if (!_stricmp(Argument[0], "blockr"))
			{

				if (Argument[1])
				{
			
					PrintString((LPSTR)"Blocked");
					PrintString(Argument[1]);
					blockrec = std::stoi(Argument[1], nullptr, 16);
					_blockPacketListRec.push_front(blockrec);

		



				}
				return FALSE;
			}
			if (!_stricmp(Argument[0], "blocks"))
			{
				//add to blocklist packet
				if (Argument[1])
				{
	

					PrintString((LPSTR)"Blocked");
					PrintString(Argument[1]);
					blocksend = std::stoi(Argument[1], nullptr, 16);
	
					_blockPacketListSent.push_front(blocksend);




				}
				return FALSE;
			}


			if (!_stricmp(Argument[0], "blocksclear"))
			{



				_blockPacketListSent.clear();
				PrintString((LPSTR)"Cleared Packet Sent block list");


				return FALSE;
			}
			if (!_stricmp(Argument[0], "blockrclear"))
			{



				_blockPacketListRec.clear();
				PrintString((LPSTR)"Cleared Packet Recive block list");


				return FALSE;
			}

	

	}
	return TRUE;
}
BOOL __fastcall PacketRec(BYTE* data, DWORD aLen)
{

	for (int n : _blockPacketListRec) { //Block packets in list



		if (data[0] == n)
		{

			return FALSE;
		}

	}



	return TRUE;
}
BOOL __fastcall PacketSend(BYTE* data, DWORD aLen)
{
	for (int n : _blockPacketListSent) {

		if (data[0] == n)
		{

			return FALSE;
		}

	}

	return TRUE;
}
void __declspec(naked) GameInput_Intercept()
{
	__asm
	{
		PUSHAD;
		MOV ECX, EBX;
		CALL Input;
		CMP EAX, 0;
		POPAD;
		JE Block;
		CALL InputCall_I;
		RET;
	Block:
		XOR EAX, EAX;
		RET;
	}
}

void __declspec(naked) GamePacketRec()//Rec
{
	__asm
	{
		pushad;

		call PacketRec;
		test eax, eax;

		popad;
		jnz OldCode;

		mov edx, 0;

	OldCode:
		mov eax, [esp + 4];
		push eax;
		call RecPacket_2;

		ret 4;
	}

}

void __declspec(naked) GamePacketSe()
{
	__asm
	{
		pushf
		pushad;

		mov ecx, [esp + 0x22 + 4 + 0xC];
		mov edx, [esp + 0x22 + 4 + 0x4];
		call PacketSend;


		test eax, eax;

		popad;
		jnz OldCode;

		mov[esp + 0x4 + 4 + 2], 0;

	OldCode:
		popf
			jnz good
			xor eax, eax
			pop ebp
			ret 0Ch
			good :
		jmp SendPacket_2
	}
}

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.
