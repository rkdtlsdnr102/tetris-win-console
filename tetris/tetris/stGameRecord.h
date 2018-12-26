#pragma once

#include <string>
#include <vector>

struct stGameRecord
{
	std::string name;
	long long play_time_milli;
	int final_stage;
	int score;

	static void loadRecord(std::vector<stGameRecord> &records, std::string filePath);
	static void saveRecord(stGameRecord &record, std::string filePath);

private:
	static void parse(std::string &record_str, stGameRecord *record);
};

