#include "预编译头.h"
#pragma comment(lib,"URlmon")
#pragma warning(disable:4996)

using namespace std;

string getTime()//
{
    time_t timep;
    time(&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));
    return tmp;
}

string editZIPFilename(string p5)//
{
    time_t timepp;
    time(&timepp);
    char tmp[64];
    strftime(tmp, sizeof(tmp), p5.c_str(), localtime(&timepp));
    return tmp;
}

int PR(int PRtype, string text)//
{
    ostringstream v1;
    string time = getTime();
    if (PRtype == 0)
    {
        v1 << "[" << time << " INFO][QuickBackup] " << text;
        cout << v1.str() << endl;
    }
    else if (PRtype == 1)
    {
        v1 << "[" << time << " WARN][QuickBackup] " << text;
        cout << v1.str() << endl;
    }
    else if (PRtype == 2)
    {
        v1 << "[" << time << " ERROR][QuickBackup] " << text;
        cout << v1.str() << endl;
    }
    else if (PRtype == 3)
    {
        v1 << "[" << time << " DEBUG][QuickBackup] " << text;
        cout << v1.str() << endl;
    }
    return 0;
}

int PRErr(int ErrorType, string ErrorCode, string text)
{
    ostringstream v9;
    string time = getTime();
    if (ErrorType == 0)
    {
        v9 << "[" << time << " ERROR][FATAL][" << ErrorCode << "] " << text;
        cout << v9.str() << endl;
        return 1;
    }
    return 0;
}

string getCmdStr(string p1/*OPP*/, string p2/*TP*/, string p3/*OPF*/)//
{
    ostringstream v2;
    /*拼合字符串 根据需求修改此部分为7-Zip.exe或bandzip.exe
    *Bandzip
        v2 << "[Bandzip目录] c -y -r -fmt:zip \"" << p1 << p3 << "\" " << "\"" << p2 << "\" ";
    *7-Zip
        v2 << "[7-Zip目录] a \"" << p1 << p3 << "\" " << "\"" << p2 << "\" -y -r";
    */

    /*The splicing string is modified to 7-Zip.exe or bandzip.exe
    *Bandzip
        v2 << "[Bandzip Folder] c -y -r -fmt:zip \"" << p1 << p3 << "\" " << "\"" << p2 << "\" ";
    *7-Zip
        v2 << "[7-Zip Folder] a \"" << p1 << p3 << "\" " << "\"" << p2 << "\" -y -r";
    */
    v2 << "bz.exe c -y -r -fmt:zip \"" << p1 << p3 << "\" " << "\"" << p2 << "\" ";
    string v3 = v2.str();
    return v3;
}

string getConfig(string configfile, string key, string defaultvaule)//
{
    Config conf(configfile);
    string r1 = conf.Read(key, defaultvaule/*默认值*/);
    return r1;
}

int StringToInt(string str)
{
    int r2 = atoi(str.c_str());
    return r2;
}

bool findFile(string filename)//
{
    ifstream f(filename.c_str());
    return f.good();
}

bool mkdir(string dirname)//
{
    ostringstream v10;
    v10 << "mkdir " << dirname;
    string v11 = v10.str();
    system(v11.c_str());
    return true;
}

bool DownloadFile(LPCWSTR url, LPCWSTR downloadPath)//
{
    HRESULT Result = URLDownloadToFile(NULL, url, downloadPath , NULL, NULL);
    switch (Result)
    {
    case S_OK:return true; break;
    default:return false;
    }
}

bool FolderExists(const wchar_t* path)//
{
    DWORD dwAttribute = GetFileAttributes(path);
    if (dwAttribute == 0XFFFFFFFF) return false; 
    else return true;
}

VOID SafeGetNativeSystemInfo(__out LPSYSTEM_INFO lpSystemInfo)//
{
    if (NULL == lpSystemInfo)	return;
    typedef VOID(WINAPI* LPFN_GetNativeSystemInfo)(LPSYSTEM_INFO lpSystemInfo);
    LPFN_GetNativeSystemInfo fnGetNativeSystemInfo = (LPFN_GetNativeSystemInfo)GetProcAddress(GetModuleHandle(_T("kernel32")), "GetNativeSystemInfo");;
    if (NULL != fnGetNativeSystemInfo)
    {
        fnGetNativeSystemInfo(lpSystemInfo);
    }
    else
    {
        GetSystemInfo(lpSystemInfo);
    }
}

int GetSystemBits()//
{
    SYSTEM_INFO si;
    SafeGetNativeSystemInfo(&si);
    if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 || si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
    {
        return 64;
    }
    return 32;
}

int getFiles()
{
    bool v1 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/config.ini", L".\\QuickBackup\\config.ini");
    bool v2 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/bz.exe", L".\\QuickBackup\\Bandzip\\bz.exe");
    bool v3 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x64.dll", L".\\QuickBackup\\Bandzip\\ark.x64.dll");
    bool v4 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x64.lgpl.dll", L".\\QuickBackup\\Bandzip\\ark.x64.lgpl.dll");
    bool v5 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x86.dll", L".\\QuickBackup\\Bandzip\\ark.x86.dll");
    bool v6 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x86.lgpl.dll", L".\\QuickBackup\\Bandzip\\ark.x86.lgpl.dll");
    if (v1 && v2 && v3 && v4 && v5 && v6)
        return 0;
    else if (v1 == false && v2 == false && v3 == false && v4 == false && v5 == false && v6 == false)
        return -1;
    else
        return 1;
}
