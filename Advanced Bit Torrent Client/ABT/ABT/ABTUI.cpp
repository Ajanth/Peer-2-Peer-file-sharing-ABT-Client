// ABTUI.cpp : implementation file
//

#include "stdafx.h"
#include "ABT.h"
#include "ABTUI.h"
#include "ABTDlg.h"
#include "afxdialogex.h"
#include "DlgMakeTorrent.h"
#include "VCheck.h"
#include "DlgMTProgress.h"
#include "SBDestination.h"
#include "ADDPEER.h"

#define PERCENT(n, d) ((100 * (double)(n)) / (double)(d))
#define ECHOMAX 1024
#define SERV_PORT 515
#define CLI_PORT 514
#define FILE_TRANSFER_PORT 8002
#define FILE_TRANSFER_PORTA 8001
#define RECV_BUFFER_SIZE 4096

// ABTUI dialog


IMPLEMENT_DYNAMIC(ABTUI, CDialogEx)
	
	HANDLE thr,fs_thr,fc_thr;//for UDP comms
	CString client_two_ip;
	unsigned long id1,fs_id,fc_id;////for UDP comms
	int ABTUI::counter;
    int ABTUI::cur_sel;
    CString strValue, strValue1,strValue2;
    DWORD A;
    DWORD B;
    DWORD C;
    DWORD D;
   struct ABTUI::list* ABTUI::head;
   ABTUI::list* temp1;//for use in update_download_queue function only
   ABTUI::list* temp2;//for use in update_context function only
   ABTUI::list *temp5;//for temp use
ABTUI::ABTUI(CWnd* pParent /*=NULL*/)
	: CDialogEx(ABTUI::IDD, pParent)
	, details_downloaded(_T(""))
	, details_uploaded(_T(""))
	, chat_history(_T(""))
{
	
}

void ABTUI::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctrlList);
	DDX_Control(pDX, IDC_LIST4, peer_listCtrl);
	//DDX_Control(pDX, IDC_LIST3, tracker_listCtrl);
	DDX_Control(pDX, IDC_IP, selected_peer);
	//DDX_Control(pDX, IDC_LIST2, friends_list);
	//DDX_Control(pDX, IDC_FRND, fri_ajanth);
	DDX_Control(pDX, IDC_FRND, fr_list);
	DDX_Control(pDX, IDC_CHAT, chat_msg);
	DDX_Control(pDX, IDC_SHARE, shared_list);
	DDX_Control(pDX, IDC_REQ, list_req);
}


BEGIN_MESSAGE_MAP(ABTUI, CDialogEx)
	ON_BN_CLICKED(IDC_DOWNLOAD, &ABTUI::OnBnClickedDownload)
	ON_BN_CLICKED(IDC_BUTTON1, &ABTUI::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &ABTUI::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDC_BUTTON2, &ABTUI::OnBnClickedButton2)
	ON_NOTIFY(NM_RCLICK, IDC_LIST4, &ABTUI::OnNMRClickList4)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST4, &ABTUI::OnLvnItemchangedList4)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &ABTUI::OnHdnItemclickList4)
	ON_NOTIFY(NM_CLICK, IDC_LIST4, &ABTUI::OnNMClickList4)
	ON_COMMAND(ID_MENU_VIEWPROFILE, &ABTUI::OnMenuViewprofile)
	ON_BN_CLICKED(IDOK, &ABTUI::OnBnClickedOk)
	ON_COMMAND(ID_FRIEND_SENDMESSAGE, &ABTUI::OnFriendSendmessage)
	ON_BN_CLICKED(IDC_BUTTON3, &ABTUI::OnBnClickedButton3)
	ON_NOTIFY(NM_CLICK, IDC_FRND, &ABTUI::OnNMClickFrnd)
	ON_NOTIFY(NM_CLICK, IDC_SHARE, &ABTUI::OnNMClickShare)
	ON_COMMAND(ID_AGAIN_AJANTH, &ABTUI::OnAgainAjanth)
	ON_COMMAND(ID_FRIEND_SHAREFOLDER, &ABTUI::OnFriendSharefolder)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_CLOSE, &ABTUI::OnBnClickedClose)
	ON_BN_CLICKED(IDC_RESUME, &ABTUI::OnBnClickedResume)
	ON_BN_CLICKED(IDC_PAUSE, &ABTUI::OnBnClickedPause)
	ON_COMMAND(ID_FRIEND_VIEWACTIVITY, &ABTUI::OnFriendViewactivity)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDCANCEL, &ABTUI::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_MIN, &ABTUI::OnBnClickedMin)
	ON_COMMAND(ID_AGAIN_CLOSE, &ABTUI::OnAgainClose)
	ON_COMMAND(ID_MENU_ADDPEER, &ABTUI::OnMenuAddpeer)
END_MESSAGE_MAP()




