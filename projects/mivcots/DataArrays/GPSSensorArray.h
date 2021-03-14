#ifndef GPS_SENSOR_ARRAY_H
#define GPS_SENSOR_ARRAY_H

#include <string>
//#include <array>
#include <vector>

//const std::array<std::pair<std::string, int>, 14> GPSSensor = { {
// TODO maybe this should be an array with more properties like the scale and more specific type too
static const std::vector<std::pair<std::string, int>> GPSSensor = { {
    {"ID", 4},
    {"latitude", 4},
    {"longitude", 4},
    {"longTime", 4},
    {"longDate", 4},
    {"geoidHeight", 4},
    {"speed", 4},
    {"angle", 4},
    {"altitude", 4},
    {"duration", 2},
    {"fix", 1},
    {"fixQuality", 1},
    {"HDOP", 1},
    {"satellites", 1},
} };


#endif
