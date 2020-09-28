// SDKDEMODlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SDKDEMO.h"
#include "SDKDEMODlg.h"
#include "LoginDlg.h"
#include "LiveDlg.h"
#include "SearchDlg.h"

#include "ToolDlg.h"
#include "ConfigDlg.h"
#include "MsgDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "strdef.h"


// CSDKDEMODlg 对话框




CSDKDEMODlg::CSDKDEMODlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSDKDEMODlg::IDD, pParent)
	, m_userID(-1)
	, m_liveDlg(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_liveDlg = new CLiveDlg;
	m_searchDlg = new CSearchDlg;
	m_toolDlg = new CToolDlg;
	m_configDlg = new CConfigDlg;
	m_msgDlg = new CMsgDlg;
	ZeroMemory(&m_deviceInfo, sizeof(NET_SDK_DEVICEINFO));
}

CSDKDEMODlg::~CSDKDEMODlg()
{
	if (m_liveDlg)
	{
		delete m_liveDlg;
		m_liveDlg = NULL;
	}
	if (m_toolDlg)
	{
		delete m_toolDlg;
		m_toolDlg = NULL;
	}
	if (m_configDlg)
	{
		delete m_configDlg;
		m_configDlg = NULL;
	}
	if (m_searchDlg)
	{
		delete m_searchDlg;
		m_searchDlg = NULL;
	}

	if (m_msgDlg)
	{
		delete m_msgDlg;
		m_msgDlg = NULL;
	}
}


void CSDKDEMODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAIN_TAB, m_tab);
}

BEGIN_MESSAGE_MAP(CSDKDEMODlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CSDKDEMODlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSDKDEMODlg::OnBnClickedCancel)
	ON_WM_DESTROY()
	ON_WM_MOVE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_MAIN_TAB, &CSDKDEMODlg::OnTcnSelchangeMainTab)
	ON_MESSAGE(WM_SHOW_MSG_DLG, &CSDKDEMODlg::OnShowMsgDlg)
	ON_BN_CLICKED(IDC_BUTTON_LogTest, &CSDKDEMODlg::OnBnClickedButtonLogtest)
END_MESSAGE_MAP()


// CSDKDEMODlg 消息处理程序

BOOL CSDKDEMODlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_tab.InsertItem(0, STR_LIVE);
	m_tab.InsertItem(1, STR_SEARCH_BACKUP);
	m_tab.InsertItem(2, STR_CONFIG);
	m_tab.InsertItem(3, STR_TOOL);

	CRect tabRect;
	m_tab.GetClientRect(&tabRect);
	tabRect.left += 2;
	tabRect.top += 20;
	tabRect.bottom -= 3;
	tabRect.right -= 3;

	
	m_liveDlg->Create(IDD_LIVE_DIALOG, &m_tab);
	m_liveDlg->MoveWindow(&tabRect);
	
	m_searchDlg->Create(IDD_SEARCH_DIALOG, &m_tab);
	m_searchDlg->MoveWindow(&tabRect);

	m_toolDlg->Create(IDD_TOOL_DIALOG, &m_tab);
	m_toolDlg->MoveWindow(&tabRect);
	m_toolDlg->setMainDlg(this);

	m_configDlg->Create(IDD_CONFIG_DIALOG, &m_tab);
	m_configDlg->MoveWindow(&tabRect);

	m_msgDlg->Create(IDD_MSG_DIALOG, NULL);
	m_msgDlg->ShowWindow(FALSE);

	if(m_userID == -1)//主动登录方式，未登录
	{
		//登录
		CLoginDlg dlg;	
		dlg.SetIp(m_IP);
		dlg.SetPort(m_port);

		while(true)
		{
			if (IDOK == dlg.DoModal())
			{
				LONG userId = NET_SDK_Login(dlg.GetIP().GetBuffer(), dlg.GetPort(), dlg.GetUserName().GetBuffer(), dlg.GetPassword().GetBuffer(), &m_deviceInfo);

				if (userId >= 0)
				{
					m_userID = userId;
					break;
				}
				else
				{
					LONG error = NET_SDK_GetLastError();
					CString msg = NET_SDK_GetErrorMsg(&error);
					//AfxMessageBox(msg);
					AfxMessageBox(STR_LOGIN_FAILED);
				}
			}
			else
			{
				OnCancel();
				break;
			}
		}
	}
	else	//被动登录方式
	{
		;
	}

	m_liveDlg->SetDeviceInfo(&m_deviceInfo, m_userID);
	m_liveDlg->ShowWindow(SW_SHOW);
	m_searchDlg->SetDeviceInfo(&m_deviceInfo, m_userID);
	m_toolDlg->setDeviceInfo(&m_deviceInfo, m_userID);
	m_configDlg->setDeviceInfo(&m_deviceInfo, m_userID);
	NET_SDK_SetSDKMessageCallBack(0, 0, excertionCallback, NULL);
	NET_SDK_SetDVRMessageCallBack(msgCallback, this);
	m_ipMap[m_userID] = m_IP;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSDKDEMODlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CSDKDEMODlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSDKDEMODlg::OnBnClickedOk()
{
	
}

