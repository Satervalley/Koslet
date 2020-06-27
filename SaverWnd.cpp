// saverwnd.cpp : implementation file
//

#include "pch.h"
#include <regstr.h>
#include <VersionHelpers.h>
#include "Koslet.h"
#include "DrawWnd.h"
#include "SaverWnd.h"
#include "TextArchiver.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


//-----------------------------------------------------------------------------
#define    REGSTR_PATH_PLUSSCR        REGSTR_PATH_SETUP _T("\\Screen Savers")
//-----------------------------------------------------------------------------



/////////////////////////////////////////////////////////////////////////////
// CSaverWnd
//-----------------------------------------------------------------------------
CSaverWnd::CSaverWnd()
{
}


//-----------------------------------------------------------------------------
CSaverWnd::~CSaverWnd()
{
}


//-----------------------------------------------------------------------------
BOOL CSaverWnd::Create()
{
	CRect rect;

	if (::GetSystemMetrics(SM_CMONITORS) == 0)
	{
		// Multiple display monitors not supported.
		rect.SetRect(0, 0,
					 ::GetSystemMetrics(SM_CXSCREEN),
					 ::GetSystemMetrics(SM_CYSCREEN));
	}
	else
	{
		// Multiple display monitors supported.
		const int nLeft = ::GetSystemMetrics(SM_XVIRTUALSCREEN);
		const int nTop = ::GetSystemMetrics(SM_YVIRTUALSCREEN);
		const int nRight = nLeft + ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
		const int nBottom = nTop + ::GetSystemMetrics(SM_CYVIRTUALSCREEN);

		rect.SetRect(nLeft, nTop, nRight, nBottom);
	}

	return this->CDrawWnd::Create(WS_EX_TOPMOST,
								  ::AfxGetAppName(),
								  WS_VISIBLE | WS_POPUP,
								  rect,
								  NULL);
}


//-----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CSaverWnd, CDrawWnd)
	//{{AFX_MSG_MAP(CSaverWnd)
	ON_WM_SYSCOMMAND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
	ON_WM_NCACTIVATE()
	ON_WM_ACTIVATE()
	ON_WM_ACTIVATEAPP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_SYSKEYDOWN()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//-----------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
// CSaverWnd message handlers
//-----------------------------------------------------------------------------
void CSaverWnd::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_SCREENSAVE || nID == SC_CLOSE)
	{
		return;
	}

	this->CDrawWnd::OnSysCommand(nID, lParam);
}


//-----------------------------------------------------------------------------
int CSaverWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_bIsDialogActive = FALSE;
	m_bReallyClose = FALSE;

	if (this->CDrawWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// disable system keys: CTRL-ALT-Delete and ALT-Tab and the Windows key
	UINT nOldVal;
	::SystemParametersInfo(SPI_SCREENSAVERRUNNING, 1 , &nOldVal, 0);

	VERIFY(::GetCursorPos(&m_ptInitCursorPos));
	m_dwInitTime = ::GetTickCount();

	this->ReadGeneralRegistry();

	return 0;
}


//-----------------------------------------------------------------------------
void CSaverWnd::OnDestroy()
{
	// restore system keys: CTRL-ALT-Delete and ALT-Tab and the Windows key
	UINT nOldVal;
	::SystemParametersInfo(SPI_SCREENSAVERRUNNING, 0, &nOldVal, 0);

	::PostQuitMessage(0);

	this->CDrawWnd::OnDestroy();
}


//-----------------------------------------------------------------------------
BOOL CSaverWnd::OnSetCursor(CWnd * /*pWnd*/,
							UINT /*nHitTest*/,
							UINT /*nMessage*/)
{
	::SetCursor(m_bIsDialogActive
				? ::AfxGetApp()->LoadStandardCursor(IDC_ARROW)
				: NULL);

	return TRUE;
}


//-----------------------------------------------------------------------------
BOOL CSaverWnd::OnNcActivate(BOOL bActive)
{
	if (!m_bIsDialogActive)
	{
		if (!bActive)
		{
			return FALSE;
		}
	}

	return this->CDrawWnd::OnNcActivate(bActive);
}


