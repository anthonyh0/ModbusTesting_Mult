
// ModbusTesting.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CModbusTestingApp: 
// �йش����ʵ�֣������ ModbusTesting.cpp
//

class CModbusTestingApp : public CWinApp
{
public:
	CModbusTestingApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CModbusTestingApp theApp;