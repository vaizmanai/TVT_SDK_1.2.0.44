// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SDKDEMO.h"
#include "LoginDlg.h"
#include <WinSock2.h>

// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
	, m_username(_T(""))
	, m_password(_T(""))
	, m_port(0)
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_username);
	DDV_MaxChars(pDX, m_username, 65);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_password);
	DDV_MaxChars(pDX, m_password, 65);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_IP, &CLoginDlg::OnBnClickedRadioIp)
	ON_BN_CLICKED(IDC_RADIO_DOMAIN, &CLoginDlg::OnBnClickedRadioDomain)
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序

void CLoginDlg::OnBnClickedOk()
{
	IsDlgButtonChecked(IDC_RADIO_IP) ?
		GetDlgItemText(IDC_IPADDRESS, m_ip) : GetDlgItemText(IDC_EDIT_DOMAIN, m_ip);

	m_port = GetDlgItemInt(IDC_EDIT_PORT);
	
	OnOK();
}



BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetDlgItemText(IDC_IPADDRESS, m_ip);
	SetDlgItemInt(IDC_EDIT_PORT, m_port, FALSE);

	m_username = _T("admin");
	m_password = _T("123456");
	
	SetDlgItemText(IDC_EDIT_DOMAIN, _T("www.google.com"));

	UpdateData(FALSE);
	
	CheckDlgButton(IDC_RADIO_IP, BST_CHECKED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CLoginDlg::OnBnClickedRadioIp()
{
	GetDlgItem(IDC_IPADDRESS)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_DOMAIN)->ShowWindow(SW_HIDE);
}

void CLoginDlg::OnBnClickedRadioDomain()
{
	GetDlgItem(IDC_IPADDRESS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_DOMAIN)->ShowWindow(SW_SHOW);
}
