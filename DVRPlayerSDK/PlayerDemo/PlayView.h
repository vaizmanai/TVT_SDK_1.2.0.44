#pragma once
#include "DVRPlayerSDK.h"
#include "DVR_NET_SDK.h"
#include "StreamWriter.h"

#include <afxmt.h>
#include <vector>
#include <stdio.h>
using std::vector;
// CPlayView 窗体视图

class CPlayView : public CFormView
{
	DECLARE_DYNCREATE(CPlayView)
	static const int PLAY_AREA_HEIGHT_PERCENT = 80;
	static const int CONTROL_BAR_MAX_HEIGHT  = 50;
	static const int DECODE_POOL_SIZE = 1024;	
protected:
	CPlayView();           // 动态创建所使用的受保护的构造函数
	virtual ~CPlayView();

public:
	enum { IDD = IDD_PLAYVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
	
	long	GetPlayPort(int pos){return m_playPort[pos];}
	void	DrawGrid(CDC *);
	void    ResetPlayArea();
	static void CALLBACK LiveCallBack(LONG lLiveHandle, NET_SDK_FRAME_INFO frameInfo, BYTE *pBuffer, void *pUser);
	static void CALLBACK IndexEndCallBack(long port, void *params);
	static void CALLBACK FileEndCallBack(long port, void *params);
	static void CALLBACK DrawCallBack(long nPort, HDC hDC, long lWidth, long lHeight, void * pwUser);
protected:
	int				m_playpos;
	BOOL			m_isPlay[16];
	BOOL			m_isPause[16];
	BOOL			m_isEnableSound[16];
	int				m_playChnn[16];
	int             m_ChnnPlayType[16];
	BOOL			m_isOpenedStream[14];
	LONG			m_liveHandle[16];
	int				m_volume[16];
	
	long			m_playPort[16];
	long			m_playFastSpeed;
	long			m_playBackSpeed;
	
	int				m_curDivide;
	CCriticalSection	m_playLock;
    bool             m_getVedioFormat[16];
    bool             m_getAudioFormat[16];
	SDK_FRAME_INFO   m_frameinfo[16];
	BYTE             *m_pBufferList[16]; 

	CStreamWriter    m_streamWriter;
	

	void			UpdateUI();
	BOOL			OpenLive();
	BOOL            OpenPlayBack();
	void			CloseLive();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateOperateUI( CCmdUI* pCmdUI );
	afx_msg void OnOperateCommand(UINT ID);
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonSlow();
	afx_msg void OnBnClickedButtonFast();
	afx_msg void OnBnClickedButtonFrame();
	afx_msg void OnBnClickedButtonSnap();
	afx_msg void OnBnClickedButtonSound();
	afx_msg void OnNMReleasedcaptureSliderPlaypos(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnChangeChannel(WPARAM, LPARAM);
	afx_msg void OnBnClickedButtonPlaytype();
	virtual void OnDraw(CDC* /*pDC*/);
	void OnClickedPlayArea(int index);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	void TestSetIndex(int index);
	void TestOpenLive(int index,int channl);
	void TestCloseLive(int index);
private:
	int m_playerType;
	vector<int> m_areaList;
	
	virtual BOOL DestroyWindow();
};


