#ifndef  _STREAM_WRITER_H_
#define  _STREAM_WRITER_H_

#include "DVRPlayerSDK.h"
#include "DVR_NET_SDK.h"
#include <stdio.h>




class CStreamWriter
{
public:
	CStreamWriter();
	~CStreamWriter();
	
	bool Init(const char *pFilePath);
	void InputData(unsigned int handle,NET_SDK_FRAME_INFO *pNetFrameInfo,unsigned char *pData);

	void SetHandle(unsigned int handle);

private:
	void WriterToFile(SDK_FRAME_INFO *pFrameInfo,unsigned char *pData);

private:
	FILE           *m_pFile;

	bool           m_bGetVedioFormat;
	SDK_FRAME_INFO *m_pAudioFormat;
	unsigned char  *m_pAudioFormatData;

	unsigned int	m_handle;
};

#endif