BOOL ABTUI::OnInitDialog()
{
	client_two_ip="192.168.1.10";
	thread_flag=true;
	//GetDlgItem(IDD_LOGIN)->ShowWindow(SW_HIDE);
	SetTimer( 2008, 1000, NULL );
	thr=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ReceiveData,this,NULL,&id1);//creating listening server thread
	CStdioFile i;
	CString nm;
	if(i.Open("C:\\ABT\\ABT.inf",CFile::modeReadWrite | CFile::shareDenyNone))
	{
	i.ReadString(nm);
	if(nm=="")
		AfxMessageBox("Error reading config...IO returned Empty");
	i.Close();
	}
	
	if(i.Open("C:\\ABT\\Activity.inf",CFile::modeCreate|CFile::shareDenyNone))
	{
	i.Close();}
	
	head=NULL;
	Bacground.LoadBitmap(IDB_BITMAP3);
	Bacground.GetBitmap(&bm);
	bitmapSize = CSize(bm.bmWidth, bm.bmHeight);
	Invalidate(1);
	view_profile=false;
	view_shared=false;
	reqfile=false;
	view_activity=false;
	CDialog::OnInitDialog();
	shared_list.ShowWindow(SW_HIDE);
	m_ctrlList.Init();
	m_ctrlList.InsertColumn(0, "Download Progress", LVCFMT_CENTER, 180);
	m_ctrlList.InsertColumn(1, "Name", LVCFMT_CENTER, 370);
	m_ctrlList.InsertColumn(2, "Download rate", LVCFMT_CENTER, 100);
	m_ctrlList.InsertColumn(3, "upload rate", LVCFMT_CENTER, 80);
	m_ctrlList.InsertColumn(4, "ETA", LVCFMT_CENTER, 90);
	m_ctrlList.InsertColumn(5, "status", LVCFMT_CENTER, 90);
	m_ctrlList.InsertColumn(6, "Downloaded", LVCFMT_CENTER, 90);
	m_ctrlList.InsertColumn(7, "Uploaded", LVCFMT_CENTER, 90);
	
	chat_msg.InsertColumn(0,"Message",LVCFMT_CENTER,200);
	//m_ctrlList.InsertColumn(5, "status", LVCFMT_LEFT, 180);
	//m_ctrlList.InsertColumn(6, "File name", LVCFMT_LEFT, 180);
	peer_listCtrl.InsertColumn(0,"Peer IP Address", LVCFMT_CENTER,110);
	peer_listCtrl.InsertColumn(1,"Port", LVCFMT_CENTER,50);
	m_ImageList=new CImageList();
	m_ImageList->Create(32,32,ILC_COLOR32,0,0);
	HICON hIcon=AfxGetApp()->LoadIcon(IDI_ICON1);
	m_ImageList->Add(hIcon);
	menu.LoadMenuA(IDR_MENU1);//popup menu creation for peer_lists
	sub = menu.GetSubMenu(0);//popup menu creation for peer_lists
	menu1.LoadMenuA(IDR_MENU3);
	sub1=menu1.GetSubMenu(0);
	menu2.LoadMenuA(IDR_MENU2);
	sub2=menu2.GetSubMenu(0);
    peer_listCtrl.InsertItem(0,"127.0.0.1");
	peer_listCtrl.InsertItem(0,"117.222.146.162");
	fr_list.InsertColumn(0,"Friends",LVCFMT_CENTER,150);
	fr_list.SetBkColor(RGB(0,0,0));
	fr_list.SetTextColor(RGB(0,245,0));
	fr_list.SetTextBkColor(RGB(0,0,0));
	peer_listCtrl.SetBkColor(RGB(20,20,0));
	peer_listCtrl.SetTextColor(RGB(255,255,255));
	peer_listCtrl.SetTextBkColor(RGB(20,20,0));
if(!StartContext())
{AfxMessageBox("failed to initialize context");
return false;
}
CString fl;
CStdioFile f;
if (f.Open("C:\\ABT\\friends.inf", CFile::modeReadWrite | CFile::shareDenyNone))
			{
			while (f.ReadString(fl))
			{
			pch=strtok_s(fl.GetBuffer(),"-",&context);
			fr_list.InsertItem(0,pch);
			}
			f.Close();
			}
dload.Id_Assigner(IDC_DOWNLOAD);
dload.SubclassDlgItem(IDC_DOWNLOAD,this);
bupload.Id_Assigner(IDC_BUTTON1);
bupload.SubclassDlgItem(IDC_BUTTON1,this);
bpause.Id_Assigner(IDC_PAUSE);
bpause.SubclassDlgItem(IDC_PAUSE,this);
bplay.Id_Assigner(IDC_RESUME);
bplay.SubclassDlgItem(IDC_RESUME,this);
bclose.Id_Assigner(IDC_CLOSE);
bclose.SubclassDlgItem(IDC_CLOSE,this);
bmin.Id_Assigner(IDC_MIN);
bmin.SubclassDlgItem(IDC_MIN,this);
GetDlgItem(IDC_LIST4)->ShowWindow(SW_HIDE);
Font1.CreateFont(18,                           // Height
                0,                             // Width
                0,                             // Escapement
                0,                             // Orientation
                FW_BOLD,                       // Weight
                FALSE,                         // Italic
                FALSE,                          // Underline
                0,                             // StrikeOut
                ANSI_CHARSET,                  // CharSet
                OUT_DEFAULT_PRECIS,            // OutPrecision
                CLIP_DEFAULT_PRECIS,           // ClipPrecision
                DEFAULT_QUALITY,               // Quality
                DEFAULT_PITCH | FF_SWISS,      // PitchAndFamily
                "Arial");
GetDlgItem(IDC_MYNAME)->SetFont(&Font1);
nm="Logged in as: "+nm;
SetDlgItemText(IDC_MYNAME,nm);
return true;
}
ABTUI::~ABTUI()
{
}



// ABTUI message handlers

