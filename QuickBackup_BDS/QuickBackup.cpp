#include "Ԥ����ͷ.h"
#pragma comment(lib,"URlmon")
#pragma warning(disable:6031)
#pragma warning(disable:4996)

using namespace std;

/*** ����ֵ ***/
string opp, opfn, _opfn, _cb, _lb;
static VA p_spscqueue, pxuid_level;
ofstream logfile;
bool v8, v9, v10, v11;
wchar_t* lv;
wchar_t* cv;
wchar_t* lr;
int cb, lb;
fstream jsonfile;

/*** �������� ***/
//(���ֺ�����δ����,�Ⲣ����һ����ϰ��[doge])
bool fix();
bool ReloadConfig();
bool RunBackup();
//bool Back(string filepath);

/*** ����ֵ ***/
//����TargetPath
string tp = ".\\worlds\\";
string BACKUPJSON;

/*** �ṹ�� ***/
// ȡ�����ı�
struct CommandRequestPacket {
	std::string toString() {
		std::string str = std::string(*(std::string*)((VA)this + 40));
		return str;
	}
};
//ȡ��ID����ѡ��
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
//ע������ (land-g7)
struct CommandRegistry
{
	void registerCommand(string const& a, char const* b, char c, char d, char e)
	{
		SYMCALL(void, MSSYM_MD5_8574de98358ff66b5a913417f44dd706, this, a, b, c, d, e);
	}
};
struct MCUUID {
	// ȡuuid�ַ���
	std::string toString() {
		std::string s;
		SYMCALL(std::string&, MSSYM_MD5_40e8abf6eb08f7ee446159cdd0a7f283, this, &s);
		return s;
	}
};
struct Actor {
	// ��ȡ����������Ϣ
	std::string getNameTag() {
		return SYMCALL(std::string&, MSSYM_MD5_7044ab83168b0fd345329e6566fd47fd, this);
	}
};
// ȡuuid
struct Player : Actor {
	MCUUID* getUuid() {
		return (MCUUID*)((VA)this + 2720);
	}
	std::string getXuid(VA level) {
		return SYMCALL(std::string&, MSSYM_MD5_337bfad553c289ba4656ac43dcb60748, level, (VA)this + 2720);
	}
};

/*** MAP ***/

