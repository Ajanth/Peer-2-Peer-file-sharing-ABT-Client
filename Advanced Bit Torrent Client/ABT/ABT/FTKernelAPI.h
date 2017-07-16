 //////////////////////////////////////////////////////////////////////////
// FTKernelAPI.h : header file
//
// English notes:
//
// Written by Hwycheng Leo (FlashBT@Hotmail.com) Copyright 2005. China.
// The official homepage: http://www.hwysoft.com
//
// FTKernelAPI is a kernel API based on Bittorrent protocol.  
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found and I will fix them into this library. 
// You can use this library in your no-commercial program. If you want to
// use it in your commercial program, you can pay for a little money.
// Please visit http://www.hwysoft.com for the detailed information.
//
// ***********************************************************************
// chinese notes:
//
// ��Ȩ���� 2005 Hwycheng Leo ��������Ȩ��. �й�. 
// �ٷ���վ: http://www.hwysoft.com/chs/
//
// FTKernelAPI �ǻ���BitTorrentЭ����ں˿�.
//
// ��������Լ��ķ���ҵ, ��ӯ���Գ���������ʹ����. �Һܸ����ܹ��յ���������
// ����/����, �ҽ����Ժ�İ汾�кܿ�Ľ���ʵ��.
// �����������ҵ�Եĳ�����ʹ�ñ��ں˿�, �����֧��һ���ķ��û�ȡ�����ں˵�
// ��ҵʹ��Ȩ. ��ϸ��Ϣ������ҵ���վ: http://www.hwysoft.com/chs/
//////////////////////////////////////////////////////////////////////////

#ifndef __FTKERNELAPI_H__
#define __FTKERNELAPI_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifdef FTKERNELAPI_EXPORTS
#define FTKERNELAPI_API __declspec(dllexport)
#else
#define FTKERNELAPI_API __declspec(dllimport)
#endif

#ifndef BTAPI
#define BTAPI _cdecl
#endif

//����64λ�޷��ų�����
//Define 64bit long integer type
typedef unsigned __int64 QWORD;

//������������ľ��
//Define downloader handle
typedef LONG *HDownloader;

//�����ȡTorrent�ļ��ľ��
//Define Torrent file handle
typedef LONG *HTorrentFile;

//��������Torrent�ļ��ľ��
//Define Making Torrent file handle
typedef LONG *HMakeTorrent;

//length of client identifier
//�û���ʶ�ĳ���
#define IDSIZE 20

//BitTorrent �ͻ��˰汾��Ϣ����
//bitorrent client version information
#define AGENTSIZE 64

//���ɵ�SHA��ɢ�еĳ���
//Length of SHA1
#define SHA1_DIGEST_LENGTH 20 

//���������
//Proxytypes
#define PROXYTYPE_NOPROXY 0
#define PROXYTYPE_SOCKS4 1
#define PROXYTYPE_SOCKS4A 2
#define PROXYTYPE_SOCKS5 3
#define PROXYTYPE_HTTP11 4

////////////////////////
//����״̬
//Download status

#define DLSTATE_NONE				0
#define DLSTATE_ALLOCING			1
#define DLSTATE_CHECKING			2
#define DLSTATE_DOWNLOAD			3
#define DLSTATE_PAUSING				4
#define DLSTATE_CHECKEND			5
#define DLSTATE_FETALERR			6
#define DLSTATE_TERMINATE			7

////////////////////////
//��������ļ���
//Downloader priority

#define DLPRIORITY_LOW				0
#define DLPRIORITY_MID				1
#define DLPRIORITY_HIG				2

////////////////////////
//�����ļ��ļ���
//File priority

#define DLFILE_LOW					0
#define DLFILE_MID					1
#define DLFILE_HIG					2

//����Piece��״̬
//Piece status
#define PIECESTATE_INCOMPLETE	0
#define PIECESTATE_DOWNLOADING	1
#define PIECESTATE_COMPLETE		2

struct _tagPeerData
{
    DWORD	m_dwIP;
    USHORT	m_nPort;
    BYTE	m_pPeerID[ IDSIZE ];

