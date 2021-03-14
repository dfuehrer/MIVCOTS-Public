#ifndef DATA_ARRAYS_H
#define DATA_ARRAYS_H

#include "GPSSensorArray.h"
#include "IMUSensorArray.h"

#include <unordered_map>
#include <string>
#include <vector>
//#include <array>

// this is an map that stores the arrays that have the order and length of our data
static const std::unordered_map<std::string, const std::vector<std::pair<std::string, int>>> dataArrayMap {
    {"GPS", GPSSensor},
    {"IMU", IMUSensor},
};
//const std::unordered_map<std::string, std::array<std::pair<std::string, int>, 25> &> dataArrayMap;

// dataArrayMap["GPS"] = GPSSensor;
// dataArrayMap.insert({"GPS", GPSSensor});
// dataArrayMap.insert({"IMU", IMUSensor});

#endif
