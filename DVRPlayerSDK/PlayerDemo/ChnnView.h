#pragma once


// CChnnView 视图

class CChnnView : public CTreeView
{
	DECLARE_DYNCREATE(CChnnView)
protected:
	CChnnView();           // 动态创建所使用的受保护的构造函数
	virtual ~CChnnView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

	virtual void OnInitialUpdate();
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
};


