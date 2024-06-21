#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <QImage>
#include <opencv2/opencv.hpp>

enum ExportType {
    FrameByFrameExport,
    SegmentFramesExport
};

enum CoordinateWindowObjectType {
    Tag,
    Person
};

struct AnchorPosition {
    int anchorID;
    qreal x;
    qreal y;
    bool isOrigin;

    AnchorPosition() : anchorID(0), x(0.0), y(0.0), isOrigin(false) {}
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
    std::optional<double> measurementTime;
    std::vector<Anchor> anchorList;
    QPointF coordinates;

    UWBData() : id(-1), timestamp(0), tagID(-1), measurementTime(std::nullopt), coordinates() {}

    UWBData(int id, long long timestamp, int tagID, std::optional<double> measurementTime, const std::vector<Anchor>& anchors, const QPointF& coordinates = QPointF())
        : id(id), timestamp(timestamp), tagID(tagID), measurementTime(measurementTime), anchorList(anchors), coordinates(coordinates) {}

    UWBData(const UWBData& other)
        : id(other.id), timestamp(other.timestamp), tagID(other.tagID), measurementTime(other.measurementTime), anchorList(other.anchorList), coordinates(other.coordinates) {}

    UWBData(UWBData&& other) noexcept
        : id(std::exchange(other.id, -1)), timestamp(std::exchange(other.timestamp, 0)), tagID(std::exchange(other.tagID, -1)), measurementTime(std::move(other.measurementTime)), anchorList(std::move(other.anchorList)), coordinates(std::move(other.coordinates)) {}

    UWBData& operator=(UWBData&& other) noexcept {
        if (this != &other) {
            id = std::exchange(other.id, -1);
            timestamp = std::exchange(other.timestamp, 0);
            tagID = std::exchange(other.tagID, -1);
            measurementTime = other.measurementTime;
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
            measurementTime = std::move(other.measurementTime);
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
    std::vector<QPointF> pixelToRealCoordinates;
    std::vector<QPointF> opticalCoordinates;

    UWBVideoData() {}

    UWBVideoData(const VideoData& videoData, const std::vector<UWBData>& uwbData, const std::vector<QPointF>& pixelToRealCoordinates, const std::vector<QPointF>& opticalCoordinates): videoData(videoData), uwbData(uwbData), pixelToRealCoordinates(pixelToRealCoordinates), opticalCoordinates(opticalCoordinates) {}

    UWBVideoData(VideoData&& videoData, std::vector<UWBData>&& uwbData, std::vector<QPointF>&& pixelToRealCoordinates, std::vector<QPointF>&& opticalCoordinates): videoData(std::move(videoData)), uwbData(std::move(uwbData)), pixelToRealCoordinates(std::move(pixelToRealCoordinates)), opticalCoordinates(std::move(opticalCoordinates)) {}

    UWBVideoData(const UWBVideoData& other): videoData(other.videoData), uwbData(other.uwbData), pixelToRealCoordinates(other.pixelToRealCoordinates), opticalCoordinates(other.opticalCoordinates) {}

    UWBVideoData(UWBVideoData&& other) noexcept: videoData(std::move(other.videoData)), uwbData(std::move(other.uwbData)), pixelToRealCoordinates(std::move(other.pixelToRealCoordinates)), opticalCoordinates(std::move(other.opticalCoordinates)) {}

    UWBVideoData& operator=(UWBVideoData&& other) noexcept {
        if (this != &other) {
            videoData = std::move(other.videoData);
            uwbData = std::move(other.uwbData);
            pixelToRealCoordinates = std::move(other.pixelToRealCoordinates);
            opticalCoordinates = std::move(other.opticalCoordinates);
        }

        return *this;
    }

    ~UWBVideoData() {}
};

struct DetectionResult {
    QPoint bottomEdgeCenter;
    cv::Rect bbox;

    DetectionResult(const QPoint& bottomEdgeCenter, const cv::Rect& bbox): bottomEdgeCenter(bottomEdgeCenter), bbox(bbox) {}

    DetectionResult(QPoint&& bottomEdgeCenter, cv::Rect&& bbox): bottomEdgeCenter(std::move(bottomEdgeCenter)), bbox(std::move(bbox)) {}

    DetectionResult(const DetectionResult& other): bottomEdgeCenter(other.bottomEdgeCenter), bbox(other.bbox) {}

    DetectionResult(DetectionResult&& other) noexcept: bottomEdgeCenter(std::move(other.bottomEdgeCenter)), bbox(std::move(other.bbox)) {}

};

struct DetectionData {
    cv::Size cameraFrameSize, detectionFrameSize;
    std::vector<DetectionResult> detectionResults;

    DetectionData(const std::vector<DetectionResult>& detectionResults, const cv::Size& cameraFrameSize, const cv::Size& detectionFrameSize): detectionResults(detectionResults), cameraFrameSize(cameraFrameSize), detectionFrameSize(detectionFrameSize) {}

    DetectionData(std::vector<DetectionResult>&& detectionResults, cv::Size&& cameraFrameSize, cv::Size&& detectionFrameSize): detectionResults(std::move(detectionResults)), cameraFrameSize(std::move(cameraFrameSize)), detectionFrameSize(std::move(detectionFrameSize)) {}

    DetectionData(const DetectionData& other): detectionResults(other.detectionResults), cameraFrameSize(other.cameraFrameSize), detectionFrameSize(other.detectionFrameSize) {}

    DetectionData(DetectionData&& other) noexcept: detectionResults(std::move(other.detectionResults)), cameraFrameSize(std::move(other.cameraFrameSize)), detectionFrameSize(std::move(other.detectionFrameSize)) {}

};


enum PredictionType {
    PredictionByOptical,
    PredictionByPixelToReal
};

#endif // STRUCTURES_H
