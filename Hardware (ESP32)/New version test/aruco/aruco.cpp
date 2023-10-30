#include <iostream>
#include <fstream>
#include <opencv2/aruco.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

void generateAruco()
{
    cv::Mat markerImage;
    std::string markerFileName;
    int dpi = 300;
    int markerSize = 200; // mm
    int markerSizePixels = (markerSize / 25.4) * dpi;
    int markerId;
    cv::Ptr<cv::aruco::Dictionary> dict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

    for (size_t i = 1; i < 10; i++)
    {
        markerId = i;
        cv::aruco::drawMarker(dict, i, markerSizePixels, markerImage);
        markerFileName = "aruco_marker_" + std::to_string(markerId) + ".png";
        cv::imwrite(markerFileName, markerImage);
    }
}

void detectAruco()
{
    cv::Mat markerImage;
    std::string markerFileName;
    int dpi = 300;
    int markerSize = 200; // mm
    int markerSizePixels = (markerSize / 25.4) * dpi;
    int markerId;
    cv::Ptr<cv::aruco::Dictionary> dict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    std::chrono::milliseconds currentTime;
    std::time_t timestamp;

    cv::namedWindow("Marker", 0);

    // cv::VideoCapture cap(2);
    std::vector<std::vector<cv::Point2f>> corners;
    std::vector<int> ids;
    int key;
    std::string videoRecordId, videoTimestamp; // video
    std::string beaconRecordId;

    // Video
    std::string filePath = "../video_from_left_camera.avi";
    cv::VideoCapture cap(filePath);
    // cv::VideoCapture cap(2);
    std::ofstream timestampFile("../aruco_video_detected_ids.txt");
    size_t frameIndex = 1;

    // Beacons
    // std::ifstream inputBeaconTimestamps("../temp.txt");
    // std::string beaconTimestamp, distanceFromAnchor101, distanceFromAnchor102;

    while (true)
    {
        cap >> markerImage;
        // inputVideoTimestamps >> videoRecordId >> videoTimestamp;
        // inputBeaconTimestamps >> beaconTimestamp >> distanceFromAnchor101 >> distanceFromAnchor102;
        cv::aruco::detectMarkers(markerImage, dict, corners, ids);
        if (ids.size() > 0)
        {
            cv::aruco::drawDetectedMarkers(markerImage, corners, ids);
            std::cout << "ID of the detected marker: " << ids[0] << "\n";
            // currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
            // timestamp = currentTime.count();
            // // timestamp = std::time({});
            timestampFile << frameIndex << " " << ids[0] << std::endl;

            // std::cout << "Frame " << frameIndex << " is recorded" << std::endl;
            frameIndex++;
        }

        cv::imshow("Marker", markerImage);
        key = cv::waitKey(1);
        if (key == 'x')
            break;
    }
}

int main()
{
    detectAruco();
    // cv::namedWindow("Aruco", 0);
    // cv::imshow("Aruco", image);

    // cv::waitKey(0);
}
