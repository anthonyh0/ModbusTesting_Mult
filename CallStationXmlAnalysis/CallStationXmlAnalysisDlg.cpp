
// CallStationXmlAnalysisDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CallStationXmlAnalysis.h"
#include "CallStationXmlAnalysisDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <fstream> 
#include "INI.h"
#include "SysLogs.h"
#include "Poco/LocalDateTime.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using Poco::LocalDateTime;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CCallStationXmlAnalysisDlg �Ի���



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
	DDX_Control(pDX, IDC_COMBO_ADAPTER, m_combo_adapter);
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


// CCallStationXmlAnalysisDlg ��Ϣ�������

BOOL CCallStationXmlAnalysisDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	initializeLog("xmlAnalysis.log");
	ITC_WriteLog(LOG_LEVEL_NORMAL, "------------------------------------------------------------");

	m_combo_procotol.EnableWindow(true);
	m_combo_ip.EnableWindow(true);
	m_combo_port.EnableWindow(true);

	m_edit_browse.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_BROWSE)->ShowWindow(SW_HIDE);

	idIndex = 0;
	m_ToolTip.Create(this);
	m_ToolTip.AddTool(GetDlgItem(IDC_EDIT_INPXML), _T("[���϶�XML�ļ�����]"));

	m_combo_procotol.ResetContent();
	m_combo_procotol.AddString("Packet");
	m_combo_procotol.AddString("TCP Server");
	m_combo_procotol.AddString("TCP Client");
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
	//m_listControl_stationList.SetTextColor(RGB(255, 0, 0));                  //��ʾ�������ɫ
	DWORD dwStyle = m_listControl_history.GetStyle();
	dwStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_SHOWSELALWAYS;
	m_listControl_history.SetExtendedStyle(dwStyle);


	m_listControl_history.InsertColumn(0, "ID", LVCFMT_CENTER, 60, 0);
	m_listControl_history.InsertColumn(1, "��Ϣ", LVCFMT_LEFT, 500, 1);
	lastNum = 0;

	selectIndex = 0;



	std::vector<std::string> vecDevsList;
	char err[1024 * 6] = { 0 };
	if (-1 == netPacket.findAllDevs(vecDevsList, err))
	{
		CString errStr;
		errStr.Format("%s", err);
		AfxMessageBox(errStr);
		((CEdit*)GetDlgItem(IDC_STATIC_STATUS))->SetWindowTextA(errStr);
	}
	else 
	{
		((CEdit*)GetDlgItem(IDC_STATIC_STATUS))->SetWindowTextA("�豸������������ȡ�ɹ�������");
		for (size_t i = 0; i < vecDevsList.size(); i++)
		{
			m_combo_adapter.AddString(vecDevsList.at(i).c_str());
		}
	}
	m_combo_adapter.SetCurSel(0);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCallStationXmlAnalysisDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCallStationXmlAnalysisDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//��һ��LPCWSTRת��Ϊstring
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

