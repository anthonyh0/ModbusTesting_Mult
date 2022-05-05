// PublicFunDef.h: interface for the CPublicFunDef class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PUBLICFUNDEF_H__66CA3927_EBD6_460A_A2C8_0DF9CEA039A4__INCLUDED_)
#define AFX_PUBLICFUNDEF_H__66CA3927_EBD6_460A_A2C8_0DF9CEA039A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afx.h>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <time.h>
#include <direct.h> //_mkdir������ͷ�ļ�
#include <io.h>     //_access������ͷ�ļ�
#include <windows.h>
#include "SysLogs.h"
#include <Winnetwk.h>
#include <WinSock2.h>
#include <Iphlpapi.h>
#include <iostream>
#include "afxdialogex.h"

#pragma comment(lib,"Iphlpapi.lib") //��Ҫ���Iphlpapi.lib��
#pragma comment(lib, "mpr.lib")
using std::vector;
typedef vector<std::string> VecString;
typedef unsigned char  uint8_t;

#define LENGTH_A 2046
#define LENGTH_B 4096
#define CHAR_TO_UPPER(ch)   ((ch >= 'a' && ch <= 'z')?(ch-0x20):ch)


#ifdef _DEBUG
	static void __dbg_printf (const char* format,...)   
	{       
		char buf[1024*1024];       
		va_list ap;       
		va_start(ap, format);       
		_vsnprintf(buf, sizeof(buf), format, ap);       
		va_end(ap);       
		OutputDebugString(buf);   
	}  
#else   
    static void __dbg_printf (const char * format,...) {}   
#endif  

struct NetConfig
{
	int networkCardNum; //ID
	std::string netWorkCardName; //��������
	std::string netWorkCardDescription; //��������
	std::string netWorkCardType; //��������
	std::string netWorkMac; //����MAC��ַ
	std::string ipAddress; //IP��ַ
	std::string subnetMask; //��������
	std::string gateway; //����
};

int				Asc2Hex(char *Dest,char *Src,int SrcLen);   // Asscii -> Hex
int				Hex2Asc(char *Dest,char *Src,int SrcLen);	 // Hex -> Asscii

CString			UT2WC(const char* buf);    //UTF-8ת����UNICODE
CString			WC2UT(const wchar_t* buf); //UNICODEת����UTF-8 
CString			MB2WC(const char* buf);    //ANSIת����UNICODE
CString			WC2MB(const wchar_t* buf); //UNICODEת����ANSI
void			Utf2GBK(CString &buffer);  //UTF-8��ת��UNICODE
void			GBK2Utf(CString &buffer);  //UNICODE��ת��UTF-8

CString			DeleteIllegalChar(CString indata);      //ɾ�����ɼ��ַ�
CString			StrSwap(CString  indata);				//CString�ַ�����
CString			getCurrentPath();						//��ȡ��ǰ·��CString
//std::string		getCurrentPath_string();
vector<CString> split(CString str,CString pattern);		//CString�ָ�

BOOL			ConnectIPC(const char* IP, const char* path, const char* User, const char* PassWord);	//����Զ��PC
BOOL			CloseIPC(const char* IP, const char* path);												//�ر�Զ��PC
BOOL			GetComputerUser(char* name, char* user);												//��ȡPC��
BOOL			ConnectRemotePC(const char* IP, const char* path, const char* user, const char* pass);  //Զ��������־PC
BOOL			IsHexString(const char* str);															//�ж��Ƿ�HEX�ַ���

