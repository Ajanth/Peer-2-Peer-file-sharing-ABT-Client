#pragma once
#include "afxcmn.h"
#include "ListCtrlEx.h"
#include "BTSButton.h"
#include "FTKernelAPI.h"
#include "afxwin.h"
#include "Profile.h"
// ABTUI dialog

class ABTUI : public CDialogEx
{
	DECLARE_DYNAMIC(ABTUI)

public:
	
	ABTUI(CWnd* pParent = NULL);   // standard constructor
	virtual ~ABTUI();
	void update_download_queue();
	void match_FTK(int row);
	static struct list
{
	int data;
	HDownloader cur_tor;
	HTorrentFile cur_fil;
	int id;
	ULONG down,up;
	list *next, *prev;
};
	HDownloader cur_click;
	
	static list *head;
	static int counter;
static int add_download_queue(HTorrentFile tfile, HDownloader hdown);
// Dialog Data
	enum { IDD = IDD_UI };

protected:
	QWORD m_qwTotalFileSize;
	DWORD m_dwPieceCount;
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	int allow_download();
	void OnSend(CString msg, CString ip);
	void OnSendChat(CString msg,CString _ip);
	void OnSendme(CString msg,CString _ip);
	void Display_download_result(CString msg);
	_tagMiniPeerData peer_list[50];
	int peer_ret;//par for gettin peer data
	int peer_max;//par for gettin peer data
	CBitmap Bacground;//for holding the background image
	BITMAP bm;//for holding the background image
	CSize bitmapSize;//for holding the background image
	ULONG gld,glu,ld,lu;//for monitoring download activities
	ULONG dd,upd;
	CString mytemp;
	CString dtemp;
	CMenu menu,menu1,menu2;
	CMenu *sub, *sub1,*sub2;
	CString myname;
	CString client_file_name;
	int mretVal;
	UINT j;
	CString peer_IP, peer_IP1, server_msg,send_IP,client_IP,msgIP,tempc;
	CString req_file_name,shared_friend_name,temp_name,shared_path;
	int peer_row;
	BOOL StartContext();
	static CListCtrlEx m_ctrlList;
	static int dcount;
	static int cur_sel;
	bool view_profile;
	bool view_activity;
	bool reqfile;
	CProfile prof;
	int prof_count;
	void update_context();
	CButton *btn;
	CBTSButton dload,bupload,bpause,bplay,bclose,bmin;
	struct {
	int perc, dspeed, upspeed;
	CString fname;
	}tor;
	CString iptoa(DWORD ip);
	afx_msg void OnBnClickedDownload();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl peer_listCtrl;
	CListCtrl tracker_listCtrl;
	afx_msg void OnBnClickedButton2();
	CString details_downloaded;
	CString details_uploaded;
	CStatic gb_details;
	afx_msg void OnNMRClickList4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnLvnItemchangedList4(NMHDR *pNMHDR, LRESULT *pResult);
	CStatic selected_peer;
	afx_msg void OnHdnItemclickList4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuViewprofile();
	afx_msg void OnViewprofile();
	afx_msg void OnUpdateMenuViewprofile(CCmdUI *pCmdUI);
public:
	void disp_msg(CString msg,CString name);
	void disp_filerequest(CString msg);
	CString friend_from_ip(CString ip);
	void activate_shared_list(CString list);
	bool view_shared;
	CImageList* m_ImageList;
	void add_friends(CString fr);
	int udp_command_processor(CString cmd, CString ip);
	int send_profile_data(CString _ip);
	void send_shared_filelist(CString _ip);
	void send_activity(CString _ip);
	void show_activity(CString act,CString ip);
    CListCtrl friends_list;
	CString fr_ip,final_ip;
	CStdioFile tmp;
	CString fr_final;
	CString file_path;
	CFont Font1;
	DWORD stu;
	CSocket echoServer;
	CString sdown,sup;
CString r1,r2;
int d1,u1,d2,u2;
	bool thread_flag;
	char *pch;
	char* context;
	CString get_ip_of_friends(CString fname);
	static CString fr_end;
	CListCtrl fri_ajanth;
	afx_msg void OnBnClickedOk();
	afx_msg void OnFriendSendmessage();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnNMClickFrnd(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl fr_list;
	CString chat_history;
	CListCtrl chat_msg;
	CListCtrl shared_list;
	afx_msg void OnNMClickShare(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAgainAjanth();
	CListBox list_req;
	afx_msg void OnFriendSharefolder();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedResume();
	afx_msg void OnBnClickedPause();
	afx_msg void OnFriendViewactivity();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedMin();
	afx_msg void OnAgainClose();
	afx_msg void OnMenuAddpeer();
};
UINT thread(LPVOID);
UINT ReceiveData(LPVOID pParam);
UINT File_server(LPVOID pParam);
UINT File_client(LPVOID pParam);