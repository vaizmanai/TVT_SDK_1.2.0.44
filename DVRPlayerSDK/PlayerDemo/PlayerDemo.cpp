// PlayerDemo.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "PlayerDemo.h"
#include "MainFrm.h"
#include "DVR_NET_SDK.h"
#include "DVRPlayerSDK.h"
#include "LangStr.h"
#include "MiniDump.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPlayerDemoApp

BEGIN_MESSAGE_MAP(CPlayerDemoApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CPlayerDemoApp::OnAppAbout)
END_MESSAGE_MAP()


// CPlayerDemoApp ����

CPlayerDemoApp::CPlayerDemoApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CPlayerDemoApp ����

CPlayerDemoApp theApp;


// CPlayerDemoApp ��ʼ��

BOOL CPlayerDemoApp::InitInstance()
{
	CMiniDump::Begin();
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	
//	_CrtSetBreakAlloc(860);
//	_CrtSetBreakAlloc(600);
	//_CrtSetBreakAlloc(633);
	//_CrtSetBreakAlloc(1156);

	CWinApp::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	// ��Ҫ���������ڣ��˴��뽫�����µĿ�ܴ���
	// ����Ȼ��������ΪӦ�ó���������ڶ���

	NET_SDK_Init();
	NET_SDK_SetConnectTime(1000);
	NET_SDK_SetReconnect(1000);
	
	PLAYER_Initialize();
	
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	LANG.LoadStr();
	// ���������ؿ�ܼ�����Դ
	if (LANG.IsChinese())
	{
		pFrame->LoadFrame(IDR_MAINFRAME_CN,
			WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
			NULL);
	}
	else
	{
		pFrame->LoadFrame(IDR_MAINFRAME,
			WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
			NULL);
	}

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	//pFrame->ShowWindow(SW_SHOW);
	pFrame->ShowWindow(SW_MAXIMIZE);
	pFrame->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand  ֮����
	return TRUE;
}


// CPlayerDemoApp ��Ϣ�������




// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CPlayerDemoApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CPlayerDemoApp ��Ϣ�������


int CPlayerDemoApp::ExitInstance()
{
	PLAYER_UnInitialize();

	CMiniDump::End();

	return CWinApp::ExitInstance();
}