//�½�һ���Ի����ڣ�ѡ���ļ�
string get_path() {
	OPENFILENAME ofn;
	char szFile[300];

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = (LPSTR)szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nFilterIndex = 1;
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
	// TODO: �ڴ����ר�ô����/����û���
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
<MSG>		����ǩ
<META>		��Ϣͷ��ǩ
<SNDR>		��Ϣ������
<DDTM>		����ʱ��
<TYPE>		��Ϣ���
<RECV>		��Ϣ������M
<TIME>		Уʱʱ��M
<RQAR>		�������ͷ��ǩM
<RQID>		���������ϢID
<BCAR>		�������ͷ��ǩ M
<AREA>		�������� M
<CHAN>		���ͨ����ʶ
<MACR>		���������ID
<REST>		�ܷ���������� #��������������������
<REAR>		��������ظ�ͷ��ǩ M
<ARST>		�������������M
<SCAR>		���ŷ���ͷ��ǩM
<RESC>		��������ͷ��ǩM
<CLAR> 		�����ر�ͷ��ǩM
<RECL>		�����رշ�����Ϣͷ��ǩ M
<ITAR>		���������Ϣͷ��ǩ M
<RESN>		���ԭ��M
<RQFL>		������Ϣ������Ϣͷ��ǩ
<RQTP>		��������
<BDGT>		�ǻ��ڱ�� O
<FLNO>		����� 0
<STAT>		������Ϣ������Ϣͷ��ǩ
<FLHT>		����������Ϣͷ��ǩM
<FLID>		����ID
<FLNO>		������Ŷ�����
<SFLG>		���������ǩ
<SFNO>		������ţ������<SFLT>��M
<FLRT>		����
<FLBG>		�ǻ���
<FLST>		����״̬ ���<FLHT>��ΪM
<IVAB>		�Զ��㲥������Ϣͷ��ǩM
<ABTI>		�Զ��㲥���Ŵ��� M
<LASL>		�Զ��㲥���ִ��� M
<ABTP>		�Զ��㲥������� M
<TTST>		TTS�㲥�ı� M
<ABST>		�Զ��㲥���û���
<STRS>		����Ԥ����Ϣͷ��ǩ
<STST>		�洢ת���ظ�״̬M
<IBGM>		����Ԥ����Ϣͷ��ǩ
<CHNL>		������Դͨ��
<IVST>		���������û���״̬
<EVNT>		�𾯸�֪��Ϣͷ��ǩ
<ORIN>		�𾯷�����
<EVST>		��״̬
<CMED>		���к������Ϣͷ��ǩ

*/

map<string, string> mapTagExplain = {
	{ "MSG", "����ǩ" }, { "META", "��Ϣͷ��ǩ" }, { "SNDR", "��Ϣ������" }, { "DDTM", "����ʱ��" }, 
	{ "TYPE", "��Ϣ���" }, { "RECV", "��Ϣ������" }, { "TIME", "Уʱʱ��" },{ "RQAR", "�������ͷ��ǩ" }, 
	{ "RQID", "���������ϢID" }, { "BCAR", "�������ͷ��ǩ" }, { "AREA", "��������" }, { "CHAN", "���ͨ����ʶ" }, 
	{ "MACR", "���������ID" }, { "REST", "�ܷ����������" },{ "REAR", "��������ظ�ͷ��ǩ" }, { "ARST", "�������������" }, 
	{ "SCAR", "���ŷ���ͷ��ǩ" }, { "RESC", "��������ͷ��ǩ" }, { "CLAR", "�����ر�ͷ��ǩ" }, { "RECL", "�����رշ�����Ϣͷ��ǩ" }, 
	{ "ITAR", "���������Ϣͷ��ǩ" },{ "RESN", "���ԭ��" }, { "RQFL", "������Ϣ������Ϣͷ��ǩ" }, { "RQTP", "��������" }, { "BDGT", "�ǻ��ڱ��" }, 
	{ "FLNO", "�����" }, { "STAT", "������Ϣ������Ϣͷ��ǩ" }, { "FLHT", "����������Ϣͷ��ǩ" }, { "FLID", "����ID" },{ "FLNO", "������Ŷ�����" }, 
	{ "SFLG", "���������ǩ" }, { "SFNO", "�������" }, { "FLRT", "����" }, { "FLBG", "�ǻ���" }, { "FLST", "����״̬" }, { "IVAB", "�Զ��㲥������Ϣͷ��ǩ" }, 
	{ "ABTI", "�Զ��㲥���Ŵ���" }, { "LASL", "�Զ��㲥���ִ���" }, { "ABTP", "�Զ��㲥�������" },{ "TTST", "TTS�㲥�ı�" }, { "ABST", "�Զ��㲥���û���" }, 
	{ "STRS", "����Ԥ����Ϣͷ��ǩ" }, { "STST", "�洢ת���ظ�״̬" }, { "IBGM", "����Ԥ����Ϣͷ��ǩ" }, { "CHNL", "������Դͨ��" }, { "IVST", "���������û���״̬" }, 
	{ "EVNT", "�𾯸�֪��Ϣͷ��ǩ" }, { "ORIN", "�𾯷�����" }, { "EVST", "��״̬" }, { "EVAC", "�𾯸�֪��Ϣ" },
	{ "CMED", "���к������Ϣͷ��ǩ" }, { "REIV", "�Զ��㲥���û�����Ϣ" }, {"REBG","���ñ�����������Ϣ"}
};
map<string, string> mapRerq_arst = {
	{ "100", "��������ɹ������Խ��в���" }, { "200", "����ʧ�ܣ������쳣" }, 
	{ "300", "��������������У����Բ���" }, { "400", "��������ʧ�ܣ�������æ" }, { "500", "��������" },
};
map<string, string> mapResc_arst = {
	{ "200", "�������ųɹ�" }, { "201", "��������ʧ�ܣ�ʧȥ����" }, { "202", "��������ʧ�ܣ���������" },
	{ "203", "��������ʧ�ܣ�������æ" }, { "204", "��������ʧ�ܣ����в�����" }, { "205", "��������ʧ�ܣ���������" },
};
map<string, string> mapItar_resn = {
	{ "500", "�����ȼ��㲥��ϴ˷���" }, { "501", "��Ƶͨ���Ͽ�" }, { "502", "��������ʧȥ��Ӧ" },
	{ "599", "����ԭ��" }
};
map<string, string> mapIvab_abtp = {
	{ "1000", "�����ǻ�" }, { "1001", "�ߴٵǻ�" }, { "1002", "��վ�ǻ�" },
	{ "1003", "�ߴٹ�վ�ǻ�" }, { "1004", "�ǻ��ڱ��" }, { "1005", "��������" }, { "1006", "����ȡ��" }
};
map<string, string> mapReiv_abst = {
	{ "2000", "���óɹ�" }, { "2001", "�Ƿ�����ID" }, { "2002", "�Ƿ��Զ��㲥�������" }, { "2003", "�Ƿ���������" },
	{ "2004", "�������Զ��㲥�����뺽��״̬����" }, { "2005", "�Զ��㲥�������" }, { "2006", "�Զ��㲥���ɴ�" }, { "2007", "�Ƿ��Զ��㲥���Ŵ���" }, 
	{ "2008", "�Ƿ��Զ��㲥���ִ���" }
};
map<string, string> mapRebg_ivst = {
	{ "100", "���óɹ�" }, { "200", "����ʧ�ܣ������쳣" }, { "500", "��������" }
};
map<string, string> mapRest_stst = {
	{ "OK", "���Խ��к��д洢ת��" }, { "ABORT", "�������д洢ת��" }
};
map<string, string> mapEvac_evst = {
	{ "ON", "�𾯷���" }, { "OFF", "�𾯽��" }
};
map<string, map<string, string>> mapFeedbackMessage = {
	{ "RERQ", mapRerq_arst }, { "RESC", mapResc_arst }, { "ITAR", mapItar_resn }, { "TVAB", mapIvab_abtp }, { "REIV", mapReiv_abst },
	{"REBG", mapRebg_ivst}, { "REST", mapRest_stst }, { "EVAC", mapEvac_evst }
};
map<string, string> mapMsgType = {
	{ "KPAL", "������Ϣ" }, { "REKP", "�������Ϣ" },{ "TSYN", "ʱ��ͬ����Ϣ" }, { "RQAR", "����������Ϣ" },{ "RERQ", "�������������Ϣ" }, { "SCAR", "����������Ϣ" },
	{ "RESC", "�������Ż�����Ϣ" }, { "CLAR", "�����ر���Ϣ" },{ "RECL", "�����رջ�����Ϣ" }, { "ITAR", "���������Ϣ" },{ "RQFL", "������Ϣ������Ϣ" }, { "REFL", "������Ϣ���������Ϣ" },
	{ "IVAB", "�Զ��㲥������Ϣ" }, { "REIV", "�Զ��㲥���û�����Ϣ" },{ "STRS", "���д洢ת��ѯ����Ϣ" }, { "REST", "���д洢ת���ظ���Ϣ" },{ "IBGM", "���ñ�������Ϣ" }, { "REBG", "���ñ�����������Ϣ" },
	{ "EVAC", "�𾯸�֪��Ϣ" }, { "EDCA", "���к������Ϣ" }
};
//������̫���ˣ���û��������룬�Ƚ��ҡ���2021��5��29��14:09:21��
void CCallStationXmlAnalysisDlg::OnBnClickedButtonParser()
{
	CString outmsg,inputXml,inputPath;
	CString strType;
	m_combo_procotol.GetWindowText(strType);
	m_edit_browse.GetWindowText(inputPath);
	TiXmlDocument *mydoc = new TiXmlDocument();
	inputXml = "";
	VecString vecRecvMsg;
	if (net.getClientRecvLength() > 0 && "TCP Client" == strType)
	{
		vecRecvMsg = net.getClientRecvStr();
		inputXml = vecRecvMsg[selectIndex].c_str();
		ITC_WriteLog(LOG_LEVEL_INFO,"TCP Client inputxml - %s", inputXml);
		if (inputXml == "")
		{
			return;
		}
	}
	else if(netPacket.getPacketDataLen() > 0 && "Packet" == strType)
	{
		vecRecvMsg = netPacket.getPacketData();
		inputXml = vecRecvMsg[selectIndex].c_str();
		ITC_WriteLog(LOG_LEVEL_INFO, "ץ��inputxml - %s", inputXml);
		if (inputXml == "")
		{
			return;
		}
	}
	else
	{
		m_myedit_xmlInput.GetWindowText(inputXml);
	}

	inputXml = transcoding2utf8(inputXml.GetBuffer()).c_str();
	inputXml.ReleaseBuffer();

	mydoc->Parse(inputXml.GetBuffer());
	inputXml.ReleaseBuffer();
	if (mydoc->Error())
	{
		CString errorLog;
		errorLog.Format("could not load the xml.Error: %s", mydoc->ErrorDesc());
		m_edit_log.SetWindowText(errorLog);
		return;
	}
	((CEdit*)GetDlgItem(IDC_EDIT_INFO))->SetWindowTextA("");
	TiXmlElement *MsgElement = mydoc->RootElement();	//��Ԫ��, MSG
	//TiXmlElement *pEle = MsgElement;
	int nIndex = 1;
	CString msgType = "";

	try
	{

		//�����ý��
		for (TiXmlElement *StuElement = MsgElement->FirstChildElement();//��һ����Ԫ��
			StuElement != NULL;
			StuElement = StuElement->NextSiblingElement())//��һ���ֵ�Ԫ��
		{
			for (TiXmlElement *sonElement = StuElement->FirstChildElement();
				sonElement != NULL;
				sonElement = sonElement->NextSiblingElement())
			{
				CString sonTemp = sonElement->Value();
				CString childValue;
				TiXmlNode* tiXmlNode = sonElement->FirstChild();
				if (tiXmlNode == NULL)
				{
					if (sonTemp == "RECV")
					{
						outmsg.Format("��Ϣ������ : -\r\n");
						printLog(outmsg);

					}
					continue;
				}
				else
				{
					childValue = sonElement->FirstChild()->Value();
				}
				
				if ("TYPE" == sonTemp)
				{
					msgType = childValue;
					iter = mapMsgType.find(childValue.GetBuffer());
					if (iter != mapMsgType.end())
					{
						outmsg.Format("��Ϣ��� : %s(%s)\r\n", childValue, iter->second.c_str());
						printLog(outmsg);
					}
					continue;
				}
				iter = mapTagExplain.find(sonElement->Value());
				if (iter != mapTagExplain.end())
				{
					outmsg.Format("%s : %s\r\n", iter->second.c_str(), childValue);
					printLog(outmsg);
				}
				//�ڶ���
				for (TiXmlElement *sonSonElement = sonElement->FirstChildElement();
					sonSonElement != NULL;
					sonSonElement = sonSonElement->NextSiblingElement())
				{
					iter = mapTagExplain.find(sonSonElement->Value());
					if (iter != mapTagExplain.end())
					{
						CString childValue1 = sonSonElement->FirstChild()->Value();
						outmsg.Format("��%s : %s\r\n", iter->second.c_str(), childValue1);
						printLog(outmsg);
					}
				}

			}
		}
	}
	catch (const std::exception&)
	{

	}
	map<string, string>mapTemp;
	if ("RERQ" == msgType)
	{
		mapTemp = mapRerq_arst;
		outmsg.Format("\r\n��Ϣ���[%s]��[�������������]����ע��\r\n", msgType);
	}
	else if ("RESC" == msgType)
	{
		mapTemp = mapResc_arst;
		outmsg.Format("\r\n��Ϣ���[%s]��[�������Ż���]����ע��\r\n", msgType);
	}
	else if ("ITAR" == msgType)
	{
		mapTemp = mapItar_resn;
		outmsg.Format("\r\n��Ϣ���[%s]��[���ԭ��]����ע��\r\n", msgType);
	}
	else if ("IVAB" == msgType)
	{
		mapTemp = mapIvab_abtp;
		outmsg.Format("\r\n��Ϣ���[%s]��[�Զ��㲥�������]����ע��\r\n", msgType);
	}
	else if ("REIV" == msgType)
	{
		mapTemp = mapReiv_abst;
		outmsg.Format("\r\n��Ϣ���[%s]��[�Զ��㲥���û���]����ע��\r\n", msgType);
	}
	else if ("REST" == msgType)
	{
		mapTemp = mapRest_stst;
		outmsg.Format("\r\n��Ϣ���[%s]��[�洢ת���ظ�״̬]����ע��\r\n", msgType);
	}
	else if ("EVAC" == msgType)
	{
		mapTemp = mapEvac_evst;
		outmsg.Format("\r\n��Ϣ���[%s]��[��״̬]����ע��\r\n", msgType);
	}
	else if ("REBG" == msgType)
	{
		mapTemp = mapRebg_ivst;
		outmsg.Format("\r\n��Ϣ���[%s]��[���������û���״̬]����ע��\r\n", msgType);
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
				outmsg.Format("��Ϣ���[%s]ע��\r\n", msgType);
				printLog(outmsg);
				outmsg.Format("%s  -  %s\r\n", iter->first.c_str(), iter->second.c_str());
				printLog(outmsg);
			}
		}
	}
}




