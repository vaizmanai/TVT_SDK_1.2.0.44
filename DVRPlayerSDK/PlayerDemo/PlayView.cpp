// PlayView.cpp : 实现文件
//

#include "stdafx.h"
#include "PlayerDemo.h"
#include "PlayView.h"
#include "SoundDlg.h"
#include "MainFrm.h"
#include "DVRPlayerSDK.h"
#include "LangStr.h"
#include <assert.h>
#include "SearchDialog.h"
// CPlayView

IMPLEMENT_DYNCREATE(CPlayView, CFormView)

CPlayView::CPlayView()
	: CFormView(CPlayView::IDD)
	, m_curDivide(0)
{

	memset(m_isPlay, 0x0, sizeof(m_isPlay));
	memset(m_isPause, 0x0, sizeof(m_isPause));
	memset(m_isEnableSound, 0x0, sizeof(m_isEnableSound));
//	memset(m_isOpenedStream, 0x0, sizeof(m_isOpenedStream));
	memset(m_volume, 0x0, sizeof(m_volume));
	
	for (int i = 0; i< 16;i++)
	{
		m_getAudioFormat[i] = false;
		m_getVedioFormat[i] = false;
		m_pBufferList[i] = NULL;
		m_playChnn[i] = -1;
		m_liveHandle[i] = -1;
		m_playPort[i] = i+1;
		m_ChnnPlayType[i] = 0;
	}
	
	m_playerType = PLAY_STREAM;

	m_areaList.push_back(IDC_PLAY_AREA);
	m_areaList.push_back(IDC_PLAY_AREA2);
	m_areaList.push_back(IDC_PLAY_AREA3);
	m_areaList.push_back(IDC_PLAY_AREA4);
	m_areaList.push_back(IDC_PLAY_AREA5);
	m_areaList.push_back(IDC_PLAY_AREA6);
	m_areaList.push_back(IDC_PLAY_AREA7);
	m_areaList.push_back(IDC_PLAY_AREA8);
	m_areaList.push_back(IDC_PLAY_AREA9);
	m_areaList.push_back(IDC_PLAY_AREA10);
	m_areaList.push_back(IDC_PLAY_AREA11);
	m_areaList.push_back(IDC_PLAY_AREA12);
	m_areaList.push_back(IDC_PLAY_AREA13);
	m_areaList.push_back(IDC_PLAY_AREA14);
	m_areaList.push_back(IDC_PLAY_AREA15);
	m_areaList.push_back(IDC_PLAY_AREA16);

	
}

CPlayView::~CPlayView()
{
	for (int i = 0;i<16;i++)
	{
		if (m_pBufferList[i])
		{
			delete [] m_pBufferList[i];
		}
	}

}

void CPlayView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_SLIDER_PLAYPOS, m_playpos);
}

BEGIN_MESSAGE_MAP(CPlayView, CFormView)
	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI_RANGE(ID_SNAP, ID_FRAME, &CPlayView::OnUpdateOperateUI)
	ON_COMMAND_RANGE(ID_SNAP, ID_FRAME, &CPlayView::OnOperateCommand)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CPlayView::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CPlayView::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_SLOW, &CPlayView::OnBnClickedButtonSlow)
	ON_BN_CLICKED(IDC_BUTTON_FAST, &CPlayView::OnBnClickedButtonFast)
	ON_BN_CLICKED(IDC_BUTTON_FRAME, &CPlayView::OnBnClickedButtonFrame)
	ON_BN_CLICKED(IDC_BUTTON_SNAP, &CPlayView::OnBnClickedButtonSnap)
	ON_BN_CLICKED(IDC_BUTTON_SOUND, &CPlayView::OnBnClickedButtonSound)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_PLAYPOS, &CPlayView::OnNMReleasedcaptureSliderPlaypos)
	ON_MESSAGE(WM_CHANGE_CHANNEL, &CPlayView::OnChangeChannel)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_PLAYTYPE, &CPlayView::OnBnClickedButtonPlaytype)
END_MESSAGE_MAP()


// CPlayView 诊断

#ifdef _DEBUG
void CPlayView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPlayView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPlayView 消息处理程序

void CPlayView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	SetScrollSizes(MM_TEXT,CSize(1, 1)); 
	SetDlgItemText(IDC_BUTTON_SNAP, LANG[STR_SNAP]);
	SetDlgItemText(IDC_BUTTON_SOUND, LANG[STR_AUDIO]);
	UpdateUI();
	
}

void CPlayView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	
	int height = (cy * PLAY_AREA_HEIGHT_PERCENT) / 100;

	if (cy - height > CONTROL_BAR_MAX_HEIGHT)
	{
		height = cy - CONTROL_BAR_MAX_HEIGHT;
	}
	
	ResetPlayArea();

	int playType = reinterpret_cast<CMainFrame *>(AfxGetMainWnd())->GetPlayType();

	if (::IsWindow(GetDlgItem(IDC_SLIDER_PLAYPOS)->GetSafeHwnd()))
	{
		GetDlgItem(IDC_SLIDER_PLAYPOS)->MoveWindow(0, height, cx, 20);
	}
	

	int button_hight = cy - height - 20;
	int button_width = 50;
	for (int i = IDC_BUTTON_PLAY; i <= IDC_BUTTON_PLAYTYPE; i++)
	{
		int index = i - IDC_BUTTON_PLAY;
		if (::IsWindow(GetDlgItem(i)->GetSafeHwnd()))
		{
			GetDlgItem(i)->MoveWindow(5 + 10 * index + button_width * index, height + 20, button_width
				,button_hight);
		}

	}
}

