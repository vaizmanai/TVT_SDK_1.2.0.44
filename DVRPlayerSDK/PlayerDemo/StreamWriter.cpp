#include "stdafx.h"
#include "StreamWriter.h"


CStreamWriter::CStreamWriter()
{
	m_pFile = NULL;

	m_bGetVedioFormat = false;

	m_pAudioFormat = NULL;
	m_pAudioFormatData = NULL;

	m_handle = 0xffffffff;
}

CStreamWriter::~CStreamWriter()
{
	if (m_pFile != NULL)
	{
		fclose(m_pFile);
	}

	if (m_pAudioFormat != NULL && m_pAudioFormatData != NULL)
	{
		delete [] m_pAudioFormat;
		delete [] m_pAudioFormatData;
	}
}


bool CStreamWriter::Init(const char *pFilePath)
{
	if(m_pFile != NULL)
	{
		return true;
	}

	m_pFile = fopen(pFilePath,"wb");

	return m_pFile != NULL;
}


void CStreamWriter::InputData(unsigned int handle,NET_SDK_FRAME_INFO *pNetFrameInfo,unsigned char *pData)
{
	if(m_pFile == NULL)
	{
		return ;
	}

	if(0xffffffff == m_handle)
	{
		m_handle = handle;
	}

	if(m_handle != handle)
	{
		return;
	}

	SDK_FRAME_INFO sdkframeInfo;
	sdkframeInfo.frameType = pNetFrameInfo->frameType;
	sdkframeInfo.keyFrame  = pNetFrameInfo->keyFrame;
	sdkframeInfo.nHeight   = pNetFrameInfo->height;
	sdkframeInfo.nWidth    = pNetFrameInfo->width;
	sdkframeInfo.nLength   = pNetFrameInfo->length;
	sdkframeInfo.nStamp    = pNetFrameInfo->time;


	if (m_bGetVedioFormat)
	{
		WriterToFile(&sdkframeInfo,pData);
	}
	else
	{
		if (sdkframeInfo.frameType == DD_FRAME_TYPE_VIDEO_FORMAT)
		{
			WriterToFile(&sdkframeInfo,pData);
			m_bGetVedioFormat = true;

			if (m_pAudioFormat != NULL && m_pAudioFormatData != NULL)
			{
				WriterToFile(m_pAudioFormat,m_pAudioFormatData);
			}
		}
		else if (sdkframeInfo.frameType == DD_FRAME_TYPE_TALK_AUDIO_FORMAT)
		{
			if (m_pAudioFormat == NULL && m_pAudioFormatData == NULL)
			{
				m_pAudioFormat = new SDK_FRAME_INFO;
				memcpy(m_pAudioFormat,&sdkframeInfo,sizeof(SDK_FRAME_INFO));
				m_pAudioFormatData = new unsigned char[m_pAudioFormat->nLength];
				memcpy(m_pAudioFormatData,pData,m_pAudioFormat->nLength);
			}
		}
	}
}


void CStreamWriter::SetHandle(unsigned int handle)
{
	m_handle = handle;
}


void CStreamWriter::WriterToFile(SDK_FRAME_INFO *pFrameInfo,unsigned char *pData)
{
	if (m_pFile != NULL)
	{
		fwrite(pFrameInfo,sizeof(SDK_FRAME_INFO),1,m_pFile);
		fwrite(pData,pFrameInfo->nLength,1,m_pFile);
		fflush(m_pFile);
	}
}