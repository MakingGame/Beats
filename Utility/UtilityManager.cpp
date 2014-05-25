#include "stdafx.h"
#include "UtilityManager.h"
#include "Serializer/Serializer.h"
#include "EnumStrGenerator/EnumStrGenerator.h"
#include "StringHelper/StringHelper.h"
#include "ServiceManager/ServiceManager.h"
#include "FileFilter/FileFilter.h"
#include "MD5/md5.h"
#include <Commdlg.h>
#include <psapi.h>
#include <shlobj.h>
#include <cderr.h>

#pragma comment(lib, "psapi.lib")

CUtilityManager* CUtilityManager::m_pInstance = NULL;

CUtilityManager::CUtilityManager()
{
}


CUtilityManager::~CUtilityManager()
{
    CStringHelper::Destroy();
    CEnumStrGenerator::Destroy();
    CServiceManager::Destroy();
}

bool CUtilityManager::AcquireSingleFilePath(bool saveOrOpen, HWND hwnd, TString& result, const TCHAR* Tiltle, const TCHAR* filter, const TCHAR* pszInitialPath)
{
    TCHAR fileName[MAX_PATH];
    OPENFILENAME dataStruct;
    memset(&dataStruct, 0, sizeof(dataStruct));
    memset(fileName, 0, MAX_PATH);

    dataStruct.lStructSize = sizeof(dataStruct);
    dataStruct.hwndOwner = hwnd;
    dataStruct.lpstrFile = fileName;
    dataStruct.lpstrTitle = Tiltle;
    dataStruct.lpstrFilter = filter;
    dataStruct.nMaxFile = MAX_PATH;
    dataStruct.lpstrInitialDir = pszInitialPath;
    dataStruct.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
    BOOL confirm = saveOrOpen ? GetSaveFileName(&dataStruct) : GetOpenFileName(&dataStruct);
    DWORD returnFlag = CommDlgExtendedError();
    bool success = confirm == TRUE && returnFlag == 0;
    if (success)
    {
        result.assign(dataStruct.lpstrFile);
    }
    return success;
}

WNDPROC g_lOriWndProc = NULL;
LRESULT CALLBACK WindowProcNew(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)  
{  
    if(message == WM_COMMAND && HIWORD(wParam) == BN_CLICKED)  
    {  
        if(LOWORD(wParam) == IDOK)
        {
            EndDialog(hwnd, IDOK);
            return 0;
        }
    }
    return CallWindowProc(g_lOriWndProc, hwnd, message, wParam, lParam);  
}  

UINT_PTR CALLBACK FolderHookProc(HWND hdlg, UINT uiMsg, WPARAM /*wParam*/, LPARAM lParam)
{
    if(uiMsg == WM_NOTIFY)
    {
        LPNMHDR hdr = (LPNMHDR)lParam;
        if(hdr->code == CDN_INITDONE)
        {
            HWND hParent = GetParent(hdlg);
            g_lOriWndProc = (WNDPROC)SetWindowLongPtr(hParent, GWLP_WNDPROC, (LPARAM)WindowProcNew);
        }
        else if(hdr->code == CDN_SELCHANGE)
        {  
            LPOFNOTIFY lpofn = (LPOFNOTIFY)lParam;
            HWND hParent = GetParent(hdlg);
            HWND hListCtrl = GetDlgItem(hParent, 0x0461);//lst2
            hListCtrl = GetDlgItem(hListCtrl, 1);
            if (hListCtrl != NULL)
            {
                size_t uSelectCount = (int) ::SendMessage(hListCtrl, LVM_GETSELECTEDCOUNT, 0, 0L);
                int iLastPos = -1;
                TCHAR szFileName[MAX_PATH];
                LVITEM lvi;
                memset(&lvi, 0, sizeof(LVITEM));
                lvi.cchTextMax = MAX_PATH;
                lvi.pszText = szFileName;
                TString strSelectFiles, strDisplayText;
                TCHAR szPath[MAX_PATH];
                ::SendMessage( hParent, CDM_GETFOLDERPATH, MAX_PATH, (LPARAM)szPath);
                strSelectFiles.append(szPath).append(_T("|"));
                for (size_t i = 0; i < uSelectCount; ++i)
                {
                    int iSelectItemPos = ::SendMessage(hListCtrl, LVM_GETNEXTITEM, iLastPos, MAKELPARAM(LVIS_SELECTED, 0));
                    ::SendMessage(hListCtrl, LVM_GETITEMTEXT, (WPARAM)iSelectItemPos, (LPARAM)&lvi);
                    strSelectFiles.append(lvi.pszText).append(_T("|"));
                    strDisplayText.append(_T("\"")).append(lvi.pszText).append(_T("\" "));
                    iLastPos = iSelectItemPos;
                }
                HWND hNameCombo = GetDlgItem(hParent, 0x47c);//cmb13
                HWND hEditCtrl = (HWND)::SendMessage(hNameCombo, CBEM_GETEDITCONTROL, 0, 0);
                ::SetWindowText(hEditCtrl, strDisplayText.c_str());
                _tcscpy(lpofn->lpOFN->lpstrFile, strSelectFiles.c_str());
            }
        } 
    }
    return NULL;
}