void CSDKDEMODlg::OnBnClickedCancel()
{
	//OnCancel();
	if (IDYES == AfxMessageBox(STR_CONFIRM_QUIT, MB_YESNO))
	{
		if (m_userID >= 0)
		{
			NET_SDK_Logout(m_userID);
			m_userID = -1;
		}
		OnCancel();
	}
}


void CSDKDEMODlg::OnDestroy()
{
	CDialog::OnDestroy();
}

void CSDKDEMODlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	
	if (!IsWindow(m_tab.GetSafeHwnd()))
	{
		return ;
	}
	if (m_tab.GetCurSel() == 0)
	{
		m_liveDlg->OnMove(x, y);
	}
}

void CSDKDEMODlg::OnTcnSelchangeMainTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	int sel = m_tab.GetCurSel();
	if (m_liveDlg->IsWindowVisible())
	{
		m_liveDlg->ShowWindow(SW_HIDE);
	}
	
	if (m_searchDlg->IsWindowVisible())
	{
		m_searchDlg->ShowWindow(SW_HIDE);
	}

	if (m_toolDlg->IsWindowVisible())
	{
		m_toolDlg->ShowWindow(SW_HIDE);
	}
	
	if (m_configDlg->IsWindowVisible())
	{
		m_configDlg->ShowWindow(SW_HIDE);
	}

	switch (sel)
	{
	case 0:
		{
			m_liveDlg->ShowWindow(SW_SHOW);
			break;
		}
	case 1:
		{
			m_searchDlg->ShowWindow(SW_SHOW);
			break;
		}
	case 2:
		{
			m_configDlg->ShowWindow(SW_SHOW);
			break;
		}
	case 3:
		{
			m_toolDlg->ShowWindow(SW_SHOW);
			break;
		}
	default:break;
	}
	*pResult = 0;
}

void CALLBACK CSDKDEMODlg::excertionCallback( DWORD dwType, LONG lUserID, LONG lHandle, void *pUser )
{
	CString temp;
	temp.Format(_T("type:%d, userID:%d, handle:%d"), dwType, lUserID, lHandle);
	AfxMessageBox(temp);
}

