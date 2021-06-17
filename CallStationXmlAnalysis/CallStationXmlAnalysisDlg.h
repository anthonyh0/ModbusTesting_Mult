
// CallStationXmlAnalysisDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "tinyxml.h"
#include <map>
#include <string>
#include "MyEdit.h"
#include "PublicFunDef.h"
#include "NetWorkManage.h"
#include "NetworkPacket.h"
#include "afxcmn.h"
using namespace std;
// CCallStationXmlAnalysisDlg 对话框
class CCallStationXmlAnalysisDlg : public CDialogEx
{
// 构造
public:
	CCallStationXmlAnalysisDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CALLSTATIONXMLANALYSIS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	NetWorkManage net;
	NetworkPacket netPacket;
	bool isFile;
	int selectIndex;
	int idIndex;
	int lastNum;
	VecString vecSerRecvInfo;
	map<string, map<string, string>>::iterator iterMsgType;
	map<string, string>::iterator iter;
	CEdit m_edit_browse;
	CEdit m_edit_log;
	CMyEdit m_myedit_xmlInput;
	CToolTipCtrl   m_ToolTip;
	CButton m_button_tcpConnect;
	CComboBox m_combo_procotol;
	CComboBox m_combo_ip;
	CComboBox m_combo_port;
	CListCtrl m_listControl_history;
	afx_msg void OnBnClickedButtonParser();
	afx_msg void OnBnClickedButtonBrowse();
	afx_msg void OnEnChangeEditInpxml();
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonTcpconnect();
	afx_msg void printLog(CString logMsg);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnNMDblclkListHistory(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CComboBox m_combo_adapter;
};