int				XH_EncodeBase64(char *pASCSrc,char *pBase64Res);					//base64����
int				XH_DecodeBase64(char *pBase64Src,char *pASCRes);					//base64����
int				ITC_WriteLog(int _level_, LPCTSTR Errinfo, ...);					//д��־����
int				initializeLog(char* szpath);										//��ʼ����־�ļ�
//new
bool			isdigit(const std::string& sSrc);									//�ж��ַ���Ϊ������
std::string		trim_left(std::string& sSrc, const std::string& sDrop = " ");		//string�������ո�tab
std::string		trim_right(std::string& sSrc, const std::string& sDrop = " ");		//string�����Ҳ�ո�tab
std::string		trim(std::string& sSrc,	const std::string& sDrop = " ");			//string��������ո�tab
int				replace(std::string &sSrc, const std::string& sBefore, const std::string& sAfter);								//string�滻�ַ�
bool			fill_left(std::string &sSrc, const std::string& sFill, const std::string::size_type iSize);						//string�������ַ�
bool			fill_right(std::string &sSrc, const std::string& sFill, const std::string::size_type iSize);					//stirng�Ҳ�����ַ�
void			token(const std::string& sSrc, VecString& vecTokens, const std::string& sSep = ",", bool bIgnorspace = true);	//string���ݷָ����ָ��ַ���
std::string		to_upper(const std::string sSrc);																				//ת��д
unsigned char	BcdToAscii(unsigned char* sSrc, unsigned char* sDest, int iLen);												//BCDתASCII
unsigned char	AsciiToBcd(unsigned char* sSrc,unsigned char* sDest, int iLen);													//ASCIIתBCD
int				Hex2ToInt(char* sSrc);
int				HexToInt(const char *ch);
int				CRC_16( unsigned char * aData, unsigned long aSize,unsigned char * output );
std::string		AddCRC(const std::string sData);
bool			FindFile(CString m_strPath,VecString &vecfile);
std::string		DeleteBlank(const std::string sData);
void			incstr(std::string &sSrc);
void			fillblank(std::string &sSrc,int inum);
bool			Remove80(std::string &sSrc);
std::string		getXHRandom(int inum);
std::string		MakeXOR(const char *ch);
void			StrToHex(unsigned char *bcd, const char *asc, int str_len);
void			SplitString(const std::string& s, std::vector< std::string >& ret, const std::string& delim);
bool			CompareFileExt(const char* sFile, const char* ext);
std::string		swapstring(const std::string buf);
void			DelillegalChar(const char* str,char* buf);
void			ChangeFileExt(const char* sFile, const char* ext, char* nbuf);
void			ChangeFilePath(const char* sFile, const char* npath, char* nbuf);
void			ExtractFilePath(const char* sFile,char* buf);
void			ExtractFileNameNoExt(const char* sFile,char* buf);
void			CreateDir(const char *dir);
void			Char2Hex(unsigned char ch, char *szHex);
void			HexStr2CharStr(char *pszHexStr, char *pucCharStr, int iSize);
void			Hex2Char(char *szHex, unsigned char *rch);
void			CharStr2HexStr(char *pucCharStr, char *pszHexStr, int iSize); //1234 -> 31323334
void			EncryptStr(char* src, char* dest);
void			DecryptStr(char* src, char* dest);

VecString		vSplitString(std::string str, std::string pattern);
std::string		WcharToChar(const wchar_t* wp, size_t m_encode = CP_ACP); // ���ֽ�ת��խ�ֽ�
std::string		transcoding2utf8(char * pStr); //���������������
std::string		getIniFieldInfor(char *section, char *keyName, char *iniPathName);

uint8_t			Hex2Ascii(uint8_t *ascii, uint8_t *hex, uint8_t hexLen);
uint8_t			Ascii2Hex(uint8_t *hex, uint8_t *ascii, uint8_t asciiLen);
std::string		uint8Str2HexStr(uint8_t *src, int len); //0001 -> 00 01
std::string		uint8Str2HexStr_noSpacer(uint8_t *src, int len); //uint8_t 0000 -> 0000
std::string		string_To_UTF8(const std::string & str); // UTF8���루"well" -> 77656C6C��
std::wstring	AnsiToUnicode(const char* buf); //���� UTF16BE���루"��ӭ" -> FEFF6B228FCE��
std::string		UnicodeToAnsi(const wchar_t* buf);
std::wstring	Utf8ToUnicode(const char* buf); //Ӣ�� UTF8���루"well" -> 77656C6C��
std::string		UnicodeToUtf8(const wchar_t* buf);
int				IncludeChinese(char *str);
CString			GetGmtTime(); //yyyy-mm-dd hh:MM:ss
vector<NetConfig> GetNetworkConfig();//��ȡ������Ϣ
std::string		get_pathOnDla(std::string filter);//�򿪶Ի�����ѡ���ļ�


#endif // !defined(AFX_PUBLICFUNDEF_H__66CA3927_EBD6_460A_A2C8_0DF9CEA039A4__INCLUDED_)
