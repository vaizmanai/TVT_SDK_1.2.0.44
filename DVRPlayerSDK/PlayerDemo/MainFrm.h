// MainFrm.h : CMainFrame 类的接口
//


#pragma once
#include "DVR_NET_SDK.h"
#include "DVRPlayerSDK.h"
#include <afxmt.h>
#include <list>

class CChnnView;
class CPlayView;
using std::list;

typedef enum _play_type_
{
	PLAY_NONE,
	PLAY_FILE,
	PLAY_STREAM,
	PLAY_BACK
}PLAY_TYPE;

class MPUB_Lock
{
public:
	MPUB_Lock();
	~MPUB_Lock();

	void Lock();	
	void UnLock();	
private:
	CRITICAL_SECTION m_Lock;
};

struct SPLAYBACKSTATUS
{
	LONG playhadle;
	LONG status;
};
class CMainFrame : public CFrameWnd
{
	static const int MIN_PLAYVIEW_WIDTH = 320;
	static const int MIN_PLAYVIEW_HEIGHT = 300;
	static const int CHNNVIEW_WIDTH_PERCENT = 30;
	static const int MAX_CHNNVIEW_WIDTH = 200;

public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	
// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CPlayView*	GetPlayView(){return m_playView;}
	CChnnView*	GetChnnView(){return m_chnnView;}
	int			GetPlayType(){return m_playType;}
	LONG		GetUserID(){return m_userID;}
// 生成的消息映射函数
protected:
	
	
	CSplitterWnd	m_spliter;
	CChnnView*		m_chnnView;
	CPlayView*		m_playView;
	int				m_playType;
	LONG			m_userID;
	NET_SDK_DEVICEINFO	m_deviceInfo;
	

	void	UpdateUI();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnOPCommand(UINT ID);
	afx_msg void OnOpenFile();
	afx_msg void OnOpenStream();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnUpdateClose(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOpenStream(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOpenFile(CCmdUI *pCmdUI);
public:

	/****************新增测试代码**********************/
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	static DWORD WINAPI TestFunctionThread(void *pParam);
	void TestFunctionProc();
	bool IsPlaying(LONG playHandle);
	void ClearPlayHandle(LONG playHandle);
	void AddPlayHandle(LONG playHandle);
	static void CALLBACK LiveCallBack(LONG lLiveHandle, NET_SDK_FRAME_INFO frameInfo, BYTE *pBuffer, void *pUser);
	void LiveCallBackProc(LONG lLiveHandle, NET_SDK_FRAME_INFO frameInfo, BYTE *pBuffer, void *pUser);
	static void CALLBACK excertionCallback(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser);
private:
	
	list<SPLAYBACKSTATUS>    m_playHandlList;
	MPUB_Lock		m_playHandlLock;
	volatile int	m_channl;
	CString			m_address;
	short			m_port;
	CString			m_username;
	CString			m_password;
};


