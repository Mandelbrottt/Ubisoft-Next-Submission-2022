#include <Windows.h>

int
APIENTRY
wWinMain(
	_In_ HINSTANCE     hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR        lpCmdLine,
	_In_ int           nCmdShow
);

int
wmain()
{
	HINSTANCE hInstance     = GetModuleHandleW(NULL);
	HINSTANCE hPrevInstance = 0;
	LPWSTR    lpCmdLine     = GetCommandLineW();
	int       nShowCmd      = 0;

	// TODO: Allocate a console
	
	// HACK: Call NextAPIs wWinMain() as a workaround to let us bootstrap NextAPI
	return wWinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}
