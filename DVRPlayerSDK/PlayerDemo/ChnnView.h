#pragma once


// CChnnView ��ͼ

class CChnnView : public CTreeView
{
	DECLARE_DYNCREATE(CChnnView)
protected:
	CChnnView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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


