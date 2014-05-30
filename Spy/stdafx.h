// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "BeatsPlatFormConfig.h"

#if (BEATS_PLATFORM == BEATS_PLATFORM_WIN32)
#ifndef WINVER                // Allow use of features specific to Windows XP or later.
#define WINVER 0x0501        // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT        // Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501    // Change this to the appropriate value to target other versions of Windows.
#endif                        

#ifndef _WIN32_WINDOWS        // Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE            // Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600    // Change this to the appropriate value to target other versions of IE.
#endif

#define WIN32_LEAN_AND_MEAN        // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#endif

#include "BeatsTchar.h"
#include "BeatsTypeDef.h"

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <vector>
#include <map>
#include "Beats/PublicDef.h"
#include "Utility/Serializer/Serializer.h"
#include "Utility/SharePtr/SharePtr.h"
#include "SpyMessage.h"

#define BEATS_SPY_VERSION 0