    //-------------------------------------

    BYTE m_bIsLocal;
    TCHAR m_szVersion[ AGENTSIZE ];
    QWORD m_qwPeerHaveSize;
    float m_fDownRate;
    float m_fUpRate;
    QWORD m_qwDownloaded;
    QWORD m_qwUploaded;
    float m_fTimeElapsed;
    float m_fPeerDownRate;
    char m_szExtInfo[ 256 ];
};

struct _tagMiniPeerData
{
    DWORD	m_dwIP;
    USHORT	m_nPort;
};

struct _tagAnnounceData
{
    TCHAR m_szURL[ 512 ];
    BYTE m_bWorking;
    BYTE m_bHeader;
    UINT m_nCount;
    UINT m_nFailed;
    float m_fTimeLeft;
    float m_fLastTime;
    TCHAR m_szExtInfo[ 512 ];
};

struct _tagFileInfo
{
    TCHAR m_szFileName[ 512 ];
    QWORD m_qwFileSize;
};

struct _tagAnnounce
{
	//Announce�ĵ�ַ�б�
	//Announce URL
    TCHAR m_szAnnounce[ 512 ];

	//Announce��״̬��Ϣ
	//Announce status
    TCHAR m_szStatus[ 512 ];

	//�ϴ����ӵ�ʱ��
	//Last time
    double m_fLastTime;

	//�ܹ�ִ�еĴ���
	//Total count
    UINT m_nTotalCount;

	//ִ��ʧ�ܵĴ���
	//Failure count
    UINT m_nFailCount;

	//(HTTP)����ʱ�ĳ�ʱʱ��
	//Timeout of http connection
    UINT m_nTimeOut;				
};

