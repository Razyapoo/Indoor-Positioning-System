#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <QImage>

enum ExportType {
    FrameByFrameExport,
    SegmentFramesExport
};

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

    Anchor& operator=(const Anchor& other) {
        if (this != &other) {
            anchorID = other.anchorID;
            distance = other.distance;

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

// struct UWBCoordinates {
//     double x = -1, y = -1;

//     UWBCoordinates(double x = -1, double y = -1) : x(x), y(y) {}

//     UWBCoordinates(const UWBCoordinates& other)
//         : x(other.x), y(other.y) {}

//     UWBCoordinates(UWBCoordinates&& other) noexcept
//         : x(std::exchange(other.x, -1)), y(std::exchange(other.y, -1)) {}

//     UWBCoordinates& operator=(UWBCoordinates&& other) noexcept {
//         if (this != &other) {
//             x = std::exchange(other.x, -1);
//             y = std::exchange(other.y, -1);

//         }
//         return *this;
//     }

//     UWBCoordinates& operator=(const UWBCoordinates& other) {
//         if (this != &other) {
//             x = other.x;
//             y = other.y;

//         }
//         return *this;
//     }

//     ~UWBCoordinates() {}
// };

struct UWBData
{
    int id;
    long long timestamp;
    int tagID;
    std::vector<Anchor> anchorList;
    QPointF coordinates;

    UWBData() : id(-1), timestamp(0), tagID(-1), coordinates() {}

    UWBData(int id, long long timestamp, int tagID, const std::vector<Anchor>& anchors, const QPointF& coordinates = QPointF())
        : id(id), timestamp(timestamp), tagID(tagID), anchorList(anchors), coordinates(coordinates) {}

    UWBData(const UWBData& other)
        : id(other.id), timestamp(other.timestamp), tagID(other.tagID), anchorList(other.anchorList), coordinates(other.coordinates) {}

    UWBData(UWBData&& other) noexcept
        : id(std::exchange(other.id, -1)), timestamp(std::exchange(other.timestamp, 0)), tagID(std::exchange(other.tagID, -1)), anchorList(std::move(other.anchorList)), coordinates(std::move(other.coordinates)) {}

    UWBData& operator=(UWBData&& other) noexcept {
        if (this != &other) {
            id = std::exchange(other.id, -1);
            timestamp = std::exchange(other.timestamp, 0);
            tagID = std::exchange(other.tagID, -1);
            anchorList = std::move(other.anchorList);
            coordinates = std::move(other.coordinates);
        }
        return *this;
    }

    // bool operator<(const UWBData& other) const {
    //     return id < other.id;
    // }

    // bool operator==(const UWBData& other) const {
    //     return id == other.id;
    // }

    UWBData& operator=(const UWBData& other) {
        if (this != &other) {
            id = other.id;
            timestamp = other.timestamp;
            tagID = other.tagID;
            anchorList = other.anchorList;
            coordinates = other.coordinates;
        }
        return *this;
    }

    ~UWBData() {}
};

struct UWBVideoData {
    VideoData videoData;
    std::vector<UWBData> uwbData;

    UWBVideoData() {}

    UWBVideoData(const VideoData& videoData, const std::vector<UWBData>& uwbData): videoData(videoData), uwbData(uwbData) {}

    UWBVideoData(VideoData&& videoData, std::vector<UWBData>&& uwbData): videoData(std::move(videoData)), uwbData(std::move(uwbData)) {}

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
