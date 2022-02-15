#include "pch.h"

#include <Windows.h>

#include "MinimalInclude.h"

int
APIENTRY
wWinMain(
	_In_ HINSTANCE     hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR        lpCmdLine,
	_In_ int           nCmdShow
);

int
main()
{
	HINSTANCE hInstance     = GetModuleHandleW(NULL);
	HINSTANCE hPrevInstance = 0;
	LPWSTR    lpCmdLine     = GetCommandLineW();
	int       nShowCmd      = 0;

	// HACK: Call NextAPIs wWinMain() as a workaround to let us bootstrap NextAPI
	return wWinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}
