#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <QImage>


// bool debug = true;
struct Anchor
{
    int anchorID;
    double distance;

    Anchor() : anchorID(0), distance(0.0) {}

    Anchor(int anchorID, double distance) : anchorID(anchorID), distance(distance) {}

    Anchor(const Anchor& other) : anchorID(other.anchorID), distance(other.distance) {}

    Anchor(Anchor&& other) noexcept : anchorID(std::exchange(other.anchorID, -1)), distance(std::exchange(other.distance, -1)) {}

    Anchor& operator=(Anchor&& other) noexcept {
        if (this != &other) {
            anchorID = std::exchange(other.anchorID, -1);
            distance = std::exchange(other.distance, -1);

        }
        return *this;
    }


    ~Anchor() {}
};

struct VideoData
{
    int id;
    QImage qImage;
    long long timestamp;

    VideoData() : timestamp(0) {}

    VideoData(int id, QImage&& qImage, long long timestamp): id(id), qImage(std::move(qImage)), timestamp(timestamp) {}

    VideoData(int id, const QImage& qImage, long long timestamp)
        : id(id), qImage(qImage), timestamp(timestamp) {}

    VideoData(const VideoData& other)
        : id(other.id), qImage(other.qImage), timestamp(other.timestamp) {}

    VideoData(VideoData&& other) noexcept
        : id(std::exchange(other.id, -1)), qImage(std::move(other.qImage)), timestamp(std::exchange(other.timestamp, 0)) {}

    VideoData& operator=(VideoData&& other) noexcept {
        if (this != &other) {
            qImage = std::move(other.qImage);
            id = std::exchange(other.id, -1);
            timestamp = std::exchange(other.timestamp, 0);
        }
        return *this;
    }

    ~VideoData() {}

};

struct UWBData
{
    int id;
    long long timestamp;
    int tagID;
    std::vector<Anchor> anchorList;

    UWBData() : id(-1), timestamp(0), tagID(-1) {}

    UWBData(int id, long long timestamp, int tagID, const std::vector<Anchor>& anchors)
        : id(id), timestamp(timestamp), tagID(tagID), anchorList(anchors) {}

    UWBData(const UWBData& other)
        : id(other.id), timestamp(other.timestamp), tagID(other.tagID), anchorList(other.anchorList) {}

    UWBData(UWBData&& other) noexcept
        : id(std::exchange(other.id, -1)), timestamp(std::exchange(other.timestamp, 0)), tagID(std::exchange(other.tagID, -1)), anchorList(std::move(other.anchorList)) {}

    UWBData& operator=(UWBData&& other) noexcept {
        if (this != &other) {
            id = std::exchange(other.id, -1);
            timestamp = std::exchange(other.timestamp, 0);
            tagID = std::exchange(other.tagID, -1);
            anchorList = std::move(other.anchorList);

        }
        return *this;
    }

    // bool operator<(const UWBData& other) const {
    //     return id < other.id;
    // }

    // bool operator==(const UWBData& other) const {
    //     return id == other.id;
    // }

    // UWBData& operator=(const UWBData& other) {
    //     if (this != &other) {
    //         id = other.id;
    //         timestamp = other.timestamp;
    //         tagID = other.tagID;
    //         anchorList = other.anchorList;

    //     }
    //     return *this;
    // }

    ~UWBData() {}
};

struct UWBVideoData {
    VideoData videoData;
    UWBData uwbData;

    UWBVideoData() {}

    UWBVideoData(const VideoData& videoData, const UWBData& uwbData): videoData(videoData), uwbData(uwbData) {}

    UWBVideoData(VideoData&& videoData, UWBData&& uwbData): videoData(std::move(videoData)), uwbData(std::move(uwbData)) {}

    UWBVideoData(const UWBVideoData& other): videoData(other.videoData), uwbData(other.uwbData) {}

    UWBVideoData(UWBVideoData&& other) noexcept: videoData(std::move(other.videoData)), uwbData(std::move(other.uwbData)) {}

    UWBVideoData& operator=(UWBVideoData&& other) noexcept {
        if (this != &other) {
            videoData = std::move(other.videoData);
            uwbData = std::move(other.uwbData);
        }

        return *this;
    }

    ~UWBVideoData() {}
};

#endif // STRUCTURES_H
