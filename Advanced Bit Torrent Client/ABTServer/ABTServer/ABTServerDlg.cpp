
// ABTServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ABTServer.h"
#include "ABTServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define ECHOMAX 1024

// CABTServerDlg dialog

	HANDLE thr;//for UDP comms
	unsigned long id1;////for UDP comms
	


CABTServerDlg::CABTServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CABTServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CABTServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CABTServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CABTServerDlg::OnBnClickedOk)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CABTServerDlg message handlers

BOOL CABTServerDlg::OnInitDialog()
{

	Bacground.LoadBitmap(IDB_BITMAP1);
	Bacground.GetBitmap(&bm);
	bitmapSize = CSize(bm.bmWidth, bm.bmHeight);
	Invalidate(1);
	CDialogEx::OnInitDialog();

	//creating listening server thread
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	checkname=false;
	registering=false;
	login=false;
	checking=false;

	// TODO: Add extra initialization here
	check_username("Ajanth");

	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CABTServerDlg::OnPaint()
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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CABTServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CABTServerDlg::OnBnClickedOk()
{
	thr=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ReceiveData,this,NULL,&id1);
	GetDlgItem(IDOK)->SetWindowTextA("Server Running....");
	GetDlgItem(IDOK)->EnableWindow(false);

	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();
}
void CABTServerDlg::set_file(CString fn)
{
fnm="C:\\ABT\\DUP\\"+fn+".inf";
}
UINT ReceiveData(LPVOID pParam)// thread for server
{
	CABTServerDlg *dlg=(CABTServerDlg*)pParam;
	AfxSocketInit(NULL);
	CSocket echoServer;  
	int recvMsgSize;
	  if ( echoServer.Create(911, SOCK_DGRAM, NULL)== false) 
		AfxMessageBox("Create socket failed");
	for(;;) { // Run forever
    // Client address
    SOCKADDR_IN echoClntAddr; 
	int clntAddrLen;
	char echoBuffer[ECHOMAX]; 
	CString mess;
    // Set the size of the in-out parameter
    clntAddrLen = sizeof(echoClntAddr);
    recvMsgSize = echoServer.ReceiveFrom(echoBuffer, 
	  ECHOMAX, (SOCKADDR*)&echoClntAddr, &clntAddrLen, 0);
    if (recvMsgSize < 0) {
      AfxMessageBox("RecvFrom() failed");
    }	
	echoBuffer[recvMsgSize]='\0';
	dlg->s_IP=inet_ntoa(echoClntAddr.sin_addr);
	mess=echoBuffer;
	if(dlg->checkname)//handling flags from here
	{
	dlg->checkname=false;
	if(dlg->check_username(echoBuffer))
		dlg->send_back("CONTINUE");
	else
		dlg->send_back("NAMEEXISTS");
	}

	else if(dlg->registering)
	{
	dlg->registering=false;
	if(!dlg->register_user(echoBuffer,"lo"))
		dlg->send_back("REGFAIL");
	else
		dlg->send_back("REGSUCS");
	}

	else if(dlg->login)
	{
		dlg->login=false;
	if(!dlg->authenticate(echoBuffer))
		dlg->send_login("LOGINFAIL");
	else
		dlg->send_login("LOGINSUCS");
	}

	else if(dlg->checking)
	{
	dlg->checking=false;
	dlg->check_upload(echoBuffer);
	}

	////checking incoming command from here
	else if(mess=="CHECKNAME")//for checking user name while registering
	{dlg->checkname=true;
	}
	else if(mess=="REGISTER")//for registering user in the database
	dlg->registering=true;
	else if(mess=="LOGIN")
		dlg->login=true;
	else if(mess=="CHECKUP")
		dlg->checking=true;



	
	}

}