//-----------------------------------------------------------------------------
void CSaverWnd::OnActivate(UINT nState, CWnd *pWndOther, BOOL bMinimized)
{
	if (!m_bIsDialogActive)
	{
		if (nState == WA_INACTIVE)
		{
			this->CloseSaverWindow();
		}
	}

	this->CDrawWnd::OnActivate(nState, pWndOther, bMinimized);
}


//-----------------------------------------------------------------------------
void CSaverWnd::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	if (!m_bIsDialogActive)
	{
		if (!bActive)
		{
			this->CloseSaverWindow();
		}
	}
	
	this->CDrawWnd::OnActivateApp(bActive, dwThreadID);
}


//-----------------------------------------------------------------------------
void CSaverWnd::OnMouseMove(UINT nFlags, CPoint point)
{
/*
	if (!m_bIsDialogActive)
	{
        if (this->ExceedThreshold())
		{
			this->CloseSaverWindow();
		}
	}
*/
	this->CDrawWnd::OnMouseMove(nFlags, point);
}


//-----------------------------------------------------------------------------
void CSaverWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bIsDialogActive)
	{
		this->CloseSaverWindow();
	}

	this->CDrawWnd::OnLButtonDown(nFlags, point);
}


//-----------------------------------------------------------------------------
void CSaverWnd::OnMButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bIsDialogActive)
	{
		this->CloseSaverWindow();
	}

	this->CDrawWnd::OnMButtonDown(nFlags, point);
}


//-----------------------------------------------------------------------------
void CSaverWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bIsDialogActive)
	{
		this->CloseSaverWindow();
	}

	this->CDrawWnd::OnRButtonDown(nFlags, point);
}


//-----------------------------------------------------------------------------
void CSaverWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (!m_bIsDialogActive)
	{
		if (nChar == VK_TAB)	// switch OSD
		{
			bOSD = !bOSD;
			wpParams.dwOSD = bOSD;
		}
		else
			this->CloseSaverWindow();
	}

	this->CDrawWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}


//-----------------------------------------------------------------------------
void CSaverWnd::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (!m_bIsDialogActive)
	{
		this->CloseSaverWindow();
	}

	this->CDrawWnd::OnSysKeyDown(nChar, nRepCnt, nFlags);
}


//-----------------------------------------------------------------------------
void CSaverWnd::OnClose()
{
	if (!m_bReallyClose)
	{
		return;
	}

	if (!m_bIsDialogActive)
	{
		BOOL bCanClose = TRUE;

		if (::GetTickCount() - m_dwInitTime > m_dwPasswordDelay * 1000)
		{
			m_bIsDialogActive = TRUE;

			bCanClose = this->VerifyPassword(this->GetSafeHwnd());

			m_bIsDialogActive = FALSE;
			VERIFY(::GetCursorPos(&m_ptInitCursorPos));
		}

		if (!bCanClose)
		{
			return;
		}
	}
	
	if (!bPreview)
	{
		wpParams.SaveToRegistry();
		if (wpParams.dwSaveSession)
		{
			GetSaveFileName();
			CTextArchiver ta;
			ta.Save(strSaveFileName, *this);
		}
	}

	this->CDrawWnd::OnClose();
}


//-----------------------------------------------------------------------------
void CSaverWnd::CloseSaverWindow()
{
	m_bReallyClose = TRUE;
	VERIFY(this->PostMessage(WM_CLOSE));
}


//-----------------------------------------------------------------------------
BOOL CSaverWnd::ExceedThreshold() const
{
	CPoint pt;
	VERIFY(::GetCursorPos(&pt));

	CSize szDxy = pt - m_ptInitCursorPos;
	if (szDxy.cx < 0)
	{
		szDxy.cx = -szDxy.cx;
	}
	if (szDxy.cy < 0)
	{
		szDxy.cy = -szDxy.cy;
	}

    return szDxy.cx > static_cast<int>(m_dwMouseThreshold) ||
		   szDxy.cy > static_cast<int>(m_dwMouseThreshold);
}


