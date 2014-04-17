#include "stdafx.h"
#include "IntPropertyDescription.h"
#include "../../Utility/StringHelper/StringHelper.h"
#include "../../../Utility/Serializer/Serializer.h"

CIntPropertyDescription::CIntPropertyDescription(CSerializer* pSerializer)
: super(ePT_Int)
, m_minValue(INT_MIN)
, m_maxValue(INT_MAX)
, m_spinStep(0)
{
    int iValue = 0;
    if (pSerializer != NULL)
    {
        (*pSerializer) >> iValue;
    }
    InitializeValue(iValue);
}

CIntPropertyDescription::CIntPropertyDescription(const CIntPropertyDescription& rRef)
: super(rRef)
, m_minValue(rRef.m_minValue)
, m_maxValue(rRef.m_maxValue)
, m_spinStep(rRef.m_spinStep)

{
    InitializeValue<int>(0);
}

CIntPropertyDescription::~CIntPropertyDescription()
{
    DestoryValue<int>();
}

bool CIntPropertyDescription::AnalyseUIParameterImpl(const std::vector<TString>& result, bool bSerializePhase/* = false */)
{
    std::vector<TString> cache;
    for (size_t i = 0; i < result.size(); ++i)
    {
        cache.clear();
        CStringHelper::GetInstance()->SplitString(result[i].c_str(), _T(":"), cache);
        BEATS_ASSERT(cache.size() == 2);
        if (_tcsicmp(cache[0].c_str(), UIParameterAttrStr[eUIPAT_DefaultValue]) == 0)
        {
            int iValue = _tstoi(cache[1].c_str());
            wxVariant var(iValue);
            SetValue(var, true);
            if (bSerializePhase)
            {
                SetValue(&iValue, eVT_DefaultValue);
            }
        }
        else if (_tcsicmp(cache[0].c_str(), UIParameterAttrStr[eUIPAT_MinValue]) == 0)
        {
            m_minValue = _tstoi(cache[1].c_str());
        }
        else if (_tcsicmp(cache[0].c_str(), UIParameterAttrStr[eUIPAT_MaxValue]) == 0)
        {
            m_maxValue = _tstoi(cache[1].c_str());
        }
        else if (_tcsicmp(cache[0].c_str(), UIParameterAttrStr[eUIPAT_SpinStep]) == 0)
        {
            m_spinStep = _tstoi(cache[1].c_str());
        }
        else
        {
            BEATS_ASSERT(false, _T("Unknown parameter for Int property %s"), cache[0].c_str());
        }
        BEATS_ASSERT(errno == 0, _T("call _tstoi failed! string: %s"), cache[1].c_str());
    }
    BEATS_ASSERT(*(int*)m_valueArray[eVT_CurrentValue] < m_maxValue && *(int*)m_valueArray[eVT_CurrentValue] > m_minValue);
    return true;
}

wxPGProperty* CIntPropertyDescription::CreateWxProperty()
{
    wxPGProperty* pProperty = new wxIntProperty(wxPG_LABEL, wxPG_LABEL, *(int*)m_valueArray[eVT_CurrentValue]);
    pProperty->SetClientData(this);
    wxVariant var(*(int*)m_valueArray[eVT_DefaultValue]);
    pProperty->SetDefaultValue(var);
    pProperty->SetModifiedStatus(!IsDataSame(true));
    pProperty->SetAttribute( wxPG_ATTR_MIN, m_minValue );
    pProperty->SetAttribute( wxPG_ATTR_MAX, m_maxValue );
    if (m_spinStep > 0)
    {
        pProperty->SetEditor( wxPGEditor_SpinCtrl );
        pProperty->SetAttribute( wxT("Step"), (int)m_spinStep );
        pProperty->SetAttribute( wxT("MotionSpin"), true );
    }

    return pProperty;
}

void CIntPropertyDescription::SetValue( wxVariant& value, bool bSaveValue /*= true*/ )
{
    int iNewValue = value.GetInteger();
    SetValue(&iNewValue, eVT_CurrentValue);
    if (bSaveValue)
    {
        SetValue(&iNewValue, eVT_SavedValue);
    }    
}

void CIntPropertyDescription::SetValue( void* pValue, EValueType type )
{
    *(int*)m_valueArray[type] = *(int*)pValue;
}

bool CIntPropertyDescription::IsDataSame( bool bWithDefaultOrXML )
{
    bool bRet = *(int*)m_valueArray[(bWithDefaultOrXML ? eVT_DefaultValue : eVT_SavedValue)] == *(int*)m_valueArray[eVT_CurrentValue];
    return bRet;
}

CPropertyDescriptionBase* CIntPropertyDescription::CreateNewInstance()
{
    CPropertyDescriptionBase* pNewProperty = new CIntPropertyDescription(*this);
    return pNewProperty;
}

void CIntPropertyDescription::GetValueAsChar( EValueType type, char* pOut )
{
    int iValue = *(int*)m_valueArray[type];
    sprintf(pOut, "%d", iValue);
}

void CIntPropertyDescription::Serialize( CSerializer& serializer )
{
    serializer << *(int*)m_valueArray[(eVT_SavedValue)];
}