int CABTServerDlg::check_upload(CString info)
{
	CString aname,fname,ext,type,size,crtime;
	pch=strtok_s(info.GetBuffer(),":",&context);
	aname=pch;
	pch=strtok_s(NULL,":",&context);
	if(pch!=NULL)
	fname=pch;
	pch=strtok_s(NULL,":",&context);
	if(pch!=NULL)
	ext=pch;
	pch=strtok_s(NULL,":",&context);
	if(pch!=NULL)
	type=pch;
	pch=strtok_s(NULL,":",&context);
	if(pch!=NULL)
	size=pch;
	pch=strtok_s(NULL,":",&context);
	if(pch!=NULL)
	{crtime=pch;
	int nret;
	nret=check_file(aname,fname,ext,type,size,crtime);
	if(nret==1)
	{
	send_back("DUPLICATE");
	return 0;
	}
	else if(nret==2)
	{
	send_back("COPYRIGHT");
	return 0;
	}
	else if(nret==0)
	{
	send_back("UPLOADSUCS");
	return 1;
	}
	


	}

}


int CABTServerDlg::check_file(CString aname,CString fname,CString ext,CString type,CString size,CString crtime)
{
	//checks for both copyright and duplication
	sDriver = "MICROSOFT ACCESS DRIVER (*.mdb)";
	sFile = "C:\\ABT\\Mansion.mdb"; 
	sDsn.Format("ODBC;DRIVER={%s};DSN='';DBQ=%s",sDriver,sFile);

	TRY
	{

		database.Open(NULL,false,false,sDsn);
		CRecordset recset( &database );
		//CHECKING FOR DUPLICATION HEREAFTER
		SqlString= "SELECT * "
				"FROM Dup WHERE Aname='"+aname+"'";

		recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);
		
		if(!recset.IsEOF())//if a match for actual file name is found
		{
			recset.Close();
		SqlString="SELECT * "
				"FROM Dup WHERE Extension ='"+ext+"'";
		recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);
		if(!recset.IsEOF())//if a match for extension  is found
		{
			recset.Close();
			SqlString="SELECT * "
				"FROM Dup WHERE Type ='"+type+"'";
			recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);
			if(!recset.IsEOF())//a match for type is found
			{
				recset.Close();
				database.Close();
			return 1;
			}

		}
		
			
		}
		else
		{
		recset.Close();
		SqlString="SELECT * "
				"FROM Dup WHERE Extension ='"+ext+"'";
		recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);
		if(!recset.IsEOF())//if a match for file name  is found
		{
			recset.Close();
			SqlString="SELECT * "
				"FROM Dup WHERE Size ='"+size+"'";
			recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);
			if(!recset.IsEOF())//a match for size is found
			{
				recset.Close();
				SqlString="SELECT * "
				"FROM Dup WHERE CTime ='"+crtime+"'";
			recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);
			if(!recset.IsEOF())//a match for extension is found
			{
			recset.Close();
			database.Close();
			return 1;
			}
			}

		}
		}
		//CHECKING FOR COPYRIGHT HEREAFTER
		recset.Close();
		SqlString= "SELECT * "
				"FROM Copy WHERE CTime ='"+crtime+"'";

		recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);

		if(!recset.IsEOF())//if a match for file creation time is found
		{
			recset.Close();
		SqlString="SELECT * "
				"FROM Copy WHERE Extension ='"+ext+"'";
		recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);
		if(!recset.IsEOF())//if a match for extension  is found
		{
			recset.Close();
			SqlString="SELECT * "
				"FROM Copy WHERE Type ='"+type+"'";
			recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);
			if(!recset.IsEOF())//a match for type is found
			{
				recset.Close();
				database.Close();
			return 2;
			}

		}
		
			
		}
		else
		{
		recset.Close();
		SqlString="SELECT * "
				"FROM Dup WHERE CTime ='"+crtime+"'";
		recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);
		if(!recset.IsEOF())//if a match for file name  is found
		{
			recset.Close();
			SqlString="SELECT * "
				"FROM Dup WHERE Size ='"+size+"'";
			recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);
			if(!recset.IsEOF())//a match for size is found
			{
				recset.Close();
				SqlString="SELECT * "
				"FROM Dup WHERE Extension ='"+ext+"'";
			recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);
			if(!recset.IsEOF())//a match for extension is found
			{
			recset.Close();
			database.Close();
			return 2;
			}
			}
		}
		}
		SqlString="INSERT INTO Dup VALUES('"+aname+"','"+fname+"','"+size+"','"+type+"','"+ext+"','"+crtime+"')";
		database.ExecuteSQL(SqlString);
		database.Close();
		//SqlString =  "INSERT INTO Register VALUES ('"+name+"','"+pwd+"','"+email+"','"+interests+"','"+loc+"','"+state+"')";
		//database.ExecuteSQL(SqlString);
		return 0;	  
		
}
CATCH(CDBException, e)
	{
		// If a database exception occured, show error msg
		//AfxMessageBox("Database error: "+e->m_strError);
		return 3;
	}
	END_CATCH;

}



