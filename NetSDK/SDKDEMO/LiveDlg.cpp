// LiveDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SDKDEMO.h"
#include "LiveDlg.h"
#include "PTZ.h"
#include "CruiseDlg.h"
#include "strdef.h"

// CLiveDlg 对话框

//#define _WRITE_USER_FILE_

IMPLEMENT_DYNAMIC(CLiveDlg, CDialog)

CLiveDlg::CLiveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLiveDlg::IDD, pParent)
	, m_deviceInfo(NULL)
	, m_isOpenAudio(FALSE)
	, m_chnn(0)
	, m_brightness(0)
	, m_hue(0)
	, m_saturation(0)
	, m_contrast(0)
	, m_volume(0)
	, m_currentDivide(0)
	, m_audioDivide(0)
	, m_presetNo(0)
	, m_cruiseNo(0)
	, m_isSingleLive(FALSE)
{
	memset(m_playHandle, 0x0, sizeof(LONG) * MAX_DIVIDE_NUM);
	memset(m_playChnn, 0x0, sizeof(int) * MAX_DIVIDE_NUM);
	memset(m_isStartLive, 0x0, sizeof(BOOL) * MAX_DIVIDE_NUM);
	memset(m_isOpenIris, 0x0, sizeof(BOOL) * MAX_DIVIDE_NUM);
	memset(m_isOpenAutoScan, 0x0, sizeof(BOOL) * MAX_DIVIDE_NUM);
	memset(m_isTrackStart, 0x0, sizeof(BOOL) * MAX_DIVIDE_NUM);
	memset(m_isTrackRecording, 0x0, sizeof(BOOL) * MAX_DIVIDE_NUM);
	memset(m_isCruiseStart, 0x0, sizeof(BOOL) * MAX_DIVIDE_NUM);
	memset(m_isSavingData, 0x0, sizeof(BOOL) * MAX_DIVIDE_NUM);
	memset(m_presetSel, 0x0, sizeof(int) * MAX_DIVIDE_NUM);
	memset(m_cruiseSel, 0x0, sizeof(int) * MAX_DIVIDE_NUM);
}

CLiveDlg::~CLiveDlg()
{
}

void CLiveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL, m_chnn);
	DDX_Slider(pDX, IDC_SLIDER_BRIGHTNESS, m_brightness);
	DDX_Slider(pDX, IDC_SLIDER_HUE, m_hue);
	DDX_Slider(pDX, IDC_SLIDER_SATURATION, m_saturation);
	DDX_Slider(pDX, IDC_SLIDER_CONTRAST, m_contrast);
	DDX_Slider(pDX, IDC_SLIDER_VOLUME, m_volume);
	DDX_Control(pDX, IDC_SLIDER_PTZ_SPEED, m_PTZSpeedCtrl);
	DDX_CBIndex(pDX, IDC_COMBO_PRESET, m_presetNo);
	DDX_CBIndex(pDX, IDC_COMBO_CRUISE, m_cruiseNo);
}


BEGIN_MESSAGE_MAP(CLiveDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CLiveDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLiveDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_LIVE, &CLiveDlg::OnBnClickedButtonLive)
	ON_CBN_SELCHANGE(IDC_COMBO_CHANNEL, &CLiveDlg::OnCbnSelchangeComboChannel)
	ON_COMMAND(ID_LIVE_CAPTURE, &CLiveDlg::OnLiveCapture)
	ON_COMMAND(ID_LIVE_AUDIO, &CLiveDlg::OnLiveAudio)
	ON_UPDATE_COMMAND_UI(ID_LIVE_AUDIO, &CLiveDlg::OnUpdateLiveAudio)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_DEFAULT, &CLiveDlg::OnBnClickedButtonColorDefault)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_SAVE, &CLiveDlg::OnBnClickedButtonColorSave)
	ON_WM_SHOWWINDOW()
	ON_WM_CONTEXTMENU()
	ON_WM_INITMENUPOPUP()
	ON_WM_HSCROLL()
	ON_CONTROL_RANGE(STN_CLICKED, IDC_LIVE_DIVIDE_1, IDC_LIVE_DIVIDE_16, OnClickedDivide)
	ON_CONTROL_RANGE(STN_DBLCLK, IDC_LIVE_DIVIDE_1, IDC_LIVE_DIVIDE_16, OnDBClickedDivide)
	ON_STN_DBLCLK(IDC_LIVE_AREA, &CLiveDlg::OnStnDblclickLiveArea)
	ON_WM_PAINT()
	//ON_BN_CLICKED(IDC_BUTTON_PTZ1, &CLiveDlg::OnBnClickedButtonPtz1)
	//ON_BN_CLICKED(IDC_BUTTON_PTZ2, &CLiveDlg::OnBnClickedButtonPtz2)
	//ON_BN_CLICKED(IDC_BUTTON_PTZ3, &CLiveDlg::OnBnClickedButtonPtz3)
	//ON_BN_CLICKED(IDC_BUTTON_PTZ4, &CLiveDlg::OnBnClickedButtonPtz4)
	//ON_BN_CLICKED(IDC_BUTTON_PTZ5, &CLiveDlg::OnBnClickedButtonPtz5)
	//ON_BN_CLICKED(IDC_BUTTON_PTZ6, &CLiveDlg::OnBnClickedButtonPtz6)
	//ON_BN_CLICKED(IDC_BUTTON_PTZ7, &CLiveDlg::OnBnClickedButtonPtz7)
	//ON_BN_CLICKED(IDC_BUTTON_PTZ8, &CLiveDlg::OnBnClickedButtonPtz8)
	//ON_BN_CLICKED(IDC_BUTTON_PTZ9, &CLiveDlg::OnBnClickedButtonPtz9)
	ON_BN_CLICKED(IDC_BUTTON_NEAR, &CLiveDlg::OnBnClickedButtonNear)
	ON_BN_CLICKED(IDC_BUTTON_FAR, &CLiveDlg::OnBnClickedButtonFar)
	ON_BN_CLICKED(IDC_BUTTON_OUT, &CLiveDlg::OnBnClickedButtonOut)
	ON_BN_CLICKED(IDC_BUTTON_IN, &CLiveDlg::OnBnClickedButtonIn)
	ON_BN_CLICKED(IDC_BUTTON_IRIS, &CLiveDlg::OnBnClickedButtonIris)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_SCAN, &CLiveDlg::OnBnClickedButtonAutoScan)
	ON_BN_CLICKED(IDC_BUTTON_PRESET_SET, &CLiveDlg::OnBnClickedButtonPresetSet)
	ON_BN_CLICKED(IDC_BUTTON_PRESET_GO, &CLiveDlg::OnBnClickedButtonPresetGo)
	ON_BN_CLICKED(IDC_BUTTON_PRESET_DEL, &CLiveDlg::OnBnClickedButtonPresetDel)
	ON_BN_CLICKED(IDC_BUTTON_CRUISE_SETUP, &CLiveDlg::OnBnClickedButtonCruiseSetup)
	ON_BN_CLICKED(IDC_BUTTON_TRACK_RECORD, &CLiveDlg::OnBnClickedButtonTrackRecord)
	ON_BN_CLICKED(IDC_BUTTON_TRACK, &CLiveDlg::OnBnClickedButtonTrack)
	ON_BN_CLICKED(IDC_BUTTON_CRUISE, &CLiveDlg::OnBnClickedButtonCruise)
	ON_CBN_SELCHANGE(IDC_COMBO_PRESET, &CLiveDlg::OnCbnSelchangeComboPreset)
	ON_CBN_SELCHANGE(IDC_COMBO_CRUISE, &CLiveDlg::OnCbnSelchangeComboCruise)
	ON_BN_CLICKED(IDC_BUTTON_CAPTURE, &CLiveDlg::OnBnClickedButtonCapture)
	ON_BN_CLICKED(IDC_BUTTON_LIVESAVE, &CLiveDlg::OnBnClickedButtonLivesave)
