// pch.cpp: ��Ԥ�����ͷ��Ӧ��Դ�ļ�

#include "Ԥ����ͷ.h"

#pragma comment(lib, "./Detours/lib.X64/detours.lib")

// ��ʹ��Ԥ�����ͷʱ����Ҫʹ�ô�Դ�ļ���������ܳɹ���

void init();
void exit();

// DLLģ���������˳�ʱ�����
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		init();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		exit();
		break;
	}
	return TRUE;
}

