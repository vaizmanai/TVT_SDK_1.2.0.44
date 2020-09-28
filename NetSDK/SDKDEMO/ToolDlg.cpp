// ToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SDKDEMO.h"
#include "ToolDlg.h"
#include "GridCellCombo.h"
#include "FormatDlg.h"
#include <WinSock2.h>
#include <Wincrypt.h>
#include "strdef.h"
#include <MMSystem.h>

TCHAR *logTypeStr[9][70] = {
	{
		STR_SYSTEM_BOOT,
		STR_SYSTEM_SHUTDOWN,
		STR_SYSTEM_REBOOT,
		STR_FORMAT_DISK_SUCCESS,
		STR_FORMAT_DISK_FAILED,
		STR_UPDATE_SUCCESS,
		STR_UPDATE_FAILED,
		STR_CLEAR_ALARM,
		STR_OPEN_ALARM,
		STR_OPEN_MANUAL_RECORD,
		STR_STOP_MANUAL_RECORD,
		STR_PTZ_CTRL,
		STR_PTZ_OPERATE,
		STR_PTZ_CTRL_FIN,
		STR_CHANGE_AUDIO,
		STR_CHANGE_VOLUME,
		STR_START_MUTE,
		STR_STOP_MUTE,
		STR_START_POLL,
		STR_STOP_POLL,
		STR_LOGIN,
		STR_LOGOUT,
		STR_CHANGE_SYS_TIME,
		STR_SNAP_SUCCESS,
		STR_SNAP_FAILED
	},

	{
		STR_CHANGE_VIDEO_TYPE,
		STR_CHANGE_VIDEO_RESOLUTION,
		STR_CHANGE_LANUAGE,
		STR_CHANGE_USER_NAME,
		STR_CHANGE_TIMEZONE,
		STR_CHANGE_TIME,
		STR_START_AUTO_ADJUST_TIME,
		STR_STOP_AUTO_ADJUST_TIME,
		STR_CHANGE_NTP_SERVER,
		STR_CHANGE_DST,
		STR_START_PASSWORD,
		STR_STOP_PASSWORD,

		STR_CHANGE_CHANNEL_NAME,
		STR_CHANGE_COLOR,
		STR_SET_MAIN_MONITOR,
		STR_SET_SPOT_MONITOR,
		STR_SET_OSD,

		STR_SET_REC_ENCODE,
		STR_SET_REC_ON,
		STR_SET_REC_AUDIO_ON,
		STR_SET_REREC_ON,
		STR_SET_PRE_ALARM_TIME,
		STR_SET_POSTS_ALARM_TIME,
		STR_SET_REC_EXPIRE_TIME,

		STR_SET_SCHEDULE_PLAN,
		STR_SET_MOTION_PLAN,
		STR_SET_ALARM_REC_PLAN,

		STR_SET_ALARM_IN_ON,
		STR_SET_ALARM_IN_TYPE,
		STR_SET_ALARM_IN_APPROACH,
		STR_SET_ALARM_IN_PLAN,

		STR_SET_MOTION_ON,
		STR_SET_MOTION_SEN,
		STR_SET_MOTION_AREA,
		STR_SET_MOTION_APPROACH,
		STR_SET_MOTION_PLAN,

		STR_SET_VLOSS_APPROACH,

		STR_SET_ALARM_OUT_ON,
		STR_SET_ALARM_OUT_PLAN,

		STR_START_AUDIO_ALARM,
		STR_STOP_AUDIO_ALARM,
		STR_SET_AUDIO_ALARM_PLAN,

		STR_CHANGE_HTTP_PORT,
		STR_CHANGE_NETWORK_PORT,
		STR_SET_IP,
		STR_DHCP_SUCCESS,
		STR_DHCP_FAILED,
		STR_SET_PPPOE,
		STR_SET_DDNS,
		STR_SET_NET_ENCODE,

		STR_CHANGE_SERIAL_PORT,
		STR_CHANGE_PRESET,
		STR_CHANGE_CRUISE,
		STR_CHANGE_TRACK,

		STR_ADD_USER,
		STR_CHANGE_AUTH,
		STR_DEL_USER,
		STR_CHANGE_PASSWORD,

		STR_DEFAULT_CONFIG,
		STR_IMPORT_CONFIG,
		STR_EXPORT_CONFIG,

		STR_SHELTER,
		STR_LOOP_RECORD_START,
		STR_LOOP_RECORD_STOP,
		STR_ADJUST_DISK_ALARM,

		STR_SET_EMAIL_SND,
		STR_SET_EMAIL_RECV,
		STR_SET_SNAP
	},

	{
		STR_PLAY,
		STR_PAUSE,
		STR_RESUME,
		STR_FF,
		STR_REW,
		STR_STOP,
		STR_NEXT,
		STR_PRE
	},

	{
		STR_BACKUP_START,
		STR_BACKUP_FIN,
		STR_BACKUP_CANCEL,
		STR_BACKUP_FAILED
	},

	{
		STR_SEARCH_TIME,
		STR_SEARCH_EVENT,
		STR_SEARCH_FILE,
		STR_SEARCH_PIC,
		STR_DEL_FILE,
		STR_LOCK_FILE,
		STR_UNLOCK_FILE,
		STR_DEL_PIC,
		STR_LOCK_PIC,
		STR_UNLOCK_PIC
	},

	{
		STR_SYS_INFO,
		STR_EVENT_INFO,
		STR_EXPORT_LOG,
		STR_NETWORK_INFO,
		STR_ONLINE_USER,
		STR_EXPORT_LOG,
		STR_EXPORT_EVENT
	},

	{
		STR_SENSOR_ALARM_START,
		STR_SENSOR_ALARM_FIN,
		STR_MOTION_START,
		STR_MOTION_STOP,
		STR_VLOSS_START,
		STR_VLOSS_STOP,
		STR_SHELTER_START,
		STR_SHELTER_STOP
	},

	{
		_T("adfsdfsd"),
	},

	{
		STR_IP_CONFLICT,
		STR_NET_WORK_ERROR,
		STR_DDNS_ERROR,
		STR_DISK_ERROR,
		STR_POWER_OFF,
		STR_UNKNOWN_ERROR	
	}
};