END_MESSAGE_MAP()


// CLiveDlg 消息处理程序

void CLiveDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnOK();
}

void CLiveDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnCancel();
}

void CLiveDlg::SetDeviceInfo( NET_SDK_DEVICEINFO* di, LONG userId)
{
	m_deviceInfo = di;
	
	CString temp;
	CComboBox * combo = reinterpret_cast<CComboBox *>(GetDlgItem(IDC_COMBO_CHANNEL));
	for (int i = 0; i < m_deviceInfo->videoInputNum; i++)
	{
		temp.Format(STR_CHANNEL_D, i + 1);
		combo->AddString(temp);
	}
	combo->SetCurSel(m_playChnn[m_currentDivide]);

	if (userId >= 0)
	{
		m_userID = userId;
	}

	GetChnnInfo(0);
}

void CLiveDlg::OnBnClickedButtonLive()
{
	UpdateData();

	if (m_isStartLive[m_currentDivide])
	{
		StopLive();
	}
	else
	{
		StartLive();
	}
	
	UpdateUI();
}

void CLiveDlg::StartLive()
{
	NET_SDK_CLIENTINFO clientInfo = {0};
	clientInfo.hPlayWnd = GetDlgItem(IDC_LIVE_DIVIDE_1 + m_currentDivide)->GetSafeHwnd();
	clientInfo.lChannel = m_chnn;
	//clientInfo.streamType = NET_SDK_MAIN_STREAM /*NET_SDK_SUB_STREAM*/;
	CComboBox* pStreamTypeCtrl = reinterpret_cast<CComboBox*>(GetDlgItem(IDC_COMBO_StreamType));
	if (pStreamTypeCtrl)
	{
		clientInfo.streamType = (LONG)pStreamTypeCtrl->GetCurSel();
	}

#ifdef _WRITE_USER_FILE_
	m_RecFilehandle[m_currentDivide] = CreateFile(_T("J:/test.rec"), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	m_recNeedFormat[m_currentDivide] = true;
#endif
#if 1
	LONG playHandle = NET_SDK_LivePlay(m_userID, &clientInfo, CLiveDlg::LiveCallBack, this);
#else
	LONG playHandle = NET_SDK_LivePlay(m_userID, &clientInfo, NULL, NULL);
#endif
	NET_SDK_RegisterDrawFun(playHandle, &CLiveDlg::drawCallBack, this);
	if (playHandle > 0)
	{
		m_playHandle[m_currentDivide] = playHandle;
		m_playChnn[m_currentDivide] = m_chnn;
		m_isStartLive[m_currentDivide] = TRUE;

		if(0 == m_currentDivide)
		{
			WAVEFORMATEX wfx;
			wfx.wFormatTag = WAVE_FORMAT_PCM;
			wfx.nChannels = 1;
			wfx.wBitsPerSample = 16;
			wfx.nSamplesPerSec = 8000;
			wfx.nBlockAlign = 2;
			wfx.nAvgBytesPerSec = 16000;
			wfx.cbSize = 0;
			m_waveOut.Start(wfx);
		}
	}
	else
	{
		DWORD dwErrorCode = NET_SDK_GetLastError();
		CString strMsg;
		strMsg.Format(_T("%s\r\n错误代码：%d"), STR_LIVE_FAILED, dwErrorCode);
		AfxMessageBox(/*STR_LIVE_FAILED*/strMsg);
	}
}

void CLiveDlg::StopLive()
{
	if (!m_isStartLive[m_currentDivide])
	{
		return;
	}
	NET_SDK_StopLivePlay(m_playHandle[m_currentDivide]);
	NET_SDK_ReleaseAudioDecoder(m_lAudioDecHandle[m_currentDivide]);
	if(0 == m_currentDivide)
	{
		m_waveOut.Stop();
	}
	m_playHandle[m_currentDivide] = 0;
	m_isStartLive[m_currentDivide] = FALSE;

#ifdef _WRITE_USER_FILE_
	CloseHandle(m_RecFilehandle[m_currentDivide]);
#endif
	CRect rect;
	CWnd *wnd = NULL;
	
	wnd = GetDlgItem(IDC_LIVE_DIVIDE_1 + m_currentDivide);
	wnd->Invalidate();
}

BOOL CLiveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CSliderCtrl *slider = NULL;
	for (int i = IDC_SLIDER_BRIGHTNESS; i <= IDC_SLIDER_CONTRAST; i++)
	{
		slider = reinterpret_cast<CSliderCtrl *>(GetDlgItem(i));
		slider->SetRangeMax(255);
	}
	
	CComboBox *combo = reinterpret_cast<CComboBox*>(GetDlgItem(IDC_COMBO_PRESET));
	CString temp;
	for (int i = 0; i < 128; i++)
	{
		temp.Format(_T("preset %d"), i + 1);
		combo->AddString(temp);
	}
	
	combo = reinterpret_cast<CComboBox*>(GetDlgItem(IDC_COMBO_CRUISE));
	for (int i = 0; i < 32; i++)
	{
		temp.Format(_T("cruise %d"), i + 1);
		combo->AddString(temp);
	}
	
	slider = reinterpret_cast<CSliderCtrl *>(GetDlgItem(IDC_SLIDER_VOLUME));
	slider->SetRangeMax(100);
	
	m_volume = 50;
	
	m_PTZSpeedCtrl.SetRangeMax(8);
	m_PTZSpeedCtrl.SetRangeMin(1);
	CButton *button = NULL;
	
	for (int i = IDC_BUTTON_PTZ1, j = IDB_BITMAP_PTZ1; i <= IDC_BUTTON_PTZ9; i++, j++)
	{
		button = reinterpret_cast<CButton*>(GetDlgItem(i));
		m_ptzBitmap[i - IDC_BUTTON_PTZ1].LoadBitmap(j);
		button->SetBitmap(m_ptzBitmap[i - IDC_BUTTON_PTZ1]);
	}

	CRect rect, drect;
	GetDlgItem(IDC_LIVE_AREA)->GetWindowRect(&rect);
	ScreenToClient(&rect);

	for(int row = 0; row < 4; row++)
	{
		for(int col = 0; col < 4; col++)
		{
			drect.top = rect.top + row * rect.Height()/4 + 1;
			drect.left = rect.left + col * rect.Width()/4 + 1;
			drect.right = drect.left + rect.Width()/4 - 1;
			drect.bottom = drect.top + rect.Height()/4 - 1;
			GetDlgItem(IDC_LIVE_DIVIDE_1+row*4+col)->MoveWindow(&drect);
		}
	}

	//码流类型

	m_eLiveStreamType = NET_SDK_MAIN_STREAM;
	CComboBox* pStreamTypeCtrl = reinterpret_cast<CComboBox*>(GetDlgItem(IDC_COMBO_StreamType));
	if (pStreamTypeCtrl)
	{
		pStreamTypeCtrl->AddString(_T("MainStream"));
		pStreamTypeCtrl->AddString(_T("SubStream"));
		pStreamTypeCtrl->SetCurSel((int)m_eLiveStreamType);
	}
	UpdateUI();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CLiveDlg::GetChnnInfo( int chnn )
{
	DWORD brightness;
	DWORD contrast;
	DWORD saturation;
	DWORD hue;

	NET_SDK_GetVideoEffect(m_userID, chnn, &brightness, &contrast, &saturation, &hue);
	m_brightness = brightness;
	m_contrast = contrast;
	m_saturation = saturation;
	m_hue = hue;
	
	UpdateData(FALSE);
}

void CLiveDlg::OnCbnSelchangeComboChannel()
{
	UpdateData();
	
	GetChnnInfo(m_chnn);

	if (m_isStartLive[m_currentDivide])
	{
		StopLive();
		StartLive();
	}
}

void CLiveDlg::OnLiveCapture()
{
	CFileDialog dlg(FALSE, _T("bmp"), _T("sample.bmp"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("bmp files(*.bmp)|*.bmp||"));
	if (IDOK == dlg.DoModal())
	{
		CString pathName = dlg.GetPathName();
		BOOL ret = NET_SDK_CapturePicture(m_playHandle[m_currentDivide], pathName.GetBuffer());
		if (!ret)
		{
			AfxMessageBox(STR_SNAP_FAILED);
		}
	}
}

void CLiveDlg::OnLiveAudio()
{
	if (m_isOpenAudio)
	{
		NET_SDK_CloseSound();
		m_isOpenAudio = FALSE;
	}	
	else
	{
		UpdateData();
		BOOL ret = NET_SDK_OpenSound(m_playHandle[m_currentDivide]);
		if (ret)
		{
			unsigned short volume = 0;
			double temp = (((double)m_volume) / 100) * 0xffff;
			volume = (short )temp;
			NET_SDK_Volume(m_playHandle[m_currentDivide], volume);
			m_isOpenAudio = TRUE;
			m_audioDivide = m_currentDivide;
		}
		else
		{
			AfxMessageBox(STR_START_AUDIO_FAILED);
		}
	}
}

void CLiveDlg::OnUpdateLiveAudio(CCmdUI *pCmdUI)
{
	if (m_isOpenAudio)
	{
		if (m_audioDivide == m_currentDivide)
		{
			pCmdUI->SetText(STR_CLOSE_AUDIO);
		}
		else
		{
			pCmdUI->Enable(FALSE);
		}
	}
	else
	{
		pCmdUI->SetText(STR_START_AUDIO);
	}
}

void CLiveDlg::OnBnClickedButtonColorDefault()
{
	DWORD brightness;
	DWORD contrast;
	DWORD saturation;
	DWORD hue;

	NET_SDK_GetDefaultVideoEffect(m_userID, &brightness, &contrast, &saturation, &hue);
	m_brightness = brightness;
	m_contrast = contrast;
	m_saturation = saturation;
	m_hue = hue;
	
	NET_SDK_SetVideoEffect(m_userID, m_chnn, m_brightness, m_contrast, m_saturation, m_hue);

	UpdateData(FALSE);
}

void CLiveDlg::OnBnClickedButtonColorSave()
{
	UpdateData();

	BOOL ret = NET_SDK_SaveVideoEffect(m_userID, m_chnn, m_brightness, m_contrast, m_saturation, m_hue);
	//if (ret)
	//{
	//	AfxMessageBox(_T("保存成功"), MB_ICONINFORMATION);
	//}
	//else
	//{
	//	DWORD error = NET_SDK_GetLastError();
	//	AfxMessageBox(_T("保存失败"));
	//}
}

void CLiveDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if (!bShow)
	{
		int temp = m_currentDivide;
		for (int i = 0; i < MAX_DIVIDE_NUM; i++)
		{
			m_currentDivide = i;
			StopLive();
		}
		m_currentDivide = temp;
		
		NET_SDK_CloseSound();
		m_isOpenAudio = FALSE;
		UpdateUI();
	}
	else
	{
		CRect rect;
		GetDlgItem(IDC_LIVE_AREA)->GetClientRect(&rect);
		CDC *dc = GetDlgItem(IDC_LIVE_AREA)->GetDC();
		CBrush brush(RGB(0, 0, 0));
		dc->FillRect(&rect, &brush);
		GetDlgItem(IDC_LIVE_AREA)->ReleaseDC(dc);
		Invalidate(TRUE);
	}
	CDialog::OnShowWindow(bShow, nStatus);
}


void CLiveDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CRect rect;
	
	if (!m_isSingleLive)
	{
		for (int i = 0; i < MAX_DIVIDE_NUM; i++)
		{
			GetDlgItem(IDC_LIVE_DIVIDE_1 + i)->GetWindowRect(&rect);
			if (rect.PtInRect(point))
			{
				OnClickedDivide(IDC_LIVE_DIVIDE_1 + i);
			}
		}
	}

	GetDlgItem(IDC_LIVE_DIVIDE_1 + m_currentDivide)->GetWindowRect(&rect);

	if (m_isStartLive[m_currentDivide] && rect.PtInRect(point))
	{
		CPoint p;
		GetCursorPos(&p);

		CMenu menu;
		menu.LoadMenu(IDR_MENU_POPUP);
		menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);
	}
}

void CLiveDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	DrawGrid();
}

void CLiveDlg::OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex,BOOL bSysMenu)
{
	//CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	// TODO: Add your message handler code here
	ASSERT(pPopupMenu != NULL);
	// Check the enabled state of various menu items.

	CCmdUI state;
	state.m_pMenu = pPopupMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);

	// Determine if menu is popup in top-level menu and set m_pOther to
	// it if so (m_pParentMenu == NULL indicates that it is secondary popup).
	HMENU hParentMenu;
	if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
		state.m_pParentMenu = pPopupMenu;    // Parent == child for tracking popup.
	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
	{
		CWnd* pParent = this;
		// Child windows don't have menus--need to go to the top!
		if (pParent != NULL &&
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				if (::GetSubMenu(hParentMenu, nIndex) == pPopupMenu->m_hMenu)
				{
					// When popup is found, m_pParentMenu is containing menu.
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}

	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
		state.m_nIndex++)
	{
		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // Menu separator or invalid cmd - ignore it.

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// Possibly a popup menu, route to first item of that popup.
			state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // First item of popup can't be routed to.
			}
			state.DoUpdate(this, TRUE);   // Popups are never auto disabled.
		}
		else
		{
			// Normal menu item.
			// Auto enable/disable if frame window has m_bAutoMenuEnable
			// set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, FALSE);
		}

		// Adjust for menu deletions and additions.
		UINT nCount = pPopupMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}

}

void CLiveDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateData();

	CPoint point;
	GetCursorPos(&point);
	CRect rect1, rect2;
	GetDlgItem(IDC_SLIDER_VOLUME)->GetWindowRect(&rect1);
	m_PTZSpeedCtrl.GetWindowRect(&rect2);
	if (rect1.PtInRect(point))
	{
		unsigned short volume = 0;
		double temp = (((double)m_volume) / 100) * 0xffff;
		volume = (short )temp;

		NET_SDK_Volume(m_playHandle[m_currentDivide], volume);
	}
	else if (!rect2.PtInRect(point))
	{
		NET_SDK_SetVideoEffect(m_userID, m_chnn, m_brightness, m_contrast, m_saturation, m_hue);
	}
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CLiveDlg::UpdateUI()
{
	for (int i = IDC_SLIDER_BRIGHTNESS; i <= IDC_SLIDER_VOLUME; i++)
	{
		GetDlgItem(i)->EnableWindow(m_isStartLive[m_currentDivide]);
	}
	
	for (int i = IDC_BUTTON_PTZ1; i <= IDC_BUTTON_PTZ9; i++)
	{
		GetDlgItem(i)->EnableWindow(m_isStartLive[m_currentDivide]);
	}

	m_PTZSpeedCtrl.EnableWindow(m_isStartLive[m_currentDivide]);

	GetDlgItem(IDC_BUTTON_NEAR)->EnableWindow(m_isStartLive[m_currentDivide]);
	GetDlgItem(IDC_BUTTON_FAR)->EnableWindow(m_isStartLive[m_currentDivide]);
	GetDlgItem(IDC_BUTTON_OUT)->EnableWindow(m_isStartLive[m_currentDivide]);
	GetDlgItem(IDC_BUTTON_IN)->EnableWindow(m_isStartLive[m_currentDivide]);
	GetDlgItem(IDC_BUTTON_IRIS)->EnableWindow(m_isStartLive[m_currentDivide]);
	GetDlgItem(IDC_BUTTON_AUTO_SCAN)->EnableWindow(m_isStartLive[m_currentDivide]);

	GetDlgItem(IDC_COMBO_PRESET)->EnableWindow(m_isStartLive[m_currentDivide]);
	GetDlgItem(IDC_BUTTON_PRESET_SET)->EnableWindow(m_isStartLive[m_currentDivide]);
	GetDlgItem(IDC_BUTTON_PRESET_GO)->EnableWindow(m_isStartLive[m_currentDivide]);
	GetDlgItem(IDC_BUTTON_PRESET_DEL)->EnableWindow(m_isStartLive[m_currentDivide]);
	GetDlgItem(IDC_BUTTON_TRACK_RECORD)->EnableWindow(m_isStartLive[m_currentDivide]);
	GetDlgItem(IDC_BUTTON_TRACK)->EnableWindow(m_isStartLive[m_currentDivide]);
	GetDlgItem(IDC_COMBO_CRUISE)->EnableWindow(m_isStartLive[m_currentDivide] && !m_isCruiseStart[m_currentDivide]);
	GetDlgItem(IDC_BUTTON_CRUISE)->EnableWindow(m_isStartLive[m_currentDivide]);
	GetDlgItem(IDC_BUTTON_CRUISE_SETUP)->EnableWindow(m_isStartLive[m_currentDivide]);
	
	GetDlgItem(IDC_BUTTON_LIVESAVE)->EnableWindow(m_isStartLive[m_currentDivide]);

	m_chnn = m_playChnn[m_currentDivide];

	if(m_isStartLive[m_currentDivide])
	{
		SetDlgItemText(IDC_BUTTON_LIVE, STR_CLOSE);
	}
	else
	{
		SetDlgItemText(IDC_BUTTON_LIVE, STR_START);
	}

	if (m_isOpenIris[m_currentDivide])
	{
		SetDlgItemText(IDC_BUTTON_IRIS, STR_CLOSE);
	}
	else
	{
		SetDlgItemText(IDC_BUTTON_IRIS, STR_START);
	}

	if (m_isOpenAutoScan[m_currentDivide])
	{
		SetDlgItemText(IDC_BUTTON_AUTO_SCAN, STR_CLOSE);
	}
	else
	{
		SetDlgItemText(IDC_BUTTON_AUTO_SCAN, STR_START);
	}

	if (m_isTrackStart[m_currentDivide])
	{
		SetDlgItemText(IDC_BUTTON_TRACK, STR_STOP);
	}
	else
	{
		SetDlgItemText(IDC_BUTTON_TRACK, STR_START);
	}

	if (m_isTrackRecording[m_currentDivide])
	{
		SetDlgItemText(IDC_BUTTON_TRACK_RECORD, STR_STOP);
	}
	else
	{
		SetDlgItemText(IDC_BUTTON_TRACK_RECORD, STR_RECORD);
	}

	if (m_isCruiseStart[m_currentDivide])
	{
		SetDlgItemText(IDC_BUTTON_CRUISE, STR_STOP);
	}
	else
	{
		SetDlgItemText(IDC_BUTTON_CRUISE, STR_START);
	}

	if(m_isSavingData[m_currentDivide])
	{
		SetDlgItemText(IDC_BUTTON_LIVESAVE, _T("stop"));
	}
	else
	{
		SetDlgItemText(IDC_BUTTON_LIVESAVE, _T("save"));
	}

	m_presetNo = m_presetSel[m_currentDivide];
	m_cruiseNo = m_cruiseSel[m_currentDivide];

	GetChnnInfo(m_playChnn[m_currentDivide]);
}