void CPlayView::UpdateUI()
{
	int playType = reinterpret_cast<CMainFrame *>(AfxGetMainWnd())->GetPlayType();
	if (playType == PLAY_FILE)
	{
		GetDlgItem(m_areaList[0])->ShowWindow(SW_SHOW);
		for (int i = 1; i < m_areaList.size(); i++)
		{
			GetDlgItem(m_areaList[i])->ShowWindow(SW_HIDE);
		}
	}
	else
	{
		for (int i = 0;i < m_areaList.size();i++)
		{
			GetDlgItem(m_areaList[i])->ShowWindow(SW_SHOW);
		}
			
	}
	
	if ((playType == PLAY_FILE) && m_isPlay[m_curDivide] && !m_isPause[m_curDivide])
	{
		SetDlgItemText(IDC_BUTTON_PLAY, _T("||"));
	}
	else
	{
		SetDlgItemText(IDC_BUTTON_PLAY, _T(">"));
		if (playType == PLAY_STREAM)
		{
			GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(!m_isPlay[m_curDivide]);
		}
	}
	
	GetDlgItem(IDC_BUTTON_SLOW)->EnableWindow((playType == PLAY_FILE) && m_isPlay[m_curDivide] && !m_isPause[m_curDivide]);
	GetDlgItem(IDC_BUTTON_FAST)->EnableWindow((playType == PLAY_FILE) && m_isPlay[m_curDivide] && !m_isPause[m_curDivide]);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(m_isPlay[m_curDivide]);
	GetDlgItem(IDC_BUTTON_FRAME)->EnableWindow((playType == PLAY_FILE) && m_isPlay[m_curDivide] && m_isPause[m_curDivide]);
	GetDlgItem(IDC_BUTTON_SNAP)->EnableWindow(m_isPlay[m_curDivide]);
	GetDlgItem(IDC_BUTTON_SOUND)->EnableWindow(m_isPlay[m_curDivide]);
	GetDlgItem(IDC_SLIDER_PLAYPOS)->EnableWindow(playType == PLAY_FILE);
}

void CPlayView::OnUpdateOperateUI( CCmdUI* pCmdUI )
{
	int playType = reinterpret_cast<CMainFrame *>(AfxGetMainWnd())->GetPlayType();
	if (playType == PLAY_NONE)
	{
		pCmdUI->Enable(FALSE);
		return ;
	}
	switch (pCmdUI->m_nID)
	{
	case ID_STOP:
		{
			m_isPlay[m_curDivide] = FALSE; //关闭时将播放标志改成FALSE（解决重复打开录像文件程序崩溃的问题） added by liuchen 20140504
		}
		break;
	case ID_SNAP:
		{
			pCmdUI->Enable(m_isPlay[m_curDivide]);
			break;
		}
	case ID_PLAY:
		{
			if ((playType == PLAY_FILE) && m_isPlay[m_curDivide] && !m_isPause[m_curDivide])
			{
				pCmdUI->SetText(LANG[STR_PAUSE]);
			}
			else
			{
				pCmdUI->SetText(LANG[STR_PLAY]);
				if (playType == PLAY_STREAM)
				{
					pCmdUI->Enable(!m_isPlay[m_curDivide]);
				}
			}
			break;
		}
	case ID_FAST:
		{
			pCmdUI->Enable((playType == PLAY_FILE) && m_isPlay[m_curDivide] && !m_isPause[m_curDivide]);
			break;
		}
	case ID_SLOW:
		{
			pCmdUI->Enable((playType == PLAY_FILE) && m_isPlay[m_curDivide] && !m_isPause[m_curDivide]);
			break;;
		}
	case ID_FRAME:
		{
			pCmdUI->Enable((playType == PLAY_FILE) && m_isPlay[m_curDivide] && m_isPause[m_curDivide]);
			break;
		}
	default:break;
	}
}



void CPlayView::OnOperateCommand( UINT ID )
{
	switch(ID)
	{
	case ID_SNAP:
		{
			OnBnClickedButtonSnap();
			break;
		}
	case ID_PLAY:
		{
			OnBnClickedButtonPlay();
			break;
		}
	case ID_STOP:
		{
			OnBnClickedButtonStop();
			break;
		}
	case ID_SLOW:
		{
			OnBnClickedButtonSlow();
			break;
		}
	case ID_FAST:
		{
			OnBnClickedButtonFast();
			break;
		}
	case ID_FRAME:
		{
			OnBnClickedButtonFrame();
			break;
		}
	}
}


