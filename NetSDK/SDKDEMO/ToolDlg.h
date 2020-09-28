#pragma once
#include "afxcmn.h"
#include "DVR_NET_SDK.h"
#include "afxdtctl.h"
#include "gridctrl.h"

// CToolDlg 对话框

class CToolDlg : public CDialog
{
	DECLARE_DYNAMIC(CToolDlg)

public:
	CToolDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CToolDlg();

// 对话框数据
	enum { IDD = IDD_TOOL_DIALOG };

protected:
	CListCtrl			m_logList;
	CTime				m_logStartTime;
	CTime				m_logEndTime;
	CTime				m_sysTime;
	BOOL				m_isStartVoiceComm;
	BOOL				m_isOpenMsgCallback;
	BOOL				m_isStartVoiceComm_MR;
	BOOL				m_runTalkTransThread;
	HANDLE				m_talkTransThread;
	HANDLE				m_talkDataFile;
	LONG				m_voiceHandle;
	LONG				m_talkTransHandle;
	LONG				m_msgCBHandle;
	NET_SDK_DEVICEINFO	*m_deviceinfo;
	LONG				m_userID;
	int					m_voiceVolume;
	CDateTimeCtrl		m_startTimeCtrl;
	CDateTimeCtrl		m_endTimeCtrl;
	CDateTimeCtrl		m_systimeCtrl;
	CGridCtrl			m_diskGrid;
	CList<NET_SDK_DISK_INFO> m_diskList;
	CWnd*				m_mainDlg;
	void UpdateUI();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	static void CALLBACK TalkTransCallBack(LONG lVoiceComHandle, char *pRecvDataBuffer, DWORD dwBufSize, BYTE byAudioFlag, void* pUser);
	static DWORD WINAPI TalkTransSendThread(LPVOID);
	void TalkTransSendProc();

	void setDeviceInfo(NET_SDK_DEVICEINFO *di, LONG userid){m_userID = userid; m_deviceinfo = di;}

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonSearchlog();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonVoiceComm();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonChangesystime();
	afx_msg void OnBnClickedButtonDiskRefresh();
	afx_msg void OnBnClickedButtonChangeDiskProperty();
	afx_msg void OnBnClickedButtonDiskFormat();
	afx_msg void OnBnClickedButtonMsgCallback();
	void setMainDlg( CWnd* dlg );
	afx_msg void OnBnClickedButtonTalktRans();
};
