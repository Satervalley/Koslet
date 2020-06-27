// drawwnd.h : header file
//

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "../Dots/CShowWnd.h"
#include "CWorldParams.h"


/////////////////////////////////////////////////////////////////////////////
// CDrawWnd window

class CDrawWnd : public CShowWnd
{
// Construction
public:
	CDrawWnd(BOOL bAutoDelete = TRUE, BOOL bP = FALSE);
	virtual ~CDrawWnd();

	virtual BOOL Create(DWORD dwExStyle, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT &rect, CWnd *pWndParent, UINT nID = 0);
	void Start(void);

protected:
	virtual void PostNcDestroy();

	BOOL m_bAutoDelete;
	BOOL bPreview{ FALSE };
	CWorldParams wpParams;
	CString strSaveFileName;

	void GetSaveFileName(void);


	//{{AFX_MSG(CDrawWnd)
	//afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

/////////////////////////////////////////////////////////////////////////////
