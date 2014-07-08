#include "stdafx.h"
#include "Utility/FilePath/FilePathTool.h"
#import <Foundation/Foundation.h>

bool CFilePathTool::Exists(const TCHAR* pszPath)
{
    BEATS_ASSERT(pszPath && pszPath[0]);
    return [[NSFileManager defaultManager] fileExistsAtPath:[NSString stringWithUTF8String:pszPath]];
}

bool CFilePathTool::IsAbsolute(const TCHAR* pszPath)
{
    BEATS_ASSERT(pszPath && pszPath[0]);
    return pszPath[0] == _T('/');
}

bool CFilePathTool::IsDirectory(const TCHAR* pszPath)
{
    BEATS_ASSERT(pszPath && pszPath[0]);
    BOOL bIsDir;
    BOOL bExists = [[NSFileManager defaultManager] fileExistsAtPath:[NSString stringWithUTF8String:pszPath] isDirectory:&bIsDir];
    return bExists && bIsDir;
}

TString CFilePathTool::ParentPath(const TCHAR* pszPath)
{
    BEATS_ASSERT(pszPath && pszPath[0]);
    TString strPath(pszPath);
    if(strPath[strPath.size()-1] == _T('/'))
    {
        strPath.resize(strPath.size() - 1);
    }
    int pos = strPath.rfind(_T('/'));
    if (pos != TString::npos)
    {
        strPath.resize(pos);
    }
    return strPath;
}

TString CFilePathTool::Extension(const TCHAR* pszPath)
{
    BEATS_ASSERT(pszPath && pszPath[0]);
    TString strPath(pszPath);
    int pos = strPath.rfind(_T('.'));
    if (pos != TString::npos)
    {
        strPath = strPath.substr(pos, strPath.length() - pos);
    }
    else
    {
        strPath.clear();
    }
    return strPath;
}

TString CFilePathTool::FileName(const TCHAR* pszPath)
{
    BEATS_ASSERT(pszPath && pszPath[0]);
    TString strPath(pszPath);
    int pos = strPath.rfind(_T('/'));
    if (pos != TString::npos)
    {
        strPath = strPath.substr(pos + 1, strPath.length() - pos);
    }
    return strPath;
}

TString CFilePathTool::MakeRelative(const TCHAR* pszFrom, const TCHAR* pszTo)
{
    // TODO: Unimplement
    BEATS_ASSERT(false, _T(__FUNCTION__)_T(" not implemented!"));
    return _T("");
}

TString CFilePathTool::MakeAbsolute(const TCHAR* pszStartPath, const TCHAR* pszRelativePath)
{
    // TODO: Unimplement
    BEATS_ASSERT(false, _T(__FUNCTION__)_T(" not implemented!"));
    return _T("");
}

TString CFilePathTool::RootPath( const TCHAR* pszPath )
{
    // TODO: Unimplement
    BEATS_ASSERT(false, _T(__FUNCTION__)_T(" not implemented!"));
    return _T("");
}

bool CFilePathTool::Canonical(TCHAR* pszOutBuffer, const TCHAR* pszOriginPath)
{
    // TODO: Unimplement
    BEATS_ASSERT(false, _T(__FUNCTION__)_T(" not implemented!"));
    return false;
}