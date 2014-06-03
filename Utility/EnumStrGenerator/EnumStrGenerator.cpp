#include "stdafx.h"
#include "EnumStrGenerator.h"
#include "Serializer/Serializer.h"
#include "StringHelper/StringHelper.h"
#include "UtilityManager.h"

static const char* ENUM_KEYWORD_STR = "enum";
static const TCHAR* SCAN_DATA_NAME = _T("ScanData.bin");

CEnumStrGenerator* CEnumStrGenerator::m_pInstance = NULL;

CEnumStrGenerator::CEnumStrGenerator()
: m_bInitFlag(false)
{
}

CEnumStrGenerator::~CEnumStrGenerator()
{
    typedef std::map<TString, SEnumScanData*> TEnumMap;
    BEATS_SAFE_DELETE_MAP(m_enumStrPool, TEnumMap);
}

bool CEnumStrGenerator::ScanEnumInFile( const TCHAR* pFileName )
{
    bool bRet = false;
    CSerializer serializer(pFileName);
    while (serializer.GetReadPos() != serializer.GetWritePos())
    {
        if (ScanKeyWordInCPlusPlusFile(ENUM_KEYWORD_STR, &serializer))
        {
            bool bIsKeyWord = IsEnumKeyword(&serializer);
            size_t startPos = serializer.GetReadPos();
            startPos += strlen(ENUM_KEYWORD_STR);
            serializer.SetReadPos(startPos);
            if (bIsKeyWord && ScanKeyWordInCPlusPlusFile("{", &serializer))
            {
                size_t endPos = serializer.GetReadPos();
                size_t enumNameLength = endPos - startPos;
                char* enumName = new char[enumNameLength + 1];
                enumName[enumNameLength] = 0;
                serializer.SetReadPos(startPos);
                serializer.Deserialize(enumName, enumNameLength);
                char* enumNewName = new char[enumNameLength + 1];
                enumNewName[enumNameLength] = 0;
                FilterCPlusPlusFileComments(enumName, enumNewName, enumNameLength);
                BEATS_ASSERT(serializer.GetReadPos() == endPos);
                TCHAR* tNameCharBuff = new TCHAR[enumNameLength + 1];
                tNameCharBuff[enumNameLength] = 0;
                CStringHelper::GetInstance()->ConvertToTCHAR(enumNewName, tNameCharBuff, enumNameLength + 1);
                std::vector<TString> filters;
                filters.push_back(_T("\r\n"));
                filters.push_back(_T(" "));
                TString strName = CStringHelper::GetInstance()->FilterString(tNameCharBuff, filters);
                std::map<TString, SEnumScanData*>::iterator iter = m_enumStrPool.find(strName.c_str());
                bool bExisting = iter == m_enumStrPool.end();
                BEATS_WARNING(bExisting, 
                    _T("The enum type %s in %s is already scanned in %s, it may be you have scaned same file twice or a enum type with same name under different name space."), 
                    strName.c_str(), 
                    pFileName,
                    iter->second->m_enumFilePath.c_str());
                // If the length is 0, it means we meet no name enum declare or a typedef.
                if (strName.length() != 0 && bExisting)
                {
                    startPos = endPos + 1; // Escape "{"
                    ScanKeyWordInCPlusPlusFile("}", &serializer);
                    endPos = serializer.GetReadPos();
                    serializer.SetReadPos(startPos);
                    size_t dataLength = endPos - startPos;
                    char* textBuff = new char[dataLength + 1];
                    textBuff[dataLength] = 0;
                    serializer.Deserialize(textBuff, dataLength);
                    BEATS_ASSERT(serializer.GetReadPos() == endPos);
                    char* newTextBuff = new char[dataLength + 1];
                    newTextBuff[dataLength] = 0;
                    size_t length = 0;
                    FilterCPlusPlusFileComments(textBuff, newTextBuff, length);
                    TCHAR* tcharBuff = new TCHAR[length + 1];
                    tcharBuff[length] = 0;
                    CStringHelper::GetInstance()->ConvertToTCHAR(newTextBuff, tcharBuff, length + 1);
                    TString strNewText = CStringHelper::GetInstance()->FilterString(tcharBuff, filters);
                    SEnumScanData* pEnumData = new SEnumScanData;
                    std::vector<TString> rawEnumString;
                    CStringHelper::GetInstance()->SplitString(strNewText.c_str(), _T(","), rawEnumString);
                    int iDefaultEnumValue = 0;
                    for (size_t i = 0; i < rawEnumString.size(); ++i)
                    {
                        SEnumData* pData = AnalyseRawEnumString(rawEnumString[i], iDefaultEnumValue);
                        pEnumData->m_enumValue.push_back(pData);
                    }
                    pEnumData->m_enumFilePath.assign(pFileName);
                    m_enumStrPool[strName] = pEnumData;

                    BEATS_SAFE_DELETE_ARRAY(textBuff);
                    BEATS_SAFE_DELETE_ARRAY(newTextBuff);
                    BEATS_SAFE_DELETE_ARRAY(tcharBuff);
                    BEATS_SAFE_DELETE_ARRAY(enumName);
                    BEATS_SAFE_DELETE_ARRAY(enumNewName);
                    BEATS_SAFE_DELETE_ARRAY(tNameCharBuff);
                    bRet = true;
                }
            }
        }
    }
    return bRet;
}

