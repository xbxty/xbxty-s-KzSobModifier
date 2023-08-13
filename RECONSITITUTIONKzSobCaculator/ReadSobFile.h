#include<string>
#include<vector>
#include<iostream>
#include<fstream>

using namespace std;

#pragma once

//记录时间用
typedef struct Time {
	string h;
	string m;
	string s;
	string ms;
}Time;

class ReadSobFile
{
public:
	Time read_time(string s);
	Time read_gamefile_time(string s);
	int read_gamefile_deaths(string s);
	void CoutFormatTime(Time time);
	void ModifySrData(string fileName, int lineNum, string lineData, bool& _is_open);
};

