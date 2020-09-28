#include "StdAfx.h"
#include "MiniDump.h"

#pragma comment(lib, "Dbghelp.lib")

LPTOP_LEVEL_EXCEPTION_FILTER g_PreviousExceptionFilter;

CMiniDump::CMiniDump(void)
{
}

CMiniDump::~CMiniDump(void)
{
}

BOOL CMiniDump::Begin()
{
	//使其不再出现错误运算窗口
	SetErrorMode(SEM_FAILCRITICALERRORS) ;

	//Unhandled Exception出现的时候，定义所要运行的Callback
	g_PreviousExceptionFilter = ::SetUnhandledExceptionFilter(&CMiniDump::UnHandledExceptionFilter);
	return TRUE;
}

BOOL CMiniDump::End()
{
	::SetUnhandledExceptionFilter(g_PreviousExceptionFilter) ;
	return TRUE;
}

LONG WINAPI CMiniDump::UnHandledExceptionFilter( struct _EXCEPTION_POINTERS *exceptionInfo )
{
	HMODULE DllHandle = NULL;


	TCHAR DumpPath[MAX_PATH] = {0,};

	SYSTEMTIME SystemTime ;

	//获取当前时间
	GetLocalTime(&SystemTime) ;
	//以当前时间为基准，创建Load文件
	_sntprintf (DumpPath, MAX_PATH, _T("%d-%d-%d %d_%d_%d.dmp"), SystemTime.wYear , SystemTime.wMonth, 
		SystemTime.wDay, SystemTime.wHour, SystemTime.wMinute , SystemTime.wSecond) ;

	//创建文件
	HANDLE FileHandle = CreateFile(DumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL) ;

	//文件创建成功的时候
	if (FileHandle != INVALID_HANDLE_VALUE)
	{
		_MINIDUMP_EXCEPTION_INFORMATION MiniDumpExceptionInfo;

		//保存MiniDump异常信息的结构体

		MiniDumpExceptionInfo.ThreadId = GetCurrentThreadId() ;
		MiniDumpExceptionInfo.ExceptionPointers = exceptionInfo;
		MiniDumpExceptionInfo.ClientPointers = NULL;

		//通过它创建Dump文件

		BOOL Success = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), 
			FileHandle, MiniDumpNormal, &MiniDumpExceptionInfo, NULL, NULL);

		if (Success)
		{
			//成功的时候
			CloseHandle(FileHandle) ;
			return EXCEPTION_EXECUTE_HANDLER;
		}
	}
	CloseHandle(FileHandle) ;
	return EXCEPTION_CONTINUE_SEARCH;
}
