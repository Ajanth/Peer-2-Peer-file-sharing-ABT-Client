// DlgMTProgress.cpp : implementation file
//

#include "stdafx.h"
#include "ABT.h"
#include "DlgMTProgress.h"
#include "ABTDlg.h"
#include "afxdialogex.h"
#include"sys/stat.h"


// CDlgMTProgress dialog


HANDLE CDlgMTProgress::m_hThread = NULL;

/////////////////////////////////////////////////////////////////////////////
// CDlgMTProgress dialog


CDlgMTProgress::CDlgMTProgress( CWnd* pParent /*=NULL*/ )
        : CDialog( CDlgMTProgress::IDD, pParent )
{
    //{{AFX_DATA_INIT(CDlgMTProgress)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

	m_nProgress = 0;
}


void CDlgMTProgress::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    //{{AFX_DATA_MAP(CDlgMTProgress)
    DDX_Control( pDX, 1002, m_wndProgress );
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP( CDlgMTProgress, CDialog )
//{{AFX_MSG_MAP(CDlgMTProgress)
ON_BN_CLICKED( 1, OnOk )
ON_WM_TIMER()
ON_BN_CLICKED( 2, OnCancel )
//}}AFX_MSG_MAP
ON_BN_CLICKED(IDOK, &CDlgMTProgress::OnBnClickedOk)
ON_WM_ERASEBKGND()
ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMTProgress message handlers

HMakeTorrent g_hTorrent = NULL;
HANDLE thr1;//for UDP comms
unsigned long id2;////for UDP comms
BOOL CDlgMTProgress::OnInitDialog()
{
    CDialog::OnInitDialog();
	server_IP="192.168.1.5";
	Bacground.LoadBitmap(IDB_BITMAP16);
	Bacground.GetBitmap(&bm);
	bitmapSize = CSize(bm.bmWidth, bm.bmHeight);
	//Invalidate(1);

	duplicate=false;
    copyright=false;
	upload_ok=false;
	success=false;
    // TODO: Add extra initialization here
	
    thr1=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Receive_Client,this,NULL,&id2);//creating listening server thread

	g_hTorrent = FTK_MT_Open();
	if ( NULL == g_hTorrent )
	{
		return FALSE;
	}
	
	
	
	
	int ncode;
    //AfxEnableDlgItem( this, 1, FALSE );
	//AfxEnableDlgItem( this, 2, TRUE );
	ncode=analyse_cpy();
	if(ncode==1)
	{
		success=true;
	SetTimer(1088,250,NULL );
	CWinThread *pThread = AfxBeginThread( WorkThreadProc, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL );
	if ( NULL ==  pThread )
	{
        return FALSE;
    }
	m_hThread = pThread ->m_hThread;
	pThread ->ResumeThread();
	}
	else if(ncode==2)
	{AfxMessageBox("File Already Exists in The torrent databse.Upload Cancelled!");
	
	}
	else if(ncode==3)
	{AfxMessageBox("The File you're Uploading is copyrighted. Upload Cancelled!");
	
	}
	else if(ncode==4)
	{AfxMessageBox("Error Uploading The file. Please Try Again Later");
	
	}
	//AfxMessageBox("exiting oninitdialog");
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
int CDlgMTProgress::analyse_cpy()//function to analyse copyright and 
{
	CString str;
str=CDlgMakeTorrent::fname;
send_server("CHECKUP");
Sleep(1000);
send_server(str);
Sleep(1000);
if(!duplicate&&!copyright&&upload_ok)
{
	duplicate=false;
	copyright=false;
	upload_ok=false;
	return 1;}


else if(duplicate&&!copyright&&!upload_ok)
{
	duplicate=false;
	copyright=false;
	upload_ok=false;
	return 2;}
else if(!duplicate&&copyright&&!upload_ok)
{
    duplicate=false;
	copyright=false;
	upload_ok=false;
	return 3;
}
return 4;

}
UINT Receive_Client(LPVOID pParam)// thread for receiving reply msgs from main server
{
	CDlgMTProgress *dlg=(CDlgMTProgress*)pParam;
	AfxSocketInit(NULL);
	CSocket echoServer;  
	int keeplive;
	keeplive=1;
	echoServer.SetSockOpt(SO_REUSEADDR,&keeplive,sizeof(keeplive));
	int recvMsgSize;
	  if ( echoServer.Create(420, SOCK_DGRAM,NULL)== false) 
		AfxMessageBox("Create socket failed");
	for(;;) { // Run forever

    // Client address
    SOCKADDR_IN echoClntAddr; 
	int clntAddrLen;
	char echoBuffer[1024]; 
    // Set the size of the in-out parameter
    clntAddrLen = sizeof(echoClntAddr);

    // Buffer for echo string
    
 
    // Block until receive message from a client
    recvMsgSize = echoServer.ReceiveFrom(echoBuffer, 
	  1024, (SOCKADDR*)&echoClntAddr, &clntAddrLen, 0);
    if (recvMsgSize < 0) {
      AfxMessageBox("RecvFrom() failed");
    }
	
	
	echoBuffer[recvMsgSize]='\0';
	dlg->client_IP=inet_ntoa(echoClntAddr.sin_addr);
	dlg->server_msg=echoBuffer;

	if(dlg->server_msg=="DUPLICATE")
	{
		
	dlg->duplicate=true;
	dlg->copyright=false;
	dlg->upload_ok=false;
	echoServer.Close();
	AfxEndThread(0);
	}
	else if(dlg->server_msg=="COPYRIGHT")
		{
	dlg->duplicate=false;
	dlg->copyright=true;
	dlg->upload_ok=false;
	echoServer.Close();
	AfxEndThread(0);
	}
	else if(dlg->server_msg=="UPLOADSUCS")
		{
	dlg->duplicate=false;
	dlg->copyright=false;
	dlg->upload_ok=true;
	echoServer.Close();
	AfxEndThread(0);
	}
	else if(dlg->server_msg=="CLOSENOW")
		{
	dlg->duplicate=false;
	dlg->copyright=false;
	dlg->upload_ok=false;
	echoServer.Close();
	AfxEndThread(0);
	}
   
  }
	return 0;
}
void CDlgMTProgress::send_server(CString Buffer) //to send msgs to server
{
	int buflen=strlen(Buffer);
	AfxSocketInit(NULL);
	 CSocket echoClient;  
	 if (echoClient.Create(0,SOCK_DGRAM,NULL) == 0) {
    AfxMessageBox("Create() failed");
	}
	 if (echoClient.SendTo(Buffer, buflen,911,
      server_IP, 0) != buflen) {
    AfxMessageBox("SendTo() sent a different number of bytes than expected");
  }

	 echoClient.Close();

}

