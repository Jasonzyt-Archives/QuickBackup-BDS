/* FILENAME = Function.h */
#pragma once
#include <iostream>
#include <string>
#include <time.h>
#include <sstream>
#include <stdio.h>
#include "getConfig.h"
#pragma warning(disable:4996)

using namespace std;

//Variable Declaration
//Function Declaration
string getTime();
string editZIPFilename(string outputfilename);
int PR(int PRtype, string text);
string getCmdStr(string p1, string p2, string p3);
string getConfig(string configfile, string key, string defaultvaule);
int StringToInt(string str);
bool findFile(string filename);
int PRErr(int ErrorType, string ErrorCode, string text);
bool mkdir(string dirname);
bool DownloadFile(const wchar_t* url, const wchar_t* downloadPath);
bool FolderExists(const wchar_t* path);
