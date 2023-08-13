
#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<fstream>
#include<Windows.h>
#include<iomanip>
#include "ReadSobFile.h"
#include "SUM.h"
#include "SHLOBJ.H"

using namespace std;

//酱圣的遗产
bool GetSteamId(int& out) {
	// Define the registry key and value names
	const char* keyName = "SOFTWARE\\Valve\\Steam\\ActiveProcess";
	const char* valueName = "ActiveUser";

	// Define the registry data type and size
	DWORD dwType = REG_DWORD;
	DWORD dwSize = sizeof(DWORD);

	// Define a variable to store the registry value
	DWORD dwValue;

	// Open the registry key
	HKEY hKey;
	LSTATUS lsRet = RegOpenKeyExA(HKEY_CURRENT_USER, keyName, 0, KEY_READ, &hKey);

	// Check if the key was opened successfully
	if (lsRet != ERROR_SUCCESS) {
		// Failed to open the key, return false
		return false;
	}

	// Read the registry value
	lsRet = RegGetValueA(hKey, NULL, valueName, RRF_RT_REG_DWORD, &dwType, &dwValue, &dwSize);

	// Close the registry key
	RegCloseKey(hKey);

	// Check if the value was read successfully
	if (lsRet != ERROR_SUCCESS) {
		// Failed to read the value, return false
		return false;
	}

	// Store the value in the out parameter
	out = dwValue;

	// Return true to indicate success
	return true;
}

//将时间转化为以秒为单位
string ChangeTimeIntoSeconds(Time _time) {
	string ansStr;
	int s = 0;
	int ms = 0;
	s = stoi(_time.h) * 3600 + stoi(_time.m) * 60 + stoi(_time.s);
	ms = stoi(_time.ms) / 10;
	ansStr = to_string(s) + "." + to_string(ms);
	return ansStr;
}

//检测输入的时间是否合法
bool JudgeCinTime(string _time) {
	int cnt = 0;  //小数点数量
	int end_pos = _time.size() - 1;  //字符串结尾位置
	for (int i = _time.size() - 1; i >= 0; --i) {
		if ((_time[i] < '0' || _time[i]>'9') && _time[i] != '.') {   //除了数字和小数点都不合法
			return false;
		}
		if (_time[i] == '.') {   //判断是否有进位的情况，有则证明输入错误
			++cnt;
			if (cnt == 1) {
				if (stoi(_time.substr(i + 1, end_pos - i)) < 0 || stoi(_time.substr(i + 1, end_pos - i)) >= 1000) {
					return false;
				}
				end_pos = i - 1;
			}
			else if (cnt == 2) {
				if (stoi(_time.substr(i + 1, end_pos - i)) < 0 || stoi(_time.substr(i + 1, end_pos - i)) >= 60) {
					return false;
				}
				end_pos = i - 1;
			}
			else if (cnt == 3) {
				if (stoi(_time.substr(i + 1, end_pos - i)) < 0 || stoi(_time.substr(i + 1, end_pos - i)) >= 60) {
					return false;
				}
				end_pos = i - 1;
			}
			else {
				return false;
			}
		}
	}
	if (cnt == 0) {  //无小数点证明只有ms，ms未判断，现需判断ms是否合法
		if (stoi(_time) < 0 || stoi(_time) >= 1000) {
			return false;
		}
	}
	else if (cnt == 1) {  //只有一个小数点证明有s和ms，s未判断，现需判断s是否合法
		if (stoi(_time.substr(0, end_pos + 1)) < 0 || stoi(_time.substr(0, end_pos + 1)) >= 60) {
			return false;
		}
	}
	else if (cnt == 2) {  //有两个小数点证明有m、s和ms，m未判断，现需判断m是否合法
		if (stoi(_time.substr(0, end_pos + 1)) < 0 || stoi(_time.substr(0, end_pos + 1)) >= 60) {
			return false;
		}
	}
	else if (cnt == 3) {  //有两个小数点证明有h、m、s和ms，h未判断，现需判断h是否合法
		if (stoi(_time.substr(0, end_pos + 1)) < 0 || stoi(_time.substr(0, end_pos + 1)) >= 60) {
			return false;
		}
	}
	else {  //其他情况，报错并返回false
		cout << " ERROR!!" << endl;
		return false;
	}
	return true;
}

