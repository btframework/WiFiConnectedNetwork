// ConnectedNetworkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ConnectedNetwork.h"
#include "ConnectedNetworkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CConnectedNetworkDlg dialog

tstring IntToHex(const int i)
{
	CString s;
	s.Format(_T("%.8X"), i);
	return tstring(s.GetBuffer());
}

tstring IntToStr(const int i)
{
	CString s;
	s.Format(_T("%d"), i);
	return tstring(s.GetBuffer());
}


CConnectedNetworkDlg::CConnectedNetworkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectedNetworkDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CConnectedNetworkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INFO, lbInfo);
}

BEGIN_MESSAGE_MAP(CConnectedNetworkDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_GET_INFO, &CConnectedNetworkDlg::OnBnClickedButtonGetInfo)
END_MESSAGE_MAP()


// CConnectedNetworkDlg message handlers

BOOL CConnectedNetworkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CConnectedNetworkDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CConnectedNetworkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CConnectedNetworkDlg::Trace(const tstring& Str)
{
	lbInfo.AddString(Str.c_str());
}

void CConnectedNetworkDlg::ShowError(const tstring& Str, const int Error)
{
	tstring Framework;
	tstring Category;
	tstring Constant;
	tstring Description;
	
	if (wclGetErrorInfo(Error, Framework, Category, Constant, Description))
		Trace(Str + _T(": 0x") + IntToHex(Error) + _T(" [") + Constant + _T("]: ") + Description);
	else
		Trace(Str + _T(": 0x") + IntToHex(Error));
}
void CConnectedNetworkDlg::OnBnClickedButtonGetInfo()
{
	int Res = Client.Open();
	if (Res != WCL_E_SUCCESS)
		ShowError(_T("Open failed"), Res);
	else
	{
		wclWiFiInterfaces Ifaces;
		Res = Client.EnumInterfaces(Ifaces);
		if (Res != WCL_E_SUCCESS)
			ShowError(_T("Enum interfaces failed"), Res);
		else
		{
			if (Ifaces.size() == 0)
				Trace(_T("No WiFi interfaces found"));
			else
			{
				for (wclWiFiInterfaces::iterator Data = Ifaces.begin(); Data != Ifaces.end(); Data++)
				{
					CwclWiFiInterface* Iface = new CwclWiFiInterface(Data->Id);
					Res = Iface->Open();
					if (Res != WCL_E_SUCCESS)
						ShowError(_T("Open interface ") + Data->Description + _T(" failed"), Res);
					else
					{
						Trace(_T("Opened ") + Data->Description);

						wclWiFiInterfaceState State;
						Res = Iface->GetState(State);
						if (Res != WCL_E_SUCCESS)
							ShowError(_T("  Get state failed"), Res);
						else
						{
							tstring Str = _T("  Interface state: ");
							switch (State)
							{
								case isNotReady:
									Str += _T("Not ready");
									break;
								case isConnected:
									Str += _T("Connected");
									break;
								case isAdHocFormed:
									Str += _T("AdHoc formed");
									break;
								case isDisconnecting:
									Str += _T("Disconnecting");
									break;
								case isDisconnected:
									Str += _T("Disconnected");
									break;
								case isAssociating:
									Str += _T("Associating");
									break;
								case isDiscovering:
									Str += _T("Discovering");
									break;
								case isAuthenticating:
									Str += _T("Authenticating");
									break;
								default:
									Str += _T("Unknown");
									break;
							}
							Trace(Str);
							
							if (State == isConnected)
							{
								wclWiFiConnectionAttributes Attr;
								Res = Iface->GetConnectionAttributes(Attr);
								if (Res != WCL_E_SUCCESS)
									ShowError(_T("  Get connection attributes failed"), Res);
								else
								{
									Str = _T("  Connecting mode: ");
									switch (Attr.Mode)
									{
										case cmProfile:
											Str += _T("Profile");
											break;
										case cmTemporaryProfile:
											Str += _T("Temporary Profile");
											break;
										case cmDiscoverySecure:
											Str += _T("Discovery Secure");
											break;
										case cmDiscoveryUnsecure:
											Str += _T("Discovery Unsecure");
											break;
										case cmAuto:
											Str += _T("Auto");
											break;
										case cmInvalid:
											Str += _T("Invalid");
											break;
										default:
											Str += _T("Unknown");
											break;
									}
									Trace(Str);
									
									if (Attr.Mode == cmProfile || Attr.Mode == cmTemporaryProfile)
										Trace(_T("  Profile: ") + Attr.ProfileName);
									
									Trace(_T("  SSID: ") + Attr.Association.Ssid);
									Trace(_T("  BSS ID: ") + Attr.Association.BssId);
									Trace(_T("  SQ: ") + IntToStr(Attr.Association.SignalQuality));
									Trace(_T("  RX Rate: ") + IntToStr(Attr.Association.RxRate));
									Trace(_T("  TX Rate: ") + IntToStr(Attr.Association.TxRate));
									
									unsigned long Ch;
									Res = Iface->GetChannel(Ch);
									if (Res != WCL_E_SUCCESS)
										ShowError(_T("  Get channel failed"), Res);
									else
										Trace(_T("  Channel: ") + IntToStr(Ch));
									
									wclWiFiBssArray BssList;
									Res = Client.EnumBss(Data->Id, Attr.Association.Ssid, Attr.Association.BssType,
										Attr.Security.SecurityEnabled, BssList);
									if (Res != WCL_E_SUCCESS)
										ShowError(_T("  Enum BSS failed"), Res);
									else
									{
										if (BssList.size() == 0)
											Trace(_T("  No BSS found"));
										else
										{
											for (wclWiFiBssArray::iterator Bss = BssList.begin(); Bss != BssList.end(); Bss++)
											{
												if (Bss->Mac == Attr.Association.BssId)
												{
													Trace(_T("  BSS RSSI: ") + IntToStr(Bss->Rssi));
													Trace(_T("  BSS LQ: ") + IntToStr(Bss->LinkQuality));
													Trace(_T("  BSS Center FREQ: ") + IntToStr(Bss->ChCenterFrequency));
												}
											}
										}
									}
								}
							}
						}
						Iface->Close();
					}
					delete Iface;
				}
			}
		}
		Client.Close();
	}
}
