// SearchDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "PlayerDemo.h"
#include "SearchDialog.h"
#include <assert.h>



// CSearchDialog 对话框

IMPLEMENT_DYNAMIC(CSearchDialog, CDialog)

CSearchDialog::CSearchDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSearchDialog::IDD, pParent)
{
	m_channl = -1;
	m_userId = -1;
	m_index  = -1;
}

CSearchDialog::~CSearchDialog()
{
}

void CSearchDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSearchDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CANCL, &CSearchDialog::OnBnClickedButtonCancl)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CSearchDialog::OnBnClickedButtonOk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FILE, &CSearchDialog::OnNMDblclkListFile)
END_MESSAGE_MAP()


// CSearchDialog 消息处理程序

void CSearchDialog::OnBnClickedButtonCancl()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CSearchDialog::OnBnClickedButtonOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CListCtrl *pListCtrl = (CListCtrl *)GetDlgItem(IDC_LIST_FILE);
	CStatic *pLabelStart = (CStatic *)GetDlgItem(IDC_STATIC_START);
	CStatic *pLabelEnd = (CStatic *)GetDlgItem(IDC_STATIC_END);
	CButton *pButtonCancle = (CButton *)GetDlgItem(IDC_BUTTON_CANCL);
	CButton *pButtonOk = (CButton *)GetDlgItem(IDC_BUTTON_OK);
	CDateTimeCtrl *pDateStart = (CDateTimeCtrl *)GetDlgItem(IDC_DATE_START);
	CDateTimeCtrl *pTimeStart = (CDateTimeCtrl *)GetDlgItem(IDC_TIME_START);
	CDateTimeCtrl *pDateEnd = (CDateTimeCtrl *)GetDlgItem(IDC_DATE_END);
	CDateTimeCtrl *pTimeEnd = (CDateTimeCtrl *)GetDlgItem(IDC_TIME_END);

	pLabelStart->ShowWindow(SW_HIDE);
	pLabelEnd->ShowWindow(SW_HIDE);
	pButtonOk->ShowWindow(SW_HIDE);
	pButtonCancle->ShowWindow(SW_HIDE);
	pDateStart->ShowWindow(SW_HIDE);
	pDateEnd->ShowWindow(SW_HIDE);
	pTimeStart->ShowWindow(SW_HIDE);
	pTimeEnd->ShowWindow(SW_HIDE);

	pListCtrl->SetExtendedStyle(pListCtrl->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);   

	CRect rect;
	GetClientRect(&rect);
	pListCtrl->MoveWindow(0,0,rect.Width(),rect.Height());
	pListCtrl->ShowWindow(SW_SHOW);
    

	
	pListCtrl->GetClientRect(&rect);
	pListCtrl->InsertColumn(0, _T("录像"), LVCFMT_LEFT,50, 0);   
	pListCtrl->InsertColumn(1, _T("开始时间"), LVCFMT_LEFT, (rect.Width()-50)/2, 1);   
	pListCtrl->InsertColumn(2, _T("结束时间"), LVCFMT_LEFT, (rect.Width()-50)/2, 2);   

	// 在列表视图控件中插入列表项，并设置列表子项文本  
	assert(m_channl >= 0);
	assert(m_userId > 0);
	CTime localtime;
	DD_TIME startTime;
	DD_TIME endTime;
    
	pDateStart->GetTime(localtime);
    startTime.year = localtime.GetYear()-1900;
	startTime.month = localtime.GetMonth()-1;
	startTime.mday = localtime.GetDay();
	startTime.wday = localtime.GetDayOfWeek();
	pTimeStart->GetTime(localtime);
	startTime.hour = localtime.GetHour();
	startTime.minute = localtime.GetMinute();
	startTime.second = localtime.GetSecond();

	pDateEnd->GetTime(localtime);
	endTime.year = localtime.GetYear()-1900;
	endTime.month = localtime.GetMonth()-1;
	endTime.mday = localtime.GetDay();
	endTime.wday = localtime.GetDayOfWeek();
	pTimeEnd->GetTime(localtime);
	endTime.hour = localtime.GetHour();
	endTime.minute = localtime.GetMinute();
	endTime.second = localtime.GetSecond();
    
	LONG ret = NET_SDK_FindFile(m_userId,m_channl,&startTime,&endTime);
    if (ret != -1)
    {
	    NET_SDK_REC_FILE  recfile;
		int count=0;
		while(NET_SDK_FindNextFile(ret,&recfile) != NET_SDK_NOMOREFILE)
		{
			CString str;
			
			str.Format(_T("录像%d"),count);
			pListCtrl->InsertItem(count,str);
			str.Format(_T("%d/%d/%d %d:%d:%d"),recfile.startTime.year+1900,recfile.startTime.month+1,recfile.startTime.mday
				,recfile.startTime.hour,recfile.startTime.minute,recfile.startTime.second);
			pListCtrl->SetItemText(count, 1, str);
			str.Format(_T("%d/%d/%d %d:%d:%d"),recfile.stopTime.year+1900,recfile.stopTime.month+1,recfile.stopTime.mday
				,recfile.stopTime.hour,recfile.stopTime.minute,recfile.stopTime.second);
			pListCtrl->SetItemText(count, 2, str);
			m_recList.push_back(recfile);
			count++;
		}
    }

	/*LONG ret = NET_SDK_FindTime(m_userId,m_channl,&startTime,&endTime);
	if (ret != -1)
	{
		NET_SDK_REC_TIME recTime;
		int count = 0;
		LONG retValue = NET_SDK_FindNextTime(ret,&recTime); 
		while(retValue != NET_SDK_FILE_NOFIND && retValue!=NET_SDK_NOMOREFILE && retValue>0)
		{
			CString str;

			str.Format(_T("录像%d"),count);
			pListCtrl->InsertItem(count,str);
			str.Format(_T("%d/%d/%d %d:%d:%d"),recTime.startTime.year+1900,recTime.startTime.month+1,recTime.startTime.mday
				,recTime.startTime.hour,recTime.startTime.minute,recTime.startTime.second);
			pListCtrl->SetItemText(count, 1, str);
			str.Format(_T("%d/%d/%d %d:%d:%d"),recTime.stopTime.year+1900,recTime.stopTime.month+1,recTime.stopTime.mday
				,recTime.stopTime.hour,recTime.stopTime.minute,recTime.stopTime.second);
			pListCtrl->SetItemText(count, 2, str);
			NET_SDK_REC_FILE recFile;
			recFile.dwChannel = recTime.dwChannel;
			recFile.startTime = recTime.startTime;
			recFile.stopTime  = recTime.stopTime; 
			m_recList.push_back(recFile);
			count++;
			retValue = NET_SDK_FindNextTime(ret,&recTime);
		}
	}*/
}

void CSearchDialog::OnNMDblclkListFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);   
	*pResult = 0;
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;
	m_index = pNMListView->iItem;
	OnOK();
}


int CSearchDialog::GetRecFile(NET_SDK_REC_FILE &recfile)
{
	if (m_index >= 0 && m_index < m_recList.size())
	{
		recfile = m_recList[m_index];
	}

	return m_index;
}
