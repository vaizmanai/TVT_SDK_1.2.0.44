// SoundDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PlayerDemo.h"
#include "SoundDlg.h"
#include "LangStr.h"


// CSoundDlg 对话框

IMPLEMENT_DYNAMIC(CSoundDlg, CDialog)

CSoundDlg::CSoundDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSoundDlg::IDD, pParent)
	, m_volume(0)
	, m_mute(FALSE)
{

}

CSoundDlg::~CSoundDlg()
{
}

void CSoundDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_SLIDER_VOLUME, m_volume);
	DDX_Check(pDX, IDC_CHECK_MUTE, m_mute);
}


BEGIN_MESSAGE_MAP(CSoundDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CSoundDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CSoundDlg::OnBnClickedOk)
	ON_WM_NCACTIVATE()
END_MESSAGE_MAP()


// CSoundDlg 消息处理程序

void CSoundDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnCancel();
}

void CSoundDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnOK();
}

void CSoundDlg::SetPos( int x, int y )
{
	m_x = x;
	m_y = y;
}

BOOL CSoundDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rect;
	GetWindowRect(&rect);
	
	SetWindowPos(NULL, m_x, m_y - rect.Height(), 0, 0, SWP_NOSIZE|SWP_NOZORDER);
	UpdateData(FALSE);
	PostMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(10, 10));
	SetDlgItemText(IDC_CHECK_MUTE, LANG[STR_MUTE]);
    CSliderCtrl *ctrl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_VOLUME);
	ctrl->SetRange(0,65535);
	ctrl->SetPos(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CSoundDlg::OnNcActivate(BOOL bActive)
{
	if (!bActive)
	{
		OnOK();
	}

	return CDialog::OnNcActivate(bActive);
}