/*int CABTServerDlg::analyse_filedesc(CString param)
{
	if(check_filecheck(param))
	{
	return 1;
	}
	return 0;
}*/
void CABTServerDlg::send_back(CString Buffer)//for sending back msgs to client
{	
	  //AfxMessageBox(Buffer);
	int buflen=strlen(Buffer);
	AfxSocketInit(NULL);
	 CSocket echoClient;  
	 if (echoClient.Create(0,SOCK_DGRAM,NULL) == 0) {
    AfxMessageBox("Create() failed");
	}
	 if (echoClient.SendTo(Buffer, buflen,420,(LPCSTR)s_IP, 0) != buflen) {
    AfxMessageBox("SendTo() sent a different number of bytes than expected");
  }

	 echoClient.Close();

}

void CABTServerDlg::send_login(CString Buffer)//for sending back msgs to client
{	
	  //AfxMessageBox(Buffer);
	int buflen=strlen(Buffer);
	AfxSocketInit(NULL);
	 CSocket echoClient;  
	 if (echoClient.Create(0,SOCK_DGRAM,NULL) == 0) {
    AfxMessageBox("Create() failed");
	}
	 if (echoClient.SendTo(Buffer, buflen,421,(LPCSTR)s_IP, 0) != buflen) {
    AfxMessageBox("SendTo() sent a different number of bytes than expected");
  }

	 echoClient.Close();

}

/*int CABTServerDlg::check_filecheck(CString desc)// checks for duplication
{
	CString strLine;CString srt;
	srt=desc;
    CStdioFile f;

if (f.Open(fnm, CFile::modeReadWrite | CFile::shareDenyNone))
{
	char *pch;
	char* context1	= NULL;
	char* context2	= NULL;
	char *pcj;
  while (f.ReadString(strLine))
  {  
	ch_fname=false;
	ch_aname=false;
	ch_size=false;
	ch_count=3;
	pcj=strtok_s(strLine.GetBuffer()," !",&context1);
	pch=strtok_s(desc.GetBuffer()," !",&context2);
	while (pch != NULL)
{
  if(strcmp(pcj,pch)==0&&ch_count==3)
	  ch_fname=true;
  else if(strcmp(pcj,pch)==0&&ch_count==2)
	  ch_aname=true;
  else if(strcmp(pcj,pch)==0&&ch_count==1)
	  ch_size=true;
  if(ch_size)
  if(ch_fname||ch_aname)
	  return 0;
  
  pcj=strtok_s(NULL," !",&context1);
  pch = strtok_s(NULL," !",&context2);
  ch_count--;
}
  }
  
  
 
}
f.WriteString("\n");
f.WriteString(srt);
	return 1;
}
*/