UINT thread(LPVOID)
{

	
    //AfxEndThread(0);
	return 0;
}
BOOL ABTUI::StartContext()
{
	ASSERT( !FTK_Context_IsInit() );
	CString strPath;
	   ::GetModuleFileName( AfxGetInstanceHandle(),
                         strPath.GetBuffer( _MAX_PATH ), _MAX_PATH );
    strPath.ReleaseBuffer();
	int nPos = strPath.ReverseFind( _T('\\') );
	ASSERT( -1 != nPos );
	strPath = strPath.Left( nPos + 1 );
	strPath = strPath+ _T("Config.ini");
    if ( !FTK_Context_Init( (strPath.GetBuffer(strPath.GetLength()))))
    {
		return FALSE;
    }

    ASSERT( FTK_Context_IsRunning() == FALSE );
    FTK_Context_Run();
    return FTK_Context_IsRunning() == TRUE;
}
int ABTUI::allow_download()
{
CStdioFile f;
CString sdown,sup;
int down,up;
if (f.Open("C:\\ABT\\monitor.inf", CFile::modeReadWrite | CFile::shareDenyNone)){
	f.ReadString(sdown);
	f.ReadString(sup);
	down=atoi(sdown.GetBuffer());
	up=atoi(sup.GetBuffer());
	f.Close();
	if(down/up>2)
    return 0;
	return 1;
	f.Close();


}

}
void ABTUI::OnBnClickedDownload()//when download button is clicked
{
	if(allow_download()){
	CABTDlg *dlg=new CABTDlg();
	dlg->Create(IDD_ABT_DIALOG);
	dlg->ShowWindow(SW_SHOW);
	}
	else
		AfxMessageBox("Please maintain atleast 1:2 seed ratio to continue downloading");
}
int ABTUI::add_download_queue(HTorrentFile tfile, HDownloader hdown)
{
	double uplimit=100,downlimit=75;
               if(head==NULL)
						{
							list *temp;		
							counter=0;
							temp = (list*)malloc(sizeof(list));	
							temp->cur_fil = tfile;
							temp->cur_tor = hdown;
							temp->id=counter++;
							ABTUI::m_ctrlList.InsertItem(temp->id,"0 %%");
							ABTUI::m_ctrlList.SetItemText(temp->id,1,FTK_Torrent_GetTorrentName(temp->cur_fil));
							temp->next = NULL;				
							head = temp;	
							FTK_Downloader_SetFixMaxDownloadRate( head->cur_tor, downlimit );
							FTK_Downloader_SetFixMaxUploadRate( head->cur_tor, uplimit);
							//AfxMessageBox("torrent added to queue one");
							
							return 1;
						}

						else
						{
							list *temp1;						// create a temporary node
							temp1=(list*)malloc(sizeof(list));	// allocate space for node
							temp1 = head;					// transfer the address of 'head' to 'temp'
							while(temp1->next!=NULL)			// go to the last node
								temp1 = temp1->next;			//tranfer the address of 'temp->next' to 'temp'
							list *temp;					// create a temporary node
							temp = (list*)malloc(sizeof(list));// allocate space for node
							temp->cur_fil = tfile;	
							temp->cur_tor = hdown;
							temp->id=counter++;
							ABTUI::m_ctrlList.InsertItem(temp->id,"0 %%");
							ABTUI::m_ctrlList.SetItemText(temp->id,1,FTK_Torrent_GetTorrentName(temp->cur_fil));
							//AfxMessageBox(FTK_Torrent_GetTorrentName(temp->cur_fil));
							temp->next = NULL;				
							temp1->next = temp;		
							FTK_Downloader_SetFixMaxDownloadRate( temp->cur_tor, downlimit );
							FTK_Downloader_SetFixMaxUploadRate( temp->cur_tor, uplimit);// 'temp' node will be the last node
							//AfxMessageBox("torrent added to queue again");
							return 1;
						}
					}

void ABTUI::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	//AfxMessageBox("timer entered");
	//strPercent.Format("%d %%",ABTUI::dcount++);
	//ABTUI::m_ctrlList.SetItemText(0,0,strPercent);
	update_download_queue();
	
	CDialog::OnTimer(nIDEvent);
}
void ABTUI::update_download_queue()
{

	if(ABTUI::head==NULL)
		return;
	temp1=ABTUI::head;
	while( temp1!=NULL )
		{
		if ( NULL == temp1->cur_tor )
	    {
		AfxMessageBox("NULL downloader");
		
	    }
		QWORD qwTotalFileHaveSize = FTK_Stat_GetTotalFileHaveSize(temp1->cur_tor);
		int nPos = PERCENT( (INT64)qwTotalFileHaveSize, (INT64)FTK_Torrent_GetFileSize(temp1->cur_fil));
		dd=FTK_Stat_GetDownloaded(temp1->cur_tor)/1024;
		mytemp.Format("%lu mb",dd/1024);
		

		if ( FTK_Downloader_GetState(temp1->cur_tor) == DLSTATE_PAUSING )
		{
		strValue = _T("paused.....");	
		ABTUI::m_ctrlList.SetItemText(temp1->id, 5,strValue);
		//AfxMessageBox(strValue);
		}
		else if( FTK_Downloader_GetState(temp1->cur_tor) == DLSTATE_CHECKING )
	      {
		strValue = _T("checking file...");
		ABTUI::m_ctrlList.SetItemText(temp1->id, 5, strValue);
	      }
		else if ( FTK_Downloader_GetState(temp1->cur_tor) == DLSTATE_DOWNLOAD )
		{
		strValue = _T("downloading...");	
		ABTUI::m_ctrlList.SetItemText(temp1->id, 5,strValue);
		strValue.Format( _T("%s"), nPos < 100 ? _T("Downloading..") : _T("Seeding.."));
		ABTUI::m_ctrlList.SetItemText(temp1->id, 5 , strValue);
		//AfxMessageBox(strValue);
		}
		
		else if ( FTK_Downloader_GetState(temp1->cur_tor) == DLSTATE_FETALERR )
		{
		strValue = _T("downloading error...");	
		ABTUI::m_ctrlList.SetItemText(temp1->id, 5, strValue);
		//AfxMessageBox(strValue);
		}
		else if ( FTK_Downloader_GetState(temp1->cur_tor) == DLSTATE_TERMINATE )
		{
		strValue = _T("stopping...");	
		ABTUI::m_ctrlList.SetItemText(temp1->id, 5, strValue);
		//AfxMessageBox(strValue);
		}
		
		
		strValue.Format("%d %%",nPos);
		ABTUI::m_ctrlList.SetItemText(temp1->id, 0 , strValue);
		strValue = AfxFormatBytes( FTK_Stat_GetDownloadRate(temp1->cur_tor) );//showing download rate in list ctrl
		ABTUI::m_ctrlList.SetItemText(temp1->id, 2 , strValue);
		strValue = AfxFormatBytes( FTK_Stat_GetUploadRate(temp1->cur_tor) );
		ABTUI::m_ctrlList.SetItemText(temp1->id, 3 , strValue);
		ABTUI::m_ctrlList.SetItemText(temp1->id,6,mytemp);
		strValue.Format( _T( "%ld %s %ld %s %ld %s" ),
                     ( UINT32 ) FTK_Stat_GetLeftTime(temp1->cur_tor) / ( 60 * 60 ),
                     _T("h"),
                     ( ( UINT32 ) FTK_Stat_GetLeftTime(temp1->cur_tor) % ( 60 * 60 ) ) / 60,
                     _T("m"),
                     ( ( UINT32 ) FTK_Stat_GetLeftTime(temp1->cur_tor) % ( 60 * 60 ) ) % 60,
                     _T("s") );//showing time left for download
	    ABTUI::m_ctrlList.SetItemText(temp1->id, 4 , strValue);
		temp1->down=FTK_Stat_GetDownloaded(temp1->cur_tor);
		temp1->up=FTK_Stat_GetUploaded(temp1->cur_tor);
		
		
		
			temp1 = temp1->next;				// tranfer the address of 'temp->next' to 'temp'
		}


}
void ABTUI::OnBnClickedButton1()//when make torrent button is clicked
{
	CDlgMakeTorrent cobj;
	int i=cobj.DoModal();
	if(i==IDOK)
	{
	CVCheck obj;
		int nret=obj.DoModal();
		if(nret==IDOK)
			{
				CDlgMTProgress dlg2;
		int ret=dlg2.DoModal();
		if(ret==IDOK)
		{
		AfxMessageBox("Add torrent to queue to start uploading");
		}

		}
	}
}

