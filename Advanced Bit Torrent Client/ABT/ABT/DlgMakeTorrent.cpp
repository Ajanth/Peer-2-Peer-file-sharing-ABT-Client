

#include "stdafx.h"
#include "ABT.h"
#include "DlgMakeTorrent.h"
#include "SBDestination.h"
#include "afxdialogex.h"
#include "DlgMTProgress.h"
#include "VCheck.h"
#include "sys/stat.h"

IMPLEMENT_DYNAMIC(CDlgMakeTorrent, CDialog)
CString CDlgMakeTorrent::vpath;
CString CDlgMakeTorrent::cinfo;
CStringArray CDlgMakeTorrent::m_arryCodePages;
CString CDlgMakeTorrent::fname;
CString CDlgMakeTorrent::fsize;
CString CDlgMakeTorrent::x;
CString CDlgMakeTorrent::y;
CString CDlgMakeTorrent::z;
CString CDlgMakeTorrent::m_strMainAnnounce;
CString CDlgMakeTorrent::m_strOtherAnnounces;
CString CDlgMakeTorrent::m_strComments;
CString CDlgMakeTorrent::m_strDestTorrentName;
CString CDlgMakeTorrent::m_strEncryptPwd;
CString CDlgMakeTorrent::m_strSourceFilePath;
int CDlgMakeTorrent::m_nSelPieceSize = 0;
BOOL CDlgMakeTorrent::m_bIsFile	= 0;
UINT CDlgMakeTorrent::m_nCodePage	= CP_ACP;
CDlgMakeTorrent::CDlgMakeTorrent(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMakeTorrent::IDD, pParent)
	, Cname(_T(""))
	, T_year(_T(""))
	, version(_T(""))
{
m_nFileOrDir = 0;
	m_strMainTracker = _T("");
	m_strOtherTracker = _T("");
	m_strNotes = _T("");
	m_strTorrentName = _T("");
	m_bEncrypt = FALSE;
	m_strPassword = _T("");
	m_strSrcPathFile = _T("");
	m_nPieceSize = 0;
	//}}AFX_DATA_INIT

	m_nCodePage = CP_ACP;
}

CDlgMakeTorrent::~CDlgMakeTorrent()
{
}

void CDlgMakeTorrent::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, 1018, m_wndCodePage);
	DDX_Radio(pDX, 1006, m_nFileOrDir);
	DDX_Text(pDX, 1015, m_strMainTracker);
	DDX_Text(pDX, 1008, m_strOtherTracker);
	//DDX_Text(pDX, 1009, m_strNotes);
	DDX_Text(pDX, 1010, m_strTorrentName);
	DDX_Check(pDX, 1012, m_bEncrypt);
	DDX_Text(pDX, 1013, m_strPassword);
	DDX_Text(pDX, 1004, m_strSrcPathFile);
	DDX_CBIndex(pDX, 10117, m_nPieceSize);
	DDX_Control(pDX, IDC_TYPE, CMB_type);
	DDX_Control(pDX, IDC_GENRE, CMB_genre);
	DDX_Control(pDX, 10117, CMB_psize);
	DDX_Text(pDX, IDC_NAME, Cname);
	DDX_Text(pDX, IDC_YEAR, T_year);
	DDX_Text(pDX, IDC_VER, version);
}


BEGIN_MESSAGE_MAP(CDlgMakeTorrent, CDialog)
	ON_BN_CLICKED(1005, &CDlgMakeTorrent::OnBnClicked1005)
	ON_BN_CLICKED(1050, &CDlgMakeTorrent::OnBnClicked1050)
	ON_BN_CLICKED(1001, &CDlgMakeTorrent::OnBnClicked1001)
	ON_BN_CLICKED(IDOK, &CDlgMakeTorrent::OnBnClickedOk)
	ON_EN_CHANGE(1015, &CDlgMakeTorrent::OnEnChange1015)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgMakeTorrent::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BACK, &CDlgMakeTorrent::OnBnClickedBack)
	ON_CBN_SELCHANGE(IDC_TYPE, &CDlgMakeTorrent::OnCbnSelchangeType)
	ON_STN_CLICKED(IDC_T5, &CDlgMakeTorrent::OnStnClickedT5)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgMakeTorrent message handlers


