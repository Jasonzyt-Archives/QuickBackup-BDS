#pragma once
#include "json/forwards.h"
#include "json/writer.h"
#include "json/value.h"
#include "json/json.h"
#include "json/reader.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <queue>
#include <chrono>
#include <map>
#include <set>
#include <utility>
#include <urlmon.h>
#include <tchar.h>
#include <direct.h>
#include <direct.h>
#include <thread>

using namespace std;
using VA = unsigned __int64;
using RVA = unsigned int;

/*** 命名空间 ***/
namespace label
{
	Json::Value a;
	Json::Value b;
	Json::Value c;
	Json::Value d;
	Json::Value e;
	Json::Value f;
	Json::Value g;
	Json::Value h;
	Json::Value i;
}
namespace toggle
{
	Json::Value a;
	Json::Value b;
	Json::Value c;
}
/*** 声明值 ***/
string opp, opfn, _opfn, _cb, _lb;
static VA p_spscqueue, pxuid_level;
ofstream logfile;
bool v8, v9, v10, v11;
int cb, lb;
fstream jsonfile, formfile, perfile;
string BACKUPJSON, FORMJSON, PERJSON;

/*** 函数声明 ***/
bool fix();
void ReloadConfig();
bool RunBackup(string _operator);
void listBackupsC(string PlayerName);
void listBackupsS();

static bool runcmd(std::string cmd);
bool sendText(string playername, string text);
static unsigned getFormId();
bool destroyForm(unsigned fid);
unsigned sendForm(std::string uuid, std::string str);
static UINT sendCustomForm(char* uuid, char* json);

string getjson(string name);
void writeJsons(string path, string opfn, string Operator);
void createFromJson(size_t fsb, int sn, string opfn, string fs, string time, string minute, string path, string _operator, string md5);
string createOperatorJson(string playername, string xuid, string uuid);
bool checkBackupPermission(string playername, string xuid);
bool checkBackPermission(string playername, string xuid);
int getQuantity();
Json::Value getAllBackups();
string getListStr(Json::Value input);
void delJsons(int sn);