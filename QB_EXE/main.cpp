#include <Windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include "../Mod/HZip/zip.h"
#include "../Mod/HZip/unzip.h"
#include "../Mod/HZip/ZipDefine.h"
#define EXEVERSION "1.0.5"
// ����ϵͳ����
#define RUNSYSTEMCMD(name,...)\
	ostringstream name;\
	name << __VA_ARGS__;\
	system(name.str().data());

#define PAUSE system("pause")

#define PRHELP std::cout << "qb.exe back <String:BackupFilePath> <Bool:AutoStart> <Bool:DelMap>" << endl\
						 << "qb.exe update\nqb.exe help" << endl\
					 	 << "qb.exe zip <String:InputType> <Bool:IsDelSrc> <String:Output> <String:Input>" << endl\
						 << "qb.exe unzip <String:ZipFilePath> <String:OutputDir>" << endl\
						 << "For more help, use \"qb.exe zip/unzip/... help\" eg:qb.exe zip help" << endl

#define ZIPHELP std::cout<< "qb.exe zip <String:InputType> <Bool:IsDelSrc> <String:Output> <String:Input>" << endl\
						 << "<String:InputType> ��������        : dir/file" << endl\
						 << "<Bool:IsDelSrc> �Ƿ�ɾ��Դ�ļ�(��) : true/false(1/0)" << endl\
						 << "<String:Output> ѹ���ļ����·��   : [PATH]" << endl\
						 << "<String:Input> �����ļ�(��)        : [PATH]" << endl\
						 << "eg:qb.exe zip file true C:/Users/QwQ/Desktop/test.zip ./tttttt.txt" << endl

using namespace std;

wchar_t* CharToWChar(const char* str)
{
	wchar_t* buffer = NULL;
	if (str)
	{
		size_t nu = strlen(str);
		size_t n = (size_t)MultiByteToWideChar(ZIP_UNICODE, 0, (const char*)str, int(nu), NULL, 0);
		buffer = 0;
		buffer = new wchar_t[n + 1];
		MultiByteToWideChar(ZIP_UNICODE, 0, (const char*)str, int(nu), buffer, int(n));
		buffer[n] = 0;
	}
	return buffer;
}

// ��ע��SrcDirӦ����[Ŀ¼��]\\[�ļ���(��ʹ��ͨ���)] StrSrcDirӦ����[Ŀ¼��]
// eg : ListFiles(".\\awa\\*.*",".\\awa");
vector<string> ListFiles(LPCSTR SrcDir, string StrSrcDir)
{
	vector<string> ret = {};
	HANDLE hFind;
	WIN32_FIND_DATAA findData;
	//LARGE_INTEGER size;
	hFind = FindFirstFileA(SrcDir, &findData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		cout << "�ݹ�����ļ�ʧ��\n";
		return ret;
	}
	//cout << "�ݹ鿪ʼ" << endl;
	do
	{
		// ����"."��".."������� 
		if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0)
			continue;
		// �����޷�ɾ����lost�ļ���
		if (strcmp(findData.cFileName, "lost") == 0)
			continue;
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)    // �Ƿ���Ŀ¼ 
		{
			ostringstream StrSrc, Src;
			StrSrc << StrSrcDir << "\\" << findData.cFileName;
			Src << StrSrcDir << "\\" << findData.cFileName << "\\*.*";
			vector<string> gret = ListFiles(Src.str().c_str(),StrSrc.str());
			ret.insert(ret.end(), gret.begin(), gret.end());
			Src.clear();
			StrSrc.clear();
		}
		else
		{
			ostringstream StrSrc;
			StrSrc << StrSrcDir << "\\" << findData.cFileName;
			ret.push_back(StrSrc.str());
			cout << StrSrc.str() << endl;
			StrSrc.clear();
		}
	} while (FindNextFileA(hFind, &findData));
	//cout << "�ݹ�����ļ��ɹ�\n";
	return ret;
}

string GetWorldName()
{
	string ret;
	fstream ln;
	ln.open("./tmp/levelname.txt");
	getline(ln, ret);
	ln.close();
	return ret;
}

ZRESULT CompressDir(vector<string> SrcFileList, string OutputZip)
{
	int i = 0;
	HZIP hz;
	hz = CreateZip(CharToWChar(OutputZip.c_str()), 0);
	do
	{
		string zfn = SrcFileList[i];
		zfn.erase(0, 1);
		ZipAdd(hz, CharToWChar(zfn.c_str()), CharToWChar(SrcFileList[i].c_str()));
		i++;
	} while (SrcFileList[i] != "");
	return CloseZip(hz);
}

