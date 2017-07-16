// Login.cpp : implementation file
//

#include "stdafx.h"
#include "ABT.h"
#include "Login.h"
#include "afxdialogex.h"
#include "ABTUI.h"

HANDLE thr3;//for UDP comms
unsigned long id4;
// CLogin dialog
CString server_IP3;
IMPLEMENT_DYNAMIC(CLogin, CDialogEx)

CLogin::CLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogin::IDD, pParent)
{

}

CLogin::~CLogin()
{
}

void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLogin, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLogin::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLogin::OnBnClickedCancel)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
BOOL CLogin::OnInitDialog()
{
	server_IP3="192.168.1.5";
	logs=false;
	logf=false;
	Bacground.LoadBitmap(IDB_BITMAP14);
	Bacground.GetBitmap(&bm);
	bitmapSize = CSize(bm.bmWidth, bm.bmHeight);
	Invalidate(1);
	thr3=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Receive_login,this,NULL,&id4);
	return TRUE;}

UINT Receive_login(LPVOID pParam)// thread for receiving reply msgs from main server
{
	
	CLogin *dlg=(CLogin*)pParam;
	AfxSocketInit(NULL);
	CSocket echoServer;  
	int live=1;
	echoServer.SetSockOpt(SO_REUSEADDR,&live,sizeof(live));
	CString str;
	int recvMsgSize;
	  if ( echoServer.Create(421, SOCK_DGRAM, NULL)== false) 
		AfxMessageBox("Create socket failed");
	

	for(;;) { // Run forever

    // Client address
		if(dlg->t_flag){
    SOCKADDR_IN echoClntAddr; 
	int clntAddrLen;
	char echoBuffer[1024]; 
    // Set the size of the in-out parameter
    clntAddrLen = sizeof(echoClntAddr);
    recvMsgSize = echoServer.ReceiveFrom(echoBuffer, 
	  1024, (SOCKADDR*)&echoClntAddr, &clntAddrLen, 0);
    if (recvMsgSize < 0) {
      AfxMessageBox("RecvFrom() failed in login");
    }
	echoBuffer[recvMsgSize]='\0';	
	str=echoBuffer;
	if(str=="LOGINFAIL")
	{
	dlg->logs=false;
	dlg->logf=true;
	}
	else if(str=="LOGINSUCS")
	{
	dlg->logf=false;
	dlg->logs=true;
	echoServer.Close();
	AfxEndThread(0);
	}
	//handle str for authentication
		}
	echoServer.Close();
	AfxEndThread(0);
	}
	return 0;
}



void CLogin::send_server(CString Buffer) //to send msgs to server for login authentication
{
	int buflen=strlen(Buffer);
	AfxSocketInit(NULL);
	 CSocket echoClient;  
	 if (echoClient.Create(0,SOCK_DGRAM,NULL) == 0) {
    AfxMessageBox("Create() failed");
	}
	 if (echoClient.SendTo(Buffer, buflen,911,
      (LPCTSTR)"192.168.1.5", 0) != buflen) {
    AfxMessageBox("SendTo() sent a different number of bytes than expected");
  }

	 echoClient.Close();

}
void CLogin::send_me(CString Buffer) //to send msgs to server for login authentication
{
	int buflen=strlen(Buffer);
	AfxSocketInit(NULL);
	 CSocket echoClient;  
	 if (echoClient.Create(0,SOCK_DGRAM,NULL) == 0) {
    AfxMessageBox("Create() failed");
	}
	 if (echoClient.SendTo(Buffer, buflen,421,
      (LPCSTR)"127.0.0.1", 0) != buflen) {
    AfxMessageBox("SendTo() sent a different number of bytes than expected");
  }

	 echoClient.Close();

}
// CLogin message handlers


void CLogin::OnBnClickedOk()//when logiin button is clicked
{
	
	CString un,pw,str;
	GetDlgItemText(IDC_UNAME,un);
	GetDlgItemText(IDC_PWD,pw);
	if(un==""||pw=="")
		AfxMessageBox("Fields Cannot be empty");
	else
	{
	send_server("LOGIN");
	Sleep(500);
	str=un+":"+pw;
	send_server(str);
	Sleep(1500);
	if(logf&&!logs)
		AfxMessageBox("Incorrect Username or password!");
	else if(logs&&!logf)
	{
		CStdioFile i;
	if(i.Open("C:\\ABT\\ABT.inf",CFile::modeCreate |CFile::modeWrite| CFile::shareDenyNone))
		i.WriteString(un);
	t_flag=false;
	Sleep(500);
	send_me("DAMNLIFE");
	CDialogEx::OnOK();

	}
	}
	//
}


void CLogin::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	t_flag=false;
	Sleep(500);
	send_me("DAMNLIFE");
	PostQuitMessage(0);
}


BOOL CLogin::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	 CDC dcMemory;
    dcMemory.CreateCompatibleDC(pDC);
    CBitmap* pOldbitmap = dcMemory.SelectObject(&Bacground);
    CRect rcClient;
    GetClientRect(&rcClient);
    const CSize& sbitmap = bitmapSize;
    pDC->BitBlt(0,0,sbitmap.cx,sbitmap.cy,&dcMemory,0,0,SRCCOPY);
    dcMemory.SelectObject(pOldbitmap);
    return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}


HBRUSH CLogin::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
    {
    case CTLCOLOR_STATIC:
        pDC->SetTextColor(RGB(0, 0, 250));
		pDC->SetBkMode(TRANSPARENT);
        return (HBRUSH)GetStockObject(NULL_BRUSH);
    default:
        return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
    }
}
