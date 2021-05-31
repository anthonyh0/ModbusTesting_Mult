// INI.cpp: implementation of the CINI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INI.h"
#include <string.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAX_SECTION 40
//#define STATION_SECTION ""
#define STATION_KEY "stationName"

#define CMD_SECTION "command"
#define CMD_KEY "Polling"

#define IP_SECTION "ipRecord"
#define IP_KEY_HEAD "ip"

#define PORT_SECTION "portRecord"
#define PORT_KEY_HEAD "port"

#define STATION_NUM_KEY "statioNname"
#define PARTITION_NUM_KEY "PartitionNum"
#define REGISTER_NUM_RANGE "RegisterNumRange"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CINI g_CINI;



CINI::CINI()
{

	memset(m_WorkDir, 0x00, sizeof(m_WorkDir));
	memset(m_configPath, 0x00, sizeof(m_configPath));
	memset(m_stationPath, 0x00, sizeof(m_stationPath));
	
	GetCurrentDirectory(255,m_WorkDir);	
	sprintf(m_configPath, "%s\\config.ini", m_WorkDir);

	memset(m_WorkDir, 0x00, sizeof(m_WorkDir));

	GetCurrentDirectory(255, m_WorkDir);
	sprintf(m_stationPath, "%s\\StationNumber.ini", m_WorkDir);

	GetIniConfig();
	GetIniStationNumber();
}

CINI::~CINI()
{   

}


void CINI::GetIniConfig()
{
	pollCmd = getIniFieldInfor(CMD_SECTION, CMD_KEY, m_configPath);

	for (size_t i = 0; i < 10;i++)
	{
		CString ipKey;
		std::string ipStr;
		ipKey.Format("%s_%d", IP_KEY_HEAD, i);
		ipStr = getIniFieldInfor(IP_SECTION, ipKey.GetBuffer(), m_configPath);
		ipKey.ReleaseBuffer();
		if ("" == ipStr)
		{
			break;
		}
		vecIp.push_back(ipStr);
	}

	for (size_t i = 0; i < 10; i++)
	{
		CString portKey;
		std::string portStr;
		portKey.Format("%s_%d", PORT_KEY_HEAD, i);
		portStr = getIniFieldInfor(PORT_SECTION, portKey.GetBuffer(), m_configPath);
		portKey.ReleaseBuffer();
		if ("" == portStr)
		{
			break;
		}
		vecPort.push_back(portStr);
	}

	return;
}

void CINI::GetIniStationNumber()
{
	int listCount = 0;
	char chSectionNames[LENGTH_B] = { 0 };//总的提出来的字符串
	std::vector<int> stationCheckList1(STATION_REG_LEN, 0); //初始化32个0元素的vector
	//AfxMessageBox(m_stationPath);
	int ret = GetPrivateProfileSectionNames(chSectionNames, LENGTH_B, m_stationPath);//获取ini文件Section个数和数据
	for (char* sectionName = chSectionNames; *sectionName != '\0'; sectionName += strlen(sectionName) + 1)
	{
		std::string strName = getIniFieldInfor(sectionName, STATION_NUM_KEY, m_stationPath);
		std::string RegisterNumRange = getIniFieldInfor(sectionName, REGISTER_NUM_RANGE, m_stationPath);
		std::string partitionRange = getIniFieldInfor(sectionName, PARTITION_NUM_KEY, m_stationPath);

		VecString vecRegTemp = vSplitString(RegisterNumRange, "-");
		VecString vecPartTemp = vSplitString(partitionRange, "-");

		//char listSelection[32] = { 0 };
		//sprintf(listSelection, "%d.%s", listCount, transcoding2utf8((char*)strName.c_str()).c_str());
		//mapIniStation.insert(std::make_pair(listCount, sectionName));


		stationRegister sr;
		sr.id = listCount;
		sr.name = strName;
		sr.partitionNum1 = atoi(vecPartTemp.at(0).c_str());
		sr.partitionNum2 = atoi(vecPartTemp.at(1).c_str());
		sr.low = atoi(vecRegTemp.at(0).c_str());
		sr.high = atoi(vecRegTemp.at(1).c_str());
		sr.vecStationCheckList = stationCheckList1;
		mapStationList.insert(std::make_pair(listCount, sr));

		listCount++;
	}
	//清空并且释放内存
	stationCheckList1.clear();
	std::vector <int>().swap(stationCheckList1);
	return;
}

void CINI::addIpAndPortToIni(std::string strIp, std::string strPort)
{
	CString ipKey, portKey;

	/*ip*/
	vector<std::string>::iterator ipResult = std::find(vecIp.begin(), vecIp.end(), strIp); //查找strIp
	if (ipResult == vecIp.end()) //没找到
	{
		int ipCount = GetPrivateProfileInt("ipRecord", "count", 0, m_configPath);
		ipKey.Format("ip_%d", ipCount);
		WritePrivateProfileString("ipRecord", ipKey, strIp.c_str(), m_configPath);
		WritePrivateProfileString("ipRecord", "count", std::to_string(ipCount + 1).c_str(), m_configPath);
		
	}

	/*port*/
	vector<std::string>::iterator portResult = std::find(vecPort.begin(), vecPort.end(), strPort);
	if (portResult == vecPort.end())
	{
		int portCount = GetPrivateProfileInt("portRecord", "count", 0, m_configPath);
		ipKey.Format("port_%d", portCount);
		WritePrivateProfileString("portRecord", ipKey, strPort.c_str(), m_configPath);
		WritePrivateProfileString("portRecord", "count", std::to_string(portCount + 1).c_str(), m_configPath);
	}

}

void CINI::getIniRecord()
{

	//用map.erase(map.begin(),map.end())比较好  清空元素
	mapHistory.erase(mapHistory.begin(), mapHistory.end());
	int count = 0;
	int recordCount = GetPrivateProfileInt("Record", "count", 0, m_configPath);
	for (size_t i = 0; i < recordCount; i++)
	{
		CString recordKey, timesKey;
		std::string recoedStr, timesStr;
		recordKey.Format("Record_%d", i);
		timesKey.Format("Time_%d", i);
		recoedStr = getIniFieldInfor("Record", recordKey.GetBuffer(), m_configPath);
		timesStr = getIniFieldInfor("Record", timesKey.GetBuffer(), m_configPath);
		recordKey.ReleaseBuffer();
		timesKey.ReleaseBuffer();
		if ("" == recoedStr)
		{
			continue;
		}
		mapHistory.insert(std::make_pair(timesStr.c_str(), recoedStr.c_str()));
	}
}

void CINI::addIniRecord(bool isClear, std::string record, std::string times)
{
	if (isClear)
	{
		::WritePrivateProfileSection("Record", NULL, m_configPath);
		WritePrivateProfileString("Record", "count", "0", m_configPath);
	}
	else
	{
		CString recordText, timeText;
		int recordCount = GetPrivateProfileInt("Record", "count", 0, m_configPath);
		recordText.Format("Record_%d", recordCount);
		timeText.Format("Time_%d", recordCount);
		WritePrivateProfileString("Record", recordText, record.c_str(), m_configPath);
		WritePrivateProfileString("Record", timeText, times.c_str(), m_configPath);
		WritePrivateProfileString("Record", "count", std::to_string(recordCount + 1).c_str(), m_configPath);

	}
}
