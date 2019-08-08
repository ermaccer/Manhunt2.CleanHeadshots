// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "MemoryMgr.h"
#include "mh2.h"

using namespace Memory::VP;

int explodeHead_ebx;
int explodeHead_unk;
int explodeHead_true = 0x5097FC;
int explodeHead_false = 0x5097C1;

void __declspec(naked) HookExtraWeaponCheck() {
	_asm {
		mov explodeHead_ebx, ebx
		mov explodeHead_unk, eax
		pushad
	}
	if (explodeHead_ebx == CT_SAWNOFF || explodeHead_ebx == CT_SHOTGUN || explodeHead_ebx == CT_SHOTGUN_TORCH)
	{
		if (explodeHead_unk != *(char*)0x69939C)
		{
			// decrease headshot distance for shotguns
			Patch<float>(0x6991C0, 10.0);
			_asm
			{
				popad
				jmp explodeHead_true
			}
		}

	}
	else
	{
		// increase headshot distance for everything else
		Patch<float>(0x6991C0, 100.0);
		_asm {
			popad
			jmp  explodeHead_false
		}
	}

}



void Init()
{
	Patch<char>(0x5097F1, 0xE9);
	Patch<int>(0x5097F1 + 1, (int)HookExtraWeaponCheck - ((int)0x5097F1 + 5));

}

extern "C"
{
	__declspec(dllexport) void InitializeASI()
	{
		Init();
	}
}