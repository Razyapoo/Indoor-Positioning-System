#ifndef DATA_READER_H
#define DATA_READER_H

#include <opencv2/opencv.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

struct Anchor
{
    int anchorID;
    double distance;
};

struct UWBData
{
    std::string id;
    long long timestamp;
    int tagID;
    std::vector<Anchor> anchorList;
};

struct VideoData
{
    std::string id;
    long long timestamp;
};

class DataReader
{
public:
    static void runDataReader();
    static int currentFrameIdx;
    static cv::VideoCapture cap;
    static bool isTrackbarActive;

private:
    static std::vector<UWBData>
    readUWBData(const std::string &filename);
    static UWBData findUWBMeasurement(const std::vector<UWBData> &uwbData, long long videoTimestamp);
    static void onTrackbarChange(int position, void *userdata);
};

#endif