
// Koslet.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CKosletApp:
// See Koslet.cpp for the implementation of this class
//
enum class SCRMODE
{
	SMNONE, SMCONFIG, SMPASSWORD, SMPREVIEW, SMSAVER
};


class CKosletApp : public CWinApp
{
public:
	CKosletApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()

protected:
	//enum SCRMODE { SMNONE, SMCONFIG, SMPASSWORD, SMPREVIEW, SMSAVER };

	SCRMODE GetScrMode(HWND& hWnd) const;

	void DoConfig(HWND hWndParent) const;
	void DoChangePassword(HWND hWndParent) const;

};

extern CKosletApp theApp;
