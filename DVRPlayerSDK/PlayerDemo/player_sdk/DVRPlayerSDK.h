
/************************************************************************
*播放器SDK
*功能：
*	1、播放特定结构的录像文件；
*	2、播放特定格式的流数据；
*说明：
*	当播放特定文件时，文件结构需按以下方式组织：
*
*	帧信息（SDK_FRAME_INFO）帧有效负载数据
*	帧信息（SDK_FRAME_INFO）帧有效负载数据
*	......
*	帧信息（SDK_FRAME_INFO）帧有效负载数据
*
*	并且视频信息帧应在视频帧之前，音频信息帧应在音频帧前
*
************************************************************************/

#ifndef _DVR_PLAYER_SDK_H_
#define _DVR_PLAYER_SDK_H_

#ifdef WIN32 
#include "stdafx.h"
#endif

#if defined(WIN32)
#ifdef DVRPLAYERSDK_EXPORTS
#define DVRPLAYERSDK_API EXTERN_C __declspec(dllexport)
#else
#define DVRPLAYERSDK_API EXTERN_C __declspec(dllimport)
#endif
#define CALL_METHOD __stdcall
#else
#define DVRPLAYERSDK_API
#define CALL_METHOD
#endif

#define DVR_PORT_NUM		100

typedef struct _sdk_frame_info
{
	unsigned long		nLength;	//帧数据长度
	unsigned short		frameType;	//参考DD_FRAME_TYPE
	unsigned short		keyFrame;	//0：非关键帧 1：关键帧
	unsigned short		nWidth;		//画面宽, 单位像素, 如果是音频数据则为0
	unsigned short		nHeight;	//画面高, 如果是音频数据则为0
	LONGLONG			nStamp;		//帧时间, 单位微秒, 从1970年1月1日0点0分0秒开始的绝对时间
}SDK_FRAME_INFO;

//解码后的图像或音频格式
enum PLAYER_FRAME_TYPE
{
	FRAME_TYPE_YUY2,
	FRAME_TYPE_YV12,
	FRAME_TYPE_RGB24,
	FRAME_TYPE_PCM16
};

//解码后的数据信息，用于解码后回调
typedef struct _player_frame_info
{
	long		nWidth;		//画面宽, 单位像素, 如果是音频数据则为0
	long		nHeight;	//画面高, 如果是音频数据则为0
	LONGLONG	nStamp;		//帧时间, 单位毫秒, 从1970年1月1日0点0分0秒开始的绝对时间
	long		nType;		//数据类型, FRAME_TYPE_YUY2, FRAME_TYPE_YV12, FRAME_TYPE_PCM16, 详见宏定义说明
}PLAYER_FRAME_INFO;

//错误码
#define E_PLAYER_NO_ERROR				0
#define E_PLAYER_INVALID_PORT			1
#define E_PLAYER_PORT_BUSY				2
#define E_PALYER_OPEN_FILE_FAILED		3
#define E_PLAYER_BAD_FORMAT_FILE		4
#define E_PLAYER_NOT_READY_FOR_PLAY		5
#define E_PLAYER_SUPPORT_FILE_ONLY		6
#define E_PLAYER_PARAM_ERROR			7
#define E_PLAYER_SUPPORT_STREAM_ONLY	8
#define E_PLAYER_DISPLAY_BUF_FULL		9		//播放缓冲区满，输入流失败
#define E_PLAYER_FILE_NO_OPEN			10		//文件播放时，文件还没有打开
#define E_PLAYER_FRAME_INDEX_NOT_READY	11		//文件索引信息还没有建立
#define E_PLAYER_ALREADY_PLAY			12		//通道已经在播放
#define E_PLAYER_BUFFER_TOO_SMALL		13
#define E_PLAYER_DATA_NOT_READY			14
#define E_PLAYER_UNKNOWN_ERROR			15

enum PLAY_SDK_RPB_SPEED
{
	PLAY_SDK_RPB_SPEED_1_32X = 1,
	PLAY_SDK_RPB_SPEED_1_16X,
	PLAY_SDK_RPB_SPEED_1_8X,		//1/8
	PLAY_SDK_RPB_SPEED_1_4X,		//1/4
	PLAY_SDK_RPB_SPEED_1_2X,		//1/2
	PLAY_SDK_RPB_SPEED_1X,		//1
	PLAY_SDK_RPB_SPEED_2X,
	PLAY_SDK_RPB_SPEED_4X,
	PLAY_SDK_RPB_SPEED_8X,
	PLAY_SDK_RPB_SPEED_16X,
	PLAY_SDK_RPB_SPEED_32X,
};