void ABTUI::update_context()
{
	peer_listCtrl.DeleteAllItems();
int context_id=ABTUI::cur_sel;
temp2=ABTUI::head;
while( temp2!=NULL )
{
	if(temp2->id==context_id)
	{
		//AfxMessageBox("matched click");
	peer_max=25;
	peer_ret=0;
	FTK_Stat_GetMiniPeerData(temp2->cur_tor, peer_list ,peer_ret , peer_max);
	for(int i=0;i<peer_ret;i++){
    A = peer_list[i].m_dwIP & 0x000000FF;
    B = peer_list[i].m_dwIP & 0x0000FF00;
    C = peer_list[i].m_dwIP & 0x00FF0000;
    D = peer_list[i].m_dwIP & 0xFF000000;
    B = B >> 8;
    C = C >> 16;
    D = D >> 24;
    strValue1.Format("%d.%d.%d.%d", A, B, C, D);
	peer_listCtrl.InsertItem(0,strValue1);
	strValue2.Format("%lu" ,peer_list[i].m_nPort);
	peer_listCtrl.SetItemText(0,1,strValue2);}
	}
	temp2 = temp2->next;
}
}
CString ABTUI::iptoa(DWORD ip)
{
	return "";
}
void ABTUI::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)//when torrent is clicked in main list
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
		*pResult = 0;
    int row = ABTUI::m_ctrlList.GetSelectionMark();
	if(ABTUI::cur_sel!=row)
	{
		ABTUI::cur_sel=row;
		match_FTK(row);
	    update_context();
	}
	
}
void ABTUI::match_FTK(int row)//to match HDownloader to currently slected row in the mainlist
{
	temp5=ABTUI::head;
	while(temp5!=NULL)
	{
	if(temp5->id==row)
	{   //AfxMessageBox("matched");
		cur_click = temp5->cur_tor;
		GetDlgItem(IDC_LIST4)->ShowWindow(SW_SHOW);
	}
	temp5=temp5->next;
	}
}

void ABTUI::OnBnClickedButton2()
{   if(!peer_listCtrl.IsWindowVisible())
{peer_listCtrl.ShowWindow(SW_SHOW);
gb_details.ShowWindow(SW_SHOW);
}
else
{peer_listCtrl.ShowWindow(SW_HIDE);
gb_details.ShowWindow(SW_HIDE);
}
}


void ABTUI::OnNMRClickList4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void ABTUI::OnContextMenu(CWnd* pWnd, CPoint point)
{
CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST4);//peer list
CListCtrl* pList2 = (CListCtrl*)GetDlgItem(IDC_FRND);//friend list
CListCtrl* pList3 = (CListCtrl*)GetDlgItem(IDC_SHARE);//shared folder list

if (pList ==(CListCtrl*)pWnd )
    {   

        
    mretVal	= sub->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_LEFTBUTTON , point.x, point.y, this );
		
    }
else if(pList2 == (CListCtrl *)pWnd)
{mretVal	= sub1->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_LEFTBUTTON , point.x, point.y, this );

}
else if(pList3==(CListCtrl *)pWnd)
	mretVal=sub2->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_LEFTBUTTON , point.x, point.y, this );
}


