#pragma once

typedef enum
{
	STR_USERNAME = 0,
	STR_PASSWORD,
	STR_IPADDRESS,
	STR_PORT,
	STR_MUTE,
	STR_SNAP,
	STR_AUDIO,
	STR_OPEN_FILE_FAILED,
	STR_OPEN_STREAM_FAILED,
	STR_CHANNEL_D,
	STR_SERVER_NONE,
	STR_SERVER_S,
	STR_FAILED,
	STR_CREATE_FILE_FAILED,
	STR_PLAY,
	STR_PAUSE,
	STR_END
}STR_DEF;

class CLangStr
{
public:
	
	void LoadStr();

	BOOL IsChinese(){return m_bCN;}
	CString operator[](int index)
	{
		if (index >= 0 && index < STR_END)
		{
			return m_str[index];
		}
		else
		{
			return "";
		}
	}
private:
	CString m_str[STR_END];
	BOOL	m_bCN;
};

extern CLangStr LANG;