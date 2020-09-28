#pragma once

#include <vector>
#include "DVR_NET_SDK.h"

using std::vector;
// CSearchDialog 对话框

class CSearchDialog : public CDialog
{
	DECLARE_DYNAMIC(CSearchDialog)

public:
	CSearchDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSearchDialog();

// 对话框数据
	enum { IDD = IDD_SEARCH_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCancl();
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnNMDblclkListFile(NMHDR *pNMHDR, LRESULT *pResult);
	void SetChannl(LONG channl){m_channl = channl;}
	void SetUserId(LONG userId){m_userId = userId;}
	int  GetRecFile(NET_SDK_REC_FILE &recfile);
private:
	LONG m_channl;
	LONG m_userId;
	int  m_index;
	vector<NET_SDK_REC_FILE> m_recList;
};
