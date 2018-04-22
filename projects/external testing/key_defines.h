#pragma once
#include<map>
#include<string>
#define numKeys 16
#define ID "AA"
#define DATE "AB"
#define TIME "AC"
#define LON "AD"
#define LAT "AE"
#define ALTITUDE "AF"
#define HEADING "AG"
#define ACC_X "AH"
#define ACC_Y "AI"
#define ACC_Z "AJ"
#define GYR_X "AK"
#define GYR_Y "AL"
#define GYR_Z "AM"
#define MAG_X "AN"
#define MAG_Y "AO"
#define MAG_Z "AP"
static std::map<std::string, std::string> keyMap {{ID, "ID"},{DATE, "DATE"},{TIME, "TIME"},{LON, "LON"},{LAT, "LAT"},{ALTITUDE, "ALTITUDE"},{HEADING, "HEADING"},{ACC_X, "ACC_X"},{ACC_Y, "ACC_Y"},{ACC_Z, "ACC_Z"},{GYR_X, "GYR_X"},{GYR_Y, "GYR_Y"},{GYR_Z, "GYR_Z"},{MAG_X, "MAG_X"},{MAG_Y, "MAG_Y"},{MAG_Z, "MAG_Z"}};