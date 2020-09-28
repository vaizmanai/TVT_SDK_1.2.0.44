// ChnnView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PlayerDemo.h"
#include "ChnnView.h"
#include "MainFrm.h"
#include "PlayView.h"
#include "LangStr.h"

// CChnnView

IMPLEMENT_DYNCREATE(CChnnView, CTreeView)

CChnnView::CChnnView()
{

}

CChnnView::~CChnnView()
{
}

BEGIN_MESSAGE_MAP(CChnnView, CTreeView)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CChnnView::OnNMDblclk)
END_MESSAGE_MAP()


// CChnnView ���

#ifdef _DEBUG
void CChnnView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CChnnView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CChnnView ��Ϣ�������

void CChnnView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

}

void CChnnView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	CString text = GetTreeCtrl().GetItemText(item);
	int chnn = 0;
	sscanf_s(text.GetBuffer(), LANG[STR_CHANNEL_D], &chnn);
	reinterpret_cast<CMainFrame *>(AfxGetMainWnd())->GetPlayView()->PostMessage(WM_CHANGE_CHANNEL, -1, chnn - 1);
	*pResult = 0;
}