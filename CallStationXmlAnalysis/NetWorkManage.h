#pragma once
#include "stdafx.h"
#include <windows.h>
#include <Winsock2.h>
#include "PublicFunDef.h"
#include "Poco/Net/Socket.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetException.h"
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/TCPServer.h>
#include <Poco/Net/TCPServerConnectionFactory.h>
#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/StreamSocket.h>
#include "Poco/Timespan.h"
#include <Poco/Timespan.h>
#include <mutex>
#include <iostream>
#include <sstream>
#pragma comment(lib,"ws2_32.lib")



using Poco::Net::Socket;
using Poco::Net::StreamSocket;
using Poco::Net::SocketAddress;
using Poco::Net::NetException;
using Poco::Net::ConnectionRefusedException;
using Poco::Net::InvalidSocketException;
using Poco::Net::TCPServerConnectionFactory;
using Poco::Net::TCPServerConnection;//构造函数需要 StreamSocket
using Poco::Timespan;
using Poco::TimeoutException;
using Poco::IOException;
using namespace std;

const int RECV_BUF_SIZE = 64 * 1024;
const int SEND_BUF_SIZE = 64 * 1024;

enum proType {
	TCPSERVER,
	TCPCLIENT,
	PACKET
};
class NetWorkManage
{
public:
	NetWorkManage();
	~NetWorkManage();

	int tcpServerOpen(int port);
	void tcpServerClose();

	int tcpCilentConect(const char *ip, int port);
	int clientSendMsg(const void* buf, int sengLen);
	int clientReceiveMsg(void *buf);
	void tcpClientClose();

	int getExceptionLength();
	int getClientRecvLength();
	int getServerRecvLength();
	bool handleServerRecvInfo(std::string info);
	void setClientRecvStr(std::string recvStr);
	void setServerRecvStr(std::string recvStr);
	int getCurrentType();
	void cleanClientRecvStr();
	void cleanServerRecvStr();
	void setServerAddr(std::string addr);
	std::string getServerAddr();
	VecString getClientRecvStr();
	VecString getServerRecvStr();
	VecString getExceptionStr();

public:
	static std::string stServerRecv;
	static std::string stSurplus;
	static VecString vecSerRecvInfo;
	static VecString vecCliRecvInfo;
	static VecString vecException;
	static int procolType;
	static StreamSocket ss;
	//static Poco::Net::TCPServer* serverPri;
private:
	//StreamSocket ss;
	std::string serverAddr;
	Poco::Net::TCPServer *serverPri;
	std::string  serverRecv;
	std::string  clientRecv;
	mutex m_mutexHandleRecv;

	CCriticalSection g_UpdateData;
};
