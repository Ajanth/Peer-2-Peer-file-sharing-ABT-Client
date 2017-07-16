
// ABTServerDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxdb.h"
#include "odbcinst.h"

// CABTServerDlg dialog
class CABTServerDlg : public CDialogEx
{
// Construction
public:
	CDatabase database;//for database connectivity
	CString SqlString;
	CString sCatID, sCategory;
	CString sDriver;
	CString sDsn;
	CString sFile;
	CString status;
	int register_user(CString info,CString ip);//end of database conectvty
    int check_username(CString name);
	int authenticate(CString info);
	int check_upload(CString info);
	int add_user(CString name,CString pwd,CString email,CString interests,CString loc,CString state);
	int check_file(CString aname,CString fname,CString ext,CString type,CString size,CString crtime);
	CABTServerDlg(CWnd* pParent = NULL);	// standard constructor
	void set_file(CString fn);
	CString fnm;
	char *pch;
	char* context;
	int ch_count;
	bool checkname,registering,login,checking;

	bool ch_fname,ch_aname,ch_size;
// Dialog Data
	enum { IDD = IDD_ABTSERVER_DIALOG };
	CString client_IP;
	int prof_count;
	CString f_name,f_size,f_type,f_year,f_ext,f_cat,f_ret;
	int analyse_filedesc(CString param);
	int check_filecheck(CString desc);
	void reply(CString msg,CString _ip);
	int check_init;
	void send_back(CString msg);
	void send_login(CString msg);
	CBitmap Bacground;//for holding the background image
	BITMAP bm;//for holding the background image
	CSize bitmapSize;//for holding the background image
	CString s_IP;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
UINT ReceiveData(LPVOID);