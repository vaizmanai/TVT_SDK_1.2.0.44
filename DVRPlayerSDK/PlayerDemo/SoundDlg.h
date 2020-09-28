#pragma once


// CSoundDlg 对话框

class CSoundDlg : public CDialog
{
	DECLARE_DYNAMIC(CSoundDlg)

public:
	CSoundDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSoundDlg();

// 对话框数据
	enum { IDD = IDD_SOUND_DIALOG };
	
	void SetPos(int x, int y);

protected:
	int		m_x;
	int		m_y;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

public:
	int m_volume;
	BOOL m_mute;
	afx_msg BOOL OnNcActivate(BOOL bActive);
};
