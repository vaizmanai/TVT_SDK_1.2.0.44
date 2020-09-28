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
	//ʹ�䲻�ٳ��ִ������㴰��
	SetErrorMode(SEM_FAILCRITICALERRORS) ;

	//Unhandled Exception���ֵ�ʱ�򣬶�����Ҫ���е�Callback
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

	//��ȡ��ǰʱ��
	GetLocalTime(&SystemTime) ;
	//�Ե�ǰʱ��Ϊ��׼������Load�ļ�
	_sntprintf (DumpPath, MAX_PATH, _T("%d-%d-%d %d_%d_%d.dmp"), SystemTime.wYear , SystemTime.wMonth, 
		SystemTime.wDay, SystemTime.wHour, SystemTime.wMinute , SystemTime.wSecond) ;

	//�����ļ�
	HANDLE FileHandle = CreateFile(DumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL) ;

	//�ļ������ɹ���ʱ��
	if (FileHandle != INVALID_HANDLE_VALUE)
	{
		_MINIDUMP_EXCEPTION_INFORMATION MiniDumpExceptionInfo;

		//����MiniDump�쳣��Ϣ�Ľṹ��

		MiniDumpExceptionInfo.ThreadId = GetCurrentThreadId() ;
		MiniDumpExceptionInfo.ExceptionPointers = exceptionInfo;
		MiniDumpExceptionInfo.ClientPointers = NULL;

		//ͨ��������Dump�ļ�

		BOOL Success = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), 
			FileHandle, MiniDumpNormal, &MiniDumpExceptionInfo, NULL, NULL);

		if (Success)
		{
			//�ɹ���ʱ��
			CloseHandle(FileHandle) ;
			return EXCEPTION_EXECUTE_HANDLER;
		}
	}
	CloseHandle(FileHandle) ;
	return EXCEPTION_CONTINUE_SEARCH;
}
