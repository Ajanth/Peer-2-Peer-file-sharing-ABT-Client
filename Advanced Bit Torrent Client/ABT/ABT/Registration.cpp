// Registration.cpp : implementation file
//

#include "stdafx.h"
#include "ABT.h"
#include "Registration.h"
#include "afxdialogex.h"


// CRegistration dialog
HANDLE thr2;//for UDP comms
unsigned long id3;////for UDP comms
CString server_IP2;
IMPLEMENT_DYNAMIC(CRegistration, CDialogEx)

CRegistration::CRegistration(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRegistration::IDD, pParent)
	, uname(_T(""))
	, eid(_T(""))
	, pass1(_T(""))
	, pass2(_T(""))
	, s_country(_T(""))
	, s_state(_T(""))
{

}

CRegistration::~CRegistration()
{

}

void CRegistration::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_USRNAME, uname);
	DDX_Text(pDX, IDC_EMAIL, eid);
	DDX_Text(pDX, IDC_PASS1, pass1);
	DDX_Text(pDX, IDC_PASS2, pass2);
	DDX_Control(pDX, IDC_MOV, CB_movies);
	DDX_Control(pDX, IDC_VID, CB_videos);
	DDX_Control(pDX, IDC_GAM, CB_games);
	DDX_Control(pDX, IDC_OTH1, CB_others);
	DDX_Control(pDX, IDC_MUS, CB_music);
	DDX_Control(pDX, IDC_SOF, CB_softwares);
	DDX_Control(pDX, IDC_AGR, CB_agree);
	DDX_Text(pDX, IDC_COUNTRY, s_country);
	DDX_Text(pDX, IDC_STATE, s_state);
}


BEGIN_MESSAGE_MAP(CRegistration, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRegistration::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BACK, &CRegistration::OnBnClickedBack)
	ON_EN_KILLFOCUS(IDC_USRNAME, &CRegistration::OnEnKillfocusUsrname)
	ON_EN_KILLFOCUS(IDC_PASS2, &CRegistration::OnEnKillfocusPass2)
	ON_EN_KILLFOCUS(IDC_EMAIL, &CRegistration::OnEnKillfocusEmail)
	ON_EN_KILLFOCUS(IDC_PASS1, &CRegistration::OnEnKillfocusPass1)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CRegistration message handlers
BOOL CRegistration::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	thr2=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Receive_thr,this,NULL,&id3);
	first=false;
	exists=false;
	not_exists=false;
	init_shows();
	server_IP2="192.168.1.5";
	Bacground.LoadBitmap(IDB_BITMAP3);
	Bacground.GetBitmap(&bm);
	bitmapSize = CSize(bm.bmWidth, bm.bmHeight);
	Invalidate(1);
return true;

}
void CRegistration::init_shows()
{
	first=false;
SetDlgItemText(IDD_REGISTRATION,"Registration Step-1");
GetDlgItem(IDC_BACK)->ShowWindow(SW_HIDE);
GetDlgItem(IDC_R1)->ShowWindow(SW_SHOW);
GetDlgItem(IDC_R2)->ShowWindow(SW_SHOW);
GetDlgItem(IDC_R3)->ShowWindow(SW_SHOW);
GetDlgItem(IDC_R4)->ShowWindow(SW_SHOW);
GetDlgItem(IDC_USRNAME)->ShowWindow(SW_SHOW);
GetDlgItem(IDC_EMAIL)->ShowWindow(SW_SHOW);
GetDlgItem(IDC_PASS1)->ShowWindow(SW_SHOW);
GetDlgItem(IDC_PASS2)->ShowWindow(SW_SHOW);
GetDlgItem(IDC_R5)->ShowWindow(SW_HIDE);
GetDlgItem(IDC_MOV)->ShowWindow(SW_HIDE);
GetDlgItem(IDC_GAM)->ShowWindow(SW_HIDE);
GetDlgItem(IDC_SOF)->ShowWindow(SW_HIDE);
GetDlgItem(IDC_VID)->ShowWindow(SW_HIDE);
GetDlgItem(IDC_MUS)->ShowWindow(SW_HIDE);
GetDlgItem(IDC_OTH1)->ShowWindow(SW_HIDE);
GetDlgItem(IDC_AGR)->ShowWindow(SW_HIDE);
GetDlgItem(IDC_R6)->ShowWindow(SW_HIDE);
GetDlgItem(IDC_R7)->ShowWindow(SW_HIDE);
GetDlgItem(IDC_COUNTRY)->ShowWindow(SW_HIDE);
GetDlgItem(IDC_STATE)->ShowWindow(SW_HIDE);

}

