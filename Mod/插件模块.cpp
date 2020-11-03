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
fstream jsonfile, formfile;
string BACKUPJSON, FORMJSON;

/*** 函数声明 ***/
//(部分函数并未声明,这并不是一个好习惯[doge])
bool fix();
bool ReloadConfig();
bool RunBackup();
//bool Back(string filepath);

/*** 定义值 ***/
//备份TargetPath
string tp = ".\\worlds\\";

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
		SYMCALL<void>("?registerCommand@CommandRegistry@@QEAAXAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PEBDW4CommandPermissionLevel@@UCommandFlag@@3@Z", this, a, b, c, d, e);
	}
};
struct MCUUID {
	// 取uuid字符串
	std::string toString() {
		std::string s;
		SYMCALL<string&>("?asString@UUID@mce@@QEBA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ", this, &s);
		return s;
	}
};
struct Actor {
	// 获取生物名称信息
	std::string getNameTag() {
		return SYMCALL<string&>("?getNameTag@Actor@@UEBAAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ", this);
	}
};
// 取uuid
struct Player : Actor {
	MCUUID* getUuid() {
		return (MCUUID*)((VA)this + 2720);
	}
	std::string getXuid(VA level) {
		return SYMCALL<string&>("?getPlayerXUID@Level@@QEBAAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AEBVUUID@mce@@@Z", level, (VA)this + 2720);
	}
	// 获取网络标识符
	VA getNetId() {
		return (VA)this + 2944;		// IDA ServerPlayer::setPermissions
	}

	// 发送数据包
	VA sendPacket(VA pkt) {
		return SYMCALL<VA>("?sendNetworkPacket@ServerPlayer@@UEBAXAEAVPacket@@@Z", this, pkt);
	}
};
struct ModalFormRequestPacket {
	char filler[0x48];
};

/*** MAP ***/
static std::map<std::string, Player*> onlinePlayers;//在线列表
static std::map<string, string> playerName;
static std::map<Player*, bool> player_isoline;
static std::map<unsigned, bool> fids;
static std::unordered_map<Player*, bool> playerSign;

/*** 函  数 ***/
static std::queue<std::function<void()>> todos;

