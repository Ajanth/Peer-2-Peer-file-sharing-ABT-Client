#pragma once
#include "afxwin.h"


// CDlgMakeTorrent dialog

class CDlgMakeTorrent : public CDialog
{
	DECLARE_DYNAMIC(CDlgMakeTorrent)

public:
	CDlgMakeTorrent(CWnd* pParent = NULL);   // standard constructor
	static CString fname,fsize,fversion,fyear,ftype,fextension;
	virtual ~CDlgMakeTorrent();
	bool vcheck;
	CBitmap Bacground;//for holding the background image
	BITMAP bm;//for holding the background image
	CSize bitmapSize;//for holding the background image
	static CString vpath;
	static CString x,y,z;
	bool isver;
	static CString cinfo;
	ULONGLONG day,month,week,hour,min;
	void show_one();
	void show_two();
	bool showing;
// Dialog Data
	enum { IDD = IDC_MAKE_TORRENT };
	// Dialog Data
	//{{AFX_DATA(CDlgMakeTorrent)
	CComboBox	m_wndCodePage;
	int		m_nFileOrDir;
	CString	m_strMainTracker;
	CString	m_strOtherTracker;
	CString	m_strNotes;
	CString	m_strTorrentName;
	BOOL	m_bEncrypt;
	CString	m_strPassword;
	CString	m_strSrcPathFile;
	int		m_nPieceSize;
	//}}AFX_DATA

	static CString m_strMainAnnounce;
	static CString m_strOtherAnnounces;
	static CString m_strComments;
	static CString m_strDestTorrentName;
	static CString m_strEncryptPwd;
	static CString m_strSourceFilePath;
	static int m_nSelPieceSize;
	static BOOL m_bIsFile;
    static UINT m_nCodePage;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnSelFileOrDir();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSelTorrentFileName();
	afx_msg void OnStartMakeTorrent();
	DECLARE_MESSAGE_MAP()
	protected:

    static BOOL CALLBACK EnumCodePageProc( LPTSTR lpCodePageString );
    int SelectCodePage( CComboBox &wndComboBox, UINT nCodePage );

protected:

    static CStringArray m_arryCodePages;
public:
	afx_msg void OnCbnSelchange1018();
	afx_msg void OnBnClicked1005();// browse source files
	afx_msg void OnBnClicked1050();// browse dest
	afx_msg void OnBnClicked1001();//start
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChange1015();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBack();
	CComboBox CMB_type;
	CComboBox CMB_genre;
	CComboBox CMB_psize;
	CString Cname;
	afx_msg void OnCbnSelchangeType();
	CString T_year;
	afx_msg void OnStnClickedT5();
	CString version;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
