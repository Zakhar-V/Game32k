#include "Base.hpp"
#include <Windows.h>

//----------------------------------------------------------------------------//
// Debug
//----------------------------------------------------------------------------//

#if USE_LOG	& 0x2
FILE* g_log = nullptr;
#endif

//----------------------------------------------------------------------------//
void LogMsg(const char* _msg, ...)
{
#if USE_LOG
	va_list _args;
	va_start(_args, _msg);

#	if USE_LOG & 0x2
	if(!g_log)
		g_log = fopen("log.txt", "w");
	if (g_log)
	{
		vfprintf(g_log, _msg, _args);
		fwrite("\n", 1, 1, g_log);
		fflush(g_log);
	}
	else
		MessageBox(0, "Couldn't create log.txt", "Warning", MB_OK | MB_ICONWARNING | MB_APPLMODAL | MB_TOPMOST);
#	endif

#	if (USE_LOG & 0x1) && defined(_DEBUG)
	vprintf(_msg, _args);
	printf("\n");
#	endif

	va_end(_args);
#endif
}
//----------------------------------------------------------------------------//
void Fatal(const char* _error)
{
	LOG("Fatal: %s", _error);
	MessageBoxA(0, _error, "Fatal error", MB_OK | MB_ICONERROR | MB_APPLMODAL | MB_TOPMOST);
	ExitProcess(-1);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
