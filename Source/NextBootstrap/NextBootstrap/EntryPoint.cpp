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

struct RaiiConsole
{
	explicit
	RaiiConsole(bool a_allocateConsole)
		: m_allocateConsole(a_allocateConsole)
	{
		const char* file_out = "CONOUT$";

		if (a_allocateConsole)
		{
			if (!AttachConsole(ATTACH_PARENT_PROCESS))
			{
				AllocConsole();
			}
		} else
		{
			file_out = "log.txt";
		}

		FILE* dummy;

		freopen_s(&dummy, file_out, "w", stdout);
		freopen_s(&dummy, file_out, "w", stderr);
	}

	~RaiiConsole()
	{
		// Not entirely necessary, but nice to do anyway
		FreeConsole();
	}

private:
	bool m_allocateConsole;
};

int
APIENTRY
wWinMain(
	_In_ HINSTANCE     hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR        lpCmdLine,
	_In_ int           nShowCmd
)
{
	std::wstring commandLine = lpCmdLine;

	bool doAllocateConsole =
	#ifdef NEXT_RELEASE
		// There's definitely better ways to detect command line args, but i'm lazy and this works
		commandLine.find(L"-c") != std::wstring::npos ||
		commandLine.find(L"--console") != std::wstring::npos;
	#else
		true;
	#endif

	RaiiConsole raiiConsole { doAllocateConsole };

	// TODO: Find easy way to mimic posix dlsym on windows to have duplicate declarations of wWinMain in different libs
	int result = NextAPI_wWinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);

	return result;
}