extern "C"
{
	//////////////////////////////////////////////////////////////////////////
	//Context interface

	//������ʼ��
    //Initialize context
	//@lpszIniFile: kernel configuration file - INI file
    FTKERNELAPI_API BOOL BTAPI FTK_Context_Init( const char *lpszIniFile = NULL );

	//�ͷ���������
    //Release context
    FTKERNELAPI_API void BTAPI FTK_Context_Release();

	//�����������¼���������
    //Context event driver
    FTKERNELAPI_API BOOL BTAPI FTK_Context_Run();

    //ǿ���˳�����, ֹͣ����
    //��ֹͣʱҪ�˳��������е��߳����Կ���Ҫ��΢�ȴ�
    //Stop context event driver
    FTKERNELAPI_API void BTAPI FTK_Context_Stop();

    //�ж��Ƿ��Ѿ���ʼ��
	//Is context initialized?
    FTKERNELAPI_API BOOL BTAPI FTK_Context_IsInit();

	//�Ƿ�����ɹ��˶˿�
    //Is context running?
    FTKERNELAPI_API BOOL BTAPI FTK_Context_IsRunning();

	//�ж��Ƿ��Ѿ�����
	//Return context thread ID
	FTKERNELAPI_API DWORD BTAPI FTK_Context_GetThreadID();

	//��ȡ�̵߳�ID
    //Return listening port
    FTKERNELAPI_API USHORT BTAPI FTK_Context_GetListenPort();

	//����ָ���˿ڰ󶨲���
    //Return binding ip address
    FTKERNELAPI_API const char *BTAPI FTK_Context_GetBindIP();

	//////////////////////////////////////////////////////////////////////////
	//Downloader interface

	//Create a new HDownloader handle
	FTKERNELAPI_API HDownloader BTAPI FTK_Downloader_Open();

	//Destroy a HDownloader hanle
	FTKERNELAPI_API void BTAPI FTK_Downloader_Close( HDownloader hDownloader );

	//��ʼ�����ض���
    //Initialize a HDownloader
	//@lpszTorrentFileName: a torrent file name
	//@pPreAllocFile: a array of preallocated files, 0->no, 1->yes
	//@nPreAllocFileLen: length of preallocated array
	//@bOnlyCheckFile: only verify data of files
	//@lpszDestFileName: the downloaded filename
	//@lpszConfig: kernel configuration file - INI file
	//@lpszLogFileName: output log filename
	//@lpszStatusFileName: downloading status filename
	//@lpszDesKey: DES-EDE2 password
	//@nCodePage: code page of the torrent file
    FTKERNELAPI_API BOOL BTAPI FTK_Downloader_Init( 
					 HDownloader hDownloader,
					 LPCTSTR lpszTorrentFileName,
                     BYTE *pPreAllocFile, int nPreAllocFileLen,
					 BOOL bOnlyCheckFile = FALSE,
                     LPCTSTR lpszDestFileName = NULL,
                     LPCTSTR lpszConfig = NULL,
                     LPCTSTR lpszLogFileName = NULL,
                     LPCTSTR lpszStatusFileName = NULL,
					 LPCTSTR lpszDesKey = NULL,
                     UINT nCodePage = CP_ACP );

	//�ͷ����ض���
    //Release downloader handle
    FTKERNELAPI_API BOOL BTAPI FTK_Downloader_Release( HDownloader hDownloader );

	//ִ�������߳�
    //Execute downloader
    FTKERNELAPI_API BOOL BTAPI FTK_Downloader_Execute( HDownloader hDownloader );

	//��ͣ����
    //Pause downloader
    FTKERNELAPI_API void BTAPI FTK_Downloader_Pause( HDownloader hDownloader );

	//�ָ�����
    //Resume downloader
    FTKERNELAPI_API void BTAPI FTK_Downloader_Resume( HDownloader hDownloader );

	//�Ƿ��Ѿ���ʼ��
    //Is downloader intialized?
    FTKERNELAPI_API BOOL BTAPI FTK_Downloader_IsInit( HDownloader hDownloader );

	//��ȡ��������Ŀǰ�Ľ׶�״̬
    //Return downloader state
    FTKERNELAPI_API UINT BTAPI FTK_Downloader_GetState( HDownloader hDownloader );

	//�Ƿ�ֻ�ǽ����ļ����
	//Only checking file, don't downloading...
	FTKERNELAPI_API BOOL BTAPI FTK_Downloader_IsOnlyCheckFile( HDownloader hDownloader );

    //�Ƿ��Ѿ����������(�ж�ѡ���ļ�����)
	//Is all selected files finished?
    FTKERNELAPI_API BOOL BTAPI FTK_Downloader_IsFinished( HDownloader hDownloader );

    //�Ƿ��Ѿ����������(ȫ������)
	//Is all files finished?
    FTKERNELAPI_API BOOL BTAPI FTK_Downloader_IsAllFinished( HDownloader hDownloader );

    //����ָ����Peer
	//Add external source peer
    FTKERNELAPI_API void BTAPI FTK_Downloader_AddSource( HDownloader hDownloader, const char *pchIP, unsigned short nPort, const BYTE *pPeerID = NULL );

    //�ⲿ��̬���Ӹ�����Announce
	//Add external announce
    FTKERNELAPI_API void BTAPI FTK_Downloader_AddExtAnnounce( HDownloader hDownloader, LPCTSTR lpszAnnounce );

    //�ֶ�����Announce���Ͻ�������
	//Reannounce manually
    FTKERNELAPI_API void BTAPI FTK_Downloader_ReannounceAll( HDownloader hDownloader );

    //���������ļ������ȼ���
    // -2:������, -1: ��, 0: һ��, 1:��
	//Set priority of file
	//-2: no download, -1: low, 0: middle, 1: high
    FTKERNELAPI_API void BTAPI FTK_Downloader_SetFilePriority( HDownloader hDownloader, int nIndex, char chPriority = 0, BOOL bDoPriority = TRUE );

    //ǿ���������½��ļ������ȼ���ֵ��Piece
	//Force to set priority of all piece immediately 
    FTKERNELAPI_API void BTAPI FTK_Downloader_ForceFilePriority( HDownloader hDownloader );

    //�������������ϴ����Ӹ���
	//Set the number of max uploading peer
    FTKERNELAPI_API void BTAPI FTK_Downloader_ChangeMaxUploads( HDownloader hDownloader, int nMaxUploads );

    //�ⲿ����, �ֶ�������������������(0.0��ʾû������)
	//Set fixed max download rate
    FTKERNELAPI_API void BTAPI FTK_Downloader_SetFixMaxDownloadRate( HDownloader hDownloader, double fMaxDownloadRate = 0.0 );

    //�ⲿ����, �ֶ����������ϴ�������(0.0��ʾû������)
	//Set fixed max upload rate
    FTKERNELAPI_API void BTAPI FTK_Downloader_SetFixMaxUploadRate( HDownloader hDownloader, double fMaxUploadRate = 0.0 );

	//��ȡ�̵߳�ID
	//Get thread ID
	FTKERNELAPI_API DWORD BTAPI FTK_Downloader_GetThreadID( HDownloader hDownloader );

	//////////////////////////////////////////////////////////////////////////
	//ȫ�ֲ�������
	//Global settings

    //��ĳ��Torrent�ļ���SHA1ֵ�������
	//Add torrent into SHA1 list
    FTKERNELAPI_API void BTAPI FTK_GlobalVar_AddTorrentSHA1( LPCTSTR lpszSHA1, HDownloader hDownloader );

    //��ĳ��Torrent�ļ���SHA1ֵ�Ӷ�����ɾ��
	//Remove torrent from SHA1 list
    FTKERNELAPI_API void BTAPI FTK_GlobalVar_RemoveTorrentSHA1( LPCTSTR lpszSHA1 );

	//////////////////////////////////////////////////////////////////////////
	//��������������б�
	//Proxy interface

	//���ý�������Trackerʱʹ�ô��������
	//Only tracker using proxy settings
	FTKERNELAPI_API void BTAPI FTK_Proxy_SetOnlyTracker( BOOL bOnlyTracker = FALSE );

	//��յ�ǰ�Ĵ�����Ϣ(������PROXYTYPE_NOPROXY)
	//Only PROXYTYPE_NOPROXY
	FTKERNELAPI_API void BTAPI FTK_Proxy_SetType( int nProxyType );
	
	//���ô���(������PROXYTYPE_NOPROXY)
	//May not be PROXYTYPE_NOPROXY
	FTKERNELAPI_API void BTAPI FTK_Proxy_SetProxy( int nProxyType, const char * pProxyHost, int ProxyPort );

	//����SOCKS5��HTTP1.1��Ҫ��֤��Ϣ�Ĵ���
	//Only SOCKS5 and HTTP1.1 proxies
	FTKERNELAPI_API void BTAPI FTK_Proxy_SetAuthProxy( int nProxyType, const char * pProxyHost, int ProxyPort, const char * pProxyUser, const char * pProxyPass );
	
	//��ȡ���������
	//Return proxy type
	FTKERNELAPI_API int BTAPI FTK_Proxy_GetProxyType();

	//////////////////////////////////////////////////////////////////////////
	//�����ļ������б�(������֧����)
	//Language interface(only for localization)

	//���������ļ�
    //Language filename
    FTKERNELAPI_API void BTAPI FTK_Language_SetFile( LPCTSTR lpszFileName );

	//////////////////////////////////////////////////////////////////////////
	//Downloader statistic information interface

    //��ȡ�ܹ��Ѿ�����(��֤�ɹ�)������
	//Get the size of having received data for all file
    FTKERNELAPI_API QWORD BTAPI FTK_Stat_GetTotalFileHaveSize( HDownloader hDownloader );

    //��ȡѡ���Ѿ�����(��֤�ɹ�)������
	//Get the size of having received data for a file
    FTKERNELAPI_API QWORD BTAPI FTK_Stat_GetSelectedFileHaveSize( HDownloader hDownloader );

    //��ȡ���һ�����ص��ֽ���
	//Get the size of lasted receiving data
    FTKERNELAPI_API DWORD BTAPI FTK_Stat_GetLastDataSize( HDownloader hDownloader );

    //��ȡ�ܹ��������ֽ���
	//Get the size of bad data
    FTKERNELAPI_API QWORD BTAPI FTK_Stat_GetBadDataSize( HDownloader hDownloader );

    //��ǰ�ܹ������ӵĸ���
	//Get the number of total peer
    FTKERNELAPI_API UINT BTAPI FTK_Stat_GetTotalPeerCount( HDownloader hDownloader );

    //��ǰ�������ӵĸ���
	//Get the number of active peer
    FTKERNELAPI_API UINT BTAPI FTK_Stat_GetActivePeerCount( HDownloader hDownloader );

    //��ǰ�ı������ӵĸ���
	//Get the number of passive peer
    FTKERNELAPI_API UINT BTAPI FTK_Stat_GetPassivePeerCount( HDownloader hDownloader );

    //��ǰ���������еĸ���
	//Get number of connecting peer
    FTKERNELAPI_API UINT BTAPI FTK_Stat_GetConnectingPeerCount( HDownloader hDownloader );

    //��ȡ�Ѿ����ӵ����ӵĸ���
	//Get number of seed
    FTKERNELAPI_API UINT BTAPI FTK_Stat_GetSeedCount( HDownloader hDownloader );

    //��ȡ�����ϴ������ӵĸ���
	//Get the number of uploading peer
    FTKERNELAPI_API UINT BTAPI FTK_Stat_GetUploadPeerCount( HDownloader hDownloader );

    //��ȡ�������ص����ӵĸ���
	//Get the number of downloading peer
    FTKERNELAPI_API UINT BTAPI FTK_Stat_GetDownloadPeerCount( HDownloader hDownloader );

    //��ȡ�ܹ����Ի�ȡ��Seed�ĸ���
	//Get the number of total available seed
    FTKERNELAPI_API UINT BTAPI FTK_Stat_GetTotalAvailSeedCount( HDownloader hDownloader );

    //��ȡ�ܹ����Ի�ȡ��Peer�ĸ���
	//Get the number of peer
    FTKERNELAPI_API UINT BTAPI FTK_Stat_GetTotalAvailPeerCount( HDownloader hDownloader );

    //�ɵ����ӵĸ���
	//Get old seed number
    FTKERNELAPI_API UINT BTAPI FTK_Stat_GetOldSeedPeerCount( HDownloader hDownloader );

    //���������Ѿ����ص����ݳ���
	//Get downloaded bytes
    FTKERNELAPI_API QWORD BTAPI FTK_Stat_GetDownloaded( HDownloader hDownloader );

    //���������Ѿ��ϴ������ݳ���
	//Get uploaded bytes
    FTKERNELAPI_API QWORD BTAPI FTK_Stat_GetUploaded( HDownloader hDownloader );

    //�����Ѿ�����ʱ��(����ʱ�䲻ͬ������ʱ��)
	//Get total running time
    FTKERNELAPI_API double BTAPI FTK_Stat_GetRunningTime( HDownloader hDownloader );

    //�������ص�ʱ��(���������, ������ʱ����ԶΪ0)
	//Get total downloaded time
    FTKERNELAPI_API double BTAPI FTK_Stat_GetDownloadTime( HDownloader hDownloader );

    //��������ʣ���ʱ��, ���������ʼ���
	//Get the left time to download
    FTKERNELAPI_API double BTAPI FTK_Stat_GetLeftTime( HDownloader hDownloader );

    //��ȡ��ǰ���ϴ���
	//Get upload rate
    FTKERNELAPI_API double BTAPI FTK_Stat_GetUploadRate( HDownloader hDownloader );

    //��ȡ��ǰ��������
	//Get downloading rate
    FTKERNELAPI_API double BTAPI FTK_Stat_GetDownloadRate( HDownloader hDownloader );

    //��ȡָ����Piece�����ؽ���
	//Get the progress of a piece
    FTKERNELAPI_API void BTAPI FTK_Stat_GetPieceState( HDownloader hDownloader, BYTE *pPiece, int nCount );

    //��ȡ����ļ��Ľ���
	//Get the progress of cheching piece
    FTKERNELAPI_API QWORD BTAPI FTK_Stat_GetCheckPieceSize( HDownloader hDownloader );

    //��ȡ����Piece�ĸ���
	//Get number of checked piece
    FTKERNELAPI_API DWORD BTAPI FTK_Stat_GetCheckPieceCount( HDownloader hDownloader );

    //��ȡ�Ѿ�����ռ�Ĵ�С
	//Get the size of allocated file
    FTKERNELAPI_API QWORD BTAPI FTK_Stat_GetAllocSpaceSize( HDownloader hDownloader );

    //��ȡ��ǰ�Ľ�����
	//Get healthy rate
    FTKERNELAPI_API double BTAPI FTK_Stat_GetHealthRate( HDownloader hDownloader );

    //��ȡ��ǰ����󻺴��С
	//Get the size of max cache
    FTKERNELAPI_API DWORD BTAPI FTK_Stat_GetMaxCacheSize( HDownloader hDownloader );

    //��ȡ�Ѿ�ʹ�õĻ����С
	//Get the size of used cache
    FTKERNELAPI_API DWORD BTAPI FTK_Stat_GetUseCacheSize( HDownloader hDownloader );

    //��ȡ����дӲ�̵��ܵĴ���
	//Get writed disk count
    FTKERNELAPI_API QWORD BTAPI FTK_Stat_GetWriteDiskCount( HDownloader hDownloader );

    //��ȡ��������дӲ�̵Ĵ���
	//Get really writed disk count
    FTKERNELAPI_API QWORD BTAPI FTK_Stat_GetRealWriteDiskCount( HDownloader hDownloader );

    //��ȡ���ζ�Ӳ�̵��ܵĴ���
	//Get readed disk count
    FTKERNELAPI_API QWORD BTAPI FTK_Stat_GetReadDiskCount( HDownloader hDownloader );

    //��ȡ����������Ӳ�̵Ĵ���
	//Get really readed disk count
    FTKERNELAPI_API QWORD BTAPI FTK_Stat_GetRealReadDiskCount( HDownloader hDownloader );

    //��ȡдӲ�̵�Ƶ��(��/��)
	//Get writing disk frenqence(number/second)
    FTKERNELAPI_API double BTAPI FTK_Stat_GetWriteDiskFrenq( HDownloader hDownloader );

    //��ȡ����дӲ�̵�Ƶ��(��/��)
	//Get really writing disk frenqence(number/second)
    FTKERNELAPI_API double BTAPI FTK_Stat_GetRealWriteDiskFrenq( HDownloader hDownloader );

    //��ȡ��Ӳ�̵�Ƶ��(��/��)
	//Get reading disk frenqence(number/second)
    FTKERNELAPI_API double BTAPI FTK_Stat_GetReadDiskFrenq( HDownloader hDownloader );

    //��ȡ������Ӳ�̵�Ƶ��(��/��)
	//Get really reading disk frenqence(number/second)
    FTKERNELAPI_API double BTAPI FTK_Stat_GetRealReadDiskFrenq( HDownloader hDownloader );

    //��ȡ��ǰ��־�ļ���·��+����
	//Get log file name
    FTKERNELAPI_API void BTAPI FTK_Stat_GetLogPathFileName( HDownloader hDownloader, char *lpszFileName );

    //��ȡָ�����ļ��Ľ���
	//Get the progress of a file
    FTKERNELAPI_API QWORD BTAPI FTK_Stat_GetFileHaveSize( HDownloader hDownloader, int nIndex );

    //��ȡ��ǰ�û�״̬��Ϣ���б�
	//Get the peers information
    FTKERNELAPI_API void BTAPI FTK_Stat_GetPeerData( HDownloader hDownloader, _tagPeerData *pData, int &nRetCount, int nMaxCount );

    //��ȡ��ǰ�û�״̬��Ϣ���б�(ֻ��ȡIP�Ͷ˿�)
	//Get the peers information(only IP and port)
    FTKERNELAPI_API void BTAPI FTK_Stat_GetMiniPeerData( HDownloader hDownloader, _tagMiniPeerData *pData, int &nRetCount, int nMaxCount );

    //��ȡ��ǰAnnounce״̬��Ϣ���б�
	//Get the status of announces
    FTKERNELAPI_API void BTAPI FTK_Stat_GetAnnounceData( HDownloader hDownloader, _tagAnnounceData *pData, int &nRetCount, int nMaxCount );

	//////////////////////////////////////////////////////////////////////////
	//��Torrent�ļ���ȡ��Ϣ�ĺ����б�
	//Torrent file interface

	//�ж��ǲ����Լ�����ļ��ܵ�Torrent�ļ�
	//Return boolean, tell us is a cipher torrent file
	FTKERNELAPI_API BOOL BTAPI FTK_Torrent_IsCipherTorrent( LPCTSTR lpszFileName );

    //ִ�д��ļ��ж�ȡ���ݲ���ʼ���������
	//Open a torrent file handle
    FTKERNELAPI_API HTorrentFile BTAPI FTK_Torrent_Open( LPCTSTR lpszFileName, UINT nCodePage = CP_ACP, LPCTSTR lpszDesKey = NULL );

    //�رյ�ǰ�򿪵�Torrent�ļ�
	//Close Torrent file handle
    FTKERNELAPI_API void BTAPI FTK_Torrent_Close( HTorrentFile hTorrentFile );

    //�ж���Ŀ¼�����ļ�
	//Return boolean, tell us is only a single file in torrent
    FTKERNELAPI_API BOOL BTAPI FTK_Torrent_IsFile( HTorrentFile hTorrentFile );

    //��ȡ Torrent ����
	//Get the name of torrent file
    FTKERNELAPI_API LPCTSTR BTAPI FTK_Torrent_GetTorrentName( HTorrentFile hTorrentFile );

    //��ȡtorrent�е��ļ��ĸ���(����һ��)
	//Get total number of files
    FTKERNELAPI_API int BTAPI FTK_Torrent_GetFilesCount( HTorrentFile hTorrentFile );
    
	//��ȡtorrent�е��ļ��б�(����һ��)
	//Get a pointer to file list
    FTKERNELAPI_API void BTAPI FTK_Torrent_GetFiles( HTorrentFile hTorrentFile, _tagFileInfo *pFileInfo );

    //��ȡInfo��Hashֵ, �ڲ�ʹ��
	//Get the binary info hash of torrent file
    FTKERNELAPI_API const BYTE * BTAPI FTK_Torrent_GetInfoHash( HTorrentFile hTorrentFile );

    //��ȡinfo��Hashֵ, ʮ������
	//Get the hex encoding info hash of torrent file
    FTKERNELAPI_API LPCTSTR BTAPI FTK_Torrent_GetHexInfoHash( HTorrentFile hTorrentFile );

    //��ȡ main annouce url
	//Get mainb announce url
    FTKERNELAPI_API LPCTSTR BTAPI FTK_Torrent_GetMainAnnounce( HTorrentFile hTorrentFile );

    //��ȡ Announce ����ĸ���
	//Get the number of all announce list
    FTKERNELAPI_API int BTAPI FTK_Torrent_GetAnnouncesCount( HTorrentFile hTorrentFile );

    //��ȡָ���� Announce ���������Announce�ĸ���
	//Get the size of a announce list
    FTKERNELAPI_API int BTAPI FTK_Torrent_GetAnnounceSize( HTorrentFile hTorrentFile, int nGroupIndex );

    //��ȡָ���� Announce ������ָ��λ�õ�Announce
	//Get a annoucne of a announce list
    FTKERNELAPI_API LPCTSTR BTAPI FTK_Torrent_GetAnnouncesVal( HTorrentFile hTorrentFile, int nGroupIndex, int nIndex );

    //��ȡ ע��
	//Get the comments
    FTKERNELAPI_API LPCTSTR BTAPI FTK_Torrent_GetComments( HTorrentFile hTorrentFile );

    //��ȡ Torrent ����ʱ��
	//Get the creation time of torrent file
    FTKERNELAPI_API time_t BTAPI FTK_Torrent_GetCreateTime( HTorrentFile hTorrentFile );

    //��ȡpiece��Ĵ�С
	//Get the size of piece
    FTKERNELAPI_API DWORD BTAPI FTK_Torrent_GetPieceSize( HTorrentFile hTorrentFile );

    //��ȡ�ļ��Ĵ�С
	//Get the total file size to transfer
    FTKERNELAPI_API QWORD BTAPI FTK_Torrent_GetFileSize( HTorrentFile hTorrentFile );

    //Piece��ĸ���
	//Get the total number of piece
    FTKERNELAPI_API DWORD BTAPI FTK_Torrent_GetPieceCount( HTorrentFile hTorrentFile );

    //��ȡPiece Hash ���б�
	//Get a pointer to piece hash list
    FTKERNELAPI_API const BYTE *BTAPI FTK_Torrent_GetPieceHashList( HTorrentFile hTorrentFile );

	//////////////////////////////////////////////////////////////////////////
	//����Torrent�ļ��ĺ����б�
	//Create torrent file interface

	//��һ������torrent�ļ��ľ��
	//Open a making torrent handle
	FTKERNELAPI_API HMakeTorrent BTAPI FTK_MT_Open();

	//�رմ򿪵�����Torrent�ļ��ľ��
	//Close a making torrent handle
	FTKERNELAPI_API void BTAPI FTK_MT_Close( HMakeTorrent hMakeTorrent );

    //ִ�����ɶ���Ĳ���
	//Start to making torrent file
    FTKERNELAPI_API BOOL BTAPI FTK_MT_MakeTorrent( HMakeTorrent hMakeTorrent );

	//ֹͣTorrent�ļ�����
	//Stop making torrent file
	FTKERNELAPI_API void BTAPI FTK_MT_StopMake( HMakeTorrent hMakeTorrent );

    //��ȡ�����ļ��Ľ���
	//Get progress of making torrent file
    FTKERNELAPI_API double BTAPI FTK_MT_GetMTProgress( HMakeTorrent hMakeTorrent );

    //���ΪTorrent�ļ�
	//Save as a torrent file
    FTKERNELAPI_API BOOL BTAPI FTK_MT_SaveAsTorrentFile( HMakeTorrent hMakeTorrent, LPCTSTR lpszFileName, LPCTSTR lpszDesKey = NULL );

    //���� annouce url
	//Set a main announce url
    FTKERNELAPI_API void BTAPI FTK_MT_SetAnnounceUrl( HMakeTorrent hMakeTorrent, LPCTSTR lpszValue );

    //��Ӷ����Announce �б�
	//Add a announce list
    FTKERNELAPI_API void BTAPI FTK_MT_AddAnnounceList( HMakeTorrent hMakeTorrent, LPCTSTR Announces[], int nCount );

    //��ն����Announce �б�
	//Clear all announce list
    FTKERNELAPI_API void BTAPI FTK_MT_ClearAnnounceList( HMakeTorrent hMakeTorrent );

    //���� ע��
	//Set comments
    FTKERNELAPI_API void BTAPI FTK_MT_SetComments( HMakeTorrent hMakeTorrent, LPCTSTR lpszValue );

    //����piece��Ĵ�С
	//Set piece size to making torrent file
    FTKERNELAPI_API void BTAPI FTK_MT_SetPieceSize( HMakeTorrent hMakeTorrent, DWORD dwPieceSize );

    //����torrent�е��ļ�
	//Set the sourece file to making torrent file
    FTKERNELAPI_API void BTAPI FTK_MT_SetFile( HMakeTorrent hMakeTorrent, LPCTSTR lpszFileName );

    //����torrent�е��ļ�Ŀ¼(����ļ�)
	//Set the source path to making torrent file
    FTKERNELAPI_API void BTAPI FTK_MT_SetPath( HMakeTorrent hMakeTorrent, LPCTSTR lpszPathName );

    //����Torrent�ļ������Դ���(ֻ���ļ�����, ע�͵���Ч)
	//Set language code of torrent file
    FTKERNELAPI_API void BTAPI FTK_MT_SetLangCodePage( HMakeTorrent hMakeTorrent, UINT nCodePage = CP_ACP );

	//////////////////////////////////////////////////////////////////////////
}

#endif//__FTKERNELAPI_H__