bool CUtilityManager::AcquireMuiltyFilePath(bool bAllowDirectory, HWND hwnd, std::vector<TString>& result, const TCHAR* Tiltle, const TCHAR* filter, const TCHAR* pszInitialPath)
{
    DWORD returnFlag = 0;
    size_t uCounter = 0;
    bool bSuccess = false;
    TCHAR* pszFileName = NULL;
    do 
    {
        pszFileName = new TCHAR[MAX_PATH * ++uCounter];
        OPENFILENAME dataStruct;
        memset(&dataStruct, 0, sizeof(dataStruct));
        memset(pszFileName, 0, MAX_PATH * uCounter);

        dataStruct.lStructSize = sizeof(dataStruct);
        dataStruct.hwndOwner = hwnd;
        dataStruct.lpstrFile = pszFileName;
        dataStruct.lpstrTitle = Tiltle;
        dataStruct.lpstrFilter = filter;
        dataStruct.nMaxFile = MAX_PATH;
        dataStruct.lpstrInitialDir = pszInitialPath;
        dataStruct.Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST;
        if (bAllowDirectory)
        {
            dataStruct.lpfnHook = FolderHookProc;
            dataStruct.Flags |= (OFN_PATHMUSTEXIST | OFN_ENABLEHOOK);
        }
        bSuccess = GetOpenFileName(&dataStruct) == TRUE;
        returnFlag = CommDlgExtendedError();
        bSuccess = bSuccess && returnFlag == 0;
        if (bSuccess)
        {
            std::vector<TString> files;
            CStringHelper::GetInstance()->SplitString(pszFileName, _T("|"), files, false);
            BEATS_ASSERT(files.size() > 0, _T("Invalid string for AcquireMuiltyFilePath : %s"), pszFileName);
            if (files.size() == 1) // Double click a file will cause this condition.
            {
                result = files;
            }
            else
            {
                result.clear();
                result.resize(files.size() - 1);
                for (size_t i = 1; i < files.size(); ++i)
                {
                    TString& strFileFullPath = result[i - 1];
                    strFileFullPath.append(files[0]).append(_T("/")).append(files[i]);
                }
            }
        }
        BEATS_SAFE_DELETE_ARRAY(pszFileName);
    } while (returnFlag == FNERR_BUFFERTOOSMALL);
    return bSuccess;
}

bool CUtilityManager::AcquireDirectory(HWND hwnd, TString& strDirectoryPath, const TCHAR* pszTitle)
{
    bool bRet = false;
    TCHAR szBuffer[MAX_PATH] = {0};
    BROWSEINFO bi;
    ZeroMemory(&bi,sizeof(BROWSEINFO));
    bi.hwndOwner = hwnd;
    bi.pszDisplayName = szBuffer;
    bi.lpszTitle = pszTitle;
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
    LPITEMIDLIST idl = SHBrowseForFolder(&bi);
    if (idl != NULL)
    {
        SHGetPathFromIDList(idl,szBuffer);
        strDirectoryPath = szBuffer;
        bRet = strDirectoryPath.length() > 0;
        strDirectoryPath.append(_T("/"));
    }
    return bRet;
}


void CUtilityManager::Init()
{
    return;    
}

bool CUtilityManager::FillDirectory(SDirectory& fileList, bool bFillSubDirectory/* = true*/, CFileFilter* pFileFilter/* = NULL*/)
{
    std::string tmp;
    TString& directoryPath = fileList.m_szPath;
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    BEATS_ASSERT(directoryPath.length() > 0);
    TCHAR lastCharacter = directoryPath.at(directoryPath.length() - 1);
    if( lastCharacter != _T('\\') &&
        lastCharacter != _T('/'))
    {
        directoryPath.append(_T("/"));
    }
    TString directoryPathMatch = directoryPath;
    directoryPathMatch.append(_T("*"));

    hFind = FindFirstFile(directoryPathMatch.c_str(), &FindFileData);
    BOOL bFindFile = hFind != INVALID_HANDLE_VALUE;
    while (bFindFile) 
    {
        if (pFileFilter == NULL || pFileFilter->FilterFile(&FindFileData))
        {
            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (_tcscmp(FindFileData.cFileName, _T(".")) != 0 &&
                    _tcscmp(FindFileData.cFileName, _T("..")) != 0)
                {
                    TString strSubDirectoryPath = directoryPath;
                    strSubDirectoryPath.append(FindFileData.cFileName).append(_T("/"));
                    SDirectory* pSubDir = new SDirectory(&fileList, strSubDirectoryPath.c_str());
                    pSubDir->SetData(FindFileData);
                    fileList.m_pDirectories->push_back(pSubDir);

                    if (bFillSubDirectory)
                    {
                        FillDirectory(*pSubDir, true, pFileFilter);
                    }
                }
            }
            else
            {
                WIN32_FIND_DATA* pFindData = new WIN32_FIND_DATA;
                memcpy(pFindData, &FindFileData, sizeof(WIN32_FIND_DATA));
                pFindData->dwReserved1 = (DWORD)(&fileList);
                fileList.m_pFileList->push_back(pFindData);

                long long uFileSize = FindFileData.nFileSizeHigh;
                uFileSize = uFileSize << 32;
            }
        }
        bFindFile = FindNextFile(hFind, &FindFileData);
        if (!bFindFile)
        {
            FindClose(hFind);
        }
    }
    return true;
}