bool CEnumStrGenerator::ScanKeyWordInCPlusPlusFile( const char* pKeyWord, CSerializer* fileSerializer)
{
    BEATS_ASSERT(strstr(pKeyWord, "//") == NULL);
    BEATS_ASSERT(strstr(pKeyWord, "/*") == NULL);

    std::vector<SBufferData> keyWords;
    SBufferData data;
    data.pData = "//";
    data.dataLength = strlen((char*)data.pData) * sizeof(char);
    keyWords.push_back(data);
    data.pData = "/*";
    data.dataLength = strlen((char*)data.pData) * sizeof(char);
    keyWords.push_back(data);

    SBufferData quoteSign;
    quoteSign.pData = "\"";
    quoteSign.dataLength = strlen((char*)quoteSign.pData) * sizeof(char);
    keyWords.push_back(quoteSign);

    data.pData = (void*)pKeyWord;
    data.dataLength = strlen((char*)data.pData) * sizeof(char);
    keyWords.push_back(data);

    SBufferData startComment;
    startComment.pData = "*/";
    startComment.dataLength = strlen((char*)startComment.pData) * sizeof(char);

    SBufferData changeLine;
    changeLine.pData = "\r\n";
    changeLine.dataLength = strlen((char*)changeLine.pData) * sizeof(char);

    int wordId = 0;
    while (fileSerializer->ReadToDataInList(keyWords, false, &wordId) != fileSerializer->GetWritePos())
    {
        if (wordId == 0) // Read "//"
        {
            fileSerializer->SetReadPos(fileSerializer->GetReadPos() + keyWords[wordId].dataLength);
            bool bReadToEnd = fileSerializer->ReadToData(changeLine) == fileSerializer->GetWritePos();//Read to end of line "/r/b"
            if (!bReadToEnd)
            {
                fileSerializer->SetReadPos(fileSerializer->GetReadPos() + changeLine.dataLength);
            }
        }
        else if (wordId == 1) // Read "/*"
        {
            fileSerializer->SetReadPos(fileSerializer->GetReadPos() + keyWords[wordId].dataLength);
            bool bReadToEnd = fileSerializer->ReadToData(startComment) == fileSerializer->GetWritePos();//Read to end of comment "*/"
            if (!bReadToEnd)
            {
                fileSerializer->SetReadPos(fileSerializer->GetReadPos() + startComment.dataLength);
            }
        }
        else if (wordId == 2)// Read " " "
        {
            fileSerializer->SetReadPos(fileSerializer->GetReadPos() + keyWords[wordId].dataLength);
            bool bReadToEnd = fileSerializer->ReadToData(quoteSign) == fileSerializer->GetWritePos();//Read to end of comment "*/"
            if (!bReadToEnd)
            {
                fileSerializer->SetReadPos(fileSerializer->GetReadPos() + quoteSign.dataLength);
            }
        }
        else if (wordId == 3) // Read the keyword
        {
            break;
        }
    }
    return wordId == 3;
}

