#include "stdafx.h"
#include "ShadeModeRenderStateParam.h"
#include "Render/Renderer.h"

CShadeModeRenderStateParam::CShadeModeRenderStateParam()
{

}

CShadeModeRenderStateParam::CShadeModeRenderStateParam(CSerializer& serializer)
    : super(serializer)
{
    DECLARE_PROPERTY_ENUM(serializer, m_nValue, 0, NULL, EShadeModeType, true, 0xFFFFFFFF, _T("��ɫģʽ"), NULL, NULL, NULL);
}

void CShadeModeRenderStateParam::Apply()
{
    CRenderer::GetInstance()->ShadeModel(m_nValue);
}
