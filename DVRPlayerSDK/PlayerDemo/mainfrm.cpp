// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "PlayerDemo.h"
#include "MainFrm.h"
#include "ChnnView.h"
#include "PlayView.h"
#include "ServerDlg.h"
#include "LangStr.h"
#include <vector>
#include <assert.h>

using std::vector;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

MPUB_Lock::MPUB_Lock()
{
	InitializeCriticalSection(&m_Lock);
}

MPUB_Lock::~MPUB_Lock()
{
	DeleteCriticalSection(&m_Lock);
}

void MPUB_Lock::Lock()
{
	EnterCriticalSection(&m_Lock);
}

void MPUB_Lock::UnLock()
{
	LeaveCriticalSection(&m_Lock);
}


// CMainFrame
IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_COMMAND(ID_OPEN_FILE, &CMainFrame::OnOpenFile)
	ON_COMMAND(ID_OPEN_STREAM, &CMainFrame::OnOpenStream)
	ON_WM_DESTROY()
	ON_COMMAND(ID_CLOSE, &CMainFrame::OnClose)
	ON_UPDATE_COMMAND_UI(ID_CLOSE, &CMainFrame::OnUpdateClose)
	ON_UPDATE_COMMAND_UI(ID_OPEN_STREAM, &CMainFrame::OnUpdateOpenStream)
	ON_UPDATE_COMMAND_UI(ID_OPEN_FILE, &CMainFrame::OnUpdateOpenFile)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
:m_playView(NULL)
, m_chnnView(NULL)
, m_playType(PLAY_NONE)
, m_userID(-1)
,m_channl(0)
{

}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_chnnView->GetTreeCtrl().InsertItem(LANG[STR_SERVER_NONE], TVI_ROOT);
	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	
    
	cs.x=GetSystemMetrics(SM_CXSCREEN)/8;
	cs.y=GetSystemMetrics(SM_CYSCREEN)/8;
	cs.cx = GetSystemMetrics(SM_CXSCREEN)/4*3;
	cs.cy = GetSystemMetrics(SM_CYSCREEN)/4*3;
	

	//cs.style &= ~WS_MAXIMIZEBOX;
	//cs.style &= ~WS_THICKFRAME;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 将焦点前移到视图窗口

}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (m_playView)
	{
		m_pViewActive = m_playView;
	}

	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	m_spliter.CreateStatic(this, 1, 2);
	m_spliter.CreateView(0, 0, RUNTIME_CLASS(CPlayView), CSize(1, 1), pContext);
	m_spliter.CreateView(0, 1, RUNTIME_CLASS(CChnnView), CSize(1, 1), pContext);
	
	m_playView = reinterpret_cast<CPlayView *>(m_spliter.GetPane(0, 0));
	m_chnnView = reinterpret_cast<CChnnView *>(m_spliter.GetPane(0, 1));
	
	m_chnnView->GetTreeCtrl().ModifyStyle(0, TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS);
	m_chnnView->EnableWindow(TRUE);
	m_playView->EnableWindow(TRUE);

	m_playView->MoveWindow(0, 0, 448, 480);
	m_chnnView->MoveWindow(448, 0, 192, 480);
	
	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	int chnn_width = (cx * CHNNVIEW_WIDTH_PERCENT) / 100;
	
	chnn_width = chnn_width <= MAX_CHNNVIEW_WIDTH ? chnn_width : MAX_CHNNVIEW_WIDTH;
	
	m_playView->MoveWindow(0, 0, cx - chnn_width, cy);
	m_chnnView->MoveWindow(cx - chnn_width, 0, chnn_width, cy);
	
}

void CMainFrame::OnSizing(UINT fwSide, LPRECT pRect)
{
	CFrameWnd::OnSizing(fwSide, pRect);
	
	CRect rect(pRect);
	int chnn_width = (rect.Width() * CHNNVIEW_WIDTH_PERCENT) / 100;
	int width = rect.Width() - chnn_width;
	if (width < MIN_PLAYVIEW_WIDTH)
	{
		pRect->right = pRect->left + ((MIN_PLAYVIEW_WIDTH) * 100 / (100 - CHNNVIEW_WIDTH_PERCENT));
	}

	if (rect.Height() < MIN_PLAYVIEW_HEIGHT)
	{
		pRect->bottom = pRect->top + MIN_PLAYVIEW_HEIGHT;
	}
}
void CMainFrame::OnOpenFile()
{
	CFileDialog dlg(TRUE, _T("rec"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("record files(*.rec)|*.rec|All Files (*.*)|*.*||"), this);
	if (IDOK == dlg.DoModal())
	{
		long port = m_playView->GetPlayPort(0);
		BOOL ret = PLAYER_OpenFile(port, dlg.GetPathName().GetBuffer());
		TRACE(_T("PLAYER_OpenFile: %d\n"), ret);
		if (ret)
		{
			m_playType = PLAY_FILE;
			m_playView->SendMessage(WM_COMMAND, MAKEWPARAM(ID_PLAY, 0), 0);
		}
		else
		{
			AfxMessageBox(LANG[STR_OPEN_FILE_FAILED]);
		}
		m_playView->ResetPlayArea();
		UpdateUI();
	}
}

void CMainFrame::OnOpenStream()
{
	CServerDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		if (m_userID > 0)
		{
			NET_SDK_Logout(m_userID);
			m_userID = -1;
			m_playType = PLAY_NONE;
		}
		
		m_address  = dlg.m_address;
		m_port     = dlg.m_port;
		m_username = dlg.m_username;
		m_password = dlg.m_password;

		if ((m_userID = NET_SDK_Login(dlg.m_address.GetBuffer(), dlg.m_port, 
			dlg.m_username.GetBuffer(), dlg.m_password.GetBuffer(), &m_deviceInfo)) > 0)
		{
			m_playType = PLAY_STREAM;
		}
		else
		{
			AfxMessageBox(LANG[STR_OPEN_STREAM_FAILED]);
		}
		m_playView->ResetPlayArea();
		UpdateUI();
	

		/*************测试代码**************/
		SetTimer(1010, 20000, NULL);
		for (int i = 0; i < 4; i++)
		{
			CreateThread(NULL,0,TestFunctionThread,this,0,NULL);
		}
	
	}
}

