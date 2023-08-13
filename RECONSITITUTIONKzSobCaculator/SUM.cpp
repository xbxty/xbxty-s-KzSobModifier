#include "SUM.h"

Time SUM::SumOfTime(vector<Time> time) {
	Time sum_of_time;
	sum_of_time.h = "0";
	sum_of_time.m = "0";
	sum_of_time.s = "0";
	sum_of_time.ms = "0";
	int h = 0;
	int m = 0;
	int s = 0;
	int ms = 0;
	for (int i = 0; i < time.size(); i++) {
		h += stoi(time[i].h);
		m += stoi(time[i].m);
		if (m >= 60) {
			h += 1;
			m = m % 60;
		}
		s += stoi(time[i].s);
		if (s >= 60) {
			m += 1;
			s = s % 60;
		}
		ms += stoi(time[i].ms);
		if (ms >= 1000) {
			s += 1;
			ms = ms % 1000;
		}
	}
	sum_of_time.h = to_string(h);
	sum_of_time.m = to_string(m);
	sum_of_time.s = to_string(s);
	sum_of_time.ms = to_string(ms);
	return sum_of_time;
}


int SUM::SumOfDeath(vector<string>deaths) {
	int sum_of_death = 0;
	int deathnum = 0;
	for (int i = 0; i < deaths.size(); i++) {
		for (int j = 0; j < deaths[i].size(); j++) {
			deathnum = deathnum * 10 + (deaths[i][j] - '0');
		}
		sum_of_death += deathnum;
		deathnum = 0;
	}
	return sum_of_death;
}