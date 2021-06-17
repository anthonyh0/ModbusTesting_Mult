#include "stdafx.h"
#include "NetWorkManage.h"

std::string NetWorkManage::stServerRecv = "";
std::string NetWorkManage::stSurplus = "";
VecString NetWorkManage::vecSerRecvInfo;
VecString NetWorkManage::vecException;
VecString NetWorkManage::vecCliRecvInfo;
int NetWorkManage::procolType = TCPSERVER;
StreamSocket NetWorkManage::ss;
//Poco::Net::TCPServer* NetWorkManage::serverPri;
NetWorkManage::NetWorkManage()
{
	clientRecv = "";
	serverRecv = "";
}
NetWorkManage::~NetWorkManage()
{
	
	if (serverPri != nullptr)
	{
		serverPri->stop();
		delete serverPri;
		serverPri = nullptr;

	}
	ss.close();
}
class TcpServerConnection : public TCPServerConnection
{
public:
	TcpServerConnection(const StreamSocket& s) :
		TCPServerConnection(s)
	{

	}

	void run(void);

};
mutex m_mutexServerRecv;

/**
* @brief CZhiTcpServerConnection::run 处理链接请求的主函数
*/
void TcpServerConnection::run(void)
{
	try
	{
		
		char buf[RECV_BUF_SIZE] = { 0 };
		CCriticalSection g_UpdateData;
		while (true)
		{
			if (socket().available())
			{
				g_UpdateData.Lock();
				//lock_guard<mutex> lg(m_mutexServerRecv);
				int nLen = socket().receiveBytes(buf, 1024);
				std::stringstream ssbuf(buf);
				ITC_WriteLog(LOG_LEVEL_INFO, "%d:%s", nLen,buf);

				NetWorkManage net;
				net.handleServerRecvInfo(buf);
			
				//socket().sendBytes(ssbuf.str().data(), ssbuf.str().length());
				memset(buf, 0x00, sizeof(buf));
				g_UpdateData.Unlock();
			}
		}
	}
	catch (std::exception& e)
	{
		//std::cout << e.what() << std::endl;
		NetWorkManage::vecException.push_back(e.what());
	}
}

class TcpServerGo : public TCPServerConnectionFactory
{
public:

	inline TCPServerConnection* createConnection(const StreamSocket &socket)
	{
		return new TcpServerConnection(socket);
	}

};
int NetWorkManage::tcpServerOpen(int port)
{
	Poco::Net::ServerSocket serverSocket(port);
	Poco::Timespan timeout(10, 0);
	serverSocket.setReceiveTimeout(timeout);
	serverSocket.listen(10);
	serverSocket.setNoDelay(true);
	Poco::Net::TCPServerParams::Ptr param = new Poco::Net::TCPServerParams;
	//设定队列的最大长度。
	param->setMaxQueued(100);
	//设定　TcpServer 最大线程的个数。
	param->setMaxThreads(100);
	serverPri = new Poco::Net::TCPServer(new TcpServerGo,serverSocket);
	serverPri->start();
	procolType = TCPSERVER;
	return 0;
}

void NetWorkManage::tcpServerClose()
{
	cleanServerRecvStr();
	serverPri->stop();
	delete  serverPri;
	serverPri = nullptr;
}

int NetWorkManage::tcpCilentConect(const char *ip, int port)
{
	int n = 0;
	char buffer[RECV_BUF_SIZE] = { "\0" };

	struct sockaddr_in netAddr;
	netAddr.sin_family = AF_INET;
	netAddr.sin_port = htons(port);
	netAddr.sin_addr.s_addr = inet_addr(ip); //inet_addr 将字符串转为数字
	SocketAddress sa((struct sockaddr *)&netAddr, sizeof(struct sockaddr));
	

	Timespan timeout(2000000);//单位（微秒），2000000 微秒=2 秒
	try
	{
		ss.connect(sa, timeout);
	}
	catch (ConnectionRefusedException&)
	{
		vecException.push_back("connect refuse");
		return 1;
	}
	catch (NetException&)
	{
		vecException.push_back("net exception");
		return 1;
	}
	catch (TimeoutException&)
	{
		vecException.push_back("connect time out");
		return 1;
	}
	//setopt timeout
	Timespan timeout3(5000000);
	ss.setReceiveTimeout(timeout3); 
	Timespan timeout4(5000000);
	ss.setSendTimeout(timeout4); 

	//setopt bufsize
	ss.setReceiveBufferSize(RECV_BUF_SIZE); 
	ss.setSendBufferSize(SEND_BUF_SIZE); 

	//setopt nodelay
	ss.setNoDelay(true); 
	procolType = TCPCLIENT;
	return 0;
}