int CRegistration::user_name_check(CString name)
{
	
	UpdateData();
	if(name.FindOneOf("@;:'!()&*%$#")>=0)
	{invalid_char=true;
	form_error="User name contains Invalid Characters";

	return 0;
	}
	send_server("CHECKNAME");
	Sleep(500);
	send_server(name);
	Sleep(1500);
	if(exists)
	{name_ok=false;
	form_error="User Name Already Taken. Please Chose a Different User Name!!";
	SetDlgItemText(IDC_STAT,"UserName Unavailable");
	}
	else if(not_exists)
	{name_ok=true;
	SetDlgItemText(IDC_STAT,"UserName available");
	}
	return 0;
}

UINT Receive_thr(LPVOID pParam)// thread for receiving reply msgs from main server
{
	CRegistration *dlg=(CRegistration*)pParam;
	AfxSocketInit(NULL);
	CSocket echoServer;  
	
	int recvMsgSize;
	  if ( echoServer.Create(420, SOCK_DGRAM, NULL)== false) 
		AfxMessageBox("Create socket failed");
	

	for(;;) { // Run forever

    // Client address
    SOCKADDR_IN echoClntAddr; 
	int clntAddrLen;
	char echoBuffer[1024]; 
    // Set the size of the in-out parameter
    clntAddrLen = sizeof(echoClntAddr);
    recvMsgSize = echoServer.ReceiveFrom(echoBuffer, 
	  1024, (SOCKADDR*)&echoClntAddr, &clntAddrLen, 0);
    if (recvMsgSize < 0) {
      AfxMessageBox("RecvFrom() failed");
    }
	echoBuffer[recvMsgSize]='\0';
	dlg->client_IP=inet_ntoa(echoClntAddr.sin_addr);
	dlg->server_msg=echoBuffer;
	
	if(dlg->server_msg=="NAMEEXISTS")
	{
	dlg->exists=true;
	dlg->not_exists=false;
	}
	else if(dlg->server_msg=="CONTINUE")
	{
	dlg->not_exists=true;
	dlg->exists=false;
	}

	else if(dlg->server_msg=="REGSUCS")
		dlg->reg_pass=true;

	else if(dlg->server_msg=="REGFAIL")
		dlg->reg_fail=true;
	//else if(dlg->server_msg=="FOR_IP")
		//dlg->my_IP=dlg->client_IP;
   
  }
	return 0;
}


void CRegistration::send_server(CString Buffer) //to send msgs to server
{
	int buflen=strlen(Buffer);
	AfxSocketInit(NULL);
	 CSocket echoClient;  
	 if (echoClient.Create(0,SOCK_DGRAM,NULL) == 0) {
    AfxMessageBox("Create() failed");
	}
	 if (echoClient.SendTo(Buffer, buflen,911,
      server_IP2, 0) != buflen) {
    AfxMessageBox("SendTo() sent a different number of bytes than expected");
  }

	 echoClient.Close();

}

void CRegistration::send_me(CString Buffer) //used only for getting my ip
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


int CRegistration::check_email(CString eid)
{
	CString p1;
	GetDlgItemText(IDC_EMAIL,p1);
	AfxMessageBox(p1);
if(p1.Find('@')==-1||p1.Find('.')==-1)
{
	form_error="Email ID is Invalid";
	
	return 0;}
return 1;
}

int CRegistration::isname_exists()
{
if(name_ok)
	return 1;
else
	{form_error="User Name Already Taken. Please Chose a Different User Name!!";
return 0;
}

}
int CRegistration::check_password(CString pass1,CString pass2)
{
	CString p1,p2;
	GetDlgItemText(IDC_PASS1,p1);
	GetDlgItemText(IDC_PASS2,p2);
	if(p1.GetLength()<6||(p1!=p2))
	{
		form_error="Password must be atleast 6 characters long and match!";
		return 0;
	}
	return 1;


}


void CRegistration::OnBnClickedOk()//when next button is clicked in registration form
{
	// TODO: Add your control notification handler code here
	if(!first)
	{
		if(check_email(eid)&&check_password(pass1,pass2)&&isname_exists())
	{show_form(2);
		first=true;
		name_ok=false;
		}
		else
		{
			if(form_error=="")
				AfxMessageBox("empty formerror");
			else
		AfxMessageBox(form_error);

		}
	}
	else
	{
	CString reginfo_s,reginfo_c;
	if(final_check())
	{
	send_server("REGISTER");
	reginfo_s=uname+":"+pass1+":"+eid+":"+intr+":"+s_country+":"+s_state;
	Sleep(300);
	send_server(reginfo_s);
	Sleep(500);
	if(reg_pass)
		{
		write_profile();
		install_ABT();
		AfxMessageBox("ABT has been successfully installed.Thank you!");
			}
	else if(reg_fail)
	AfxMessageBox("Server may be down.Your registration Failed. Please try again later");
	
	}
	else
		AfxMessageBox(form_error);

	}
	//CDialogEx::OnOK();
}