void CMainFrame::UpdateUI()
{
	m_playView->EnableWindow(m_playType != PLAY_NONE);
	m_chnnView->EnableWindow(m_playType == PLAY_STREAM);
	
	if (m_playType == PLAY_STREAM)
	{
		m_chnnView->GetTreeCtrl().DeleteAllItems();
		CTreeCtrl &tree = m_chnnView->GetTreeCtrl();
		CString devname;
		devname.Format(LANG[STR_SERVER_S], m_deviceInfo.deviceName);
		HTREEITEM item = tree.InsertItem(devname, TVI_ROOT);
		
		CString temp;
		HTREEITEM prevItem = NULL;
		for (int i = 0; i < m_deviceInfo.videoInputNum; i++)
		{
			temp.Format(LANG[STR_CHANNEL_D], i + 1);
			prevItem = tree.InsertItem(temp, item, prevItem);
		}
		tree.Expand(item, TVE_EXPAND);
		CDC *dc = GetDC();
		m_playView->DrawGrid(dc);
		ReleaseDC(dc);
	}
	else
	{
		m_chnnView->GetTreeCtrl().DeleteAllItems();
		m_chnnView->GetTreeCtrl().InsertItem(LANG[STR_SERVER_NONE], TVI_ROOT);
	}
}

void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();
	if (m_playType != PLAY_NONE)
	{
		OnClose();
	}
	PLAYER_UnInitialize();
	NET_SDK_Cleanup();
}

void CMainFrame::OnClose()
{
	if (m_playType == PLAY_FILE)
	{
		m_playView->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STOP, 0), 0);
		long port = m_playView->GetPlayPort(0);
		PLAYER_CloseFile(port);
	}
	else if(m_playType == PLAY_STREAM)
	{
		m_playView->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STOP, 0), 0);
		NET_SDK_Logout(m_userID);
		m_userID = -1;
	}
	m_playType = PLAY_NONE;
	m_playView->SendMessage(WM_CHANGE_CHANNEL, 0, 0);
	UpdateUI();
}

void CMainFrame::OnUpdateClose(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_playType != PLAY_NONE);
}

void CMainFrame::OnUpdateOpenStream(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_playType == PLAY_NONE);
}

void CMainFrame::OnUpdateOpenFile(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_playType == PLAY_NONE);
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	/*****************************************************************/
	/***********************轮询测试代码*****************************/
	/*****************************************************************/
	/*LONG userID;
	if (nIDEvent == 1010)
	{
		
		m_playView->TestCloseLive(0);
		m_playView->TestCloseLive(1);
		m_playView->TestCloseLive(2);
		m_playView->TestCloseLive(3);
		
		
		m_playView->TestOpenLive(0,1);
		m_playView->TestOpenLive(1,2);
		m_playView->TestOpenLive(2,3);
		m_playView->TestOpenLive(3,4);
	}*/
	
	CFrameWnd::OnTimer(nIDEvent);
}


void CALLBACK CMainFrame::excertionCallback( DWORD dwType, LONG lUserID, LONG lHandle, void *pUser )
{
}

DWORD WINAPI CMainFrame::TestFunctionThread(void *pParam)
{
	CMainFrame *pDlg = (CMainFrame *)(pParam);
	//pDlg->TestFunctionProc();
	return 0;
}

