#include "Ԥ����ͷ.h"
#pragma comment(lib,"URlmon")

using namespace std;

/*** ����ֵ ***/
string opp, opfn;
static VA p_spscqueue;
ofstream logfile;
bool v8, v9, v10, v11;

/*** ����ֵ ***/
//����TargetPath
string tp = "world\\";

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
	int sb = GetSystemBits(); //sb = SystemBits
	bool v1 = findFile("QuickBackup\\config.ini");
	bool v6 = FolderExists(L"QuickBackup");
	bool v7 = findFile("QuickBackup\\Bandzip\\bz.exe");
	if (sb == 64)
	{
		v8 = findFile("QuickBackup\\Bandzip\\ark.x64.dll");
		v9 = findFile("QuickBackup\\Bandzip\\ark.x64.lgpl.dll");
	}
	else if (sb == 32)
	{
		v10 = findFile("QuickBackup\\Bandzip\\ark.x86.dll");
		v11 = findFile("QuickBackup\\Bandzip\\ark.x86.lgpl.dll");
	}
	if (v1 == false)
	{
		PR(0, u8"�����������,��ʼ��������ļ�(��),�������ʧ�������û�Ȩ��,����ʹ�ù���Ա����BDS");
		if (FolderExists(L"QuickBackup") == false)
		{
			mkdir("QuickBackup");
			mkdir("QuickBackup\\Bandzip");
		}
		else if (FolderExists(L"QucikBackup\\Bandzip") == false)
		{
			mkdir("QuickBackup\\Bandzip");
		}
		bool v2 = FolderExists(L"QuickBackup");
		if (v2 == false)
		{
			PR(2, u8"�޷������ļ���");
			PR(2, u8"�޷������ļ���");
			PR(2, u8"�޷������ļ���");
		}
		bool v3 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/config.ini", L".\\QuickBackup\\config.ini");
		bool a1 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/bz.exe", L".\\QuickBackup\\Bandzip\\bz.exe");
		if (sb == 64)
		{
			DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x64.dll", L".\\QuickBackup\\Bandzip\\ark.x64.dll");
			DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x64.lgpl.dll", L".\\QuickBackup\\Bandzip\\ark.x64.lgpl.dll");
		}
		else if (sb == 32)
		{
			DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x86.dll", L".\\QuickBackup\\Bandzip\\ark.x86.dll");
			DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x86.lgpl.dll", L".\\QuickBackup\\Bandzip\\ark.x86.lgpl.dll");
		}
		if (v3 && a1)
		{
			PR(0, u8"����ļ����سɹ� ��ǰ��QuickBackup�ļ����޸�");
		}
		else
		{
			PR(2, u8"����ļ�����ʧ�� ��������");
			bool v4 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/config.ini", L".\\QuickBackup\\config.ini");
			bool a2 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/bz.exe", L".\\QuickBackup\\Bandzip\\bz.exe");
			if (sb == 64)
			{
				DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x64.dll", L".\\QuickBackup\\Bandzip\\ark.x64.dll");
				DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x64.lgpl.dll", L".\\QuickBackup\\Bandzip\\ark.x64.lgpl.dll");
			}
			else if (sb == 32)
			{
				DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x86.dll", L".\\QuickBackup\\Bandzip\\ark.x86.dll");
				DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x86.lgpl.dll", L".\\QuickBackup\\Bandzip\\ark.x86.lgpl.dll");
			}
			if (v4 && a2)
			{
				PR(0, u8"����ļ����سɹ� ��ǰ��QuickBackup�ļ����޸�");
			}
			else
			{
				PR(2, u8"����ļ�����ʧ�� ��������");
				bool v5 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/config.ini", L".\\QuickBackup\\config.ini");
				bool a3 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/bz.exe", L".\\QuickBackup\\Bandzip\\bz.exe");
				if (sb == 64)
				{
					DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x64.dll", L".\\QuickBackup\\Bandzip\\ark.x64.dll");
					DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x64.lgpl.dll", L".\\QuickBackup\\Bandzip\\ark.x64.lgpl.dll");
				}
				else if (sb == 32)
				{
					DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x86.dll", L".\\QuickBackup\\Bandzip\\ark.x86.dll");
					DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x86.lgpl.dll", L".\\QuickBackup\\Bandzip\\ark.x86.lgpl.dll");
				}
				if (v5 && a3)
				{
					PR(0, u8"����ļ����سɹ� ��ǰ��QuickBackup�ļ����޸�");
				}
				else
				{
					PR(2, u8"����ļ�����ʧ�� �����������ӻ��ֶ���GitHub����");
					PR(2, u8"������Ĭ����������...");
				}
			}
		}
	}
	else
	{
		//bz.exe
		if (v7 == false)
		{
			PR(2, u8"ȱ��bz.exe,��ʼ��ͼ��ȡ����ļ�");
			bool b1 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/bz.exe", L".\\QuickBackup\\Bandzip\\bz.exe");
			if (b1)
				PR(0, u8"bz.exe���سɹ�");
			else
				PR(2, u8"bz.exe����ʧ�� �����в�ȫ");
		}
		//ark.x64.dll
		if (v8 == false && sb == 64)
		{
			PR(2, u8"ȱ��ark.x64.dll,��ʼ��ͼ��ȡ����ļ�");
			bool b2 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x64.dll", L".\\QuickBackup\\Bandzip\\ark.x64.dll");
			if (b2)
				PR(0, u8"ark.x64.dll���سɹ�");
			else
				PR(2, u8"ark.x64.dll����ʧ�� �����в�ȫ");
		}
		//ark.x64.lgpl.dll
		if (v9 == false && sb == 64)
		{
			PR(2, u8"ȱ��ark.x64.lgpl.dll,��ʼ��ͼ��ȡ����ļ�");
			bool b2 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x64.lgpl.dll", L".\\QuickBackup\\Bandzip\\ark.x64.lgpl.dll");
			if (b2)
				PR(0, u8"ark.x64.lgpl.dll���سɹ�");
			else
				PR(2, u8"ark.x64.lgpl.dll����ʧ�� �����в�ȫ");
		}
		//ark.x86.dll
		if (v10 == false && sb == 32)
		{
			PR(2, u8"ȱ��ark.x86.dll,��ʼ��ͼ��ȡ����ļ�");
			bool b3 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x86.dll", L".\\QuickBackup\\Bandzip\\ark.x86.dll");
			if (b3)
				PR(0, u8"ark.x86.dll���سɹ�");
			else
				PR(2, u8"ark.x86.dll����ʧ�� �����в�ȫ");
		}
		//ark.x64.lgpl.dll
		if (v11 == false && sb == 32)
		{
			PR(2, "ȱ��ark.x86.lgpl.dll,��ʼ��ͼ��ȡ����ļ�");
			bool b4 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/ark.x86.lgpl.dll", L".\\QuickBackup\\Bandzip\\ark.x86.lgpl.dll");
			if (b4)
				PR(0, u8"ark.x86.lgpl.dll���سɹ�");
			else
				PR(2, u8"ark.x86.lgpl.dll����ʧ�� �����в�ȫ");
		}
	}
	opp = getConfig("QuickBackup\\config.ini", "OutputPath", "backup\\");
	opfn = getConfig("QuickBackup\\config.ini", "OutputFilname", "%Y-%m-%d-%H.zip");
	logfile.open("QuickBackup\\qb.log", ios::out | ios::app);
	PR(0, u8"���������(Version 1.2.4)(GitHub Repository:https://www.github.com/Jasonzyt/QuickBackup-BDS)");
}
//����˳�->Ԥ����ͷ.cpp
void exit()
{
	PR(0, u8"������˳�");
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
THook(void, MSSYM_B1QA5setupB1AE20ChangeSettingCommandB2AAE22SAXAEAVCommandRegistryB3AAAA1Z, CommandRegistry* _this) {
	_this->registerCommand("qb_reload", u8"����QuickBackup���", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_backup", u8"ִ��һ�α���", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_auto_on", u8"�����Զ�����", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_auto_off", u8"�ر��Զ�����", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_back", u8"�򿪻ص�GUI", 0, { 0 }, { 0x40 });
	original(_this);
}
//GetXuid
static VA pxuid_level;
THook(Player*, MSSYM_MD5_c4b0cddb50ed88e87acce18b5bd3fb8a,Player* _this, VA level, __int64 a3, int a4, __int64 a5, __int64 a6, void* xuid, std::string& strxuid, __int64* a9, __int64 a10, __int64 a11) {
	pxuid_level = level;
	return original(_this, level, a3, a4, a5, a6, xuid, strxuid, a9, a10, a11);
}
//�����
THook(void, MSSYM_B1QA6handleB1AE20ServerNetworkHandlerB2AAE26UEAAXAEBVNetworkIdentifierB2AAE24AEBVCommandRequestPacketB3AAAA1Z,
	VA _this, VA id, CommandRequestPacket* crp) {
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
		return;
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