void CLiveDlg::OnClickedDivide( UINT index )
{
	m_currentDivide = index - IDC_LIVE_DIVIDE_1;
	UpdateUI();
	DrawGrid();
}

void CLiveDlg::OnDBClickedDivide( UINT index )
{
	if (m_isSingleLive)
	{
		m_isSingleLive = FALSE;
		GetDlgItem(IDC_LIVE_DIVIDE_1 + m_currentDivide)->MoveWindow(m_oldRect);
		for (int i = 0; i < MAX_DIVIDE_NUM; i++)
		{
			GetDlgItem(IDC_LIVE_DIVIDE_1 + i)->ShowWindow(SW_SHOW);
		}
		DrawGrid();
	}
	else
	{
		if (m_isStartLive[m_currentDivide])
		{
			m_isSingleLive = TRUE;

			GetDlgItem(IDC_LIVE_DIVIDE_1 + m_currentDivide)->GetWindowRect(&m_oldRect);
			ScreenToClient(&m_oldRect);

			CRect rect;
			GetDlgItem(IDC_LIVE_AREA)->GetWindowRect(&rect);
			ScreenToClient(&rect);

			GetDlgItem(m_currentDivide + IDC_LIVE_DIVIDE_1)->MoveWindow(rect);

			for (int i = 0; i < MAX_DIVIDE_NUM; i++)
			{
				if (i != m_currentDivide)
				{
					GetDlgItem(IDC_LIVE_DIVIDE_1 + i)->ShowWindow(SW_HIDE);
				}
			}
		}
	}
}

