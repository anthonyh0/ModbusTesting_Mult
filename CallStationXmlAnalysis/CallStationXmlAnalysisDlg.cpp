
// CallStationXmlAnalysisDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CallStationXmlAnalysis.h"
#include "CallStationXmlAnalysisDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <fstream> 
#include "INI.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CCallStationXmlAnalysisDlg 对话框



CCallStationXmlAnalysisDlg::CCallStationXmlAnalysisDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCallStationXmlAnalysisDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCallStationXmlAnalysisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INPXML, m_myedit_xmlInput);
	DDX_Control(pDX, IDC_EDIT_BROWSE, m_edit_browse);
	DDX_Control(pDX, IDC_EDIT_INFO, m_edit_log);
	DDX_Control(pDX, IDC_BUTTON_TCPCONNECT, m_button_tcpConnect);
	DDX_Control(pDX, IDC_COMBO_PROCOTOL, m_combo_procotol);
	DDX_Control(pDX, IDC_COMBO_IP, m_combo_ip);
	DDX_Control(pDX, IDC_COMBO_PORT, m_combo_port);
	DDX_Control(pDX, IDC_LIST_HISTORY, m_listControl_history);
}

BEGIN_MESSAGE_MAP(CCallStationXmlAnalysisDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PARSER, &CCallStationXmlAnalysisDlg::OnBnClickedButtonParser)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CCallStationXmlAnalysisDlg::OnBnClickedButtonBrowse)
	ON_EN_CHANGE(IDC_EDIT_INPXML, &CCallStationXmlAnalysisDlg::OnEnChangeEditInpxml)
	ON_BN_CLICKED(IDC_BUTTON_TCPCONNECT, &CCallStationXmlAnalysisDlg::OnBnClickedButtonTcpconnect)
	ON_BN_CLICKED(IDC_BUTTON1, &CCallStationXmlAnalysisDlg::OnBnClickedButton1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_HISTORY, &CCallStationXmlAnalysisDlg::OnNMDblclkListHistory)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CCallStationXmlAnalysisDlg 消息处理程序

BOOL CCallStationXmlAnalysisDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	idIndex = 0;
	m_ToolTip.Create(this);
	m_ToolTip.AddTool(GetDlgItem(IDC_EDIT_INPXML), _T("[可拖动XML文件到这]"));

	m_combo_procotol.ResetContent();
	m_combo_procotol.AddString("TCP Server");
	m_combo_procotol.SetCurSel(0);

	m_combo_port.ResetContent();
	m_combo_ip.ResetContent();

	for each (std::string var in g_CINI.vecIp)
	{
		m_combo_ip.AddString(var.c_str());
	}
	m_combo_ip.SetCurSel(0);

	for each (std::string var in g_CINI.vecPort)
	{
		m_combo_port.AddString(var.c_str());
	}
	m_combo_port.SetCurSel(0);

	m_listControl_history.ModifyStyle(0, LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS);
	m_listControl_history.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);
	//m_listControl_stationList.SetTextColor(RGB(255, 0, 0));                  //显示字体的颜色
	DWORD dwStyle = m_listControl_history.GetStyle();
	dwStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_SHOWSELALWAYS;
	m_listControl_history.SetExtendedStyle(dwStyle);


	m_listControl_history.InsertColumn(0, "ID", LVCFMT_CENTER, 60, 0);
	m_listControl_history.InsertColumn(1, "信息", LVCFMT_LEFT, 500, 1);
	lastNum = 0;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCallStationXmlAnalysisDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCallStationXmlAnalysisDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCallStationXmlAnalysisDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//将一个LPCWSTR转换为string
string Lpcwstr2String(LPCWSTR lps) {
	int len = WideCharToMultiByte(CP_ACP, 0, lps, -1, NULL, 0, NULL, NULL);
	if (len <= 0) {
		return "";
	}
	else {
		char *dest = new char[len];
		WideCharToMultiByte(CP_ACP, 0, lps, -1, dest, len, NULL, NULL);
		dest[len - 1] = 0;
		string str(dest);
		delete[] dest;
		return str;
	}
}

//新建一个对话窗口，选择文件
string get_path() {
	OPENFILENAME ofn;
	char szFile[300];

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = (LPSTR)szFile;
	ofn.lpstrFile[0] = '\0';
	LPTSTR        lpstrCustomFilter;
	DWORD         nMaxCustFilter;
	ofn.nFilterIndex = 1;
	LPTSTR        lpstrFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "xml file(*.xml)\0*.xml\0ALL file(*.*)\0*.*\0\0";;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;

	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	string path_image = "";
	if (GetOpenFileName(&ofn)) {
		return ofn.lpstrFile;
	}
	else {
		return "";
	}
}

BOOL CCallStationXmlAnalysisDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_NCLBUTTONDBLCLK) { return TRUE; }
	ASSERT(pMsg != NULL);
	if (pMsg->message == WM_MOUSEMOVE ||
		pMsg->message == WM_LBUTTONDOWN ||
		pMsg->message == WM_LBUTTONUP)
	{
		m_ToolTip.RelayEvent(pMsg);
	}

	return CDialog::PreTranslateMessage(pMsg);
	//return CDialogEx::PreTranslateMessage(pMsg);
}


/*
<MSG>		根标签
<META>		消息头标签
<SNDR>		消息发送者
<DDTM>		发送时间
<TYPE>		消息类别
<RECV>		消息接收者M
<TIME>		校时时间M
<RQAR>		请求分区头标签M
<RQID>		请求分区消息ID
<BCAR>		请求分区头标签 M
<AREA>		单个分区 M
<CHAN>		输出通道标识
<MACR>		请求分区宏ID
<REST>		总分区请求回馈 #见（分区请求回馈代码表）
<REAR>		分区请求回复头标签 M
<ARST>		单分区请求回馈M
<SCAR>		播放分区头标签M
<RESC>		分区反馈头标签M
<CLAR> 		分区关闭头标签M
<RECL>		分区关闭反馈消息头标签 M
<ITAR>		分区打断消息头标签 M
<RESN>		打断原因M
<RQFL>		航班信息请求消息头标签
<RQTP>		请求种类
<BDGT>		登机口编号 O
<FLNO>		航班号 0
<STAT>		航班信息回馈消息头标签
<FLHT>		单条航班信息头标签M
<FLID>		航班ID
<FLNO>		主航班号二字码
<SFLG>		共享航班组标签
<SFNO>		共享航班号，如果有<SFLT>则M
<FLRT>		航线
<FLBG>		登机口
<FLST>		航班状态 如果<FLHT>则为M
<IVAB>		自动广播调用消息头标签M
<ABTI>		自动广播播放次数 M
<LASL>		自动广播语种代码 M
<ABTP>		自动广播种类代码 M
<TTST>		TTS广播文本 M
<ABST>		自动广播调用回馈
<STRS>		调用预存消息头标签
<STST>		存储转发回复状态M
<IBGM>		调用预存消息头标签
<CHNL>		背景音源通道
<IVST>		背景音调用回馈状态
<EVNT>		火警告知消息头标签
<ORIN>		火警发起者
<EVST>		火警状态
<CMED>		呼叫宏结束消息头标签

*/