//-----------------------------------------------------------------------------
void CSaverWnd::ReadGeneralRegistry()
{
	// default values in case they're not in registry
	m_dwPasswordDelay = 0;
	m_dwMouseThreshold = 3;
	m_bMuteSound = FALSE;

	HKEY hKey = NULL;
	LONG lRes = ::RegOpenKeyEx(HKEY_CURRENT_USER,
							   REGSTR_PATH_PLUSSCR,
							   0,
							   KEY_QUERY_VALUE,
							   &hKey);
	if (lRes == ERROR_SUCCESS)
	{
		DWORD dwValType = 0;

		DWORD dwVal = 0;

		DWORD dwValSize = sizeof(dwVal);
		lRes = ::RegQueryValueEx(hKey,
								  _T("Password Delay"),
								  NULL,
								  &dwValType,
								  reinterpret_cast<LPBYTE>(&dwVal),
								  &dwValSize);
		if (lRes == ERROR_SUCCESS)
		{
			ASSERT(dwValType == REG_DWORD);
			m_dwPasswordDelay = dwVal;
		}

		dwValSize = sizeof(dwVal);
		lRes = ::RegQueryValueEx(hKey,
								 _T("Mouse Threshold"),
								 NULL,
								 &dwValType,
								 reinterpret_cast<LPBYTE>(&dwVal),
								 &dwValSize);
		if (lRes == ERROR_SUCCESS)
		{
			ASSERT(dwValType == REG_DWORD);
			m_dwMouseThreshold = dwVal;
		}

		dwValSize = sizeof(dwVal);
		lRes = ::RegQueryValueEx(hKey,
								 _T("Mute Sound"),
								 NULL,
								 &dwValType,
								 reinterpret_cast<LPBYTE>(&dwVal),
								 &dwValSize);
		if (lRes == ERROR_SUCCESS)
		{
			ASSERT(dwValType == REG_DWORD);
			m_bMuteSound = dwVal;
		}

		VERIFY(::RegCloseKey(hKey) == ERROR_SUCCESS);
	}
}


//-----------------------------------------------------------------------------
BOOL CSaverWnd::VerifyPassword(HWND hWndParent) const
{
	// On Windows NT, we return TRUE immediately. This lets the saver quit,
	// and the system manages passwords.
	// On Windows 9x, we call VerifyScreenSavePwd. This checks the appropriate
	// registry key and, if necessary, pops up a verify dialog.
	//OSVERSIONINFO osv = { sizeof(OSVERSIONINFO) };
	//VERIFY(::GetVersionEx(&osv));
	if (::IsWindows7OrGreater()/*osv.dwPlatformId == VER_PLATFORM_WIN32_NT*/)
	{
		return TRUE;
	}

	BOOL bRes = TRUE;

	const HINSTANCE hpwdcpl = ::LoadLibrary(_T("PASSWORD.CPL"));

	if (hpwdcpl != NULL)
	{
		typedef BOOL (WINAPI *PVERIFYSCREENSAVEPWD)(HWND hWnd);
		
		const PVERIFYSCREENSAVEPWD pVerifyScreenSavePwd =
			reinterpret_cast<PVERIFYSCREENSAVEPWD>
				(::GetProcAddress(hpwdcpl, "VerifyScreenSavePwd"));
		
		if (pVerifyScreenSavePwd != NULL)
		{
			TRACE(_T("About to call VerifyPwProc\n"));
			bRes = pVerifyScreenSavePwd(hWndParent);
		}
		else
		{
			TRACE(_T("Unable to get VerifyPwProc address. Aborting\n"));
		}

		VERIFY(::FreeLibrary(hpwdcpl));
	}
	else
	{
		TRACE(_T("Unable to load PASSWORD.CPL. Aborting\n"));
	}

	return bRes;
}


