#include <cstdio>

#include <Windows.h>
#include <string>

int
APIENTRY
NextAPI_wWinMain(
	_In_ HINSTANCE     hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR        lpCmdLine,
	_In_ int           nShowCmd
);

int
APIENTRY
wWinMain(
	_In_ HINSTANCE     hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR        lpCmdLine,
	_In_ int           nShowCmd
)
{
	const char* file_out = "CONOUT$";

#ifndef NEXT_RELEASE
	if (!AttachConsole(ATTACH_PARENT_PROCESS))
        AllocConsole();
#else
	file_out = "log.txt";
#endif
	
	FILE* dummy;
	
    freopen_s(&dummy, file_out, "w", stdout);
    freopen_s(&dummy, file_out, "w", stderr);

	// TODO: Find easy way to mimic posix dlsym on windows to have duplicate declarations of wWinMain in different libs
	int result = NextAPI_wWinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);

#ifndef NEXT_RELEASE
	// Not entirely necessary, but nice to do anyway
	FreeConsole();
#endif
	
	return result;
}
