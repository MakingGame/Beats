#include "stdafx.h"
#include "ComponentProjectDirectory.h"
#include "ComponentProject.h"
#include "ComponentProxyManager.h"

CComponentProjectDirectory::CComponentProjectDirectory(CComponentProjectDirectory* pParent, const TString& strName)
: m_pParent(pParent)
, m_strName(strName)
{
    if (pParent != NULL)
    {
        pParent->m_pChildrenVec->push_back(this);
    }
    m_pFilesList = new std::vector<size_t>();
    m_pChildrenVec = new std::vector<CComponentProjectDirectory*>();
}

CComponentProjectDirectory::~CComponentProjectDirectory()
{
    DeleteAll(false);
    BEATS_SAFE_DELETE(m_pFilesList);
    BEATS_SAFE_DELETE(m_pChildrenVec);
}

bool CComponentProjectDirectory::AddFile(const TString& strFileName, std::map<size_t, std::vector<size_t> >& conflictMap)
{
    CComponentProject* pProject = CComponentProxyManager::GetInstance()->GetProject();
    size_t uFileId = pProject->RegisterFile(strFileName, conflictMap);
#ifdef _DEBUG
    bool bFind = false;
    for (size_t i = 0; i < m_pFilesList->size(); ++i)
    {
        if (m_pFilesList->at(i) == uFileId)
        {
            bFind = true;
            break;
        }
    }
    BEATS_ASSERT(!bFind, _T("File %s is already registered with id %d"), strFileName.c_str(), uFileId);

#endif
    m_pFilesList->push_back(uFileId);
    return true;
}

CComponentProjectDirectory* CComponentProjectDirectory::AddDirectory(const TString& strDirectoryName)
{
    CComponentProjectDirectory* pRet = NULL;
    bool bAlreadyExists = false;
    std::vector<CComponentProjectDirectory*>::iterator iter = m_pChildrenVec->begin();
    for (; iter != m_pChildrenVec->end(); ++iter)
    {
        if ((*iter)->m_strName.compare(strDirectoryName) == 0)
        {
            bAlreadyExists = true;
            break;
        }
    }
    if (!bAlreadyExists)
    {
        pRet = new CComponentProjectDirectory(this, strDirectoryName);
    }
    return pRet;
}

void CComponentProjectDirectory::InsertDirectory(CComponentProjectDirectory* pDirectory, CComponentProjectDirectory* pPrevDirectory/* = NULL*/)
{
#ifdef _DEBUG
    for (size_t i = 0; i < m_pChildrenVec->size(); i++)
    {
        if (m_pChildrenVec->at(i) == pDirectory)
        {
            BEATS_ASSERT(false, _T("Can't insert a directory which already exists"));
            break;
        }
    }
#endif
    if (pPrevDirectory != NULL)
    {
        bool bFind = false;
        for (auto iter = m_pChildrenVec->begin(); iter != m_pChildrenVec->end(); ++iter)
        {
            if (*iter == pPrevDirectory)
            {
                m_pChildrenVec->insert(iter, pDirectory);
                bFind = true;
                break;
            }
        }
        BEATS_ASSERT(bFind, _T("Previous directory is not found!"));
    }
    else
    {
        m_pChildrenVec->push_back(pDirectory);
    }
    pDirectory->SetParent(this);
}

void CComponentProjectDirectory::InsertFile(size_t uFileId, size_t uPrevFileId/* = 0xFFFFFFFF*/)
{
#ifdef _DEBUG
    bool bFind = false;
    for (size_t i = 0; i < m_pFilesList->size(); ++i)
    {
        if (m_pFilesList->at(i) == uFileId)
        {
            bFind = true;
            break;
        }
    }
    BEATS_ASSERT(!bFind, _T("Can't insert file with id %d which exists already!"), uFileId);
#endif
    if (uPrevFileId != 0xFFFFFFFF)
    {
        bool bFind = false;
        for (auto iter = m_pFilesList->begin(); iter != m_pFilesList->end(); ++iter)
        {
            if (*iter == uPrevFileId)
            {
                bFind = true;
                m_pFilesList->insert(iter, uFileId);
                break;
            }
        }
        BEATS_ASSERT(bFind, _T("Can't insert file %d before previous file %d"), uFileId, uPrevFileId);
    }
    else
    {
        m_pFilesList->push_back(uFileId);
    }
}

bool CComponentProjectDirectory::DeleteFile(size_t uFileId)
{
    bool bRet = false;
    if (RemoveFile(uFileId))
    {
        CComponentProxyManager* pComopnentManager = CComponentProxyManager::GetInstance();
        CComponentProject* pProject = pComopnentManager->GetProject();
        TString strFileName = pProject->GetComponentFileName(uFileId);
        bool bDeleteCurWorkingFile = strFileName.compare(pComopnentManager->GetCurrentWorkingFilePath()) == 0;
        if (bDeleteCurWorkingFile)
        {
            pComopnentManager->CloseFile(strFileName.c_str());
        }
        pProject->UnregisterFile(uFileId);
        bRet = true;
    }
    return bRet;
}

bool CComponentProjectDirectory::RemoveFile(size_t uFileId)
{
    bool bRet = false;
    for (std::vector<size_t>::iterator iter = m_pFilesList->begin(); iter != m_pFilesList->end(); ++iter)
    {
        if (*iter == uFileId)
        {
            m_pFilesList->erase(iter);
            bRet = true;
            break;
        }
    }
    if (!bRet)
    {
        for (std::vector<CComponentProjectDirectory*>::iterator iter = m_pChildrenVec->begin(); iter != m_pChildrenVec->end(); ++iter)
        {
            bRet = (*iter)->RemoveFile(uFileId);
            if (bRet)
            {
                break;
            }
        }
    }

    return bRet;
}

