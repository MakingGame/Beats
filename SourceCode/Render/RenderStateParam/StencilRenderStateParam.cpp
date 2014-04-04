#include "stdafx.h"
#include "StencilRenderStateParam.h"
#include "Render/Renderer.h"

CStencilRenderStateParam::CStencilRenderStateParam()
{

}

CStencilRenderStateParam::CStencilRenderStateParam(CSerializer& serializer)
    : super(serializer)
{
    DECLARE_PROPERTY_ENUM(serializer, m_nFail, 0, NULL, EStencilType, true, 0xFFFFFFFF, _T("ʧ��ģʽ"), NULL, NULL, NULL);
    DECLARE_PROPERTY_ENUM(serializer, m_nZFail, 0, NULL, EStencilType, true, 0xFFFFFFFF, _T("Zʧ��ģʽ"), NULL, NULL, NULL);
    DECLARE_PROPERTY_ENUM(serializer, m_nZPass, 0, NULL, EStencilType, true, 0xFFFFFFFF, _T("Z�ɹ�ģʽ"), NULL, NULL, NULL);
}

void CStencilRenderStateParam::Apply()
{
    CRenderer::GetInstance()->StencilOp(m_nFail, m_nZFail, m_nZPass);
}

