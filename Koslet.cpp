
// Koslet.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "Koslet.h"
#include "KosletDlg.h"
#include "SaverWnd.h"
#include "DrawWnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKosletApp

BEGIN_MESSAGE_MAP(CKosletApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CKosletApp construction

CKosletApp::CKosletApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CKosletApp object

CKosletApp theApp;


// CKosletApp initialization

BOOL CKosletApp::InitInstance()
{
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


	//AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	//CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	//CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	HWND hWnd = NULL;
	switch (this->GetScrMode(hWnd))
	{
	case SCRMODE::SMSAVER:
	{
		// Run the screen saver
		CSaverWnd* const pWnd = new CSaverWnd;
		pWnd->Create();
		m_pMainWnd = pWnd;
		return TRUE;
	}

	case SCRMODE::SMPREVIEW:
	{
		// Preview
		CWnd* const pWndParent = CWnd::FromHandle(hWnd);
		ASSERT(pWndParent != NULL);

		CDrawWnd* const pWnd = new CDrawWnd(TRUE, TRUE);
		CRect rect;
		pWndParent->GetClientRect(&rect);
		pWnd->Create(0, _T("Koslet preview"), WS_VISIBLE | WS_CHILD, rect, pWndParent);
		m_pMainWnd = pWnd;
		return TRUE;
	}

	case SCRMODE::SMCONFIG:
		// Configure
		this->DoConfig(hWnd);
		break;

	case SCRMODE::SMPASSWORD:
		// Password
		this->DoChangePassword(hWnd);
		break;

	case SCRMODE::SMNONE:
		// Nothing
		break;

	default:
		ASSERT(FALSE);
		break;
	}

	// Delete the shell manager created above.
	//if (pShellManager != nullptr)
	//{
	//	delete pShellManager;
	//}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}


//----------------------------------------------------------------------------------------------------------------------
SCRMODE CKosletApp::GetScrMode(HWND & hWnd) const
{
	SCRMODE ScrMode = SCRMODE::SMNONE;
	hWnd = NULL;

	TCHAR* pc = ::GetCommandLine();
	ASSERT(pc != NULL);

	if (*pc == _T('\"'))
	{
		pc++;
		while (*pc != _T('\0') && *pc != _T('\"'))
		{
			pc++;
		}
	}
	else
	{
		while (*pc != _T('\0') && *pc != _T(' '))
		{
			pc++;
		}
	}

	if (*pc != _T('\0'))
	{
		pc++;
	}

	while (*pc == _T(' '))
	{
		pc++;
	}

	if (*pc == _T('\0'))
	{
		ScrMode = SCRMODE::SMCONFIG;
		hWnd = NULL;
	}
	else
	{
		if (*pc == _T('-') || *pc == _T('/'))
		{
			pc++;
		}

		if (*pc == _T('s') || *pc == _T('S'))
		{
			ScrMode = SCRMODE::SMSAVER;
		}
		else
		{
			if (*pc == _T('p') || *pc == _T('P') ||
				*pc == _T('l') || *pc == _T('L'))
			{
				pc++;
				while (*pc == _T(' ') || *pc == _T(':'))
				{
					pc++;
				}

				ScrMode = SCRMODE::SMPREVIEW;
				hWnd = HWND(::_ttoi(pc));
			}
			else
			{
				if (*pc == _T('c') || *pc == _T('C'))
				{
					pc++;
					while (*pc == _T(' ') || *pc == _T(':'))
					{
						pc++;
					}

					ScrMode = SCRMODE::SMCONFIG;
					hWnd = (*pc == _T('\0')) ? ::GetForegroundWindow() : HWND(::_ttoi(pc));
				}
				else
				{
					if (*pc == _T('a') || *pc == _T('A'))
					{
						pc++;
						while (*pc == _T(' ') || *pc == _T(':'))
						{
							pc++;
						}

						ScrMode = SCRMODE::SMPASSWORD;
						hWnd = HWND(::_ttoi(pc));
					}
				}
			}
		}
	}

	return ScrMode;
}


//----------------------------------------------------------------------------------------------------------------------
void CKosletApp::DoConfig(HWND hWndParent) const
{
	CKosletDlg dlg;
	CWorldParams wp;
	wp.LoadFromRegistry();
	dlg.PutValues(wp);
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetValues(wp);
		wp.SaveToRegistry();
	}
}


//----------------------------------------------------------------------------------------------------------------------
void CKosletApp::DoChangePassword(HWND hWndParent) const
{
	// This only gets called on Windows 9x, when started with the /a option.
	const HINSTANCE hmpr = ::LoadLibrary(_T("MPR.DLL"));

	if (hmpr != NULL)
	{
		typedef VOID(WINAPI* PPWDCHANGEPASSWORD)
			(LPCSTR lpcRegkeyname, HWND hWnd,
				UINT uiReserved1, UINT uiReserved2);

		const PPWDCHANGEPASSWORD pPwdChangePassword =
			reinterpret_cast<PPWDCHANGEPASSWORD>
			(::GetProcAddress(hmpr, "PwdChangePasswordW"));

		if (pPwdChangePassword != NULL)
		{
			pPwdChangePassword("SCRSAVE", hWndParent, 0, 0);
		}
		else
		{
			TRACE(_T("PwdChangeProc not found: cannot change password.\n"));
		}

		VERIFY(::FreeLibrary(hmpr));
	}
	else
	{
		TRACE(_T("MPR.DLL not found: cannot change password.\n"));
	}
}

//#if !defined(_AFXDLL) && defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
//void AfxRegisterMFCCtrlClasses() {}
//#endif