BOOL CALLBACK CSDKDEMODlg::msgCallback( LONG lCommand, LONG lUserID, char *pBuf, DWORD dwBufLen, void *pUser )
{
	CSDKDEMODlg *pThis = (CSDKDEMODlg *)pUser;
	if (lCommand == NET_SDK_ALARM)
	{
		int num = dwBufLen / sizeof(NET_SDK_ALARMINFO);
		NET_SDK_ALARMINFO *ai = reinterpret_cast<NET_SDK_ALARMINFO *>(pBuf);

		for (int i = 0; i < num; i++)
		{
			CString temp;
			CString ttemp;
			CString ttype;
			switch(ai[i].dwAlarmType)
			{
			case NET_SDK_ALARM_TYPE_MOTION:
				ttype = STR_MOTION;
				ttemp.Format(STR_CHANNEL_D, ai[i].dwChannel);
				break;
			case NET_SDK_ALARM_TYPE_SENSOR:
				ttype = STR_SENSOR;
				ttemp.Format(STR_SENSOR_D, ai[i].dwSensorIn);
				break;
			case NET_SDK_ALARM_TYPE_VLOSS:
				ttype = STR_VIDEO_LOSS;
				ttemp.Format(STR_CHANNEL_D, ai[i].dwChannel);
				break;
			case NET_SDK_ALARM_TYPE_SHELTER:
				ttype = STR_SHELTER;
				ttemp.Format(STR_CHANNEL_D, ai[i].dwChannel);
				break;
			case NET_SDK_ALARM_TYPE_DISK_FULL:
				ttype = STR_DISK_FULL;
				ttemp.Format(STR_DISK_NO_D, ai[i].dwDisk);
				break;
			case NET_SDK_ALARM_TYPE_DISK_UNFORMATTED:
				ttype = STR_DISK_UNFORMAT;
				ttemp.Format(STR_DISK_NO_D, ai[i].dwDisk);
				break;
			case NET_SDK_ALARM_TYPE_DISK_WRITE_FAIL:
				ttype = STR_DISK_READ_WRITE_ERROR;
				ttemp.Format(STR_DISK_NO_D, ai[i].dwDisk);
				break;
			}
			temp.Format(STR_MSG_FORMAT, pThis->m_ipMap[lUserID], ttype, ttemp);
			pThis->m_msgDlg->SendMessage(WM_MSG_CALLBACK, 0, (LPARAM)temp.GetBuffer());
			temp.ReleaseBuffer();
		}
	}
	else if (lCommand == NET_SDK_RECORD)
	{
		int num = dwBufLen / sizeof(NET_SDK_RECORD_STATUS);
		NET_SDK_RECORD_STATUS *rs = reinterpret_cast<NET_SDK_RECORD_STATUS *>(pBuf);
		
		for (int i = 0; i < num; i++)
		{
			CString temp;
			CString ttype;
			switch(rs[i].dwRecordType)
			{
			case DD_RECORD_TYPE_MANUAL:
				ttype = STR_MANUAL_RECORD;
				break;
			case DD_RECORD_TYPE_SCHEDULE:
				ttype = STR_SCHEDULE_RECORD;
				break;
			case DD_RECORD_TYPE_MOTION:
				ttype = STR_MOTION_RECORD;
				break;
			case DD_RECORD_TYPE_SENSOR:
				ttype = STR_SENSOR_RECORD;
				break;
			case DD_RECORD_TYPE_BEHAVIOR:
				ttype = STR_BEHIVOR_RECORD;
				break;
			case DD_RECORD_TYPE_NONE:
				ttype = STR_NONE_RECORD;
				break;
			}
			temp.Format(STR_RECORD_MSG_FORMAT, pThis->m_ipMap[lUserID], rs->dwChannel, ttype);
			
			pThis->m_msgDlg->SendMessage(WM_MSG_CALLBACK, 0, (LPARAM)temp.GetBuffer());
			temp.ReleaseBuffer();
		}
	}
	else
	{
		ASSERT(FALSE);
	}
	return 0;
}

LRESULT CSDKDEMODlg::OnShowMsgDlg(WPARAM wparam, LPARAM lparam)
{
	int ifShow = (int)lparam;
	if (ifShow)
	{
		m_msgDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_msgDlg->ShowWindow(SW_HIDE);
	}
	return 0;
}
void CSDKDEMODlg::OnBnClickedButtonLogtest()
{
	//test
	//登录
	CLoginDlg loginDlg;	
	loginDlg.SetIp("192.168.3.117");
	loginDlg.SetPort(6036);

	while(true)
	{
		if (IDOK == loginDlg.DoModal())
		{
			LONG userId = NET_SDK_Login(loginDlg.GetIP().GetBuffer(), loginDlg.GetPort(), loginDlg.GetUserName().GetBuffer(), loginDlg.GetPassword().GetBuffer(), &m_deviceInfo);

			if (userId >= 0)
			{
				m_userID = userId;
				break;
			}
			else
			{
// 				LONG error = NET_SDK_GetLastError();
// 				CString msg = NET_SDK_GetErrorMsg(&error);
// 				//AfxMessageBox(msg);
	
				DWORD dwErrorCode = NET_SDK_GetLastError();
				CString strMsg;
				strMsg.Format(_T("%s\r\n错误代码：%d"), STR_LIVE_FAILED, dwErrorCode);
				AfxMessageBox(/*STR_LIVE_FAILED*/strMsg);
			}
		}
		else
		{
			OnCancel();
			break;
		}
	}

	m_liveDlg->SetDeviceInfo(&m_deviceInfo, m_userID);
}
