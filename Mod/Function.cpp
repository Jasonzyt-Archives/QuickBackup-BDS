#include "预编译头.h"
#pragma comment(lib,"URlmon")
#pragma warning(disable:4996)

using namespace std;

string getTime()
{
    time_t timep;
    time(&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));
    return tmp;
}

string getCustomTime(const char *Format)
{
    time_t timep;
    time(&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), Format, localtime(&timep));
    return tmp;
}

string getMinute()
{
    time_t timep;
    time(&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d-%H-%M", localtime(&timep));
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
    v2 << "QuickBackup\\Bandzip\\bz.exe c -y -r -fmt:zip \"" << p1 << p3 << "\" " << "\"" << p2 << "\" ";
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

bool findFile(string filename)
{
    ifstream f(filename.c_str());
    return f.good();
}

bool mkdir(string dirname)
{
    ostringstream v10;
    v10 << "mkdir " << dirname;
    string v11 = v10.str();
    system(v11.c_str());
    return true;
}

bool DownloadFile(LPCWSTR url, LPCWSTR downloadPath)
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

int getFiles()
{
    bool v1 = false;
    bool v2 = false;
    bool v3 = false;
    bool v4 = false;
    bool v5 = false;
    bool v6 = false;
    if (findFile(".\\QuickBackup\\config.ini"))
    {
        int v7 = rename(".\\QuickBackup\\config.ini", ".\\QuickBackup\\config.old.ini");
        if (v7 == 0)
            v1 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/config.ini", L".\\QuickBackup\\config.ini");
        else
            PR(2, u8"重命名文件失败"); v1 = false;
    }
    else
    {
        v1 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/config.ini", L".\\QuickBackup\\config.ini");
        v2 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/bz.exe", L".\\QuickBackup\\Bandzip\\bz.exe");
        v3 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x64.dll", L".\\QuickBackup\\Bandzip\\ark.x64.dll");
        v4 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x64.lgpl.dll", L".\\QuickBackup\\Bandzip\\ark.x64.lgpl.dll");
        v5 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x86.dll", L".\\QuickBackup\\Bandzip\\ark.x86.dll");
        v6 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x86.lgpl.dll", L".\\QuickBackup\\Bandzip\\ark.x86.lgpl.dll");
    }
    if (v1 && v2 && v3 && v4 && v5 && v6)
        return 0;
    else if (v1 == false && v2 == false && v3 == false && v4 == false && v5 == false && v6 == false)
        return -1;
    else
        return 1;
}

size_t GetFileSizeByte(const std::string& file_name) {
    struct _stat info;
    _stat(file_name.c_str(), &info);
    size_t size = info.st_size;
    return size; //单位是：byte
}

string GetFileSize(const std::string& file_name) {
    struct _stat info;
    _stat(file_name.c_str(), &info);
    size_t size = info.st_size;
    if (size >= 1024 && size < 1048576)
    {
        ostringstream a;
        a << size / 1024 << "KB";
        return a.str();
    }
    else if (size >= 1048576 && size < 1073741824)
    {
        ostringstream a;
        a << size / 1048576 << "MB";
        return a.str();
    }
    else if (size >=1073741824  && size < 1099511627776)
    {
        ostringstream a;
        a << size / 1073741824 << "GB";
        return a.str();
    }
    else
    {
        ostringstream a;
        a << size << "Byte";
        return a.str();
    }
}