// CToolDlg 对话框

IMPLEMENT_DYNAMIC(CToolDlg, CDialog)

CToolDlg::CToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolDlg::IDD, pParent)
	, m_logStartTime(0)
	, m_logEndTime(0)
	, m_isStartVoiceComm(FALSE)
	, m_isStartVoiceComm_MR(FALSE)
	, m_isOpenMsgCallback(FALSE)
	, m_runTalkTransThread(FALSE)
	, m_talkDataFile(INVALID_HANDLE_VALUE)
	, m_voiceHandle(-1)
	, m_voiceVolume(0)
	, m_sysTime(CTime::GetCurrentTime())
{

}

CToolDlg::~CToolDlg()
{
}

void CToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_logList);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_START_TIME, m_logStartTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_END_TIME, m_logEndTime);
	DDX_Slider(pDX, IDC_SLIDER_VOICECOMM_VOLUME, m_voiceVolume);
	DDX_Control(pDX, IDC_DATETIMEPICKER_START_TIME, m_startTimeCtrl);
	DDX_Control(pDX, IDC_DATETIMEPICKER_END_TIME, m_endTimeCtrl);
	DDX_Control(pDX, IDC_DATETIMEPICKER_SYSTIME, m_systimeCtrl);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_SYSTIME, m_sysTime);
	DDX_GridControl(pDX, IDC_CUSTOM_DISK_LIST, m_diskGrid);
}


BEGIN_MESSAGE_MAP(CToolDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CToolDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CToolDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SEARCHLOG, &CToolDlg::OnBnClickedButtonSearchlog)
	ON_BN_CLICKED(IDC_BUTTON_VOICE_COMM, &CToolDlg::OnBnClickedButtonVoiceComm)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_CHANGESYSTIME, &CToolDlg::OnBnClickedButtonChangesystime)
	ON_BN_CLICKED(IDC_BUTTON_DISK_REFRESH, &CToolDlg::OnBnClickedButtonDiskRefresh)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_DISK_PROPERTY, &CToolDlg::OnBnClickedButtonChangeDiskProperty)
	ON_BN_CLICKED(IDC_BUTTON_DISK_FORMAT, &CToolDlg::OnBnClickedButtonDiskFormat)
	ON_BN_CLICKED(IDC_BUTTON_MSG_CALLBACK, &CToolDlg::OnBnClickedButtonMsgCallback)
	ON_BN_CLICKED(IDC_BUTTON_TALKT_RANS, &CToolDlg::OnBnClickedButtonTalktRans)
