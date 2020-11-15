#pragma once
#define WIN32_LEAN_AND_MEAN
// 当前版本
#define QBVERSION "3.1.0"
// Windows 头文件
#include <windows.h>
// C++标准库 头文件
#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <queue>
#include <chrono>
#include <map>
#include <set>
#include <utility>
#include <urlmon.h>
#include <tchar.h>
#include <direct.h>
#include <thread>
#include <sys/stat.h>
#include <tchar.h>
#include <wincrypt.h>
#include <cstring>
// 项目 头文件
#include "Functions.h"
#include "getConfig.h"
// JsonCPP 库 头文件
#include "json/forwards.h"
#include "json/writer.h"
#include "json/value.h"
#include "json/json.h"
#include "json/reader.h"


using VA = unsigned __int64;
using RVA = unsigned int;
template<typename Type>
using Ptr = Type*;

extern "C" {
	_declspec(dllimport) int HookFunction(void* oldfunc, void** poutold, void* newfunc);
	_declspec(dllimport) void* GetServerSymbol(char const* name);
}
template<typename ret, typename... Args>
inline ret SYMCALL(const char* fn, Args... args) {
	return ((ret(*)(Args...))GetServerSymbol(fn))(args...);
}
struct HookRegister {
	HookRegister(void* address, void* hook, void** org) {
		if (HookFunction(address, org, hook) != 0) printf("FailedToHook: %p\n", address);
	}
	HookRegister(char const* sym, void* hook, void** org) {
		auto found = GetServerSymbol(sym);
		if (found == nullptr) printf("FailedToHook: %p\n", sym);
		if (HookFunction(found, org, hook)!= 0) printf("FailedToHook: %s\n", sym);
	}
};
#define SYMHOOK(name,ret,sym,...)				\
struct name {									\
	typedef ret(*original_type)(__VA_ARGS__);	\
	static original_type& _original() {			\
		static original_type storage;			\
		return storage;							\
	}											\
	template <typename... T>					\
	static ret original(T&&... params) {		\
		return _original()(std::forward<T>(params)...);\
	}											\
	static ret _hook(__VA_ARGS__);				\
};												\
HookRegister name{sym,&name::_hook,(void**)&name::_original()};\
ret name::_hook(__VA_ARGS__)

// PR(0, u8"标准流输出"<<u8"!!!")
#define PR(type, ...)\
	if (type == 0)\
		std::cout << "[" << getTime() << " INFO][QuickBackup] " << __VA_ARGS__ << endl;\
	else if (type == 1)\
		std::cout << "[" << getTime() << " WARN][QuickBackup] " << __VA_ARGS__ << endl;\
	else if (type == 2)\
		std::cout << "[" << getTime() << " ERROR][QuickBackup] " << __VA_ARGS__ << endl;\
	else if (type == 3)\
		std::cout << "[" << getTime() << " DEBUG][QuickBackup] " << __VA_ARGS__ << endl;\
	else\
		std::cout << __VA_ARGS__ << endl

//#define CS(...) std::ostringstream v;v << __VA_ARGS__;std::string ret = v.str();(ret)
// 获取文件MD5值
//#define GETFILEMD5(filename,buffer) TCHAR fstr[MAX_PATH] = _T(filename);GetFileMd5(fstr, buffer);