void ABTUI::OnLvnItemchangedList4(NMHDR *pNMHDR, LRESULT *pResult)//when mouse button is clicked inside peer list ctrl
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	//peer_row=peer_listCtrl.GetSelectionMark();
	
	//peer_IP=peer_listCtrl.GetItemText(peer_row,0);
	//AfxMessageBox(peer_IP);
	//selected_peer.SetWindowTextA(peer_IP);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void ABTUI::OnHdnItemclickList4(NMHDR *pNMHDR, LRESULT *pResult)//use for sorting the peer list ctrl items
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void ABTUI::OnNMClickList4(NMHDR *pNMHDR, LRESULT *pResult)//when peer list is clicked 
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	peer_row=peer_listCtrl.GetSelectionMark();
	Invalidate();
	peer_listCtrl.SetItemState(peer_row,LVIS_SELECTED,LVIS_SELECTED);
	peer_IP1=peer_listCtrl.GetItemText(peer_row,0);
	if(peer_IP!=peer_IP1)
	{peer_IP=peer_IP1;
		selected_peer.SetWindowTextA(peer_IP);}
	Invalidate();
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
int ABTUI::send_profile_data(CString _ip)// function to send profile data to requestor
{
	CString strLine;
    CStdioFile f;
if (f.Open("C:\\ABT\\Profile.inf", CFile::modeRead | CFile::shareDenyNone))
{
  while (f.ReadString(strLine))
  {  
	  OnSend(strLine, _ip);
  }
  return 1;
}
	return 0;
}
void ABTUI::send_shared_filelist(CString ip)
{
	//AfxMessageBox("sending shared list");
CString list;
WIN32_FIND_DATA ffd;
HANDLE hFind = INVALID_HANDLE_VALUE;
DWORD dwError=0;
CString strLine;
CStdioFile f;
if (f.Open("C:\\ABT\\shared.inf", CFile::modeRead | CFile::shareDenyNone))
{
 f.ReadString(strLine);
 shared_path=strLine;
 strLine=strLine+"\\*.torrent";
	  hFind = FindFirstFile(strLine, &ffd);
	   if (INVALID_HANDLE_VALUE == hFind) 
		{ AfxMessageBox("FindFirstFile error");
	   return;}
	   list = ffd.cFileName;
	   list+=":";
   while (FindNextFile(hFind, &ffd)!= 0)
   {
   list=list+ffd.cFileName;
		list+=":";
   }
   OnSend(list,ip);

  
}
}
UINT File_client(LPVOID pParam)
{
	ABTUI *dlg=(ABTUI*)pParam;
	AfxSocketInit(NULL);
	CSocket sockClient;
	sockClient.Create();
	CString strIP= client_two_ip;
	sockClient.Connect( strIP, FILE_TRANSFER_PORTA );
	BOOL bRet = TRUE;
	int dataLength, cbBytesRet, cbLeftToReceive;	// used to monitor the progress of a receive operation
	
	BYTE* recdData = NULL;	// pointer to buffer for receiving data (memory is allocated after obtaining file size)
	
	CFile destFile;
	CFileException fe;
	BOOL bFileIsOpen = FALSE;
	if( !( bFileIsOpen = destFile.Open(dlg->client_file_name, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary, &fe ) ) )
	{
		AfxMessageBox("error opening local file to write");		
		bRet = FALSE;
		goto PreReturnCleanup;
	}
	cbLeftToReceive = sizeof( dataLength );
	
	do
	{
		BYTE* bp = (BYTE*)(&dataLength) + sizeof(dataLength) - cbLeftToReceive;
		cbBytesRet = sockClient.Receive( bp, cbLeftToReceive );
		
		// test for errors and get out if they occurred
		if ( cbBytesRet == SOCKET_ERROR || cbBytesRet == 0 )
		{
			
			
			AfxMessageBox("error while receiving file length from server ekk");			
			bRet = FALSE;
			goto PreReturnCleanup;
		}
		
		// good data was retrieved, so accumulate it with already-received data
		cbLeftToReceive -= cbBytesRet;
		
	}
	while ( cbLeftToReceive > 0 );
	dataLength = ntohl( dataLength );

	// now get the file in RECV_BUFFER_SIZE chunks at a time

	recdData = new byte[RECV_BUFFER_SIZE];
	cbLeftToReceive = dataLength;
	do
	{	
		int iiGet, iiRecd;
		
		iiGet = (cbLeftToReceive<RECV_BUFFER_SIZE) ? cbLeftToReceive : RECV_BUFFER_SIZE ;
		iiRecd = sockClient.Receive( recdData, iiGet );
		
		// test for errors and get out if they occurred
		if ( iiRecd == SOCKET_ERROR || iiRecd == 0 )
		{
			AfxMessageBox("error while gettin file length from server 099");			
			bRet = FALSE;
			goto PreReturnCleanup;
		}
		destFile.Write( recdData, iiRecd); // Write it
		cbLeftToReceive -= iiRecd;
		
	} 
	while ( cbLeftToReceive > 0 );
	PreReturnCleanup:		// labelled "goto" destination
	// free allocated memory
	// if we got here from a goto that skipped allocation, delete of NULL pointer
	// is permissible under C++ standard and is harmless
	delete[] recdData;		
	if ( bFileIsOpen )
		destFile.Close();	// only close file if it's open (open might have failed above)
	sockClient.Close();
	if(!bRet)
		dlg->Display_download_result("Torrent Download failed");
	else
		dlg->Display_download_result("Torrent Download successful. open C://ABT to find torrent");
	return bRet;

}