#ifdef __cplusplus
extern "C" {
#endif //#ifdef __cplusplus

	//播放文件时，为文件建立完索引后回调
	typedef void (CALL_METHOD * FileRefDoneCallbackFun) (long nPort, void * pUser);
	
	typedef void (CALL_METHOD *PlayDrawCallBack)(long nPort, HDC hDC, long lWidth, long lHeight, void * pwUser);

	//解码一帧后回调
	typedef void (CALL_METHOD * DecDataCallbackFun)(long nPort,const char * pBuf, long nSize, PLAYER_FRAME_INFO * pFrameInfo, void * pUser);

	//文件播放完时的需要调用的回调函数
	typedef void(CALL_METHOD * FileEndCallbackFun)(long nPort, void * pUser);

	DVRPLAYERSDK_API BOOL CALL_METHOD PLAYER_Initialize();
	DVRPLAYERSDK_API void CALL_METHOD PLAYER_UnInitialize();

	DVRPLAYERSDK_API DWORD CALL_METHOD PLAYER_GetSdkVersion();
	DVRPLAYERSDK_API DWORD CALL_METHOD PLAYER_GetLastError();

	DVRPLAYERSDK_API BOOL CALL_METHOD PLAYER_OpenFile(long nPort, const char * sFileName);
	DVRPLAYERSDK_API BOOL CALL_METHOD PLAYER_CloseFile(long nPort);

	DVRPLAYERSDK_API BOOL CALL_METHOD PLAYER_Play(long nPort, HWND hWnd);
	DVRPLAYERSDK_API BOOL CALL_METHOD PLAYER_Stop(long nPort);

	DVRPLAYERSDK_API BOOL CALL_METHOD PLAYER_Pause(long nPort, BOOL bPause);
	DVRPLAYERSDK_API BOOL CALL_METHOD PLAYER_Fast(long nPort,long speed);
	DVRPLAYERSDK_API BOOL CALL_METHOD PLAYER_BackFast(long nPort,long speed);
	DVRPLAYERSDK_API BOOL CALL_METHOD PLAYER_SetPlayPos(long nPort, float fRelativePos);
	DVRPLAYERSDK_API float CALL_METHOD PLAYER_GetPlayPos(long nPort);
	DVRPLAYERSDK_API BOOL CALL_METHOD PLAYER_SetFileEndMsg(long nPort, HWND hWnd, UINT nMsg);
	DVRPLAYERSDK_API BOOL CALL_METHOD PLAYER_OneByOne(long nPort);
	DVRPLAYERSDK_API DWORD CALL_METHOD PLAYER_GetFileTime(long nPort);
	DVRPLAYERSDK_API DWORD CALL_METHOD PLAYER_GetPlayedTime(long nPort);

	DVRPLAYERSDK_API BOOL CALL_METHOD PLAYER_SetDecCallBack(long nPort, DecDataCallbackFun pfnDecDataCallback, void * pUser);
	DVRPLAYERSDK_API BOOL CALL_METHOD PLAYER_SetDrawCallBack(long nPort,PlayDrawCallBack pfnDrawCallback, void * pUser);
	DVRPLAYERSDK_API BOOL CALL_METHOD PLAYER_ConvertToBmpFile(char * pBuf, long nSize, long nWidth, long nHeight, long nType, const char *sFileName);
	DVRPLAYERSDK_API BOOL CALL_METHOD PLAYER_SetFileEndCallback(long nPort, FileEndCallbackFun pfnFileEnd, void * pUser);

	DVRPLAYERSDK_API DWORD CALL_METHOD PLAYER_GetFileHeadLength();

	DVRPLAYERSDK_API BOOL CALL_METHOD PLAYER_OpenStream(long nPort, PBYTE pFileHeadBuf, DWORD nSize, DWORD nBufPoolSize);
	DVRPLAYERSDK_API BOOL CALL_METHOD PLAYER_InputData(long nPort, SDK_FRAME_INFO *pFrameInfo, PBYTE pBuf, DWORD nSize);
	DVRPLAYERSDK_API BOOL CALL_METHOD PLAYER_CloseStream(long nPort);

	DVRPLAYERSDK_API BOOL CALL_METHOD PLAYER_SetVolume(long nPort, WORD nVolume);
	DVRPLAYERSDK_API BOOL CALL_METHOD PLAYER_PlaySound(long nPort);
	DVRPLAYERSDK_API BOOL CALL_METHOD PLAYER_StopSound();

	DVRPLAYERSDK_API BOOL CALL_METHOD PLAYER_RefreshPlay(long nPort);

	DVRPLAYERSDK_API BOOL CALL_METHOD PLAYER_GetPictureSize(long nPort, long *pWidth, long *pHeight);

	DVRPLAYERSDK_API BOOL CALL_METHOD PLAYER_SetDisplayBufTime(long nPort, DWORD nMillSeconds);

	DVRPLAYERSDK_API BOOL CALL_METHOD PLAYER_SetFileRefCallBack(long nPort, FileRefDoneCallbackFun pfnFileRefDone, void * pUser);

	DVRPLAYERSDK_API BOOL CALL_METHOD PLAYER_GetBMP(long nPort, PBYTE pBitmap, DWORD nBufSize, DWORD* pBmpSize);

	DVRPLAYERSDK_API BOOL CALL_METHOD PLAYER_SetFrameType(long nPort, DWORD nType);

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus


#endif