void CMainFrame::TestFunctionProc()
{
	LONG userID = -1;
	NET_SDK_DEVICEINFO deviceInfo;
	LONG channl = m_channl++;
	vector<NET_SDK_REC_FILE> recList;
	recList.clear();
	DD_TIME startTime = {1,1,0,2,6,4,113};
	DD_TIME endTime = {59,59,22,2,6,4,113};

	userID = NET_SDK_Login(m_address.GetBuffer(),m_port,m_username.GetBuffer(),m_password.GetBuffer(),&deviceInfo);
	
	if (userID < 0)
	{
		assert(false);
		return;
	}

	while(true)
	{	
		/************现场测试**************/
		/*NET_SDK_CLIENTINFO clientInfo;
		ZeroMemory(&clientInfo, sizeof(NET_SDK_CLIENTINFO));
		clientInfo.lChannel = 5;
		clientInfo.streamType = NET_SDK_MAIN_STREAM;
		LONG handle = NET_SDK_LivePlay(m_userID, &clientInfo, &LiveCallBack, (LPVOID)m_playView);
		Sleep(30000);
		if (handle > 0)
		{
			TRACE("Hello World!\n");
			NET_SDK_StopLivePlay(handle);
		}*/


		/*******************回放测试**********************/
	

		LONG retValue = NET_SDK_FindFile(m_userID,channl,&startTime,&endTime);
		if (retValue != -1)
		{
			NET_SDK_REC_FILE  recfile;
			while(NET_SDK_FindNextFile(retValue,&recfile) != NET_SDK_NOMOREFILE)
			{
				recList.push_back(recfile);	
			}
		}
		
		vector<NET_SDK_REC_FILE>::iterator iter = recList.begin(),end = recList.end();
		for (; iter!=end; iter++)
		{	
			NET_SDK_REC_FILE  recfile = *iter;
			LONG ret = NET_SDK_PlayBackByTime(userID,&channl,1,&recfile.startTime,&recfile.stopTime,NULL);	

			if (ret != -1)
			{
				AddPlayHandle(ret);
				TRACE("Add PlayBack Handle:%d\n",ret);
				if (NET_SDK_SetPlayDataCallBack(ret, &CMainFrame::LiveCallBack, (LPVOID)this))
				{
					while(IsPlaying(ret))
					{
						Sleep(10);
					}
				}

				ClearPlayHandle(ret);
				NET_SDK_StopPlayBack(ret);
				TRACE("Remove PlayBack Handle:%d\n",ret);
			}
			else
			{
				TRACE("PlayBack Failed!\n");
			}
		}
		
		recList.clear();
	}
}

void CALLBACK CMainFrame::LiveCallBack( LONG lLiveHandle, NET_SDK_FRAME_INFO frameInfo, BYTE *pBuffer, void *pUser )
{
	CMainFrame *pMainFrame = reinterpret_cast<CMainFrame *>(pUser);
	pMainFrame->LiveCallBackProc(lLiveHandle,frameInfo,pBuffer,pUser);
}


void CMainFrame::LiveCallBackProc(LONG lLiveHandle, NET_SDK_FRAME_INFO frameInfo, BYTE *pBuffer, void *pUser)
{
	m_playHandlLock.Lock();
	list<SPLAYBACKSTATUS>::iterator iter = m_playHandlList.begin(),end = m_playHandlList.end();
	if (frameInfo.frameType == DD_FRAME_TYPE_END)
	{
		for (; iter != end; iter++)
		{
			SPLAYBACKSTATUS &playbackStatus = *iter;
			if (playbackStatus.playhadle == lLiveHandle)
			{
				TRACE("DD_FRAME_TYPE_END\n");
				playbackStatus.status = 0;
			}
		}
	}
	m_playHandlLock.UnLock();

	Sleep(2);
}


bool CMainFrame::IsPlaying(LONG playHandle)
{
	bool ret = false;
	m_playHandlLock.Lock();
	list<SPLAYBACKSTATUS>::iterator iter = m_playHandlList.begin(),end = m_playHandlList.end();
	
	for (; iter != end; iter++)
	{
		SPLAYBACKSTATUS &playbackStatus = *iter;
		if (playbackStatus.playhadle == playHandle)
		{
			if (playbackStatus.status == 1)
			{
				ret = true;
			}
			else
			{
				ret = false;
			}
			break;
		}
	}

	m_playHandlLock.UnLock();
	return ret;
}


void CMainFrame::ClearPlayHandle(LONG playHandle)
{
	m_playHandlLock.Lock();
	list<SPLAYBACKSTATUS>::iterator iter = m_playHandlList.begin(),end = m_playHandlList.end();

	for (; iter != end; iter++)
	{
		SPLAYBACKSTATUS &playbackStatus = *iter;
		if (playbackStatus.playhadle == playHandle)
		{
			m_playHandlList.erase(iter);
			break;
		}
	}

	m_playHandlLock.UnLock();
}


void CMainFrame::AddPlayHandle(LONG playHandle)
{
	m_playHandlLock.Lock();
	SPLAYBACKSTATUS playbackStatus;
	playbackStatus.playhadle = playHandle;
	playbackStatus.status = 1;
	m_playHandlList.push_back(playbackStatus);
	m_playHandlLock.UnLock();
}