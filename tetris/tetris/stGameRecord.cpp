#include "stGameRecord.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include "tetrisUtility.h"

#define DELIM ' '
#define COLS 4


void stGameRecord::loadRecord(std::vector<stGameRecord> *records, const std::string &filePath)
{
	std::fstream fs(filePath,std::fstream::in);

	if (fs.is_open())
	{
		std::string str;

		fs.seekg(0, fs.end);

		bool file_empty = (fs.tellg() == 0);

		if (file_empty)
		{
			std::printf("Empty File!!");
		}
		else
		{
			fs.seekg(0, fs.beg);

			while (!fs.eof())
			{
				std::getline(fs, str);

				stGameRecord record;

				parse(str, &record);

				records->push_back(record);
			}

			std::sort(records->begin(), records->end(), std::greater<stGameRecord>());
		}

		fs.close();
	}
	else
	{
		std::printf("File open error!!");
	}
}

void stGameRecord::parse(std::string &record_str, stGameRecord *record)
{
	std::stringstream ss(record_str);

	std::string str[COLS];

	int col_idx = 0;

	while (std::getline(ss, str[col_idx++], DELIM));

	record->name = str[0];
	record->play_time_milli = std::stoll(str[1]);
	record->final_stage = std::stoi(str[2]);
	record->score = std::stoi(str[3]);
}

void stGameRecord::saveRecord(stGameRecord &record, const std::string &filePath)
{
	std::fstream fs(filePath, std::fstream::app | std::fstream::out);

	if (fs.is_open())
	{		
		std::streampos org_pos = fs.tellg();

		fs.seekg(0, fs.end);

		bool file_empty = (fs.tellg() == 0);

		fs.seekg(0, org_pos);

		if (!file_empty)
			fs.put('\n');//put linefeed character if any records already exists

		std::string record_str;

		record_str = record.to_string();

		fs.write(record_str.c_str(), record_str.length());

		fs.close();
	}
	else
	{
		std::printf("File open error!!");
	}

}

std::string stGameRecord::to_string(int pad) {

	std::string str = "";

	str += name;
	for(int i=1;i<=pad;i++)
		str += DELIM;

	str += std::to_string(play_time_milli);
	for (int i = 1; i <= pad; i++)
		str += DELIM;

	str += std::to_string(final_stage);
	for (int i = 1; i <= pad; i++)
		str += DELIM;

	str += std::to_string(score);

	return str;
}

bool stGameRecord::operator>(const stGameRecord &rhs) const{

	return score > rhs.score;
}