//检测输入的选择模式的数字是否合法
bool JudgeModeNum(int a, string mode) {
	if (a == 0) {  //不可能出现0，出现就返回false
		return false;
	}
	else if (a > 0 && a < 10) {  //a只有1位
		if (mode.size() > 1) {
			return false;
		}
		if (mode[0] - '0' < 1 || mode[0] - '0' > a) {
			return false;
		}
	}
	else {   //a有两位，不考虑三位及以上的情况，因为不可能出现
		if (mode.size() > 2) {
			return false;
		}
		if (mode.size() == 1) {
			if (mode[0] - '0' < 0 || mode[0] - '0' > 9) {
				return false;
			}
		}
		if (mode.size() == 2) {
			if (mode[0] - '0' < 0 || mode[0] - '0' > 9 || mode[1] - '0' < 0 || mode[1] - '0' > 9) {
				return false;
			}
		}
		if (stoi(mode) > a || stoi(mode) < 0) {
			return false;
		}
	}
	return true;
}

//将char转化为string
string CharToStr(char* contentChar) {   
	string temp;
	for (int i = 0; contentChar[i] != '\0'; i++) {
		temp += contentChar[i];
	}
	return temp;
}

//获取关卡名
string Name_of_Level(int x) {  //通过x的值获取关卡名
	string name = "0";
	switch (x) {
	case 1:
		name = "Factory：";
		break;
	case 2:
		name = "Hotel：";
		break;
	case 3:
		name = "Club：";
		break;
	case 4:
		name = "Prison：";
		break;
	case 5:
		name = "Studio：";
		break;
	case 6:
		name = "Mansion：";
		break;
	case 7:
		name = "Chinatown：";
		break;
	case 8:
		name = "Drg.tape：";
		break;
	case 9:
		name = "Sltr.h：";
		break;
	case 10:
		name = "Bunker：";
		break;
	case 11:
		name = "Bunker Pt.2：";
		break;
	case 12:
		name = "Psych.：";
		break;
	default:
		name = "Error!";
		break;
	}
	return name;
}

//主模式选择
string ChooseMode(string mode) { 
	while (1) {
		cout << "  Choose the Mode you want to do(Input only number):\n"
			<< "--------------------------------------------------------" << endl;
		cout << "  1.Change sob" << " " << "2.Read sob" << " " << "3.Quit" << endl;
		cin >> mode;
		if (!JudgeModeNum(3, mode)) {
			cout << " Please enter the correct number!" << endl << endl;;
			Sleep(500);
		}
		else {
			break;
		}
	}
	return mode;
}

//选择难度
string ChooseDifficulty(string NorH) {
	while (1) {
		cout << "  Choose the Mode Normal or Hard (Input only number):\n"
			<< "--------------------------------------------------------" << endl;
		cout << "  1.Normal" << " " << "2.Hard" << " " << "3. Back" << endl;
		cin >> NorH;
		if (!JudgeModeNum(3, NorH)) {
			cout << " Please enter the correct number!" << endl << endl;
			Sleep(500);
		}
		else {
			break;
		}
	}
	return NorH;
}

//选择时间或死亡数
string ChooseTimeOrDeathToSum(string TorD) {
	while (1) {
		cout << "You want to change time or deaths(Input only number)：" << endl;
		cout << "-------------------------------------------------------" << endl;
		cout << "  " << "1.Time" << " " << "2.Deaths" << endl;
		cin >> TorD;
		if (!JudgeModeNum(2, TorD)) {
			cout << " Please enter the correct number!" << endl << endl;
			Sleep(500);
		}
		else {
			break;
		}
	}
	return TorD;
}

