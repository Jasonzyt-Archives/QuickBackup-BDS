#include "Ԥ����ͷ.h"
#include "���ģ��.h"
#pragma comment(lib,"URlmon")
#pragma warning(disable:6031)
#pragma warning(disable:4996)    

using namespace std;

/*** ����ֵ ***/
//����TargetPath
string tp = ".\\worlds\\";

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
		SYMCALL<void>("?registerCommand@CommandRegistry@@QEAAXAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PEBDW4CommandPermissionLevel@@UCommandFlag@@3@Z", this, a, b, c, d, e);
	}
};
struct MCUUID {
	// ȡuuid�ַ���
	std::string toString() {
		std::string s;
		SYMCALL<string&>("?asString@UUID@mce@@QEBA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ", this, &s);
		return s;
	}
};
struct Actor {
	// ��ȡ����������Ϣ
	std::string getNameTag() {
		return SYMCALL<string&>("?getNameTag@Actor@@UEBAAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ", this);
	}
};
// ȡuuid
struct Player : Actor {
	MCUUID* getUuid() {
		return (MCUUID*)((VA)this + 2720);
	}
	std::string getXuid(VA level) {
		return SYMCALL<string&>("?getPlayerXUID@Level@@QEBAAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AEBVUUID@mce@@@Z", level, (VA)this + 2720);
	}
	// ��ȡ�����ʶ��
	VA getNetId() {
		return (VA)this + 2944;		// IDA ServerPlayer::setPermissions
	}
	// �������ݰ�
	VA sendPacket(VA pkt) {
		return SYMCALL<VA>("?sendNetworkPacket@ServerPlayer@@UEBAXAEAVPacket@@@Z", this, pkt);
	}
};
struct ModalFormRequestPacket {
	char filler[0x48];
};

/*** MAP ***/
static std::map<std::string, Player*> onlinePlayers;//�����б�
static std::map<string, string> playerName;
static std::map<Player*, bool> player_isoline;
static std::map<unsigned, bool> fids;
static std::unordered_map<Player*, bool> playerSign;

/*** ��  �� ***/

static std::queue<std::function<void()>> todos;

