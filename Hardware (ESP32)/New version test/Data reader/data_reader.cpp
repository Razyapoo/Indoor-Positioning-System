#include "data_reader.hpp"
int DataReader::currentFrameIdx = 0;
cv::VideoCapture DataReader::cap;
bool DataReader::isTrackbarActive = false;

void DataReader::onTrackbarChange(int position, void *userdata)
{
    currentFrameIdx = position;
    cap.set(cv::CAP_PROP_POS_FRAMES, position);
    isTrackbarActive = true;
}

std::vector<UWBData> DataReader::readUWBData(const std::string &filename)
{
    std::vector<UWBData> data;
    std::ifstream file(filename);
    UWBData record;
    std::string line;
    Anchor anchor;

    // while (file >> temp.id >> temp.timestamp >> temp.tagID >> temp.anchor101.anchorID >> temp.anchor101.distance >> temp.anchor102.anchorID >> temp.anchor102.distance)
    while (std::getline(file, line, '\n'))
    {
        std::istringstream ss(line);

        ss >> record.id >> record.timestamp >> record.tagID;

        record.anchorList.clear();
        while (ss >> anchor.anchorID >> anchor.distance)
        {
            record.anchorList.push_back(anchor);
        }

        data.push_back(record);
    }

    file.close();

    return data;
}

UWBData DataReader::findUWBMeasurement(const std::vector<UWBData> &uwbData, long long frameTimestamp)
{
    UWBData closestUWB = uwbData[0];
    long long minDiff = std::abs(frameTimestamp - closestUWB.timestamp);

    for (const auto &data : uwbData)
    {
        long long diff = std::abs(frameTimestamp - data.timestamp);
        if (diff < minDiff)
        {
            closestUWB = data;
            minDiff = diff;
        }
    }

    return closestUWB;
}

void DataReader::runDataReader()
{
    cap = cv::VideoCapture("/home/oskar/Documents/Master Thesis/Hardware (ESP32)/New version test/Data reader/data/Experiment 3.1 - Anchor 101, Tag 1/video_from_left_camera.mp4");

    if (!cap.isOpened())
    {
        throw std::runtime_error("Error opening video file");
    }

    std::vector<VideoData> videoData;
    std::ifstream videoDataFile("/home/oskar/Documents/Master Thesis/Hardware (ESP32)/New version test/Data reader/data/Experiment 3.1 - Anchor 101, Tag 1/timestamp.txt");
    VideoData frameData;

    while (videoDataFile >> frameData.id >> frameData.timestamp)
    {
        videoData.push_back(frameData);
    }

    int totalFrames = (int)cap.get(cv::CAP_PROP_FRAME_COUNT);
    // int currentFrameIdx = 0;
    long long frameTimestamp = 0;
    cv::Mat frame;

    cv::namedWindow("Video");
    cv::createTrackbar("Frame", "Video", nullptr, totalFrames, onTrackbarChange);

    std::vector<UWBData> uwbData = readUWBData("/home/oskar/Documents/Master Thesis/Hardware (ESP32)/New version test/Data reader/data/Experiment 3.1 - Anchor 101, Tag 1/timestamp_ESP32.txt");

    bool isPaused = false;
    while (true)
    {
        if (!isPaused || (isPaused && isTrackbarActive))
        {
            if (isTrackbarActive)
            {
                cap.set(cv::CAP_PROP_POS_FRAMES, currentFrameIdx);
                isTrackbarActive = false;
            }
            else
            {
                currentFrameIdx = cap.get(cv::CAP_PROP_POS_FRAMES);
            }
            cap.read(frame);

            if (frame.empty())
                break;

            cv::resize(frame, frame, cv::Size(1280, 960));
            if (currentFrameIdx < videoData.size())
            {
                frameTimestamp = videoData[currentFrameIdx].timestamp;
                std::string frameTimestampText = "Timestamp: " + std::to_string(frameTimestamp);
                cv::putText(frame, frameTimestampText, cv::Point(50, 50), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 255), 2);
            }

            UWBData closestUWB = findUWBMeasurement(uwbData, frameTimestamp);

            std::string uwbText = "ID: " + closestUWB.id +
                                  ", uwb_timestamp: " + std::to_string(closestUWB.timestamp) +
                                  ", TagID: " + std::to_string(closestUWB.tagID);

            for (const auto &anchor : closestUWB.anchorList)
            {
                uwbText += ", AnchorID: " + std::to_string(closestUWB.anchorList[0].anchorID) +
                           ", Anchor Distance: " + std::to_string(closestUWB.anchorList[0].distance);
            }

            cv::putText(frame, uwbText, cv::Point(50, 100), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 255), 2);

            cv::imshow("Video", frame);
            cv::setTrackbarPos("Frame", "Video", currentFrameIdx + 1);
            isTrackbarActive = false;
        }
        else
        {
        }

        int key = cv::waitKey(25);

        if (key == 27)
        {
            break;
        }
        else if (key == 32)
        {
            isPaused = !isPaused;
        }
    }

    cap.release();
    cv::destroyAllWindows();
}

int main()
{
    DataReader::runDataReader();

    return 0;
}