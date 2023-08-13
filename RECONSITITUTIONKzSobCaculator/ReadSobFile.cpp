#include "ReadSobFile.h"

Time ReadSobFile::read_time(string s) {
	int cnt = 0;             //记录.的数量，判断到没到h/m/s
	int temp_sum1 = 0;       //记录这一段的时间
	int temp_sum2 = 0;
	int temp_sum3 = 0;
	int temp_sum4 = 0;
	Time t;
	t.h = "0";
	t.m = "0";
	t.s = "0";
	t.ms = "0";
	for (int i = 0; i < s.size(); i++) {
		if (s[i] != '.') {       //分别记录h、m、s、ms，通过.的数量来判断
			if (cnt == 0) {
				temp_sum1 = ((s[i] - '0') + temp_sum1 * 10);
			}
			if (cnt == 1) {
				temp_sum2 = ((s[i] - '0') + temp_sum2 * 10);
			}
			if (cnt == 2) {
				temp_sum3 = ((s[i] - '0') + temp_sum3 * 10);
			}
			if (cnt == 3) {
				temp_sum4 = ((s[i] - '0') + temp_sum4 * 10);
			}
		}
		else {
			cnt++;
		}
	}
	if (cnt == 1) {
		t.h = "0";
		t.m = "0";
		t.s = to_string(temp_sum1);
		t.ms = to_string(temp_sum2 * 10);
	}
	else if (cnt == 2) {
		t.h = "0";
		t.m = to_string(temp_sum1);
		t.s = to_string(temp_sum2);
		t.ms = to_string(temp_sum3 * 10);
	}
	else if (cnt == 3) {
		t.h = to_string(temp_sum1);
		t.m = to_string(temp_sum2);
		t.s = to_string(temp_sum3);
		t.ms = to_string(temp_sum4 * 10);
	}
	return t;
};

Time ReadSobFile::read_gamefile_time(string s) {
	int cnt = 0;             //记录.的数量，判断到没到h/m/s
	int temp_sum1 = 0;        //记录这一段的时间
	int temp_sum2 = 0;
	Time t;
	t.h = "0";
	t.m = "0";
	t.s = "0";
	t.ms = "0";
	for (int i = 0; i < s.size(); i++) {
		if (s[i] != '.') {
			if (cnt == 0) {
				temp_sum1 = ((s[i] - '0') + temp_sum1 * 10);
			}
			if (cnt == 1) {
				temp_sum2 = ((s[i] - '0') + temp_sum2 * 10);
			}
		}
		else {
			cnt++;
		}
	}
	t.ms = to_string(temp_sum2 * 10);  //接下来把以s为单位的时间转换为xx h xx m xx s xx ms的形式
	if (temp_sum1 >= 60) {
		t.m = to_string(temp_sum1 / 60);
		t.s = to_string(temp_sum1 % 60);
	}
	else {
		t.s = to_string(temp_sum1);
	}
	if (stoi(t.m) >= 60) {
		t.h = to_string(stoi(t.m) / 60);
		t.m = to_string(stoi(t.m) % 60);
	}
	return t;
}

int ReadSobFile::read_gamefile_deaths(string s) {
	int death = 0;
	for (int i = 0; i < s.size(); i++) {
		death = death * 10 + (s[i] - '0');
	}
	return death;
}

void ReadSobFile::CoutFormatTime(Time time) {
	int ZeroPos = 0;
	vector<bool> ZeroOrNot(5, false);
	if (stoi(time.h) == 0) {
		ZeroOrNot[1] = true;
	}
	if (ZeroOrNot[1] == true && stoi(time.m) == 0) {
		ZeroOrNot[2] = true;
	}
	if (ZeroOrNot[2] == true && stoi(time.s) == 0) {
		ZeroOrNot[3] = true;
	}
	if (ZeroOrNot[3] == true && stoi(time.ms) == 0) {
		ZeroOrNot[4] = true;
	}
	for (int i = 4; i >= 0; --i) {
		ZeroPos = i;
		if (ZeroOrNot[i]) {
			break;
		}
	}
	if (ZeroPos == 0) {
		if (stoi(time.m) < 10) {
			time.m = "0" + time.m;
		}
		if (stoi(time.s) < 10) {
			time.s = "0" + time.s;
		}
		if (stoi(time.ms) < 10) {
			time.ms = "00" + time.ms;
		}
		else if (stoi(time.ms) < 100) {
			time.ms = "0" + time.ms;
		}
		cout << time.h << "h" << time.m << "m" << time.s << "s" << time.ms << "ms";
	}
	else if (ZeroPos == 1) {
		if (stoi(time.s) < 10) {
			time.s = "0" + time.s;
		}
		if (stoi(time.ms) < 10) {
			time.ms = "00" + time.ms;
		}
		else if (stoi(time.ms) < 100) {
			time.ms = "0" + time.ms;
		}
		cout << time.m << "m" << time.s << "s" << time.ms << "ms";
	}
	else if (ZeroPos == 2) {
		if (stoi(time.ms) < 10) {
			time.ms = "00" + time.ms;
		}
		else if (stoi(time.ms) < 100) {
			time.ms = "0" + time.ms;
		}
		cout << time.s << "s" << time.ms << "ms";
	}
	else if (ZeroPos == 3) {
		if (stoi(time.ms) < 10) {
			time.ms = "00" + time.ms;
		}
		else if (stoi(time.ms) < 100) {
			time.ms = "0" + time.ms;
		}
		cout << "0s" << time.ms << "ms";
	}
	else if (ZeroPos == 4) {
		if (stoi(time.ms) < 10) {
			time.ms = "00" + time.ms;
		}
		else if (stoi(time.ms) < 100) {
			time.ms = "0" + time.ms;
		}
		cout << "0s" << time.ms << "ms";
	}
	else {
		cout << " ERROR!";
	}
}

void ReadSobFile::ModifySrData(string fileName, int lineNum, string lineData, bool& _is_open) {
	ifstream infile;
	infile.open(fileName, ios::in);
	if (!infile.is_open()) {  //判断是否打开
		_is_open = false;
		return;
	}
	int line = 1;
	string str;
	string strFileData = ""; //先整体读出再整体写入，麻烦与简单的矛盾结合体
	while (infile >> str) {
		if (line == lineNum) {
			strFileData += lineData;
			strFileData += "\n";  //换行符
		}
		else {
			strFileData += str;
			strFileData += "\n";
		}
		line++;
	}
	infile.close();
	ofstream outfile;   //写入
	outfile.open(fileName, ios::out | ios::trunc);
	if (!outfile.is_open()) {   //是否打开
		_is_open = false;
		return;
	}
	outfile << strFileData;
	cout << "Successful Changing!" << endl;
	outfile.close();
}