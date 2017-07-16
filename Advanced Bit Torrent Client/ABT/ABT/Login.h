#pragma once


// CLogin dialog

class CLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CLogin)

public:
	CLogin(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLogin();
	void send_server(CString msg);
// Dialog Data
	enum { IDD = IDD_LOGIN };
	bool logf,logs;
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CBitmap Bacground;//for holding the background image
	BITMAP bm;//for holding the background image
	CSize bitmapSize;//for holding the background image
	DWORD dw;
	bool t_flag;
	void send_me(CString h);
	afx_msg void OnBnClickedCancel();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
UINT Receive_login(LPVOID pParam);