unsigned long long CUtilityManager::BuildDirectoryToList( SDirectory* pDirectory, std::vector<WIN32_FIND_DATA*>& listToAppend )
{
    unsigned long long totalFileSize = 0;
    for (size_t i = 0; i < pDirectory->m_pFileList->size(); ++i)
    {
        listToAppend.push_back((*pDirectory->m_pFileList)[i]);
        long long uFileSize = (*pDirectory->m_pFileList)[i]->nFileSizeHigh;
        uFileSize = uFileSize << 32;
        uFileSize += (*pDirectory->m_pFileList)[i]->nFileSizeLow;
        totalFileSize += uFileSize;
    }
    for (size_t i = 0; i < pDirectory->m_pDirectories->size(); ++i)
    {
        totalFileSize += BuildDirectoryToList((*pDirectory->m_pDirectories)[i], listToAppend);
    }
    return totalFileSize;
}

void CUtilityManager::DeserializeDirectory(SDirectory* pDirectory, CSerializer& serializer, long long* pTotalDataSize /*= NULL*/, size_t* pFileCount /*= NULL*/)
{
    serializer >> pDirectory->m_szPath;
    size_t uFileListCount = 0;
    serializer >> uFileListCount;
    for (size_t i = 0; i < uFileListCount; ++i)
    {
        WIN32_FIND_DATA* pFileData = new WIN32_FIND_DATA;
        serializer >> pFileData->cAlternateFileName;
        serializer >> pFileData->cFileName;
        serializer >> pFileData->dwFileAttributes;
        serializer >> pFileData->ftCreationTime;
        serializer >> pFileData->ftLastAccessTime;
        serializer >> pFileData->ftLastWriteTime;
        serializer >> pFileData->nFileSizeHigh;
        serializer >> pFileData->nFileSizeLow;
        pFileData->dwReserved1 = (DWORD)pDirectory;
        pDirectory->m_pFileList->push_back(pFileData);
        if (pTotalDataSize != NULL)
        {
            long long fileSize = pFileData->nFileSizeHigh;
            fileSize = fileSize << 32;
            fileSize += pFileData->nFileSizeLow;
            *pTotalDataSize += fileSize;
        }
    }
    if (pFileCount != NULL)
    {
        *pFileCount += uFileListCount;
    }
    serializer.Deserialize(&pDirectory->m_data, sizeof(WIN32_FIND_DATA));
    size_t uDirectoriesCount = 0;
    serializer >> uDirectoriesCount;
    for (size_t i = 0; i < uDirectoriesCount; ++i)
    {
        SDirectory* pSubDirectory = new SDirectory(pDirectory, NULL);
        DeserializeDirectory(pSubDirectory, serializer, pTotalDataSize, pFileCount);
        pDirectory->m_pDirectories->push_back(pSubDirectory);
    }
}

void CUtilityManager::SerializeDirectory( SDirectory* pDirectory, CSerializer& serializer )
{
    serializer << pDirectory->m_szPath;
    serializer << pDirectory->m_pFileList->size();
    for (size_t i = 0; i < pDirectory->m_pFileList->size(); ++i)
    {
        WIN32_FIND_DATA* pFileData = (*pDirectory->m_pFileList)[i];
        serializer << pFileData->cAlternateFileName;
        serializer << pFileData->cFileName;
        serializer << pFileData->dwFileAttributes;
        serializer << pFileData->ftCreationTime;
        serializer << pFileData->ftLastAccessTime;
        serializer << pFileData->ftLastWriteTime;
        serializer << pFileData->nFileSizeHigh;
        serializer << pFileData->nFileSizeLow;
    }
    serializer.Serialize(&pDirectory->m_data, sizeof(WIN32_FIND_DATA));
    serializer << pDirectory->m_pDirectories->size();
    for (size_t i = 0; i < pDirectory->m_pDirectories->size(); ++i)
    {
        SerializeDirectory((*pDirectory->m_pDirectories)[i], serializer);
    }
}