bool findFile(string filename)
{
	ifstream f(filename.c_str());
	return f.good();
}
string StrLower(string in)
{
	transform(in.begin(), in.end(), in.begin(), ::tolower);
	return in;
}
int main(int argc, char* argv[])
{
	vector<string> p;
	for (int i = 0; i < argc; i++)
	{
		p.push_back(StrLower(argv[i]));
	}
	cout << "QuickBackup - qb.exe v" << EXEVERSION << " By JasonZYT (https://github.com/Jasonzyt/QuickBackup-BDS)" << endl;
	cout << "Get parameter list successfully : ";
	for (int i = 0; i < argc; i++)
	{
		cout << argv[i] << " ";
	}
	cout << endl << endl;
	if (argc == 1 || p[1] == "help")
	{
		PRHELP;
	}
	else if (p[1] != "zip" && p[1] != "unzip" && p[1] != "back" && p[1] != "update" && p[1] != "backup")
	{
		cout << "Bad command!!! Unknown command:" << argv[1] << endl;
		PRHELP;
	}
	else if (p[1] == "zip")
	{
		bool b;
		if (p[2] == "help")
		{
			ZIPHELP;
		}
		else if (argc <= 5)
		{
			cout << "��������5��" << endl;
			ZIPHELP;
		}
		else
		{
			if (p[2] != "dir" && p[2] != "file")
			{
				cout << "<InputType>��ֵ���Ϸ�:ֻ����ʹ��dir��file" << endl;
				ZIPHELP;
			}
			if (p[3] == "true" || p[3] == "1")
				b = true;
			else if (p[3] == "false" || p[3] == "0")
				b = false;
			else
			{
				b = false;
				cout << "<IsDelSrc>��ֵ���Ϸ�:��Ĭ��Ϊfalse" << endl;
				ZIPHELP;
			}
			if (p[2] == "file")
			{
				HZIP hz;
				string outfp = argv[4];
				hz = CreateZip(CharToWChar(outfp.c_str()), 0);
				for (int i = 5; i < argc; i++)
				{
					if (!findFile(argv[i]))
						cout << "�ļ�\"" << argv[i] << "\"������" << endl; continue;
					string infn = argv[i];
					int pos = infn.find_last_of('\\');
					if (pos == string::npos)
					{
						pos = infn.find_last_of('/');
						if (pos == string::npos)
						{
							cout << "�ļ�\"" << infn << "\"���벻��ȷ" << endl;
							continue;
						}
					}
					string fn(infn.substr(pos + 1));
					cout << fn;
					ZipAdd(hz, CharToWChar(fn.c_str()), CharToWChar(argv[i]));
					cout << argv[i] << endl;
				}
				if (CloseZip(hz) == 0)
					cout << "ѹ���ɹ�";
				else
					cout << "ѹ��ʧ��";
			}
		}
	}
	else if (p[1] == "backup")
	{
		string levelname = GetWorldName();
		cout << "World-Name : " << levelname << endl;
		vector<string> fl = ListFiles(".\\tmp\\*.*", ".\\tmp");
		int i = 0;
		HZIP hz;
		hz = CreateZip(CharToWChar(argv[2]), 0);
		do
		{
			ostringstream o;
			string zfn = fl[i];
			zfn.erase(0, 5);
			o << levelname << zfn;
			ZipAdd(hz,CharToWChar(o.str().c_str()) ,CharToWChar(fl[i].c_str()));
			DeleteFile(CharToWChar(fl[i].c_str()));
			i++;
		} while (fl[i] != "");
		CloseZip(hz);
		//HZIP hz;
		//hz = CreateZip(CharToWChar(argv[2]), 0);
		//ZipAddFolder(hz, CharToWChar("./tmp/"));
		//ZipAddFolder(hz, CharToWChar("./tmp"), CharToWChar(argv[2]));
		//CompressDirToZip("./tmp", argv[2]);
		//CompressDirToZip("./Bandzip", "./debugg.zip");
		//system("del /S /Q tmp");
		//int ret = CloseZip(hz);
		//cout << ret;
	}
	//PAUSE;
	return 0;
}