map<string, string> mapTagExplain = {
	{ "MSG", "根标签" }, { "META", "消息头标签" }, { "SNDR", "消息发送者" }, { "DDTM", "发送时间" }, 
	{ "TYPE", "消息类别" }, { "RECV", "消息接收者" }, { "TIME", "校时时间" },{ "RQAR", "请求分区头标签" }, 
	{ "RQID", "请求分区消息" }, { "BCAR", "请求分区头标签" }, { "AREA", "单个分区" }, { "CHAN", "输出通道标识" }, 
	{ "MACR", "请求分区宏ID" }, { "REST", "总分区请求回馈" },{ "REAR", "分区请求回复头标签" }, { "ARST", "单分区请求回馈" }, 
	{ "SCAR", "播放分区头标签" }, { "RESC", "分区反馈头标签" }, { "CLAR", "分区关闭头标签" }, { "RECL", "分区关闭反馈消息头标签" }, 
	{ "ITAR", "分区打断消息头标签" },{ "RESN", "打断原因" }, { "RQFL", "航班信息请求消息头标签" }, { "RQTP", "请求种类" }, { "BDGT", "登机口编号" }, 
	{ "FLNO", "航班号" }, { "STAT", "航班信息回馈消息头标签" }, { "FLHT", "单条航班信息头标签" }, { "FLID", "航班ID" },{ "FLNO", "主航班号二字码" }, 
	{ "SFLG", "共享航班组标签" }, { "SFNO", "共享航班号" }, { "FLRT", "航线" }, { "FLBG", "登机口" }, { "FLST", "航班状态" }, { "IVAB", "自动广播调用消息头标签" }, 
	{ "ABTI", "自动广播播放次数" }, { "LASL", "自动广播语种代码" }, { "ABTP", "自动广播种类代码" },{ "TTST", "TTS广播文本" }, { "ABST", "自动广播调用回馈" }, 
	{ "STRS", "调用预存消息头标签" }, { "STST", "存储转发回复状态" }, { "IBGM", "调用预存消息头标签" }, { "CHNL", "背景音源通道" }, { "IVST", "背景音调用回馈状态" }, 
	{ "EVNT", "火警告知消息头标签" }, { "ORIN", "火警发起者" }, { "EVST", "火警状态" }, { "EVAC", "火警告知消息" },
	{ "CMED", "呼叫宏结束消息头标签" }, { "REIV", "自动广播调用回馈消息" }, {"REBG","调用背景音反馈消息"}
};
map<string, string> mapRerq_arst = {
	{ "100", "分区请求成功，可以进行播放" }, { "200", "请求失败，参数异常" }, 
	{ "300", "部分请求分区空闲，可以播放" }, { "400", "分区请求失败，分区繁忙" }, { "500", "其他错误" },
};
map<string, string> mapResc_arst = {
	{ "200", "分区播放成功" }, { "201", "分区播放失败，失去连接" }, { "202", "分区播放失败，参数错误" },
	{ "203", "分区播放失败，分区繁忙" }, { "204", "分区播放失败，呼叫不存在" }, { "205", "分区播放失败，其他错误" },
};
map<string, string> mapItar_resn = {
	{ "500", "高优先级广播打断此分区" }, { "501", "音频通道断开" }, { "502", "基础矩阵失去响应" },
	{ "599", "其他原因" }
};
map<string, string> mapIvab_abtp = {
	{ "1000", "正常登机" }, { "1001", "催促登机" }, { "1002", "过站登机" },
	{ "1003", "催促过站登机" }, { "1004", "登机口变更" }, { "1005", "航班延误" }, { "1006", "航班取消" }
};
map<string, string> mapReiv_abst = {
	{ "2000", "调用成功" }, { "2001", "非法航班ID" }, { "2002", "非法自动广播种类代码" }, { "2003", "非法分区代码" },
	{ "2004", "所请求自动广播种类与航班状态不符" }, { "2005", "自动广播处理错误" }, { "2006", "自动广播不可达" }, { "2007", "非法自动广播播放次数" }, 
	{ "2008", "非法自动广播语种代码" }
};
map<string, string> mapRebg_ivst = {
	{ "100", "调用成功" }, { "200", "调用失败，参数异常" }, { "500", "其他错误" }
};
map<string, string> mapRest_stst = {
	{ "OK", "可以进行呼叫存储转发" }, { "ABORT", "放弃呼叫存储转发" }
};
map<string, string> mapEvac_evst = {
	{ "ON", "火警发生" }, { "OFF", "火警解除" }
};
map<string, map<string, string>> mapFeedbackMessage = {
	{ "RERQ", mapRerq_arst }, { "RESC", mapResc_arst }, { "ITAR", mapItar_resn }, { "TVAB", mapIvab_abtp }, { "REIV", mapReiv_abst },
	{"REBG", mapRebg_ivst}, { "REST", mapRest_stst }, { "EVAC", mapEvac_evst }
};
//编译器太卡了，还没有整理代码，比较乱。（2021年5月29日14:09:21）
void CCallStationXmlAnalysisDlg::OnBnClickedButtonParser()
{
	CString outmsg,inputXml,inputPath;
	m_edit_browse.GetWindowText(inputPath);
	TiXmlDocument *mydoc = new TiXmlDocument();
	m_myedit_xmlInput.GetWindowText(inputXml);
	mydoc->Parse(inputXml.GetBuffer());
	if (mydoc->Error())
	{
		CString errorLog;
		errorLog.Format("could not load the xml.Error: %s", mydoc->ErrorDesc());
		m_edit_log.SetWindowText(errorLog);
		return;
	}
	inputXml.ReleaseBuffer();
	((CEdit*)GetDlgItem(IDC_EDIT_INFO))->SetWindowTextA("");
	TiXmlElement *MsgElement = mydoc->RootElement();	//根元素, MSG
	//TiXmlElement *pEle = MsgElement;
	int nIndex = 1;
	CString msgType = "";
	//遍历该结点
	for (TiXmlElement *StuElement = MsgElement->FirstChildElement();//第一个子元素
		StuElement != NULL;
		StuElement = StuElement->NextSiblingElement())//下一个兄弟元素
	{
		for (TiXmlElement *sonElement = StuElement->FirstChildElement();
			sonElement != NULL;
			sonElement = sonElement->NextSiblingElement())
		{
			CString childValue = sonElement->FirstChild()->Value();
			CString sonTemp = sonElement->Value();
			if ("TYPE" == sonTemp)
			{
				msgType = childValue;
			}
			iter = mapTagExplain.find(sonElement->Value());
			if (iter != mapTagExplain.end())
			{
				
				outmsg.Format("%s : %s\r\n", iter->second.c_str(), childValue);
				printLog(outmsg);
			}
			for (TiXmlElement *sonSonElement = sonElement->FirstChildElement();
				sonSonElement != NULL;
				sonSonElement = sonSonElement->NextSiblingElement())
			{
				iter = mapTagExplain.find(sonSonElement->Value());
				if (iter != mapTagExplain.end())
				{
					CString childValue1 = sonSonElement->FirstChild()->Value();
					outmsg.Format("└%s : %s\r\n", iter->second.c_str(), childValue1);
					printLog(outmsg);
				}
			}

		}
	}
	map<string, string>mapTemp;
	if ("RERQ" == msgType)
	{
		mapTemp = mapRerq_arst;
		outmsg.Format("\r\n消息类别[%s]中[单分区请求回馈]代码注释\r\n", msgType);
	}
	else if ("RESC" == msgType)
	{
		mapTemp = mapResc_arst;
		outmsg.Format("\r\n消息类别[%s]中[分区播放回馈]代码注释\r\n", msgType);
	}
	else if ("ITAR" == msgType)
	{
		mapTemp = mapItar_resn;
		outmsg.Format("\r\n消息类别[%s]中[打断原因]代码注释\r\n", msgType);
	}
	else if ("IVAB" == msgType)
	{
		mapTemp = mapIvab_abtp;
		outmsg.Format("\r\n消息类别[%s]中[自动广播种类代码]代码注释\r\n", msgType);
	}
	else if ("REIV" == msgType)
	{
		mapTemp = mapReiv_abst;
		outmsg.Format("\r\n消息类别[%s]中[自动广播调用回馈]代码注释\r\n", msgType);
	}
	else if ("REST" == msgType)
	{
		mapTemp = mapRest_stst;
		outmsg.Format("\r\n消息类别[%s]中[存储转发回复状态]代码注释\r\n", msgType);
	}
	else if ("EVAC" == msgType)
	{
		mapTemp = mapEvac_evst;
		outmsg.Format("\r\n消息类别[%s]中[火警状态]代码注释\r\n", msgType);
	}
	else if ("REBG" == msgType)
	{
		mapTemp = mapRebg_ivst;
		outmsg.Format("\r\n消息类别[%s]中[背景音调用回馈状态]代码注释\r\n", msgType);
	}
	else
	{
		return;
	}

	printLog(outmsg);
	for (iter = mapTemp.begin(); iter != mapTemp.end(); iter++)
	{

		outmsg.Format("%s  -  %s\r\n", iter->first.c_str(), iter->second.c_str());
		printLog(outmsg);
	}
	iterMsgType = mapFeedbackMessage.find(msgType.GetBuffer());
	msgType.ReleaseBuffer();
	if (iterMsgType != mapFeedbackMessage.end())
	{

		iter = iterMsgType->second.find(iterMsgType->first);
		if (iter != iterMsgType->second.end())
		{
			for (iter = iterMsgType->second.begin(); iter != iterMsgType->second.end(); iter++)
			{
				outmsg.Format("消息类别[%s]注释\r\n", msgType);
				printLog(outmsg);
				outmsg.Format("%s  -  %s\r\n", iter->first.c_str(), iter->second.c_str());
				printLog(outmsg);
			}
		}
	}
}




