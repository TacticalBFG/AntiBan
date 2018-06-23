// AntiBan.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"


// NOTE: If you're trying to include this into your project and getting errors, it's because your project's character set is incorrect.
// To fix this: go to Project > Properties > General > Character Set > select "Use Multi-Byte character set". This will screw with your widechars, so just modify them to work /shrug
#define log_addy_start 0xB838D
int WINAPI h_MessageBox(
	_In_opt_ HWND    hWnd,
	_In_opt_ LPCTSTR lpText,
	_In_opt_ LPCTSTR lpCaption,
	_In_     UINT    uType
) {
	std::string str_lp = lpText;
	std::string str_cap = lpCaption;
	if (str_cap == "Roblox Crash") {
		DWORD log_start = log_addy_start + (DWORD)GetModuleHandleA(NULL);
		DWORD old;
		for (int i = 0; i < 79; i++) {
			VirtualProtect((LPVOID)(log_start + i), 1, PAGE_EXECUTE_READWRITE, &old);
			*(char*)(log_start + i) = 0x90;
			VirtualProtect((LPVOID)(log_start + i), 1, old, &old);
		}
		str_lp = "Roblox has crashed and AntiBan has caught it. Your logs will not be uploaded.\nWritten by Tactical BFG"; // Crash message. You can change this if you want.
	}

	wchar_t* str_lp_res = new wchar_t[4096];
	wchar_t* str_cap_res = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, NULL, str_lp.c_str(), -1, str_lp_res, 4096);
	MultiByteToWideChar(CP_ACP, NULL, str_cap.c_str(), -1, str_cap_res, 4096);
	return MessageBoxW(hWnd, str_lp_res, str_cap_res, uType);
}


void InitUploadCheckHook() {
	DWORD o;
	VirtualProtect((LPVOID)&MessageBoxA, 1, PAGE_EXECUTE_READWRITE, &o);
	*(char*)(&MessageBoxA) = 0xE9;
	*(DWORD*)((DWORD)&MessageBoxA + 1) = ((DWORD)&h_MessageBox - (DWORD)&MessageBoxA) - 5;
	VirtualProtect((LPVOID)&MessageBoxA, 1, o, &o);
}

// Credit to sloppey/louka for making this. All i'm really doing is just compiling and providing a sourced example for people who don't understand how this works.
// https://github.com/sloppey/Roblox-Disable-Logs
int main() {
	InitUploadCheckHook(); // Start the hook to MessageBoxA.

	MessageBoxA(0, "AntiBan has been loaded. Written by Tactical BFG", "AntiBan Message", 0); // Test MessageBox hook and alert the user it's been loaded.
}