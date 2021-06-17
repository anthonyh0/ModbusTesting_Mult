// SysLogs.cpp: implementation of the CSysLogs class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "SysLogs.h"
#include <time.h>
#include <crtdbg.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSysLogs g_SysLog;

CSysLogs::CSysLogs()
{
  
}

CSysLogs::~CSysLogs()
{

}

bool CSysLogs::SetFileName(TCHAR *FileName)
{
	CFileStatus     fs;
	CFileException  e;
	CStdioFile      m_CFile;

	if (m_File != NULL)
	{
		fclose(m_File);
	}
	/*
	//we wanna save the logfile the same place as this process is...
	TCHAR file[MAX_PATH];
	int r = GetModuleFileName(NULL, file, MAX_PATH);
	_ASSERTE(r != 0);
	if (r == 0)
	{
		return false;
	}
	//now we have the filename and path, let's remove the filename so we only have the path...
	TCHAR *mid = file + r;
	while (*(--mid) != '\\' && mid > file);
	if (mid <= file)
	{
		return false;
	}
	*(++mid) = '\0';
	_tcscpy(m_FileName, file);
    _tcscpy(m_FileName, "\\LOGS\\");
	_tcscat(m_FileName, FileName);
	*/
	_tcscpy(m_FileName, FileName);

	//创建路径
	if (!CFile::GetStatus("D:\\ChipCoding",fs))
	{
		CreateDirectory("D:\\ChipCoding",0);   
	}
	
	// 创建日志文件	
    if(!CFile::GetStatus(m_FileName,fs))
	{
		if (m_CFile.Open(m_FileName, CFile::modeCreate, &e))
		{
			m_CFile.Close();
		} 
	}

	//new and open the logfile...
	m_File = _tfopen(m_FileName, _T("a+"));
	_ASSERTE(m_File != NULL);
	if (m_File == NULL)
	{
		return false;
	}
    fclose(m_File);

	return true;
}

void CSysLogs::SetSourceFileName(char *filename)
{
	//strip the path from the filename...
	char *mid = filename + strlen(filename);
	while (mid > filename)
	{
		if (*(--mid) == '\\')
		{
			mid++;
			break;
		}
	}
	//store the filename...
	strcpy(m_SourceFile, mid);
}

void CSysLogs::SetLogLevel(CSysLogLevels LogLevel)
{
	m_LogLevel = LogLevel;
}

void CSysLogs::LogNow(TCHAR *LoglevelName, TCHAR *LogString)
{
	m_File = _tfopen(m_FileName, _T("a+"));

	if (m_File == NULL)
	{
		_ASSERTE(!"Filename is not set...");
		return;
	}
  
	//get the current date and time, and format it to the format we wanna use...
	time_t now;
	time(&now);
	struct tm *tmnow = localtime(&now);
	char strnow[25];
	strftime(strnow, 24, "%Y-%m-%d %H:%M:%S", tmnow);

#ifdef _UNICODE
	if (m_LogLevel == levelDeveloperInfo)
	{
		fprintf(m_File, "%s\t%S\t%s, %d\t%S\n", strnow, LoglevelName, m_SourceFile, m_LineNumber, LogString);
	}
	else
	{
		fprintf(m_File, "%s\t%S\t%S\n", strnow, LoglevelName, LogString);
	}
#else
	if (m_LogLevel == levelDeveloperInfo)
	{
		fprintf(m_File, "%s\t%s\t%s, %d\t%s\n", strnow, LoglevelName, m_SourceFile, m_LineNumber, LogString);
	}
	else
	{
		fprintf(m_File, "%s\t%s\t%s\n", strnow, LoglevelName, LogString);
	}
#endif

#ifdef SYSLOG_TRACE
	TCHAR mid[1025] = {0};
	wsprintf(mid, _T("%s\n"), LogString);
	OutputDebugString(mid);
#endif

#ifdef SYSLOG_STDOUT
	TCHAR mid2[1025] = {0};
	wsprintf(mid2, _T("%s\n"), LogString);
	wprintf(mid2);
#endif
    fclose(m_File);
}

void CSysLogs::ReplaceCRLF(TCHAR *s)
{
	TCHAR *mid = s;
	while (*mid != '\0')
	{
		switch (*mid)
		{
		case '\r':
			*mid = '|';
			break;
		case '\n':
			*mid = '|';
			break;
		}
		mid++;
	}
}

void CSysLogs::LogInfo(TCHAR *format, ...)
{
	if (m_LogLevel == levelDeveloperInfo || m_LogLevel == levelInfo)
	{
		//never corrupt the last error value...
		DWORD LastError = GetLastError();
		//do the actual logging...
		TCHAR mid[1025] = {0}; //the wvsprintf function never puts more than 1024 bytes in a string...
		va_list args;
		va_start(args, format);
		wvsprintf(mid, format, args);
		ReplaceCRLF(mid);
		LogNow(_T("Info"), mid);
		va_end(args);
		SetLastError(LastError);
	}
	//LeaveCriticalSection(&m_cs);
	m_cs.Unlock();
}

void CSysLogs::LogError(TCHAR *format, ...)
{
	if (m_LogLevel == levelDeveloperInfo || m_LogLevel == levelInfo || m_LogLevel == levelError)
	{
		//never corrupt the last error value...
		DWORD LastError = GetLastError();
		//do the actual logging...
		TCHAR mid[1025] = {0}; //the wvsprintf function never puts more than 1024 bytes in a string...
		va_list args;
		va_start(args, format);
		wvsprintf(mid, format, args);
		ReplaceCRLF(mid);
		LogNow(_T("Error"), mid);
		va_end(args);
		SetLastError(LastError);
	}
	//LeaveCriticalSection(&m_cs);
	m_cs.Unlock();
}

void CSysLogs::LogDevInfo(TCHAR *format, ...)
{
	if (m_LogLevel == levelDeveloperInfo)
	{
		//never corrupt the last error value...
		DWORD LastError = GetLastError();
		//do the actual logging...
		TCHAR mid[1025] = {0}; //the wvsprintf function never puts more than 1024 bytes in a string...
		va_list args;
		va_start(args, format);
		wvsprintf(mid, format, args);
		ReplaceCRLF(mid);
		LogNow(_T("DevInfo"), mid);
		va_end(args);
		SetLastError(LastError);
	}
	//LeaveCriticalSection(&m_cs);
	m_cs.Unlock();
}
