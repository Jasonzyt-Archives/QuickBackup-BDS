#pragma once
#include "Ԥ����ͷ.h"

/*
struct CommandRequestPacket {
	// ȡ�����ı�
	std::string toString() {			// IDA ServerNetworkHandler::handle
		std::string str = std::string(*(std::string*)((VA)this + 40));
		return str;
	}
};

struct ModalFormResponsePacket {
	//ȡ�����ID
	UINT getFormId() {
		return *(UINT*)((VA)this + 40);
	}

	// ȡѡ�����
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