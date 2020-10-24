#include "预编译头.h"
#pragma comment(lib,"URlmon")
#pragma warning(disable:6031)
#pragma warning(disable:4996)

using namespace std;

/*** 声明值 ***/
string opp, opfn, _opfn, _cb, _lb;
static VA p_spscqueue, pxuid_level;
ofstream logfile;
bool v8, v9, v10, v11;
wchar_t* lv;
wchar_t* cv;
wchar_t* lr;
int cb, lb;
fstream jsonfile;

/*** 函数声明 ***/
//(部分函数并未声明,这并不是一个好习惯[doge])
bool fix();
bool ReloadConfig();
bool RunBackup();
//bool Back(string filepath);

/*** 定义值 ***/
//备份TargetPath
string tp = ".\\worlds\\";
string BACKUPJSON;

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
struct Actor {
	// 获取生物名称信息
	std::string getNameTag() {
		return SYMCALL(std::string&, MSSYM_MD5_7044ab83168b0fd345329e6566fd47fd, this);
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
// 获取JSON文件内容
string getjson(string name) 
{
	ifstream in;
	in.open(name);
	std::string content((std::istreambuf_iterator<char>(in)), (std::istreambuf_iterator<char>()));
	in.close();
	if (content == "") { return "{}"; }
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(content, root) || content == "{}") 
	{
		return content;
	}
	else
	{
		PR(1, u8"Backup.json读取失败 这可能意味着回档功能无法使用");
	}
	return content;
}
//插件载入->预编译头.cpp
void init() 
{
	PR(0, u8"QuickBackup Loaded");
	if (FolderExists(L"QuickBackup"))
	{
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
	}
	else
	{
		PR(0, u8"初次启动插件 请输入命令qb_fix下载相关文件");
	}
	PR(0, u8"开始读取配置");
	opp = getConfig("QuickBackup\\config.ini", "OutputPath", "backup\\");
	opfn = getConfig("QuickBackup\\config.ini", "OutputFilname", "%Y-%m-%d-%H.zip");
	logfile.open("QuickBackup\\qb.log", ios::out | ios::app);
	logfile.close();
	if (!findFile(".\\QuickBackup\\Backup.json"))
	{
		jsonfile.open("QuickBackup\\Backup.json", ios::out | ios::app);
		jsonfile << "{}";
		jsonfile.close();
	}
	BACKUPJSON = getjson(".\\QuickBackup\\Backup.json");
	// PR(0, u8"Tips:发送qb_update快速更新新版本");
	PR(0, u8"插件已启动(Version 2.0.6 #2020-10-24 Programmer's Day Special Edition#)(GitHub Repository:https://www.github.com/Jasonzyt/QuickBackup-BDS)");
}
//插件退出->预编译头.cpp
void exit()
{
	PR(0, u8"插件已退出(Version 2.0.6 #2020-10-24 Programmer's Day Special Edition#)(GitHub Repository:https://www.github.com/Jasonzyt/QuickBackup-BDS)");
}
//JSON写入
void writeBackupJson(string path)
{
	Json::Value jsonroot;
	Json::Value m;
	Json::Reader reader;
	BACKUPJSON = getjson(".\\QuickBackup\\Backup.json");
	reader.parse(BACKUPJSON, jsonroot);
	string time = getMinute();
	m["PATH"] = Json::Value(path);
	jsonroot[time] = Json::Value(m);
	//缩进写入 
	Json::StyledWriter w_json;
	string inputstr = w_json.write(jsonroot);
	//输出到文件 
	jsonfile.open("QuickBackup\\Backup.json", ios::out); 
	if (!jsonfile.is_open())
		PR(2, u8"没有找到记录文件:\"Backup.json\".");
	jsonfile << inputstr;
	jsonfile.close();
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
	BACKUPJSON = getjson(".\\QuickBackup\\Backup.json");
	return true;
}
//运行备份
bool RunBackup() 
{
	_opfn = editZIPFilename(opfn);
	PR(0, u8"Starting Backup. 开始备份");
	PR(0, u8"Starting Compress. 开始压缩");
	system((getCmdStr(opp, tp, _opfn)).c_str());
	ostringstream bp;
	bp << opp << _opfn;
	bool v1 = findFile(bp.str());
	if (v1)
	{
		PR(0, u8"Backup Successful. 备份成功");
		writeBackupJson(bp.str());
		return true;
	}
	else
	{
		PR(2, u8"Backup Fail. 备份失败");
		return false;
	}	
}

/*
bool CheckUpdate()
{
	DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/.lastestupdate", L".\\QuickBackup\\.lastupdate");
	WritePrivateProfileString(L"CURRENTVERSION", L"Current_Version", L"2.0.6", L".\\QuickBackup\\.lastupdate");
	WritePrivateProfileString(L"CURRENTVERSION", L"Is_Beta", L"1", L".\\QuickBackup\\.lastupdate");
	GetPrivateProfileString(L"LASTUPDATE", L"Lastest_Version", L"", lv, 128, L".\\QuickBackup\\.lastupdate");
	GetPrivateProfileString(L"LASTUPDATE", L"Lastest_Release", L"", lr, 128, L".\\QuickBackup\\.lastupdate");
	GetPrivateProfileString(L"CURRENTVERSION", L"Current_Version", L"", cv, 128, L".\\QuickBackup\\.lastupdate");
	cb = GetPrivateProfileInt(L"CURRENTVERSION", L"Is_Beta", NULL, L".\\QuickBackup\\.lastupdate");
	lb = GetPrivateProfileInt(L"LASTUPDATE", L"Is_Beta", NULL, L".\\QuickBackup\\.lastupdate");
	if (cb == 0)
		_cb = "Release";
	else
		_cb = "Beta";
	if (lb == 0)
		_lb = "Release";
	else
		_lb = "Beta";
	ostringstream a;
	if (lr == cv)
	{
		a << u8"此版本是最新的发行版本(当前版本:" << _cb << u8"-" << cv << u8",最新版本:" << _lb << u8"-" << lv << u8")";
		PR(0, a.str());
		return true;
	}
	else if (lv == cv)
	{
		a << u8"此版本是最新版本(当前版本:" << _cb << u8"-" << cv << u8",最新发行版本:Release-" << lr << u8")";
		PR(0, a.str());
		return true;
	}
	else
	{
		a << u8"此版本已经过时 发送/qb_update来更新(当前版本:" << _cb << u8"-" << cv << u8",最新版本:" << _lb << u8"-" << lv << u8",最新发行版本:Release-" << lr << u8")";
		PR(0, a.str());
		return false;
	}
}
*/
//bool AutoOn() 
//{	}

/*** THook ***/
//假命令注册->Line 7
THook(void, MSSYM_B1QA5setupB1AE20ChangeSettingCommandB2AAE22SAXAEAVCommandRegistryB3AAAA1Z, CommandRegistry* _this) 
{
	_this->registerCommand("qb_reload", u8"重载QuickBackup插件", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_backup", u8"执行一次备份", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_auto_on", u8"开启自动备份", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_auto_off", u8"关闭自动备份", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_back", u8"打开回档GUI", 0, { 0 }, { 0x40 });
	//_this->registerCommand("qb_fix", u8"检查问题并修复", 0, { 0 }, { 0x40 });
	original(_this);
}
//GetXuid
THook(Player*, MSSYM_MD5_c4b0cddb50ed88e87acce18b5bd3fb8a,Player* _this, VA level, __int64 a3, int a4, __int64 a5, __int64 a6, void* xuid, std::string& strxuid, __int64* a9, __int64 a10, __int64 a11) {
	pxuid_level = level;
	return original(_this, level, a3, a4, a5, a6, xuid, strxuid, a9, a10, a11);
}
//后台命令处理
THook(bool, MSSYM_MD5_b5c9e566146b3136e6fb37f0c080d91e,VA _this, std::string* cmd) 
{
	string cmdstr = *cmd;
	logfile.open("QuickBackup\\qb.log");
	if (cmdstr == "qb_backup")
	{
		bool v1 = RunBackup();
		if (v1)
		{
			PR(0, u8"备份成功");
			logfile << "[" << getTime() << "] " << "控制台执行了一次备份(成功) 备份文件在" << opp << _opfn << endl;
		}
		else
		{
			PR(2, u8"备份失败,请发送qb_fix或qb_reload后重试");
			logfile << "[" << getTime() << "] " << "控制台执行了一次备份(失败)" << endl;
		}
		return false;
	}
	if (cmdstr == "qb_reload")
	{
		bool v2 = ReloadConfig();
		if (v2)
		{
			PR(0, u8"重载成功");
			logfile << "[" << getTime() << "] " << " 控制台执行了一次重载(成功)" << endl;
		}
		return false;
	}
	if (cmdstr == "qb_fix")
	{
		bool v3 = fix();
		if (v3)
		{
			PR(0, u8"修复成功");
			logfile << "[" << getTime() << "] " << " 控制台执行了一次修复(成功)" << endl;
		}
		else
		{
			PR(2, u8"修复失败 请重试或手动修复");
			logfile << "[" << getTime() << "] " << " 控制台执行了一次修复(失败)" << endl;
		}
		return false;
	}
	if (cmdstr == "qb_auto_on")
	{
		//thread ab1(AutoOn);
		//PR(0, u8"自动备份已开启");
		return false;
	}
	if (cmdstr == "qb" || cmdstr == "qb_" || cmdstr == "qb_help" || cmdstr == "qb help" || cmdstr == "help qb")
	{
		cout << "QuickBackup Server Commands\nqb_backup:Run once backup (NOT AutoBackup)\nqb_reload:Reload config file\nqb_fix:Run fix function,downlaod config file and bandzip console application";
		return false;
	}
	if (cmdstr == "test")
	{
		writeBackupJson("WDNMD:\\114514");
	}
	logfile.close();
	original(_this, cmd);
}
//玩家命令处理
THook(void, MSSYM_B1QA6handleB1AE20ServerNetworkHandlerB2AAE26UEAAXAEBVNetworkIdentifierB2AAE24AEBVCommandRequestPacketB3AAAA1Z,VA _this, VA id, CommandRequestPacket* crp) 
{
	Player* pPlayer = SYMCALL(Player*, MSSYM_B2QUE15getServerPlayerB1AE20ServerNetworkHandlerB2AAE20AEAAPEAVServerPlayerB2AAE21AEBVNetworkIdentifierB2AAA1EB1AA1Z,_this, id, *((char*)crp + 16));
	string uid = pPlayer->getXuid(pxuid_level);
	string playername = pPlayer->getNameTag();
	auto cmd = crp->toString();
	logfile.open("QuickBackup\\qb.log");
	if (cmd == "/qb_backup") 
	{
		bool v1 = RunBackup();
		if (v1)
		{
			sendText(playername, u8"[QuickBackup]备份成功");
			cout << "[" << getTime() << "] " << "玩家 " << playername << " 执行了一次备份(成功) 备份文件在" << opp << _opfn << endl;
			logfile << "[" << getTime() << "] " << "玩家 " << playername << " 执行了一次备份(成功) 备份文件在" << opp << _opfn << endl;
		}
		else
		{
			sendText(playername, u8"[QuickBackup]备份失败,请使用/qb_reload后重试");
			cout << "[" << getTime() << "] " << "玩家 " << playername << " 执行了一次备份(失败)" << endl;
			logfile << "[" << getTime() << "] " << "玩家 " << playername << " 执行了一次备份(失败)" << endl;
		}
	}
	if (cmd == "/qb_reload")
	{
		bool v2 = ReloadConfig();
		if (v2)
		{
			sendText(playername, u8"[QuickBackup]重载成功");
			cout << "[" << getTime() << "] " << "玩家 " << playername << " 执行了一次重载(成功)" << endl;
			logfile << "[" << getTime() << "] " << "玩家 " << playername << " 执行了一次重载(成功)" << endl;
		}
	}
	logfile.close();
	original(_this, id, crp);
}
