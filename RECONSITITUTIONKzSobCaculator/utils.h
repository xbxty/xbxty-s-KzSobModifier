// utils.h
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <Windows.h>
#include <ShlObj.h>
#include <string>
namespace utils 
{
	bool GetKnownFolderWrapper(std::string& out, REFKNOWNFOLDERID rfid);//��ȡLocal�ļ��е�λ�ã�������ϸ�ˡ�
	bool GetSteamId(int& out);//��ע�����ֱ�ӻ�ȡsteam id
}

#endif // UTILS_H