void CCallStationXmlAnalysisDlg::OnBnClickedButtonBrowse()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������.
	OnBnClickedButtonParser();
}


void CCallStationXmlAnalysisDlg::OnBnClickedButtonTcpconnect()
{
	CString strType, strIp, strPort, strAdapter;
	int adapterIndex = 0;
	m_combo_procotol.GetWindowText(strType);
	m_combo_ip.GetWindowText(strIp);
	m_combo_port.GetWindowText(strPort);
	m_combo_adapter.GetWindowTextA(strAdapter);
	adapterIndex = m_combo_adapter.GetCurSel();
	ITC_WriteLog(LOG_LEVEL_INFO, "adapter Index:%d", adapterIndex);
	CString connectStaus;
	m_button_tcpConnect.GetWindowTextA(connectStaus);
	bool isConnect = false;
	if ("����" == connectStaus)
	{
		isConnect = false;
	}
	else
	{
		isConnect = true;
	}

	if ("TCP Client" == strType)
	{
		ITC_WriteLog(LOG_LEVEL_INFO, "TCP Client");
		if (isConnect)//�����ӣ�Ҫ�Ͽ�
		{
			net.tcpClientClose();
			m_button_tcpConnect.SetWindowTextA("����");
			KillTimer(1);
			((CEdit*)GetDlgItem(IDC_STATIC_STATUS))->SetWindowTextA("");
			m_combo_procotol.EnableWindow(true);
			m_combo_ip.EnableWindow(true);
			m_combo_port.EnableWindow(true);
		}
		else//�ѶϿ���Ҫ����
		{

			int ret = net.tcpCilentConect(strIp, atoi(strPort));
			m_button_tcpConnect.SetWindowTextA("�Ͽ�");
			
			if (ret == 1)//����ʧ��
			{
				CString errorStr;
				for (size_t i = 0; i < net.getExceptionLength(); i++)
				{
					//m_edit_log.SetWindowTextA(net.getExceptionStr().at(i).c_str());
					errorStr = net.getExceptionStr().at(i).c_str();
				}
				m_button_tcpConnect.SetWindowTextA("����");
				((CEdit*)GetDlgItem(IDC_STATIC_STATUS))->SetWindowTextA("����ʧ��\r\n" + errorStr);
				m_combo_procotol.EnableWindow(true);
				m_combo_ip.EnableWindow(true);
				m_combo_port.EnableWindow(true);
			}
			else
			{
				SetTimer(1, 500, NULL);
				g_CINI.addIpAndPortToIni(strIp.GetBuffer(), strPort.GetBuffer());
				((CEdit*)GetDlgItem(IDC_STATIC_STATUS))->SetWindowTextA("���ӳɹ�");
				m_combo_procotol.EnableWindow(false);
				m_combo_ip.EnableWindow(false);
				m_combo_port.EnableWindow(false);
			}

		}

	} 
	else if ("TCP Server" == strType)
	{
		ITC_WriteLog(LOG_LEVEL_INFO, "TCP Server");
		if (isConnect)//�����ӣ�Ҫ�Ͽ�
		{
			m_button_tcpConnect.SetWindowTextA("����");
			net.tcpServerClose();
			m_combo_procotol.EnableWindow(true);
			m_combo_ip.EnableWindow(true);
			m_combo_port.EnableWindow(true);
			KillTimer(1);
			((CEdit*)GetDlgItem(IDC_STATIC_STATUS))->SetWindowTextA("");
		}
		else//�ѶϿ���Ҫ����
		{
			m_button_tcpConnect.SetWindowTextA("�Ͽ�");
			net.tcpServerOpen(atoi(strPort));
			g_CINI.addIpAndPortToIni("localhost", strPort.GetBuffer());
			((CEdit*)GetDlgItem(IDC_STATIC_STATUS))->SetWindowTextA("������");
			m_combo_procotol.EnableWindow(false);
			m_combo_ip.EnableWindow(false);
			m_combo_port.EnableWindow(false);
			SetTimer(1, 500, NULL);
		}
	}
	else if ("Packet" == strType)
	{

		if (isConnect)//�����ӣ�Ҫ�Ͽ�
		{
			netPacket.packetStop();
			m_button_tcpConnect.SetWindowTextA("����");
			m_combo_procotol.EnableWindow(true);
			m_combo_ip.EnableWindow(true);
			m_combo_port.EnableWindow(true);
			m_combo_adapter.EnableWindow(true);
			KillTimer(1);
			KillTimer(2);
			((CEdit*)GetDlgItem(IDC_STATIC_STATUS))->SetWindowTextA("");
		}
		else//�ѶϿ���Ҫ����
		{
			
			ITC_WriteLog(LOG_LEVEL_INFO, "ץ��");
			//Sleep(5);
			CString strFilter;
			char errStr[1024 * 6] = {0};
			if (strIp == "" && strPort == "")
			{
				strFilter = "";
			}
			else
			{
				strFilter.Format("src host %s and port %s", strIp, strPort);
			}


			if (-1 == netPacket.openPacket(adapterIndex, strFilter.GetBuffer(), errStr))
			{
				((CEdit*)GetDlgItem(IDC_STATIC_STATUS))->SetWindowTextA(errStr);
				ITC_WriteLog(LOG_LEVEL_INFO, "%s",errStr);
				m_button_tcpConnect.SetWindowTextA("����");
				m_combo_procotol.EnableWindow(true);
				m_combo_ip.EnableWindow(true);
				m_combo_port.EnableWindow(true);
				m_combo_adapter.EnableWindow(true);
			}
			else
			{
				  //netPacket.packetStart();
				ITC_WriteLog(LOG_LEVEL_INFO, "����/������ʱ��1,2");
				SetTimer(1, 100, NULL);
				SetTimer(2, 3000, NULL);
				ITC_WriteLog(LOG_LEVEL_INFO, "����/������ʱ��1,2���");
				m_button_tcpConnect.SetWindowTextA("�Ͽ�");
				((CEdit*)GetDlgItem(IDC_STATIC_STATUS))->SetWindowTextA("Adapter: " + strAdapter + "\r\nFilter: " + strFilter + ",  ������ʼ������");
				g_CINI.addIpAndPortToIni(strIp.GetBuffer(), strPort.GetBuffer());
				m_combo_procotol.EnableWindow(false);
				m_combo_ip.EnableWindow(false);
				m_combo_port.EnableWindow(false);
				m_combo_adapter.EnableWindow(false);
			}
			strFilter.ReleaseBuffer();
			//strAdapter.ReleaseBuffer();
			
		}
	}
	strPort.ReleaseBuffer();
	strIp.ReleaseBuffer();
	ITC_WriteLog(LOG_LEVEL_INFO, "����-����");
}

