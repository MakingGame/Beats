#ifndef BEATS_COMPONENTS_COMPONENT_COMPONENTINSTANCE_H__INCLUDE
#define BEATS_COMPONENTS_COMPONENT_COMPONENTINSTANCE_H__INCLUDE

#include "../expdef.h"
#include "ComponentBase.h"

class COMPONENTS_DLL_DECL CComponentInstance : public CComponentBase
{
    typedef CComponentBase super;
public:
    CComponentInstance();
    virtual ~CComponentInstance();
    virtual void Uninitialize() override;
    DECLARE_REFLECT_GUID(CComponentInstance, 0x4314AEF, CComponentBase)

public:
    void SetProxyComponent(class CComponentProxy* pProxy);
    class CComponentProxy* GetProxyComponent() const;
private:
    class CComponentProxy* m_pProxyComponent;
};

#endif