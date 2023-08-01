#include <lazy_importer.hpp>
#include <Windows.h>
#pragma comment(linker, "/merge:.data=.text")
#pragma comment(linker, "/merge:.rdata=.text")
#pragma comment(linker, "/merge:.pdata=.text")
#pragma comment(linker, "/align:512")

#define LOAD_FUNCTION(f) auto Loaded_##f = LI_FN(f);
#define LOAD_FUNCTION_IN(f,m) auto Loaded_##f = LI_FN(f).in_safe_cached(m);

VOID NTAPI
main(
	PVOID DllHandle,
	DWORD Reason,
	PVOID Reserved
)
{
	CHAR UserDllString[] = "User32.dll";

	LOAD_FUNCTION(LoadLibraryA);
	LOAD_FUNCTION(VirtualAlloc);

	auto hUser = Loaded_LoadLibraryA(UserDllString);

	LOAD_FUNCTION_IN(MessageBoxA, hUser);

	auto buffer = Loaded_VirtualAlloc((PVOID)0x190000000, 90000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (buffer == (PVOID)0x190000000) Loaded_MessageBoxA(nullptr, UserDllString, UserDllString, MB_OK);
}