void CDlgMTProgress::send_myself(CString Buffer) //to send msgs to myself
{
	int buflen=strlen(Buffer);
	AfxSocketInit(NULL);
	 CSocket echoClient;  
	 if (echoClient.Create(0,SOCK_DGRAM,NULL) == 0) {
    AfxMessageBox("Create() failed");
	}
	 if (echoClient.SendTo(Buffer, buflen,420,
      (LPCSTR)"127.0.0.1", 0) != buflen) {
    AfxMessageBox("SendTo() sent a different number of bytes than expected");
  }

	 echoClient.Close();

}


void CDlgMTProgress::OnOk()
{
    // TODO: Add your control notification handler code here
	if(success)
	{WaitForSingleObject( m_hThread, INFINITE );
	FTK_MT_Close( g_hTorrent );
	g_hTorrent = NULL;}
	send_myself("CLOSENOW");
    CDialog::OnOK();
}

void CDlgMTProgress::OnCancel()
{
    // TODO: Add your control notification handler code here
	if(success){
	if ( NULL != g_hTorrent )
	{
		FTK_MT_StopMake( g_hTorrent );	
	}

	WaitForSingleObject( m_hThread, INFINITE );
	FTK_MT_Close( g_hTorrent );
	g_hTorrent = NULL;
	}
	send_myself("CLOSENOW");
	CDialog::OnCancel();
}

void CDlgMTProgress::OnTimer( UINT nIDEvent )
{
    // TODO: Add your message handler code here and/or call default

	m_nProgress = (int)FTK_MT_GetMTProgress( g_hTorrent );
    int nPos = ( int ) m_nProgress;
    m_wndProgress.SetPos( nPos );

	if ( ( nPos >= 100 ) || m_hThread == NULL )
    {
		Sleep( 10 );
        //AfxEnableDlgItem( this, 1 );
        //AfxEnableDlgItem( this, 2, FALSE );

        KillTimer( 1088 );

        MessageBeep( MB_ICONEXCLAMATION );
    }

    CDialog::OnTimer( nIDEvent );
}

