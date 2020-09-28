#include "StdAfx.h"
#include "LangStr.h"
#include "resource.h"

CLangStr LANG;

void CLangStr::LoadStr()
{
	LANGID lang = GetSystemDefaultLangID();
	int start = 0;
	if (PRIMARYLANGID(lang) == LANG_CHINESE)
	{
		start = IDS_CHINESE + 1;
		m_bCN = TRUE;
	}
	else
	{
		start = IDS_ENGLISH + 1;
		m_bCN = FALSE;
	}

	for (int i = 0; i < STR_END; i++)
	{
		m_str[i].LoadString(start + i);
	}
}
