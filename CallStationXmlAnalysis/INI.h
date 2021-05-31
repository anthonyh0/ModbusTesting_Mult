// INI.h: interface for the CINI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INI_H__B7DBC83F_DB86_4F04_A20D_31C8AEF15071__INCLUDED_)
#define AFX_INI_H__B7DBC83F_DB86_4F04_A20D_31C8AEF15071__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <vector>
#include <string>
#include <map>
#include "PublicFunDef.h"

using std::vector;

struct stationRegister
{
	std::string name;
	int id;
	int partitionNum1;
	int partitionNum2;
	int low;
	int high;
	std::vector<int> vecStationCheckList;
};


class CINI  
{
public:

	char    m_configPath[MAX_PATH];
	char    m_stationPath[MAX_PATH];
	char    m_WorkDir[MAX_PATH];

	size_t STATION_REG_LEN = 32;//每个车站包含寄存器个数

	vector<std::string> vecIp;
	vector<std::string> vecPort;

	std::string pollCmd;
	std::map<int, stationRegister> mapStationList;
	std::map<CString, CString> mapHistory;
	void addIpAndPortToIni(std::string strIp, std::string strPort);
	void getIniRecord();
	void addIniRecord(bool isClear, std::string record, std::string times);

public:

	CINI();
	virtual ~CINI();

private:
	void GetIniConfig();
	void GetIniStationNumber();
private:
   
};

extern CINI g_CINI;

#endif // !defined(AFX_INI_H__B7DBC83F_DB86_4F04_A20D_31C8AEF15071__INCLUDED_)