void CLiveDlg::OnStnDblclickLiveArea()
{
	if (m_isStartLive[m_currentDivide])
	{
		StopLive();
		StartLive();
	}
	
	GetDlgItem(IDC_LIVE_AREA)->ShowWindow(SW_HIDE);

	for (int i = 0; i < MAX_DIVIDE_NUM; i++)
	{
		GetDlgItem(IDC_LIVE_DIVIDE_1 + i)->ShowWindow(SW_SHOW);
	}

	DrawGrid();
}

void CLiveDlg::OnPaint()
{
	CDialog::OnPaint();
	//TRACE(_T("called paint\n"));
	DrawGrid();
	/*for(int i = 0; i < 4; i++)
	{
		if(m_isStartLive[i])
		{
			NET_SDK_RefreshPlay(m_playHandle[m_currentDivide]);
		}
	}*/
}

void CLiveDlg::DrawGrid()
{
	//if (GetDlgItem(IDC_LIVE_AREA)->IsWindowVisible())
	//{
	//	return;
	//}
	if (m_isSingleLive)
	{
		return;
	}
	CDC *dc = GetDC();
	CWnd *wnd = NULL;
	CRect rect;
	CBrush bb(RGB(0, 0, 255));
	CBrush rb(RGB(255, 0, 0));

	for (int i = 0; i < MAX_DIVIDE_NUM; i++)
	{
		wnd = GetDlgItem(IDC_LIVE_DIVIDE_1 + i);
		wnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.top--;
		rect.left--;
		rect.right++;
		rect.bottom++;
		//dc = wnd->GetDC();
		if (i == m_currentDivide)
		{
			dc->FrameRect(&rect, &rb);
		}
		else
		{
			dc->FrameRect(&rect, &bb);
		}

		//wnd->ReleaseDC(dc);
	}
	ReleaseDC(dc);
}

void CLiveDlg::OnBnClickedButtonNear()
{
	NET_SDK_PTZControl(m_playHandle[m_currentDivide], PTZ_CMD_NEAR, m_PTZSpeedCtrl.GetPos());
}

void CLiveDlg::OnBnClickedButtonFar()
{
	NET_SDK_PTZControl(m_playHandle[m_currentDivide], PTZ_CMD_FAR, m_PTZSpeedCtrl.GetPos());
}

void CLiveDlg::OnBnClickedButtonOut()
{
	NET_SDK_PTZControl(m_playHandle[m_currentDivide], PTZ_CMD_ZOOM_OUT, m_PTZSpeedCtrl.GetPos());
}

void CLiveDlg::OnBnClickedButtonIn()
{
	NET_SDK_PTZControl(m_playHandle[m_currentDivide], PTZ_CMD_ZOOM_IN, m_PTZSpeedCtrl.GetPos());
}

void CLiveDlg::OnBnClickedButtonIris()
{
	if (m_isOpenIris[m_currentDivide])
	{
		NET_SDK_PTZControl(m_playHandle[m_currentDivide], PTZ_CMD_IRIS_CLOSE, m_PTZSpeedCtrl.GetPos());
		m_isOpenIris[m_currentDivide] = FALSE;
	}
	else
	{
		NET_SDK_PTZControl(m_playHandle[m_currentDivide], PTZ_CMD_IRIS_OPEN, m_PTZSpeedCtrl.GetPos());
		m_isOpenIris[m_currentDivide] = TRUE;
	}
	UpdateUI();
}

void CLiveDlg::OnBnClickedButtonAutoScan()
{
	if (m_isOpenAutoScan[m_currentDivide])
	{
		NET_SDK_PTZAutoScan(m_playHandle[m_currentDivide], PTZ_CMD_AUTO_SCAN_STOP);
		m_isOpenAutoScan[m_currentDivide] = FALSE;
	}
	else
	{
		NET_SDK_PTZAutoScan(m_playHandle[m_currentDivide], PTZ_CMD_AUTO_SCAN_START);
		m_isOpenAutoScan[m_currentDivide] = TRUE;
	}
	UpdateUI();
}

void CLiveDlg::OnBnClickedButtonPresetSet()
{
	UpdateData();
	NET_SDK_PTZPreset(m_playHandle[m_currentDivide], PTZ_CMD_PRESET_SET, m_presetNo);
}

void CLiveDlg::OnBnClickedButtonPresetGo()
{
	UpdateData();
	NET_SDK_PTZPreset(m_playHandle[m_currentDivide], PTZ_CMD_PRESET_GO, m_presetNo);
}

