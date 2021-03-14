#ifndef IMU_SENSOR_ARRAY_H
#define IMU_SENSOR_ARRAY_H

#include <string>
//#include <array>
#include <vector>

//const std::array<std::pair<std::string, int>, 25> IMUSensor = { {
static const std::vector<std::pair<std::string, int>> IMUSensor = { {
    {"ID", 4},

    // quaternions
    {"quatX", 4},
    {"quatY", 4},
    {"quatZ", 4},
    {"quatW", 4},

    // degrees
    {"eulerX", 2},
    {"eulerY", 2},
    {"eulerZ", 2},

    // rad/s
    {"angularVelocityX", 2},
    {"angularVelocityY", 2},
    {"angularVelocityZ", 2},

    // m/s^2
    {"linearAccelerationX", 2},
    {"linearAccelerationY", 2},
    {"linearAccelerationZ", 2},
    
    {"absoluteAccelerationX", 2},
    {"absoluteAccelerationY", 2},
    {"absoluteAccelerationZ", 2},

    {"gravityAccelerationX", 2},
    {"gravityAccelerationY", 2},
    {"gravityAccelerationZ", 2},

    // In microtesla
    {"magneticFieldX", 2},
    {"magneticFieldY", 2},
    {"magneticFieldZ", 2},
    
    {"duration", 2},
    {"temperature", 1},
} };


#endif