END_MESSAGE_MAP()


// CToolDlg 消息处理程序

void CToolDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnOK();
}

void CToolDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnCancel();
}

void CToolDlg::OnBnClickedButtonSearchlog()
{
	UpdateData();
	m_logList.DeleteAllItems();

	DWORD type = 0x0;
	CButton *button;
	for (int i = IDC_CHECK_LOG_SYSOP; i <= IDC_CHECK_LOG_ERROR; i++)
	{
		button = reinterpret_cast<CButton*>(GetDlgItem(i));
		if (button->GetCheck())
		{
			type |= (0x1) << (i - IDC_CHECK_LOG_SYSOP); 
		}
	}

	DD_TIME start = {m_logStartTime.GetSecond(), m_logStartTime.GetMinute(), m_logStartTime.GetHour(), 0,
			 m_logStartTime.GetDay(), m_logStartTime.GetMonth() - 1, m_logStartTime.GetYear() - 1900};

	DD_TIME end = {m_logEndTime.GetSecond(), m_logEndTime.GetMinute(), m_logEndTime.GetHour(), 0,
		m_logEndTime.GetDay(), m_logEndTime.GetMonth() - 1, m_logEndTime.GetYear() - 1900};

	LONG loghandle = NET_SDK_FindDVRLog(m_userID, type, &start, &end);
	LONG result = 0;

	NET_SDK_LOG log;
	
	if (loghandle > 0)
	{
		while (true)
		{
			result = NET_SDK_FindNextLog(loghandle, &log);
			if (result == NET_SDK_FILE_NOFIND)
			{
				AfxMessageBox(STR_FIND_NO_LOG);
				break;
			}
			if (result == NET_SDK_NOMOREFILE)
			{
				break;
			}
			int n = m_logList.GetItemCount();
			m_logList.InsertItem(n, _T(""));
			 
			m_logList.SetItemText(n, 0, logTypeStr[log.dwMajorType - 1][log.dwMinorType - 1]);
			m_logList.SetItemText(n, 1, log.sNetUser);
			CTime time(log.strLogTime.year + 1900, log.strLogTime.month + 1, log.strLogTime.mday,
				log.strLogTime.hour, log.strLogTime.minute, log.strLogTime.second);
			m_logList.SetItemText(n, 2, time.Format(_T("%Y-%m-%d %H:%M:%S")));
			
			IN_ADDR addr;
			addr.S_un.S_addr = log.dwRemoteHostAddr;
			m_logList.SetItemText(n, 3, inet_ntoa(addr));
		}

		NET_SDK_FindLogClose(loghandle);
	}	
	else
	{
		AfxMessageBox(_T("没有找到日志!"));
	}
}

BOOL CToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_diskGrid.SetRowCount(1);
	m_diskGrid.SetFixedRowCount(1);
	m_diskGrid.SetListMode(TRUE);
	m_diskGrid.SetColumnCount(4);

	m_diskGrid.SetItemText(0, 0, STR_TOTAL_SIZE);
	m_diskGrid.SetItemText(0, 1, STR_LEFT_SIZE);
	m_diskGrid.SetItemText(0, 2, STR_STATUS);
	m_diskGrid.SetItemText(0, 3, STR_PROPERTY);

	m_diskGrid.SetColumnWidth(0, 80);
	m_diskGrid.SetColumnWidth(1, 80);
	m_diskGrid.SetColumnWidth(2, 68);
	m_diskGrid.SetColumnWidth(3, 68);

	m_logList.InsertColumn(0, STR_TYPE);
	m_logList.InsertColumn(1, STR_USERNAME);
	m_logList.InsertColumn(2, STR_TIME);
	m_logList.InsertColumn(3, STR_IP);

	m_logList.SetColumnWidth(0, 120);
	m_logList.SetColumnWidth(1, 75);
	m_logList.SetColumnWidth(2, 130);
	m_logList.SetColumnWidth(3, 130);

	m_logList.SetExtendedStyle(m_logList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_startTimeCtrl.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	m_endTimeCtrl.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	m_systimeCtrl.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));

	SYSTEMTIME st;
	CTime::GetCurrentTime().GetAsSystemTime(st);
	st.wHour = 0;
	st.wSecond = 0;
	st.wMinute = 0;
	m_logStartTime = CTime(st);
	st.wHour = 23;
	st.wSecond = 59;
	st.wMinute = 59;
	m_logEndTime = CTime(st);
	
	m_voiceVolume = 50;
	
	UpdateData(FALSE);
	CButton *button = NULL;
	for (int i = IDC_CHECK_LOG_SYSOP; i <= IDC_CHECK_LOG_ERROR; i++)
	{	
		button = reinterpret_cast<CButton *>(GetDlgItem(i));
		button->SetCheck(TRUE);
	}
	UpdateUI();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CToolDlg::OnBnClickedButtonVoiceComm()
{
	if (m_isStartVoiceComm_MR)
	{
		return;
	}
	if (!m_isStartVoiceComm)
	{
		LONG handle = NET_SDK_StartVoiceCom(m_userID, FALSE, NULL, NULL);
		if (handle > 0)
		{
			m_voiceHandle = handle;
			m_isStartVoiceComm = TRUE;
		}
		else
		{
			AfxMessageBox(STR_START_FAILED);
		}
	}
	else
	{
		NET_SDK_StopVoiceCom(m_voiceHandle);
		m_isStartVoiceComm = FALSE;
		m_voiceHandle = -1;
	}
	UpdateUI();
}

void CToolDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CPoint point;
	GetCursorPos(&point);

	CRect rect;
	GetDlgItem(IDC_SLIDER_VOICECOMM_VOLUME)->GetWindowRect(&rect);
	
	UpdateData();

	if (rect.PtInRect(point))
	{
		unsigned short volume = 0;
		double temp = (((double)m_voiceVolume) / 100) * 0xffff;
		volume = (short )temp;

		NET_SDK_SetVoiceComClientVolume(m_voiceHandle, volume);	
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CToolDlg::UpdateUI()
{
	if (m_isStartVoiceComm)
	{
		SetDlgItemText(IDC_BUTTON_VOICE_COMM, STR_CLOSE);
	}
	else
	{
		SetDlgItemText(IDC_BUTTON_VOICE_COMM, STR_START);
	}

	if (m_isStartVoiceComm_MR)
	{
		SetDlgItemText(IDC_BUTTON_TALKT_RANS, STR_CLOSE);
	}
	else
	{
		SetDlgItemText(IDC_BUTTON_TALKT_RANS, STR_TRANS);
	}
	
	if (m_isOpenMsgCallback)
	{
		SetDlgItemText(IDC_BUTTON_MSG_CALLBACK, STR_CLOSE);
	}
	else
	{
		SetDlgItemText(IDC_BUTTON_MSG_CALLBACK, STR_START);
	}

	GetDlgItem(IDC_SLIDER_VOICECOMM_VOLUME)->EnableWindow(m_isStartVoiceComm);

	BOOL hasDisk = m_diskGrid.GetRowCount() > 1;
	
	GetDlgItem(IDC_BUTTON_CHANGE_DISK_PROPERTY)->EnableWindow(hasDisk);
	GetDlgItem(IDC_BUTTON_DISK_FORMAT)->EnableWindow(hasDisk);
	GetDlgItem(IDC_BUTTON_VOICE_COMM)->EnableWindow(!m_isStartVoiceComm_MR);
	GetDlgItem(IDC_BUTTON_TALKT_RANS)->EnableWindow(!m_isStartVoiceComm);
}

void CToolDlg::OnBnClickedButtonChangesystime()
{
	UpdateData();
	BOOL ret = NET_SDK_ChangTime(m_userID, m_sysTime.GetTime());
	if (!ret)
	{
		AfxMessageBox(STR_SET_TIME_FAILED);
	}
	else
	{
		AfxMessageBox(STR_SUCCESS, MB_ICONINFORMATION);
	}
}

void CToolDlg::OnBnClickedButtonDiskRefresh()
{
	m_diskGrid.DeleteNonFixedRows();
	m_diskList.RemoveAll();

	LONG diskHandle = NET_SDK_FindDisk(m_userID);
	if (diskHandle > 0)
	{
		while (true)
		{
			NET_SDK_DISK_INFO diskInfo;
			if (!NET_SDK_GetNextDiskInfo(diskHandle, &diskInfo))
			{
				NET_SDK_FindDiskClose(diskHandle);
				break;
			}

			m_diskList.AddTail(diskInfo);

			CString temp;
			int n = m_diskGrid.InsertRow(_T(""));

			temp.Format(_T("%0.2f"), (double)diskInfo.diskTotalSpace / 1024.0);
			m_diskGrid.SetItemText(n, 0, temp);
			m_diskGrid.SetItemState(n, 0, GVIS_READONLY);

			temp.Format(_T("%0.2f"), (double)diskInfo.diskFreeSpace / 1024.0);
			m_diskGrid.SetItemText(n, 1, temp);
			m_diskGrid.SetItemState(n, 1, GVIS_READONLY);

			if (diskInfo.diskStatus == NET_SDK_DISK_NORMAL)
			{
				temp = STR_NORMAL;
			}
			else if (diskInfo.diskStatus == NET_SDK_DISK_NO_FORMAT)
			{
				temp = STR_UNFORMAT;
			}
			m_diskGrid.SetItemText(n, 2, temp);
			m_diskGrid.SetItemState(n, 2, GVIS_READONLY);

			if (diskInfo.diskProperty == NET_SDK_DISK_READ_WRITE)
			{
				temp = STR_READ_WRITE;
			}
			else if (diskInfo.diskProperty == NET_SDK_DISK_ONLY_READ)
			{
				temp = STR_READ_ONLY;
			}
			else
			{
				temp = _T("");
			}

			m_diskGrid.SetItemText(n, 3, temp);
			m_diskGrid.SetCellType(n, 3, RUNTIME_CLASS(CGridCellCombo));
			CGridCellCombo *cell = (CGridCellCombo *)m_diskGrid.GetCell(n, 3);
			cell->SetStyle(cell->GetStyle() | CBS_DROPDOWNLIST);
			CStringArray options;
			temp = STR_READ_WRITE;
			options.Add(temp);
			temp = STR_READ_ONLY;
			options.Add(temp);
			cell->SetOptions(options);
		}
		m_diskGrid.Refresh();
	}
	else
	{
		AfxMessageBox(STR_FIND_DISK_FAILED);
	}
	UpdateUI();
}

void CToolDlg::OnBnClickedButtonChangeDiskProperty()
{
	int n = m_diskGrid.GetFocusCell().row;
	if (n <= 0)
	{
		AfxMessageBox(STR_SELECT_DISK);
		return ;
	}
	NET_SDK_DISK_INFO &diskInfo = m_diskList.GetAt(m_diskList.FindIndex(n - 1));
	CString temp = m_diskGrid.GetItemText(n, 3);
	LONG newProperty = -1;
	if (temp.CompareNoCase(STR_READ_ONLY) == 0)
	{
		newProperty = NET_SDK_DISK_ONLY_READ;
	}
	else if (temp.CompareNoCase(STR_READ_WRITE) == 0)
	{
		newProperty = NET_SDK_DISK_READ_WRITE;
	}

	if (diskInfo.diskProperty == newProperty)
	{
		AfxMessageBox(STR_PROPERTY_NOT_CHANGE);
		return ;
	}
	if (AfxMessageBox(STR_CONFIRM_MODIFY_DISK, MB_YESNO) == IDNO)
	{
		return;
	}
	
	BOOL ret = NET_SDK_ChangeDiskProperty(m_userID, diskInfo.diskIndex, newProperty);
	if (ret)
	{
		diskInfo.diskProperty = newProperty;
		AfxMessageBox(STR_SUCCESS, MB_ICONINFORMATION);
	}
	else
	{
		if (diskInfo.diskProperty == NET_SDK_DISK_READ_WRITE)
		{
			temp = STR_READ_WRITE;
		}
		else if (diskInfo.diskProperty == NET_SDK_DISK_ONLY_READ)
		{
			temp = STR_READ_ONLY;
		}
		m_diskGrid.SetItemText(n, 3, temp);
		m_diskGrid.RedrawRow(n);
		AfxMessageBox(STR_MODIFY_FAILED);
	}
}

void CToolDlg::OnBnClickedButtonDiskFormat()
{
	int n = m_diskGrid.GetFocusCell().row;
	if (n <= 0)
	{
		AfxMessageBox(STR_SELECT_DISK);
		return ;
	}
	NET_SDK_DISK_INFO &diskInfo = m_diskList.GetAt(m_diskList.FindIndex(n - 1));
	if (AfxMessageBox(STR_CONFIRM_FORMAT, MB_YESNO) == IDNO)
	{
		return;
	}

	LONG formatHandle = NET_SDK_FormatDisk(m_userID, diskInfo.diskIndex);
	if (formatHandle > 0)
	{
		CFormatDlg dlg;
		dlg.SetFormatHandle(formatHandle);
		dlg.DoModal();
		NET_SDK_CloseFormatHandle(formatHandle);
	}
	else
	{
		AfxMessageBox(STR_FAILED);
	}
}

void CToolDlg::OnBnClickedButtonMsgCallback()
{
	if (m_isOpenMsgCallback)
	{
		NET_SDK_CloseAlarmChan(m_msgCBHandle);
		m_mainDlg->SendMessage(WM_SHOW_MSG_DLG, 0, 0);
		m_isOpenMsgCallback = FALSE;
	}
	else
	{
		LONG handle  = NET_SDK_SetupAlarmChan(m_userID);
		if (handle > 0)
		{
			m_msgCBHandle = handle;
			m_mainDlg->SendMessage(WM_SHOW_MSG_DLG, 0, 1);
			m_isOpenMsgCallback = TRUE;
		}
		else
		{
			AfxMessageBox(STR_FAILED);
		}
		
	}
	UpdateUI();
}

void CToolDlg::setMainDlg( CWnd* dlg )
{
	m_mainDlg = dlg;
}

void CToolDlg::OnBnClickedButtonTalktRans()
{
	if (m_isStartVoiceComm)
	{
		return;
	}

	if (m_isStartVoiceComm_MR)
	{
		NET_SDK_StopVoiceCom(m_talkTransHandle);
		m_runTalkTransThread = FALSE;
		WaitForSingleObject(m_talkTransThread, INFINITE);
		m_isStartVoiceComm_MR = FALSE;
		m_talkTransHandle = -1;
		CloseHandle(m_talkDataFile);
		m_talkDataFile = INVALID_HANDLE_VALUE;
	}
	else
	{
		LONG handle = NET_SDK_StartVoiceCom_MR(m_userID, TRUE, &CToolDlg::TalkTransCallBack, this);
		if (handle > 0)
		{
			char *pAudioInfo = new char[20];
			BOOL bRet = NET_SDK_GetAudioInfo(handle, pAudioInfo, 20);
			if(bRet)
			{
				WAVEFORMATEX *pWaveFormat = (WAVEFORMATEX *)pAudioInfo;
				TRACE("tag:%d, bits:%d, bytes:%d\n", pWaveFormat->wFormatTag, pWaveFormat->wBitsPerSample, pWaveFormat->nAvgBytesPerSec);
			}
			delete [] pAudioInfo;
			m_runTalkTransThread = TRUE;
			m_talkTransThread = CreateThread(NULL, 0, &CToolDlg::TalkTransSendThread, this, 0, NULL);
			m_talkTransHandle = handle;
			m_isStartVoiceComm_MR = TRUE;
			m_talkDataFile = CreateFile(_T("D:\\TalkTrans.data"), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		}
		else
		{
			AfxMessageBox(STR_FAILED);
		}
	}
	UpdateUI();
}

void CToolDlg::TalkTransCallBack( LONG lVoiceComHandle, char *pRecvDataBuffer, DWORD dwBufSize, BYTE byAudioFlag, void* pUser )
{
	DWORD written = 0;
	CToolDlg *pThis = reinterpret_cast<CToolDlg *>(pUser);
	WriteFile(pThis->m_talkDataFile, pRecvDataBuffer, dwBufSize, &written, NULL);
}

DWORD WINAPI CToolDlg::TalkTransSendThread( LPVOID params )
{
	CToolDlg *pThis = reinterpret_cast<CToolDlg *>(params);
	pThis->TalkTransSendProc();
	return 0;
}

void CToolDlg::TalkTransSendProc()
{
	unsigned char *buff = new unsigned char[3200];
	HCRYPTPROV provider;
	CryptAcquireContext(&provider, NULL, NULL, PROV_RSA_FULL, 0);

	while(m_runTalkTransThread)
	{
		CryptGenRandom(provider, 3200, buff);
		NET_SDK_VoiceComSendData(m_talkTransHandle, (char *)buff, 3200);
		Sleep(100);
	}
	CryptReleaseContext(provider, 0);
	delete[] buff;
}
