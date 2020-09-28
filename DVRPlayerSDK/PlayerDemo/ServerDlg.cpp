// ServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PlayerDemo.h"
#include "ServerDlg.h"
#include "LangStr.h"


// CServerDlg 对话框

IMPLEMENT_DYNAMIC(CServerDlg, CDialog)

CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
	, m_username(_T(""))
	, m_password(_T(""))
	, m_serverIp(0)
	, m_port(0)
{

}

CServerDlg::~CServerDlg()
{
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_username);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_password);
	DDX_IPAddress(pDX, IDC_IPADDRESS, m_serverIp);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
}


BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CServerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CServerDlg 消息处理程序

BOOL CServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetDlgItemText(IDC_IPADDRESS, _T("192.168.3.122"));
	SetDlgItemText(IDC_EDIT_PORT, _T("6036"));
	SetDlgItemText(IDC_EDIT_USERNAME, _T("admin"));
	SetDlgItemText(IDC_EDIT_PASSWORD, _T("123456"));
	
	SetDlgItemText(IDC_STATIC_IP, LANG[STR_IPADDRESS]);
	SetDlgItemText(IDC_STATIC_PORT, LANG[STR_PORT]);
	SetDlgItemText(IDC_STATIC_USERNAME, LANG[STR_USERNAME]);
	SetDlgItemText(IDC_STATIC_PASSWORD, LANG[STR_PASSWORD]);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CServerDlg::OnBnClickedOk()
{
	GetDlgItemText(IDC_IPADDRESS, m_address);
	OnOK();
}