void CCallStationXmlAnalysisDlg::printLog(CString logMsg)
{
	((CEdit*)GetDlgItem(IDC_EDIT_INFO))->SetSel(GetDlgItem(IDC_EDIT_INFO)->GetWindowTextLength(), GetDlgItem(IDC_EDIT_INFO)->GetWindowTextLength());
	((CEdit*)GetDlgItem(IDC_EDIT_INFO))->ReplaceSel(logMsg);
}


void CCallStationXmlAnalysisDlg::OnBnClickedButton1()
{

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	vecSerRecvInfo.clear();
	int nLen = 0;
	CString strType;
	m_combo_procotol.GetWindowText(strType);
	if ("Packet" == strType)
	{
		ITC_WriteLog(LOG_LEVEL_INFO, "����Packet�����б�����");
		vecSerRecvInfo = netPacket.getPacketData();
		nLen = netPacket.getPacketDataLen();
	}
	else 
	{
		switch (net.getCurrentType())
		{
		case TCPSERVER:
			vecSerRecvInfo = net.getServerRecvStr();
			nLen = net.getServerRecvLength();

			break;
		case TCPCLIENT:
			vecSerRecvInfo = net.getClientRecvStr();
			nLen = net.getClientRecvLength();
			break;
		default:

			break;
		}

	}

	if (nLen > 0)
	{
		m_listControl_history.DeleteAllItems();
		for (size_t i = 0; i < nLen; i++) {
			CString strMsgId = "";
			strMsgId.Format("%02d", i + 1);
			m_listControl_history.InsertItem(i, strMsgId);
			m_listControl_history.SetItemText(i, 1, vecSerRecvInfo.at(i).c_str());
			m_listControl_history.SetItemText(i, 1, transcoding2utf8((char*)vecSerRecvInfo.at(i).c_str()).c_str());
		}
	}
	//UpdateWindow();
}