void CPlayView::OnBnClickedButtonPlay()
{
	if (!m_isPlay[m_curDivide])
	{
		int playType = reinterpret_cast<CMainFrame *>(AfxGetMainWnd())->GetPlayType();
		m_isPause[m_curDivide] = FALSE;

		if (playType == PLAY_STREAM)
		{
			OpenLive();
		}
		else if (playType == PLAY_FILE)
		{
			TRACE("open file!\n");
			m_isPlay[m_curDivide] = TRUE;

			//PLAYER_SetFileRefCallBack(m_playPort, &CPlayView::IndexEndCallBack, this);

			BOOL ret = PLAYER_Play(m_playPort[m_curDivide], GetDlgItem(IDC_PLAY_AREA)->GetSafeHwnd());
			PLAYER_PlaySound(m_playPort[m_curDivide]);
			m_isEnableSound[m_curDivide] = TRUE;
			PLAYER_SetDrawCallBack(m_playPort[m_curDivide],DrawCallBack,NULL);		
			//PLAYER_SetFileEndCallback(m_playPort[m_curDivide], &CPlayView::FileEndCallBack, this);
			ret = PLAYER_SetFileEndMsg(m_playPort[m_curDivide],this->GetSafeHwnd(),10000);
			TRACE("PLAYER_PLAY : %d\n", ret);
			SetTimer(1010, 500, NULL);
		}
	}
	else
	{
		m_isPause[m_curDivide] = !m_isPause[m_curDivide];
		BOOL ret = PLAYER_Pause(m_playPort[m_curDivide], m_isPause[m_curDivide]);
		TRACE("PLAYER_Pause : %d\n", ret);
	}
	UpdateUI();
	
}


void CPlayView::OnBnClickedButtonStop()
{
	int playType = reinterpret_cast<CMainFrame *>(AfxGetMainWnd())->GetPlayType();
	if(playType == PLAY_FILE)
	{
		KillTimer(1010);
		KillTimer(2020);
	}
	else
	{
		m_playLock.Lock();
		CloseLive();	
		m_playLock.Unlock();
	}
	
	UpdateUI();
}

void CPlayView::OnBnClickedButtonSlow()
{
	BOOL ret = PLAYER_BackFast(m_playPort[m_curDivide],PLAY_SDK_RPB_SPEED_32X);
	TRACE("PLAYER_Slow : %d\n", ret);
}

void CPlayView::OnBnClickedButtonFast()
{
	BOOL ret = PLAYER_Fast(m_playPort[m_curDivide],PLAY_SDK_RPB_SPEED_4X);
	TRACE("PLAYER_Fast : %d\n", ret);
}

void CPlayView::OnBnClickedButtonFrame()
{
	BOOL ret = PLAYER_OneByOne(m_playPort[m_curDivide]);
	TRACE("PLAYER_OneByOne : %d\n", ret);
}

