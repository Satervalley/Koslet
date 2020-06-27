
// KosletDlg.h : header file
//

#pragma once

#include "CWorldParams.h"


// CKosletDlg dialog
class CKosletDlg : public CDialog
{
// Construction
public:
	CKosletDlg(CWnd* pParent = nullptr);	// standard constructor

	void PutValues(CWorldParams& wp);
	void GetValues(CWorldParams& wp);
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KOSLET_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	int nG;
	int nStepLength{ 2 };
	int nCommon;
	int nGiantStar;
	int nBlackhole;
	int nAntiGCommon;
	int nAntiGGiantStar;
	int nAntiGBlackhole;
	CComboBox cmbTrail;
	CComboBox cmbExplosion;
	int nTrail, nExplosion;
	BOOL bSaveSession;
	BOOL bOSD;

	afx_msg void OnClickedButtonAbout();
	virtual void OnOK();
	CComboBox cmbStepLength;
};