bool CComponentProjectDirectory::DeleteDirectory(CComponentProjectDirectory* pDirectory)
{
    bool bRet = RemoveDirectory(pDirectory);
    if (bRet)
    {
        BEATS_SAFE_DELETE(pDirectory);
    }
    return bRet;
}

bool CComponentProjectDirectory::RemoveDirectory(CComponentProjectDirectory* pDirectory)
{
    bool bRet = false;
    for (std::vector<CComponentProjectDirectory*>::iterator iter = m_pChildrenVec->begin(); iter != m_pChildrenVec->end(); ++iter)
    {
        if ((*iter) == pDirectory)
        {
            m_pChildrenVec->erase(iter);
            pDirectory->SetParent(NULL);
            bRet = true;
            break;
        }
    }
    return bRet;
}

const TString& CComponentProjectDirectory::GetName() const
{
    return m_strName;
}

const std::vector<CComponentProjectDirectory*>& CComponentProjectDirectory::GetChildren() const
{
    return *m_pChildrenVec;
}

const std::vector<size_t>& CComponentProjectDirectory::GetFileList() const
{
    return *m_pFilesList;
}

void CComponentProjectDirectory::SetParent(CComponentProjectDirectory* pParent)
{
    BEATS_ASSERT(m_pParent == NULL || pParent == NULL);
    m_pParent = pParent;
}

CComponentProjectDirectory* CComponentProjectDirectory::GetParent() const
{
    return m_pParent;
}

CComponentProjectDirectory* CComponentProjectDirectory::FindChild(const TCHAR* pszChildName) const
{
    CComponentProjectDirectory* pRet = NULL;
    for (size_t i = 0; i < m_pChildrenVec->size(); ++i)
    {
        if (m_pChildrenVec->at(i)->GetName().compare(pszChildName) == 0)
        {
            pRet = m_pChildrenVec->at(i);
            break;
        }
    }
    return pRet;
}

void CComponentProjectDirectory::Serialize(CSerializer& serializer) const
{
    serializer << m_strName;
    serializer << m_pFilesList->size();
    for (std::vector<size_t>::iterator iter = m_pFilesList->begin(); iter != m_pFilesList->end(); ++iter)
    {
        serializer << *iter;
    }
    serializer << m_pChildrenVec->size();
    for (size_t i = 0; i < m_pChildrenVec->size(); ++i)
    {
        (*m_pChildrenVec)[i]->Serialize(serializer);
    }
}

void CComponentProjectDirectory::Deserialize(CSerializer& serializer)
{
    serializer >> m_strName;
    size_t uFileCount = 0;
    serializer >> uFileCount;
    for (size_t i = 0; i < uFileCount; ++i)
    {
        size_t uFileId = 0;
        serializer >> uFileId;
        m_pFilesList->push_back(uFileId);
    }
    size_t uChildrenCount = 0;
    serializer >> uChildrenCount;
    for (size_t i = 0; i < uChildrenCount; ++i)
    {
        CComponentProjectDirectory* pDirectory = new CComponentProjectDirectory(this, _T(""));
        pDirectory->Deserialize(serializer);
    }
}

TString CComponentProjectDirectory::GenerateLogicPath() const
{
    std::vector<TString> vecPath;
    const CComponentProjectDirectory* pCurDirectory = this;
    while (pCurDirectory != NULL)
    {
        vecPath.push_back(pCurDirectory->GetName());
        pCurDirectory = pCurDirectory->GetParent();
    }
    TString strRet;
    while (vecPath.size() > 0)
    {
        strRet.append(vecPath.back()).append(_T("/"));
        vecPath.pop_back();
    }
    return strRet;
}

TString CComponentProjectDirectory::MakeRelativeLogicPath(CComponentProjectDirectory* pFromDirectory)
{
    CComponentProjectDirectory* pCurDirectory = pFromDirectory;
    std::vector<CComponentProjectDirectory*> fromPath;
    while (pCurDirectory != NULL)
    {
        fromPath.push_back(pCurDirectory);
        pCurDirectory = pCurDirectory->GetParent();
    }
    pCurDirectory = this;
    std::vector<CComponentProjectDirectory*> toPath;
    while (pCurDirectory != NULL)
    {
        toPath.push_back(pCurDirectory);
        pCurDirectory = pCurDirectory->GetParent();
    }
    BEATS_ASSERT(toPath.back() == fromPath.back() && toPath.back() == CComponentProxyManager::GetInstance()->GetProject()->GetRootDirectory());
    while (fromPath.size() > 0 && toPath.size() > 0 && fromPath.back() == toPath.back())
    {
        fromPath.pop_back();
        toPath.pop_back();
    }
    TString strRet;
    while (fromPath.size() > 0)
    {
        strRet.append(_T("../"));
        fromPath.pop_back();
    }
    while (toPath.size() > 0)
    {
        strRet.append(toPath.back()->GetName());
        toPath.pop_back();
    }
    return strRet;
}

bool CComponentProjectDirectory::DeleteAll(bool bUpdateProject)
{
    for (std::vector<CComponentProjectDirectory*>::iterator iter = m_pChildrenVec->begin(); iter != m_pChildrenVec->end(); ++iter)
    {
        BEATS_SAFE_DELETE(*iter);
    }
    m_pChildrenVec->clear();
    if (bUpdateProject)
    {
        while (m_pFilesList->size() != 0)
        {
            DeleteFile(*m_pFilesList->begin());
        }
    }
    else
    {
        m_pFilesList->clear();
    }
    return true;
}