void CPlayView::OnBnClickedButtonSnap()
{
	CFileDialog dlg(FALSE, _T("bmp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		_T("bmp files(*.bmp)|*.bmp||"));
	
	long width, height;
	PLAYER_GetPictureSize(m_playPort[m_curDivide], &width, &height);

	if (IDOK == dlg.DoModal())
	{
		BYTE *buff = new BYTE[2000 * 2000];
		ZeroMemory(buff, 2000 * 2000);

		DWORD size = 0;
		BOOL ret = PLAYER_GetBMP(m_playPort[m_curDivide], buff, 2000 * 2000, &size);
		if (ret)
		{
			HANDLE of = CreateFile(dlg.GetPathName().GetBuffer(), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
			if (of == INVALID_HANDLE_VALUE)
			{
				AfxMessageBox(LANG[STR_CREATE_FILE_FAILED]);
			}
			else
			{
				DWORD writen;
				WriteFile(of, buff, size, &writen, NULL);
				CloseHandle(of);
			}
		}
		else
		{
			AfxMessageBox(LANG[STR_FAILED]);
		}
		delete[] buff;
	}
}

void CPlayView::OnBnClickedButtonSound()
{
	/*CSoundDlg dlg;
	CPoint point;
	GetCursorPos(&point);
	dlg.SetPos(point.x, point.y);
	dlg.m_volume = m_volume[m_curDivide];
	dlg.m_mute = !m_isEnableSound;
	dlg.DoModal();
	AfxGetMainWnd()->DrawMenuBar();
    UpdateUI();
	m_volume[m_curDivide] = dlg.m_volume;
	TRACE("%d\n",dlg.m_volume);
	PLAYER_SetVolume(m_playPort[m_curDivide], m_volume[m_curDivide]);

	if (m_isEnableSound[m_curDivide] && dlg.m_mute)
	{
		PLAYER_StopSound();
		m_isEnableSound[m_curDivide] = FALSE;
	}
	else if(!m_isEnableSound[m_curDivide] && !dlg.m_mute)
	{
		PLAYER_SetVolume(m_playPort[m_curDivide], m_volume[m_curDivide]);
		PLAYER_PlaySound(m_playPort[m_curDivide]);
		m_isEnableSound[m_curDivide] = TRUE;
	}*/
	

	if (m_isEnableSound[m_curDivide])
	{
		PLAYER_StopSound();
		m_isEnableSound[m_curDivide] = FALSE;
	}
	else if(!m_isEnableSound[m_curDivide])
	{
		m_volume[m_curDivide] = 60000;
		PLAYER_SetVolume(m_playPort[m_curDivide], m_volume[m_curDivide]);
		PLAYER_PlaySound(m_playPort[m_curDivide]);
		m_isEnableSound[m_curDivide] = TRUE;
	}
}
void CPlayView::OnNMReleasedcaptureSliderPlaypos(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData();

	KillTimer(1010);
	int ret = PLAYER_SetPlayPos(m_playPort[m_curDivide], (float)m_playpos / 100);
	SetTimer(2020, 500, NULL);

	*pResult = 0;
}

void CPlayView::OnTimer(UINT_PTR nIDEvent)
{
	CPoint point;
	GetCursorPos(&point);
	CRect rect;
	GetDlgItem(IDC_SLIDER_PLAYPOS)->GetWindowRect(&rect);

	if (nIDEvent == 1010 && !rect.PtInRect(point))
	{
		float pos = PLAYER_GetPlayPos(m_playPort[m_curDivide]);
		if (pos - 0.0 >= 0.00000001)
		{
			m_playpos = pos * 100;
			if (m_playpos >= 100)
			{
				OnBnClickedButtonStop();
			}
			UpdateData(FALSE);
		}
		
	}
	else if (nIDEvent == 2020)
	{
		float pos = PLAYER_GetPlayPos(m_playPort[m_curDivide]);
		int temp = pos * 100;
		if(abs(temp - m_playpos) <= 2)
		{
			KillTimer(2020);
			SetTimer(1010, 500, NULL);
		}
	}


	CFormView::OnTimer(nIDEvent);
}

void CALLBACK CPlayView::DrawCallBack(long nPort, HDC hDC, long lWidth, long lHeight, void * pwUser)
{
	MoveToEx(hDC,50,50,NULL);
	LineTo(hDC,200,50);
	LineTo(hDC,200,200);
	LineTo(hDC,50,200);
	LineTo(hDC,50,50);
}

void CALLBACK CPlayView::LiveCallBack( LONG lLiveHandle, NET_SDK_FRAME_INFO frameInfo, BYTE *pBuffer, void *pUser )
{	
	
	
	CPlayView *pThis = reinterpret_cast<CPlayView *>(pUser);
	CWnd* PlayAreaArray[16] = {pThis->GetDlgItem(IDC_PLAY_AREA),
		pThis->GetDlgItem(IDC_PLAY_AREA2),
		pThis->GetDlgItem(IDC_PLAY_AREA3), 
		pThis->GetDlgItem(IDC_PLAY_AREA4),
		pThis->GetDlgItem(IDC_PLAY_AREA5),
		pThis->GetDlgItem(IDC_PLAY_AREA6),
		pThis->GetDlgItem(IDC_PLAY_AREA7),
		pThis->GetDlgItem(IDC_PLAY_AREA8),
		pThis->GetDlgItem(IDC_PLAY_AREA9),
		pThis->GetDlgItem(IDC_PLAY_AREA10),
		pThis->GetDlgItem(IDC_PLAY_AREA11),
		pThis->GetDlgItem(IDC_PLAY_AREA12),
		pThis->GetDlgItem(IDC_PLAY_AREA13),
		pThis->GetDlgItem(IDC_PLAY_AREA14),
		pThis->GetDlgItem(IDC_PLAY_AREA15),
		pThis->GetDlgItem(IDC_PLAY_AREA16)};

	/*CWnd *PlayArea = NULL;
	int curDivide = 0;
	for (int i = 0; i < 16; i++)
	{
		if (frameInfo.channel == pThis->m_playChnn[i])
		{
			PlayArea = PlayAreaArray[i];
			curDivide = i;
			break;
		}
	}*/

	pThis->m_streamWriter.Init("D:/hello.rec");
	pThis->m_streamWriter.InputData(lLiveHandle,&frameInfo,pBuffer);
	

	CWnd *PlayArea = NULL;
	int curDivide = -1;
	for (int i = 0; i < 16; i++)
	{
		if (lLiveHandle == pThis->m_liveHandle[i])
		{
			PlayArea = PlayAreaArray[i];
			curDivide = i;
			break;
		}
	}
	
	if (curDivide < 0)
	{
		return ;
	}

	switch(frameInfo.frameType)
	{

	case DD_FRAME_TYPE_VIDEO_FORMAT:
		{
			unsigned int a = sizeof(SDK_FRAME_INFO);
			unsigned int b = sizeof(BITMAPINFOHEADER);
			pThis->m_playLock.Lock();
			SDK_FRAME_INFO frameinfo;
			frameinfo.frameType = frameInfo.frameType;
			frameinfo.keyFrame = frameInfo.keyFrame;
			frameinfo.nHeight = frameInfo.height;
			frameinfo.nLength = frameInfo.length;
			frameinfo.nStamp = frameInfo.time;
			frameinfo.nWidth = frameInfo.width;
			if (!pThis->m_isPlay[curDivide])
			{
				
				BOOL ret =PLAYER_OpenStream(pThis->m_playPort[curDivide], pBuffer, frameInfo.length, DECODE_POOL_SIZE);
				

				assert(ret==TRUE);
				//PLAYER_SetFrameType(pThis->m_playPort[curDivide], FRAME_TYPE_RGB24);

				ret = PLAYER_Play(pThis->m_playPort[curDivide], PlayArea->GetSafeHwnd());
				PLAYER_SetDrawCallBack(pThis->m_playPort[curDivide],DrawCallBack,NULL);
				assert(ret == TRUE);
				pThis->m_isPlay[curDivide] = TRUE;
			
			}
			pThis->m_playLock.Unlock();
			pThis->m_getVedioFormat[curDivide] = true;
			BOOL bRet = FALSE;
			if (pThis->m_getAudioFormat[curDivide])
			{	
				while(!bRet)
				{
					bRet = PLAYER_InputData(pThis->m_playPort[curDivide], &(pThis->m_frameinfo[curDivide]), pThis->m_pBufferList[curDivide], pThis->m_frameinfo[curDivide].nLength);
					if (!bRet)
					{
						Sleep(1);
					}
				}
			}
			break;
		}
	case DD_FRAME_TYPE_AUDIO_FORMAT:
	case DD_FRAME_TYPE_AUDIO:
	case DD_FRAME_TYPE_VIDEO:
		{			
			SDK_FRAME_INFO frameinfo;
			frameinfo.frameType = frameInfo.frameType;
			frameinfo.keyFrame = frameInfo.keyFrame;
			frameinfo.nHeight = frameInfo.height;
			frameinfo.nLength = frameInfo.length;
			frameinfo.nStamp = frameInfo.time;
			frameinfo.nWidth = frameInfo.width;
			BOOL bRet = FALSE;

			if (frameInfo.frameType == DD_FRAME_TYPE_AUDIO_FORMAT)
			{
				if (!pThis->m_getVedioFormat[curDivide])
				{
					pThis->m_frameinfo[curDivide] = frameinfo;
					pThis->m_pBufferList[curDivide] = new BYTE[frameinfo.nLength];
					assert(pThis->m_pBufferList[curDivide]);
					memcpy(pThis->m_pBufferList[curDivide],pBuffer,frameinfo.nLength);

					pThis->m_getAudioFormat[curDivide] = true;
					break;
				}
			}



			if (pThis->m_isPlay[curDivide])
			{
				while(!bRet)
				{
					bRet = PLAYER_InputData(pThis->m_playPort[curDivide], &frameinfo, pBuffer, frameinfo.nLength);
					if (!bRet)
					{
						Sleep(1);
					}

					if (!pThis->m_isPlay[curDivide])
					{
						break;
					}
				}
			}

			break;
		}

	default:break;
	}
}

LRESULT CPlayView::OnChangeChannel( WPARAM wparam, LPARAM lparam)
{
	/*int divide = static_cast<int>(wparam);
	int chnn = static_cast<int>(lparam);
	
	for (int i=0; i<16; i++)
	{
		if (m_liveHandle[i]>0 && m_playChnn[i] == chnn)
		{
			return 0;
		}
	}
	if (divide >= 0)
	{
		m_curDivide = divide;
	}
	else
	{
		divide = m_curDivide;
	}

	int playType = reinterpret_cast<CMainFrame *>(AfxGetMainWnd())->GetPlayType();
	LONG userID = reinterpret_cast<CMainFrame *>(AfxGetMainWnd())->GetUserID();
	if (playType == PLAY_STREAM)
	{	
		if (m_playerType ==  PLAY_STREAM)
		{
			
			if (m_liveHandle[divide] > 0 && m_playChnn[divide] != chnn)
			{
				
				m_playChnn[divide] = chnn;	
				NET_SDK_StopLivePlay(m_liveHandle[divide]);
				m_liveHandle[divide] = -1;

				NET_SDK_CLIENTINFO clientInfo;
				ZeroMemory(&clientInfo, sizeof(NET_SDK_CLIENTINFO));
				clientInfo.lChannel = chnn;
				clientInfo.streamType = NET_SDK_SUB_STREAM;
				clientInfo.streamType = NET_SDK_MAIN_STREAM;
				LONG handle = NET_SDK_LivePlay(userID, &clientInfo, &CPlayView::LiveCallBack, (LPVOID)this);
				if (handle > 0)
				{
					m_liveHandle[divide] = handle;
				}
			}
			else
			{
				m_playChnn[divide] = chnn;
				OpenLive();
			}

			UpdateUI();
			if (chnn >= 0)
			{
				CString temp;
				temp.Format(_T("PlayerDemo:Channel %d"), chnn + 1);
				AfxGetMainWnd()->SetWindowText(temp);
				m_curDivide++;
				CDC *dc = GetDC();
				DrawGrid(dc);
				ReleaseDC(dc);
			}
		}
		else
		{
			CSearchDialog searchDialog;
			searchDialog.SetChannl(chnn);
			searchDialog.SetUserId(userID);
			searchDialog.DoModal();
			NET_SDK_REC_FILE recfile;
			if (searchDialog.GetRecFile(recfile) >= 0)
			{
				LONG ret;
				LONG tmp = chnn;
				TRACE("Start:%d-%d-%d %d:%d:%d w:%d\n",recfile.startTime.year,recfile.startTime.month,recfile.startTime.mday,recfile.startTime.hour,recfile.startTime.minute,recfile.startTime.second,recfile.startTime.wday);
				TRACE("Start:%d-%d-%d %d:%d:%d w:%d\n",recfile.stopTime.year,recfile.stopTime.month,recfile.stopTime.mday,recfile.stopTime.hour,recfile.stopTime.minute,recfile.stopTime.second,recfile.stopTime.wday);
				ret = NET_SDK_PlayBackByTime(userID,&tmp,1,&recfile.startTime,&recfile.stopTime,NULL);	
				
				if (ret != -1)
				{
					if (NET_SDK_SetPlayDataCallBack(ret, &CPlayView::LiveCallBack, (LPVOID)this))
					{
						m_playChnn[m_curDivide] = chnn;
						m_ChnnPlayType[m_curDivide] = PLAY_BACK;
						m_liveHandle[m_curDivide] = ret;
					}
					
				}
				
			}

		}
		
    }
	else
	{
		AfxGetMainWnd()->SetWindowText(_T("PlayerDemo"));
	}*/
	

	int chnn = static_cast<int>(lparam);
	int divide = m_curDivide;

	if ((m_liveHandle[divide]>0) && (m_playChnn[divide]==chnn))
	{
		return 0;
	}
	

	int playType = reinterpret_cast<CMainFrame *>(AfxGetMainWnd())->GetPlayType();
	LONG userID = reinterpret_cast<CMainFrame *>(AfxGetMainWnd())->GetUserID();
	if (playType == PLAY_STREAM)
	{	
		if (m_playerType ==  PLAY_STREAM)
		{

			if (m_liveHandle[divide] > 0 && m_playChnn[divide] != chnn)
			{

				m_playChnn[divide] = chnn;	
				NET_SDK_StopLivePlay(m_liveHandle[divide]);
				m_liveHandle[divide] = -1;

				NET_SDK_CLIENTINFO clientInfo;
				ZeroMemory(&clientInfo, sizeof(NET_SDK_CLIENTINFO));
				clientInfo.lChannel = chnn;
				clientInfo.streamType = NET_SDK_SUB_STREAM;
				LONG handle = NET_SDK_LivePlay(userID, &clientInfo, &CPlayView::LiveCallBack, (LPVOID)this);
				if (handle > 0)
				{
					m_liveHandle[divide] = handle;
				}
			}
			else
			{
				m_playChnn[divide] = chnn;
				OpenLive();
			}

			UpdateUI();
			if (chnn >= 0)
			{
				CString temp;
				temp.Format(_T("PlayerDemo:Channel %d"), chnn + 1);
				AfxGetMainWnd()->SetWindowText(temp);
				m_curDivide++;
				CDC *dc = GetDC();
				DrawGrid(dc);
				ReleaseDC(dc);
				
			}
		}
		else
		{
			CSearchDialog searchDialog;
			searchDialog.SetChannl(chnn);
			searchDialog.SetUserId(userID);
			searchDialog.DoModal();
			NET_SDK_REC_FILE recfile;
			if (searchDialog.GetRecFile(recfile) >= 0)
			{
				LONG ret;
				LONG tmp = chnn;
				recfile.stopTime.hour = 23;
				recfile.stopTime.minute  = 59;
				recfile.stopTime.second = 59;
				ret = NET_SDK_PlayBackByTime(userID,&tmp,1,&recfile.startTime,&recfile.stopTime,NULL);	

				if (ret != -1)
				{
					if (NET_SDK_SetPlayDataCallBack(ret, &CPlayView::LiveCallBack, (LPVOID)this))
					{
						m_playChnn[m_curDivide] = chnn;
						m_ChnnPlayType[m_curDivide] = PLAY_BACK;
						m_liveHandle[m_curDivide] = ret;
					}

				}

			}

		}

	}
	else
	{
		AfxGetMainWnd()->SetWindowText(_T("PlayerDemo"));
	}
	return 0;
}

BOOL CPlayView::OpenLive()
{
	LONG userID = reinterpret_cast<CMainFrame *>(AfxGetMainWnd())->GetUserID();

	NET_SDK_CLIENTINFO clientInfo;
	ZeroMemory(&clientInfo, sizeof(NET_SDK_CLIENTINFO));
	clientInfo.lChannel = m_playChnn[m_curDivide];
	clientInfo.streamType = NET_SDK_MAIN_STREAM;
	LONG handle = 0;
	
	//DD_TIME start, end;
	//start.year = 2011 - 1900;
	//start.mday = 17;
	//start.month = 10;
	//start.hour = 10;
	//start.minute = 0;
	//start.second = 0;
	//
	//end.year = 2011 - 1900;
	//end.mday = 17;
	//end.month = 10;
	//end.hour = 11;
	//end.minute = 0;
	//end.second = 0;
    
	
	handle = NET_SDK_LivePlay(userID, &clientInfo, &CPlayView::LiveCallBack, (LPVOID)this);

	//handle = NET_SDK_PlayBackByTime(userID, &clientInfo.lChannel, 1, &start, &end, NULL);
	if (handle > 0)
	{
		//NET_SDK_SetPlayDataCallBack(handle, &CPlayView::LiveCallBack, this);
		//m_isPlay[m_curDivide] = TRUE;
		m_liveHandle[m_curDivide] = handle;
		m_ChnnPlayType[m_curDivide] = PLAY_STREAM;
		return TRUE;
	}
	else
	{  
		clientInfo.streamType = NET_SDK_SUB_STREAM;
		handle = NET_SDK_LivePlay(userID, &clientInfo, &CPlayView::LiveCallBack, (LPVOID)this);
		if (handle > 0)
		{
			m_liveHandle[m_curDivide] = handle;
			m_ChnnPlayType[m_curDivide] = PLAY_STREAM;		
			return TRUE;
		}
		return FALSE;
	}
}


BOOL CPlayView::OpenPlayBack()
{
	return TRUE;
}


void CPlayView::CloseLive()
{
	
	if (m_isPlay[m_curDivide])
	{
		m_getVedioFormat[m_curDivide] = false;
		m_getAudioFormat[m_curDivide] = false;
		
		if (m_isEnableSound[m_curDivide])
		{
			PLAYER_StopSound();
			m_isEnableSound[m_curDivide] = FALSE;
		}

		if (m_pBufferList[m_curDivide] != NULL)
		{
			delete []  m_pBufferList[m_curDivide];
			m_pBufferList[m_curDivide] = NULL;
		}
		if (m_ChnnPlayType[m_curDivide] == PLAY_STREAM)
		{
			NET_SDK_StopLivePlay(m_liveHandle[m_curDivide]);
			PLAYER_Stop(m_playPort[m_curDivide]);
			PLAYER_CloseStream(m_playPort[m_curDivide]);
		}
		else if(m_ChnnPlayType[m_curDivide] == PLAY_BACK)
		{
			NET_SDK_PlayBackControl(m_liveHandle[m_curDivide], NET_SDK_PLAYCTRL_STOPAUDIO, 0, NULL);
			NET_SDK_StopPlayBack(m_liveHandle[m_curDivide]);
			PLAYER_Stop(m_playPort[m_curDivide]);
			PLAYER_CloseStream(m_playPort[m_curDivide]);
		}
		
	}
	m_isPlay[m_curDivide] = FALSE;
	m_liveHandle[m_curDivide] = -1;
}


void CPlayView::DrawGrid(CDC *pDC)
{
	int playType = reinterpret_cast<CMainFrame *>(AfxGetMainWnd())->GetPlayType();
	if (playType != PLAY_STREAM)
	{
		return;
	}
	CRect rect;
	CBrush brush(RGB(86, 0, 0));
	CBrush rBrush(RGB(86, 150, 0));
  
    for (int i = 0; i < m_areaList.size();i++)
    {
		GetDlgItem(m_areaList[i])->GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.top--;rect.left--;
		rect.right++;rect.bottom++;
		if (m_curDivide == i)
		{
			pDC->FrameRect(&rect, &rBrush);
		}
		else
		{
			pDC->FrameRect(&rect, &brush);
		}
    }

}

void CPlayView::OnDraw(CDC* pDC)
{
	int playType = reinterpret_cast<CMainFrame *>(AfxGetMainWnd())->GetPlayType();
	
	DrawGrid(pDC);

	/*if (playType == PLAY_FILE)
	{
		PLAYER_RefreshPlay(m_playPort[m_curDivide]);
	}*/
}

void CPlayView::IndexEndCallBack( long port, void *params )
{
	CPlayView *pThis = reinterpret_cast<CPlayView *>(params);

	AfxMessageBox(_T("Index Finished!"));
}

void CALLBACK CPlayView::FileEndCallBack( long port, void *params )
{
	AfxMessageBox(_T("File End!"));
}

void CPlayView::ResetPlayArea()
{
	int playType = reinterpret_cast<CMainFrame *>(AfxGetMainWnd())->GetPlayType();

	CRect rect;
	GetWindowRect(&rect);
	int cx = rect.Width();
	int cy = rect.Height();

	int height = (cy * PLAY_AREA_HEIGHT_PERCENT) / 100;

	if (cy - height > CONTROL_BAR_MAX_HEIGHT)
	{
		height = cy - CONTROL_BAR_MAX_HEIGHT;
	}
	
	int heightPerArea = height / 4;
	int widthPerArea = cx / 4;
	if (playType == PLAY_FILE )
	{
		if (::IsWindow(GetDlgItem(IDC_PLAY_AREA)->GetSafeHwnd()))
		{
			GetDlgItem(IDC_PLAY_AREA)->MoveWindow(0, 0, cx, height);
		}
	}
	else if(playType == PLAY_STREAM|| playType == PLAY_NONE)
	{
		for (int i = 0;i < 4;i++)
		{
			for (int j = 0;j<4;j++)
			{
				int index = m_areaList[i*4+j]; 
				if(::IsWindow(GetDlgItem(index)->GetSafeHwnd()))
				{
					GetDlgItem(index)->MoveWindow(widthPerArea*j+2,heightPerArea*i+2,widthPerArea - 2, heightPerArea - 2);
				}
			}
		}
	}
}



void CPlayView::OnClickedPlayArea(int index)
{
   assert(index>=0&&index<16);
   m_curDivide = index;
   CDC *dc = GetDC();
   DrawGrid(dc);
   ReleaseDC(dc);
   UpdateUI();
}


BOOL CPlayView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	UINT nID = LOWORD(wParam);
	HWND hWndCtrl = (HWND)lParam;
	int nCode = HIWORD(wParam);
	int index = -1;
    if (nCode == BN_CLICKED)
    {
		for (int i = 0;i<m_areaList.size();i++)
		{
			if (nID == m_areaList[i])
			{
				index = i;
				break;
			}
		}
    }
    if (index >= 0)
    {

		OnClickedPlayArea(index);
    }

	return CFormView::OnCommand(wParam, lParam);
}

void CPlayView::OnBnClickedButtonPlaytype()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_playerType == PLAY_STREAM)
	{
		GetDlgItem(IDC_BUTTON_PLAYTYPE)->SetWindowText(_T("回放"));
		m_playerType = PLAY_BACK;
	} 
	else
	{
		GetDlgItem(IDC_BUTTON_PLAYTYPE)->SetWindowText(_T("现场"));
		m_playerType = PLAY_STREAM;
	}

}