void CCallStationXmlAnalysisDlg::OnBnClickedButtonBrowse()
{
	// TODO:  在此添加控件通知处理程序代码
	CString inputPath;
	isFile = false;
	m_edit_browse.SetWindowTextA(get_path().c_str());
	m_edit_browse.GetWindowTextA(inputPath);
	if (inputPath.Trim() != "")
	{
		isFile = true;
		ifstream myfile(inputPath);
		string temp;
		((CEdit*)GetDlgItem(IDC_EDIT_INPXML))->SetWindowTextA("");
		while (getline(myfile, temp))
		{
			CString strTemp;
			strTemp.Format("%s\r\n", temp.c_str());
			((CEdit*)GetDlgItem(IDC_EDIT_INPXML))->SetSel(GetDlgItem(IDC_EDIT_INPXML)->GetWindowTextLength(), GetDlgItem(IDC_EDIT_INPXML)->GetWindowTextLength());
			((CEdit*)GetDlgItem(IDC_EDIT_INPXML))->ReplaceSel(transcoding2utf8(strTemp.GetBuffer()).c_str());
		}
		myfile.close();
	}

}



void CCallStationXmlAnalysisDlg::OnEnChangeEditInpxml()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码.
	OnBnClickedButtonParser();
}


void CCallStationXmlAnalysisDlg::OnBnClickedButtonTcpconnect()
{
	CString strType, strIp, strPort;
	m_combo_procotol.GetWindowText(strType);
	m_combo_ip.GetWindowText(strIp);
	m_combo_port.GetWindowText(strPort);
	CString connectStaus;
	m_button_tcpConnect.GetWindowTextA(connectStaus);
	bool isConnect = false;
	if ("连接" == connectStaus)
	{
		isConnect = false;
	}
	else
	{
		isConnect = true;
	}

	if ("TCP Client" == strType)
	{
		if (isConnect)//已连接，要断开
		{
			net.tcpClientClose();
			m_button_tcpConnect.SetWindowTextA("连接");
		}
		else//已断开，要连接
		{
			int ret = net.tcpCilentConect(strIp, atoi(strPort));
			m_button_tcpConnect.SetWindowTextA("断开");
			if (ret == 1)//连接失败
			{
				for (size_t i = 0; i < net.getExceptionLength(); i++)
				{
					m_edit_log.SetWindowTextA(net.getExceptionStr().at(i).c_str());
				}
				m_button_tcpConnect.SetWindowTextA("连接");
			}

		}

	} 
	else if ("TCP Server" == strType)
	{
		if (isConnect)//已连接，要断开
		{
			m_button_tcpConnect.SetWindowTextA("连接");
			net.tcpServerClose();
			KillTimer(1);
		}
		else//已断开，要连接
		{
			m_button_tcpConnect.SetWindowTextA("断开");
			net.tcpServerOpen(atoi(strPort));
			g_CINI.addIpAndPortToIni("localhost", strPort.GetBuffer());
			SetTimer(1, 500, NULL);
		}
	}


}