bool CUtilityManager::WriteDataToFile(FILE* pFile, void* pData, size_t uDataLength, size_t uRetryCount/* = 20*/)
{
    BEATS_ASSERT(pFile != NULL, _T("File handle is invalid in CUtilityManager::WriteDataToFile"));
    BEATS_ASSERT(pData != NULL, _T("Data is NULL in CUtilityManager::WriteDataToFile"));
    BEATS_ASSERT(uDataLength > 0, _T("uDataLength is 0 in CUtilityManager::WriteDataToFile"));
    size_t uCounter = 0;
    while (uCounter < uDataLength && uRetryCount > 0)
    {
        DWORD uWriteCounter = 0;
        uWriteCounter = fwrite((unsigned char*)pData + uCounter, sizeof(unsigned char), uDataLength - uCounter, pFile);
        uCounter += uWriteCounter;
        --uRetryCount;
    }
    BEATS_ASSERT(uCounter == uDataLength, _T("Write Data Failed with retry count %d"), uRetryCount);
    return uCounter == uDataLength;
}

bool CUtilityManager::ReadDataFromFile(FILE* pFile, void* pData, size_t uDataLength, size_t uRetryCount/* = 20*/)
{
    BEATS_ASSERT(pFile != NULL, _T("File handle is invalid in CUtilityManager::WriteDataToFile"));
    BEATS_ASSERT(pData != NULL, _T("Data is NULL in CUtilityManager::WriteDataToFile"));
    BEATS_ASSERT(uDataLength > 0, _T("uDataLength is 0 in CUtilityManager::WriteDataToFile"));
    size_t uCounter = 0;
    while (uCounter < uDataLength && uRetryCount > 0)
    {
        DWORD uReadCounter = 0;
        uReadCounter = fread((unsigned char*)pData + uCounter, sizeof(unsigned char), uDataLength - uCounter, pFile);
        uCounter += uReadCounter;
        --uRetryCount;
    }
    BEATS_ASSERT(uCounter == uDataLength, _T("Write Data Failed with retry count %d"), uRetryCount);
    return uCounter == uDataLength;

}

bool CUtilityManager::GetProcessModule( size_t uProcessId, std::vector<TString>& modulePath )
{
    bool bRet = false;
    HMODULE moduleCache[1024];
    DWORD cacheNeeded = 0;
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, (DWORD)uProcessId);
    if (hProcess != NULL)
    {
        if(EnumProcessModules(hProcess, moduleCache, sizeof(moduleCache), &cacheNeeded))
        {
            UINT moduleCount = cacheNeeded / sizeof(HMODULE);
            TCHAR pathCache[MAX_PATH];
            for(UINT i = 0; i < moduleCount; ++i)
            {
                GetModuleFileName(moduleCache[i], pathCache, MAX_PATH);
                modulePath.push_back(pathCache);
            }
            bRet = true;
        }
        CloseHandle(hProcess);
    }
    return bRet;
}

bool CUtilityManager::CalcMD5( CMD5& md5, SDirectory& fileList )
{
    bool bRet = false;
    for (size_t i = 0; i < fileList.m_pFileList->size(); ++i)
    {
        WIN32_FIND_DATA* pFileData = fileList.m_pFileList->at(i);
        TString strFilePath = fileList.m_szPath;
        strFilePath.append(pFileData->cFileName);
        FILE* pFile = _tfopen(strFilePath.c_str(), _T("rb"));
        if (pFile != NULL)
        {
            md5.Update(pFile);
            fclose(pFile);
        }
    }
    for (size_t i = 0; i < fileList.m_pDirectories->size(); ++i)
    {
        CalcMD5(md5, *fileList.m_pDirectories->at(i));
    }
    return bRet;
}

void CUtilityManager::SetThreadName( DWORD dwThreadId, const char* pszThreadName )
{
#ifdef _DEBUG
    static const size_t MS_VC_EXCEPTION = 0x406d1388;
    struct
    {
        DWORD dwType;        // must be 0x1000
        LPCSTR szName;       // pointer to name (in same addr space)
        DWORD dwThreadID;    // thread ID (-1 caller thread)
        DWORD dwFlags;       // reserved for future use, most be zero
    } info;
    info.dwType = 0x1000;
    info.szName = pszThreadName;
    info.dwThreadID = dwThreadId;
    info.dwFlags = 0;
    __try
    {
        RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(DWORD), (DWORD *)&info);
    }
    _except (EXCEPTION_CONTINUE_EXECUTION)
    {
    }
#endif
}
