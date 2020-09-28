#pragma once

#include <DbgHelp.h>

class CMiniDump
{
public:
	CMiniDump(void);
	~CMiniDump(void);

	static BOOL Begin();
	static BOOL End();

	static LONG WINAPI UnHandledExceptionFilter(struct _EXCEPTION_POINTERS *exceptionInfo);
};
