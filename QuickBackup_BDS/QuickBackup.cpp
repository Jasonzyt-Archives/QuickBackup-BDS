#include "Ԥ����ͷ.h"
#pragma comment(lib,"URlmon")
#pragma warning(disable:6031)
#pragma warning(disable:4996)

using namespace std;

/*** ����ֵ ***/
string opp, opfn;
static VA p_spscqueue;
ofstream logfile;
bool v8, v9, v10, v11;

/*** ����ֵ ***/
//����TargetPath
string tp = ".\\world\\";

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
// �������ṹ��
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
	// ��ȡ����������Ϣ
	std::string getNameTag() {
		return SYMCALL(std::string&, MSSYM_MD5_7044ab83168b0fd345329e6566fd47fd, this);
	}
	// ��ȡ���ﵱǰ����ά��ID
	int getDimensionId() {
		int dimensionId;
		SYMCALL(int&, MSSYM_B1QE14getDimensionIdB1AA5ActorB2AAA4UEBAB1QA2AVB2QDE11AutomaticIDB1AE10VDimensionB2AAA1HB2AAA2XZ, this, &dimensionId);
		return dimensionId;
	}


	// ��ȡ���ﵱǰ��������
	Vec3* getPos() {
		return SYMCALL(Vec3*, MSSYM_B1QA6getPosB1AA5ActorB2AAE12UEBAAEBVVec3B2AAA2XZ, this);
	}

	// ��ȡ��������
	std::string getTypeName() {
		std::string actor_typename;
		SYMCALL(std::string&, MSSYM_MD5_01064f7d893d9f9ef50acf1f931d1d79, &actor_typename, this);
		return actor_typename;
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
static std::map<std::string, Player*> onlinePlayersList;

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
//�������->Ԥ����ͷ.cpp
void init() 
{
	PR(0, u8"QuickBackup Loaded");
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
	PR(0, u8"��ʼ��ȡ����");
	opp = getConfig("QuickBackup\\config.ini", "OutputPath", "backup\\");
	opfn = getConfig("QuickBackup\\config.ini", "OutputFilname", "%Y-%m-%d-%H.zip");
	logfile.open("QuickBackup\\qb.log", ios::out | ios::app);
	PR(0, u8"���������(Version 2.0.1)(GitHub Repository:https://www.github.com/Jasonzyt/QuickBackup-BDS)");
}
//����˳�->Ԥ����ͷ.cpp
void exit()
{
	PR(0, u8"������˳�");
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
	return true;
}
//���б���
bool RunBackup() 
{
	string _opfn = editZIPFilename(opfn);
	PR(0, u8"Starting Backup. ��ʼ����");
	PR(0, u8"Starting Compress. ��ʼѹ��");
	system((getCmdStr(opp, tp, _opfn)).c_str());
	bool v1 = findFile(opp + _opfn);
	if (v1)
	{
		PR(0, u8"Backup Successful. ���ݳɹ�");
		return true;
	}
	else
	{
		PR(0, u8"Backup Fail. ����ʧ��");
		return false;
	}	
}
/*** THook ***/
//������ע��->Line 7
THook(void, MSSYM_B1QA5setupB1AE20ChangeSettingCommandB2AAE22SAXAEAVCommandRegistryB3AAAA1Z, CommandRegistry* _this) 
{
	_this->registerCommand("qb_reload", u8"����QuickBackup���", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_backup", u8"ִ��һ�α���", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_auto_on", u8"�����Զ�����", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_auto_off", u8"�ر��Զ�����", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_back", u8"�򿪻ص�GUI", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_fix", u8"������Ⲣ�޸�", 0, { 0 }, { 0x40 });
	original(_this);
}
//GetXuid
static VA pxuid_level;
THook(Player*, MSSYM_MD5_c4b0cddb50ed88e87acce18b5bd3fb8a,Player* _this, VA level, __int64 a3, int a4, __int64 a5, __int64 a6, void* xuid, std::string& strxuid, __int64* a9, __int64 a10, __int64 a11) {
	pxuid_level = level;
	return original(_this, level, a3, a4, a5, a6, xuid, strxuid, a9, a10, a11);
}
//��̨�����
THook(bool, MSSYM_MD5_b5c9e566146b3136e6fb37f0c080d91e,VA _this, std::string* cmd) 
{
	string cmdstr = *cmd;
	if (cmdstr == "qb_backup")
	{
		bool v1 = RunBackup();
		if (v1)
		{
			PR(0, u8"���ݳɹ�");
		}
		else
		{
			PR(2, u8"����ʧ��,��ǰ������̨�������");
		}
		return false;
	}
	if (cmdstr == "qb_reload")
	{
		bool v2 = ReloadConfig();
		if (v2)
		{
			PR(0, u8"���سɹ�");
		}
		return false;
	}
	if (cmdstr == "qb_fix")
	{
		bool v3 = fix();
		if (v3)
		{
			PR(0, u8"�޸��ɹ�");
		}
		else
		{
			PR(2, u8"�޸�ʧ�� �����Ի��ֶ��޸�");
		}
		return false;
	}
	original(_this, cmd);
}
//��������
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
			sendText(uid, u8"[QuickBackup]���ݳɹ�");
		}
		else
		{
			sendText(uid, u8"[QuickBackup]����ʧ��,��ǰ������̨�������");
		}
	}
	if (cmd == "/qb_reload")
	{
		bool v2 = ReloadConfig();
		if (v2)
		{
			sendText(uid, u8"[QuickBackup]���سɹ�");
		}
	}
	original(_this, id, crp);
}
