// SysLogs.h: interface for the CSysLogs class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSLOGS_H__EEA22A5A_AC99_4735_8431_F6817D34B090__INCLUDED_)
#define AFX_SYSLOGS_H__EEA22A5A_AC99_4735_8431_F6817D34B090__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <stdio.h>
//#include <windows.h>
#include <afxmt.h>
#include <afx.h>
#define SYSLOG_SETFILENAME(filename)     g_SysLog.SetFileName(filename)
#define SYSLOG_SETLOGLEVEL_NOTHING       g_SysLog.SetLogLevel(levelNothing)

#define SYSLOG_SETLOGLEVEL_ERROR         g_SysLog.SetLogLevel(levelError)
#define SYSLOG_SETLOGLEVEL_INFO          g_SysLog.SetLogLevel(levelInfo)
#define SYSLOG_SETLOGLEVEL_NORMAL        g_SysLog.SetLogLevel(levelInfo)
#define SYSLOG_SETLOGLEVEL_DEVELOPERINFO g_SysLog.SetLogLevel(levelDeveloperInfo)

#define SYSLOG_INFO    g_SysLog.Lock();\
                       g_SysLog.LogInfo

#define SYSLOG_NORMAL  g_SysLog.Lock();\
                       g_SysLog.LogInfo

#define SYSLOG_ERROR   g_SysLog.Lock();\
                       g_SysLog.LogError

#define SYSLOG_DEVINFO g_SysLog.Lock();\
                       g_SysLog.LogDevInfo

/*
#define SYSLOG_INFO g_SysLog.Lock();\
	    g_SysLog.SetLineNumber(__LINE__);\
	    g_SysLog.SetSourceFileName(__FILE__);\
        g_SysLog.LogInfo

#define SYSLOG_ERROR g_SysLog.Lock();\
	    g_SysLog.SetLineNumber(__LINE__);\
	    g_SysLog.SetSourceFileName(__FILE__);\
        g_SysLog.LogError

#define SYSLOG_DEVINFO g_SysLog.Lock();\
	    g_SysLog.SetLineNumber(__LINE__);\
	    g_SysLog.SetSourceFileName(__FILE__);\
        g_SysLog.LogDevInfo
*/

typedef enum CSysLogLevels
{
	levelNothing       = 0,
	levelError         = 1,
	levelInfo          = 2,
	levelDeveloperInfo = 3
}CSysLogLevels;

// enum for LOG_LEVEL
typedef enum LOG_LEVEL
{
	DISABLE_LOG       = 0,
	LOG_LEVEL_ERROR   = 1,
	LOG_LEVEL_INFO	  = 2,
	LOG_LEVEL_NORMAL  = 3,
	LOG_LEVEL_BUFFER  = 4,
	LOG_LEVEL_TRACE   = 5,
	LOG_LEVEL_DEBUG   = 6,
	LOG_LEVEL_WARN    = 7,
	ENABLE_LOG        = 8
		
}LogLevel;

class CSysLogs  
{
protected:	
	CCriticalSection m_cs;
	CSysLogLevels    m_LogLevel;
	FILE  *m_File;
	TCHAR m_FileName[MAX_PATH];	
	char  m_SourceFile[MAX_PATH];
	int   m_LineNumber;

	void  LogNow(TCHAR *LoglevelName, TCHAR *LogString);
	void  ReplaceCRLF(TCHAR *s);

public:
	CSysLogs();
	virtual ~CSysLogs();
	//void Lock() {EnterCriticalSection(&m_cs);};
	void Lock() {m_cs.Lock();};
	void SetLineNumber(int line) {m_LineNumber = line;};
	void SetSourceFileName(char *filename);
	bool SetFileName(TCHAR *FileName);
	void SetLogLevel(CSysLogLevels LogLevel);
	void LogError(TCHAR *format, ...);
	void LogInfo(TCHAR *format, ...);
	void LogDevInfo(TCHAR *format, ...);
	
};

extern CSysLogs g_SysLog;

#endif // !defined(AFX_SYSLOGS_H__EEA22A5A_AC99_4735_8431_F6817D34B090__INCLUDED_)
