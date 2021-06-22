#pragma once

#define HAVE_REMOTE
#include "stdafx.h"
#include "pcap.h"
#include <iostream>
#include "vector"
#include <WinSock2.h>
#include <thread>
#include <mutex>
#include "PublicFunDef.h"
//#include <afxwin.h>
#include <windows.h>
#define WRONGNUM -1
class NetworkPacket
{
public:
	NetworkPacket();
	~NetworkPacket();

public:
	int findAllDevs(std::vector<std::string> &vecDevs, char *errbuf);//获取设备适配器名称列表 返回获取数量，return -1 ：表示获取失败 
	void setDevs(std::string strDevs);
	std::vector<std::string> getDevs();
	int openPacket(char* devsName, char* packetFilter, char* errbuf);//打开抓包，设置过滤器，返回状态return -1 ：表示获取失败
	int openPacket(int devsIndex, char* packetFilter, char* errbuf);//打开抓包，设置过滤器，返回状态return -1 ：表示获取失败
	int packetStart();
	int packetStop();
	void packetGetData();
	int getPacketDataLen();
	std::string getPacketErr();
	void setPacketData(std::string strData);
	std::vector<std::string> getPacketData();

private:
	struct bpf_program fcode;
	struct pcap_pkthdr* header;
	const u_char* pkt_data;
	pcap_if_t* alldevs;
	pcap_if_t* pDevs;
	pcap_t* adhandle;
	u_int uintNetmask;
	bool isdevs;
	bool m_bRun1;
	int devsCount;
	struct tm* ltime;
	char timestr[16];
	time_t local_tv_sec;
	std::vector<std::string> vecPacketData;
	std::vector<std::string> vecDevs;
	std::string errStr;
};

