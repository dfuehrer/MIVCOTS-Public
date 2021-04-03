#ifndef DATA_INTERFACE_H
#define DATA_INTERFACE_H

#include <map>
#include <vector>
#include <string>

#include "GPSSensorArray.h"

// the idea with this is ill import this into other files and then i can just got the data array for gps by just accessing "GPS"

const std::map<std::string, const std::vector<std::pair<std::string, int>>> dataArrayMap = {
    {"GPS", GPSSensor},
};


#endif //DATA_INTERFACE_H