void CCallStationXmlAnalysisDlg::OnNMDblclkListHistory(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	POSITION ps;
	int nIndex;

	ps = m_listControl_history.GetFirstSelectedItemPosition();
	nIndex = m_listControl_history.GetNextSelectedItem(ps);

	m_listControl_history.SetItemState(nIndex, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	CString str = "";

	str = str + m_listControl_history.GetItemText(nIndex, 1);
	m_myedit_xmlInput.SetWindowText(_T(""));
	//CString utf8Code = transcoding2utf8(str.GetBuffer()).c_str();
	//str.ReleaseBuffer();
	m_myedit_xmlInput.SetWindowText(str);
	selectIndex = nIndex;
	ITC_WriteLog(LOG_LEVEL_INFO, "set nIndex: %d", selectIndex);
	OnBnClickedButtonParser();
}




void CCallStationXmlAnalysisDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 1)
	{

		char chTemp[1024] = {0};
		CString strType;
		m_combo_procotol.GetWindowText(strType);
		if ("Packet" == strType)
		{
			netPacket.packetGetData();
		}
		else
		{
			switch (net.getCurrentType())
			{
			case TCPSERVER:
		
				if (net.getServerRecvLength() > lastNum || (net.getServerRecvLength() == 0))
				{
					OnBnClickedButton1();
					lastNum = net.getServerRecvLength();
				}
				break;
			case TCPCLIENT:
				net.clientReceiveMsg(chTemp);
				if (net.getClientRecvLength() > lastNum || (net.getClientRecvLength() == 0))
				{
					OnBnClickedButton1();
					lastNum = net.getClientRecvLength();
				}
				break;
			default:

				break;
			}

		}
	}
	else if (nIDEvent == 2)
	{
		if (netPacket.getPacketDataLen() > lastNum || (netPacket.getPacketDataLen() == 0))
		{
			OnBnClickedButton1();
			lastNum = netPacket.getPacketDataLen();
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


