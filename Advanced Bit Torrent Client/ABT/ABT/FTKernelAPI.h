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
// 版权所有 2005 Hwycheng Leo 保留所有权利. 中国. 
// 官方网站: http://www.hwysoft.com/chs/
//
// FTKernelAPI 是基于BitTorrent协议的内核库.
//
// 你可以在自己的非商业, 非盈利性程序中自由使用它. 我很高兴能够收到您反馈的
// 错误/建议, 我将在以后的版本中很快改进和实现.
// 如果你想在商业性的程序中使用本内核库, 你可以支付一定的费用获取到本内核的
// 商业使用权. 详细信息请访问我的网站: http://www.hwysoft.com/chs/
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

//定义64位无符号长整型
//Define 64bit long integer type
typedef unsigned __int64 QWORD;

//定义下载任务的句柄
//Define downloader handle
typedef LONG *HDownloader;

//定义读取Torrent文件的句柄
//Define Torrent file handle
typedef LONG *HTorrentFile;

//定义生成Torrent文件的句柄
//Define Making Torrent file handle
typedef LONG *HMakeTorrent;

//length of client identifier
//用户标识的长度
#define IDSIZE 20

//BitTorrent 客户端版本信息长度
//bitorrent client version information
#define AGENTSIZE 64

//生成的SHA的散列的长度
//Length of SHA1
#define SHA1_DIGEST_LENGTH 20 

//代理的类型
//Proxytypes
#define PROXYTYPE_NOPROXY 0
#define PROXYTYPE_SOCKS4 1
#define PROXYTYPE_SOCKS4A 2
#define PROXYTYPE_SOCKS5 3
#define PROXYTYPE_HTTP11 4

////////////////////////
//下载状态
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
//下载任务的级别
//Downloader priority

#define DLPRIORITY_LOW				0
#define DLPRIORITY_MID				1
#define DLPRIORITY_HIG				2

////////////////////////
//下载文件的级别
//File priority

#define DLFILE_LOW					0
#define DLFILE_MID					1
#define DLFILE_HIG					2

//定义Piece的状态
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
	//Announce的地址列表
	//Announce URL
    TCHAR m_szAnnounce[ 512 ];

	//Announce的状态信息
	//Announce status
    TCHAR m_szStatus[ 512 ];

	//上次连接的时间
	//Last time
    double m_fLastTime;

	//总共执行的次数
	//Total count
    UINT m_nTotalCount;

	//执行失败的次数
	//Failure count
    UINT m_nFailCount;

	//(HTTP)连接时的超时时间
	//Timeout of http connection
    UINT m_nTimeOut;				
};

