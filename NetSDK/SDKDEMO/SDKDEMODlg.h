// SDKDEMODlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "DVR_NET_SDK.h"

class CLiveDlg;
class CSearchDlg;
class CToolDlg;
class CConfigDlg;
class CMsgDlg;

// CSDKDEMODlg �Ի���
class CSDKDEMODlg : public CDialog
{
// ����
public:
	CSDKDEMODlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CSDKDEMODlg();
// �Ի�������
	enum { IDD = IDD_SDKDEMO_DIALOG };

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	
	void SetDeviceInfo(CString ip, WORD port){m_IP = ip;m_port = port;}

	void SetDeviceInfo(LONG userID, NET_SDK_DEVICEINFO *pDeviceInfo)
	{
		m_userID = userID;
		m_deviceInfo = *pDeviceInfo;
	}

// ʵ��
protected:
	HICON m_hIcon;
	CTabCtrl m_tab;
	CString m_IP;
	WORD	m_port;

	
	LONG					m_userID;
	NET_SDK_DEVICEINFO		m_deviceInfo;
	CLiveDlg				*m_liveDlg;
	CSearchDlg				*m_searchDlg;
	CToolDlg				*m_toolDlg;
	CConfigDlg				*m_configDlg;
	CMsgDlg					*m_msgDlg;
	CMap<LONG, LONG&, CString, CString&> m_ipMap;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDestroy();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnTcnSelchangeMainTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnShowMsgDlg(WPARAM wparam, LPARAM lparam);
	static void CALLBACK excertionCallback(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser);
	static BOOL CALLBACK msgCallback(LONG lCommand, LONG lUserID, char *pBuf, DWORD dwBufLen, void *pUser);

	afx_msg void OnBnClickedButtonLogtest();
};
