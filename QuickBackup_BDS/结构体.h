/*#pragma once
#include "预编译头.h"
using VA = unsigned __int64;
using RVA = unsigned int;

//结构体声明
struct CommandRequestPacket 
{
	std::string toString();
};

struct ModalFormResponsePacket 
{
	UINT getFormId();
	std::string getSelectStr();
};

struct CommandRegistry
{
	void registerCommand(string const& a, char const* b, char c, char d, char e);
};

struct MCUUID {
	// 取uuid字符串
	std::string toString();
};

// 玩家坐标结构体
struct Vec3 {
	float x;
	float y;
	float z;
	std::string toJsonString();
};

struct Actor {
	// 获取生物名称信息
	std::string getNameTag();
	// 获取生物当前所处维度ID
	int getDimensionId();
	// 获取生物当前所在坐标
	Vec3* getPos();
	// 获取生物类型
	std::string getTypeName();
};

struct PlayerActor {
	// 取uuid
	MCUUID* getUuid();
	std::string getXuid(VA level);
};
*/