bool CEnumStrGenerator::FilterCPlusPlusFileComments( const char* text, char* outBuffer, size_t& bufferLenth )
{
    bufferLenth = 0;
    const char* pReader = text;
    char* pWriter = outBuffer;
    while (*pReader != 0)
    {
        if (memcmp(pReader, "//", 2) == 0)
        {
            while (memcmp(pReader, "\r\n", 2) != 0)
            {
                ++pReader;
            }
            pReader += 2;
        }
        else if (memcmp(pReader, "/*", 2) == 0)
        {
            while (memcmp(pReader, "*/", 2) != 0)
            {
                ++pReader;
            }
            pReader += 2;
        }
        else
        {
            *(pWriter++) = *(pReader++);
            ++bufferLenth;
        }
    }
    *pWriter = 0;
    return true;
}

bool CEnumStrGenerator::GetEnumValueData( const TCHAR* pEnumType, const std::vector<SEnumData*>*& pResult)
{
    BEATS_ASSERT(m_bInitFlag, _T("Call CEnumStrGenerator::Init before get enum value!"));
    std::map<TString, SEnumScanData*>::iterator iter = m_enumStrPool.find(pEnumType);
    bool bRet = false;
    if ( iter != m_enumStrPool.end())
    {
        pResult = &iter->second->m_enumValue;
        bRet = true;
    }
    return bRet;
}

bool CEnumStrGenerator::ScanEnumInDirectory( const TCHAR* pDirectory )
{
    bool bRet = false;
#if (BEATS_PLATFORM == BEATS_PLATFORM_WIN32)
    const TCHAR* pFileStr = _T("*.*");
    TString wildCmpStr(pDirectory);
    size_t pathSize = _tcslen(pDirectory);
    if (pDirectory[pathSize - 1] != '/')
    {
        wildCmpStr.append(_T("/"));
    }
    wildCmpStr.append(pFileStr);
    TFileData fileData;
    HANDLE searchHandle = ::FindFirstFile(wildCmpStr.c_str(), &fileData);
    if (searchHandle != INVALID_HANDLE_VALUE)
    {
        ScanEnum(fileData, pDirectory);
        while (::FindNextFile(searchHandle, &fileData))
        {
            ScanEnum(fileData, pDirectory);
        }
        FindClose(searchHandle);
        bRet = true;
    }

    BEATS_ASSERT(GetLastError() == ERROR_NO_MORE_FILES, _T("Scan Enum failed in path %s"), pDirectory);
#endif
    return bRet;
}

