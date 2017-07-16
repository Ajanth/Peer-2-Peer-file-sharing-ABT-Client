
// ABTDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ABT.h"
#include "ABTDlg.h"
#include "afxdialogex.h"
#include "ProxyDummy.h"
#include "SBDestination.h"
#include "ABTUI.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CListCtrlEx ABTUI::m_ctrlList;
int ABTUI::dcount;
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProxySettings)
    
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CABTDlg dialog



CABTDlg::CABTDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CABTDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CABTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CABTDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CABTDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_DestPath, &CABTDlg::OnBnClickedDestpath)
	ON_BN_CLICKED(IDC_StartDownload, &CABTDlg::OnBnClickedStartdownload)
	ON_BN_CLICKED(IDC_BUTTON1, &CABTDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK3, &CABTDlg::OnBnClickedOk3)
	ON_BN_CLICKED(IDCANCEL2, &CABTDlg::OnBnClickedCancel2)
END_MESSAGE_MAP()


// CABTDlg message handlers

BOOL CABTDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ABTUI::dcount++;
	prg_check=0;
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CABTDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CABTDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}
 
BOOL CABTDlg::BrowseForFolder(HWND hwnd, LPCTSTR szCurrent)
{
    BROWSEINFO   bi;
    LPITEMIDLIST pidl= NULL;
	LPMALLOC pMalloc = NULL;
	ZeroMemory(&bi,sizeof(BROWSEINFO));
    TCHAR    szDisplay[MAX_PATH];
    bi.hwndOwner      = hwnd;
    bi.pszDisplayName = szDisplay;
    bi.lpszTitle      = TEXT("Please choose a folder.");
    bi.ulFlags        = BIF_RETURNONLYFSDIRS;
    bi.lpfn           = NULL;
    pidl = SHBrowseForFolder(&bi);
 
    if (NULL != pidl)
    {
        retval = SHGetPathFromIDList(pidl, n_strFolderPath.GetBuffer(n_strFolderPath.GetLength()));
		if(SUCCEEDED(SHGetMalloc(&pMalloc)) && pMalloc);
        pMalloc->Free(pidl);  
        pMalloc->Release(); 
		
    }
    else
    {
        retval = FALSE;
    }
    return retval;
}
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CABTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CABTDlg::OnBnClickedButton2()//when select toorent button is clicked
{
	 CFileDialog Open_Torrent(true,NULL,NULL,OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST,_T("Torrent Files (*.torrent)|*.torrent||"));
	 iRet = Open_Torrent.DoModal();
	 l_strFileName= Open_Torrent.GetPathName();
	  if(iRet == IDOK)
	  {
		  CWnd* pWnd = GetDlgItem(IDC_EDIT1);
		  pWnd->SetWindowText(l_strFileName);
	  }
	 
}
void CABTDlg::OnBnClickedDestpath()
{
    CSBDestination SB;
	CString fl;
    CStdioFile f;

		SB.SetTitle( _T("Select Folder To Share") );
		SB.SetInitialSelection( _T("") );
		if ( TRUE == SB.SelectFolder() )
		{
			fl=SB.GetSelectedFolder();
			fl.Replace("\\","\\\\");		
			n_strFolderPath=fl;
			AfxMessageBox(fl);
			SetDlgItemText(IDC_EDIT2,fl);
		}
	 m_strDestPath=n_strFolderPath;

	
}


