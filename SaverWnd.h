// saverwnd.h : header file
//

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "DrawWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CSaverWnd window

class CSaverWnd : public CDrawWnd
{
// Construction
public:
	CSaverWnd();
	virtual ~CSaverWnd();

	BOOL Create();

protected:
	//{{AFX_MSG(CSaverWnd)
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnSetCursor(CWnd *pWnd, UINT nHitTest, UINT nMessage);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnActivate(UINT nState, CWnd *pWndOther, BOOL bMinimized);
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL m_bIsDialogActive;    // if Verify Password dialog box is active
	BOOL m_bReallyClose;    // for NT, if a WM_CLOSE came from us or it.

	CPoint m_ptInitCursorPos;    // cursor's position in screen coordinates
	DWORD m_dwInitTime;    // initial time in milliseconds

	DWORD m_dwPasswordDelay;    // Password Delay in seconds
	DWORD m_dwMouseThreshold;    // Mouse Threshold in pixels
	BOOL m_bMuteSound;    // Mute Sound

	void CloseSaverWindow();
	BOOL ExceedThreshold() const;
	void ReadGeneralRegistry();
	BOOL VerifyPassword(HWND hWndParent) const;
};

/////////////////////////////////////////////////////////////////////////////
