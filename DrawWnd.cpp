// drawwnd.cpp : implementation file
//

#include "pch.h"
#include "Koslet.h"
#include "DrawWnd.h"
#include "TextArchiver.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


// CDrawWnd
//-----------------------------------------------------------------------------
CDrawWnd::CDrawWnd(BOOL bAutoDelete /* = TRUE */, BOOL bP/* = FALSE*/)
{
	m_bAutoDelete = bAutoDelete;
	bPreview = bP;
}


//-----------------------------------------------------------------------------
CDrawWnd::~CDrawWnd()
{
}


//-----------------------------------------------------------------------------
BOOL CDrawWnd::Create(DWORD dwExStyle, LPCTSTR lpszWindowName, DWORD dwStyle,
					  const RECT &rect, CWnd *pWndParent, UINT nID /* = 0 */)
{
	BOOL b = this->CreateEx(dwExStyle,
						  ::AfxRegisterWndClass(CS_OWNDC/*CS_HREDRAW | CS_VREDRAW,
												NULL,
							static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH)),
										::AfxGetApp()->LoadIcon(IDR_MAINFRAME)*/),
						  lpszWindowName, dwStyle,
						  rect, pWndParent, nID);
	//EnableD2DSupport();
	if(b)
		SetTimer(1u, 500u, nullptr);
	return b;
}


//-----------------------------------------------------------------------------
void CDrawWnd::PostNcDestroy()
{
	if (m_bAutoDelete)
	{
		delete this;
	}
}


//-----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CDrawWnd, CShowWnd)
	//{{AFX_MSG_MAP(CDrawWnd)
	//ON_WM_PAINT()
	//}}AFX_MSG_MAP
	//ON_REGISTERED_MESSAGE(AFX_WM_DRAW2D, &CDrawWnd::OnDraw2d)
	//ON_MESSAGE(WM_USER_DRAW_WORLD2, &CDrawWnd::OnUserDrawWorld2)
	ON_WM_TIMER()
END_MESSAGE_MAP()


//-----------------------------------------------------------------------------
/*
void CDrawWnd::OnPaint()
{
	static bool b{ false };
	if (!b)
	{
		b = true;
		Start();
	}
//	CPaintDC dc(this);

	// Add your drawing code here.
}
*/


//----------------------------------------------------------------------------------------------------------------------
void CDrawWnd::Start(void)
{
	int ntc = 1;
	if (!wpParams.LoadFromRegistry())
		wpParams.SetDefault();
	if (bPreview)
	{
		wpParams.MakePreviewValues();
		InitWorldRaw(1, wpParams.dwG, wpParams.dwCommon, wpParams.dwGiantStar, wpParams.dwBlackhole,
			wpParams.dwAntiGCommon, wpParams.dwAntiGGiantStar, wpParams.dwAntiGBlackhole);
		CRect rect;
		GetClientRect(&rect);
		pWorld->SetConstraintRect(rect, 8);
		bOSD = false;
	}
	else // not preview
	{
		ntc = wpParams.GetThreadCount();
		if (wpParams.dwSaveSession) // try to load previous session
		{
			GetSaveFileName();
			CTextArchiver ta;
			if (!ta.Load(strSaveFileName, *this, wpParams, GetRenderTarget()))
			{
				InitWorldRaw(ntc, wpParams.dwG, wpParams.dwCommon, wpParams.dwGiantStar, wpParams.dwBlackhole,
					wpParams.dwAntiGCommon, wpParams.dwAntiGGiantStar, wpParams.dwAntiGBlackhole);
			}
			else // load saved data, continue session
			{
				CRect rect;
				GetClientRect(&rect);
				int nw = rect.Width();
				int nh = rect.Height();
				pWorld->Init(nw, nh, wpParams.Total(), m_hWnd);
				pWorld->SetConstraintRect(rect, int(::powf(pWorld->rdRawData.fG, 0.25) * 6.f));
			}
		}
		else // start new session
		{
			InitWorldRaw(ntc, wpParams.dwG, wpParams.dwCommon, wpParams.dwGiantStar, wpParams.dwBlackhole,
				wpParams.dwAntiGCommon, wpParams.dwAntiGGiantStar, wpParams.dwAntiGBlackhole);
		}
		bOSD = wpParams.dwOSD;
	}
	SThreadParams::bUsingSIMD = wpParams.SIMD();
	SThreadParams::SetStepLength(wpParams.dwStepLength);
	pWorld->SetEventFrequency(wpParams.dwTrail, wpParams.dwExplosion);
	if (ntc > 1)
		RunMultiThread(ntc);
	else
		Run();
}


//-----------------------------------------------------------------------------
void CDrawWnd::GetSaveFileName(void)
{
	CWinApp* pApp = ::AfxGetApp();
	CString str;
	::GetModuleFileName(pApp->m_hInstance, str.GetBufferSetLength(1025), 1025);
	str.ReleaseBuffer();
	int nPos = str.ReverseFind(L'.');
	if (nPos >= 0)
	{
		str = str.Left(nPos + 1);
	}
	else
		str += _T(".");
	strSaveFileName = str + _T("sav");
}


//----------------------------------------------------------------------------------------------------------------------
void CDrawWnd::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(1u);
	Start();
}
