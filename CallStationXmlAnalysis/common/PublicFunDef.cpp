// PublicFunDef.cpp: implementation of the CPublicFunDef class.
//
//////////////////////////////////////////////////////////////////////
//
//#include "stdafx.h"
#include <iostream>
#include <fstream> 
#include "PublicFunDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//CSysLogs*   m_LOG;

//LPCSTR m_inipath;
//LPCSTR m_WorkDir;

///////////////////////////////////////////////////////////////////////////////
// hex to asc: 0x22 -> "22"
int Hex2Asc(char *Dest,char *Src,int SrcLen)
{
	int i;
	for ( i = 0; i<SrcLen; i ++ )
	{
		sprintf(Dest + i * 2,"%02X",(unsigned char)Src[i]);
	}
	Dest[i * 2] = 0;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// asc to hex: "22" -> 0x22
int Asc2Hex(char *Dest,char *Src,int SrcLen)
{
	int i;
	for ( i = 0; i < SrcLen / 2; i ++ )
	{
		sscanf(Src + i * 2,"%02X",(unsigned char *)&Dest[i]);
	}
	return TRUE;
}

CString UT2WC(const char* buf)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, buf, -1, NULL, 0);
	std::vector<wchar_t> unicode(len);
	MultiByteToWideChar(CP_UTF8, 0, buf, -1, &unicode[0], len);
	
    return CString(&unicode[0]);
}

CString WC2UT(const wchar_t* buf)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, buf, -1, NULL, 0, NULL, NULL);
	std::vector<char> utf8(len);
	WideCharToMultiByte(CP_UTF8, 0, buf, -1, &utf8[0], len, NULL, NULL);
	
    return CString(&utf8[0]);
}

CString MB2WC(const char* buf)
{
	int len = MultiByteToWideChar(CP_ACP, 0, buf, -1, NULL, 0);
	std::vector<wchar_t> unicode(len);
	MultiByteToWideChar(CP_ACP, 0, buf, -1, &unicode[0], len);
	
    return CString(&unicode[0]);
}

CString WC2MB(const wchar_t* buf)
{
	int len = WideCharToMultiByte(CP_ACP, 0, buf, -1, NULL, 0, NULL, NULL);
	std::vector<char> utf8(len);
	WideCharToMultiByte(CP_ACP, 0, buf, -1, &utf8[0], len, NULL, NULL);
	
    return CString(&utf8[0]);
}

void Utf2GBK(CString &buffer)
{
	//UTF8转到本地字符集
	int len=0;
	if( len=MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, (LPCTSTR)buffer, -1, NULL,0))
	{  
		//如果len为0，则不需要转换，因为buffer本身就是本地字符集
		unsigned short * wszGBK = new unsigned short[len+1];
		memset(wszGBK, 0, len * 2 + 2);
		len = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)buffer, -1, (LPWSTR)wszGBK, len);
		
		len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL); 
		char *szGBK=new char[len + 1];
		memset(szGBK, 0, len + 1);
		len=WideCharToMultiByte (CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL,NULL);
		
		buffer = szGBK;
		delete[] szGBK;
		delete[] wszGBK;
	}
}

void GBK2Utf(CString &buffer)
{   
	//本地字符转换到UTF8
	int len=0;
	if(len=MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, (LPCTSTR)buffer, -1, NULL,0))
	{ 
		unsigned short * wszGBK = new unsigned short[len+1];
		memset(wszGBK, 0, len * 2 + 2);
		len=MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)buffer, -1, (LPWSTR)wszGBK, len);
		
		len = WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL); 
		char *szGBK=new char[len + 1];
		memset(szGBK, 0, len + 1);
		len=WideCharToMultiByte (CP_UTF8, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL,NULL);
		
		buffer = szGBK;
		delete[] szGBK;
		delete[] wszGBK;
	}
	
}

CString StrSwap(CString indata)
{	
	int nLen = indata.GetLength();
	if(nLen < 2)//不足2个字符，不交换
	   return "";
	char* pData = indata.GetBuffer(nLen);
	for(int i=0; i<nLen-1; i+=2)
	{
		char chTemp = *(pData+i);
		*(pData+i) = *(pData+i+1);
		*(pData+i+1) = chTemp;
	}
	indata.ReleaseBuffer();
	return indata;	
}

std::string swapstring(const std::string buf)
{
    std::string::size_type len = buf.length();
	if(len < 2)  //不足2个字符，不交换	
	   return buf;	

	char* pData = new char[len+1];
	memset(pData,0x00,len+1);
	strncpy(pData,buf.c_str(),len);		
	//char* pData = (char*)buf.c_str();
	for(int i=0; i<len-1; i+=2)
	{
		char chTemp  = *(pData+i);
		*(pData+i)   = *(pData+i+1);
		*(pData+i+1) = chTemp;
	}	
	return pData;
}

// 去除非法字符  
CString DeleteIllegalChar(CString indata)
{
	unsigned char p;
	int iLen = indata.GetLength();
	for (int i = 0; i<iLen; i++)
	{
		p = indata.GetAt(i);
		if (p<0x21 || p>0x7E)		
		{
			indata.Replace(p,' ');
		}
	}
	indata.Replace(" ","");
	return indata;
}

void DelillegalChar(const char* str,char* buf)
{
    int len = strlen(str);
	
	int j=0;
	for (int i=0; i<len; i++)
	{
		unsigned char tmp = *(str+i);
		if (tmp<0x21 || tmp>0x7E)
		{
            continue;
		}
		else
		{
			strncpy(buf+j,str+i,1);
			j++;
		}
	}
}

BOOL IsHexString(const char* str)
{
    #define _ishex(_v1)  (((_v1)>='0'&&(_v1)<='9')||((_v1)>='A'&&(_v1)<='F')||((_v1)>='a'&&(_v1)<='f'))

    int len = strlen(str);
	for(int i=0; i<len; i++)
	{
		if(!_ishex(str[i]))
			return FALSE;
	}
	return TRUE;
}