BOOL CPlayView::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
    
	for (int i = 0;i < 16; i++)
	{
		m_curDivide = i;
		CloseLive();
	}
	return CFormView::DestroyWindow();
}



/***测试代码***/
void CPlayView::TestSetIndex(int index)
{
	assert(index>=0&&index<16);
	m_curDivide = index;
	CDC *dc = GetDC();
	DrawGrid(dc);
	ReleaseDC(dc);
	UpdateUI();
}


void CPlayView::TestOpenLive(int index,int channl)
{
	TestSetIndex(index);
	int chnn = channl;
	int divide = m_curDivide;

	if ((m_liveHandle[divide]>0) && (m_playChnn[divide]==chnn))
	{
		return ;
	}


	int playType = reinterpret_cast<CMainFrame *>(AfxGetMainWnd())->GetPlayType();
	LONG userID = reinterpret_cast<CMainFrame *>(AfxGetMainWnd())->GetUserID();
	if (playType == PLAY_STREAM)
	{	
		if (m_playerType ==  PLAY_STREAM)
		{

			if (m_liveHandle[divide] > 0 && m_playChnn[divide] != chnn)
			{

				m_playChnn[divide] = chnn;	
				NET_SDK_StopLivePlay(m_liveHandle[divide]);
				m_liveHandle[divide] = -1;

				NET_SDK_CLIENTINFO clientInfo;
				ZeroMemory(&clientInfo, sizeof(NET_SDK_CLIENTINFO));
				clientInfo.lChannel = chnn;
				clientInfo.streamType = NET_SDK_SUB_STREAM;
				LONG handle = NET_SDK_LivePlay(userID, &clientInfo, &CPlayView::LiveCallBack, (LPVOID)this);
				if (handle > 0)
				{
					m_liveHandle[divide] = handle;
				}
			}
			else
			{
				m_playChnn[divide] = chnn;
				OpenLive();
			}

			UpdateUI();
			if (chnn >= 0)
			{
				CString temp;
				temp.Format(_T("PlayerDemo:Channel %d"), chnn + 1);
				AfxGetMainWnd()->SetWindowText(temp);
				m_curDivide++;
				CDC *dc = GetDC();
				DrawGrid(dc);
				ReleaseDC(dc);
			}
		}
		else
		{
			CSearchDialog searchDialog;
			searchDialog.SetChannl(chnn);
			searchDialog.SetUserId(userID);
			searchDialog.DoModal();
			NET_SDK_REC_FILE recfile;
			if (searchDialog.GetRecFile(recfile) >= 0)
			{
				LONG ret;
				LONG tmp = chnn;
				ret = NET_SDK_PlayBackByTime(userID,&tmp,1,&recfile.startTime,&recfile.stopTime,NULL);	

				if (ret != -1)
				{
					if (NET_SDK_SetPlayDataCallBack(ret, &CPlayView::LiveCallBack, (LPVOID)this))
					{
						m_playChnn[m_curDivide] = chnn;
						m_ChnnPlayType[m_curDivide] = PLAY_BACK;
						m_liveHandle[m_curDivide] = ret;
					}

				}

			}

		}

	}
	else
	{
		AfxGetMainWnd()->SetWindowText(_T("PlayerDemo"));
	}
	return ;
}


void CPlayView::TestCloseLive(int index)
{
	TestSetIndex(index);
	int playType = reinterpret_cast<CMainFrame *>(AfxGetMainWnd())->GetPlayType();
	if(playType == PLAY_FILE)
	{
		KillTimer(1010);
		KillTimer(2020);
	}
	m_playLock.Lock();
	CloseLive();	
	m_playLock.Unlock();

	UpdateUI();
}