/*** ��  �� ***/
//ִ�к�ָ̨��
static bool runcmd(std::string cmd) 
{
	if (p_spscqueue != 0)
		return SYMCALL(bool, MSSYM_MD5_b5c9e566146b3136e6fb37f0c080d91e, p_spscqueue, cmd);
	return false;
}
//����Ϸ����ҷ����ı�
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
// ��ȡJSON�ļ�����
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
		PR(1, u8"Backup.json��ȡʧ�� �������ζ�Żص������޷�ʹ��");
	}
	return content;
}
//�������->Ԥ����ͷ.cpp
void init() 
{
	PR(0, u8"QuickBackup Loaded");
	if (FolderExists(L"QuickBackup"))
	{
		if (findFile("QuickBackup\\config.ini") == false)
			PR(2, u8"ȱ��config.ini�ļ� �뷢������qb_fix���޸��˴���");
		if (findFile("QuickBackup\\Bandzip\\bz.exe") == false)
			PR(2, u8"ȱ��bz.exe�ļ� �뷢������qb_fix���޸��˴���");
		if (findFile("QuickBackup\\Bandzip\\ark.x64.dll") == false)
			PR(2, u8"ȱ��ark.x64.dll�ļ� �뷢������qb_fix���޸��˴���");
		if (findFile("QuickBackup\\Bandzip\\ark.x86.dll") == false)
			PR(2, u8"ȱ��ark.x86.dll�ļ� �뷢������qb_fix���޸��˴���");	
		if (findFile("QuickBackup\\Bandzip\\ark.x64.lgpl.dll") == false)
			PR(2, u8"ȱ��ark.x64.lgpl.dll�ļ� �뷢������qb_fix���޸��˴���");
		if (findFile("QuickBackup\\Bandzip\\ark.x86.lgpl.dll") == false)
			PR(2, u8"ȱ��ark.x86.lgpl.dll�ļ� �뷢������qb_fix���޸��˴���");
	}
	else
	{
		PR(0, u8"����������� ����������qb_fix��������ļ�");
	}
	PR(0, u8"��ʼ��ȡ����");
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
	// PR(0, u8"Tips:����qb_update���ٸ����°汾");
	PR(0, u8"���������(Version 2.0.6 #2020-10-24 Programmer's Day Special Edition#)(GitHub Repository:https://www.github.com/Jasonzyt/QuickBackup-BDS)");
}
//����˳�->Ԥ����ͷ.cpp
void exit()
{
	PR(0, u8"������˳�(Version 2.0.6 #2020-10-24 Programmer's Day Special Edition#)(GitHub Repository:https://www.github.com/Jasonzyt/QuickBackup-BDS)");
}
//JSONд��
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
	//����д�� 
	Json::StyledWriter w_json;
	string inputstr = w_json.write(jsonroot);
	//������ļ� 
	jsonfile.open("QuickBackup\\Backup.json", ios::out); 
	if (!jsonfile.is_open())
		PR(2, u8"û���ҵ���¼�ļ�:\"Backup.json\".");
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
		PR(2, u8"�޷������ļ���");
		PR(2, u8"�޷������ļ���");
		PR(2, u8"�޷������ļ���");
	}
	int a = getFiles();
	if (a == 0)
	{
		PR(0, u8"����ļ����سɹ� ��ǰ��QuickBackup�ļ����޸�");
		return true;
	}
	else if (a == 1)
	{
		PR(1, u8"�����ļ�����ʧ�� ��������");
		int b = getFiles();
		if (b == 0)
		{
			PR(0, u8"����ļ����سɹ� ��ǰ��QuickBackup�ļ����޸�");
			return true;
		}
		else {
			PR(2, u8"����ļ�����ʧ�� �����������ӻ��ֶ���GitHub����");
			PR(2, u8"������Ĭ����������...");
			return false;
		}

	}
	else {
		PR(2, u8"����ļ���ȫ����ʧ�� �����������ӻ��ֶ���GitHub����");
		PR(2, u8"������Ĭ����������...");
		return false;
	}
}
//���ö�ȡ
bool ReloadConfig()
{
	opp = getConfig("QuickBackup\\config.ini", "OutputPath", "backup\\");
	opfn = getConfig("QuickBackup\\config.ini", "OutputFilname", "%Y-%m-%d-%H.zip");
	BACKUPJSON = getjson(".\\QuickBackup\\Backup.json");
	return true;
}
//���б���
bool RunBackup() 
{
	_opfn = editZIPFilename(opfn);
	PR(0, u8"Starting Backup. ��ʼ����");
	PR(0, u8"Starting Compress. ��ʼѹ��");
	system((getCmdStr(opp, tp, _opfn)).c_str());
	ostringstream bp;
	bp << opp << _opfn;
	bool v1 = findFile(bp.str());
	if (v1)
	{
		PR(0, u8"Backup Successful. ���ݳɹ�");
		writeBackupJson(bp.str());
		return true;
	}
	else
	{
		PR(2, u8"Backup Fail. ����ʧ��");
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
		a << u8"�˰汾�����µķ��а汾(��ǰ�汾:" << _cb << u8"-" << cv << u8",���°汾:" << _lb << u8"-" << lv << u8")";
		PR(0, a.str());
		return true;
	}
	else if (lv == cv)
	{
		a << u8"�˰汾�����°汾(��ǰ�汾:" << _cb << u8"-" << cv << u8",���·��а汾:Release-" << lr << u8")";
		PR(0, a.str());
		return true;
	}
	else
	{
		a << u8"�˰汾�Ѿ���ʱ ����/qb_update������(��ǰ�汾:" << _cb << u8"-" << cv << u8",���°汾:" << _lb << u8"-" << lv << u8",���·��а汾:Release-" << lr << u8")";
		PR(0, a.str());
		return false;
	}
}
*/
//bool AutoOn() 
//{	}

