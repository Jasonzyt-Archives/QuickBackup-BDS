/*#pragma once
#include "Ԥ����ͷ.h"
using VA = unsigned __int64;
using RVA = unsigned int;

//�ṹ������
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
	// ȡuuid�ַ���
	std::string toString();
};

// �������ṹ��
struct Vec3 {
	float x;
	float y;
	float z;
	std::string toJsonString();
};

struct Actor {
	// ��ȡ����������Ϣ
	std::string getNameTag();
	// ��ȡ���ﵱǰ����ά��ID
	int getDimensionId();
	// ��ȡ���ﵱǰ��������
	Vec3* getPos();
	// ��ȡ��������
	std::string getTypeName();
};

struct PlayerActor {
	// ȡuuid
	MCUUID* getUuid();
	std::string getXuid(VA level);
};
*/