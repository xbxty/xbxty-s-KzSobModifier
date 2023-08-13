// utils.h
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <Windows.h>
#include <ShlObj.h>
#include <string>
namespace utils 
{
	bool GetKnownFolderWrapper(std::string& out, REFKNOWNFOLDERID rfid);//获取Local文件夹的位置，不能再细了。
	bool GetSteamId(int& out);//从注册表里直接获取steam id
}

#endif // UTILS_H