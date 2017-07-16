// ProxyDummy.cpp : implementation file
//

#include "stdafx.h"
#include "ABT.h"
#include "ProxyDummy.h"
#include "afxdialogex.h"
#include "FTKernelAPI.h"


// ProxyDummy dialog

IMPLEMENT_DYNAMIC(ProxyDummy, CDialogEx)

ProxyDummy::ProxyDummy(CWnd* pParent /*=NULL*/)
	: CDialogEx(ProxyDummy::IDD, pParent)
{
	   m_nProxyType = 0;
    m_bAuthRequired = FALSE;
    m_strProxyServer = _T( "" );
    m_nProxyPort = 0;
    m_bUseOnlyHTTP = FALSE;
    m_strUserName = _T( "" );
    m_strPassword = _T( "" );
	
}
BOOL ProxyDummy::OnInitDialog() 
{
	CDialog::OnInitDialog();
	( ( CComboBox * ) GetDlgItem(IDC_TYPE_COMBO) )->ResetContent();

    TCHAR *szItems[] = {
                           _T( "No Proxy" ),
                           _T( "Socks4" ),
                           _T( "Socks4a" ),
                           _T( "Socks5" ),
                           _T( "HTTP1.1" )
                       };
	 for ( register int i = 0; i < 5; i++ )
    {
        ( ( CComboBox * )GetDlgItem(IDC_TYPE_COMBO) )->AddString( szItems[ i ] );
    }

    ( ( CComboBox * ) GetDlgItem(IDC_TYPE_COMBO) )->SetCurSel( m_nProxyType );	
	return TRUE;
}
ProxyDummy::~ProxyDummy()
{
}

void ProxyDummy::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex( pDX,IDC_TYPE_COMBO, m_nProxyType );
    DDX_Check( pDX,IDC_AUTHENTI_CHECK, m_bAuthRequired);
    DDX_Text( pDX,IDC_SERVER_EDIT, m_strProxyServer);
    DDX_Text( pDX, IDC_PORT_EDIT, m_nProxyPort );
    DDX_Check( pDX, IDC_CON_CHECK, m_bUseOnlyHTTP);
    DDX_Text( pDX, IDC_USER_EDIT, m_strUserName );
    DDX_Text( pDX, IDC_PASSWORD_EDIT, m_strPassword );
}


BEGIN_MESSAGE_MAP(ProxyDummy, CDialogEx)
	ON_BN_CLICKED(IDOK6, &ProxyDummy::OnBnClickedOk6)
END_MESSAGE_MAP()

void ProxyDummy::OnProxySettings() 
{
	
	
}
// ProxyDummy message handlers


void ProxyDummy::OnBnClickedOk6()
{
	CDialog::OnOK();
}