BOOL ConnectIPC(const char* IP,const char* path, const char* User, const char* PassWord) 
{ 
	NETRESOURCE NetResouce;
	char tmp[255]="\\\\"; 
    strcat(tmp,IP); 
    //strcat(tmp,"\\E$\\LOGS"); 
	strcat(tmp,path);   
	//NetResouce.dwScope      = RESOURCE_CONNECTED;
	//NetResouce.dwUsage      = RESOURCEUSAGE_CONNECTABLE;
	NetResouce.dwType       = RESOURCETYPE_ANY; 
	NetResouce.lpLocalName  = "";
	NetResouce.lpRemoteName = tmp;
    NetResouce.lpProvider   = NULL;	
    //CONNECT_UPDATE_PROFILE 表示创建永久性连接
	DWORD ret = WNetAddConnection2(&NetResouce,PassWord,User,CONNECT_UPDATE_PROFILE);
    if (ret == NO_ERROR) 
	{		
		return TRUE; 
	}
    else 
	{
	   if (ret == 1219)
	   {
		   //如果出现1219错误，选断开再连接
           ret = WNetCancelConnection2(NetResouce.lpRemoteName, 0, TRUE);
		   /*
		   ret = WNetAddConnection2(&NetResouce,PassWord,User,CONNECT_UPDATE_PROFILE);
		   if (ret == NO_ERROR) 
		   {		
			   return TRUE; 
		   }
		   else
		   {
			   return FALSE; 
		   }
		   */
		   return FALSE; 
	   } 
	   else
	   {
           return FALSE; 
	   }       
	}

}

BOOL CloseIPC(const char* IP, const char* path)
{	
	char tmp[255]="\\\\"; 
    strcat(tmp,IP); 
    //strcat(tmp,"\\E$\\LOGS"); 
	strcat(tmp,path);
	DWORD ret = WNetCancelConnection2(tmp, CONNECT_UPDATE_PROFILE,TRUE);
	if (ret == NO_ERROR)
	{
		return TRUE;
	} 
	else
	{
		return FALSE; 
	}
} 

BOOL GetComputerUser(char* name, char* user) 
{

	char  szBuffer[500];
	DWORD dwNameLen;
	
	dwNameLen = 500;
	memset(szBuffer,0,500);
	if (!GetComputerName(szBuffer, &dwNameLen)) 
	{
		//printf("Error  %d\n", GetLastError());
		return FALSE;
	}
	else
	{
		memcpy(name,szBuffer,dwNameLen);
		name[dwNameLen]='\0';
		//printf("计算机名为: %s\n", szBuffer);
	}
	
	dwNameLen = 500;
	memset(szBuffer,0,500);
	if (!GetUserName(szBuffer, &dwNameLen))
	{		
		//printf("Error  %d\n", GetLastError());
		return FALSE;
	}
	else
	{
		memcpy(user,szBuffer,dwNameLen);
		user[dwNameLen]='\0';
		//printf("当前用户名为：%s\n", szBuffer);
	}
	return TRUE;
}

BOOL ConnectRemotePC(const char* IP, const char* path, const char* user, const char* pass)
{
	//char* user = "Administrator";
	//char* pass = "!Qw2!Qw2";
    //char* pass = "admin@123";
	if (!ConnectIPC(IP,path,user,pass))
	{
		//AfxMessageBox("创建远程日志出错");
		//AfxMessageBox("Create remote logs error!");
		return FALSE;
	}
	return TRUE;
}

