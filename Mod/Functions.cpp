#include "预编译头.h"
#include <io.h>
#pragma warning(disable:4996)

using namespace std;

/* *********************************  系统时间处理区  *********************************** */
string getTime()
{
    time_t timep;
    time(&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));
    return tmp;
}

string getCustomTime(const char *Format)
{
    time_t timep;
    time(&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), Format, localtime(&timep));
    return tmp;
}

string getMinute()
{
    time_t timep;
    time(&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d-%H-%M", localtime(&timep));
    return tmp;
}

string editZIPFilename(string p5)//
{
    time_t timepp;
    time(&timepp);
    char tmp[64];
    strftime(tmp, sizeof(tmp), p5.c_str(), localtime(&timepp));
    return tmp;
}

/* *********************************  大杂烩区  *********************************** */
/*string getCmdStr(string p1, string p2, string p3)
{
    ostringstream v2;
    拼合字符串 根据需求修改此部分为7-Zip.exe或bandzip.exe
    *Bandzip
        v2 << "[Bandzip目录] c -y -r -fmt:zip \"" << p1 << p3 << "\" " << "\"" << p2 << "\" ";
    *7-Zip
        v2 << "[7-Zip目录] a \"" << p1 << p3 << "\" " << "\"" << p2 << "\" -y -r";
    

    Splicing string is modified to 7-Zip.exe or bandzip.exe
    *Bandzip
        v2 << "[Bandzip Folder] c -y -r -fmt:zip \"" << p1 << p3 << "\" " << "\"" << p2 << "\" ";
    *7-Zip
        v2 << "[7-Zip Folder] a \"" << p1 << p3 << "\" " << "\"" << p2 << "\" -y -r";
    
    v2 << "QuickBackup\\Bandzip\\bz.exe c -y -r -fmt:zip \"" << p1 << p3 << "\" " << "\"" << p2 << "\" ";
    string v3 = v2.str();
    return v3;
}*/

string getConfig(string configfile, string key, string defaultvaule)
{
    Config conf(configfile);
    string r1 = conf.Read(key, defaultvaule/*默认值*/);
    return r1;
}

/* *********************************  文件(夹)处理区  *********************************** */
bool findFile(string filename)
{
    ifstream f(filename.c_str());
    return f.good();
}

bool mkdir(string dirname)
{
    ostringstream v10;
    v10 << "mkdir " << dirname;
    string v11 = v10.str();
    system(v11.c_str());
    return true;
}

bool DownloadFile(LPCWSTR url, LPCWSTR downloadPath)
{
    HRESULT Result = URLDownloadToFile(NULL, url, downloadPath , NULL, NULL);
    switch (Result)
    {
    case S_OK:return true; break;
    default:return false;
    }
}

bool FolderExists(const wchar_t* path)//
{
    DWORD dwAttribute = GetFileAttributes(path);
    if (dwAttribute == 0XFFFFFFFF) return false; 
    else return true;
}

int getFiles()
{
    bool v1 = false;
    bool v2 = false;
    if (findFile(".\\QuickBackup\\config.ini"))
    {
		v1 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/config.ini", L".\\QuickBackup\\config.new.ini");
    }
	else
		v1 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/config.ini", L".\\QuickBackup\\config.ini");
    v2 = DownloadFile(L"http://download.skytown.xyz:15434/Filedir/QuickBackup/QB_EXE_lastest.exe", L".\\QuickBackup\\qb.exe");
	if (v1 && v2)
		return 0;
    else if (v1 == false && v2 == false)
        return -1;
    else
        return 1;
}

size_t GetFileSizeByte(const std::string& file_name) 
{
	size_t size;
	FILE* file = fopen(file_name.c_str(), "rb");
	if (file)
	{
		size_t size = filelength(fileno(file));
		fclose(file);
		return size;
	}
	else
	{
		return -1;
	}
}

string GetFileSize(const std::string& file_name) 
{
	size_t size = 0;
	FILE* file = fopen(file_name.c_str(), "rb");
	if (file)
	{
		size_t size = filelength(fileno(file));
		fclose(file);
	}
	else
	{
		return "";
	}
    if (size >= 1024 && size < 1048576)
    {
        ostringstream a;
        a << size / 1024 << "KB";
        return a.str();
    }
    else if (size >= 1048576 && size < 1073741824)
    {
        ostringstream a;
        a << size / 1048576 << "MB";
        return a.str();
    }
    else if (size >=1073741824  && size < 1099511627776)
    {
        ostringstream a;
        a << size / 1073741824 << "GB";
        return a.str();
    }
    else
    {
        ostringstream a;
        a << size << "Byte";
        return a.str();
    }
}

/********************************************************************************
*
*   FUNCTION NAME: GetWorldName
*   Input        : 无
*   Retrun Value : string				读取到的地图名称
*
*---------------------------------- PURPOSE -----------------------------------
*  通过Config类读取配置文件获取地图名称
*******************************************************************************/
string GetWorldName()
{
	return getConfig("./server.properties", "level-name", "");
}

/* *********************************  字符串处理区  *********************************** */
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

/* *********************************  ZIP处理区  *********************************** */
//HZIP https://www.codeproject.com/Articles/7530/Zip-Utils-Clean-Elegant-Simple-Cplusplus-Win
//ZipFunction https://github.com/DoreRandom/Zip
//-------------------ZipFunction.h内函数用法-------------------//
//Exp1 解压到指定目录
////ExtractZipToDir("C:\\Users\\Administrator\\Desktop\\filename.zip", "C:\\Users\\Administrator\\Desktop\\foldername");																								//解压到当前目录
//Exp2 解压到当前目录
////ExtractZipToDir("C:\\Users\\Administrator\\Desktop\\filename.zip");
//Exp3 压缩到指定目录
////CompressDirToZip("C:\\Users\\Administrator\\Desktop\\filename.zip", "C:\\Users\\Administrator\\Desktop\\foldername");
//Exp4 压缩到当前目录
////CompressDirToZip("C:\\Users\\Administrator\\Desktop\\filename.zip");
//Exp5 压缩多个目录
////std::vector<std::string> paths;
////paths.push_back("C:\\Users\\Administrator\\Desktop\\foldername1");
////paths.push_back("C:\\Users\\Administrator\\Desktop\\foldername2");
////paths.push_back("C:\\Users\\Administrator\\Desktop\\foldername3");
////CompressDirToZip(paths, "c:\\users\\administrator\\desktop\\filename.zip");
//Exp6 获取zip压缩过程中的错误信息并输出
////std::cout << GetZipErrorMessage(0);

void ReturnCheckOutput(int in)
{
	switch (in)
	{
	case ZR_OK:
		PR(0, u8"压缩成功");
		break;
	case ZR_NOFILE:
		PR(2, u8"找不到需要压缩的文件");
		break;
	case ZR_NODUPH:
		PR(2, u8"无法复制句柄");
		break;
	case ZR_NOALLOC:
		PR(2, u8"未能分配某些资源");
		break;
	case ZR_WRITE:
		PR(2, u8"写入文件时出现错误");
		break;
	case ZR_NOTFOUND:
		PR(2, u8"在ZIP中找不到该文件");
		break;
	case ZR_MORE:
		PR(2, u8"还有更多数据要解压缩");
		break;
	case ZR_CORRUPT:
		PR(2, u8"压缩文件已损坏或不是ZIP格式文件");
		break;
	case ZR_READ:
		PR(2, u8"读取文件时出现常规错误");
		break;
	case ZR_PASSWORD:
		PR(2, u8"没有获得正确的密码来解压缩文件");
		break;
	case ZR_ARGS:
		PR(2, u8"参数错误");
		break;
	case ZR_NOTMMAP:
		PR(2, u8"尝试ZipGetMemory 但这只适用于MMap ZIP文件");
		break;
	case ZR_MEMSIZE:
		PR(2, u8"内存不足");
		break;
	case ZR_FAILED:
		PR(2, u8"调用此函数时该操作已失败");
		break;
	case ZR_ENDED:
		PR(2, u8"ZIP创建已关闭");
		break;
	case ZR_MISSIZE:
		PR(2, u8"显示的输入文件大小错误");
		break;
	case ZR_PARTIALUNZ:
		PR(2, u8"文件已经部分解压缩");
		break;
	case ZR_ZMODE:
		PR(0, u8"尝试混合创建/打开zip");
		break;
	default:
		PR(1, u8"未定义的标识符(这并不影响程序运行,但请注意)");
		break;
	}
}

/* *********************************  MD5处理区  *********************************** */
#ifndef HAVE_OPENSSL

#define F(x, y, z)   ((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z)   ((y) ^ ((z) & ((x) ^ (y))))
#define H(x, y, z)   ((x) ^ (y) ^ (z))
#define I(x, y, z)   ((y) ^ ((x) | ~(z)))
#define STEP(f, a, b, c, d, x, t, s) \
		(a) += f((b), (c), (d)) + (x) + (t); \
		(a) = (((a) << (s)) | (((a) & 0xffffffff) >> (32 - (s)))); \
		(a) += (b);

#if defined(__i386__) || defined(__x86_64__) || defined(__vax__)
#define SET(n) \
			(*(MD5_u32 *)&ptr[(n) * 4])
#define GET(n) \
			SET(n)
#else
#define SET(n) \
			(ctx->block[(n)] = \
			(MD5_u32)ptr[(n) * 4] | \
			((MD5_u32)ptr[(n) * 4 + 1] << 8) | \
			((MD5_u32)ptr[(n) * 4 + 2] << 16) | \
			((MD5_u32)ptr[(n) * 4 + 3] << 24))
#define GET(n) \
			(ctx->block[(n)])
#endif

typedef unsigned int MD5_u32;

typedef struct {
	MD5_u32 lo, hi;
	MD5_u32 a, b, c, d;
	unsigned char buffer[64];
	MD5_u32 block[16];
} MD5_CTX;

static void MD5_Init(MD5_CTX* ctx);
static void MD5_Update(MD5_CTX* ctx, const void* data, unsigned long size);
static void MD5_Final(unsigned char* result, MD5_CTX* ctx);

static const void* body(MD5_CTX* ctx, const void* data, unsigned long size) {
	const unsigned char* ptr;
	MD5_u32 a, b, c, d;
	MD5_u32 saved_a, saved_b, saved_c, saved_d;

	ptr = (const unsigned char*)data;

	a = ctx->a;
	b = ctx->b;
	c = ctx->c;
	d = ctx->d;

	do {
		saved_a = a;
		saved_b = b;
		saved_c = c;
		saved_d = d;

		STEP(F, a, b, c, d, SET(0), 0xd76aa478, 7)
			STEP(F, d, a, b, c, SET(1), 0xe8c7b756, 12)
			STEP(F, c, d, a, b, SET(2), 0x242070db, 17)
			STEP(F, b, c, d, a, SET(3), 0xc1bdceee, 22)
			STEP(F, a, b, c, d, SET(4), 0xf57c0faf, 7)
			STEP(F, d, a, b, c, SET(5), 0x4787c62a, 12)
			STEP(F, c, d, a, b, SET(6), 0xa8304613, 17)
			STEP(F, b, c, d, a, SET(7), 0xfd469501, 22)
			STEP(F, a, b, c, d, SET(8), 0x698098d8, 7)
			STEP(F, d, a, b, c, SET(9), 0x8b44f7af, 12)
			STEP(F, c, d, a, b, SET(10), 0xffff5bb1, 17)
			STEP(F, b, c, d, a, SET(11), 0x895cd7be, 22)
			STEP(F, a, b, c, d, SET(12), 0x6b901122, 7)
			STEP(F, d, a, b, c, SET(13), 0xfd987193, 12)
			STEP(F, c, d, a, b, SET(14), 0xa679438e, 17)
			STEP(F, b, c, d, a, SET(15), 0x49b40821, 22)
			STEP(G, a, b, c, d, GET(1), 0xf61e2562, 5)
			STEP(G, d, a, b, c, GET(6), 0xc040b340, 9)
			STEP(G, c, d, a, b, GET(11), 0x265e5a51, 14)
			STEP(G, b, c, d, a, GET(0), 0xe9b6c7aa, 20)
			STEP(G, a, b, c, d, GET(5), 0xd62f105d, 5)
			STEP(G, d, a, b, c, GET(10), 0x02441453, 9)
			STEP(G, c, d, a, b, GET(15), 0xd8a1e681, 14)
			STEP(G, b, c, d, a, GET(4), 0xe7d3fbc8, 20)
			STEP(G, a, b, c, d, GET(9), 0x21e1cde6, 5)
			STEP(G, d, a, b, c, GET(14), 0xc33707d6, 9)
			STEP(G, c, d, a, b, GET(3), 0xf4d50d87, 14)
			STEP(G, b, c, d, a, GET(8), 0x455a14ed, 20)
			STEP(G, a, b, c, d, GET(13), 0xa9e3e905, 5)
			STEP(G, d, a, b, c, GET(2), 0xfcefa3f8, 9)
			STEP(G, c, d, a, b, GET(7), 0x676f02d9, 14)
			STEP(G, b, c, d, a, GET(12), 0x8d2a4c8a, 20)
			STEP(H, a, b, c, d, GET(5), 0xfffa3942, 4)
			STEP(H, d, a, b, c, GET(8), 0x8771f681, 11)
			STEP(H, c, d, a, b, GET(11), 0x6d9d6122, 16)
			STEP(H, b, c, d, a, GET(14), 0xfde5380c, 23)
			STEP(H, a, b, c, d, GET(1), 0xa4beea44, 4)
			STEP(H, d, a, b, c, GET(4), 0x4bdecfa9, 11)
			STEP(H, c, d, a, b, GET(7), 0xf6bb4b60, 16)
			STEP(H, b, c, d, a, GET(10), 0xbebfbc70, 23)
			STEP(H, a, b, c, d, GET(13), 0x289b7ec6, 4)
			STEP(H, d, a, b, c, GET(0), 0xeaa127fa, 11)
			STEP(H, c, d, a, b, GET(3), 0xd4ef3085, 16)
			STEP(H, b, c, d, a, GET(6), 0x04881d05, 23)
			STEP(H, a, b, c, d, GET(9), 0xd9d4d039, 4)
			STEP(H, d, a, b, c, GET(12), 0xe6db99e5, 11)
			STEP(H, c, d, a, b, GET(15), 0x1fa27cf8, 16)
			STEP(H, b, c, d, a, GET(2), 0xc4ac5665, 23)
			STEP(I, a, b, c, d, GET(0), 0xf4292244, 6)
			STEP(I, d, a, b, c, GET(7), 0x432aff97, 10)
			STEP(I, c, d, a, b, GET(14), 0xab9423a7, 15)
			STEP(I, b, c, d, a, GET(5), 0xfc93a039, 21)
			STEP(I, a, b, c, d, GET(12), 0x655b59c3, 6)
			STEP(I, d, a, b, c, GET(3), 0x8f0ccc92, 10)
			STEP(I, c, d, a, b, GET(10), 0xffeff47d, 15)
			STEP(I, b, c, d, a, GET(1), 0x85845dd1, 21)
			STEP(I, a, b, c, d, GET(8), 0x6fa87e4f, 6)
			STEP(I, d, a, b, c, GET(15), 0xfe2ce6e0, 10)
			STEP(I, c, d, a, b, GET(6), 0xa3014314, 15)
			STEP(I, b, c, d, a, GET(13), 0x4e0811a1, 21)
			STEP(I, a, b, c, d, GET(4), 0xf7537e82, 6)
			STEP(I, d, a, b, c, GET(11), 0xbd3af235, 10)
			STEP(I, c, d, a, b, GET(2), 0x2ad7d2bb, 15)
			STEP(I, b, c, d, a, GET(9), 0xeb86d391, 21)

			a += saved_a;
		b += saved_b;
		c += saved_c;
		d += saved_d;

		ptr += 64;
	} while (size -= 64);

	ctx->a = a;
	ctx->b = b;
	ctx->c = c;
	ctx->d = d;

	return ptr;
}

void MD5_Init(MD5_CTX* ctx) {
	ctx->a = 0x67452301;
	ctx->b = 0xefcdab89;
	ctx->c = 0x98badcfe;
	ctx->d = 0x10325476;

	ctx->lo = 0;
	ctx->hi = 0;
}

void MD5_Update(MD5_CTX* ctx, const void* data, unsigned long size) {
	MD5_u32 saved_lo;
	unsigned long used, free;

	saved_lo = ctx->lo;
	if ((ctx->lo = (saved_lo + size) & 0x1fffffff) < saved_lo)
		ctx->hi++;
	ctx->hi += size >> 29;
	used = saved_lo & 0x3f;

	if (used) {
		free = 64 - used;
		if (size < free) {
			memcpy(&ctx->buffer[used], data, size);
			return;
		}

		memcpy(&ctx->buffer[used], data, free);
		data = (unsigned char*)data + free;
		size -= free;
		body(ctx, ctx->buffer, 64);
	}

	if (size >= 64) {
		data = body(ctx, data, size & ~(unsigned long)0x3f);
		size &= 0x3f;
	}

	memcpy(ctx->buffer, data, size);
}

void MD5_Final(unsigned char* result, MD5_CTX* ctx) {
	unsigned long used, free;
	used = ctx->lo & 0x3f;
	ctx->buffer[used++] = 0x80;
	free = 64 - used;

	if (free < 8) {
		memset(&ctx->buffer[used], 0, free);
		body(ctx, ctx->buffer, 64);
		used = 0;
		free = 64;
	}

	memset(&ctx->buffer[used], 0, free - 8);

	ctx->lo <<= 3;
	ctx->buffer[56] = ctx->lo;
	ctx->buffer[57] = ctx->lo >> 8;
	ctx->buffer[58] = ctx->lo >> 16;
	ctx->buffer[59] = ctx->lo >> 24;
	ctx->buffer[60] = ctx->hi;
	ctx->buffer[61] = ctx->hi >> 8;
	ctx->buffer[62] = ctx->hi >> 16;
	ctx->buffer[63] = ctx->hi >> 24;
	body(ctx, ctx->buffer, 64);
	result[0] = ctx->a;
	result[1] = ctx->a >> 8;
	result[2] = ctx->a >> 16;
	result[3] = ctx->a >> 24;
	result[4] = ctx->b;
	result[5] = ctx->b >> 8;
	result[6] = ctx->b >> 16;
	result[7] = ctx->b >> 24;
	result[8] = ctx->c;
	result[9] = ctx->c >> 8;
	result[10] = ctx->c >> 16;
	result[11] = ctx->c >> 24;
	result[12] = ctx->d;
	result[13] = ctx->d >> 8;
	result[14] = ctx->d >> 16;
	result[15] = ctx->d >> 24;
	memset(ctx, 0, sizeof(*ctx));
}
#else
#include <openssl/md5.h>
#endif

/* Return Calculated raw result(always little-endian), the size is always 16 */
void md5bin(const void* dat, size_t len, unsigned char out[16]) {
	MD5_CTX c;
	MD5_Init(&c);
	MD5_Update(&c, dat, (unsigned long)len);
	MD5_Final(out, &c);
}

static char hb2hex(unsigned char hb) {
	hb = hb & 0xF;
	return hb < 10 ? '0' + hb : hb - 10 + 'a';
}

string md5file(const char* filename) {
	//FILE* file;
	//fopen_s(&file, filename, "rb");
	// res = md5file(file);
	//std::fclose(file);
	return string();
}

string md5file(std::FILE* file) {

	MD5_CTX c;
	MD5_Init(&c);

	char buff[BUFSIZ];
	unsigned char out[16];
	size_t len = 0;
	while ((len = std::fread(buff, sizeof(char), BUFSIZ, file)) > 0) {
		MD5_Update(&c, buff, (unsigned long)len);
	}
	MD5_Final(out, &c);

	string res;
	for (size_t i = 0; i < 16; ++i) {
		res.push_back(hb2hex(out[i] >> 4));
		res.push_back(hb2hex(out[i]));
	}
	return res;
}

string md5(const void* dat, size_t len) {
	string res;
	unsigned char out[16];
	md5bin(dat, len, out);
	for (size_t i = 0; i < 16; ++i) {
		res.push_back(hb2hex(out[i] >> 4));
		res.push_back(hb2hex(out[i]));
	}
	return res;
}

std::string md5(std::string dat) {
	return md5(dat.c_str(), dat.length());
}

/* Generate shorter md5sum by something like base62 instead of base16 or base10. 0~61 are represented by 0-9a-zA-Z */
string md5sum6(const void* dat, size_t len) {
	static const char* tbl = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	string res;
	unsigned char out[16];
	md5bin(dat, len, out);
	for (size_t i = 0; i < 6; ++i) {
		res.push_back(tbl[out[i] % 62]);
	}
	return res;
}

std::string md5sum6(std::string dat) {
	return md5sum6(dat.c_str(), dat.length());
}

bool CheckFileMD5(string filename, string inputMD5)
{
	FILE* fp = fopen(filename.c_str(), "wb");
	string md5 = md5file(fp);
	fclose(fp);
	if (md5 == inputMD5)
		return true;
	else
		return false;
}

string GetFileMD5(string filename)
{
	FILE* fp = fopen(filename.c_str(), "wb");
	if (fp)
	{
		string md5 = md5file(fp);
		fclose(fp);
		return md5;
	}
	else
		return "";
}