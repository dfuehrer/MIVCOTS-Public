#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>


std::string &ltrim(std::string &s);
std::string &rtrim(std::string &s);
// Trim whitespace from a string in place
std::string &trim(std::string &s);

long convertTimestamp(long timestamp);
#endif //UTILITIES_H
