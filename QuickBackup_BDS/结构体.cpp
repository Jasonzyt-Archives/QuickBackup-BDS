/*#include "结构体.h"
#include "预编译头.h"

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
		SYMCALL(void, MSSYM_MD5_8574de98358ff66b5a913417f44dd706, this, a, b, c, d, e);
	}
};
// 取uuid
struct Player : Actor {
	MCUUID* getUuid() {	
		return (MCUUID*)((VA)this + 2720);
	}

	std::string getXuid(VA level) {
		return SYMCALL(std::string&, MSSYM_MD5_337bfad553c289ba4656ac43dcb60748,level, (VA)this + 2720);
	}
};

struct Actor {
	// 获取生物名称信息
	std::string getNameTag() {
		return SYMCALL(std::string&,MSSYM_MD5_7044ab83168b0fd345329e6566fd47fd,this);
	}
	// 获取生物当前所处维度ID
	int getDimensionId() {
		int dimensionId;
		SYMCALL(int&,MSSYM_B1QE14getDimensionIdB1AA5ActorB2AAA4UEBAB1QA2AVB2QDE11AutomaticIDB1AE10VDimensionB2AAA1HB2AAA2XZ,this, &dimensionId);
		return dimensionId;
	}


	// 获取生物当前所在坐标
	Vec3* getPos() {
		return SYMCALL(Vec3*,MSSYM_B1QA6getPosB1AA5ActorB2AAE12UEBAAEBVVec3B2AAA2XZ, this);
	}

	// 获取生物类型
	std::string getTypeName() {
		std::string actor_typename;
		SYMCALL(std::string&,MSSYM_MD5_01064f7d893d9f9ef50acf1f931d1d79,&actor_typename, this);
		return actor_typename;
	}
};
struct MCUUID {
	// 取uuid字符串
	std::string toString() {
		std::string s;
		SYMCALL(std::string&, MSSYM_MD5_40e8abf6eb08f7ee446159cdd0a7f283, this, &s);
		return s;
	}
};

// 玩家坐标结构体
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
*/