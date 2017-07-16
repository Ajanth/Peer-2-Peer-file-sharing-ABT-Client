// ADDPEER.cpp : implementation file
//

#include "stdafx.h"
#include "ABT.h"
#include "ADDPEER.h"
#include "afxdialogex.h"


// CADDPEER dialog

IMPLEMENT_DYNAMIC(CADDPEER, CDialogEx)

CADDPEER::CADDPEER(CWnd* pParent /*=NULL*/)
	: CDialogEx(CADDPEER::IDD, pParent)
{

}

CADDPEER::~CADDPEER()
{
}

void CADDPEER::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CADDPEER, CDialogEx)
	ON_BN_CLICKED(IDOK4, &CADDPEER::OnBnClickedOk4)
	ON_BN_CLICKED(IDCANCEL3, &CADDPEER::OnBnClickedCancel3)
END_MESSAGE_MAP()


// CADDPEER message handlers


void CADDPEER::OnBnClickedOk4()
{
	DWORD dwIP = 0L;
    ( ( CIPAddressCtrl * ) GetDlgItem( 1001 ) ) ->GetAddress( dwIP );

    struct in_addr addr;
    addr.S_un.S_addr = ntohl( dwIP );

    if ( dwIP != 0L )
    {
        strcpy_s( m_szIPAddress, inet_ntoa( ( struct in_addr ) addr ) );
    }

	CString port;
	GetDlgItemText(IDC_EDIT1,port);
	m_nPort=_ttoi(port);
    UpdateData( TRUE );	
	Sleep(2000);
	CDialogEx::OnOK();
}


void CADDPEER::OnBnClickedCancel3()
{
	CDialogEx::OnCancel();
}