void safeTick(std::function<void()> F) {
	todos.push(F);
}
//执行后台指令
static bool runcmd(std::string cmd)
{
	if (p_spscqueue != 0)
		return SYMCALL<bool>("??$inner_enqueue@$0A@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@?$SPSCQueue@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$0CAA@@@AEAA_NAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z", p_spscqueue, cmd);
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
static unsigned getFormId() {
	unsigned id = time(0);
	do {
		--id;
	} while (id == 0 || fids[id]);
	fids[id] = true;
	return id;
}
bool destroyForm(unsigned fid)
{
	if (fids[fid]) {
		fids.erase(fid);
		return true;
	}
	return false;
}
unsigned sendForm(std::string uuid, std::string str)
{
	unsigned fid = getFormId();
	// 此处自主创建包
	auto fr = [uuid, fid, str]() {
		Player* p = onlinePlayers[uuid];
		if (playerSign[p]) {
			VA tpk;
			ModalFormRequestPacket sec;
			SYMCALL<VA>("?createPacket@MinecraftPackets@@SA?AV?$shared_ptr@VPacket@@@std@@W4MinecraftPacketIds@@@Z", &tpk, 100);
			*(VA*)(tpk + 40) = fid;
			*(std::string*)(tpk + 48) = str;
			p->sendPacket(tpk);
		}
	};
	safeTick(fr);
	return fid;
}
// 发送指定玩家一个自定义GUI界面
static UINT sendCustomForm(char* uuid, char* json) {
	Player* p = onlinePlayers[uuid];
	if (!playerSign[p])
		return 0;
	return sendForm(uuid, json);
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
		Json::Value root;
		jsonfile.open("QuickBackup\\Backup.json", ios::out | ios::app);
		root["Quantity"] = Json::Value(0);
		//缩进写入 
		Json::StyledWriter w_json;
		string inputstr = w_json.write(root);
		//输出到文件 
		if (!jsonfile.is_open())
			PR(2, u8"没有找到记录文件:\"Backup.json\".");
		jsonfile << inputstr;
		jsonfile.close();
	}
	formfile.open("QuickBackup\\Form.json", ios::out | ios::app);
	formfile.close();
	BACKUPJSON = getjson(".\\QuickBackup\\Backup.json");
	FORMJSON = getjson(".\\QuickBackup\\From.json");
	// PR(0, u8"Tips:发送qb_update快速更新新版本");
	PR(0, u8"感谢 TISUnion(https://www.github.com/TISUnion/QuickBackupM) 的灵感支持");
	PR(0, u8"感谢 BDSJSRunner(https://mzgl.coding.net/public/BDSJSRunner/BDSJSRunner/git/files/master/BDSJSRunner) 的代码支持(部分源代码来自JSRunner)");
	PR(0, u8"插件已启动(Version 3.0.1 #BDXCore Edition#)(GitHub Repository:https://www.github.com/Jasonzyt/QuickBackup-BDS)");
}
//插件退出->预编译头.cpp
void exit()
{
	PR(0, u8"插件已退出(Version 3.0.1 #BDXCore Edition#)(GitHub Repository:https://www.github.com/Jasonzyt/QuickBackup-BDS)");
}
//BACKUP/FROM JSON写入
void writeJsons(string path, string opfn)
{	
	//Backup.json
	Json::Value jsonroot;
	Json::Value m;
	Json::Value backups;
	//From.json
	Json::Value form;
	Json::Value label;

	Json::Reader reader;
	Json::StyledWriter w_form;
	Json::StyledWriter w_json;
	BACKUPJSON = getjson(".\\QuickBackup\\Backup.json");
	FORMJSON = getjson(".\\QuickBackup\\From.json");
	reader.parse(FORMJSON, form);
	reader.parse(BACKUPJSON, jsonroot);
	// 获取当前系统时间
	string time = getTime();
	string minute = getMinute();
	// 获取备份文件大小(Byte)
	size_t fsb = GetFileSizeByte(path);
	string fs = GetFileSize(path);
	// 当前数量读取
	int num = jsonroot["Quantity"].asInt();
	// 序号=数量+1
	int sn = num + 1;
	ostringstream a;a << sn;
	string sns = a.str();
	// 输出路径&数量&文件大小
	cout << "[" << time << u8" INFO][QuickBackup] 此次备份存档路径:" << path << endl;
	cout << "[" << time << u8" INFO][QuickBackup] 当前备份存档数量:" << sn << endl;
	cout << "[" << time << u8" INFO][QuickBackup] 此次备份存档大小:" << fs << endl;
	// 数量+1
	jsonroot["Quantity"] = Json::Value(sn);

	// 写入路径
	m["PATH"] = Json::Value(path);
	// 写入备份时间
	m["MINUTE"] = Json::Value(minute);
	// 写入备份精确时间
	m["TIME"] = Json::Value(time);
	// 写入备份文件大小
	m["FILESIZE"][0] = Json::Value(fsb);
	// 写入备份文件精确大小
	m["FILESIZE"][1] = Json::Value(fs);
	// 挂载到根节点上
	jsonroot["Backups"].append(m);

	// 写入表单基础信息
	label["type"] = Json::Value("label");
	label["text"] = Json::Value(u8"请选择要回的存档"); // u8必不可少 否则写入的全是乱码
	form["type"] = Json::Value("from");
	form["title"] = Json::Value(u8"QuickBackup 回档GUI"); // 同上
	// 写入表单content
	form["content"] = Json::Value(label);
	form["buttons"].append(opfn);

	// 输出字符串
	string inputformstr = w_form.write(form);
	string inputbackupstr = w_json.write(jsonroot);
	// 输出到文件 
	jsonfile.open("QuickBackup\\Backup.json", ios::out);
	if (!jsonfile.is_open())
		PR(2, u8"没有找到记录文件:\"Backup.json\".");
	jsonfile << inputbackupstr;
	jsonfile.close();
	formfile.open(".\\QuickBackup\\Form.json", ios::out);
	if (!formfile.is_open())
		PR(2, u8"没有找到表单记录文件:\"Form.json\".");
	formfile << inputformstr;
	formfile.close();
}
// 写入文件
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
		writeJsons(bp.str(),_opfn);
		return true;
	}
	else
	{
		PR(2, u8"Backup Fail. 备份失败");
		return false;
	}
}
/*** SYMHOOK ***/
//假命令注册->Line 7
SYMHOOK(FakeCommandReg, void, "?setup@ChangeSettingCommand@@SAXAEAVCommandRegistry@@@Z", CommandRegistry* _this)
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
SYMHOOK(GetXuid, Player*, "??0Player@@QEAA@AEAVLevel@@AEAVPacketSender@@W4GameType@@AEBVNetworkIdentifier@@EVUUID@mce@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$unique_ptr@VCertificate@@U?$default_delete@VCertificate@@@std@@@8@55@Z", Player* _this, VA level, __int64 a3, int a4, __int64 a5, __int64 a6, void* xuid, std::string& strxuid, __int64* a9, __int64 a10, __int64 a11) {
	pxuid_level = level;
	return original(_this, level, a3, a4, a5, a6, xuid, strxuid, a9, a10, a11);
}
//后台命令处理
SYMHOOK(ServerCommand, bool, "??$inner_enqueue@$0A@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@?$SPSCQueue@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$0CAA@@@AEAA_NAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z", VA _this, std::string* cmd)
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
	logfile.close();
	original(_this, cmd);
}
//玩家命令处理
SYMHOOK(PlayerCommand, void, "?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVCommandRequestPacket@@@Z", VA _this, VA id, CommandRequestPacket* crp)
{
	Player* pPlayer = SYMCALL<Player*>("?_getServerPlayer@ServerNetworkHandler@@AEAAPEAVServerPlayer@@AEBVNetworkIdentifier@@E@Z", _this, id, *((char*)crp + 16));
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
/* 取表单选择序号
SYMHOOK(GetFormSelected, void, "?handle@?$PacketHandlerDispatcherInstance@VModalFormResponsePacket@@$0A@@@UEBAXAEBVNetworkIdentifier@@AEAVNetEventCallback@@AEAV?$shared_ptr@VPacket@@@std@@@Z", VA _this, VA id, VA handle, ModalFormResponsePacket** fp)
{
	ModalFormResponsePacket* fmp = *fp;

	Player* p = SYMCALL<Player*>("?_getServerPlayer@ServerNetworkHandler@@AEAAPEAVServerPlayer@@AEBVNetworkIdentifier@@E@Z", handle, id, *(char*)((VA)fmp + 16));
	string  uid = p->getXuid(pxuid_level);
	if (p != NULL)
	{
		UINT fid = fmp->getFormId();
		if (destroyForm(fid))
		{
			auto formid = fid;
			auto selected = fmp->getSelectStr();
		}
	}

	original(_this, id, handle, fp);
}
*/
//SYMHOOK(结构名, int, "符号", int a/*参数列表*/) {
// 	// 函数体
//return original(a);
//}
//void init() {
//	SYMCALL<int>("符号",0/*参数列表*/);
//}