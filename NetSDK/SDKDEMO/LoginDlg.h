#pragma once

// CLoginDlg 对话框

class CLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLoginDlg();

// 对话框数据
	enum { IDD = IDD_LOGIN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	CString m_username;
	CString m_password;
	CString m_ip;
	short m_port;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioIp();
	afx_msg void OnBnClickedRadioDomain();
	afx_msg void OnBnClickedOk();

	CString GetUserName(){return m_username;};
	CString GetPassword(){return m_password;};
	void SetIp(CString ip){m_ip = ip;}
	void SetPort(WORD port){m_port = port;}
	WORD GetPort(){return m_port;}
	CString GetIP(){return m_ip;}

};
