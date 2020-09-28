#pragma once
#include "DVR_NET_SDK.h"
#include "afxcmn.h"

// CBackupDlg 对话框

class CBackupDlg : public CDialog
{
	DECLARE_DYNAMIC(CBackupDlg)

public:
	CBackupDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBackupDlg();
	
// 对话框数据
	enum { IDD = IDD_BACKUP_DIALOG };

protected:
	NET_SDK_REC_FILE	   *m_backupFiles;
	int						m_fileNum;
	int						m_doneNum;
	LONG					m_userID;
	LONG					m_fileHanle;
	CString					m_backupPath;
	CProgressCtrl			m_progress;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void refreshTip();
	DECLARE_MESSAGE_MAP()
public:
	void SetBackupInfo(LONG userid, NET_SDK_REC_FILE *files, int num, CString path){m_userID = userid;
	m_backupFiles = files; m_fileNum = num;m_backupPath = path;};

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
};
