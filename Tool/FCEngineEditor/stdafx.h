// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include "PublicDef.h"
#include "wx/wx.h"

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// TODO: reference additional headers your program requires here
// STL Headers
#include <vector>
#include <map>
#include <list>
#include <mutex>
#include <thread>

#include "GL/glew.h"
#include "Resource/ResourcePublic.h"
#include "Render/RenderPublic.h"
#include "Utility/BeatsUtility/SharePtr.h"

#define NULLDATA -1
#define DEFAULT_PGBUTTONWIDTH 20
#define DEFAULT_DEPTH 32
#define WHEELSPEED 3.0F * 0.016F
#define SHIFTWHEELSPEED 6.0F * 0.016F
#define MAXFPS 60
#define WARNING "ERROR : "