UINT File_server(LPVOID pParam)// thread for sending torrent files 
{
	ABTUI *dlg=(ABTUI*)pParam;
	AfxSocketInit(NULL);
	CString fName;//hold the source file path
	CSocket sockSrvr; 
	sockSrvr.Create(FILE_TRANSFER_PORT); // Creates our server socket
	sockSrvr.Listen(); // Start listening for the client at PORT
	CSocket sockConnection;
	sockSrvr.Accept(sockConnection); 
	int fileLength, cbLeftToSend;	// used to monitor the progress of a sending operation
	BYTE* sendData = NULL;	// pointer to buffer for sending data (memory is allocated after sending file size)
	CFile sourceFile;
	CFileException fe;
	BOOL bFileIsOpen = FALSE;
	if( !( bFileIsOpen = sourceFile.Open( dlg->file_path, CFile::modeRead | CFile::typeBinary, &fe ) ) )
	{

		AfxMessageBox("Error while opening source file");
		goto PreReturnCleanup;
	}
	fileLength = sourceFile.GetLength();
	fileLength = htonl( fileLength );
	
	cbLeftToSend = sizeof( fileLength );
	do
	{
		int cbBytesSent;
		BYTE* bp = (BYTE*)(&fileLength) + sizeof(fileLength) - cbLeftToSend;
		cbBytesSent = sockConnection.Send( bp, cbLeftToSend);
		if ( cbBytesSent == SOCKET_ERROR )
		{
			int iErr = ::GetLastError();
			AfxMessageBox("Error sending file length to client");
			goto PreReturnCleanup;
		}
		cbLeftToSend -= cbBytesSent;
			}
	while ( cbLeftToSend>0 );

	sendData = new BYTE[RECV_BUFFER_SIZE];
	cbLeftToSend = sourceFile.GetLength();

	do
	{
		// read next chunk of SEND_BUFFER_SIZE bytes from file
	
		int sendThisTime, doneSoFar, buffOffset;
		
		sendThisTime = sourceFile.Read( sendData, RECV_BUFFER_SIZE );
		buffOffset = 0;
		do
		{
			doneSoFar = sockConnection.Send( sendData + buffOffset, sendThisTime); 
			if ( doneSoFar == SOCKET_ERROR )
			{
				AfxMessageBox("Error while sending file to client");
				goto PreReturnCleanup;
			}

			buffOffset += doneSoFar;
			sendThisTime -= doneSoFar;
			cbLeftToSend -= doneSoFar;
		}
		while ( sendThisTime > 0 );
		}
	while ( cbLeftToSend > 0 );
	PreReturnCleanup:		// labelled goto destination
	
	// free allocated memory
	// if we got here from a goto that skipped allocation, delete of NULL pointer
	// is permissible under C++ standard and is harmless
	delete[] sendData;
	
	if ( bFileIsOpen )
		sourceFile.Close();		// only close file if it's open (open might have failed above)
	sockConnection.Close();
	
	return 0;

}

void ABTUI::send_activity(CString ip)
{
	CString strLine,ftemp;
    CStdioFile f;
if (f.Open("C:\\ABT\\Activity.inf", CFile::modeRead | CFile::shareDenyNone))
{
	f.ReadString(strLine);
	ftemp=strLine;
  while(f.ReadString(strLine)){
      ftemp+=":";
	  ftemp+=strLine; }
}
if(ftemp=="")
	ftemp="No Activity detected";
OnSend(ftemp,ip);
}

int ABTUI::udp_command_processor(CString cmd, CString ip)
{
	//AfxMessageBox(cmd);
	if(cmd=="VIEW_PROF")
	{
	send_profile_data(ip);
	return 1;
	}
	else if(cmd=="VIEW_SHAR")
	{
	send_shared_filelist(ip);
	}
	else if(cmd=="REQFIL")
	{
		reqfile=true;
		OnSend("REQACK",ip);
		
	
	}
	else if(cmd=="REQACK")
	{
	OnSend(req_file_name,ip);
	}
	else if(cmd=="VIEW_ACTIVITY")
	{
	send_activity(ip);
	}
	else if(cmd=="DENY")
	{
	AfxMessageBox("Your file request has been denied by the user");
	}
	else if(cmd=="NODENY")
	{
		Sleep(1000);
	fc_thr=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)File_client,this,NULL,&fc_id);
	}
return 1;
}
void ABTUI::OnSend(CString Buffer, CString s_IP) //to send msgs to client
{
	if(Buffer=="VIEW_PROF")
	{view_profile=true;
	prof_count=8;
	}
	else if(Buffer=="VIEW_SHAR")
	view_shared=true;
	else if(Buffer=="VIEW_ACTIVITY")
		view_activity=true;
	int buflen=strlen(Buffer);
	AfxSocketInit(NULL);
	 CSocket echoClient;  
	 if (echoClient.Create(0,SOCK_DGRAM,NULL) == 0) {
    AfxMessageBox("Create() failed");
	}
	 if (echoClient.SendTo(Buffer, buflen,CLI_PORT,
      (LPCSTR)s_IP, 0) != buflen) {
    AfxMessageBox("SendTo() sent a different number of bytes than expected");
  }

	 echoClient.Close();

}
void ABTUI::OnSendChat(CString Buffer, CString s_IP) //to send msgs to client
{
	int buflen=strlen(Buffer);
	AfxSocketInit(NULL);
	 CSocket echoClient;  
	 if (echoClient.Create(0,SOCK_DGRAM,NULL) == 0) {
    AfxMessageBox("Create() failed");
	}
	 if (echoClient.SendTo(Buffer, buflen,CLI_PORT,
      (LPCSTR)s_IP, 0) != buflen) {
    AfxMessageBox("SendTo() sent a different number of bytes than expected");
  }

	 echoClient.Close();

}
void ABTUI::OnSendme(CString Buffer, CString s_IP) //to send msgs to client
{
	int buflen=strlen(Buffer);
	AfxSocketInit(NULL);
	 CSocket echoClient;  
	 if (echoClient.Create(0,SOCK_DGRAM,NULL) == 0) {
    AfxMessageBox("Create() failed");
	}
	 if (echoClient.SendTo(Buffer, buflen,SERV_PORT,
      (LPCSTR)s_IP, 0) != buflen) {
    AfxMessageBox("SendTo() sent a different number of bytes than expected");
  }

	 echoClient.Close();

}
CString ABTUI::get_ip_of_friends(CString fname)
{
CString fl;
CStdioFile f;
if (f.Open("C:\\ABT\\friends.inf", CFile::modeReadWrite | CFile::shareDenyNone))
			{
			while (f.ReadString(fl))
			{
			pch=strtok_s(fl.GetBuffer(),"-",&context);
			if(fname==pch)
			{
			pch=strtok_s(NULL,"-",&context);
			return pch;
			}
			}
			f.Close();
			}
return "255.255.255.1";
}
CString ABTUI::friend_from_ip(CString ip)//resolve friend name using ip address
{
CString fl,temp;
CStdioFile f;
if (f.Open("C:\\ABT\\friends.inf", CFile::modeReadWrite | CFile::shareDenyNone))
			{
			while (f.ReadString(fl))
			{
			pch=strtok_s(fl.GetBuffer(),"-",&context);
			temp=pch;
			pch=strtok_s(NULL,"-",&context);
			if(ip==pch)
			{
			return temp;
			}
			}
			f.Close();
			}
return ip;
}
void ABTUI::disp_msg(CString msg,CString ip)
{
	final_ip=friend_from_ip(ip);
	chat_msg.InsertItem(chat_msg.GetItemCount()+1,final_ip+":"+msg);
}


