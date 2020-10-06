#include "预编译头.h"
#pragma comment(lib,"URlmon")
#pragma warning(disable:6031)
#pragma warning(disable:4996)

using namespace std;

/*** 声明值 ***/
string opp, opfn;
static VA p_spscqueue;
ofstream logfile;
bool v8, v9, v10, v11;

/*** 定义值 ***/
//备份TargetPath
string tp = ".\\world\\";

/*** 结构体 ***/
// 取命令文本
struct CommandRequestPacket {
	std::string toString() {
		std::string str = std::string(*(std::string*)((VA)this + 40));
		return str;
	}
};
//取表单ID及表单选项
struct ModalFormResponsePacket {
	UINT getFormId() {
		return *(UINT*)((VA)this + 40);
	}
	std::string getSelectStr() {
		std::string x = *(std::string*)((VA)this + 48);
		auto l = x.length();
		if (x.c_str()[l - 1] == '\n') {
			return l > 1 ? x.substr(0, l - 1) :
				x;
		}
		return x;
	}
};
//注册命令 (land-g7)
struct CommandRegistry
{
	void registerCommand(string const& a, char const* b, char c, char d, char e)
	{
		SYMCALL(void, MSSYM_MD5_8574de98358ff66b5a913417f44dd706, this, a, b, c, d, e);
	}
};
struct MCUUID {
	// 取uuid字符串
	std::string toString() {
		std::string s;
		SYMCALL(std::string&, MSSYM_MD5_40e8abf6eb08f7ee446159cdd0a7f283, this, &s);
		return s;
	}
};
// 玩家坐标结构体
struct Vec3 {
	float x;
	float y;
	float z;

	std::string toJsonString() {
		char str[256];
		sprintf_s(str, "%d.%d.%d", (int)x, (int)y, (int)z);
		return std::string(str);
	}
};
struct Actor {
	// 获取生物名称信息
	std::string getNameTag() {
		return SYMCALL(std::string&, MSSYM_MD5_7044ab83168b0fd345329e6566fd47fd, this);
	}
	// 获取生物当前所处维度ID
	int getDimensionId() {
		int dimensionId;
		SYMCALL(int&, MSSYM_B1QE14getDimensionIdB1AA5ActorB2AAA4UEBAB1QA2AVB2QDE11AutomaticIDB1AE10VDimensionB2AAA1HB2AAA2XZ, this, &dimensionId);
		return dimensionId;
	}


	// 获取生物当前所在坐标
	Vec3* getPos() {
		return SYMCALL(Vec3*, MSSYM_B1QA6getPosB1AA5ActorB2AAE12UEBAAEBVVec3B2AAA2XZ, this);
	}

	// 获取生物类型
	std::string getTypeName() {
		std::string actor_typename;
		SYMCALL(std::string&, MSSYM_MD5_01064f7d893d9f9ef50acf1f931d1d79, &actor_typename, this);
		return actor_typename;
	}
};
// 取uuid
struct Player : Actor {
	MCUUID* getUuid() {
		return (MCUUID*)((VA)this + 2720);
	}
	std::string getXuid(VA level) {
		return SYMCALL(std::string&, MSSYM_MD5_337bfad553c289ba4656ac43dcb60748, level, (VA)this + 2720);
	}
};

/*** MAP ***/
static std::map<std::string, Player*> onlinePlayersList;

