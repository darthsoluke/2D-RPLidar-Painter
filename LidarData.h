#ifndef LIDARDATA_H
#define LIDARDATA_H

#define NUM_POINTS 1000

#include <cstdint>

class LidarData
{
public:
    struct DataContents {
        uint8_t quality = 0;
        float distance_mm = 0.0f;
        float angle_deg = 0.0f;
        float rpm = 0.0f;
    };

    static DataContents lidardata;
    static DataContents points[NUM_POINTS];   // 静态成员声明

    LidarData() = default;
    ~LidarData() = default;


};

#endif // LIDARDATA_H