void ABTUI::disp_filerequest(CString msg)
{
list_req.AddString(msg);
CStdioFile f;
if (f.Open("C:\\ABT\\file_req.inf", CFile::modeReadWrite | CFile::shareDenyNone))
{
	while(f.ReadString(tempc))
	{}
	f.WriteString("\n");
	f.WriteString(msg);
}
f.Close();
}


void ABTUI::add_friends(CString fr)
{
fr=fr_final;
}
void ABTUI::show_activity(CString act,CString ip)
{
act.Replace(":","\n");
AfxMessageBox(act);
}
void ABTUI::activate_shared_list(CString list)//display shared files listctrl
{
CString fl;
CStdioFile f;			
shared_list.SetImageList(m_ImageList,LVSIL_NORMAL);
			pch=strtok_s(list.GetBuffer(),":",&context);
			if(pch!="."||pch!="..")
			shared_list.InsertItem(0,pch,0);
			shared_list.SetBkColor(RGB(255,255,255));
			shared_friend_name=temp_name;
			while(pch!=NULL){
				if(pch!="."||pch!="..")
				shared_list.InsertItem(shared_list.GetItemCount()+1,pch,0);
				pch=strtok_s(NULL,":",&context);}
			view_shared=false;
			}
void ABTUI::Display_download_result(CString msg)
{
AfxMessageBox(msg);
}
UINT ReceiveData(LPVOID pParam)// thread for server
{
	ABTUI *dlg=(ABTUI*)pParam;
	AfxSocketInit(NULL);
	//CSocket echoServer;  
	
	int recvMsgSize;
	  if ( dlg->echoServer.Create(SERV_PORT, SOCK_DGRAM, NULL)== false) 
		AfxMessageBox("Create socket failed");
	for(;;) { // Run forever
    // Client address
    SOCKADDR_IN echoClntAddr; 
	int clntAddrLen;
	char echoBuffer[ECHOMAX]; 
    // Set the size of the in-out parameter
    clntAddrLen = sizeof(echoClntAddr);

    // Buffer for echo string
    // Block until receive message from a client
    recvMsgSize = dlg->echoServer.ReceiveFrom(echoBuffer, 
	  ECHOMAX, (SOCKADDR*)&echoClntAddr, &clntAddrLen, 0);
    if (recvMsgSize < 0) {
      AfxMessageBox("RecvFrom() failed");
    }
	echoBuffer[recvMsgSize]='\0';
	//AfxMessageBox(echoBuffer);
	dlg->client_IP=inet_ntoa(echoClntAddr.sin_addr);
	dlg->server_msg=echoBuffer;
	//AfxMessageBox(echoBuffer);
	if(dlg->view_profile)
	{
		switch(dlg->prof_count)
		{
			
		case 8:
			{dlg->prof.usr_name=echoBuffer;
		    dlg->prof_count--;
			break;}
		case 7:
			{dlg->prof.rl_name=echoBuffer;
		    dlg->prof_count--;
			dlg->fr_ip=echoBuffer;
			break;}
		case 6:
			{dlg->prof.e_id=echoBuffer;
		    dlg->prof_count--;
			break;}
		case 5:
			{dlg->prof.login_time=echoBuffer;
		    dlg->prof_count--;
			break;}
		case 4:
			{dlg->prof.loc=echoBuffer;
		    dlg->prof_count--;
			break;}
		case 3:
			{dlg->prof.tot_download=echoBuffer;
		    dlg->prof_count=0;
			dlg->view_profile=false;
			if(dlg->prof.DoModal()==IDOK)
			{
				CListCtrl *aa=(CListCtrl *)dlg->GetDlgItem(IDC_FRND);
			aa->InsertItem(0,dlg->fr_end);
			CString strLine;
			CStdioFile f;
			if (f.Open("C:\\ABT\\friends.inf", CFile::modeReadWrite | CFile::shareDenyNone))
			{
			while (f.ReadString(strLine))
			{}
			f.WriteString("\n");
			dlg->fr_end=dlg->fr_end+"-"+dlg->fr_ip;
			f.WriteString(dlg->fr_end);
			}
			}
			break;}		
		
		}
	
	}
	
	//AfxMessageBox(dlg->client_IP);
	else if(dlg->view_shared)
	{
	dlg->activate_shared_list(echoBuffer);
	}
	else if(dlg->reqfile)
	{
		CString fnm;
		fnm=echoBuffer;
	dlg->tempc=dlg->friend_from_ip(dlg->client_IP);
	dlg->tempc=dlg->tempc + " Has Requested your shared file :" + echoBuffer + "Allow file sharing?";
	dlg->reqfile=false;
	dlg->file_path=dlg->shared_path+"\\"+fnm;
	//dlg->disp_msg(dlg->tempc,dlg->client_IP);
	int nret=AfxMessageBox(dlg->tempc, MB_YESNO|MB_ICONEXCLAMATION);
	if(nret==IDYES)
	{
		dlg->OnSend("NODENY",dlg->client_IP);
		fs_thr=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)File_server,dlg,NULL,&fs_id);//creating listening server thread

	//add code for file transfer
	}
	else
	{
	dlg->OnSend("DENY",dlg->client_IP);
	}
	}
	else if(dlg->view_activity)
	{
	dlg->show_activity(echoBuffer,dlg->client_IP);
	dlg->view_activity=false;
	}

	else if(dlg->server_msg=="VIEW_PROF"||dlg->server_msg=="VIEW_SHAR"||dlg->server_msg=="VIEW_ACTIVITY"||dlg->server_msg=="REQFIL"||dlg->server_msg=="REQACK"||dlg->server_msg=="NODENY"||dlg->server_msg=="DENY")
		dlg->udp_command_processor(dlg->server_msg , dlg->client_IP);
	else if(dlg->server_msg=="CLOSEME")
	{
		dlg->echoServer.Close();
		AfxEndThread(0);
	}
	else 
		dlg->disp_msg(echoBuffer,dlg->client_IP);
	
	//AfxMessageBox(dlg->server_msg);

   
  }
	return 0;
}
void ABTUI::OnMenuViewprofile()//sending VIEW_PROF command to the peer selected
{
	//AfxMessageBox(peer_IP);
	OnSend("VIEW_PROF",peer_IP);
}
void ABTUI::OnBnClickedOk()
{
	
	//CDialogEx::OnOK();
}