void CDlgMakeTorrent::OnBnClicked1005()//onselfileordir function in original
{
UpdateData();

	if ( 0 == m_nFileOrDir )
	{
		CString strFilter = _T( "Any File(*.*)|*.*||" );
		CFileDialog OpenDlg( TRUE, _T( "*" ), _T( "*.*" ),
							 OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_FILEMUSTEXIST,
							 strFilter );

		if ( IDOK != OpenDlg.DoModal() )
		{
			return ;
		}

		SetDlgItemText( 1004, OpenDlg.GetPathName() );
		SetDlgItemText( 1010, OpenDlg.GetPathName() + _T(".Torrent") );
		
	}
	else
	{
		CSBDestination SB;
		SB.SetTitle( _T("Select Folder") );
		SB.SetInitialSelection( _T("") );
		if ( TRUE == SB.SelectFolder() )
		{
			SetDlgItemText( 1004, SB.GetSelectedFolder() );
			SetDlgItemText( 1010, SB.GetSelectedFolder() + _T(".Torrent") );
			//SetDlgItemText(IDC_REPORT,"SCANNING YOUR FOLDER");
		//CString op=ExecCmd("avgscanx.exe /SCAN=D:\\audacity.exe");
//		SetDlgItemText(IDC_REPORT,op);
		}
	}
}
void CDlgMakeTorrent::show_two()
{
	GetDlgItem(1015)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_1015)->ShowWindow(SW_SHOW);
	GetDlgItem(1008)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_1008)->ShowWindow(SW_SHOW);
	GetDlgItem(10118)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_10118)->ShowWindow(SW_HIDE);
	//GetDlgItem(1012)->ShowWindow(SW_SHOW);
	//GetDlgItem(IDC_1012)->ShowWindow(SW_SHOW);
	GetDlgItem(10117)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_10117)->ShowWindow(SW_SHOW);
	//GetDlgItem(1013)->ShowWindow(SW_SHOW);
	//GetDlgItem(IDC_1013)->ShowWindow(SW_SHOW);
	
	GetDlgItem(1004)->ShowWindow(SW_HIDE);
	GetDlgItem(1005)->ShowWindow(SW_HIDE);
	//GetDlgItem(1007)->ShowWindow(SW_HIDE);
	//GetDlgItem(1006)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_1007)->ShowWindow(SW_HIDE);
	GetDlgItem(1010)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_1010)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_T1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_T2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_T3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_T4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_T5)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TYPE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_GENRE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NAME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SIZE)->ShowWindow(SW_HIDE);
	GetDlgItem(1050)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_GP1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_T6)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_VER)->ShowWindow(SW_HIDE);
	showing=!showing;
}
void CDlgMakeTorrent::show_one()
{
	GetDlgItem(1015)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_1015)->ShowWindow(SW_HIDE);
	GetDlgItem(1008)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_1008)->ShowWindow(SW_HIDE);
	GetDlgItem(10118)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_10118)->ShowWindow(SW_HIDE);
	//GetDlgItem(1012)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_1012)->ShowWindow(SW_HIDE);
	GetDlgItem(10117)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_10117)->ShowWindow(SW_HIDE);
	//GetDlgItem(1013)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_1013)->ShowWindow(SW_HIDE);
	
	GetDlgItem(1004)->ShowWindow(SW_SHOW);
	GetDlgItem(1005)->ShowWindow(SW_SHOW);
	//GetDlgItem(1007)->ShowWindow(SW_SHOW);
	//GetDlgItem(1006)->ShowWindow(SW_SHOW);
	//GetDlgItem(IDC_1007)->ShowWindow(SW_SHOW);
	GetDlgItem(1010)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_1010)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_T1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_T2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_T3)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_T4)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_T5)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_TYPE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_GENRE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_YEAR)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_NAME)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_SIZE)->ShowWindow(SW_SHOW);
	GetDlgItem(1050)->ShowWindow(SW_SHOW);
	//GetDlgItem(IDC_GP1)->ShowWindow(SW_SHOW);
	
	showing=!showing;
}
BOOL CALLBACK CDlgMakeTorrent::EnumCodePageProc( LPTSTR lpCodePageString )
{
    m_arryCodePages.Add( lpCodePageString );
    return TRUE;
}
int CDlgMakeTorrent::SelectCodePage( CComboBox &wndComboBox, UINT nCodePage )
{
    int nIndex = -1;
    for ( register int i = 0; i < wndComboBox.GetCount(); i++ )
    {
        if ( nCodePage == wndComboBox.GetItemData( i ) )
        {
            nIndex = i;
            break;
        }
    }

    return nIndex;
}
BOOL CDlgMakeTorrent::OnInitDialog() 
{
	CDialog::OnInitDialog();
	showing=false;
	vcheck=false;
	isver=false;
	Bacground.LoadBitmap(IDB_BITMAP16);
	Bacground.GetBitmap(&bm);
	bitmapSize = CSize(bm.bmWidth, bm.bmHeight);
	//Invalidate(1);

	CMB_type.AddString("Movies");
	CMB_type.AddString("Other");
	CMB_type.AddString("Songs");
	CMB_type.AddString("Softwares");
	
	
	CMB_psize.AddString("32");
	CMB_psize.AddString("64");
	CMB_psize.AddString("128");
	CMB_psize.AddString("256");
	CMB_psize.AddString("512");
	// TODO: Add extra initialization here
	show_one();
    EnumSystemCodePages(EnumCodePageProc, CP_INSTALLED );
	
    UINT nCodePage = CP_ACP;
    CPINFOEX CpinfoEX;
    int nIndex = -1;

    for ( register int i = 0; i < m_arryCodePages.GetSize(); i++ )
    {
        nCodePage = _ttoi( m_arryCodePages[ i ] );
        if ( GetCPInfoEx( nCodePage, 0, &CpinfoEX ) )
        {
            nIndex = m_wndCodePage.AddString( CpinfoEX.CodePageName );
            m_wndCodePage.SetItemData( nIndex, nCodePage );
        }
    }

    nIndex = SelectCodePage( m_wndCodePage, GetACP() );
    if ( -1 != nIndex )
    {
        m_wndCodePage.SetCurSel( nIndex );
    }
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgMakeTorrent::OnBnClicked1050()//onseltorrentfilename
{
	CString strFilter = _T( "Torrent File(*.torrent)|*.torrent||" );
    CFileDialog SaveDlg( FALSE, _T( "torrent" ), _T( "*.torrent" ),
                         OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER, strFilter );

    if ( IDOK != SaveDlg.DoModal() )
    {
        return ;
    }

    SetDlgItemText( 1010, SaveDlg.GetPathName() );
}
void CDlgMakeTorrent::OnStartMakeTorrent() //function not used
{
	// TODO: Add your control notification handler code here
	AfxMessageBox("function called oh ");
	UpdateData();
	
	if ( m_strSrcPathFile.IsEmpty() )
	{
		AfxMessageBox( _T("Please select source file or directory") );
		return;
	}

	if ( m_strMainTracker.IsEmpty() )
	{
		AfxMessageBox( _T("Please input main tracker server") );
		return;
	}

	if ( m_strTorrentName.IsEmpty() )
	{
		AfxMessageBox( _T("Please input main tracker server") );
		return;
	}

	if ( m_bEncrypt && m_strPassword.IsEmpty() )
	{
		AfxMessageBox( _T("Please input your password") );
		return;
	}

	CDlgMakeTorrent::m_strMainAnnounce		= m_strMainTracker;
	CDlgMakeTorrent::m_strOtherAnnounces	= m_strOtherTracker;
	CDlgMakeTorrent::m_strComments	= m_strNotes;
	CDlgMakeTorrent::m_strDestTorrentName	= m_strTorrentName;
	CDlgMakeTorrent::m_strEncryptPwd	= m_strPassword;
	CDlgMakeTorrent::m_strSourceFilePath = m_strSrcPathFile;
	CDlgMakeTorrent::m_nSelPieceSize = m_nPieceSize;
	CDlgMakeTorrent::m_bIsFile	= m_nFileOrDir == 0;
	CDlgMakeTorrent::m_nCodePage	= m_wndCodePage.GetItemData( m_wndCodePage.GetCurSel() );

	CDlgMTProgress dlg;
	dlg.DoModal();
}

void CDlgMakeTorrent::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
void CDlgMakeTorrent::OnCancel() 
{
	// TODO: Add extra cleanup here
	
}
void CDlgMakeTorrent::OnBnClicked1001()//when next button is clicked
{
	UpdateData();
	if(showing)
	{
	
	if ( m_strSrcPathFile.IsEmpty() )
	{
		AfxMessageBox( _T("Please select source file or directory") );
		return;
	}
	show_two();
	GetDlgItem(IDC_BACK)->ShowWindow(SW_SHOW);
	showing=false;
	}
	else{
	
		if(!vcheck){
	if ( m_strMainTracker.IsEmpty() )
	{
		AfxMessageBox( _T("Please input main tracker server") );
		return;
	}

	if ( m_strTorrentName.IsEmpty() )
	{
		AfxMessageBox( _T("Please select Dest to save .Torrent") );
		return;
	}

	if ( m_bEncrypt && m_strPassword.IsEmpty() )
	{
		AfxMessageBox( _T("Please input your password") );
		return;
}
	CDlgMakeTorrent::m_strMainAnnounce		= m_strMainTracker;
	CDlgMakeTorrent::m_strOtherAnnounces	= m_strOtherTracker;
	CDlgMakeTorrent::m_strComments	= m_strNotes;
	CDlgMakeTorrent::m_strDestTorrentName	= m_strTorrentName;
	CDlgMakeTorrent::m_strEncryptPwd	= m_strPassword;
	CDlgMakeTorrent::m_strSourceFilePath = m_strSrcPathFile;
	CDlgMakeTorrent::m_nSelPieceSize = m_nPieceSize;
	CDlgMakeTorrent::m_bIsFile	= m_nFileOrDir == 0;
	CDlgMakeTorrent::m_nCodePage	= m_wndCodePage.GetItemData( m_wndCodePage.GetCurSel() );
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind = FindFirstFile(m_strSrcPathFile , &FindFileData);
	
	ULONGLONG FileSize = FindFileData.nFileSizeHigh;//getting file info to analyse
      FileSize <<= sizeof( FindFileData.nFileSizeHigh ) * 8; //these details will be sent to the server for analysing
      FileSize |= FindFileData.nFileSizeLow;
	  CDlgMakeTorrent::vpath=m_strSrcPathFile;
	CDlgMakeTorrent::fsize.Format("%u",FileSize); 
	CDlgMakeTorrent::fname=FindFileData.cFileName;
	CFileTime crtime=FindFileData.ftCreationTime;
	CString str1,ext,type;
	str1=FindFileData.cFileName;
	char *pck,*context;
	pck=strtok_s(str1.GetBuffer(),".",&context);
	if(pck!=NULL)
		pck=strtok_s(NULL,".",&context);
	if(pck!=NULL)
		ext=pck;
	day=crtime.GetTime();
	//week=crtime.Week;
	//hour=crtime.Hour;
	//min=crtime.Minute;
	CMB_type.GetLBText(CMB_type.GetCurSel(),type);
	struct stat infoo;
	stat(m_strSrcPathFile,&infoo);
	ULONG idm=crtime.Day-infoo.st_mtime;
	CDlgMakeTorrent::cinfo.Format("%uD%uG%u",infoo.st_mtime,idm,infoo.st_ino);

	CDlgMakeTorrent::fname=Cname+":"+CDlgMakeTorrent::fname+":"+ext+":"+type+":"+CDlgMakeTorrent::fsize+":"+CDlgMakeTorrent::cinfo;
	//AfxMessageBox(CDlgMakeTorrent::fname);
	vcheck=true;
	}
		
		else if(vcheck)
		{

		CDialog::OnOK();
		}
	}
}

void CDlgMakeTorrent::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}


