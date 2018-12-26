#include "stGameRecord.h"
#include <fstream>
#include <sstream>

#define DELIM ' '
#define COLS 4


void stGameRecord::loadRecord(std::vector<stGameRecord> &records, std::string filePath)
{
	std::fstream fs(filePath,std::fstream::in | std::fstream::out);

	if (fs.is_open())
	{
		std::string str;

		while (!fs.eof())
		{
			std::getline(fs, str);

			stGameRecord record;

			parse(str, &record);

			records.push_back(record);
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

void stGameRecord::saveRecord(stGameRecord &record, std::string filePath)
{
	std::fstream fs(filePath, std::fstream::app | std::fstream::out);
	
	
	if (fs.is_open())
	{
		std::string record_str;

		record_str = record.name;
		record_str += DELIM;
		record_str += std::to_string(record.play_time_milli);
		record_str += DELIM;
		record_str += std::to_string(record.final_stage);
		record_str += DELIM;
		record_str += std::to_string(record.score);

		fs.write(record_str.c_str(), record_str.length());

		fs.close();
	}
	else
	{
		std::printf("File open error!!");
	}

}