void ABTUI::OnFriendSendmessage()//when view shared file list is clicked on a frined menu
{
	CString ip;
	GetDlgItemText(IDC_IP,ip);
	OnSend("VIEW_SHAR",ip);
	shared_list.ShowWindow(SW_SHOW);
	view_shared=true;
}


void ABTUI::OnBnClickedButton3()//when send message button is clicked
{
	CString msg,IP;
	GetDlgItemText(IDC_PRECHAT,msg);
	GetDlgItemText(IDC_IP,IP);
	if(IP=="")
	return;
	if(msg!="")
	OnSendChat(msg,IP);
	chat_msg.InsertItem(chat_msg.GetItemCount()+1,"Me:"+msg);
}


void ABTUI::OnNMClickFrnd(NMHDR *pNMHDR, LRESULT *pResult)//when mouse clicked inside friends list
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	peer_row=fr_list.GetSelectionMark();
	Invalidate();
	 fr_list.SetItemState(peer_row,LVIS_SELECTED,LVIS_SELECTED);
	msgIP=fr_list.GetItemText(peer_row,0);
	temp_name=msgIP;
	selected_peer.SetWindowText(get_ip_of_friends(msgIP));
	Invalidate();
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void ABTUI::OnNMClickShare(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	peer_row=shared_list.GetSelectionMark();
	req_file_name=shared_list.GetItemText(peer_row,0);
	//AfxMessageBox(req_file_name);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void ABTUI::OnAgainAjanth()//when request file is clicked
{
	OnSend("REQFIL",get_ip_of_friends(shared_friend_name));
	client_file_name="C:\\ABT\\"+req_file_name;
}


void ABTUI::OnFriendSharefolder()//when share folder context menu is clicked
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
			if (f.Open("C:\\ABT\\shared.inf", CFile::modeWrite | CFile::modeCreate | CFile::shareDenyNone))
			{f.WriteString(fl);
			f.Close();}
			
		}
}


BOOL ABTUI::OnEraseBkgnd(CDC* pDC)
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


void ABTUI::OnBnClickedClose()//while app is closing check seed ratio
{
	ABTUI::list *temp4;
	if(ABTUI::head!=NULL)
	{
	temp4=ABTUI::head;
	gld=1;
	glu=1;
	while( temp4!=NULL )
	{
	gld=gld+temp4->down;
	glu=glu+temp4->up;
	temp4=temp4->next;
	}

	CStdioFile f;
	sdown.Format("%lu",gld);
	sup.Format("%lu",glu);
if (f.Open("C:\\ABT\\monitor.inf", CFile::modeReadWrite | CFile::shareDenyNone)){
	f.ReadString(r1);
	f.ReadString(r2);
	if(r1!=""&&r2!="")
	{
	d1=atoi(r1);
	u1=atoi(r2);
	}
	if(sdown!="")
	{
	d2=atoi(sdown);
	d1=d1+d2;}
	sdown.Format("%d",d1);
	u2=atoi(sup);
	u1=u1+u2;
	sup.Format("%d",u1);
	f.Close();
	}
if(f.Open("C:\\ABT\\monitor.inf", CFile::modeCreate |CFile::modeWrite | CFile::shareDenyNone))
{
	f.WriteString(sdown);
	f.WriteString("\n");
	f.WriteString(sup);
	f.Close();
}
	}
    OnSendme("CLOSEME","127.0.0.1");
	Sleep(1500);
	PostQuitMessage(0);
	//CDialogEx::OnOK();
}


void ABTUI::OnBnClickedResume()
{
	if(cur_click!=NULL)
	FTK_Downloader_Resume(cur_click);
}


void ABTUI::OnBnClickedPause()
{
	if(cur_click!=NULL)
	FTK_Downloader_Pause(cur_click);
	
}


void ABTUI::OnFriendViewactivity()
{
	CString ip;
	GetDlgItemText(IDC_IP,ip);
	OnSend("VIEW_ACTIVITY",ip);

}


HBRUSH ABTUI::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void ABTUI::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnCancel();
}


void ABTUI::OnBnClickedMin()
{
	// TODO: Add your control notification handler code here
	ShowWindow(SW_MINIMIZE);
}


void ABTUI::OnAgainClose()
{
	GetDlgItem(IDC_SHARE)->ShowWindow(SW_HIDE);
}


void ABTUI::OnMenuAddpeer()
{
	CADDPEER obj;
	CString valu;
	int nret=obj.DoModal();
	if(nret==IDOK)
	{FTK_Downloader_AddSource(cur_click, obj.m_szIPAddress, obj.m_nPort, NULL );
	valu=obj.m_szIPAddress;
	peer_listCtrl.InsertItem(0,valu);
	valu.Format("%lu" ,obj.m_nPort);
	peer_listCtrl.SetItemText(0,1,valu);
	}
}