//字符串分割函数
vector<CString> split(CString str,CString pattern)
{
    int pos;
    vector<CString> result;

    str+= pattern;    //扩展字符串以方便操作

    int size = str.GetLength();
	
    for(int i=0; i<size; i++)
    {
        pos = str.Find(pattern,i);
        if(pos < size)
        {
            CString s = str.Mid(i,pos-i);
            result.push_back(s);
            i = pos + pattern.GetLength()-1;
        }
    }
    return result;
}
//字符串分割函数
VecString vSplitString(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//扩展字符串以方便操作
	int size = str.size();
	for (int i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}
int  ITC_WriteLog(int _level_,  LPCTSTR Errinfo, ...)
{
	CString tmp,err;
	va_list args;
	va_start(args, Errinfo);
	err.FormatV(Errinfo,args);
	va_end(args);

	switch (_level_)
	{
    case LOG_LEVEL_NORMAL:	
		tmp.Format(":");
		tmp = tmp + " -- " + err;
		SYSLOG_SETLOGLEVEL_NORMAL;
		SYSLOG_NORMAL(tmp.GetBuffer(0));	
		tmp.ReleaseBuffer();
		break;
		
	case LOG_LEVEL_ERROR:	     
		tmp.Format(":");
		tmp = tmp + " -- " + err;
		SYSLOG_SETLOGLEVEL_ERROR;
		SYSLOG_ERROR(tmp.GetBuffer(0));	
		tmp.ReleaseBuffer();
		break;
		
	case LOG_LEVEL_INFO:	
		tmp = ": -- " + err;
		SYSLOG_SETLOGLEVEL_INFO;
		SYSLOG_INFO(tmp.GetBuffer(0));
		tmp.ReleaseBuffer();
		break;
		
	default:
		break;
	} 
	return 0; 

}

int initializeLog(char* logName)
{
	//创建日志目录或文件	
	CFileStatus     fs;
	CFileException  e;
	CStdioFile      m_File;
	char szPath[1024] = { 0 };
	std::string currentPath = getCurrentPath();
	//创建路径
	if (!CFile::GetStatus(currentPath.c_str(), fs))
	{
		CreateDirectory(currentPath.c_str(), 0);
	}

	sprintf(szPath, "%s\\%s", currentPath.c_str(), logName);

	// 创建日志文件	
	if (!CFile::GetStatus(szPath, fs))
	{
		if (m_File.Open(szPath, CFile::modeCreate, &e))
		{
			m_File.Close();
		}
	}
	SYSLOG_SETFILENAME(szPath);
	return 0;
}

int XH_EncodeBase64(char *pASCSrc,char *pBase64Res)
{	int  srcLen;
	char cMap[65]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; 
	char  *pCurSrc,*pCurRes;
	int  i,j,k,group;
	char ch_Asc[4],ch_Base[4],ch,cha;
	
	if(!pASCSrc || !pBase64Res) return -1;

	srcLen = strlen(pASCSrc);
	if(srcLen<1)
	{	*pBase64Res = 0;
		return -1;
	}
	
	group = srcLen/3;
	i = 0;
	pCurSrc = pASCSrc;
	pCurRes = pBase64Res;

	while(i<group)
	{	for(j=0;j<3;j++)
		{	if(pCurSrc[j]!=0)
			{	
		      ch_Asc[j] = *(pCurSrc+j);		
			}
			else
			{	for(k=j;k<4;k++)
				{	ch_Asc[k] = 0;
				}
			}
		}
		ch_Base[0] = ((ch_Asc[0]>>2)&0x3f);
		*pCurRes = cMap[ch_Base[0]];
		
		ch_Base[1] = ((ch_Asc[0]<<4)&0x30);
		ch_Base[1] += ((ch_Asc[1]>>4)&0x0f);
		*(pCurRes+1) = cMap[ch_Base[1]];
		
		ch_Base[2] = (ch_Asc[1]<<2)&0x3c;
		ch_Base[2] += ((ch_Asc[2]>>6)&0x03);
		*(pCurRes+2) = cMap[ch_Base[2]];
		
		ch_Base[3] = (ch_Asc[2]&0x3f);
		*(pCurRes+3) = cMap[ch_Base[3]];
		
		i++;
		pCurSrc+=3;
		pCurRes+=4;
	}

	if((srcLen%3) == 1)
	{	ch = *pCurSrc;
		ch = ch>>2;
		ch = ch&(0x3f);
		*(pCurRes++) = cMap[ch];

		ch = *pCurSrc;
		ch = ch<<4;
		ch = ch&(0x30);
		*(pCurRes++) = cMap[ch];

		*(pCurRes++) = '=';
		*(pCurRes++) = '=';
		*pCurRes = 0;
	}
	else if((srcLen%3) == 2)
	{	ch = *pCurSrc;
		ch = (ch>>2);
		ch = ch&(0x3f);
		*(pCurRes++) = cMap[ch];

		ch  = *pCurSrc;
		cha = *(pCurSrc+1);
		ch  = ch<<4;
		ch  = ch&(0x30);
		cha = (cha>>4);
		cha = cha&(0x0f);
		cha += ch;
		*(pCurRes++) = cMap[cha];

		ch = *(pCurSrc+1);
		ch = ch<<2;
		ch = (ch&(0x3c));
		*(pCurRes++) = cMap[ch];
		*(pCurRes++) = '=';
		*pCurRes = 0;
	}
	else
		*pCurRes = 0;
	return 1;
}

int XH_DecodeBase64(char *pBase64Src,char *pASCRes)
{	int  srcLen;
	char cMap[65]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; 
	char  *pCurSrc,*pCurRes;
	int  i=0,j,group;
	char ch[4],ch_Asc[4];
	
	srcLen = strlen(pBase64Src);
	if(srcLen % 4 != 0) 
	{	*pASCRes = 0;
		return -1;
	}

	group = srcLen/4;
	pCurSrc = pBase64Src;
	pCurRes = pASCRes;

	do
	{	for(j=0;j<4;j++)
		{	ch[j] = *(pCurSrc+j);

			if(ch[j]>='A' && ch[j]<='Z')
			{	ch_Asc[j] = ch[j] - 'A';
			}
			else if(ch[j]>='a' && ch[j]<='z')
			{	ch_Asc[j]  = ch[j] -'a' + 26;
			}	
			else if(ch[j]>='0' && ch[j]<='9')
			{	ch_Asc[j] = ch[j] - '0' + 52;
			}
			else if(ch[j] == '+')
			{	ch_Asc[j] = 62;
			}
			else if(ch[j] == '/')
			{	ch_Asc[j] = 63;
			}
			else if(ch[j] == '=')
			{	ch_Asc[j] = '=';
			}
			else
			{	*pASCRes = 0;
				return -1;
			}
		}	
		*pCurRes = (ch_Asc[0]<<2) + (((ch_Asc[1]&0x30)>>4)&0x0f);

		if(ch[2]!='=')
			*(pCurRes+1) = ((ch_Asc[1]&0x0f)<<4) + (((ch_Asc[2]&0x3c)>>2)&0x3f);
		else
		{	*(pCurRes+1) = (ch_Asc[1]&0x0f)<<4;
			*(pCurRes+2) = 0;
			return 1;
		}
		if(ch[3]!='=')
			*(pCurRes+2) = ((ch_Asc[2]&0x03)<<6) + ch_Asc[3];
		else
		{	*(pCurRes+2) = (ch_Asc[2]&0x03)<<6;
			*(pCurRes+3) = 0;
			return 1;
		}
		i++;
		pCurRes += 3;
		pCurSrc += 4;

	}while(i<group);
	*(pCurRes) = 0;
	return 1;
}


//////////////////////////////////////////////////////////////////////////
// 类型转换
//////////////////////////////////////////////////////////////////////////
template <typename T1, typename T2>
bool type2type(const T1& t1, T2& t2)
{
	std::stringstream str_stream;
	str_stream << t1;
	str_stream >> t2;
	return str_stream.eof() && !str_stream.fail();
}
//////////////////////////////////////////////////////////////////////////
// 判断字符串是否全部由数字组成
//////////////////////////////////////////////////////////////////////////
bool isdigit(const std::string& sSrc)
{
	for (int i = 0; i < sSrc.length(); i++)
	{
		if (!isdigit(sSrc[i]))
		{
			return false;
		}
	}
	
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 去除字符串前的指定字符
//////////////////////////////////////////////////////////////////////////
std::string trim_left(std::string& sSrc, const std::string& sDrop/* = " "*/)
{
	return sSrc.erase(0, sSrc.find_first_not_of(sDrop));
}

//////////////////////////////////////////////////////////////////////////
// 去除字符串后的指定字符
//////////////////////////////////////////////////////////////////////////
std::string trim_right(std::string& sSrc, const std::string& sDrop/* = " "*/)
{
	return sSrc.erase(sSrc.find_last_not_of(sDrop) + 1);
}

//////////////////////////////////////////////////////////////////////////
// 去除字符串前后的指定字符
//////////////////////////////////////////////////////////////////////////
std::string trim(std::string& sSrc,	const std::string& sDrop/* = " "*/)
{
	sSrc.erase(sSrc.find_last_not_of(sDrop) + 1);
	return sSrc.erase(0, sSrc.find_first_not_of(sDrop));
}

//////////////////////////////////////////////////////////////////////////
// 替换字符串
//////////////////////////////////////////////////////////////////////////
int replace(std::string &sSrc, const std::string& sBefore, const std::string& sAfter)
{
	int n = 0;
	std::string::size_type spos = 0;
	std::string::size_type pos  = std::string::npos;

    if (sBefore.length()==0)
    {
		return n;
    }
     
	while (spos < sSrc.size() && (pos = sSrc.find(sBefore, spos)) 
		   != std::string::npos)
	{
		sSrc.replace(pos, sBefore.size(), sAfter);
		n++;
		spos = pos + sAfter.size();
	}
	return n;
}

//////////////////////////////////////////////////////////////////////////
// 字符串左填充
//////////////////////////////////////////////////////////////////////////
bool fill_left(std::string &sSrc, const std::string& sFill, const std::string::size_type iSize)
{
	if (sSrc.size() > iSize)
		return false;
	while (sSrc.size() < iSize)
		sSrc.insert(0, sFill);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 字符串右填充
//////////////////////////////////////////////////////////////////////////
bool fill_right(std::string &sSrc, const std::string& sFill, const std::string::size_type iSize)
{
	if (sSrc.size() > iSize)
		return false;
	while (sSrc.size() < iSize)
		sSrc.append(sFill);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 根据分隔符分割字符串
//////////////////////////////////////////////////////////////////////////
void token(const std::string& sSrc, VecString& vecTokens, const std::string& sSep /*= ","*/, bool bIgnorspace/* = true*/)
{
	std::string sTmp;
	std::string::size_type lastPos = 0;
	std::string::size_type pos = sSrc.find_first_of(sSep, lastPos);
	
	while (std::string::npos != pos)
	{
		sTmp = sSrc.substr(lastPos, pos - lastPos);
		if (!bIgnorspace)
			vecTokens.push_back(sTmp);
		else {
			trim(sTmp);
			if (!sTmp.empty())
				vecTokens.push_back(sTmp);
		}
		lastPos = pos + 1;
		pos = sSrc.find_first_of(sSep, lastPos);
	}
	
	sTmp = sSrc.substr(lastPos);
	if (!bIgnorspace) 
	{
//		if (!sTmp.empty())
			vecTokens.push_back(sTmp);
	}
	else 
	{
		trim(sTmp);
		if (!sTmp.empty())
			vecTokens.push_back(sTmp);
	}
}

std::string to_upper(const std::string sSrc)
{
	std::string sNewStr = sSrc;
	for (int i = 0; i < sNewStr.length(); i++)
	{
		sNewStr[i] = toupper(sNewStr[i]);
	}

	return sNewStr;
}

//////////////////////////////////////////////////////////////////////////
unsigned char BcdToAscii(unsigned char* sSrc, unsigned char* sDest, int iLen)
{
	int i, j;
	unsigned char chTmp1, chTmp2;
	unsigned char szTmp[1024*1000];
    j = 0;
	
	for (i = 0; i < iLen; i++) 
	{
		chTmp1 = sSrc[i];
		
		chTmp2 = chTmp1 >> 4 ;
		if ((chTmp2 > 0x09) && (chTmp2 < 0x10))
			chTmp2 = chTmp2 + 0x07;
		chTmp2 = chTmp2 + 0x30;
		szTmp[j] = chTmp2;
		
		j++;
		
		chTmp1 = sSrc[i];
		chTmp2 = chTmp1 & 0x0f ;
		if ((chTmp2 > 0x09) && (chTmp2 < 0x10))
			chTmp2 = chTmp2 + 0x07;
		chTmp2 = chTmp2 + 0x30;
		szTmp[j] = chTmp2;
		
		j++;
    }
    memcpy(sDest, szTmp, iLen * 2);
    return 0;
}

unsigned char AsciiToBcd(unsigned char* sSrc,unsigned char* sDest, int iLen)
{
//MessageBox(NULL,"1","提示",MB_OK);	
	BYTE btTmp1, btTmp2, btLastByte, btResult[1024*1000];
	int iCpt;
	
	btLastByte = iLen % 2;
	
	int nLen = iLen / 2;
	memset(btResult, 0, nLen + 1);
	for (iCpt = 0; iCpt < nLen; iCpt++){
		btTmp1 = sSrc[2 * iCpt];
		btTmp2 = sSrc[2 * iCpt + 1];
		if ((btTmp1 >= 'A') && (btTmp1 <= 'F'))
			btTmp1 -= 7;
		else{
			if((btTmp1 >= 'a') && (btTmp1 <= 'f'))
				btTmp1 -= 39;
		}
		if ((btTmp2 >= 'A') && (btTmp2 <= 'F'))
			btTmp2 -= 7;
		else{
			if((btTmp2 >= 'a') && (btTmp2<='f'))
				btTmp2 -= 39;
		}
		btResult[iCpt] = (BYTE)((btTmp1 << 4) | (btTmp2 & 0x0F));
	}	
	if (btLastByte){
		btTmp1 = sSrc[2*iCpt];
		if ((btTmp1 >= 'A') && (btTmp1 <= 'F'))
			btTmp1 -= 7;
		else{
			if((btTmp1 >= 'a') && (btTmp1 <= 'f'))
				btTmp1 -= 39;
		}
		btResult[nLen] = (BYTE)(btTmp1 << 4);
	}	
	if (btLastByte) 
		memcpy(sDest, btResult, nLen + 1);
	else
		memcpy(sDest, btResult, nLen);
	return 0;
}

int Hex2ToInt(char* sSrc)
{
	int i = (BYTE)sSrc[0] * 256;
	i += (BYTE)sSrc[1];
    return i;
}

int CRC_16( unsigned char * aData, unsigned long aSize,unsigned char * output )
{
	
	
	//  构造 16 位 CRC 表
	const unsigned short  int cnCRC_16    = 0x8005;
    // CRC-16    = X16 + X15 + X2 + X0
	const unsigned short  int cnCRC_CCITT = 0x1021;
    // CRC-CCITT = X16 + X12 + X5 + X0，据说这个 16 位 CRC 多项式比上一个要好
	const unsigned long  cnCRC_32    = 0x04C10DB7;
    // CRC-32    = X32 + X26 + X23 + X22 + X16 + X11 + X10 + X8 + X7 + X5 + X4 + X2 + X1 + X0
	unsigned long Table_CRC[256]; // CRC 表
    //BuildTable16( cnCRC_16 ); //  or cnCRC_CCITT
	///	BuildTable16(cnCRC_CCITT);
    unsigned short  int ii, j;
    unsigned short  int nData;
    unsigned short  int nAccum1;
    unsigned short  int aPoly=cnCRC_CCITT;
	
    for ( ii = 0; ii < 256; ii++ )
    {
        nData = ( unsigned short  int )( ii << 8 );
        nAccum1 = 0;
        for ( j = 0; j < 8; j++ )
        {
            if ( ( nData ^ nAccum1 ) & 0x8000 )
                nAccum1 = ( nAccum1 << 1 ) ^ aPoly;
            else
                nAccum1 <<= 1;
            nData <<= 1;
        }
        Table_CRC[ii] = ( unsigned long )nAccum1;
    }
	
    unsigned long  i;
    unsigned short  int  nAccum = 0;
	char Tmp_CRC_1[3];

    for ( i = 0; i < aSize; i++ )
        nAccum = ( nAccum << 8 ) ^ ( unsigned short  int )Table_CRC[( nAccum >> 8 ) ^ *aData++];
	
	Tmp_CRC_1[0]=(nAccum>>8)&0xFF;
    Tmp_CRC_1[1]=nAccum&0xFF;
	BcdToAscii((unsigned char *)Tmp_CRC_1,output,2);
    output[4]=0x00;
	return 0;
}

std::string AddCRC(const std::string sData)
{
	unsigned char szCRC[5];
	memset(szCRC, 0x00, sizeof(szCRC));
	
	CRC_16((unsigned char*)sData.c_str(), sData.length(), szCRC);
	
	return sData + (char*)szCRC;
}


int HexToInt(const char *ch)
{
	int resu=0;
	for(int i=0;i<strlen(ch);i++)
	{
		switch (ch[i]){
		case 'a':
		case 'A': resu=resu * 16 + 10;break;
		case 'b':
		case 'B': resu=resu * 16 + 11;break;
		case 'c':
		case 'C': resu=resu * 16 + 12;break;
		case 'd':
		case 'D': resu=resu * 16 + 13;break;
		case 'e':
		case 'E': resu=resu * 16 + 14;break;
		case 'f':
		case 'F': resu=resu * 16 + 15;break;
		default:
		resu=resu * 16 + (ch[i] - '0');
		}
	}
	return resu;

}


bool FindFile(CString m_strPath,VecString &vecfile)
{
    if(m_strPath.GetAt(m_strPath.GetLength() - 1) !='\\')  
    {  
        m_strPath += "\\";  
    }  
	
    if(GetFileAttributes(m_strPath) == -1)  
    {  
        return FALSE;  
    }  
	
    CString sDir = m_strPath;  
    sDir += _T("*.*");  
    WIN32_FIND_DATA fd;  
    HANDLE hFile = FindFirstFile((LPCTSTR)sDir,&fd);  
    if(hFile!=INVALID_HANDLE_VALUE)//查找成功  
    {  
        int index = 1;  
        CString sName = CString(fd.cFileName);  
 
        while(FindNextFile(hFile,&fd))  
        {  
            sName = CString(fd.cFileName);  
			
            //if((FILE_ATTRIBUTE_DIRECTORY&fd.dwFileAttributes) && sName !="." && sName !="..")//是目录  
			if((FILE_ATTRIBUTE_DIRECTORY&fd.dwFileAttributes))//是目录  
            {  
                continue;  
            }  
            else  
            {  
				CString strout=m_strPath+sName;
				vecfile.push_back(strout.GetBuffer(0));
				strout.ReleaseBuffer();
                
            }  
        }  
    } 
	FindClose(hFile);
    return TRUE;  
	
}

std::string DeleteBlank(const std::string sData)
{
	std::string stroutdata="";
	for (int i=0;i<sData.length();i++)
	{
		if (sData.substr(i,1).compare(" ")!=0)
		{
			stroutdata+=sData[i];
		}
	}
	
	return stroutdata;
}

void incstr(std::string &sSrc)
{
	std::string sNewStr = "";
	int ileng=sSrc.length();
	bool bflag=true;
	for (int i=ileng-1;i>=0;i--)
	{
		std::string strtemp=sSrc.substr(i,1);
		if (bflag)
		{
			int itemp=atoi(strtemp.c_str());
			if (itemp<8)
			{
				bflag=false;
				itemp++;
			}
			else
				itemp=0;

			char ch1[2];
			memset(ch1,0x00,sizeof(ch1));
			_itoa(itemp,ch1,10);
			strtemp=(char*)ch1;
			sNewStr=strtemp+sNewStr;
		}
		else
		{
			sNewStr=strtemp+sNewStr;
		}
	}
	sSrc=sNewStr;
}


void fillblank(std::string &sSrc,int inum)
{
	while (sSrc.length()<inum)
	{
		sSrc+=" ";
	}
}

bool Remove80(std::string &sSrc)
{
	if (sSrc.length()%2!=0)
	{
		return false;
	}
	while(sSrc.substr(sSrc.length()-2).compare("00")==0)
	{
		sSrc=sSrc.substr(0,sSrc.length()-2);
	}
	if (sSrc.substr(sSrc.length()-2).compare("80")!=0)
	{
		return false;
	}
	else
		sSrc=sSrc.substr(0,sSrc.length()-2);

	return true;
}

std::string getXHRandom(int inum)
{
	/*
	 std::string strout="";
	 srand( (unsigned)time(NULL));
	 for (int i=0;i<inum;i++)
	 {
		 char szbuf[10];
		 std::string strtemp;
		 memset(szbuf,0x00,sizeof(szbuf));
		 int iran=rand();
		 itoa(iran,szbuf,10);
		 strtemp=szbuf;
		 strout+=strtemp.substr(0,1);
	 }
	 */
	std::string strout="";
	srand( (unsigned)time(NULL));
	for (int i=0;i<inum;i++)
	{
		char szbuf[10];
		std::string strtemp;
		memset(szbuf,0x00,sizeof(szbuf));
		int iran=rand()/16;
		_itoa(iran,szbuf,16);
		strtemp=szbuf;
		strout+=strtemp.substr(0,1);
	}
	return strout;
}

std::string MakeXOR(const char *ch)
{
	 std::string strout="";
	 std::string strtemp="";
	 for (int i=0;i<strlen(ch);i++)
	 {
		 switch(ch[i])
		 {
			case '0' : strtemp='F';break;
			case '1' : strtemp='E';break;
			case '2' : strtemp='D';break;
			case '3' : strtemp='C';break;
			case '4' : strtemp='B';break;
			case '5' : strtemp='A';break;
			case '6' : strtemp='9';break;
			case '7' : strtemp='8';break;
			case '8' : strtemp='7';break;
			case '9' : strtemp='6';break;
			case 'a' : 
			case 'A' : strtemp='5';break;
			case 'b' : 
			case 'B' : strtemp='4';break;
			case 'c' : 
			case 'C' : strtemp='3';break;
			case 'd' : 
			case 'D' : strtemp='2';break;
			case 'e' : 
			case 'E' : strtemp='1';break;
			case 'f' : 
			case 'F' : strtemp='0';break;
			default:
				strtemp="";

		 }
		 strout+=strtemp;

	 }
	 return strout;
}

void StrToHex(unsigned char *bcd, const char *asc, int str_len)
{
	 unsigned char flag;
	 unsigned char is_high, by;
	 int j = 0;
	 
	 is_high = !(str_len % 2);
	 flag = (str_len % 2);
	 bcd[0] = 0x00;
	 
	 for (j=0; j<str_len; j++)
	 {
		 by = asc[j];
		 if (by==' ') by = 0;
		 if ( !(by&0x10) && by) by += 9;
		 
		 if (is_high)  bcd[(j+flag)/2] = by << 4;
		 else
		 {
			 by&=0x0f;
			 bcd[(j+flag)/2] |= by;
		 }
		 is_high = !is_high;
	 }
	 return ;/*BCDLen(j);*/
}

bool CompareFileExt(const char* sFile, const char* ext)
{
	char path_buffer[_MAX_PATH] = {0};    
	char drive[_MAX_DRIVE]      = {0};    
	char dir[_MAX_DIR]          = {0};    
	char fname[_MAX_FNAME]      = {0};    
	char fext[_MAX_EXT]         = {0};

	_splitpath(sFile, drive, dir, fname, fext);

	if (_memicmp(ext,fext,strlen(fext)) != 0)
	{
		return false;
	}
	return true;
}

void ChangeFileExt(const char* sFile, const char* ext, char* nbuf)
{
	char path_buffer[_MAX_PATH] = {0};    
	char drive[_MAX_DRIVE]      = {0};    
	char dir[_MAX_DIR]          = {0};    
	char fname[_MAX_FNAME]      = {0};    
	char fext[_MAX_EXT]         = {0};
	
	_splitpath(sFile, drive, dir, fname, fext);
	
	sprintf(path_buffer,"%s%s%s%s",drive,dir,fname,ext);
	
	strcpy(nbuf,path_buffer);
	
	return;
}

void ChangeFilePath(const char* sFile, const char* npath, char* nbuf)
{
	char path_buffer[_MAX_PATH] = {0};    
	char drive[_MAX_DRIVE]      = {0};    
	char dir[_MAX_DIR]          = {0};    
	char fname[_MAX_FNAME]      = {0};    
	char fext[_MAX_EXT]         = {0};
	
	_splitpath(sFile, drive, dir, fname, fext);
	
	sprintf(path_buffer,"%s%s%s",npath,fname,fext);
	
	strcpy(nbuf,path_buffer);
	
	return;
}

void ExtractFilePath(const char* sFile,char* buf)
{
	char path_buffer[_MAX_PATH] = {0};    
	char drive[_MAX_DRIVE]      = {0};    
	char dir[_MAX_DIR]          = {0};    
	char fname[_MAX_FNAME]      = {0};    
	char fext[_MAX_EXT]         = {0};
	
	_splitpath(sFile, drive, dir, fname, fext);
	
	sprintf(path_buffer,"%s%s",drive,dir);
	
	strcpy(buf,path_buffer);
	
	return;
}

void ExtractFileNameNoExt(const char* sFile,char* buf)
{
	char path_buffer[_MAX_PATH] = {0};    
	char drive[_MAX_DRIVE]      = {0};    
	char dir[_MAX_DIR]          = {0};    
	char fname[_MAX_FNAME]      = {0};    
	char fext[_MAX_EXT]         = {0};
	
	_splitpath(sFile, drive, dir, fname, fext);
	
	sprintf(path_buffer,"%s%s",drive,dir);
	
	strcpy(buf,fname);
	
	return;
}

//注意：当字符串为空时，也会返回一个空字符串  
void SplitString(const std::string& s, std::vector< std::string >& ret, const std::string& delim)  
{  
	std::string::size_type pos1, pos2;
    pos2 = s.find(delim);
    pos1 = 0;
    while(std::string::npos != pos2)
    {
        ret.push_back(s.substr(pos1, pos2-pos1));
		
        pos1 = pos2 + delim.size();
        pos2 = s.find(delim, pos1);
    }
    if(pos1 != s.length())
        ret.push_back(s.substr(pos1));
}  

void CreateDir(const char *dir)
{
    int m = 0, n;
	std::string str1, str2;
    
    str1 = dir;
    str2 = str1.substr( 0, 2 );
    str1 = str1.substr( 3, str1.size() );
    
    while( m >= 0 )
    {
        m = str1.find('\\');
		
        str2 += '\\' + str1.substr( 0, m );    
        n = _access( str2.c_str(), 0 ); //判断该目录是否存在
        if( n == -1 )
        {
            _mkdir( str2.c_str() );     //创建目录
        }
        
        str1 = str1.substr( m+1, str1.size() );
    }
}

/*单个字符转十六进制字符串，长度增大2被*/
void Char2Hex(unsigned char ch, char *szHex)
{
    int i;
    unsigned char byte[2];
    byte[0] = ch/16;
    byte[1] = ch%16;
    for(i=0; i<2; i++)
    {
        if(byte[i] >= 0 && byte[i] <= 9)
            szHex[i] = '0' + byte[i];
        else
            szHex[i] = 'a' + byte[i] - 10;
    }
    szHex[2] = 0;
}

void HexStr2CharStr(char *pszHexStr, char *pucCharStr, int iSize)
{
    int i;
    unsigned char ch;
    if (iSize%2 != 0) return;
    for(i=0; i<iSize/2; i++)
    {
        Hex2Char(pszHexStr+2*i, &ch);
        pucCharStr[i] = ch;
    }
}

/*两个字符转换成一个字符，长度为原来的1/2*/
void Hex2Char(char *szHex, unsigned char *rch)
{
    int i;
    for(i=0; i<2; i++)
    {
        if(*(szHex + i) >='0' && *(szHex + i) <= '9')
            *rch = (*rch << 4) + (*(szHex + i) - '0');
        else if(*(szHex + i) >='a' && *(szHex + i) <= 'f')
            *rch = (*rch << 4) + (*(szHex + i) - 'a' + 10);
        else
            break;
    }
}

void CharStr2HexStr(char *pucCharStr, char *pszHexStr, int iSize) //1234 -> 31323334
{
    int i;
    char szHex[3];
    pszHexStr[0] = 0;
    for(i=0; i<iSize; i++)
    {
        Char2Hex(pucCharStr[i], szHex);
        strcat(pszHexStr, szHex);
    }
}

void EncryptStr(char* src, char* dest)
{
	char temp[1024];
	memset(temp,0x00,sizeof(temp));   
	XH_EncodeBase64(src,temp);
    CharStr2HexStr(temp,dest,strlen(temp));
}

void DecryptStr(char* src, char* dest)
{
	char temp[1024];
	memset(temp,0x00,sizeof(temp));
    HexStr2CharStr(src,temp,strlen(src));
	XH_DecodeBase64(temp,dest);
}
std::string WcharToChar(const wchar_t* wp, size_t m_encode) // 宽字节转换窄字节
{
	std::string str;
	int len = WideCharToMultiByte(m_encode, 0, wp, wcslen(wp), NULL, 0, NULL, NULL);
	char *m_char = new char[len + 1];
	WideCharToMultiByte(m_encode, 0, wp, wcslen(wp), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	str = m_char;
	delete m_char;
	return str;
}
std::string transcoding2utf8(char * pStr) //解决中文乱码问题
{
	int nBufferSize = MultiByteToWideChar(CP_UTF8, 0, pStr, -1, NULL, 0); //取得所需缓存的多少
	wchar_t *pBuffer = (wchar_t*)malloc(nBufferSize * sizeof(wchar_t));//申请缓存空间
	MultiByteToWideChar(CP_UTF8, 0, pStr, -1, pBuffer, nBufferSize*sizeof(wchar_t));//转码
	return WcharToChar(pBuffer).c_str();
}
std::string getIniFieldInfor(char *section, char *keyName, char *iniPathName)
{
	char chTemp[LENGTH_B];
	memset(chTemp, 0x00, sizeof(chTemp));
	GetPrivateProfileString(section, keyName, NULL, chTemp, LENGTH_B, iniPathName);
	return chTemp;
}

CString getCurrentPath()
{
	CString strCurPath, returnStr;
	GetModuleFileName(NULL, strCurPath.GetBuffer(MAX_PATH), MAX_PATH);
	strCurPath.ReleaseBuffer(MAX_PATH);
	returnStr = strCurPath.Mid(0, strCurPath.ReverseFind('\\'));
	return returnStr;
}
//std::string getCurrentPath_string()
//{
//	std::string strCurPath, returnStr;
//	GetModuleFileName(NULL, (char*)strCurPath.c_str(), MAX_PATH);
//	returnStr = strCurPath.substr(0, strCurPath.rfind("\\"));
//	return returnStr;
//}
/**
* @brief ascii convert hex
* @par param[in] *hex:hex data
* @par param[in] *ascii:ascii data
* @par param[in] asciiLen:length of ascii
* @retval length
*/
uint8_t Ascii2Hex(uint8_t *hex, uint8_t *ascii, uint8_t asciiLen)
{
	uint8_t i, ch, value;
	value = 0;
	for (i = 0; i < (asciiLen >> 1); i++) {
		ch = CHAR_TO_UPPER(ascii[i * 2]);
		if (ch >= '0' && ch <= '9') {
			value = ch - '0';
		}
		else if (ch >= 'A' && ch <= 'F') {
			value = ch - 'A' + 0x0A;
		}
		else {
			return i;
		}
		hex[i] = (value << 4);
		ch = CHAR_TO_UPPER(ascii[i * 2 + 1]);
		if (ch >= '0' && ch <= '9') {
			value = ch - '0';
		}
		else if (ch >= 'A' && ch <= 'F') {
			value = ch - 'A' + 0x0A;
		}
		else {
			return i;
		}
		hex[i] += value;
	}
	return i;
}
/**
* @brief hex convert ascii
* @par param[in] *ascii:ascii data
* @par param[in] *hex:hex data
* @par param[in] hexLen:length of hex
* @retval length
*/
uint8_t Hex2Ascii(uint8_t *ascii, uint8_t *hex, uint8_t hexLen) //0x0001 -> 30303031
{
	uint8_t i, value;
	for (i = 0; i < hexLen; i++) {
		value = (hex[i] >> 4);
		if (value > 9) {
			value += 0x07;
		}
		ascii[2 * i] = value + 0x30;
		value = (hex[i] & 0x0F);
		if (value > 9) {
			value += 0x07;
		}
		ascii[2 * i + 1] = value + 0x30;
	}
	return hexLen * 2;
}

std::string uint8Str2HexStr(uint8_t *src, int len) //0000 -> 00 00
{
	std::string destStr = "";
	for (size_t i = 0; i < len; i++)
	{
		char chTemp[3] = { 0 };
		char chHpos = NULL, chLpos = NULL;
		char chTemp1 = NULL;
		char chTemp2 = NULL;
		memcpy(&chHpos, &src[i], 1);
		memcpy(&chLpos, &src[i], 1);
		chTemp1 = ((chHpos >> 4) & 0x0f);
		chTemp2 = (chLpos & 0x0f);
		if (chTemp1>=0x00 && chTemp1 <=0x09)
		{
			chTemp[0] = chTemp1 + 48;
		}
		else if (chTemp1 >= 0x0A && chTemp1 <= 0x0F)
		{
			chTemp[0] = chTemp1 + 55;
		}
		else
		{
			chTemp[0] = chTemp1;
		}

		if (chTemp2 >= 0x00 && chTemp2 <= 0x09)
		{
			chTemp[1] = chTemp2 + 48;
		}
		else if (chTemp2 >= 0x0A && chTemp2 <= 0x0F)
		{
			chTemp[1] = chTemp2 + 55;
		}
		else
		{
			chTemp[1] = chTemp2;
		}
		chTemp[2] = '\0';
		destStr += chTemp;
		destStr += " ";
	}
	return destStr.c_str();
}

std::string uint8Str2HexStr_noSpacer(uint8_t *src, int len) //0000 -> 00 00
{
	std::string destStr = "";
	for (size_t i = 0; i < len; i++)
	{
		char chTemp[3] = { 0 };
		char chHpos = NULL, chLpos = NULL;
		char chTemp1 = NULL;
		char chTemp2 = NULL;
		memcpy(&chHpos, &src[i], 1);
		memcpy(&chLpos, &src[i], 1);
		chTemp1 = ((chHpos >> 4) & 0x0f);
		chTemp2 = (chLpos & 0x0f);
		if (chTemp1 >= 0x00 && chTemp1 <= 0x09)
		{
			chTemp[0] = chTemp1 + 48;
		}
		else if (chTemp1 >= 0x0A && chTemp1 <= 0x0F)
		{
			chTemp[0] = chTemp1 + 55;
		}
		else
		{
			chTemp[0] = chTemp1;
		}

		if (chTemp2 >= 0x00 && chTemp2 <= 0x09)
		{
			chTemp[1] = chTemp2 + 48;
		}
		else if (chTemp2 >= 0x0A && chTemp2 <= 0x0F)
		{
			chTemp[1] = chTemp2 + 55;
		}
		else
		{
			chTemp[1] = chTemp2;
		}
		chTemp[2] = '\0';
		destStr += chTemp;
	}
	return destStr.c_str();
}
// std::string 转换为 UTF-8 编码
std::string string_To_UTF8(const std::string & str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴 
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}
std::wstring AnsiToUnicode(const char* buf) //中文 UTF16BE编码（"欢迎" -> FEFF6B228FCE）
{
	int len = ::MultiByteToWideChar(CP_ACP, 0, buf, -1, NULL, 0);
	if (len == 0) return L"";

	std::vector<wchar_t> unicode(len);
	::MultiByteToWideChar(CP_ACP, 0, buf, -1, &unicode[0], len);

	return &unicode[0];
}

std::string UnicodeToAnsi(const wchar_t* buf)
{
	int len = ::WideCharToMultiByte(CP_ACP, 0, buf, -1, NULL, 0, NULL, NULL);
	if (len == 0) return "";

	std::vector<char> utf8(len);
	::WideCharToMultiByte(CP_ACP, 0, buf, -1, &utf8[0], len, NULL, NULL);

	return &utf8[0];
}

std::wstring Utf8ToUnicode(const char* buf) //英文 UTF8编码（"well" -> 77656C6C）
{
	int len = ::MultiByteToWideChar(CP_UTF8, 0, buf, -1, NULL, 0);
	if (len == 0) return L"";

	std::vector<wchar_t> unicode(len);
	::MultiByteToWideChar(CP_UTF8, 0, buf, -1, &unicode[0], len);

	return &unicode[0];
}

std::string UnicodeToUtf8(const wchar_t* buf)
{
	int len = ::WideCharToMultiByte(CP_UTF8, 0, buf, -1, NULL, 0, NULL, NULL);
	if (len == 0) return "";

	std::vector<char> utf8(len);
	::WideCharToMultiByte(CP_UTF8, 0, buf, -1, &utf8[0], len, NULL, NULL);

	return &utf8[0];
}
//返回0：无中文，返回1：有中文
int IncludeChinese(char *str)
{
	char c;
	while (1)
	{
		c = *str++;
		if (c == 0) break;  //如果到字符串尾则说明该字符串没有中文字符
		if (c & 0x80)        //如果字符高位为1且下一字符高位也是1则有中文字符
			if (*str & 0x80) return 1;
	}
	return 0;
}

CString GetGmtTime()
{
	CString str;
	CTime tm; tm = CTime::GetCurrentTime();
	str = tm.Format("%Y-%m-%d %X");
	return str;
}

vector<NetConfig> GetNetworkConfig()
{
	vector<NetConfig> vecNetConfig;
	//PIP_ADAPTER_INFO结构体指针存储本机网卡信息
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	//得到结构体大小,用于GetAdaptersInfo参数
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	//调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	//记录网卡数量
	DWORD netCardNum = 0;
	GetNumberOfInterfaces(&netCardNum);
	//cout << "网卡数量：" << netCardNum << endl; netCardNum = 0;
	//记录每张网卡上的IP地址数量
	int IPnumPerNetCard = 0;
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		//如果函数返回的是ERROR_BUFFER_OVERFLOW
		//则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小
		//这也是说明为什么stSize既是一个输入量也是一个输出量
		//释放原来的内存空间
		delete pIpAdapterInfo;
		//重新申请内存空间用来存储所有网卡信息
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		//再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量
		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	}
	if (ERROR_SUCCESS == nRel)
	{
		//输出网卡信息
		//可能有多网卡,因此通过循环去判断
		while (pIpAdapterInfo)
		{
			//cout << "网卡序号：" << ++netCardNum << "\t" << pIpAdapterInfo->Index << endl;
			NetConfig netConfig;
			netConfig.netWorkCardName = pIpAdapterInfo->AdapterName;//网卡名称
			netConfig.netWorkCardDescription = pIpAdapterInfo->Description;//网卡描述
			std::string netType = "";
			switch (pIpAdapterInfo->Type)//网卡类型
			{
			case MIB_IF_TYPE_OTHER:        netType = "OTHER"; break;
			case MIB_IF_TYPE_ETHERNET:    netType = "ETHERNET"; break;
			case MIB_IF_TYPE_TOKENRING:    netType = "TOKENRING"; break;
			case MIB_IF_TYPE_FDDI:        netType = "FDDI"; break;
			case MIB_IF_TYPE_PPP:        netType = "PPP"; break;
			case MIB_IF_TYPE_LOOPBACK:    netType = "LOOPBACK"; break;
			case MIB_IF_TYPE_SLIP:        netType = "SLIP"; break;
			default:                    netType = ""; break;
			}
			netConfig.netWorkCardType = netType;
			char chMac[18] = { 0 };
			int macIndex = 0;

			//网卡MAC地址
			for (DWORD i = 0; i < pIpAdapterInfo->AddressLength; i++)
			{
				if (i < pIpAdapterInfo->AddressLength - 1)
				{
					sprintf(chMac + macIndex * 3, "%02X-", pIpAdapterInfo->Address[i]);
				}
				else
				{
					sprintf(chMac + macIndex * 3, "%02X", pIpAdapterInfo->Address[i]);
				}
				macIndex++;
			}
			chMac[17] = 0;
			netConfig.netWorkMac = chMac;
			//网卡IP地址如下
			IPnumPerNetCard = 0;
			//可能网卡有多IP,因此通过循环去判断
			IP_ADDR_STRING* pIpAddrString = &(pIpAdapterInfo->IpAddressList);
			do
			{

				netConfig.networkCardNum = ++IPnumPerNetCard; //网卡上的IP计数
				netConfig.ipAddress = pIpAddrString->IpAddress.String;//IP 地址
				netConfig.subnetMask = pIpAddrString->IpMask.String;//子网掩码
				netConfig.gateway = pIpAdapterInfo->GatewayList.IpAddress.String;//网关地址
				vecNetConfig.push_back(netConfig);
				pIpAddrString = pIpAddrString->Next;
			} while (pIpAddrString);
			pIpAdapterInfo = pIpAdapterInfo->Next;
		}

	}
	//释放内存空间
	if (pIpAdapterInfo)
	{
		delete  pIpAdapterInfo;
	}
	return vecNetConfig;
}
std::string get_pathOnDla(std::string filter) {
	OPENFILENAME ofn;
	char szFile[300];

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = (LPSTR)szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nFilterIndex = 1;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = filter.c_str();//"xml file(*.xml)\0*.xml\0ALL file(*.*)\0*.*\0\0";
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;

	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	std::string path_image = "";
	if (GetOpenFileName(&ofn)) {
		return ofn.lpstrFile;
	}
	else {
		return "";
	}
}