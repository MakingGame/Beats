// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� SPY_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// BEATS_SPY_DLL_DECL ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�


#ifdef __cplusplus
extern "C" {
#endif

#ifdef _USRDLL
#ifdef SPY_EXPORTS
#define BEATS_SPY_DLL_DECL __declspec(dllexport)
#else
#define BEATS_SPY_DLL_DECL __declspec(dllimport)
#endif
#else
#define BEATS_SPY_DLL_DECL
#endif

struct SBeatsSocket;
class CFileFilter;
BEATS_SPY_DLL_DECL bool Init(void);
BEATS_SPY_DLL_DECL bool Uninit(void);
BEATS_SPY_DLL_DECL const hostent* Spy_GetHostInfo();
BEATS_SPY_DLL_DECL const SBeatsSocket* Spy_Connect(uint32_t uIPAddress);
BEATS_SPY_DLL_DECL bool Spy_GetNetAdapterInfo(std::string& strInfo);


typedef void(*TCommandFeedBackFunc)(const char* pszFeedBackInfo, void* pUserDataPtr);
BEATS_SPY_DLL_DECL bool Spy_SendSystemCommand(SOCKET sock, const char* pszCmd, TCommandFeedBackFunc pFeedbackFunc, void* pUserDataPtr);

typedef void(*TFileInfoFeedbackFunc)(char m_type, void* pFileInfo, void* pUserDataPtr);
BEATS_SPY_DLL_DECL bool Spy_SendFileInfoCommand(SOCKET sock, const TCHAR* pszCmd, TFileInfoFeedbackFunc pFeedbackFunc, void* pUserDataPtr);
BEATS_SPY_DLL_DECL bool Spy_SendMessageCommand(SOCKET sock, const TCHAR* pszMsg, const TCHAR* pszCapital);

BEATS_SPY_DLL_DECL bool Spy_DownloadFiles(SOCKET sock, const std::vector<TString>& files, const TString& strStorePath, CFileFilter* pFilter);
BEATS_SPY_DLL_DECL bool Spy_UploadFiles(SOCKET sock, const std::vector<TString>& files, const TString& strStorePath, CFileFilter* pFilter);

#ifdef __cplusplus
}
#endif
