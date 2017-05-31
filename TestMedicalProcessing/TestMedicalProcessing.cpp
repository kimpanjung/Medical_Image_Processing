
// TestMedicalProcessing.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TestMedicalProcessing.h"
#include "TestMedicalProcessingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMedicalProcessingApp

BEGIN_MESSAGE_MAP(CTestMedicalProcessingApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTestMedicalProcessingApp construction

CTestMedicalProcessingApp::CTestMedicalProcessingApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTestMedicalProcessingApp object

CTestMedicalProcessingApp theApp;


// CTestMedicalProcessingApp initialization

BOOL CTestMedicalProcessingApp::InitInstance()
{

	// REPETITION_CHECK
	m_hEventRep = ::CreateEvent(NULL, FALSE, FALSE, _T("REPETITION_CHECK_EVENT"));
	if (::GetLastError() == ERROR_ALREADY_EXISTS || m_hEventRep == NULL)
	{
		AfxMessageBox(_T("프로그램이 이미 실행중입니다."));
		return FALSE;
	}

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	

	// GDI+ 초기화..
	GdiplusStartupInput gpsi;
	if(GdiplusStartup(&m_gpToken, &gpsi, NULL) != Ok) {
		AfxMessageBox(_T("GDI+ FAIL"));
		return FALSE;
	}






	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CTestMedicalProcessingDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