void CDlgMakeTorrent::OnEnChange1015()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.


	// TODO:  Add your control notification handler code here
}


void CDlgMakeTorrent::OnBnClickedButton1()
{
	

	// TODO: Add your control notification handler code here
}


void CDlgMakeTorrent::OnBnClickedBack()
{
	show_one();
    GetDlgItem(IDC_BACK)->ShowWindow(SW_HIDE);
	// TODO: Add your control notification handler code here
}


void CDlgMakeTorrent::OnCbnSelchangeType()
{
	UpdateData();
	CMB_genre.ResetContent();
	int item = CMB_type.GetCurSel();
	CMB_type.GetLBText(item,CDlgMakeTorrent::x);
	switch(item)
	{
	case 0:
		{
	CMB_genre.AddString("Action");
	CMB_genre.AddString("Adventure");
	CMB_genre.AddString("Horror");
	
	GetDlgItem(IDC_T6)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_VER)->ShowWindow(SW_HIDE);
		break;
		}
	case 1:
		{
		CMB_genre.AddString("NA");
		GetDlgItem(IDC_T6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_VER)->ShowWindow(SW_HIDE);
	break;
		}
	
	case 2:
		{
		CMB_genre.AddString("Games");
		CMB_genre.AddString("Utilities");
		CMB_genre.AddString("Custom");
		GetDlgItem(IDC_T6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_VER)->ShowWindow(SW_SHOW);
		break;
		}
		case 3:
		{
	CMB_genre.AddString("Rock");
		CMB_genre.AddString("Pop");
		CMB_genre.AddString("Rap");
		GetDlgItem(IDC_T6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_VER)->ShowWindow(SW_HIDE);
		break;
		}
	}
}


void CDlgMakeTorrent::OnStnClickedT5()
{
	// TODO: Add your control notification handler code here
}


BOOL CDlgMakeTorrent::OnEraseBkgnd(CDC* pDC)
{
	CDC dcMemory;
    dcMemory.CreateCompatibleDC(pDC);
    CBitmap* pOldbitmap = dcMemory.SelectObject(&Bacground);
    CRect rcClient;
    GetClientRect(&rcClient);
    const CSize& sbitmap = bitmapSize;
    pDC->BitBlt(0,0,sbitmap.cx,sbitmap.cy,&dcMemory,0,0,SRCCOPY);
    dcMemory.SelectObject(pOldbitmap);
    return TRUE;
}


HBRUSH CDlgMakeTorrent::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
    {
    case CTLCOLOR_STATIC:
        pDC->SetTextColor(RGB(255, 0, 0));
		pDC->SetBkMode(TRANSPARENT);
        return (HBRUSH)GetStockObject(NULL_BRUSH);
    default:
        return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    }
}
