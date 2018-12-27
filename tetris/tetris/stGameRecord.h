#pragma once

#include <string>
#include <vector>

struct stGameRecord
{
	std::string name;
	long long play_time_milli;
	int final_stage;
	int score;

	static void loadRecord(std::vector<stGameRecord> *records, const std::string &filePath);
	static void saveRecord(stGameRecord &record, const std::string &filePath);

	std::string to_string(int pad=1);
	bool operator>(const stGameRecord &rhs) const ;

private:
	static void parse(std::string &record_str, stGameRecord *record);
};

