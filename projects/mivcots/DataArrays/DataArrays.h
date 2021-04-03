#ifndef DATA_INTERFACE_H
#define DATA_INTERFACE_H

#include <map>
#include <vector>
#include <string>

#include "GPSSensorArray.h"

const std::map<std::string, const std::vector<std::pair<std::string, int>>> dataArrayMap = {
    {"GPS", GPSSensor},
};


#endif //DATA_INTERFACE_H
