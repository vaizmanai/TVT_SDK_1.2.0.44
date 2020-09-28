
/************************************************************************
*������SDK
*���ܣ�
*	1�������ض��ṹ��¼���ļ���
*	2�������ض���ʽ�������ݣ�
*˵����
*	�������ض��ļ�ʱ���ļ��ṹ�谴���·�ʽ��֯��
*
*	֡��Ϣ��SDK_FRAME_INFO��֡��Ч��������
*	֡��Ϣ��SDK_FRAME_INFO��֡��Ч��������
*	......
*	֡��Ϣ��SDK_FRAME_INFO��֡��Ч��������
*
*	������Ƶ��Ϣ֡Ӧ����Ƶ֮֡ǰ����Ƶ��Ϣ֡Ӧ����Ƶ֡ǰ
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
	unsigned long		nLength;	//֡���ݳ���
	unsigned short		frameType;	//�ο�DD_FRAME_TYPE
	unsigned short		keyFrame;	//0���ǹؼ�֡ 1���ؼ�֡
	unsigned short		nWidth;		//�����, ��λ����, �������Ƶ������Ϊ0
	unsigned short		nHeight;	//�����, �������Ƶ������Ϊ0
	LONGLONG			nStamp;		//֡ʱ��, ��λ΢��, ��1970��1��1��0��0��0�뿪ʼ�ľ���ʱ��
}SDK_FRAME_INFO;

//������ͼ�����Ƶ��ʽ
enum PLAYER_FRAME_TYPE
{
	FRAME_TYPE_YUY2,
	FRAME_TYPE_YV12,
	FRAME_TYPE_RGB24,
	FRAME_TYPE_PCM16
};

//������������Ϣ�����ڽ����ص�
typedef struct _player_frame_info
{
	long		nWidth;		//�����, ��λ����, �������Ƶ������Ϊ0
	long		nHeight;	//�����, �������Ƶ������Ϊ0
	LONGLONG	nStamp;		//֡ʱ��, ��λ����, ��1970��1��1��0��0��0�뿪ʼ�ľ���ʱ��
	long		nType;		//��������, FRAME_TYPE_YUY2, FRAME_TYPE_YV12, FRAME_TYPE_PCM16, ����궨��˵��
}PLAYER_FRAME_INFO;

//������
#define E_PLAYER_NO_ERROR				0
#define E_PLAYER_INVALID_PORT			1
#define E_PLAYER_PORT_BUSY				2
#define E_PALYER_OPEN_FILE_FAILED		3
#define E_PLAYER_BAD_FORMAT_FILE		4
#define E_PLAYER_NOT_READY_FOR_PLAY		5
#define E_PLAYER_SUPPORT_FILE_ONLY		6
#define E_PLAYER_PARAM_ERROR			7
#define E_PLAYER_SUPPORT_STREAM_ONLY	8
#define E_PLAYER_DISPLAY_BUF_FULL		9		//���Ż���������������ʧ��
#define E_PLAYER_FILE_NO_OPEN			10		//�ļ�����ʱ���ļ���û�д�
#define E_PLAYER_FRAME_INDEX_NOT_READY	11		//�ļ�������Ϣ��û�н���
#define E_PLAYER_ALREADY_PLAY			12		//ͨ���Ѿ��ڲ���
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

	//�����ļ�ʱ��Ϊ�ļ�������������ص�
	typedef void (CALL_METHOD * FileRefDoneCallbackFun) (long nPort, void * pUser);
	
	typedef void (CALL_METHOD *PlayDrawCallBack)(long nPort, HDC hDC, long lWidth, long lHeight, void * pwUser);

	//����һ֡��ص�
	typedef void (CALL_METHOD * DecDataCallbackFun)(long nPort,const char * pBuf, long nSize, PLAYER_FRAME_INFO * pFrameInfo, void * pUser);

	//�ļ�������ʱ����Ҫ���õĻص�����
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