void CCallStationXmlAnalysisDlg::printLog(CString logMsg)
{
	((CEdit*)GetDlgItem(IDC_EDIT_INFO))->SetSel(GetDlgItem(IDC_EDIT_INFO)->GetWindowTextLength(), GetDlgItem(IDC_EDIT_INFO)->GetWindowTextLength());
	((CEdit*)GetDlgItem(IDC_EDIT_INFO))->ReplaceSel(logMsg);
}


void CCallStationXmlAnalysisDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	vecSerRecvInfo.clear();

	vecSerRecvInfo = net.getServerRecvStr();
	if (net.getServerRecvLength() > 0)
	{
		 m_listControl_history.DeleteAllItems();
		for (size_t i = 0; i < net.getServerRecvLength(); i++){
			CString strMsgId = "";
			strMsgId.Format("%02d", i+1);
			m_listControl_history.InsertItem(i, strMsgId);
			m_listControl_history.SetItemText(i, 1, vecSerRecvInfo.at(i).c_str());
		}
	}
	UpdateWindow();
}


void CCallStationXmlAnalysisDlg::OnNMDblclkListHistory(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	POSITION ps;
	int nIndex;

	ps = m_listControl_history.GetFirstSelectedItemPosition();
	nIndex = m_listControl_history.GetNextSelectedItem(ps);

	m_listControl_history.SetItemState(nIndex, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	CString str = "";

	str = str + m_listControl_history.GetItemText(nIndex, 1);
	m_myedit_xmlInput.SetWindowText(_T(""));
	m_myedit_xmlInput.SetWindowText(str);
	OnBnClickedButtonParser();
}




void CCallStationXmlAnalysisDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (net.getServerRecvLength() > lastNum || (net.getServerRecvLength() == 0))
	{
		OnBnClickedButton1();
		lastNum = net.getServerRecvLength();
	}

	CDialogEx::OnTimer(nIDEvent);
}
