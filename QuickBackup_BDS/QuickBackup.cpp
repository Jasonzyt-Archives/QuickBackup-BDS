#include "Ԥ����ͷ.h"
#pragma comment(lib, "urlmon.lib")

using namespace std;

/*** ����ֵ ***/
string opp, opfn;

/*** ����ֵ ***/
//����TargetPath
string tp = "world\\";

/*** MAP ***/
static std::map<std::string, Player*> onlinePlayersList;

/*** �ṹ�� ***/
//ע������ (land-g7)
struct CommandRegistry
{
	void registerCommand(string const& a, char const* b, char c, char d, char e)
	{
		SYMCALL(void, MSSYM_MD5_8574de98358ff66b5a913417f44dd706, this, a, b, c, d, e);
	}
};
static VA p_spscqueue;
//ִ�к�ָ̨��
static bool runcmd(std::string cmd) 
{
	if (p_spscqueue != 0)
		return SYMCALL(bool, MSSYM_MD5_b5c9e566146b3136e6fb37f0c080d91e, p_spscqueue, cmd);
	return false;
}
// ȡ�����ı�
struct CommandRequestPacket 
{
	std::string toString() 
	{
		std::string str = std::string(*(std::string*)((VA)this + 40));
		return str;
	}
};

/*** ��  �� ***/
//����Ϸ����ҷ����ı�
bool sendText(string playername, string text) 
{
	runcmd("tellraw " + playername + " {\"rawtext\":[{\"text\":\"" + text + "\"}]}");
}

//�������->Ԥ����ͷ.cpp
void init() 
{
	PR(0, u8"QuickBackup Loaded");
	bool v1 = findFile("QuickBackup/config.ini");
	bool v6 = FolderExists(L"QuickBackup");
	if (v1 == false && v6 == false)
	{
		PR(0, "�����������,��ʼ��������ļ�(��),�������ʧ�������û�Ȩ��,����ʹ�ù���Ա����BDS");
		mkdir("QuickBackup");
		bool v2 = FolderExists(L"QuickBackup");
		if (v2 == false)
		{
			PR(2, "�޷������ļ���");
			PR(2, "�޷������ļ���");
			PR(2, "�޷������ļ���");
		}
		bool v3 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/config.ini", L".\\QuickBackup\\config.ini");
		if (v3)
		{
			PR(0, "config.ini���سɹ� ��ǰ��QuickBackup�ļ����޸�");
		}
		else
		{
			PR(2, "config.ini����ʧ�� ��������");
			bool v4 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/config.ini", L".\\QuickBackup\\config.ini");
			if (v4)
			{
				PR(0, "config.ini���سɹ� ��ǰ��QuickBackup�ļ����޸�");
			}
			else
			{
				PR(2, "config.ini����ʧ�� ��������");
				bool v5 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/config.ini", L".\\QuickBackup\\config.ini");
				if (v5)
				{
					PR(0, "config.ini���سɹ� ��ǰ��QuickBackup�ļ����޸�");
				}
				else
				{
					PR(2, "config.ini����ʧ�� �����������ӻ��ֶ���GitHub����");
					PR(2, "������Ĭ����������...");
				}
			}
		}
	}
	else if (v1 == false && v6 == false)
	{
		PR(2, "�Ҳ��������ļ�,��ʼ��ͼ��ȡ����ļ�");
		bool v3 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/config.ini", L".\\QuickBackup\\config.ini");
		if (v3)
		{
			PR(0, "config.ini���سɹ� ��ǰ��QuickBackup�ļ����޸�");
		}
		else
		{
			PR(2, "config.ini����ʧ�� ��������");
			bool v4 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/config.ini", L".\\QuickBackup\\config.ini");
			if (v4)
			{
				PR(0, "config.ini���سɹ� ��ǰ��QuickBackup�ļ����޸�");
			}
			else
			{
				PR(2, "config.ini����ʧ�� ��������");
				bool v5 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/config.ini", L".\\QuickBackup\\config.ini");
				if (v5)
				{
					PR(0, "config.ini���سɹ� ��ǰ��QuickBackup�ļ����޸�");
				}
				else
				{
					PR(2, "config.ini����ʧ�� �����������ӻ��ֶ���GitHub����");
					PR(2, "������Ĭ����������...");
				}
			}
		}
	}
	opp = getConfig("QuickBackup\\config.ini", "OutputPath", "backup\\");
	opfn = getConfig("QuickBackup\\config.ini", "OutputFilname", "%Y-%m-%d-%H.zip");
	PR(0, "���������(Version 1.0.5)(GitHub Repository:https://www.github.com/Jasonzyt/QuickBackup-BDS)");
}
//����˳�->Ԥ����ͷ.cpp
void exit()
{
	PR(0, "������˳�");
}

//���ö�ȡ
bool ReloadConfig()
{
	opp = getConfig("QuickBackup\\config.ini", "OutputPath", "backup\\");
	opfn = getConfig("QuickBackup\\config.ini", "OutputFilname", "%Y-%m-%d-%H.zip");
	return true;
}
//���б���
string RunBackup() 
{
	string _opfn = editZIPFilename(opfn);
	PR(0, "Starting Backup. ��ʼ����");
	PR(0, "Starting Compress. ��ʼѹ��");
	system((getCmdStr(opp, tp, _opfn)).c_str());
	PR(0, "Backup Successful. ���ݳɹ�");
	return "Successful";
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
//�����
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