int NetWorkManage::clientSendMsg(const void* buf, int sengLen)
{
	int sendLen = 0;
	sendLen = ss.sendBytes(buf, sengLen); //block
	return sendLen;
}

int NetWorkManage::clientReceiveMsg(void *buf)
{
	int recvLen = 0;
	char buffer[RECV_BUF_SIZE] = { "\0" };
	memset(buffer, 0, sizeof(buffer));
	if (ss.available())
	{
		recvLen = ss.receiveBytes(buffer, sizeof(buffer)); //block
		//setClientRecvStr(buffer);
		handleServerRecvInfo(buffer);
		memcpy(buf, buffer, recvLen);
	}
	return recvLen;
}

void NetWorkManage::tcpClientClose()
{
	vecException.clear();
	vector<string>().swap(vecException);//清空容量
	ss.close();
	cleanClientRecvStr();
}

int NetWorkManage::getExceptionLength()
{
	return vecException.size();
}

int NetWorkManage::getClientRecvLength()
{
	return vecCliRecvInfo.size();
}

int NetWorkManage::getServerRecvLength()
{
	return vecSerRecvInfo.size();
}

VecString NetWorkManage::getClientRecvStr()
{
	return vecCliRecvInfo;
}

VecString NetWorkManage::getServerRecvStr()
{
	return vecSerRecvInfo;
}

void NetWorkManage::setClientRecvStr(std::string recvStr)
{
	ITC_WriteLog(LOG_LEVEL_INFO, "set ClientRecv: %s", recvStr.c_str());
	vecCliRecvInfo.push_back(recvStr);
}

void NetWorkManage::setServerRecvStr(std::string recvStr)
{
	ITC_WriteLog(LOG_LEVEL_INFO, "set Recv: %s", recvStr.c_str());
	vecSerRecvInfo.push_back(recvStr);
}

int NetWorkManage::getCurrentType()
{

	return procolType;
}

void NetWorkManage::cleanClientRecvStr()
{
	vecCliRecvInfo.clear();
	vector<string>().swap(vecCliRecvInfo);//清空容量
}

void NetWorkManage::cleanServerRecvStr()
{
	if (vecSerRecvInfo.size() != 0)
	{
		vecSerRecvInfo.clear();
		vector<string>().swap(vecSerRecvInfo);//清空容量

	}
}

void NetWorkManage::setServerAddr(std::string addr)
{
	serverAddr = addr;
}

std::string NetWorkManage::getServerAddr()
{

	return serverAddr;
}

VecString NetWorkManage::getExceptionStr()
{
	return vecException;
}

bool NetWorkManage::handleServerRecvInfo(std::string info)
{
	info.erase(0, info.find_first_not_of(" "));
	info.erase(info.find_last_not_of(" ") + 1);
	if (info.empty())
	{
		return false;
	}
	g_UpdateData.Lock();
	//std::string XML_HEAD = "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
	std::string XML_HEAD = "<MSG>";
	std::string XML_END = "</MSG>";
	stSurplus.erase(0, stSurplus.find_first_not_of(" "));
	stSurplus.erase(stSurplus.find_last_not_of(" ") + 1);
	stSurplus.append(info);
	std::string strTemp = stSurplus;
	//std::string strHead = stSurplus.substr(0, 38);
	//std::string strEnd = stSurplus.substr(stSurplus.size() - 6);
	//setClientRecvStr(strTemp);
	stSurplus = "";
	for (;;)
	{
		if (strTemp.find("<MSG>") != -1 && strTemp.find("</MSG>") != -1)
		{
				int a = strTemp.find(XML_HEAD);
				int b = strTemp.find(XML_END);
				if (a != -1 && b != -1)
				{
					string strUnpacking = strTemp.substr(a, b + 6);
					switch (procolType)
					{
					case TCPSERVER:
						setServerRecvStr(strUnpacking);
						break;
					case TCPCLIENT:
						setClientRecvStr(strUnpacking);
						break;
					default:
						break;
					}
					
					strTemp.erase(strTemp.find(XML_HEAD), strUnpacking.length());
				}
				else
				{
					break;
				}
		}
		else
		{
			stSurplus = "";
			//stSurplus = strTemp;
			break;
		}
	}
	g_UpdateData.Unlock();
	return true;
}