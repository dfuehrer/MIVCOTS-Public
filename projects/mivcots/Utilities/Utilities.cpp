#include "Utilities.h"

// trim from start
std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
		std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}

long convertTimestamp(long timestamp)
{
	// Format: hh mm ss mmm
	long ms = timestamp % 1000;
	long sec = (timestamp % 100000 - ms) / 1000;
	long min = (timestamp % 10000000 - sec - ms) / 100000;
	long hour = timestamp / 10000000;
	return ms + sec * 1000 + min * 60 * 1000 + hour * 60 * 60 * 1000;
}