void CLiveDlg::OnBnClickedButtonPresetDel()
{
	UpdateData();
	NET_SDK_PTZPreset(m_playHandle[m_currentDivide], PTZ_CMD_PRESET_DEL, m_presetNo);
}

void CLiveDlg::OnBnClickedButtonCruiseSetup()
{
	UpdateData();
	CCruiseDlg dlg;
	if (IDOK == dlg.DoModal())
	{
		int n = dlg.GetCruiseNum();
		DD_CRUISE_POINT_INFO * cpi = new DD_CRUISE_POINT_INFO[n];
		memset(cpi, 0x0, sizeof(DD_CRUISE_POINT_INFO) * n);
		dlg.GetCruiseInfo((LPVOID)cpi);
		BOOL ret = NET_SDK_PTZSetCruise(m_playHandle[m_currentDivide], m_cruiseNo, cpi, n);
		TRACE("set cruise ret: %d", ret);
	}
}

void CLiveDlg::OnBnClickedButtonTrackRecord()
{
	if (m_isTrackRecording[m_currentDivide])
	{
		NET_SDK_PTZTrack(m_playHandle[m_currentDivide], PTZ_CMD_TRACK_START_RECORD);
		m_isTrackRecording[m_currentDivide] = FALSE;
	}
	else
	{
		NET_SDK_PTZTrack(m_playHandle[m_currentDivide], PTZ_CMD_TRACK_STOP_RECORD);
		m_isTrackRecording[m_currentDivide] = TRUE;
	}
	UpdateUI();
}

void CLiveDlg::OnBnClickedButtonTrack()
{
	if (m_isTrackStart[m_currentDivide])
	{
		NET_SDK_PTZTrack(m_playHandle[m_currentDivide], PTZ_CMD_TRACK_STOP);
		m_isTrackStart[m_currentDivide] = FALSE;
	}
	else
	{
		NET_SDK_PTZTrack(m_playHandle[m_currentDivide], PTZ_CMD_TRACK_START);
		m_isTrackStart[m_currentDivide] = TRUE;
	}
	UpdateUI();
}

void CLiveDlg::OnBnClickedButtonCruise()
{
	UpdateData();
	if (m_isCruiseStart[m_currentDivide])
	{
		NET_SDK_PTZCruise(m_playHandle[m_currentDivide], PTZ_CMD_CRUISE_STOP, m_cruiseNo);
		m_isCruiseStart[m_currentDivide] = FALSE;
	}
	else
	{
		NET_SDK_PTZCruise(m_playHandle[m_currentDivide], PTZ_CMD_CRUISE_RUN, m_cruiseNo);
		m_isCruiseStart[m_currentDivide] = TRUE;
	}
	UpdateUI();
}

void CLiveDlg::OnCbnSelchangeComboPreset()
{
	UpdateData();
	m_presetSel[m_currentDivide] = m_presetNo;
}

void CLiveDlg::OnCbnSelchangeComboCruise()
{
	UpdateData();
	m_cruiseSel[m_currentDivide] = m_cruiseNo;
}