//选择关卡
string ChooseLevel(string level) {
	while (1) {
		cout << "Which level you want to choose(Input only number):" << endl;
		cout << "-------------------------------------------------------------------------------------------" << endl;
		cout << " " << " " << "1.Factory 2.Hotel 3.Club 4.Prison 5.Studio 6.Mansion 7.Chinatown " << endl;
		cout << " " << " " << "8.Drg.tape 9.Sltr.h 10.Bunker 11.Bunker Pt.2 12.Psych. 13.Back" << endl << endl;
		cin >> level;
		if (!JudgeModeNum(13, level)) {
			cout << " Please enter the correct number!" << endl << endl;
			Sleep(500);
		}
		else {
			break;
		}
	}
	return level;
}

//输入时间
string CinRightFormatLevelTime(string LevelTime) {
	while (1) {
		cout << "You want change it to(Input only number):" << endl;
		cout << "Ps. The format should be xx.xx.xx.xx  " << endl;
		cout << "e.g. if you want to change the time of a level to 3m00s200ms. You should enter 3.00.20 " << endl;
		cin >> LevelTime;
		if (!JudgeCinTime(LevelTime)) {
			cout << " Please enter the correct format of time!!" << endl << endl;
			Sleep(500);
		}
		else {
			break;
		}
	}
	return LevelTime;
}

//输入死亡数
string CinLevelDeath(string death) {
	bool legal = true;
	while (1) {
		cout << "You want change it to:" << endl;
		cout << "Ps. The format should be xx  " << endl;
		cout << "e.g. if you want to change the deaths of a level to 5 . You should enter 5 " << endl;
		cin >> death;   //获取输入死亡数
		for (int i = 0; i < death.size(); i++) {  //检查
			if (death[i] < '0' || death[i]>'9') {  //非法输入
				legal = false;
				break;
			}
		}
		if (!legal) {
			cout << "WTF??? Please enter the correct number!" << endl << endl;
			Sleep(500);
		}
		else {
			break;
		}
	}
	return death;
}

//选择是否计算医生
string ChoosePsychOrNot(string psych) {
	while (1) {
		cout << "\n  Please choose whether with boss(Input only number):\n"
			<< "---------------------------------------------------------\n";
		cout << "    1.Yes" << "\t" << "2.No" << "\t" << "3.Back" << endl;
		cin >> psych;
		if (!JudgeModeNum(3, psych)) {
			cout << " Please enter the correct number!" << endl << endl;
			Sleep(500);
		}
		else {
			break;
		}
	}
	return psych;
}

