
// KosletDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Koslet.h"
#include "KosletDlg.h"
#include "XHTMLStatic.h"
#include "Koslet_av.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CXHTMLStatic htmlContent;
	const TCHAR* htmlStr = {
							_T("<b><font size=\"+8\" color=\"#ff8000\"><center>Koslet</center></font></b><br><br>")
							_T("<font size=\"+6\" color=\"#a08060\"><center>A simple N-Body simulation screensaver</center></font><br>")
							_T("<font color=\"#C6D6FD\"><hr size=2></font><br>")
							_T("<font color=\"#408010\"><center>%s</center></font><br>")
							_T("<center>Copyright&copy; 2020 - %d Darquid studio</center>")
							_T("<br><b><center>All rights reserved</center></b><br><br>")
//							_T("<br><br>Kamac is open source software.    ")
//		_T("<a href=\"https://github.com/Satervalley/Kamac-git\"><font color=\"blue\"><u>source code</u></font></a>    ")
		_T("<a href=\"mailto:darquid@126.com\"><font color=\"blue\"><u>contact me</u></font></a>    ")
//		_T("<a href=\"https://github.com/Satervalley/Kamac-git/blob/master/donate.md\"><font color=\"blue\"><u>donate</u></font></a>")
	};

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog()
	{
		CDialog::OnInitDialog();

		htmlContent.ModifyStyleEx(WS_EX_TRANSPARENT, 0);
		CString str, strVer;
		strVer.Format(_T("Version %d.%02d Build %d.%d on %d.%02d.%02d"), AutoVersion::nMajor, AutoVersion::nMinor,
			AutoVersion::nBuild, AutoVersion::nRevision, AutoVersion::nYear, AutoVersion::nMonth, AutoVersion::nDay);
		str.Format(htmlStr, strVer, AutoVersion::nYear);
		htmlContent.SetWindowText(str);

		return TRUE;  // return TRUE unless you set the focus to a control
					  // EXCEPTION: OCX Property Pages should return FALSE
	}
};

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CONTENT, htmlContent);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CKosletDlg dialog



CKosletDlg::CKosletDlg(CWnd* pParent /*=nullptr*/) : CDialog(IDD_KOSLET_DIALOG, pParent)
, nG(0)
, nCommon(0)
, nGiantStar(0)
, nBlackhole(0)
, nAntiGCommon(0)
, nAntiGGiantStar(0)
, nAntiGBlackhole(0)
, bSaveSession(FALSE)
, bOSD(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKosletDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_G, nG);
	DDV_MinMaxInt(pDX, nG, 1, 4000);
	DDX_Text(pDX, IDC_EDIT_COMMON, nCommon);
	DDV_MinMaxInt(pDX, nCommon, 0, 5000);
	DDX_Text(pDX, IDC_EDIT_GIANTSTAR, nGiantStar);
	DDV_MinMaxInt(pDX, nGiantStar, 0, 256);
	DDX_Text(pDX, IDC_EDIT_BLACKHOLE, nBlackhole);
	DDV_MinMaxInt(pDX, nBlackhole, 0, 16);
	DDX_Text(pDX, IDC_EDIT_ANTIG_COMMON, nAntiGCommon);
	DDV_MinMaxInt(pDX, nAntiGCommon, 0, 5000);
	DDX_Text(pDX, IDC_EDIT_ANTIG_GIANTSTAR, nAntiGGiantStar);
	DDV_MinMaxInt(pDX, nAntiGGiantStar, 0, 256);
	DDX_Text(pDX, IDC_EDIT_ANTIG_BLACKHOLE, nAntiGBlackhole);
	DDV_MinMaxInt(pDX, nAntiGBlackhole, 0, 16);
	DDX_Control(pDX, IDC_COMBO_TRAIL, cmbTrail);
	DDX_Control(pDX, IDC_COMBO_EXPLOSION, cmbExplosion);
	DDX_Check(pDX, IDC_CHECK_SAVEANDRESTORE, bSaveSession);
	DDX_Check(pDX, IDC_CHECK_OSD, bOSD);
	DDX_Control(pDX, IDC_COMBO_STEPLENGTH, cmbStepLength);
}

BEGIN_MESSAGE_MAP(CKosletDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, &CKosletDlg::OnClickedButtonAbout)
END_MESSAGE_MAP()


// CKosletDlg message handlers
BOOL CKosletDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	UpdateData(FALSE);
	cmbStepLength.SetCurSel(nStepLength);
	cmbTrail.SetCurSel(nTrail);
	cmbExplosion.SetCurSel(nExplosion);

	return TRUE;  // return TRUE  unless you set the focus to a control
}


//----------------------------------------------------------------------------------------------------------------------
void CKosletDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CKosletDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}


// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CKosletDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//----------------------------------------------------------------------------------------------------------------------
void CKosletDlg::OnClickedButtonAbout()
{
	CAboutDlg dlg;

	dlg.DoModal();
}


//----------------------------------------------------------------------------------------------------------------------
void CKosletDlg::PutValues(CWorldParams& wp)
{
	wp.Normalize();
	nG = (int)wp.dwG;
	nStepLength = wp.dwStepLength;
	nCommon = (int)wp.dwCommon;
	nGiantStar = (int)wp.dwGiantStar;
	nBlackhole = (int)wp.dwBlackhole;
	nAntiGCommon = (int)wp.dwAntiGCommon;
	nAntiGGiantStar = (int)wp.dwAntiGGiantStar;
	nAntiGBlackhole = (int)wp.dwAntiGBlackhole;
	nTrail = (int)wp.dwTrail;
	nExplosion = (int)wp.dwExplosion;
	bSaveSession = (BOOL)wp.dwSaveSession;
	bOSD = (BOOL)wp.dwOSD;
}


//----------------------------------------------------------------------------------------------------------------------
void CKosletDlg::GetValues(CWorldParams& wp)
{
	wp.dwG = nG;
	wp.dwStepLength = nStepLength;
	wp.dwCommon = nCommon;
	wp.dwGiantStar = nGiantStar;
	wp.dwBlackhole = nBlackhole;
	wp.dwAntiGCommon = nAntiGCommon;
	wp.dwAntiGGiantStar = nAntiGGiantStar;
	wp.dwAntiGBlackhole = nAntiGBlackhole;
	wp.dwTrail = nTrail;
	wp.dwExplosion = nExplosion;
	wp.dwSaveSession = bSaveSession;
	wp.dwOSD = bOSD;
	wp.Normalize();
}


//----------------------------------------------------------------------------------------------------------------------
void CKosletDlg::OnOK()
{
	UpdateData();

	nStepLength = cmbStepLength.GetCurSel();
	nTrail = cmbTrail.GetCurSel();
	nExplosion = cmbExplosion.GetCurSel();

	CDialog::OnOK();
}



