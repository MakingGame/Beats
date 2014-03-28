// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#pragma warning(disable:4611) // for load png warning

#define WIN32_DEV
//#define WIN64_DEV
//#define ANDROID_DEV
//#define IOS_DEV

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
#include <windows.h>

#include <thread>
#include "GL/glew.h"
#include "glfw3.h"
#include "CCConsole.h"
#include "CCGeometry.h" //TODO: For to use some basic data type, such as "Size" "Rect"
#include "ccUtils.h"

//STL headers
#include <list>
#include <map>
#include <vector>
#include <set>

#include "PublicDef.h"

#include "math/kazmath/include/kazmath/GL/matrix.h"
#include "math/kazmath/include/kazmath/mat4.h"
#include "math/kazmath/include/kazmath/vec4.h"
#include "math/kazmath/include/kazmath/vec3.h"
#include "math/kazmath/include/kazmath/vec2.h"
#include "Utility/BeatsUtility/SharePtr.h"

#include "Render/RenderPublic.h"
#include "Resource/ResourcePublic.h"
