#ifndef KEY_DEFINES_H
#define KEY_DEFINES_H
#include<map>
#include<string>
#define numKeys 63
#define ID_U "AAU"
#define ID_D "AAD"
#define ID_S "AAS"
#define DATE_U "ABU"
#define DATE_D "ABD"
#define DATE_S "ABS"
#define TIME_U "ACU"
#define TIME_D "ACD"
#define TIME_S "ACS"
#define LON_U "ADU"
#define LON_D "ADD"
#define LON_S "ADS"
#define LAT_U "AEU"
#define LAT_D "AED"
#define LAT_S "AES"
#define ALTITUDE_U "AFU"
#define ALTITUDE_D "AFD"
#define ALTITUDE_S "AFS"
#define HEADING_U "AGU"
#define HEADING_D "AGD"
#define HEADING_S "AGS"
#define ACC_X_U "AHU"
#define ACC_X_D "AHD"
#define ACC_X_S "AHS"
#define ACC_Y_U "AIU"
#define ACC_Y_D "AID"
#define ACC_Y_S "AIS"
#define ACC_Z_U "AJU"
#define ACC_Z_D "AJD"
#define ACC_Z_S "AJS"
#define GYR_X_U "AKU"
#define GYR_X_D "AKD"
#define GYR_X_S "AKS"
#define GYR_Y_U "ALU"
#define GYR_Y_D "ALD"
#define GYR_Y_S "ALS"
#define GYR_Z_U "AMU"
#define GYR_Z_D "AMD"
#define GYR_Z_S "AMS"
#define MAG_X_U "ANU"
#define MAG_X_D "AND"
#define MAG_X_S "ANS"
#define MAG_Y_U "AOU"
#define MAG_Y_D "AOD"
#define MAG_Y_S "AOS"
#define MAG_Z_U "APU"
#define MAG_Z_D "APD"
#define MAG_Z_S "APS"
#define ANALYSIS_COUNT_U "AQU"
#define ANALYSIS_COUNT_D "AQD"
#define ANALYSIS_COUNT_S "AQS"
#define MPH_U "ARU"
#define MPH_D "ARD"
#define MPH_S "ARS"
#define TEMP_U "ASU"
#define TEMP_D "ASD"
#define TEMP_S "ASS"
#define MESSAGE_NUMBER_U "ATU"
#define MESSAGE_NUMBER_D "ATD"
#define MESSAGE_NUMBER_S "ATS"
#define NUMBER_KEYS_U "AUU"
#define NUMBER_KEYS_D "AUD"
#define NUMBER_KEYS_S "AUS"
static std::map<std::string, std::string> keyMap {
{ID_U, "ID_U"},
{ID_D, "ID_D"},
{ID_S, "ID_S"},
{DATE_U, "DATE_U"},
{DATE_D, "DATE_D"},
{DATE_S, "DATE_S"},
{TIME_U, "TIME_U"},
{TIME_D, "TIME_D"},
{TIME_S, "TIME_S"},
{LON_U, "LON_U"},
{LON_D, "LON_D"},
{LON_S, "LON_S"},
{LAT_U, "LAT_U"},
{LAT_D, "LAT_D"},
{LAT_S, "LAT_S"},
{ALTITUDE_U, "ALTITUDE_U"},
{ALTITUDE_D, "ALTITUDE_D"},
{ALTITUDE_S, "ALTITUDE_S"},
{HEADING_U, "HEADING_U"},
{HEADING_D, "HEADING_D"},
{HEADING_S, "HEADING_S"},
{ACC_X_U, "ACC_X_U"},
{ACC_X_D, "ACC_X_D"},
{ACC_X_S, "ACC_X_S"},
{ACC_Y_U, "ACC_Y_U"},
{ACC_Y_D, "ACC_Y_D"},
{ACC_Y_S, "ACC_Y_S"},
{ACC_Z_U, "ACC_Z_U"},
{ACC_Z_D, "ACC_Z_D"},
{ACC_Z_S, "ACC_Z_S"},
{GYR_X_U, "GYR_X_U"},
{GYR_X_D, "GYR_X_D"},
{GYR_X_S, "GYR_X_S"},
{GYR_Y_U, "GYR_Y_U"},
{GYR_Y_D, "GYR_Y_D"},
{GYR_Y_S, "GYR_Y_S"},
{GYR_Z_U, "GYR_Z_U"},
{GYR_Z_D, "GYR_Z_D"},
{GYR_Z_S, "GYR_Z_S"},
{MAG_X_U, "MAG_X_U"},
{MAG_X_D, "MAG_X_D"},
{MAG_X_S, "MAG_X_S"},
{MAG_Y_U, "MAG_Y_U"},
{MAG_Y_D, "MAG_Y_D"},
{MAG_Y_S, "MAG_Y_S"},
{MAG_Z_U, "MAG_Z_U"},
{MAG_Z_D, "MAG_Z_D"},
{MAG_Z_S, "MAG_Z_S"},
{ANALYSIS_COUNT_U, "ANALYSIS_COUNT_U"},
{ANALYSIS_COUNT_D, "ANALYSIS_COUNT_D"},
{ANALYSIS_COUNT_S, "ANALYSIS_COUNT_S"},
{MPH_U, "MPH_U"},
{MPH_D, "MPH_D"},
{MPH_S, "MPH_S"},
{TEMP_U, "TEMP_U"},
{TEMP_D, "TEMP_D"},
{TEMP_S, "TEMP_S"},
{MESSAGE_NUMBER_U, "MESSAGE_NUMBER_U"},
{MESSAGE_NUMBER_D, "MESSAGE_NUMBER_D"},
{MESSAGE_NUMBER_S, "MESSAGE_NUMBER_S"},
{NUMBER_KEYS_U, "NUMBER_KEYS_U"},
{NUMBER_KEYS_D, "NUMBER_KEYS_D"},
{NUMBER_KEYS_S, "NUMBER_KEYS_S"}};
#endif //KEY_DEFINES_H
