#pragma once


// CServerDlg 对话框

class CServerDlg : public CDialog
{
	DECLARE_DYNAMIC(CServerDlg)

public:
	CServerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CServerDlg();

// 对话框数据
	enum { IDD = IDD_STREAM_SERVER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

public:
	CString m_username;
	CString m_password;
	CString m_address;
	DWORD m_serverIp;
	short m_port;
};