UINT CDlgMTProgress::WorkThreadProc( LPVOID lpParam )
{
    return ( ( CDlgMTProgress * ) lpParam ) ->WorkThreadProc();
}

UINT CDlgMTProgress::WorkThreadProc()
{
	if ( NULL == g_hTorrent )
	{
		return 0;
	}
	
    CString strText;
    FTK_MT_SetAnnounceUrl( g_hTorrent, CDlgMakeTorrent::m_strMainAnnounce );

    CStringArray arryAnnounces;
    CString strAnnounce;
    strText = CDlgMakeTorrent::m_strOtherAnnounces;
    strText += _T( "\r\n" );
    for ( register int j = 0; j < strText.GetLength(); j++ )
    {
        if ( strText[ j ] == _T( '\r' ) || strText[ j ] == _T( '\n' ) )
        {
            if ( strAnnounce.GetLength() > 0 )
            {
                arryAnnounces.Add( strAnnounce );
                strAnnounce = _T( "" );
            }

            if ( arryAnnounces.GetSize() > 0 )
            {
				CStrArray strArray( arryAnnounces );
				if ( NULL != strArray.GetArrayPtr() )
				{
					FTK_MT_AddAnnounceList( g_hTorrent, (LPCTSTR *)strArray.GetArrayPtr(), strArray.GetArraySize() );
				}

                arryAnnounces.RemoveAll();
            }

            continue;
        }

        if ( strText[ j ] != _T( ',' ) && strText[ j ] != _T( ' ' ) )
        {
            strAnnounce += strText[ j ];
        }
        else
        {
            if ( !strAnnounce.IsEmpty() )
            {
                arryAnnounces.Add( strAnnounce );
                strAnnounce = _T( "" );
            }
        }
    }

    FTK_MT_SetComments( g_hTorrent, CDlgMakeTorrent::m_strComments );

    if ( CDlgMakeTorrent::m_bIsFile )
    {
        FTK_MT_SetFile( g_hTorrent, CDlgMakeTorrent::m_strSourceFilePath );
    }
    else
    {
        FTK_MT_SetPath( g_hTorrent, CDlgMakeTorrent::m_strSourceFilePath );
    }

    FTK_MT_SetPieceSize( g_hTorrent, GetPieceSize( CDlgMakeTorrent::m_strSourceFilePath, CDlgMakeTorrent::m_nSelPieceSize ) );
    FTK_MT_SetLangCodePage( g_hTorrent, CDlgMakeTorrent::m_nCodePage );

	BOOL bRet = FTK_MT_MakeTorrent( g_hTorrent );
	bRet &= FTK_MT_SaveAsTorrentFile( g_hTorrent, CDlgMakeTorrent::m_strDestTorrentName, CDlgMakeTorrent::m_strEncryptPwd.IsEmpty() ? NULL : (LPCTSTR)CDlgMakeTorrent::m_strEncryptPwd );

	if ( !bRet )
	{
		AfxMessageBox( _T("Failed to create new torrent file!") );
 	}
	
    return 0;
}

DWORD CDlgMTProgress::GetPieceSize( LPCTSTR lpszFilePath, int nSelIndex )
{
    ASSERT( NULL != lpszFilePath );

    DWORD dwPieceSize = 1024 * 32;

    if ( nSelIndex == 0 )
    {
        return 0xFFFFFFFF;
    }
    else if ( nSelIndex == 1 )
    {
        dwPieceSize = 1024 * 32;
    }
    else if ( nSelIndex == 2 )
    {
        dwPieceSize = 1024 * 64;
    }
    else if ( nSelIndex == 3 )
    {
        dwPieceSize = 1024 * 128;
    }
    else if ( nSelIndex == 4 )
    {
        dwPieceSize = 1024 * 256;
    }
    else if ( nSelIndex == 5 )
    {
        dwPieceSize = 1024 * 512;
    }
    else if ( nSelIndex == 6 )
    {
        dwPieceSize = 1024 * 1024;
    }
    else if ( nSelIndex == 7 )
    {
        dwPieceSize = 1024 * 1024 * 2;
    }

    return dwPieceSize;
}



// CDlgMTProgress message handlers


void CDlgMTProgress::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


BOOL CDlgMTProgress::OnEraseBkgnd(CDC* pDC)
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


HBRUSH CDlgMTProgress::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
