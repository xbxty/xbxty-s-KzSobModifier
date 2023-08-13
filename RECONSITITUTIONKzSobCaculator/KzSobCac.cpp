
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

//��ʥ���Ų�
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

//��ʱ��ת��Ϊ����Ϊ��λ
string ChangeTimeIntoSeconds(Time _time) {
	string ansStr;
	int s = 0;
	int ms = 0;
	s = stoi(_time.h) * 3600 + stoi(_time.m) * 60 + stoi(_time.s);
	ms = stoi(_time.ms) / 10;
	ansStr = to_string(s) + "." + to_string(ms);
	return ansStr;
}

//��������ʱ���Ƿ�Ϸ�
bool JudgeCinTime(string _time) {
	int cnt = 0;  //С��������
	int end_pos = _time.size() - 1;  //�ַ�����βλ��
	for (int i = _time.size() - 1; i >= 0; --i) {
		if ((_time[i] < '0' || _time[i]>'9') && _time[i] != '.') {   //�������ֺ�С���㶼���Ϸ�
			return false;
		}
		if (_time[i] == '.') {   //�ж��Ƿ��н�λ�����������֤���������
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
	if (cnt == 0) {  //��С����֤��ֻ��ms��msδ�жϣ������ж�ms�Ƿ�Ϸ�
		if (stoi(_time) < 0 || stoi(_time) >= 1000) {
			return false;
		}
	}
	else if (cnt == 1) {  //ֻ��һ��С����֤����s��ms��sδ�жϣ������ж�s�Ƿ�Ϸ�
		if (stoi(_time.substr(0, end_pos + 1)) < 0 || stoi(_time.substr(0, end_pos + 1)) >= 60) {
			return false;
		}
	}
	else if (cnt == 2) {  //������С����֤����m��s��ms��mδ�жϣ������ж�m�Ƿ�Ϸ�
		if (stoi(_time.substr(0, end_pos + 1)) < 0 || stoi(_time.substr(0, end_pos + 1)) >= 60) {
			return false;
		}
	}
	else if (cnt == 3) {  //������С����֤����h��m��s��ms��hδ�жϣ������ж�h�Ƿ�Ϸ�
		if (stoi(_time.substr(0, end_pos + 1)) < 0 || stoi(_time.substr(0, end_pos + 1)) >= 60) {
			return false;
		}
	}
	else {  //�����������������false
		cout << " ERROR!!" << endl;
		return false;
	}
	return true;
}

//��������ѡ��ģʽ�������Ƿ�Ϸ�
bool JudgeModeNum(int a, string mode) {
	if (a == 0) {  //�����ܳ���0�����־ͷ���false
		return false;
	}
	else if (a > 0 && a < 10) {  //aֻ��1λ
		if (mode.size() > 1) {
			return false;
		}
		if (mode[0] - '0' < 1 || mode[0] - '0' > a) {
			return false;
		}
	}
	else {   //a����λ����������λ�����ϵ��������Ϊ�����ܳ���
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

//��charת��Ϊstring
string CharToStr(char* contentChar) {   
	string temp;
	for (int i = 0; contentChar[i] != '\0'; i++) {
		temp += contentChar[i];
	}
	return temp;
}

//��ȡ�ؿ���
string Name_of_Level(int x) {  //ͨ��x��ֵ��ȡ�ؿ���
	string name = "0";
	switch (x) {
	case 1:
		name = "Factory��";
		break;
	case 2:
		name = "Hotel��";
		break;
	case 3:
		name = "Club��";
		break;
	case 4:
		name = "Prison��";
		break;
	case 5:
		name = "Studio��";
		break;
	case 6:
		name = "Mansion��";
		break;
	case 7:
		name = "Chinatown��";
		break;
	case 8:
		name = "Drg.tape��";
		break;
	case 9:
		name = "Sltr.h��";
		break;
	case 10:
		name = "Bunker��";
		break;
	case 11:
		name = "Bunker Pt.2��";
		break;
	case 12:
		name = "Psych.��";
		break;
	default:
		name = "Error!";
		break;
	}
	return name;
}

//��ģʽѡ��
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

//ѡ���Ѷ�
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

//ѡ��ʱ���������
string ChooseTimeOrDeathToSum(string TorD) {
	while (1) {
		cout << "You want to change time or deaths(Input only number)��" << endl;
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

//ѡ��ؿ�
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

//����ʱ��
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

//����������
string CinLevelDeath(string death) {
	bool legal = true;
	while (1) {
		cout << "You want change it to:" << endl;
		cout << "Ps. The format should be xx  " << endl;
		cout << "e.g. if you want to change the deaths of a level to 5 . You should enter 5 " << endl;
		cin >> death;   //��ȡ����������
		for (int i = 0; i < death.size(); i++) {  //���
			if (death[i] < '0' || death[i]>'9') {  //�Ƿ�����
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

//ѡ���Ƿ����ҽ��
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
	
	bool _is_open = true;   //�Ƿ��
	ReadSobFile ReadSobInfo;    //��ȡ�ļ�������
	SUM SumOfData;  //�����

	int id = 0;
	//��ȡsteam��id
	bool JudgeForSteam = GetSteamId(id);
	if (JudgeForSteam) {    //�ɹ������id
		cout << "Your Steam id:" << " " << id << endl;
	}
	else {   //ʧ����˵����steam
		cout << "Please check if your steam is correctly installed! The project will quit in 3s." << endl;
		Sleep(3000);
		return 0;
	}
	//��ȡ�ļ�·����ʹ�ý�ʥ�Ų�
	string Path_of_File = "D:\\Steam\\userdata\\1272762122\\460950\\ac\\WinAppDataLocal\\Katana_ZERO\\KatanaSpeedrunOpt1272762122.zero";
	cout << "Path of your SRsavingfile: " << Path_of_File << endl << endl;
	int cnt = 0;   //����ר��
	int row = -1;   //����ͳ��
	int DifficultyOfRun = -1;   //�Ƿ�Ϊ����ģʽ
	int NumOfDeath = -1;   //��¼������
	int KindToCaculate = -1;  //�Ƿ����ҽ��
	//��¼ʱ��
	Time FileTime;
	FileTime.h = "0";
	FileTime.m = "0";
	FileTime.s = "0";
	FileTime.ms = "0";
	string mode;    //ѡ��ģʽ
	string NormalOrHard;   //ѡ����ͨ��������
	string TimeOrDeath;   //ѡ��ʱ���������
	string SecretBoss;  //�Ƿ�ѡ��ҽ��
	string LevelName;  //�ؿ�����
	string LevelTime;  //�ؿ�ʱ��
	string LevelDeath;  //�ؿ�������
	string str;    //�ַ�����ȡר��
	vector<Time>SumOfTime;  //��¼ʱ��������
	vector<string>deaths;   //��¼������
	
	while (1) {
		_main:mode = ChooseMode(mode);
		//�Ķ�sob
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
			//ʱ��
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
					if (DifficultyOfRun == 1) {    //��ͨģʽ
						row = (stoi(LevelName) - 1) * 18 + 39;
					}
					else{   //����ģʽ
						row = (stoi(LevelName) - 1) * 18 + 48;
					}
					ReadSobInfo.ModifySrData(Path_of_File, row, ChangeTimeIntoSeconds(FileTime), _is_open);   //�޸�
					DifficultyOfRun = 0;  //���㣬��ֹ��bug
					if (_is_open == false) {  //��ʧ��
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
			//����
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
					if (DifficultyOfRun == 1) {    //��ͨģʽ
						row = (stoi(LevelName) - 1) * 18 + 40;
					}
					else {   //����ģʽ
						row = (stoi(LevelName) - 1) * 18 + 49;
					}
					ReadSobInfo.ModifySrData(Path_of_File, row, to_string(NumOfDeath), _is_open);
					if (_is_open == false) {  //��ʧ��
						cout << "Error in opening! The project will quit in 5s." << endl;
						Sleep(5000);
						return 0;
					}
					DifficultyOfRun = 0;  //���㣬��ֹ��bug
					cout << endl;
					Sleep(500);
				}
				else {
					cout << " ERROR! The project will close in 3s!" << endl;
					Sleep(3000);
					return 0;
				}
			}
			//���ñ���
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
		//��ȡ�ļ�������sob
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
			//����
			else if(SecretBoss[0] - '0' == 2){
				KindToCaculate = 2;
			}
			//��
			else {
				KindToCaculate = 1;
			}
			cout << "\nLevel" << "           " << "Time" << "              " << "Deaths" << endl;
			cout << "---------------------------------------------------" << endl;
			ifstream infile;
			infile.open(Path_of_File, ios::in);
			if (!infile.is_open()) {  //��ʧ��
				cout << " Error in opening! The project will quit in 3s." << endl;
				Sleep(3000);
				return 0;
			}
			row = 1;
			while (infile >> str) {
				//������ҽ��
				if (KindToCaculate == 1) {
					if (row > 249) {
						break;
					}
				}
				//��������ҽ��
				if (KindToCaculate == 2) {
					if (row > 231) {
						break;
					}
				}
				if (DifficultyOfRun == 1) {    //��ͨģʽ
					if (((row - 39) >= 0) && (row - 39) % 18 == 0) {  //��ȡʱ��
						if (str[0] == '-') {
							cout << " You haven't finish all the level, We can't give you the correct sum of time!" << endl;
							goto diff;
						}
						SumOfTime.push_back(ReadSobInfo.read_gamefile_time(str));
					}
					if (((row - 39) >= 0) && (row - 39) % 18 == 1) {  //��ȡ������
						deaths.push_back(str);
					}
				}
				else if (DifficultyOfRun == 2) {   //����ģʽ
					if (((row - 39) >= 0) && (row - 39) % 18 == 9) {  //��ȡʱ��
						if (str[0] == '-') {
							cout << " You haven't finish all the level, We can't give you the correct sum of time!" << endl;
							goto diff;
						}
						SumOfTime.push_back(ReadSobInfo.read_gamefile_time(str));
					}
					if (((row - 39) >= 0) && (row - 39) % 18 == 10) {  //��ȡ������
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
			cout << "Personal Best" << "��" << " ";
			ReadSobInfo.CoutFormatTime(FileTime);
			cout << std::right << "  \t    " << to_string(SumOfData.SumOfDeath(deaths));
			cout << endl;
			cout << endl;
			Sleep(500);
			//���ñ���
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
		//�˳�����
		else if ((mode[0] - '0') == 3) {
			break;
		}
		//�������
		else {
			cout << " ERROR! The project will close in 3s!" << endl << endl;
			Sleep(3000);
			return 0;
		}
	}

	//�������ȴ��˳�
	cout << " Thank you for using! The project will quit in 3s. " << endl;
	Sleep(3000);
	return 0;
}