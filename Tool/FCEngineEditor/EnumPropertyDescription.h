#ifndef BEATS_COMPONENTS_PROPERTY_WXWIDGETSIMPLEMENT_ENUMPROPERTYDESCRIPTION_H__INCLUDE
#define BEATS_COMPONENTS_PROPERTY_WXWIDGETSIMPLEMENT_ENUMPROPERTYDESCRIPTION_H__INCLUDE

#include "wxwidgetsPropertyBase.h"

struct SEnumData;
struct SEnumPropertyData
{
    SEnumPropertyData(const std::vector<SEnumData*>* pData)
        : m_pData(pData)
    {
        BEATS_ASSERT(pData != NULL);
    }
    ~SEnumPropertyData()
    {
    }

    const std::vector<SEnumData*>* m_pData;
    std::vector<TString> m_displayString;
};

// this Wrapper only for recycle memory after the global static variable is destructed.
struct SEnumPropertyDataMapWrapper
{
    SEnumPropertyDataMapWrapper()
    {

    }
    ~SEnumPropertyDataMapWrapper()
    {
        typedef std::map<TString, SEnumPropertyData*> TEnumPropertyDataType;
        BEATS_SAFE_DELETE_MAP(m_data, TEnumPropertyDataType);
    }

    std::map<TString, SEnumPropertyData*> m_data;
};
class CEnumPropertyDescription : public CWxwidgetsPropertyBase
{
    typedef CWxwidgetsPropertyBase super;
public:
    CEnumPropertyDescription(CSerializer* pSerializer);
    CEnumPropertyDescription(const CEnumPropertyDescription& rRef);

    virtual ~CEnumPropertyDescription();

    virtual bool AnalyseUIParameterImpl(const std::vector<TString>& parameterUnit, bool bSerializePhase = false);
    virtual wxPGProperty* CreateWxProperty();
    virtual void SetValue(wxVariant& value, bool bSaveValue = true);
    virtual void SetValue(void* pValue, EValueType type);
    virtual bool IsDataSame(bool bWithDefaultOrXML);
    virtual CPropertyDescriptionBase* CreateNewInstance();
    virtual void GetValueAsChar(EValueType type, char* pOut);
    virtual void Serialize(CSerializer& serializer);

private:
    SEnumPropertyData* m_pPropertyData;
    static SEnumPropertyDataMapWrapper m_enumPropertyDataMap;
};
#endif