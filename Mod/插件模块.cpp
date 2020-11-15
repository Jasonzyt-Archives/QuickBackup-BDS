#include "预编译头.h"
#include "插件模块.h"
#pragma comment(lib,"URlmon")
#pragma warning(disable:6031)
#pragma warning(disable:4996)    

using namespace std;

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

void safeTick(std::function<void()> F) 
{
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
	ostringstream o;
	if (playername == "all")
	{
		o << u8"tellraw @a {\"rawtext\":[{\"text\":\"" << text << u8"\"}]}";
		runcmd(o.str());
		return true;
	}
	else
	{
		o << u8"tellraw " << playername << " {\"rawtext\":[{\"text\":\"" << text << u8"\"}]}";
		runcmd(o.str());
		return true;
	}
}
static unsigned getFormId() 
{
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
static UINT sendCustomForm(char* uuid, char* json) 
{
	Player* p = onlinePlayers[uuid];
	if (!playerSign[p])
		return 0;
	return sendForm(uuid, json);
}
//插件载入->预编译头.cpp
void init()
{
	PR(0, u8"QuickBackup Loaded");
	if (findFile(".\\QuickBackup\\Form.json"))
	{
		if (!findFile(".\\QuickBackup\\.DoNotDeleteMe"))
		{
			int v;
			PR(1, u8"此次更新表单相关函数代码变动较大 请输入 114514 来同意删除Form.json重新创建");
			cout << u8"Input \"114514\" :";
			cin >> v;
			if (v == 114514)
			{
				fstream f;
				f.open(".\\QuickBackup\\.DoNotDeleteMe", ios::out | ios::app);
				f.close();
				DeleteFile(L".\\QuickBackup\\Form.json");
			}
			else
			{
				PR(0, u8"\n进程即将结束");
				system("pause");
				exit(1);
			}
		}
	}
	else
	{
		fstream f;
		f.open(".\\QuickBackup\\.DoNotDeleteMe", ios::out | ios::app);
		f.close();
	}
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
	perfile.open("QuickBackup\\permissions.json", ios::out | ios::app);
	perfile.close();
	BACKUPJSON = getjson(".\\QuickBackup\\Backup.json");
	FORMJSON = getjson(".\\QuickBackup\\From.json");
	// PR(0, u8"Tips:发送qb_update快速更新新版本");
	PR(0, u8"感谢 TISUnion(https://www.github.com/TISUnion/QuickBackupM) 的灵感支持");
	PR(0, u8"感谢 BDSJSRunner(https://mzgl.coding.net/public/BDSJSRunner/BDSJSRunner/git/files/master/BDSJSRunner) 的代码支持(部分源代码来自JSRunner)");
	PR(0, u8"插件已启动(Version " << QBVERSION << u8"#BETA##BDXCore Edition#)(GitHub Repository:https://www.github.com/Jasonzyt/QuickBackup-BDS)");
}
/* *********************************  Json处理写入区  *********************************** */
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
		PR(1, u8"Json读取失败 这可能意味着回档功能无法使用");
	}
	return content;
}
// BACKUP/FROM JSON写入
void writeJsons(string path, string opfn,string Operator)
{	
	//Backup.json
	Json::Value jsonroot;
	Json::Value m;
	Json::Value backups;
	Json::Value o;
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
	// 获取文件MD5值
	FILE* fp = fopen(path.data(), "wb");
	string md5 = md5file(fp);
	fclose(fp);
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
	// 输出备份INFO
	PR(0, u8"此次备份存档路径:" << path);
	PR(0, u8"当前备份存档数量:" << sn);
	PR(0, u8"此次备份存档大小:" << fs << "(" << fsb << " Byte)");
	PR(0, u8"此次备份的执行者:" << Operator);
	PR(0, u8"此次备份文件的MD5值:" << md5);
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
	// 写入运行者信息
	if (Operator == "Server")
		m["Operator"] = Json::Value("Server");
	else
	{
		reader.parse(Operator, o);
		m["Operator"] = Json::Value(o);
	}
	// 写入备份MD5
	m["MD5"] = Json::Value(md5);
	// 挂载到根节点上
	jsonroot["Backups"].append(m);
	// 写入表单基础信息
	label["type"] = Json::Value("label");
	label["text"] = Json::Value(u8"请选择要回的存档"); // u8必不可少 否则写入的全是乱码
	form["type"] = Json::Value("from");
	form["title"] = Json::Value(u8"QuickBackup 回档GUI"); // 同上
	// 写入表单content
	form["content"].append(label);
	// 写入表单buttons
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
string createOperatorJson(string playername, string xuid, string uuid)
{
	Json::Value a;
	Json::FastWriter b;
	a["Playername"] = Json::Value(playername);
	a["Xuid"] = Json::Value(xuid);
	a["Uuid"] = Json::Value(uuid);
	return b.write(a);
}
// $opfn.from.json 创建&写入
void createFromJson(size_t fsb, int sn, string opfn, string fs, string time, string minute, string path, string _operator, string md5)
{
	// 文件创建模块
	fstream file;
	ostringstream fn;
	fn << opfn << ".form.json";
	file.open(fn.str(), ios::out | ios::app);
	file.close();
	// 表单Json写入内存模块
	Json::Value form;
	Json::Value content;
	Json::Value bts;

	Json::Reader reader;
	Json::StyledWriter in;
		// 数据拼接
		ostringstream a, b, c, d, e, f, g, h, i;
		a << u8"QuickBackup - " << opfn;
		b << u8"备份序号:" << sn;
		c << u8"备份时间:" << minute;
		d << u8"备份精确时间:" << time;
		e << u8"备份文件大小:" << fs << "(" << fsb << " Byte)";
		f << u8"备份输出文件名:" << opfn;
		g << u8"备份目录:" << path;
		h << u8"本次备份执行者:" << _operator;
		i << u8"此次备份文件MD5值:" << md5;
		// 写入表单基础信息
		form["type"] = Json::Value("custom_from");
		form["title"] = Json::Value(a.str());
		// 写入表单label
		label::a["type"] = Json::Value("label");
		label::a["text"] = Json::Value(u8"备份存档详细信息");
		label::b["type"] = Json::Value("label");
		label::b["text"] = Json::Value(b.str());
		label::c["type"] = Json::Value("label");
		label::c["text"] = Json::Value(c.str());
		label::d["type"] = Json::Value("label");
		label::d["text"] = Json::Value(d.str());
		label::e["type"] = Json::Value("label");
		label::e["text"] = Json::Value(e.str());
		label::f["type"] = Json::Value("label");
		label::f["text"] = Json::Value(f.str());
		label::g["type"] = Json::Value("label");
		label::g["text"] = Json::Value(g.str());
		label::h["type"] = Json::Value("label");
		label::h["text"] = Json::Value(h.str());
		label::i["type"] = Json::Value("label");
		label::i["text"] = Json::Value(i.str());
		// 写入表单toggle
		toggle::a["type"] = Json::Value("toggle");
		toggle::a["default"] = Json::Value(false);
		toggle::a["text"] = Json::Value(u8"回档完是否自动重启");
		// 挂载到content
		content["content"].append(label::a);
		content["content"].append(label::b);
		content["content"].append(label::c);
		content["content"].append(label::d);
		content["content"].append(label::e);
		content["content"].append(label::f);
		content["content"].append(label::g);
		content["content"].append(label::h);
		content["content"].append(label::i);
		content["content"].append(toggle::a);


	form.append(content);
}
// 获取备份数量
int getQuantity()
{
	Json::Value root;
	Json::Reader reader;
	BACKUPJSON = getjson(".\\QuickBackup\\Backup.json");
	reader.parse(BACKUPJSON, root);
	return root["Quantity"].asInt();
}
// BACKUP/FORM JSON删除
void delJsons(int sn);
// 获取所有备份
Json::Value getAllBackups()
{
	Json::Value root;
	Json::Value ret;
	Json::Reader reader;
	BACKUPJSON = getjson(".\\QuickBackup\\Backup.json");
	reader.parse(BACKUPJSON, root);
	int quantity = getQuantity();
	for (int a = 1; a <= quantity; a++)
	{	
		ostringstream o;
		string str;
		o << a;
		str = o.str();
		Json::Value tmp = root["Backups"][a - 1];
		ret[str] = Json::Value(tmp);
	}
	return ret;
}
string getListStr(Json::Value input)
{
	ostringstream o;
	string a = input["TIME"].asString();
	string b;
	if (input["Operator"] == Json::Value("Server"))
		b = "Server";
	else
		b = input["Operator"]["Playername"].asString();
	int c = input["FILESIZE"][0].asInt();
	string d = input["FILESIZE"][1].asString();
	string e = input["MD5"].asString();
	o   << u8"-备份时间:" << a 
		<< u8"\n-文件大小:" << d << u8"(" << c << u8" Byte)" 
		<< u8"\n-备份执行者:" << b 
		<< u8"\n-备份文件MD5值:" << e << endl;
	return o.str();
}
/* *********************************  主要功能函数  *********************************** */
// 输出所有备份(客户端)
void listBackupsC(string PlayerName)
{
	Json::Value root = getAllBackups();
	ostringstream w;
	string s, a, b, c, d, e;
	int q = getQuantity();
	w << u8"§b当前共有 " << q << u8" 个备份" << endl;
	sendText(PlayerName,w.str());
	for (int i = 1; i <= q; i++)
	{
		ostringstream oo, o; o << i;
		string s = o.str();
		Json::Value m = root[s];
		string output = getListStr(m);
		oo << u8"备份序号[" << i << "]";
		sendText(PlayerName, oo.str());
		sendText(PlayerName, output);
	}
}
// 输出所有备份(服务端)
void listBackupsS()
{
	Json::Value root = getAllBackups();
	//Json::StyledWriter w;cout << w.write(root);
	int q = getQuantity();
	cout << u8"当前共有 " << q << u8" 个备份" << endl;
	for (int i = 1; i <= q; i++)
	{
		ostringstream o;o << i;
		string s = o.str();
		Json::Value m = root[s];
		string output = getListStr(m);
		cout << u8"备份序号[" << i << "]\n";
		cout << output;
	}
}
// 修复
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
bool RunBackup(string _operator)
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
		writeJsons(bp.str(), _opfn, _operator);
		return true;
	}
	else
	{
		PR(2, u8"Backup Fail. 备份失败");
		return false;
	}
}
/* *********************************  游戏SYMHOOK  *********************************** */
//假命令注册->Line 7
SYMHOOK(FakeCommandReg, void, "?setup@ChangeSettingCommand@@SAXAEAVCommandRegistry@@@Z", CommandRegistry* _this)
{
	_this->registerCommand("qb_reload", u8"重载QuickBackup插件", 0, { 0 }, { 0x40 }); 
	_this->registerCommand("qb_backup", u8"执行一次备份", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_list", u8"列出所有备份", 0, { 0 }, { 0x40 });//
	_this->registerCommand("qb_help", u8"显示qb命令帮助", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_backgui", u8"打开回档GUI", 0, { 0 }, { 0x40 });//
	_this->registerCommand("qb_gui", u8"打开插件GUI", 0, { 0 }, { 0x40 });//
	_this->registerCommand("qb_reload", u8"重载本插件", 0, { 0 }, { 0x40 });
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
		bool v1 = RunBackup("Server");
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
	if (cmdstr == "qb_list")
	{
		listBackupsS();
		return false;
	}
	if (cmdstr == "qb" || cmdstr == "qb_" || cmdstr == "qb_help" || cmdstr == "qb help" || cmdstr == "help qb")
	{
		cout << u8"QuickBackup 服务端命令\nqb_backup:运行一次备份\nqb_reload:重载插件\nqb_fix:运行修复函数,下载Bandzip控制台程序\nqb_list:显示所有备份\nqb_back <序号>:回到<序号>这个存档";
		return false;
	}
	logfile.close();
	original(_this, cmd);
}
//玩家命令处理
SYMHOOK(PlayerCommand, void, "?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVCommandRequestPacket@@@Z", VA _this, VA id, CommandRequestPacket* crp)
{
	Player* pPlayer = SYMCALL<Player*>("?_getServerPlayer@ServerNetworkHandler@@AEAAPEAVServerPlayer@@AEBVNetworkIdentifier@@E@Z", _this, id, *((char*)crp + 16));
	string xuid = pPlayer->getXuid(pxuid_level);
	string playername = pPlayer->getNameTag();
	string uuid = pPlayer->getUuid()->toString();
	auto cmd = crp->toString();
	logfile.open("QuickBackup\\qb.log");
	if (cmd == "/qb_backup")
	{
		string _operator = createOperatorJson(playername, xuid, uuid);
		bool v1 = RunBackup(_operator);
		if (v1)
		{
			sendText(playername, u8"§b[QuickBackup] §a备份成功");
			cout << "[" << getTime() << "] " << "玩家 " << playername << " 执行了一次备份(成功) 备份文件在" << opp << _opfn << endl;
			logfile << "[" << getTime() << "] " << "玩家 " << playername << " 执行了一次备份(成功) 备份文件在" << opp << _opfn << endl;
		}
		else
		{
			sendText(playername, u8"§b[QuickBackup] §c备份失败,请使用/qb_reload后重试");
			cout << "[" << getTime() << "] " << "玩家 " << playername << " 执行了一次备份(失败)" << endl;
			logfile << "[" << getTime() << "] " << "玩家 " << playername << " 执行了一次备份(失败)" << endl;
		}
		return;
	}
	if (cmd == "/qb_reload")
	{
		bool v2 = ReloadConfig();
		if (v2)
		{
			sendText(playername, u8"§b[QuickBackup] §a重载成功");
			cout << "[" << getTime() << "] " << "玩家 " << playername << " 执行了一次重载(成功)" << endl;
			logfile << "[" << getTime() << "] " << "玩家 " << playername << " 执行了一次重载(成功)" << endl;
		}
		return;
	}
	if (cmd == "qb" || cmd == "qb_" || cmd == "qb_help" || cmd == "qb help" || cmd == "help qb")
	{
		sendText(playername, u8"§cQuickBackup 客户端命令\n§6qb_backup:运行一次备份\n§eqb_reload:重载插件\n§aqb_list:显示所有备份\n§3qb_backgui:显示回档GUI\n§bqb_gui:显示插件GUI");
		return;
	}
	logfile.close();
	original(_this, id, crp);
}
SYMHOOK(GetCommandQueue, VA, "??0?$SPSCQueue@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$0CAA@@@QEAA@_K@Z", VA _this) {
	p_spscqueue = original(_this);
	return p_spscqueue;
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