void safeTick(std::function<void()> F) 
{
	todos.push(F);
}
//ִ�к�ָ̨��
static bool runcmd(std::string cmd)
{
	if (p_spscqueue != 0)
		return SYMCALL<bool>("??$inner_enqueue@$0A@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@?$SPSCQueue@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$0CAA@@@AEAA_NAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z", p_spscqueue, cmd);
	return false;
}
//����Ϸ����ҷ����ı�
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
	// �˴�����������
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
// ����ָ�����һ���Զ���GUI����
static UINT sendCustomForm(char* uuid, char* json) 
{
	Player* p = onlinePlayers[uuid];
	if (!playerSign[p])
		return 0;
	return sendForm(uuid, json);
}
//�������->Ԥ����ͷ.cpp
void init()
{
	PR(0, u8"QuickBackup Loaded");
	if (findFile(".\\QuickBackup\\Form.json"))
	{
		if (!findFile(".\\QuickBackup\\.DoNotDeleteMe"))
		{
			int v;
			PR(1, u8"�˴θ��±���غ�������䶯�ϴ� ������ 114514 ��ͬ��ɾ��Form.json���´���");
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
				PR(0, u8"\n���̼�������");
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
		Json::Value root;
		jsonfile.open("QuickBackup\\Backup.json", ios::out | ios::app);
		root["Quantity"] = Json::Value(0);
		//����д�� 
		Json::StyledWriter w_json;
		string inputstr = w_json.write(root);
		//������ļ� 
		if (!jsonfile.is_open())
			PR(2, u8"û���ҵ���¼�ļ�:\"Backup.json\".");
		jsonfile << inputstr;
		jsonfile.close();
	}
	formfile.open("QuickBackup\\Form.json", ios::out | ios::app);
	formfile.close();
	perfile.open("QuickBackup\\permissions.json", ios::out | ios::app);
	perfile.close();
	BACKUPJSON = getjson(".\\QuickBackup\\Backup.json");
	FORMJSON = getjson(".\\QuickBackup\\From.json");
	// PR(0, u8"Tips:����qb_update���ٸ����°汾");
	PR(0, u8"��л TISUnion(https://www.github.com/TISUnion/QuickBackupM) �����֧��");
	PR(0, u8"��л BDSJSRunner(https://mzgl.coding.net/public/BDSJSRunner/BDSJSRunner/git/files/master/BDSJSRunner) �Ĵ���֧��(����Դ��������JSRunner)");
	PR(0, u8"���������(Version " << QBVERSION << u8"#BETA##BDXCore Edition#)(GitHub Repository:https://www.github.com/Jasonzyt/QuickBackup-BDS)");
}
/* *********************************  Json����д����  *********************************** */
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
		PR(1, u8"Json��ȡʧ�� �������ζ�Żص������޷�ʹ��");
	}
	return content;
}
// BACKUP/FROM JSONд��
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
	// ��ȡ�ļ�MD5ֵ
	FILE* fp = fopen(path.data(), "wb");
	string md5 = md5file(fp);
	fclose(fp);
	// ��ȡ��ǰϵͳʱ��
	string time = getTime();
	string minute = getMinute();
	// ��ȡ�����ļ���С(Byte)
	size_t fsb = GetFileSizeByte(path);
	string fs = GetFileSize(path);
	// ��ǰ������ȡ
	int num = jsonroot["Quantity"].asInt();
	// ���=����+1
	int sn = num + 1;
	ostringstream a;a << sn;
	string sns = a.str();
	// �������INFO
	PR(0, u8"�˴α��ݴ浵·��:" << path);
	PR(0, u8"��ǰ���ݴ浵����:" << sn);
	PR(0, u8"�˴α��ݴ浵��С:" << fs << "(" << fsb << " Byte)");
	PR(0, u8"�˴α��ݵ�ִ����:" << Operator);
	PR(0, u8"�˴α����ļ���MD5ֵ:" << md5);
	// ����+1
	jsonroot["Quantity"] = Json::Value(sn);
	// д��·��
	m["PATH"] = Json::Value(path);
	// д�뱸��ʱ��
	m["MINUTE"] = Json::Value(minute);
	// д�뱸�ݾ�ȷʱ��
	m["TIME"] = Json::Value(time);
	// д�뱸���ļ���С
	m["FILESIZE"][0] = Json::Value(fsb);
	// д�뱸���ļ���ȷ��С
	m["FILESIZE"][1] = Json::Value(fs);
	// д����������Ϣ
	if (Operator == "Server")
		m["Operator"] = Json::Value("Server");
	else
	{
		reader.parse(Operator, o);
		m["Operator"] = Json::Value(o);
	}
	// д�뱸��MD5
	m["MD5"] = Json::Value(md5);
	// ���ص����ڵ���
	jsonroot["Backups"].append(m);
	// д���������Ϣ
	label["type"] = Json::Value("label");
	label["text"] = Json::Value(u8"��ѡ��Ҫ�صĴ浵"); // u8�ز����� ����д���ȫ������
	form["type"] = Json::Value("from");
	form["title"] = Json::Value(u8"QuickBackup �ص�GUI"); // ͬ��
	// д���content
	form["content"].append(label);
	// д���buttons
	form["buttons"].append(opfn);
	// ����ַ���
	string inputformstr = w_form.write(form);
	string inputbackupstr = w_json.write(jsonroot);
	// ������ļ� 
	jsonfile.open("QuickBackup\\Backup.json", ios::out);
	if (!jsonfile.is_open())
		PR(2, u8"û���ҵ���¼�ļ�:\"Backup.json\".");
	jsonfile << inputbackupstr;
	jsonfile.close();
	formfile.open(".\\QuickBackup\\Form.json", ios::out);
	if (!formfile.is_open())
		PR(2, u8"û���ҵ�����¼�ļ�:\"Form.json\".");
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
// $opfn.from.json ����&д��
void createFromJson(size_t fsb, int sn, string opfn, string fs, string time, string minute, string path, string _operator, string md5)
{
	// �ļ�����ģ��
	fstream file;
	ostringstream fn;
	fn << opfn << ".form.json";
	file.open(fn.str(), ios::out | ios::app);
	file.close();
	// ��Jsonд���ڴ�ģ��
	Json::Value form;
	Json::Value content;
	Json::Value bts;

	Json::Reader reader;
	Json::StyledWriter in;
		// ����ƴ��
		ostringstream a, b, c, d, e, f, g, h, i;
		a << u8"QuickBackup - " << opfn;
		b << u8"�������:" << sn;
		c << u8"����ʱ��:" << minute;
		d << u8"���ݾ�ȷʱ��:" << time;
		e << u8"�����ļ���С:" << fs << "(" << fsb << " Byte)";
		f << u8"��������ļ���:" << opfn;
		g << u8"����Ŀ¼:" << path;
		h << u8"���α���ִ����:" << _operator;
		i << u8"�˴α����ļ�MD5ֵ:" << md5;
		// д���������Ϣ
		form["type"] = Json::Value("custom_from");
		form["title"] = Json::Value(a.str());
		// д���label
		label::a["type"] = Json::Value("label");
		label::a["text"] = Json::Value(u8"���ݴ浵��ϸ��Ϣ");
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
		// д���toggle
		toggle::a["type"] = Json::Value("toggle");
		toggle::a["default"] = Json::Value(false);
		toggle::a["text"] = Json::Value(u8"�ص����Ƿ��Զ�����");
		// ���ص�content
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
// ��ȡ��������
int getQuantity()
{
	Json::Value root;
	Json::Reader reader;
	BACKUPJSON = getjson(".\\QuickBackup\\Backup.json");
	reader.parse(BACKUPJSON, root);
	return root["Quantity"].asInt();
}
// BACKUP/FORM JSONɾ��
void delJsons(int sn);
// ��ȡ���б���
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
	o   << u8"-����ʱ��:" << a 
		<< u8"\n-�ļ���С:" << d << u8"(" << c << u8" Byte)" 
		<< u8"\n-����ִ����:" << b 
		<< u8"\n-�����ļ�MD5ֵ:" << e << endl;
	return o.str();
}
/* *********************************  ��Ҫ���ܺ���  *********************************** */
// ������б���(�ͻ���)
void listBackupsC(string PlayerName)
{
	Json::Value root = getAllBackups();
	ostringstream w;
	string s, a, b, c, d, e;
	int q = getQuantity();
	w << u8"��b��ǰ���� " << q << u8" ������" << endl;
	sendText(PlayerName,w.str());
	for (int i = 1; i <= q; i++)
	{
		ostringstream oo, o; o << i;
		string s = o.str();
		Json::Value m = root[s];
		string output = getListStr(m);
		oo << u8"�������[" << i << "]";
		sendText(PlayerName, oo.str());
		sendText(PlayerName, output);
	}
}
// ������б���(�����)
void listBackupsS()
{
	Json::Value root = getAllBackups();
	//Json::StyledWriter w;cout << w.write(root);
	int q = getQuantity();
	cout << u8"��ǰ���� " << q << u8" ������" << endl;
	for (int i = 1; i <= q; i++)
	{
		ostringstream o;o << i;
		string s = o.str();
		Json::Value m = root[s];
		string output = getListStr(m);
		cout << u8"�������[" << i << "]\n";
		cout << output;
	}
}
// �޸�
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
bool RunBackup(string _operator)
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
		writeJsons(bp.str(), _opfn, _operator);
		return true;
	}
	else
	{
		PR(2, u8"Backup Fail. ����ʧ��");
		return false;
	}
}
/* *********************************  ��ϷSYMHOOK  *********************************** */
//������ע��->Line 7
SYMHOOK(FakeCommandReg, void, "?setup@ChangeSettingCommand@@SAXAEAVCommandRegistry@@@Z", CommandRegistry* _this)
{
	_this->registerCommand("qb_reload", u8"����QuickBackup���", 0, { 0 }, { 0x40 }); 
	_this->registerCommand("qb_backup", u8"ִ��һ�α���", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_list", u8"�г����б���", 0, { 0 }, { 0x40 });//
	_this->registerCommand("qb_help", u8"��ʾqb�������", 0, { 0 }, { 0x40 });
	_this->registerCommand("qb_backgui", u8"�򿪻ص�GUI", 0, { 0 }, { 0x40 });//
	_this->registerCommand("qb_gui", u8"�򿪲��GUI", 0, { 0 }, { 0x40 });//
	_this->registerCommand("qb_reload", u8"���ر����", 0, { 0 }, { 0x40 });
	original(_this);
}
//GetXuid
SYMHOOK(GetXuid, Player*, "??0Player@@QEAA@AEAVLevel@@AEAVPacketSender@@W4GameType@@AEBVNetworkIdentifier@@EVUUID@mce@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$unique_ptr@VCertificate@@U?$default_delete@VCertificate@@@std@@@8@55@Z", Player* _this, VA level, __int64 a3, int a4, __int64 a5, __int64 a6, void* xuid, std::string& strxuid, __int64* a9, __int64 a10, __int64 a11) {
	pxuid_level = level;
	return original(_this, level, a3, a4, a5, a6, xuid, strxuid, a9, a10, a11);
}
//��̨�����
SYMHOOK(ServerCommand, bool, "??$inner_enqueue@$0A@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@?$SPSCQueue@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$0CAA@@@AEAA_NAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z", VA _this, std::string* cmd)
{
	string cmdstr = *cmd;
	logfile.open("QuickBackup\\qb.log");
	if (cmdstr == "qb_backup")
	{
		bool v1 = RunBackup("Server");
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
	if (cmdstr == "qb_list")
	{
		listBackupsS();
		return false;
	}
	if (cmdstr == "qb" || cmdstr == "qb_" || cmdstr == "qb_help" || cmdstr == "qb help" || cmdstr == "help qb")
	{
		cout << u8"QuickBackup ���������\nqb_backup:����һ�α���\nqb_reload:���ز��\nqb_fix:�����޸�����,����Bandzip����̨����\nqb_list:��ʾ���б���\nqb_back <���>:�ص�<���>����浵";
		return false;
	}
	logfile.close();
	original(_this, cmd);
}
//��������
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
			sendText(playername, u8"��b[QuickBackup] ��a���ݳɹ�");
			cout << "[" << getTime() << "] " << "��� " << playername << " ִ����һ�α���(�ɹ�) �����ļ���" << opp << _opfn << endl;
			logfile << "[" << getTime() << "] " << "��� " << playername << " ִ����һ�α���(�ɹ�) �����ļ���" << opp << _opfn << endl;
		}
		else
		{
			sendText(playername, u8"��b[QuickBackup] ��c����ʧ��,��ʹ��/qb_reload������");
			cout << "[" << getTime() << "] " << "��� " << playername << " ִ����һ�α���(ʧ��)" << endl;
			logfile << "[" << getTime() << "] " << "��� " << playername << " ִ����һ�α���(ʧ��)" << endl;
		}
		return;
	}
	if (cmd == "/qb_reload")
	{
		bool v2 = ReloadConfig();
		if (v2)
		{
			sendText(playername, u8"��b[QuickBackup] ��a���سɹ�");
			cout << "[" << getTime() << "] " << "��� " << playername << " ִ����һ������(�ɹ�)" << endl;
			logfile << "[" << getTime() << "] " << "��� " << playername << " ִ����һ������(�ɹ�)" << endl;
		}
		return;
	}
	if (cmd == "qb" || cmd == "qb_" || cmd == "qb_help" || cmd == "qb help" || cmd == "help qb")
	{
		sendText(playername, u8"��cQuickBackup �ͻ�������\n��6qb_backup:����һ�α���\n��eqb_reload:���ز��\n��aqb_list:��ʾ���б���\n��3qb_backgui:��ʾ�ص�GUI\n��bqb_gui:��ʾ���GUI");
		return;
	}
	logfile.close();
	original(_this, id, crp);
}
SYMHOOK(GetCommandQueue, VA, "??0?$SPSCQueue@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$0CAA@@@QEAA@_K@Z", VA _this) {
	p_spscqueue = original(_this);
	return p_spscqueue;
}
/* ȡ��ѡ�����
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
