// ConnectedNetworkDlg.h : header file
//

#pragma once
#include "afxwin.h"

#include "..\\..\\..\\..\\WCL7\\CPP\\Source\\WiFi\\wclWiFi.h"
#include "..\\..\\..\\..\\WCL7\\CPP\\Source\\Common\\wclErrors.h"
#include "..\\..\\..\\..\\WCL7\\CPP\\Source\\Common\\wclHelpers.h"

using namespace wclCommon;
using namespace wclWiFi;


// CConnectedNetworkDlg dialog
class CConnectedNetworkDlg : public CDialog
{
// Construction
public:
	CConnectedNetworkDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CONNECTEDNETWORK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CwclWiFiClient Client;
	CListBox lbInfo;

	void Trace(const tstring& Str);
	void ShowError(const tstring& Str, const int Error);

public:
	afx_msg void OnBnClickedButtonGetInfo();
};
