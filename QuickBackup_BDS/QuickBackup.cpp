#include "预编译头.h"
#pragma comment(lib, "urlmon.lib")

using namespace std;

/*** 声明值 ***/
string opp, opfn;
static VA p_spscqueue;
ofstream logfile;

/*** 定义值 ***/
//备份TargetPath
string tp = "world\\";

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
	logfile.open("QuickBackup\\qb.log", ios::out | ios::app);
	PR(0, "插件已启动(Version 1.0.6)(GitHub Repository:https://www.github.com/Jasonzyt/QuickBackup-BDS)");
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
bool RunBackup() 
{
	string _opfn = editZIPFilename(opfn);
	PR(0, "Starting Backup. 开始备份");
	PR(0, "Starting Compress. 开始压缩");
	system((getCmdStr(opp, tp, _opfn)).c_str());
	bool v1 = findFile(opp + _opfn);
	if (v1)
	{
		PR(0, "Backup Successful. 备份成功");
		return true;
	}
	else
	{
		PR(0, "Backup Fail. 备份失败");
		return false;
	}	
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
//GetXuid
static VA pxuid_level;
THook(Player*, MSSYM_MD5_c4b0cddb50ed88e87acce18b5bd3fb8a,Player* _this, VA level, __int64 a3, int a4, __int64 a5, __int64 a6, void* xuid, std::string& strxuid, __int64* a9, __int64 a10, __int64 a11) {
	pxuid_level = level;
	return original(_this, level, a3, a4, a5, a6, xuid, strxuid, a9, a10, a11);
}
//命令处理
THook(void, MSSYM_B1QA6handleB1AE20ServerNetworkHandlerB2AAE26UEAAXAEBVNetworkIdentifierB2AAE24AEBVCommandRequestPacketB3AAAA1Z,
	VA _this, VA id, CommandRequestPacket* crp) {
	Player* pPlayer = SYMCALL(Player*, MSSYM_B2QUE15getServerPlayerB1AE20ServerNetworkHandlerB2AAE20AEAAPEAVServerPlayerB2AAE21AEBVNetworkIdentifierB2AAA1EB1AA1Z,_this, id, *((char*)crp + 16));
	string uid = pPlayer->getXuid(pxuid_level);
	auto cmd = crp->toString();
	auto pr = pPlayer->getDimensionId();
	if (cmd == "/qb_backup") {
		bool v1 = RunBackup();
		if (v1)
		{
			sendText(uid, "[QuickBackup]备份成功");
		}
		return;
	}
	original(_this, id, crp);
}

int main()
{

}