int main() {
	cout << "Welcome to use the KzSobModifier!(Only be effective in xbxty's Kz)" << endl;
	cout << "Warning: This project can only be effective in the type 'Action Only'! " << endl;
	cout << "         " << "Other types will take this project into trouble. So please do not have a try in other ones." << endl;
	
	bool _is_open = true;   //是否打开
	ReadSobFile ReadSobInfo;    //读取文件数据类
	SUM SumOfData;  //求和类

	int id = 0;
	//获取steam的id
	bool JudgeForSteam = GetSteamId(id);
	if (JudgeForSteam) {    //成功则读出id
		cout << "Your Steam id:" << " " << id << endl;
	}
	else {   //失败则说明无steam
		cout << "Please check if your steam is correctly installed! The project will quit in 3s." << endl;
		Sleep(3000);
		return 0;
	}
	//获取文件路径，使用酱圣遗产
	string Path_of_File = "D:\\Steam\\userdata\\1272762122\\460950\\ac\\WinAppDataLocal\\Katana_ZERO\\KatanaSpeedrunOpt1272762122.zero";
	cout << "Path of your SRsavingfile: " << Path_of_File << endl << endl;
	int cnt = 0;   //计数专用
	int row = -1;   //行数统计
	int DifficultyOfRun = -1;   //是否为困难模式
	int NumOfDeath = -1;   //记录死亡数
	int KindToCaculate = -1;  //是否计算医生
	//记录时间
	Time FileTime;
	FileTime.h = "0";
	FileTime.m = "0";
	FileTime.s = "0";
	FileTime.ms = "0";
	string mode;    //选择模式
	string NormalOrHard;   //选择普通或者困难
	string TimeOrDeath;   //选择时间或死亡数
	string SecretBoss;  //是否选择医生
	string LevelName;  //关卡名称
	string LevelTime;  //关卡时间
	string LevelDeath;  //关卡死亡数
	string str;    //字符串读取专用
	vector<Time>SumOfTime;  //记录时间便于求和
	vector<string>deaths;   //记录死亡数
	
	while (1) {
		_main:mode = ChooseMode(mode);
		//改动sob
		if ((mode[0] - '0') == 1) {
			NormalOrHard = ChooseDifficulty(NormalOrHard);
			if (NormalOrHard[0] - '0' == 1) {
				DifficultyOfRun = 1;
			}
			else if(NormalOrHard[0] - '0' == 2){
				DifficultyOfRun = 2;
			}
			else {
				mode = "";
				goto _main;
			}
		_time:TimeOrDeath = ChooseTimeOrDeathToSum(TimeOrDeath);
			//时间
			if (TimeOrDeath[0] - '0' == 1) {
				LevelName = ChooseLevel(LevelName);
				if (stoi(LevelName) == 13) {
					TimeOrDeath = "";
					goto _time;
				}
				else if (stoi(LevelName) < 13 && stoi(LevelName) > 0) {
					LevelTime = CinRightFormatLevelTime(LevelTime);
					FileTime = ReadSobInfo.read_time(LevelTime);
					row = (stoi(LevelName) - 1) * 18 + 39;
					if (DifficultyOfRun == 1) {    //普通模式
						row = (stoi(LevelName) - 1) * 18 + 39;
					}
					else{   //困难模式
						row = (stoi(LevelName) - 1) * 18 + 48;
					}
					ReadSobInfo.ModifySrData(Path_of_File, row, ChangeTimeIntoSeconds(FileTime), _is_open);   //修改
					DifficultyOfRun = 0;  //置零，防止出bug
					if (_is_open == false) {  //打开失败
						cout << "Error in opening! The project will quit in 3s." << endl;
						Sleep(3000);
						return 0;
					}
					cout << endl;
					Sleep(500);
				}
				else {
					cout << " ERROR! The project will close in 3s!" << endl;
					Sleep(3000);
					return 0;
				}
			}
			//死亡
			else {
				LevelName = ChooseLevel(LevelName);
				if (stoi(LevelName) == 13) {
					TimeOrDeath = "";
					goto _time;
				}
				else if (stoi(LevelName) < 13 && stoi(LevelName) > 0) {
					LevelDeath = CinLevelDeath(LevelDeath);
					NumOfDeath = ReadSobInfo.read_gamefile_deaths(LevelDeath);
					row = (stoi(LevelName) - 1) * 18 + 40;
					if (DifficultyOfRun == 1) {    //普通模式
						row = (stoi(LevelName) - 1) * 18 + 40;
					}
					else {   //困难模式
						row = (stoi(LevelName) - 1) * 18 + 49;
					}
					ReadSobInfo.ModifySrData(Path_of_File, row, to_string(NumOfDeath), _is_open);
					if (_is_open == false) {  //打开失败
						cout << "Error in opening! The project will quit in 5s." << endl;
						Sleep(5000);
						return 0;
					}
					DifficultyOfRun = 0;  //置零，防止出bug
					cout << endl;
					Sleep(500);
				}
				else {
					cout << " ERROR! The project will close in 3s!" << endl;
					Sleep(3000);
					return 0;
				}
			}
			//重置变量
			row = -1;
			DifficultyOfRun = -1;
			NumOfDeath = -1;
			FileTime.h = "0";
			FileTime.m = "0";
			FileTime.s = "0";
			FileTime.ms = "0";
			mode = "";
			NormalOrHard = "";
			TimeOrDeath = "";
			LevelName = "";
			LevelTime = "";
			LevelDeath = "";
		}
		//读取文件并计算sob
		else if ((mode[0] - '0') == 2) {
			diff:NormalOrHard = ChooseDifficulty(NormalOrHard);
			if (NormalOrHard[0] - '0' == 1) {
				DifficultyOfRun = 1;
			}
			else if (NormalOrHard[0] - '0' == 2) {
				DifficultyOfRun = 2;
			}
			else {
				mode = "";
				goto _main;
			}
			SecretBoss = ChoosePsychOrNot(SecretBoss);
			if (SecretBoss[0] - '0' == 3) {
				goto diff;
			}
			//不算
			else if(SecretBoss[0] - '0' == 2){
				KindToCaculate = 2;
			}
			//算
			else {
				KindToCaculate = 1;
			}
			cout << "\nLevel" << "           " << "Time" << "              " << "Deaths" << endl;
			cout << "---------------------------------------------------" << endl;
			ifstream infile;
			infile.open(Path_of_File, ios::in);
			if (!infile.is_open()) {  //打开失败
				cout << " Error in opening! The project will quit in 3s." << endl;
				Sleep(3000);
				return 0;
			}
			row = 1;
			while (infile >> str) {
				//含心理医生
				if (KindToCaculate == 1) {
					if (row > 249) {
						break;
					}
				}
				//不含心理医生
				if (KindToCaculate == 2) {
					if (row > 231) {
						break;
					}
				}
				if (DifficultyOfRun == 1) {    //普通模式
					if (((row - 39) >= 0) && (row - 39) % 18 == 0) {  //获取时间
						if (str[0] == '-') {
							cout << " You haven't finish all the level, We can't give you the correct sum of time!" << endl;
							goto diff;
						}
						SumOfTime.push_back(ReadSobInfo.read_gamefile_time(str));
					}
					if (((row - 39) >= 0) && (row - 39) % 18 == 1) {  //获取死亡数
						deaths.push_back(str);
					}
				}
				else if (DifficultyOfRun == 2) {   //困难模式
					if (((row - 39) >= 0) && (row - 39) % 18 == 9) {  //获取时间
						if (str[0] == '-') {
							cout << " You haven't finish all the level, We can't give you the correct sum of time!" << endl;
							goto diff;
						}
						SumOfTime.push_back(ReadSobInfo.read_gamefile_time(str));
					}
					if (((row - 39) >= 0) && (row - 39) % 18 == 10) {  //获取死亡数
						deaths.push_back(str);
					}
				}
				else {
					cout << " ERROR! The project will close in 3s!" << endl;
					Sleep(3000);
					return 0;
				}
				++row;
			}
			infile.close();
			FileTime = SumOfData.SumOfTime(SumOfTime);
			cnt = 1;
			while (cnt <= SumOfTime.size()) {
				cout << std::left << setw(16) << Name_of_Level(cnt);
				ReadSobInfo.CoutFormatTime(SumOfTime[cnt - 1]);
				cout << std::right << "  \t    " << to_string(ReadSobInfo.read_gamefile_deaths(deaths[cnt - 1]));
				cout << endl;
				cnt++;
			}
			cout << "Personal Best" << "：" << " ";
			ReadSobInfo.CoutFormatTime(FileTime);
			cout << std::right << "  \t    " << to_string(SumOfData.SumOfDeath(deaths));
			cout << endl;
			cout << endl;
			Sleep(500);
			//重置变量
			cnt = 0;
			row = -1;
			DifficultyOfRun = -1;
			KindToCaculate = -1;
			FileTime.h = "0";
			FileTime.m = "0";
			FileTime.s = "0";
			FileTime.ms = "0";
			mode = "";
			NormalOrHard = "";
			SecretBoss = "";
			vector <Time>().swap(SumOfTime);
			vector <string>().swap(deaths);
		}
		//退出程序
		else if ((mode[0] - '0') == 3) {
			break;
		}
		//输入错误
		else {
			cout << " ERROR! The project will close in 3s!" << endl << endl;
			Sleep(3000);
			return 0;
		}
	}

	//结束并等待退出
	cout << " Thank you for using! The project will quit in 3s. " << endl;
	Sleep(3000);
	return 0;
}