#ifndef BEATS_COMPONENTS_COMPONENT_COMPONENTPROJECTDATA_H__INCLUDE
#define BEATS_COMPONENTS_COMPONENT_COMPONENTPROJECTDATA_H__INCLUDE

#include "../expdef.h"
#include <set>

class COMPONENTS_DLL_DECL CComponentProjectDirectory
{
public:
    CComponentProjectDirectory(CComponentProjectDirectory* pParent, const TString& strName);
    ~CComponentProjectDirectory();

    bool AddFile(const TString& strFileName, std::map<size_t, std::vector<size_t> >& conflictMap);
    CComponentProjectDirectory* AddDirectory(const TString& strDirectoryName);
    bool DeleteFile(size_t uFileId);
    bool DeleteDirectory(CComponentProjectDirectory* pDirectory);
    bool DeleteAll(bool bUpdateProject);

    const TString& GetName() const;
    const std::vector<CComponentProjectDirectory*>& GetChildren() const;
    const std::set<size_t>& GetFileList() const;
    CComponentProjectDirectory* GetParent() const;
    CComponentProjectDirectory* FindChild(const TCHAR* pszChildName) const;

    void Serialize(CSerializer& serializer) const;
    void Deserialize(CSerializer& serializer);

    TString GenerateLogicPath() const;

private:
    CComponentProjectDirectory* m_pParent;
    TString m_strName;
    std::set<size_t>* m_pFilesSet;
    std::vector<CComponentProjectDirectory*>* m_pChildrenVec;
};


#endif