int CABTServerDlg::authenticate(CString str)//for checking username and password
{
	CString name,pwd,pw;
	pch=strtok_s(str.GetBuffer(),":",&context);
	if(pch!=NULL)
	name=pch;
	pch=strtok_s(NULL,":",&context);
	if(pch!=NULL)
	pwd=pch;
	sDriver = "MICROSOFT ACCESS DRIVER (*.mdb)";
	sFile = "C:\\ABT\\Mansion.mdb"; 
	sDsn.Format("ODBC;DRIVER={%s};DSN='';DBQ=%s",sDriver,sFile);
	TRY
	{

		database.Open(NULL,false,false,sDsn);
		CRecordset recset( &database );
		SqlString =  "SELECT * "
				"FROM Register WHERE UName='"+name+"'";
		recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);
		    if(!recset.IsEOF())
			{recset.GetFieldValue("Password",pw);
			if(pw!=pwd)
			{   recset.Close();
				database.Close();
				return 0;}
			else 
			{recset.Close();
				database.Close();
				return 1;}
			database.Close();}
			else
			{recset.Close();
				database.Close();
				return 0;}
		
		return 1;	  
		
}
CATCH(CDBException, e)
	{
		// If a database exception occured, show error msg
		//AfxMessageBox("Database error: "+e->m_strError);
		return 0;
	}
	END_CATCH;


	return 0;

}


int CABTServerDlg::register_user(CString info,CString ip)
{
	CString name,pwd,email,interests,country,state;
	pch=strtok_s(info.GetBuffer(),":",&context);
	if(pch!=NULL)
	name=pch;
	pch=strtok_s(NULL,":",&context);
	if(pch!=NULL)
	pwd=pch;
	pch=strtok_s(NULL,":",&context);
	if(pch!=NULL)
	email=pch;
	pch=strtok_s(NULL,":",&context);
	if(pch!=NULL)
	interests=pch;
	pch=strtok_s(NULL,":",&context);
	if(pch!=NULL)
	country=pch;
	pch=strtok_s(NULL,":",&context);
	if(pch!=NULL)
	state=pch;
	if(!add_user(name,pwd,email,interests,country,state))
	return 0;
	return 1;
}


int CABTServerDlg::add_user(CString name,CString pwd,CString email,CString interests,CString loc,CString state)
{
	sDriver = "MICROSOFT ACCESS DRIVER (*.mdb)";
	sFile = "C:\\ABT\\Mansion.mdb"; 
	sDsn.Format("ODBC;DRIVER={%s};DSN='';DBQ=%s",sDriver,sFile);
	TRY
	{

		database.Open(NULL,false,false,sDsn);
		SqlString =  "INSERT INTO Register VALUES ('"+name+"','"+pwd+"','"+email+"','"+interests+"','"+loc+"','"+state+"')";
		database.ExecuteSQL(SqlString);
		database.Close();
		return 1;	  
		
}
CATCH(CDBException, e)
	{
		// If a database exception occured, show error msg
		//AfxMessageBox("Database error: "+e->m_strError);
		return 0;
	}
	END_CATCH;
	
}

int CABTServerDlg::check_username(CString name)
{
	CString nm;
	sDriver = "MICROSOFT ACCESS DRIVER (*.mdb)";
	sFile = "C:\\ABT\\Mansion.mdb"; 
	sDsn.Format("ODBC;DRIVER={%s};DSN='';DBQ=%s",sDriver,sFile);
TRY
	{
		database.Open(NULL,false,false,sDsn);
			
		CRecordset recset( &database );

		SqlString =  "SELECT * "
				"FROM Register WHERE UName='"+name+"'";
		
		recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);
		    if(!recset.IsEOF())
			{recset.GetFieldValue("UName",nm);
			//recset.GetFieldValue("Location",sCategory);
			recset.Close();
			database.Close();
			return 0;}
			else
			{
				recset.Close();
				database.Close();
				return 1;
			}
		
}
CATCH(CDBException, e)
	{
		// If a database exception occured, show error msg
		AfxMessageBox("Database error: "+e->m_strError);
	}
	END_CATCH;
}

BOOL CABTServerDlg::OnEraseBkgnd(CDC* pDC)
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