BOOL CLiveDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		for (int i = IDC_BUTTON_PTZ1; i <= IDC_BUTTON_PTZ9; i++)
		{
			if (pMsg->hwnd == GetDlgItem(i)->GetSafeHwnd())
			{
				OnPTZCtrl(i);
				break;
			}
		}
	}
	else if (pMsg->message == WM_LBUTTONUP)
	{
		for (int i = IDC_BUTTON_PTZ1; i <= IDC_BUTTON_PTZ9; i++)
		{
			if (pMsg->hwnd == GetDlgItem(i)->GetSafeHwnd())
			{
				OnPTZCtrlStop(i);
				break;
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CLiveDlg::OnPTZCtrl( int id )
{
	long command = 0;
	switch (id)
	{
	case IDC_BUTTON_PTZ1:
		command = PTZ_CMD_LEFT_DOWN;
		break;
	case IDC_BUTTON_PTZ2:
		command = PTZ_CMD_DOWN;
		break;
	case IDC_BUTTON_PTZ3:
		command = PTZ_CMD_RIGHT_DOWN;
		break;
	case IDC_BUTTON_PTZ4:
		command = PTZ_CMD_LEFT;
		break;
	case IDC_BUTTON_PTZ5:
		command = PTZ_CMD_STOP;
		break;
	case IDC_BUTTON_PTZ6:
		command = PTZ_CMD_RIGHT;
		break;
	case IDC_BUTTON_PTZ7:
		command = PTZ_CMD_LEFT_UP;
		break;
	case IDC_BUTTON_PTZ8:
		command = PTZ_CMD_UP;
		break;
	case IDC_BUTTON_PTZ9:
		command = PTZ_CMD_RIGHT_UP;
		break;
	default:
		break;
	}
	BOOL ret = NET_SDK_PTZControl(m_playHandle[m_currentDivide], command, m_PTZSpeedCtrl.GetPos());
	TRACE("ptz ctrl ret: %d\n", ret);
}

void CLiveDlg::OnPTZCtrlStop( int id )
{
	BOOL ret = NET_SDK_PTZControl(m_playHandle[m_currentDivide], PTZ_CMD_STOP, m_PTZSpeedCtrl.GetPos());
	TRACE("ptz ctrl stop ret: %d\n", ret);
}

void CLiveDlg::OnBnClickedButtonCapture()
{

	CString fname;
	CFileDialog dlg(FALSE, _T(".bmp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("bmp files(*.bmp)|*.bmp||"));

	if (dlg.DoModal() == IDOK)
	{
		fname = dlg.GetPathName();
		char *data = new char[2000 * 2000];
		DWORD size;
		BOOL ret = NET_SDK_CaptureJPEGPicture(m_userID, 0, NULL, data, 2000 *2000, &size);
		if (ret)
		{
			HANDLE h = CreateFile(fname.GetBuffer(), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
			if (h == INVALID_HANDLE_VALUE)
			{
				AfxMessageBox(_T("创建文件失败了！"));
				return ;
			}
			DWORD written;
			WriteFile(h, data, size, &written, NULL);
			CloseHandle(h);
		}
		delete[] data;
	}
	
}

void CLiveDlg::OnBnClickedButtonLivesave()
{
	if (m_isSavingData[m_currentDivide])
	{
		BOOL ret = NET_SDK_StopSaveLiveData(m_playHandle[m_currentDivide]);
		if (ret)
		{
			m_isSavingData[m_currentDivide] = FALSE;
		}
	}
	else
	{
		CFileDialog dlg(FALSE, _T(".avi"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("avi files(*.avi)|*.avi||"));
		if (IDOK == dlg.DoModal())
		{
			
			BOOL ret = NET_SDK_SaveLiveData(m_playHandle[m_currentDivide], dlg.GetPathName().GetBuffer());
			if (ret)
			{
				m_isSavingData[m_currentDivide] = TRUE;
			}
			else
			{
				AfxMessageBox(_T("ERROR!"));
			}
		}
	}
	UpdateUI();
}

#pragma pack(4)
typedef struct _sdk_frame_info
{
	unsigned long		nLength;	//帧数据长度
	unsigned short		frameType;	//参考DD_FRAME_TYPE
	unsigned short		keyFrame;	//0：非关键帧 1：关键帧
	unsigned short		nWidth;		//画面宽, 单位像素, 如果是音频数据则为0
	unsigned short		nHeight;	//画面高, 如果是音频数据则为0
	LONGLONG			nStamp;		//帧时间, 单位毫秒, 从1970年1月1日0点0分0秒开始的绝对时间
}SDK_FRAME_INFO;
#pragma pack()

void CLiveDlg::LiveCallBack( LONG lLiveHandle, NET_SDK_FRAME_INFO frameInfo, BYTE *pBuffer, void *pUser )
{
	CLiveDlg *pThis = reinterpret_cast<CLiveDlg *>(pUser);
	
	DWORD written = 0;

	int curDivide = -1;
	for(int i = 0; i < MAX_DIVIDE_NUM; i++)
	{
		if(pThis->m_playHandle[i] == lLiveHandle)
		{
			curDivide = i;
		}
	}

	if(curDivide < 0)
	{
		ASSERT(FALSE);
		return;
	}

	if (frameInfo.frameType == DD_FRAME_TYPE_VIDEO_FORMAT)
	{
		SDK_FRAME_INFO info;
		info.keyFrame = frameInfo.keyFrame;
		info.nLength = frameInfo.length;
		info.nHeight = frameInfo.height;
		info.nWidth = frameInfo.width;
		info.nStamp = frameInfo.time;
		info.frameType = frameInfo.frameType;
#ifdef _WRITE_USER_FILE_
		WriteFile(pThis->m_RecFilehandle[curDivide], &info, sizeof(SDK_FRAME_INFO), &written, NULL);
		WriteFile(pThis->m_RecFilehandle[curDivide], pBuffer, frameInfo.length, &written, NULL);
		pThis->m_recNeedFormat[curDivide] = false;
#endif
	}
	else if (!pThis->m_recNeedFormat[curDivide] && frameInfo.frameType == DD_FRAME_TYPE_VIDEO)
	{
		SDK_FRAME_INFO info;
		info.keyFrame = frameInfo.keyFrame;
		info.nLength = frameInfo.length;
		info.nHeight = frameInfo.height;
		info.nWidth = frameInfo.width;
		info.nStamp = frameInfo.time;
		info.frameType = frameInfo.frameType;
#ifdef _WRITE_USER_FILE_
		WriteFile(pThis->m_RecFilehandle[curDivide], &info, sizeof(SDK_FRAME_INFO), &written, NULL);
		WriteFile(pThis->m_RecFilehandle[curDivide], pBuffer, frameInfo.length, &written, NULL);
#endif
	}
	else if((frameInfo.frameType == DD_FRAME_TYPE_AUDIO_FORMAT) && (0 == curDivide))
	{
		pThis->m_lAudioDecHandle[curDivide] = NET_SDK_InitAudioDecoder(pBuffer, frameInfo.length);
	}
	else if((frameInfo.frameType == DD_FRAME_TYPE_AUDIO) && (0 == curDivide))
	{
		unsigned char *pOutBuffer = new unsigned char [3200];
		LONG outLen;
		NET_SDK_DecodeAudioFrame(pThis->m_lAudioDecHandle[curDivide], pBuffer, frameInfo.length, pOutBuffer, &outLen);
		pThis->m_waveOut.WriteBuf(pOutBuffer, outLen);
		
		if (pOutBuffer)
		{
			delete[] pOutBuffer;
			pOutBuffer = NULL;
		}
	}

}

void CALLBACK CLiveDlg::drawCallBack( LONG lLiveHandle, HDC hDC, void *pUser )
{
	CLiveDlg *pThis = reinterpret_cast<CLiveDlg *>(pUser);

	if (lLiveHandle == pThis->m_playHandle[0])
	{
		TextOut(hDC, 10, 10, _T("OH YEAH! OH NO! OH MY GOD!"), _tcslen(_T("OH YEAH! OH NO! OH MY GOD!")));
	}
}