/*** 函  数 ***/
//执行后台指令
static bool runcmd(std::string cmd) 
{
	if (p_spscqueue != 0)
		return SYMCALL(bool, MSSYM_MD5_b5c9e566146b3136e6fb37f0c080d91e, p_spscqueue, cmd);
	return false;
}
//向游戏内玩家发送文本
bool sendText(string playername, string text) 
{
	if (playername == "all")
	{
		runcmd("tellraw @a {\"rawtext\":[{\"text\":\"" + text + "\"}]}");
		return true;
	}
	else
		runcmd("tellraw " + playername + " {\"rawtext\":[{\"text\":\"" + text + "\"}]}");
	return true;
}
//插件载入->预编译头.cpp
void init() 
{
	PR(0, u8"QuickBackup Loaded");
	if (findFile("QuickBackup\\config.ini") == false)
		PR(2, u8"缺少config.ini文件 请发送命令qb_fix来修复此错误");
	if (findFile("QuickBackup\\Bandzip\\bz.exe") == false)
		PR(2, u8"缺少bz.exe文件 请发送命令qb_fix来修复此错误");
	if (findFile("QuickBackup\\Bandzip\\ark.x64.dll") == false)
		PR(2, u8"缺少ark.x64.dll文件 请发送命令qb_fix来修复此错误");
	if (findFile("QuickBackup\\Bandzip\\ark.x86.dll") == false)
		PR(2, u8"缺少ark.x86.dll文件 请发送命令qb_fix来修复此错误");	
	if (findFile("QuickBackup\\Bandzip\\ark.x64.lgpl.dll") == false)
		PR(2, u8"缺少ark.x64.lgpl.dll文件 请发送命令qb_fix来修复此错误");
	if (findFile("QuickBackup\\Bandzip\\ark.x86.lgpl.dll") == false)
		PR(2, u8"缺少ark.x86.lgpl.dll文件 请发送命令qb_fix来修复此错误");
	PR(0, u8"开始读取配置");
	opp = getConfig("QuickBackup\\config.ini", "OutputPath", "backup\\");
	opfn = getConfig("QuickBackup\\config.ini", "OutputFilname", "%Y-%m-%d-%H.zip");
	logfile.open("QuickBackup\\qb.log", ios::out | ios::app);
	PR(0, u8"插件已启动(Version 2.0.1)(GitHub Repository:https://www.github.com/Jasonzyt/QuickBackup-BDS)");
}
//插件退出->预编译头.cpp
void exit()
{
	PR(0, u8"插件已退出");
}
bool fix()
{
	if (FolderExists(L"QuickBackup") == false)
	{
		mkdir("QuickBackup");
		mkdir("QuickBackup\\Bandzip");
	}
	else if (FolderExists(L"QucikBackup\\Bandzip") == false)
	{
		mkdir("QuickBackup\\Bandzip");
	}
	if (FolderExists(L"QuickBackup") == false)
	{
		PR(2, u8"无法创建文件夹");
		PR(2, u8"无法创建文件夹");
		PR(2, u8"无法创建文件夹");
	}
	int a = getFiles();
	if (a == 0)
	{
		PR(0, u8"相关文件下载成功 请前往QuickBackup文件夹修改");
		return true;
	}
	else if (a == 1)
	{
		PR(1, u8"部分文件下载失败 正在重试");
		int b = getFiles();
		if (b == 0)
		{
			PR(0, u8"相关文件下载成功 请前往QuickBackup文件夹修改");
			return true;
		}
		else {
			PR(2, u8"相关文件下载失败 请检查网络连接或手动从GitHub下载");
			PR(2, u8"正在以默认配置启动...");
			return false;
		}

	}
	else {
		PR(2, u8"相关文件完全下载失败 请检查网络连接或手动从GitHub下载");
		PR(2, u8"正在以默认配置启动...");
		return false;
	}
}
//配置读取
bool ReloadConfig()
{
	opp = getConfig("QuickBackup\\config.ini", "OutputPath", "backup\\");
	opfn = getConfig("QuickBackup\\config.ini", "OutputFilname", "%Y-%m-%d-%H.zip");
	return true;
}
//运行备份
bool RunBackup() 
{
	string _opfn = editZIPFilename(opfn);
	PR(0, u8"Starting Backup. 开始备份");
	PR(0, u8"Starting Compress. 开始压缩");
	system((getCmdStr(opp, tp, _opfn)).c_str());
	bool v1 = findFile(opp + _opfn);
	if (v1)
	{
		PR(0, u8"Backup Successful. 备份成功");
		return true;
	}
	else
	{
		PR(0, u8"Backup Fail. 备份失败");
		return false;
	}	
}
/*** THook ***/
//假命令注册->Line 7
THook(void, MSSYM_B1QA5setupB1AE20ChangeSettingCommandB2AAE22SAXAEAVCommandRegistryB3AAAA1Z, CommandRegistry* _this) 
{
	_this->registerCommand("qb_reload", u8"重载QuickBackup插件", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_backup", u8"执行一次备份", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_auto_on", u8"开启自动备份", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_auto_off", u8"关闭自动备份", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_back", u8"打开回档GUI", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_fix", u8"检查问题并修复", 0, { 0 }, { 0x40 });
	original(_this);
}
//GetXuid
static VA pxuid_level;
THook(Player*, MSSYM_MD5_c4b0cddb50ed88e87acce18b5bd3fb8a,Player* _this, VA level, __int64 a3, int a4, __int64 a5, __int64 a6, void* xuid, std::string& strxuid, __int64* a9, __int64 a10, __int64 a11) {
	pxuid_level = level;
	return original(_this, level, a3, a4, a5, a6, xuid, strxuid, a9, a10, a11);
}
//后台命令处理
THook(bool, MSSYM_MD5_b5c9e566146b3136e6fb37f0c080d91e,VA _this, std::string* cmd) 
{
	string cmdstr = *cmd;
	if (cmdstr == "qb_backup")
	{
		bool v1 = RunBackup();
		if (v1)
		{
			PR(0, u8"备份成功");
		}
		else
		{
			PR(2, u8"备份失败,请前往控制台检查问题");
		}
		return false;
	}
	if (cmdstr == "qb_reload")
	{
		bool v2 = ReloadConfig();
		if (v2)
		{
			PR(0, u8"重载成功");
		}
		return false;
	}
	if (cmdstr == "qb_fix")
	{
		bool v3 = fix();
		if (v3)
		{
			PR(0, u8"修复成功");
		}
		else
		{
			PR(2, u8"修复失败 请重试或手动修复");
		}
		return false;
	}
	original(_this, cmd);
}
//玩家命令处理
THook(void, MSSYM_B1QA6handleB1AE20ServerNetworkHandlerB2AAE26UEAAXAEBVNetworkIdentifierB2AAE24AEBVCommandRequestPacketB3AAAA1Z,VA _this, VA id, CommandRequestPacket* crp) 
{
	Player* pPlayer = SYMCALL(Player*, MSSYM_B2QUE15getServerPlayerB1AE20ServerNetworkHandlerB2AAE20AEAAPEAVServerPlayerB2AAE21AEBVNetworkIdentifierB2AAA1EB1AA1Z,_this, id, *((char*)crp + 16));
	string uid = pPlayer->getXuid(pxuid_level);
	auto cmd = crp->toString();
	auto pr = pPlayer->getDimensionId();
	if (cmd == "/qb_backup") 
	{
		bool v1 = RunBackup();
		if (v1)
		{
			sendText(uid, u8"[QuickBackup]备份成功");
		}
		else
		{
			sendText(uid, u8"[QuickBackup]备份失败,请前往控制台检查问题");
		}
	}
	if (cmd == "/qb_reload")
	{
		bool v2 = ReloadConfig();
		if (v2)
		{
			sendText(uid, u8"[QuickBackup]重载成功");
		}
	}
	original(_this, id, crp);
}
