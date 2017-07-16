#pragma once
#include "afxwin.h"


// CRegistration dialog

class CRegistration : public CDialogEx
{
	DECLARE_DYNAMIC(CRegistration)

public:
	CRegistration(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRegistration();
	CString usrname,email,pwd1,pwd2;
// Dialog Data
	enum { IDD = IDD_REGISTRATION };
	void init_shows();
	bool first,second;
	void show_form(int count);
	int user_name_check(CString uname);
	int final_check();
	int check_email(CString eid);
	int check_password(CString pwd1,CString pwd2);
	int isname_exists();
	void write_profile();
	void send_me(CString msg);
	void install_ABT();
	void send_server(CString Buffer);
	CString client_IP,server_msg, form_error, intr,cnt,ste,my_IP;
	bool exists,not_exists, name_ok,invalid_char,reg_fail,reg_pass;//init all these to false 
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBack();
	afx_msg void OnEnKillfocusUsrname();
	CBitmap Bacground;//for holding the background image
	BITMAP bm;//for holding the background image
	CSize bitmapSize;//for holding the background image
	CString uname;
	CString eid;
	CString pass1;
	CString pass2;
	CButton CB_movies;
	CButton CB_videos;
	CButton CB_games;
	CButton CB_others;
	CButton CB_music;
	CButton CB_softwares;
	CButton CB_agree;
	CString s_country;
	CString s_state;
	afx_msg void OnEnKillfocusPass2();
	afx_msg void OnEnKillfocusEmail();
	afx_msg void OnEnKillfocusPass1();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
UINT Receive_thr(LPVOID x);