/*** THook ***/
//������ע��->Line 7
THook(void, MSSYM_B1QA5setupB1AE20ChangeSettingCommandB2AAE22SAXAEAVCommandRegistryB3AAAA1Z, CommandRegistry* _this) 
{
	_this->registerCommand("qb_reload", u8"����QuickBackup���", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_backup", u8"ִ��һ�α���", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_auto_on", u8"�����Զ�����", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_auto_off", u8"�ر��Զ�����", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_back", u8"�򿪻ص�GUI", 0, { 0 }, { 0x40 });
	//_this->registerCommand("qb_fix", u8"������Ⲣ�޸�", 0, { 0 }, { 0x40 });
	original(_this);
}
//GetXuid
THook(Player*, MSSYM_MD5_c4b0cddb50ed88e87acce18b5bd3fb8a,Player* _this, VA level, __int64 a3, int a4, __int64 a5, __int64 a6, void* xuid, std::string& strxuid, __int64* a9, __int64 a10, __int64 a11) {
	pxuid_level = level;
	return original(_this, level, a3, a4, a5, a6, xuid, strxuid, a9, a10, a11);
}
//��̨�����
THook(bool, MSSYM_MD5_b5c9e566146b3136e6fb37f0c080d91e,VA _this, std::string* cmd) 
{
	string cmdstr = *cmd;
	logfile.open("QuickBackup\\qb.log");
	if (cmdstr == "qb_backup")
	{
		bool v1 = RunBackup();
		if (v1)
		{
			PR(0, u8"���ݳɹ�");
			logfile << "[" << getTime() << "] " << "����ִ̨����һ�α���(�ɹ�) �����ļ���" << opp << _opfn << endl;
		}
		else
		{
			PR(2, u8"����ʧ��,�뷢��qb_fix��qb_reload������");
			logfile << "[" << getTime() << "] " << "����ִ̨����һ�α���(ʧ��)" << endl;
		}
		return false;
	}
	if (cmdstr == "qb_reload")
	{
		bool v2 = ReloadConfig();
		if (v2)
		{
			PR(0, u8"���سɹ�");
			logfile << "[" << getTime() << "] " << " ����ִ̨����һ������(�ɹ�)" << endl;
		}
		return false;
	}
	if (cmdstr == "qb_fix")
	{
		bool v3 = fix();
		if (v3)
		{
			PR(0, u8"�޸��ɹ�");
			logfile << "[" << getTime() << "] " << " ����ִ̨����һ���޸�(�ɹ�)" << endl;
		}
		else
		{
			PR(2, u8"�޸�ʧ�� �����Ի��ֶ��޸�");
			logfile << "[" << getTime() << "] " << " ����ִ̨����һ���޸�(ʧ��)" << endl;
		}
		return false;
	}
	if (cmdstr == "qb_auto_on")
	{
		//thread ab1(AutoOn);
		//PR(0, u8"�Զ������ѿ���");
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
//��������
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
			sendText(playername, u8"[QuickBackup]���ݳɹ�");
			cout << "[" << getTime() << "] " << "��� " << playername << " ִ����һ�α���(�ɹ�) �����ļ���" << opp << _opfn << endl;
			logfile << "[" << getTime() << "] " << "��� " << playername << " ִ����һ�α���(�ɹ�) �����ļ���" << opp << _opfn << endl;
		}
		else
		{
			sendText(playername, u8"[QuickBackup]����ʧ��,��ʹ��/qb_reload������");
			cout << "[" << getTime() << "] " << "��� " << playername << " ִ����һ�α���(ʧ��)" << endl;
			logfile << "[" << getTime() << "] " << "��� " << playername << " ִ����һ�α���(ʧ��)" << endl;
		}
	}
	if (cmd == "/qb_reload")
	{
		bool v2 = ReloadConfig();
		if (v2)
		{
			sendText(playername, u8"[QuickBackup]���سɹ�");
			cout << "[" << getTime() << "] " << "��� " << playername << " ִ����һ������(�ɹ�)" << endl;
			logfile << "[" << getTime() << "] " << "��� " << playername << " ִ����һ������(�ɹ�)" << endl;
		}
	}
	logfile.close();
	original(_this, id, crp);
}
