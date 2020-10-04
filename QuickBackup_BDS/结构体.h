#pragma once
#include "预编译头.h"

/*
struct CommandRequestPacket {
	// 取命令文本
	std::string toString() {			// IDA ServerNetworkHandler::handle
		std::string str = std::string(*(std::string*)((VA)this + 40));
		return str;
	}
};

struct ModalFormResponsePacket {
	//取发起表单ID
	UINT getFormId() {
		return *(UINT*)((VA)this + 40);
	}

	// 取选择序号
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
*/