void CRegistration::write_profile()
{
CStdioFile f;
CString str;
if (f.Open("C:\\ABT\\profile.inf", CFile::modeWrite | CFile::modeCreate | CFile::shareDenyNone))
{
	f.WriteString(uname);
	f.WriteString("\n");
	send_me("FOR_IP");
	Sleep(300);
	if(my_IP!="")
		f.WriteString(my_IP);
	f.WriteString("\n");
	f.WriteString(eid);
	f.WriteString("\n");
	f.WriteString(intr);
	f.WriteString("\n");
	f.WriteString(s_country);
	f.WriteString("\n");
	f.WriteString(s_state);
	f.Close();

}
else
	AfxMessageBox("Error while writing profile data to file");
}


void CRegistration::install_ABT()
{
	CStdioFile f;
	if (f.Open("C:\\ABT\\ABT.inf", CFile::modeWrite | CFile::modeCreate | CFile::shareDenyNone))
	{
	f.WriteString(uname);
	f.Close();
	}
	else
	{
	AfxMessageBox("Error Opening file..Installation Failed");
	}
}


int CRegistration::final_check()//check form 2 in registration
{
	form_error="no-error";
	UpdateData();
	intr="";
if(CB_games.GetCheck()==BST_CHECKED)
	intr+="Games.";
if(CB_music.GetCheck()==BST_CHECKED)
	intr+="Music.";
if(CB_videos.GetCheck()==BST_CHECKED)
	intr+="Videos.";
if(CB_movies.GetCheck()==BST_CHECKED)
	intr+="Movies.";
if(CB_others.GetCheck()==BST_CHECKED)
	intr+="Others.";
if(CB_softwares.GetCheck()==BST_CHECKED)
	intr+="Softwares";
if(intr=="")
{form_error="Please chose what you download the most";
return 0;
}
if(s_country==""||s_state=="")
{
form_error="Please Fill in your country and state";
return 0;
}
if(CB_agree.GetCheck()!=BST_CHECKED)
{
	form_error="You must agree to the P2P terms to install ";
return 0;
}
return 1;
}


void CRegistration::show_form(int code)
{

if(code==2)
{
	SetDlgItemText(IDD_REGISTRATION,"Registration Step-2");
	GetDlgItem(IDC_BACK)->ShowWindow(SW_SHOW);
GetDlgItem(IDC_R1)->ShowWindow(SW_HIDE);
GetDlgItem(IDC_R2)->ShowWindow(SW_HIDE);
GetDlgItem(IDC_R3)->ShowWindow(SW_HIDE);
GetDlgItem(IDC_R4)->ShowWindow(SW_HIDE);
GetDlgItem(IDC_USRNAME)->ShowWindow(SW_HIDE);
GetDlgItem(IDC_EMAIL)->ShowWindow(SW_HIDE);
GetDlgItem(IDC_PASS1)->ShowWindow(SW_HIDE);
GetDlgItem(IDC_PASS2)->ShowWindow(SW_HIDE);
GetDlgItem(IDC_R5)->ShowWindow(SW_SHOW);
GetDlgItem(IDC_MOV)->ShowWindow(SW_SHOW);
GetDlgItem(IDC_GAM)->ShowWindow(SW_SHOW);
GetDlgItem(IDC_SOF)->ShowWindow(SW_SHOW);
GetDlgItem(IDC_VID)->ShowWindow(SW_SHOW);
GetDlgItem(IDC_MUS)->ShowWindow(SW_SHOW);
GetDlgItem(IDC_OTH1)->ShowWindow(SW_SHOW);
GetDlgItem(IDC_AGR)->ShowWindow(SW_SHOW);
GetDlgItem(IDC_R6)->ShowWindow(SW_SHOW);
GetDlgItem(IDC_R7)->ShowWindow(SW_SHOW);
GetDlgItem(IDC_COUNTRY)->ShowWindow(SW_SHOW);
GetDlgItem(IDC_STATE)->ShowWindow(SW_SHOW);
}

}

void CRegistration::OnBnClickedBack()//when back button is  clicked in form
{
	GetDlgItem(IDC_BACK)->ShowWindow(SW_HIDE);
	init_shows();
}


void CRegistration::OnEnKillfocusUsrname()//when username field loses focus
{
	UpdateData(1);
	Invalidate();
	user_name_check(uname);
}


void CRegistration::OnEnKillfocusPass2()
{
}


void CRegistration::OnEnKillfocusEmail()
{
}


void CRegistration::OnEnKillfocusPass1()
{
}


HBRUSH CRegistration::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
switch (nCtlColor)
    {
    case CTLCOLOR_STATIC:
        pDC->SetTextColor(RGB(255, 0, 0));
		pDC->SetBkMode(TRANSPARENT);
        return (HBRUSH)GetStockObject(NULL_BRUSH);
    default:
        return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
    }
}


BOOL CRegistration::OnEraseBkgnd(CDC* pDC)
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
}