void CABTDlg::OnBnClickedStartdownload()
{
	CString str;
	GetDlgItem(IDC_EDIT1)->GetWindowText(str);
	UpdateData();
	if (str.IsEmpty())
	{
		AfxMessageBox( _T("Please select the Torrent file!") );
		return;
	}
	m_strTorrentFile = str;
	m_bInitContext	= TRUE;
	
	if ( !StartDownload() )
	{
        AfxMessageBox( _T("Failed to sart download, what to do???") );
		return;
	}
	this->ShowWindow(SW_HIDE);
	//CDialog::OnOK();

}
CString CABTDlg::get_myname()
{

CStdioFile f;
CString name;
if (f.Open("C:\\Windows\\ABTName.inf", CFile::modeReadWrite | CFile::shareDenyNone))
{
f.ReadString(name);
f.Close();
return name; 
}
return "Unknown User";
}
void CABTDlg::update_activity(CString str)
{
CStdioFile f;
CString name;
if (f.Open("C:\\ABT\\Activity.inf", CFile::modeReadWrite | CFile::shareDenyNone))
{
f.SeekToEnd();
f.WriteString(str);
f.WriteString("\n");
f.Close();
}
else
	AfxMessageBox("Error Accessing Configuration file(s).Unable to update activity");
}
BOOL CABTDlg::StartDownload()
{
	
    ASSERT( NULL == m_hDownloader );
    CString strPath;
    ::GetModuleFileName( AfxGetInstanceHandle(),
                         strPath.GetBuffer( _MAX_PATH ), _MAX_PATH );
    strPath.ReleaseBuffer();
	int nPos = strPath.ReverseFind( _T('\\') );
	ASSERT( -1 != nPos );
	CString strStartupPath = strPath.Left( nPos + 1 );
	
	HTorrentFile hTorrentFile = FTK_Torrent_Open( m_strTorrentFile, CP_ACP, NULL);
	if ( NULL == hTorrentFile )
	{
		AfxMessageBox( _T("torrent file null") );
		return FALSE;
	}
	CByteArray arryPreAllocFile;
	arryPreAllocFile.SetSize( FTK_Torrent_GetFilesCount( hTorrentFile ) );
	memset( arryPreAllocFile.GetData(), 0x00, arryPreAllocFile.GetSize() );
	nPos = m_strTorrentFile.ReverseFind( _T('\\') );
	ASSERT( -1 != nPos );
	CString strDestPath = m_strTorrentFile.Left( nPos + 1 );
	m_strKeyValue = FTK_Torrent_GetHexInfoHash( hTorrentFile );
	m_strKeyValue.Replace( _T("%"), _T("") );
	m_hDownloader = FTK_Downloader_Open();
	AfxMessageBox(m_strDestPath);
    BOOL bRet = FTK_Downloader_Init(
					m_hDownloader,
                    m_strTorrentFile,
					arryPreAllocFile.GetData(),
					arryPreAllocFile.GetSize(),
					FALSE,
                    m_strDestPath + _T("\\") + FTK_Torrent_GetTorrentName( hTorrentFile ),
                    strStartupPath + _T( "Config.INI" ),
                    strStartupPath + _T( "Log\\" ) + _T( "Downloader.log" ),
                    strStartupPath + m_strKeyValue + _T( ".status" ),
					NULL,
                    CP_ACP );

    if ( FALSE == bRet )
    {
        FTK_Downloader_Close( m_hDownloader );
        m_hDownloader = NULL;
		AfxMessageBox( _T("error line 303") );
        return FALSE;
    }

    bRet = FTK_Downloader_Execute( m_hDownloader );
    if ( FALSE == bRet )
    {
        FTK_Downloader_Close( m_hDownloader );
        m_hDownloader = NULL;
		AfxMessageBox( _T("error line 312") );
        return FALSE;
    }

    FTK_GlobalVar_AddTorrentSHA1( m_strKeyValue, m_hDownloader );
	m_qwTotalFileSize	= FTK_Torrent_GetFileSize( hTorrentFile );
	m_dwPieceCount	= FTK_Torrent_GetPieceCount( hTorrentFile );
	if(!ABTUI::add_download_queue(hTorrentFile, m_hDownloader))
		AfxMessageBox("error adding torrent to queue. startdownload line 305");

	tname=FTK_Torrent_GetTorrentName(hTorrentFile);
	wstr=" Downloading " +tname;
	update_activity(wstr);
	return TRUE;
}

/*void CBitZamDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
    CSliderCtrl * pSliderCtrl = ( CSliderCtrl * ) pScrollBar;
    int nCurrentPos = pSliderCtrl->GetPos();
    CString strValue;
    strValue.Format( _T( "%ld KB/s" ), nCurrentPos );

    if ( 1015 == pSliderCtrl->GetDlgCtrlID() )
    {
        double fMaxDownloadRate = ( double ) nCurrentPos * 1024.0;
        if ( nCurrentPos >= 8192 )
        {
            fMaxDownloadRate = 0.0;
            strValue.Format( _T( "%s" ), _T("Unlimited") );
        }

        SetDlgItemText( 1016, strValue );

		if ( NULL != m_hDownloader )
		{
			FTK_Downloader_SetFixMaxDownloadRate( m_hDownloader, fMaxDownloadRate );
		}
    }

    if ( 1027 == pSliderCtrl->GetDlgCtrlID() )
    {
        double fMaxUploadRate = ( double ) nCurrentPos * 1024.0;
        if ( nCurrentPos >= 4096 )
        {
            fMaxUploadRate = 0.0;
            strValue.Format( _T( "%s" ), _T("Unlimited") );
        }

        SetDlgItemText( 1028, strValue );

		if ( NULL != m_hDownloader )
		{
			FTK_Downloader_SetFixMaxUploadRate( m_hDownloader, fMaxUploadRate );
		}
    }*/



void CABTDlg::OnBnClickedButton1()//dialog box for proxy settings
{
	ProxyDummy obj;
	obj.OnProxySettings();
	if(IDOK != obj.DoModal())
		return;
    int nProxyType =obj.m_nProxyType;
    BOOL bAuthRequired = obj.m_bAuthRequired;
    CString strProxyServer = obj.m_strProxyServer;
    int nProxyPort = obj.m_nProxyPort;
    BOOL bUseOnlyHTTP = obj.m_bUseOnlyHTTP;
    CString strUserName = obj.m_strUserName;
    CString strPassword = obj.m_strPassword;
	if ( ( nProxyType > 0 && nProxyType <= 4 ) &&
		!strProxyServer.IsEmpty() &&
		( nProxyPort > 0 && nProxyPort <= 65535 ) )
	{
		bAuthRequired = bAuthRequired && ( nProxyType == PROXYTYPE_SOCKS5 || nProxyType == PROXYTYPE_HTTP11 );

		if ( bAuthRequired &&
			!strUserName.IsEmpty() &&
			!strPassword.IsEmpty() )
		{
			FTK_Proxy_SetAuthProxy( nProxyType, 
				strProxyServer, nProxyPort,
				strUserName, strPassword );
		}
		else
		{
			FTK_Proxy_SetProxy( nProxyType, 
				strProxyServer, nProxyPort );
		}

		FTK_Proxy_SetOnlyTracker( bUseOnlyHTTP );
	}
	else
	{
		FTK_Proxy_SetType( PROXYTYPE_NOPROXY );
		AfxMessageBox( _T("line 80") );
	}
}


void CABTDlg::OnBnClickedOk3()
{
	// TODO: Add your control notification handler code here
}


void CABTDlg::OnBnClickedCancel2()//when cancel button is clicked
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