bool CEnumStrGenerator::ScanEnum( const TFileData& fileData, const TString& fullDirectoryPath )
{
#if (BEATS_PLATFORM == BEATS_PLATFORM_WIN32)
    if ((fileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == 0)
    {
        if ((fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
        {
            const TCHAR* pReader = &fileData.cFileName[_tcslen(fileData.cFileName) - 4];
            if (memcmp(pReader, _T(".cpp"), 4 * sizeof(TCHAR)) == 0 ||
                memcmp(pReader + 2, _T(".h"), 2 * sizeof(TCHAR)) == 0)
            {
                TString fileFullPath = fullDirectoryPath;
                fileFullPath.append(_T("/")).append(fileData.cFileName);
                ScanEnumInFile(fileFullPath.c_str());
            }
        }
        else
        {
            if (_tcsicmp(fileData.cFileName, _T(".")) != 0 &&
                _tcsicmp(fileData.cFileName, _T("..")) != 0 )
            {
                TString fileFullPath = fullDirectoryPath;
                fileFullPath.append(_T("/")).append(fileData.cFileName);
                ScanEnumInDirectory(fileFullPath.c_str());
            }
        }
    }
#endif
    return true;
}

bool CEnumStrGenerator::IsEnumKeyword( CSerializer* pSerailizer )
{
    bool bRet = true;
    //Check front.
    if (pSerailizer->GetReadPos() > 1)
    {
        unsigned char byteData = pSerailizer->GetBuffer()[pSerailizer->GetReadPos() - 1];
        if (byteData != '\n' && byteData != ' ' && byteData != '\t')
        {
            bRet = false;
        }
    }
    if (bRet)
    {
        //Check after, there must be some characters after enum.
        unsigned char byteData = pSerailizer->GetBuffer()[pSerailizer->GetReadPos() + strlen(ENUM_KEYWORD_STR)];
        if (byteData != '\n' && byteData != ' ' && byteData != '\t' && byteData != '{')
        {
            bRet = false;
        }
    }
    return bRet;
}

void CEnumStrGenerator::Init(const std::vector<TString>& scanPathList)
{
    if (!LoadCacheFile())
    {
        for(size_t i = 0; i < scanPathList.size(); ++i)
        {
            ScanEnumInDirectory(scanPathList[i].c_str());
        }
        SaveCacheFile();
    }
    m_bInitFlag = true;
}

void CEnumStrGenerator::SaveCacheFile()
{
    CSerializer serializer;
    serializer << m_enumStrPool.size();
    std::map<TString, SEnumScanData*>::iterator iter = m_enumStrPool.begin();
    for (; iter != m_enumStrPool.end(); ++iter)
    {
        serializer << iter->first;
        serializer << iter->second->m_enumFilePath;
        serializer << iter->second->m_enumValue.size();
        for (size_t i = 0; i < iter->second->m_enumValue.size(); ++i)
        {
            serializer << iter->second->m_enumValue[i]->m_str;
            serializer << iter->second->m_enumValue[i]->m_value;
        }
    }
    TString fullPathStr = CUtilityManager::GetInstance()->FileRemoveName(CUtilityManager::GetInstance()->GetModuleFileName().c_str());
    fullPathStr.append(_T("/")).append(SCAN_DATA_NAME);
    serializer.Deserialize(fullPathStr.c_str());
}

bool CEnumStrGenerator::LoadCacheFile()
{
    bool bRet = false;
    TString fullPathStr = CUtilityManager::GetInstance()->FileRemoveName(CUtilityManager::GetInstance()->GetModuleFileName().c_str());
    fullPathStr.append(_T("/")).append(SCAN_DATA_NAME);
    if (CUtilityManager::GetInstance()->FileExists(fullPathStr.c_str()))
    {
        CSerializer serializer(fullPathStr.c_str());
        size_t enumTypeCount = 0;
        serializer >> enumTypeCount;
        for(size_t i = 0; i < enumTypeCount; ++i)
        {
            TString enumTypeStr;
            serializer >> enumTypeStr;
            SEnumScanData* pEnumScanData = new SEnumScanData;
            serializer >> pEnumScanData->m_enumFilePath;
            size_t dataStrCount = 0;
            serializer >> dataStrCount;
            for (size_t j = 0; j < dataStrCount; ++j)
            {
                SEnumData* pEnumData = new SEnumData();
                serializer >> pEnumData->m_str;
                serializer >> pEnumData->m_value;
                pEnumScanData->m_enumValue.push_back(pEnumData);
            }
            BEATS_ASSERT(m_enumStrPool.find(enumTypeStr) == m_enumStrPool.end());
            m_enumStrPool[enumTypeStr] = pEnumScanData;
        }
        bRet = true;
    }
    return bRet;
}

SEnumData* CEnumStrGenerator::AnalyseRawEnumString( const TString& rawEnumStr, int& curValue )
{
    SEnumData* pEnumData = new SEnumData;
    std::vector<TString> result;
    CStringHelper::GetInstance()->SplitString(rawEnumStr.c_str(), _T("="), result, true);
    BEATS_ASSERT(result.size() == 2 || result.size() == 1);
    if (result.size() == 2)
    {
        pEnumData->m_str = result[0].c_str();
        const TCHAR* pStrData = result[1].c_str(); 
        TCHAR* pStopPos = NULL;
        bool bIsHex = pStrData[0] == '0' && (pStrData[1] == 'x' || pStrData[1] == 'X');
        pEnumData->m_value = _tcstoul(pStrData, &pStopPos, bIsHex ? 16 : 10);
        curValue = pEnumData->m_value + 1;
    }
    else
    {
        pEnumData->m_str = rawEnumStr;
        pEnumData->m_value = curValue++;
    }
    return pEnumData;
}
