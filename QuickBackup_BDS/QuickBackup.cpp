#include "预编译头.h"
#pragma comment(lib, "urlmon.lib")

using namespace std;

/*** 声明值 ***/
string opp, opfn;

/*** 定义值 ***/
//备份TargetPath
string tp = "world\\";

/*** MAP ***/
static std::map<std::string, Player*> onlinePlayersList;

/*** 结构体 ***/
//注册命令 (land-g7)
struct CommandRegistry
{
	void registerCommand(string const& a, char const* b, char c, char d, char e)
	{
		SYMCALL(void, MSSYM_MD5_8574de98358ff66b5a913417f44dd706, this, a, b, c, d, e);
	}
};
static VA p_spscqueue;
//执行后台指令
static bool runcmd(std::string cmd) 
{
	if (p_spscqueue != 0)
		return SYMCALL(bool, MSSYM_MD5_b5c9e566146b3136e6fb37f0c080d91e, p_spscqueue, cmd);
	return false;
}
// 取命令文本
struct CommandRequestPacket 
{
	std::string toString() 
	{
		std::string str = std::string(*(std::string*)((VA)this + 40));
		return str;
	}
};

/*** 函  数 ***/
//向游戏内玩家发送文本
bool sendText(string playername, string text) 
{
	runcmd("tellraw " + playername + " {\"rawtext\":[{\"text\":\"" + text + "\"}]}");
}

//插件载入->预编译头.cpp
void init() 
{
	PR(0, u8"QuickBackup Loaded");
	bool v1 = findFile("QuickBackup/config.ini");
	bool v6 = FolderExists(L"QuickBackup");
	if (v1 == false && v6 == false)
	{
		PR(0, "初次启动插件,开始创建相关文件(夹),如果创建失败请检查用户权限,建议使用管理员启动BDS");
		mkdir("QuickBackup");
		bool v2 = FolderExists(L"QuickBackup");
		if (v2 == false)
		{
			PR(2, "无法创建文件夹");
			PR(2, "无法创建文件夹");
			PR(2, "无法创建文件夹");
		}
		bool v3 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/config.ini", L".\\QuickBackup\\config.ini");
		if (v3)
		{
			PR(0, "config.ini下载成功 请前往QuickBackup文件夹修改");
		}
		else
		{
			PR(2, "config.ini下载失败 正在重试");
			bool v4 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/config.ini", L".\\QuickBackup\\config.ini");
			if (v4)
			{
				PR(0, "config.ini下载成功 请前往QuickBackup文件夹修改");
			}
			else
			{
				PR(2, "config.ini下载失败 正在重试");
				bool v5 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/config.ini", L".\\QuickBackup\\config.ini");
				if (v5)
				{
					PR(0, "config.ini下载成功 请前往QuickBackup文件夹修改");
				}
				else
				{
					PR(2, "config.ini下载失败 请检查网络连接或手动从GitHub下载");
					PR(2, "正在以默认配置启动...");
				}
			}
		}
	}
	else if (v1 == false && v6 == false)
	{
		PR(2, "找不到配置文件,开始试图获取相关文件");
		bool v3 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/config.ini", L".\\QuickBackup\\config.ini");
		if (v3)
		{
			PR(0, "config.ini下载成功 请前往QuickBackup文件夹修改");
		}
		else
		{
			PR(2, "config.ini下载失败 正在重试");
			bool v4 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/config.ini", L".\\QuickBackup\\config.ini");
			if (v4)
			{
				PR(0, "config.ini下载成功 请前往QuickBackup文件夹修改");
			}
			else
			{
				PR(2, "config.ini下载失败 正在重试");
				bool v5 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/config.ini", L".\\QuickBackup\\config.ini");
				if (v5)
				{
					PR(0, "config.ini下载成功 请前往QuickBackup文件夹修改");
				}
				else
				{
					PR(2, "config.ini下载失败 请检查网络连接或手动从GitHub下载");
					PR(2, "正在以默认配置启动...");
				}
			}
		}
	}
	opp = getConfig("QuickBackup\\config.ini", "OutputPath", "backup\\");
	opfn = getConfig("QuickBackup\\config.ini", "OutputFilname", "%Y-%m-%d-%H.zip");
	PR(0, "插件已启动(Version 1.0.5)(GitHub Repository:https://www.github.com/Jasonzyt/QuickBackup-BDS)");
}
//插件退出->预编译头.cpp
void exit()
{
	PR(0, "插件已退出");
}

//配置读取
bool ReloadConfig()
{
	opp = getConfig("QuickBackup\\config.ini", "OutputPath", "backup\\");
	opfn = getConfig("QuickBackup\\config.ini", "OutputFilname", "%Y-%m-%d-%H.zip");
	return true;
}
//运行备份
string RunBackup() 
{
	string _opfn = editZIPFilename(opfn);
	PR(0, "Starting Backup. 开始备份");
	PR(0, "Starting Compress. 开始压缩");
	system((getCmdStr(opp, tp, _opfn)).c_str());
	PR(0, "Backup Successful. 备份成功");
	return "Successful";
}
/*** THook ***/
//假命令注册->Line 7
THook(void, MSSYM_B1QA5setupB1AE20ChangeSettingCommandB2AAE22SAXAEAVCommandRegistryB3AAAA1Z, CommandRegistry* _this) {
	_this->registerCommand("qb_reload", u8"重载QuickBackup插件", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_backup", u8"执行一次备份", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_auto_on", u8"开启自动备份", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_auto_off", u8"关闭自动备份", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_back", u8"打开回档GUI", 0, { 0 }, { 0x40 });
	original(_this);
}
//命令处理
THook(void, MSSYM_B1QA6handleB1AE20ServerNetworkHandlerB2AAE26UEAAXAEBVNetworkIdentifierB2AAE24AEBVCommandRequestPacketB3AAAA1Z,
	VA _this, VA id, CommandRequestPacket* crp) {
	Player* pPlayer = SYMCALL(Player*, MSSYM_B2QUE15getServerPlayerB1AE20ServerNetworkHandlerB2AAE20AEAAPEAVServerPlayerB2AAE21AEBVNetworkIdentifierB2AAA1EB1AA1Z,_this, id, *((char*)crp + 16));
	string uid = pPlayer->getXuid(pxuid_level);
	auto cmd = crp->toString();
	auto dimensionId = pPlayer->getDimensionId();
	if (cmd == "/qb_backup") {
		if (dimensionId == 0) {
			gui_helpfid[uid] = landhelp_gui(uid);
		}
		return;
	}
	original(_this, id, crp);
}