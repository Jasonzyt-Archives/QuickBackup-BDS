/* FILENAME = Function.h */
#pragma once
#include"‘§±‡“ÎÕ∑.h"
#pragma warning(disable:4996)

using namespace std;

//Variable Declaration
//Function Declaration
string getTime();
string getMinute();
string editZIPFilename(string outputfilename);
int PR(int PRtype, string text);
string getCmdStr(string p1, string p2, string p3);
string getConfig(string configfile, string key, string defaultvaule);
int StringToInt(string str);
bool findFile(string filename);
int PRErr(int ErrorType, string ErrorCode, string text);
bool mkdir(string dirname);
bool DownloadFile(LPCWSTR url, LPCWSTR downloadPath);
bool FolderExists(const wchar_t* path);
int getFiles();
size_t GetFileSizeByte(const std::string& file_name);
string GetFileSize(const std::string& file_name);
