// PlayerDemo.cpp : 定义应用程序的类行为。
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


// CPlayerDemoApp 构造

CPlayerDemoApp::CPlayerDemoApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CPlayerDemoApp 对象

CPlayerDemoApp theApp;


// CPlayerDemoApp 初始化

BOOL CPlayerDemoApp::InitInstance()
{
	CMiniDump::Begin();
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	
//	_CrtSetBreakAlloc(860);
//	_CrtSetBreakAlloc(600);
	//_CrtSetBreakAlloc(633);
	//_CrtSetBreakAlloc(1156);

	CWinApp::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	// 若要创建主窗口，此代码将创建新的框架窗口
	// 对象，然后将其设置为应用程序的主窗口对象

	NET_SDK_Init();
	NET_SDK_SetConnectTime(1000);
	NET_SDK_SetReconnect(1000);
	
	PLAYER_Initialize();
	
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	LANG.LoadStr();
	// 创建并加载框架及其资源
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

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	//pFrame->ShowWindow(SW_SHOW);
	pFrame->ShowWindow(SW_MAXIMIZE);
	pFrame->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand  之后发生
	return TRUE;
}


// CPlayerDemoApp 消息处理程序




// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// 用于运行对话框的应用程序命令
void CPlayerDemoApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CPlayerDemoApp 消息处理程序


int CPlayerDemoApp::ExitInstance()
{
	PLAYER_UnInitialize();

	CMiniDump::End();

	return CWinApp::ExitInstance();
}