extern "C"
{
	//////////////////////////////////////////////////////////////////////////
	//Context interface

	//环境初始化
    //Initialize context
	//@lpszIniFile: kernel configuration file - INI file
    FTKERNELAPI_API BOOL BTAPI FTK_Context_Init( const char *lpszIniFile = NULL );

	//释放整个环境
    //Release context
    FTKERNELAPI_API void BTAPI FTK_Context_Release();

	//整个环境的事件驱动引擎
    //Context event driver
    FTKERNELAPI_API BOOL BTAPI FTK_Context_Run();

    //强迫退出环境, 停止运行
    //在停止时要退出正在运行的线程所以可能要稍微等待
    //Stop context event driver
    FTKERNELAPI_API void BTAPI FTK_Context_Stop();

    //判断是否已经初始化
	//Is context initialized?
    FTKERNELAPI_API BOOL BTAPI FTK_Context_IsInit();

	//是否监听成功了端口
    //Is context running?
    FTKERNELAPI_API BOOL BTAPI FTK_Context_IsRunning();

	//判断是否已经运行
	//Return context thread ID
	FTKERNELAPI_API DWORD BTAPI FTK_Context_GetThreadID();

	//获取线程的ID
    //Return listening port
    FTKERNELAPI_API USHORT BTAPI FTK_Context_GetListenPort();

	//重新指定端口绑定操作
    //Return binding ip address
    FTKERNELAPI_API const char *BTAPI FTK_Context_GetBindIP();

	//////////////////////////////////////////////////////////////////////////
	//Downloader interface

	//Create a new HDownloader handle
	FTKERNELAPI_API HDownloader BTAPI FTK_Downloader_Open();

	//Destroy a HDownloader hanle
	FTKERNELAPI_API void BTAPI FTK_Downloader_Close( HDownloader hDownloader );

	//初始化下载对象
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

	//释放下载对象
    //Release downloader handle
    FTKERNELAPI_API BOOL BTAPI FTK_Downloader_Release( HDownloader hDownloader );

	//执行下载线程
    //Execute downloader
    FTKERNELAPI_API BOOL BTAPI FTK_Downloader_Execute( HDownloader hDownloader );

	//暂停下载
    //Pause downloader
    FTKERNELAPI_API void BTAPI FTK_Downloader_Pause( HDownloader hDownloader );

	//恢复下载
    //Resume downloader
    FTKERNELAPI_API void BTAPI FTK_Downloader_Resume( HDownloader hDownloader );

	//是否已经初始化
    //Is downloader intialized?
    FTKERNELAPI_API BOOL BTAPI FTK_Downloader_IsInit( HDownloader hDownloader );

	//获取下载任务目前的阶段状态
    //Return downloader state
    FTKERNELAPI_API UINT BTAPI FTK_Downloader_GetState( HDownloader hDownloader );

	//是否只是进行文件检查
	//Only checking file, don't downloading...
	FTKERNELAPI_API BOOL BTAPI FTK_Downloader_IsOnlyCheckFile( HDownloader hDownloader );

    //是否已经完成了下载(判断选择文件下载)
	//Is all selected files finished?
    FTKERNELAPI_API BOOL BTAPI FTK_Downloader_IsFinished( HDownloader hDownloader );

    //是否已经完成了下载(全部下载)
	//Is all files finished?
    FTKERNELAPI_API BOOL BTAPI FTK_Downloader_IsAllFinished( HDownloader hDownloader );

    //连接指定的Peer
	//Add external source peer
    FTKERNELAPI_API void BTAPI FTK_Downloader_AddSource( HDownloader hDownloader, const char *pchIP, unsigned short nPort, const BYTE *pPeerID = NULL );

    //外部动态增加辅助的Announce
	//Add external announce
    FTKERNELAPI_API void BTAPI FTK_Downloader_AddExtAnnounce( HDownloader hDownloader, LPCTSTR lpszAnnounce );

    //手动控制Announce马上进行连接
	//Reannounce manually
    FTKERNELAPI_API void BTAPI FTK_Downloader_ReannounceAll( HDownloader hDownloader );

    //设置下载文件的优先级别
    // -2:不下载, -1: 低, 0: 一般, 1:高
	//Set priority of file
	//-2: no download, -1: low, 0: middle, 1: high
    FTKERNELAPI_API void BTAPI FTK_Downloader_SetFilePriority( HDownloader hDownloader, int nIndex, char chPriority = 0, BOOL bDoPriority = TRUE );

    //强制立刻重新将文件的优先级赋值给Piece
	//Force to set priority of all piece immediately 
    FTKERNELAPI_API void BTAPI FTK_Downloader_ForceFilePriority( HDownloader hDownloader );

    //重新设置最大的上传连接个数
	//Set the number of max uploading peer
    FTKERNELAPI_API void BTAPI FTK_Downloader_ChangeMaxUploads( HDownloader hDownloader, int nMaxUploads );

    //外部调用, 手动设置最大的下载率限制(0.0表示没有限制)
	//Set fixed max download rate
    FTKERNELAPI_API void BTAPI FTK_Downloader_SetFixMaxDownloadRate( HDownloader hDownloader, double fMaxDownloadRate = 0.0 );

    //外部调用, 手动设置最大的上传率限制(0.0表示没有限制)
	//Set fixed max upload rate
    FTKERNELAPI_API void BTAPI FTK_Downloader_SetFixMaxUploadRate( HDownloader hDownloader, double fMaxUploadRate = 0.0 );

	//获取线程的ID
	//Get thread ID
	FTKERNELAPI_API DWORD BTAPI FTK_Downloader_GetThreadID( HDownloader hDownloader );

	//////////////////////////////////////////////////////////////////////////
	//全局参数设置
	//Global settings

    //将某个Torrent文件的SHA1值加入队列
	//Add torrent into SHA1 list
    FTKERNELAPI_API void BTAPI FTK_GlobalVar_AddTorrentSHA1( LPCTSTR lpszSHA1, HDownloader hDownloader );

    //将某个Torrent文件的SHA1值从队列中删除
	//Remove torrent from SHA1 list
    FTKERNELAPI_API void BTAPI FTK_GlobalVar_RemoveTorrentSHA1( LPCTSTR lpszSHA1 );

	//////////////////////////////////////////////////////////////////////////
	//代理服务器函数列表
	//Proxy interface

	//设置仅在连接Tracker时使用代理服务器
	//Only tracker using proxy settings
	FTKERNELAPI_API void BTAPI FTK_Proxy_SetOnlyTracker( BOOL bOnlyTracker = FALSE );

	//清空当前的代理信息(仅用于PROXYTYPE_NOPROXY)
	//Only PROXYTYPE_NOPROXY
	FTKERNELAPI_API void BTAPI FTK_Proxy_SetType( int nProxyType );
	
	//设置代理(不能是PROXYTYPE_NOPROXY)
	//May not be PROXYTYPE_NOPROXY
	FTKERNELAPI_API void BTAPI FTK_Proxy_SetProxy( int nProxyType, const char * pProxyHost, int ProxyPort );

	//设置SOCKS5和HTTP1.1需要认证信息的代理
	//Only SOCKS5 and HTTP1.1 proxies
	FTKERNELAPI_API void BTAPI FTK_Proxy_SetAuthProxy( int nProxyType, const char * pProxyHost, int ProxyPort, const char * pProxyUser, const char * pProxyPass );
	
	//获取代理的类型
	//Return proxy type
	FTKERNELAPI_API int BTAPI FTK_Proxy_GetProxyType();

	//////////////////////////////////////////////////////////////////////////
	//语言文件函数列表(多语言支持用)
	//Language interface(only for localization)

	//设置语言文件
    //Language filename
    FTKERNELAPI_API void BTAPI FTK_Language_SetFile( LPCTSTR lpszFileName );

	//////////////////////////////////////////////////////////////////////////
	//Downloader statistic information interface

    //获取总共已经下载(验证成功)的数据
	//Get the size of having received data for all file
    FTKERNELAPI_API QWORD BTAPI FTK_Stat_GetTotalFileHaveSize( HDownloader hDownloader );

    //获取选择已经下载(验证成功)的数据
	//Get the size of having received data for a file
    FTKERNELAPI_API QWORD BTAPI FTK_Stat_GetSelectedFileHaveSize( HDownloader hDownloader );

    //获取最近一次下载的字节数
	//Get the size of lasted receiving data
    FTKERNELAPI_API DWORD BTAPI FTK_Stat_GetLastDataSize( HDownloader hDownloader );

    //获取总共丢弃的字节数
	//Get the size of bad data
    FTKERNELAPI_API QWORD BTAPI FTK_Stat_GetBadDataSize( HDownloader hDownloader );

    //当前总共的连接的个数
	//Get the number of total peer
    FTKERNELAPI_API UINT BTAPI FTK_Stat_GetTotalPeerCount( HDownloader hDownloader );

    //当前主动连接的个数
	//Get the number of active peer
    FTKERNELAPI_API UINT BTAPI FTK_Stat_GetActivePeerCount( HDownloader hDownloader );

    //当前的被动连接的个数
	//Get the number of passive peer
    FTKERNELAPI_API UINT BTAPI FTK_Stat_GetPassivePeerCount( HDownloader hDownloader );

    //当前正在连接中的个数
	//Get number of connecting peer
    FTKERNELAPI_API UINT BTAPI FTK_Stat_GetConnectingPeerCount( HDownloader hDownloader );

    //获取已经连接的种子的个数
	//Get number of seed
    FTKERNELAPI_API UINT BTAPI FTK_Stat_GetSeedCount( HDownloader hDownloader );

    //获取正在上传的连接的个数
	//Get the number of uploading peer
    FTKERNELAPI_API UINT BTAPI FTK_Stat_GetUploadPeerCount( HDownloader hDownloader );

    //获取正在下载的连接的个数
	//Get the number of downloading peer
    FTKERNELAPI_API UINT BTAPI FTK_Stat_GetDownloadPeerCount( HDownloader hDownloader );

    //获取总共可以获取的Seed的个数
	//Get the number of total available seed
    FTKERNELAPI_API UINT BTAPI FTK_Stat_GetTotalAvailSeedCount( HDownloader hDownloader );

    //获取总共可以获取的Peer的个数
	//Get the number of peer
    FTKERNELAPI_API UINT BTAPI FTK_Stat_GetTotalAvailPeerCount( HDownloader hDownloader );

    //旧的种子的个数
	//Get old seed number
    FTKERNELAPI_API UINT BTAPI FTK_Stat_GetOldSeedPeerCount( HDownloader hDownloader );

    //本次运行已经下载的数据长度
	//Get downloaded bytes
    FTKERNELAPI_API QWORD BTAPI FTK_Stat_GetDownloaded( HDownloader hDownloader );

    //本次运行已经上传的数据长度
	//Get uploaded bytes
    FTKERNELAPI_API QWORD BTAPI FTK_Stat_GetUploaded( HDownloader hDownloader );

    //本次已经运行时间(运行时间不同与下载时间)
	//Get total running time
    FTKERNELAPI_API double BTAPI FTK_Stat_GetRunningTime( HDownloader hDownloader );

    //本次下载的时间(如果是做种, 则下载时间永远为0)
	//Get total downloaded time
    FTKERNELAPI_API double BTAPI FTK_Stat_GetDownloadTime( HDownloader hDownloader );

    //本次下载剩余的时间, 根据下载率计算
	//Get the left time to download
    FTKERNELAPI_API double BTAPI FTK_Stat_GetLeftTime( HDownloader hDownloader );

    //获取当前的上传率
	//Get upload rate
    FTKERNELAPI_API double BTAPI FTK_Stat_GetUploadRate( HDownloader hDownloader );

    //获取当前的下载率
	//Get downloading rate
    FTKERNELAPI_API double BTAPI FTK_Stat_GetDownloadRate( HDownloader hDownloader );

    //获取指定的Piece的下载进度
	//Get the progress of a piece
    FTKERNELAPI_API void BTAPI FTK_Stat_GetPieceState( HDownloader hDownloader, BYTE *pPiece, int nCount );

    //获取检查文件的进度
	//Get the progress of cheching piece
    FTKERNELAPI_API QWORD BTAPI FTK_Stat_GetCheckPieceSize( HDownloader hDownloader );

    //获取检查的Piece的个数
	//Get number of checked piece
    FTKERNELAPI_API DWORD BTAPI FTK_Stat_GetCheckPieceCount( HDownloader hDownloader );

    //获取已经分配空间的大小
	//Get the size of allocated file
    FTKERNELAPI_API QWORD BTAPI FTK_Stat_GetAllocSpaceSize( HDownloader hDownloader );

    //获取当前的健康率
	//Get healthy rate
    FTKERNELAPI_API double BTAPI FTK_Stat_GetHealthRate( HDownloader hDownloader );

    //获取当前的最大缓存大小
	//Get the size of max cache
    FTKERNELAPI_API DWORD BTAPI FTK_Stat_GetMaxCacheSize( HDownloader hDownloader );

    //获取已经使用的缓存大小
	//Get the size of used cache
    FTKERNELAPI_API DWORD BTAPI FTK_Stat_GetUseCacheSize( HDownloader hDownloader );

    //获取本次写硬盘的总的次数
	//Get writed disk count
    FTKERNELAPI_API QWORD BTAPI FTK_Stat_GetWriteDiskCount( HDownloader hDownloader );

    //获取本次真正写硬盘的次数
	//Get really writed disk count
    FTKERNELAPI_API QWORD BTAPI FTK_Stat_GetRealWriteDiskCount( HDownloader hDownloader );

    //获取本次读硬盘的总的次数
	//Get readed disk count
    FTKERNELAPI_API QWORD BTAPI FTK_Stat_GetReadDiskCount( HDownloader hDownloader );

    //获取本次真正读硬盘的次数
	//Get really readed disk count
    FTKERNELAPI_API QWORD BTAPI FTK_Stat_GetRealReadDiskCount( HDownloader hDownloader );

    //获取写硬盘的频率(次/秒)
	//Get writing disk frenqence(number/second)
    FTKERNELAPI_API double BTAPI FTK_Stat_GetWriteDiskFrenq( HDownloader hDownloader );

    //获取真正写硬盘的频率(次/秒)
	//Get really writing disk frenqence(number/second)
    FTKERNELAPI_API double BTAPI FTK_Stat_GetRealWriteDiskFrenq( HDownloader hDownloader );

    //获取读硬盘的频率(次/秒)
	//Get reading disk frenqence(number/second)
    FTKERNELAPI_API double BTAPI FTK_Stat_GetReadDiskFrenq( HDownloader hDownloader );

    //获取真正读硬盘的频率(次/秒)
	//Get really reading disk frenqence(number/second)
    FTKERNELAPI_API double BTAPI FTK_Stat_GetRealReadDiskFrenq( HDownloader hDownloader );

    //获取当前日志文件的路径+名称
	//Get log file name
    FTKERNELAPI_API void BTAPI FTK_Stat_GetLogPathFileName( HDownloader hDownloader, char *lpszFileName );

    //获取指定的文件的进度
	//Get the progress of a file
    FTKERNELAPI_API QWORD BTAPI FTK_Stat_GetFileHaveSize( HDownloader hDownloader, int nIndex );

    //获取当前用户状态信息的列表
	//Get the peers information
    FTKERNELAPI_API void BTAPI FTK_Stat_GetPeerData( HDownloader hDownloader, _tagPeerData *pData, int &nRetCount, int nMaxCount );

    //获取当前用户状态信息的列表(只获取IP和端口)
	//Get the peers information(only IP and port)
    FTKERNELAPI_API void BTAPI FTK_Stat_GetMiniPeerData( HDownloader hDownloader, _tagMiniPeerData *pData, int &nRetCount, int nMaxCount );

    //获取当前Announce状态信息的列表
	//Get the status of announces
    FTKERNELAPI_API void BTAPI FTK_Stat_GetAnnounceData( HDownloader hDownloader, _tagAnnounceData *pData, int &nRetCount, int nMaxCount );

	//////////////////////////////////////////////////////////////////////////
	//打开Torrent文件获取信息的函数列表
	//Torrent file interface

	//判断是不是自己定义的加密的Torrent文件
	//Return boolean, tell us is a cipher torrent file
	FTKERNELAPI_API BOOL BTAPI FTK_Torrent_IsCipherTorrent( LPCTSTR lpszFileName );

    //执行从文件中读取数据并初始化对象操作
	//Open a torrent file handle
    FTKERNELAPI_API HTorrentFile BTAPI FTK_Torrent_Open( LPCTSTR lpszFileName, UINT nCodePage = CP_ACP, LPCTSTR lpszDesKey = NULL );

    //关闭当前打开的Torrent文件
	//Close Torrent file handle
    FTKERNELAPI_API void BTAPI FTK_Torrent_Close( HTorrentFile hTorrentFile );

    //判断是目录还是文件
	//Return boolean, tell us is only a single file in torrent
    FTKERNELAPI_API BOOL BTAPI FTK_Torrent_IsFile( HTorrentFile hTorrentFile );

    //获取 Torrent 名称
	//Get the name of torrent file
    FTKERNELAPI_API LPCTSTR BTAPI FTK_Torrent_GetTorrentName( HTorrentFile hTorrentFile );

    //获取torrent中的文件的个数(至少一个)
	//Get total number of files
    FTKERNELAPI_API int BTAPI FTK_Torrent_GetFilesCount( HTorrentFile hTorrentFile );
    
	//获取torrent中的文件列表(至少一个)
	//Get a pointer to file list
    FTKERNELAPI_API void BTAPI FTK_Torrent_GetFiles( HTorrentFile hTorrentFile, _tagFileInfo *pFileInfo );

    //获取Info的Hash值, 内部使用
	//Get the binary info hash of torrent file
    FTKERNELAPI_API const BYTE * BTAPI FTK_Torrent_GetInfoHash( HTorrentFile hTorrentFile );

    //获取info的Hash值, 十六进制
	//Get the hex encoding info hash of torrent file
    FTKERNELAPI_API LPCTSTR BTAPI FTK_Torrent_GetHexInfoHash( HTorrentFile hTorrentFile );

    //获取 main annouce url
	//Get mainb announce url
    FTKERNELAPI_API LPCTSTR BTAPI FTK_Torrent_GetMainAnnounce( HTorrentFile hTorrentFile );

    //获取 Announce 分组的个数
	//Get the number of all announce list
    FTKERNELAPI_API int BTAPI FTK_Torrent_GetAnnouncesCount( HTorrentFile hTorrentFile );

    //获取指定的 Announce 分组包含的Announce的个数
	//Get the size of a announce list
    FTKERNELAPI_API int BTAPI FTK_Torrent_GetAnnounceSize( HTorrentFile hTorrentFile, int nGroupIndex );

    //获取指定的 Announce 分组中指定位置的Announce
	//Get a annoucne of a announce list
    FTKERNELAPI_API LPCTSTR BTAPI FTK_Torrent_GetAnnouncesVal( HTorrentFile hTorrentFile, int nGroupIndex, int nIndex );

    //获取 注释
	//Get the comments
    FTKERNELAPI_API LPCTSTR BTAPI FTK_Torrent_GetComments( HTorrentFile hTorrentFile );

    //获取 Torrent 创建时间
	//Get the creation time of torrent file
    FTKERNELAPI_API time_t BTAPI FTK_Torrent_GetCreateTime( HTorrentFile hTorrentFile );

    //获取piece块的大小
	//Get the size of piece
    FTKERNELAPI_API DWORD BTAPI FTK_Torrent_GetPieceSize( HTorrentFile hTorrentFile );

    //获取文件的大小
	//Get the total file size to transfer
    FTKERNELAPI_API QWORD BTAPI FTK_Torrent_GetFileSize( HTorrentFile hTorrentFile );

    //Piece块的个数
	//Get the total number of piece
    FTKERNELAPI_API DWORD BTAPI FTK_Torrent_GetPieceCount( HTorrentFile hTorrentFile );

    //获取Piece Hash 的列表
	//Get a pointer to piece hash list
    FTKERNELAPI_API const BYTE *BTAPI FTK_Torrent_GetPieceHashList( HTorrentFile hTorrentFile );

	//////////////////////////////////////////////////////////////////////////
	//制作Torrent文件的函数列表
	//Create torrent file interface

	//打开一个制作torrent文件的句柄
	//Open a making torrent handle
	FTKERNELAPI_API HMakeTorrent BTAPI FTK_MT_Open();

	//关闭打开的制作Torrent文件的句柄
	//Close a making torrent handle
	FTKERNELAPI_API void BTAPI FTK_MT_Close( HMakeTorrent hMakeTorrent );

    //执行生成对象的操作
	//Start to making torrent file
    FTKERNELAPI_API BOOL BTAPI FTK_MT_MakeTorrent( HMakeTorrent hMakeTorrent );

	//停止Torrent文件制作
	//Stop making torrent file
	FTKERNELAPI_API void BTAPI FTK_MT_StopMake( HMakeTorrent hMakeTorrent );

    //获取制作文件的进度
	//Get progress of making torrent file
    FTKERNELAPI_API double BTAPI FTK_MT_GetMTProgress( HMakeTorrent hMakeTorrent );

    //另存为Torrent文件
	//Save as a torrent file
    FTKERNELAPI_API BOOL BTAPI FTK_MT_SaveAsTorrentFile( HMakeTorrent hMakeTorrent, LPCTSTR lpszFileName, LPCTSTR lpszDesKey = NULL );

    //设置 annouce url
	//Set a main announce url
    FTKERNELAPI_API void BTAPI FTK_MT_SetAnnounceUrl( HMakeTorrent hMakeTorrent, LPCTSTR lpszValue );

    //添加额外的Announce 列表
	//Add a announce list
    FTKERNELAPI_API void BTAPI FTK_MT_AddAnnounceList( HMakeTorrent hMakeTorrent, LPCTSTR Announces[], int nCount );

    //清空额外的Announce 列表
	//Clear all announce list
    FTKERNELAPI_API void BTAPI FTK_MT_ClearAnnounceList( HMakeTorrent hMakeTorrent );

    //设置 注释
	//Set comments
    FTKERNELAPI_API void BTAPI FTK_MT_SetComments( HMakeTorrent hMakeTorrent, LPCTSTR lpszValue );

    //设置piece块的大小
	//Set piece size to making torrent file
    FTKERNELAPI_API void BTAPI FTK_MT_SetPieceSize( HMakeTorrent hMakeTorrent, DWORD dwPieceSize );

    //设置torrent中的文件
	//Set the sourece file to making torrent file
    FTKERNELAPI_API void BTAPI FTK_MT_SetFile( HMakeTorrent hMakeTorrent, LPCTSTR lpszFileName );

    //设置torrent中的文件目录(多个文件)
	//Set the source path to making torrent file
    FTKERNELAPI_API void BTAPI FTK_MT_SetPath( HMakeTorrent hMakeTorrent, LPCTSTR lpszPathName );

    //设置Torrent文件的语言代码(只对文件名称, 注释等有效)
	//Set language code of torrent file
    FTKERNELAPI_API void BTAPI FTK_MT_SetLangCodePage( HMakeTorrent hMakeTorrent, UINT nCodePage = CP_ACP );

	//////////////////////////////////////////////////////////////////////////
}

#endif//__FTKERNELAPI_H__