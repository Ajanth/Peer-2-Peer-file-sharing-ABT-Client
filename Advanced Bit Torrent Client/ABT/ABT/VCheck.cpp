// VCheck.cpp : implementation file
//

#include "stdafx.h"
#include "ABT.h"
#include "VCheck.h"
#include "afxdialogex.h"
#include "DlgMakeTorrent.h"
#include "DlgMTProgress.h"
// CVCheck dialog

IMPLEMENT_DYNAMIC(CVCheck, CDialogEx)

CVCheck::CVCheck(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVCheck::IDD, pParent)
{

}

CVCheck::~CVCheck()
{
}

void CVCheck::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVCheck, CDialogEx)
	ON_BN_CLICKED(IDOK, &CVCheck::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CVCheck::OnBnClickedCancel)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CVCheck::OnInitDialog() 
{
	Bacground.LoadBitmap(IDB_BITMAP16);
	Bacground.GetBitmap(&bm);
	bitmapSize = CSize(bm.bmWidth, bm.bmHeight);
	Invalidate(1);

CDialogEx::OnInitDialog();
path=CDlgMakeTorrent::vpath;
path.Replace("\\","\\\\");
Font1.CreateFont(14,                           // Height
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
GetDlgItem(IDC_RESULT)->SetFont(&Font1);
cmd="avgscanx.exe /SCAN=\""+path+"\"";
SetDlgItemText(IDC_RESULT,ExecCmd(cmd));
return true;
}
CString CVCheck::ExecCmd( LPCSTR pCmdArg)
{
   // Handle Inheritance - to pipe child's stdout via pipes to parent, handles must be inherited.
   //   SECURITY_ATTRIBUTES.bInheritHandle must be TRUE
   //   CreateProcess parameter bInheritHandles must be TRUE;
   //   STARTUPINFO.dwFlags must have STARTF_USESTDHANDLES set.
 
   CString strResult; // Contains result of cmdArg.
 
   HANDLE hChildStdoutRd; // Read-side, used in calls to ReadFile() to get child's stdout output.
   HANDLE hChildStdoutWr; // Write-side, given to child process using si struct.
 
   BOOL fSuccess;
 
   // Create security attributes to create pipe.
   SECURITY_ATTRIBUTES saAttr = {sizeof(SECURITY_ATTRIBUTES)} ;
   saAttr.bInheritHandle       = TRUE; // Set the bInheritHandle flag so pipe handles are inherited by child process. Required.
   saAttr.lpSecurityDescriptor = NULL;
 
   // Create a pipe to get results from child's stdout.
   // I'll create only 1 because I don't need to pipe to the child's stdin.
   if ( !CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0) )
   {
      return strResult;
   }
 
   STARTUPINFO si = { sizeof(STARTUPINFO) }; // specifies startup parameters for child process.
 
   si.dwFlags     = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES; // STARTF_USESTDHANDLES is Required.
   si.hStdOutput  = hChildStdoutWr; // Requires STARTF_USESTDHANDLES in dwFlags.
   si.hStdError   = hChildStdoutWr; // Requires STARTF_USESTDHANDLES in dwFlags.
   // si.hStdInput remains null.
   si.wShowWindow = SW_HIDE; // Prevents cmd window from flashing. Requires STARTF_USESHOWWINDOW in dwFlags.
 
   PROCESS_INFORMATION pi  = { 0 };
 
   // Create the child process.
   fSuccess = CreateProcess(
      NULL,
      (LPSTR)pCmdArg,     // command line
      NULL,               // process security attributes
      NULL,               // primary thread security attributes
      TRUE,               // TRUE=handles are inherited. Required.
      CREATE_NEW_CONSOLE, // creation flags
      NULL,               // use parent's environment
      NULL,               // use parent's current directory
      &si,                // __in, STARTUPINFO pointer
      &pi);               // __out, receives PROCESS_INFORMATION
 
   if (! fSuccess)
   {
      return strResult;
   }
   WaitForSingleObject( pi.hProcess, INFINITE);
   
   //TerminateProcess( pi.hProcess, 0 ); // Kill process if it is still running. Tested using cmd "ping blah -n 99"
 
   // Close the write end of the pipe before reading from the read end of the pipe.
   if (!CloseHandle(hChildStdoutWr))
   {
      return strResult;
   }
 
   // Read output from the child process.
   for (;;)
   {
      DWORD dwRead;
      CHAR chBuf[4096];
 
      // Read from pipe that is the standard output for child process.
      bool done = !ReadFile( hChildStdoutRd, chBuf, 4096, &dwRead, NULL) || dwRead == 0;
	  
      if( done )
      {
         break;
      }
 
      // Append result to string.
      strResult += CString( chBuf, dwRead) ;
   }
 
   // Close process and thread handles.
   CloseHandle( hChildStdoutRd );
 
   // CreateProcess docs specify that these must be closed. 
   CloseHandle( pi.hProcess );
   CloseHandle( pi.hThread );
 
   return strResult;
}
// CVCheck message handlers


void CVCheck::OnBnClickedOk()
{
	
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CVCheck::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


HBRUSH CVCheck::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


BOOL CVCheck::OnEraseBkgnd(CDC* pDC)
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
