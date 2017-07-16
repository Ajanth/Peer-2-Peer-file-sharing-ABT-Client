
// ABTDlg.h : header file
//

#pragma once

#include "FTKernelAPI.h"
// CABTDlg dialog
inline CString AfxFormatBytes( INT64 nBytes )
{
    CString strResult;
    if ( nBytes >= ( INT64 ) 0x10000000000L )
    {
        strResult.Format( _T( "%-.2f TB" ), nBytes / 1099511627776.0f );
    }
    else if ( nBytes >= 0x40000000 )
    {
        strResult.Format( _T( "%-.2f GB" ), nBytes / 1073741824.0f );
    }
    else if ( nBytes >= 0x100000 )
    {
        strResult.Format( _T( "%-.2f MB" ), nBytes / 1048576.0f );
    }
    else if ( nBytes >= 0x400 )
    {
        strResult.Format( _T( "%-.2f KB" ), nBytes / 1024.0f );
    }
    else
    {
        strResult.Format( _T( "%ld Byte" ), nBytes );
    }

    return strResult;
}

class CABTDlg : public CDialogEx
{
// Construction
public:
	CABTDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ABT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	afx_msg void OnSelchangeProxyType();
	afx_msg void OnClickAuthRequired();
	afx_msg void OnClickTestProxy();
	virtual BOOL OnInitDialog();
	HICON m_hIcon;
	
	BOOL StopContext();
	BOOL StartDownload();
	BOOL StopDownload();
	HDownloader m_hDownloader;
	CString m_strKeyValue;
	BOOL m_bInitContext;
	QWORD m_qwTotalFileSize;
	DWORD m_dwPieceCount;
	CString m_strDestPath;
	CString strPercent;
	
	afx_msg void OnDestroy();
	afx_msg void OnMakeTorrent();
	afx_msg void OnSelFiles();
	afx_msg void OnAddExtAnnounce();
	afx_msg void OnAddPeer();
	afx_msg void OnManualAnnounce();
	// Generated message map functions
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	 CString l_strFileName,m_strFolderPath,n_strFolderPath;
	 BOOL BrowseForFolder(HWND hwnd, LPCTSTR szCurrent);
	 int iRet;
	 LPTSTR szPath;
	 CString	m_strTorrentFile;
	 void OnProxySettings();
	CString act,tname,wstr,tmpstr;
	 BOOL retval; 
	 CString get_myname();
	 void update_activity(CString str);
	 afx_msg void OnBnClickedButton6();
	 afx_msg void OnBnClickedButton7();
	 afx_msg void OnBnClickedDestpath();
	 afx_msg void OnBnClickedStartdownload();
	 int local_dcount;
	 int prg_check;

	 afx_msg void OnBnClickedButton1();
	 afx_msg void OnBnClickedOk3();
	 afx_msg void OnBnClickedCancel2();
};
