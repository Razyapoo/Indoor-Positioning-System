#include "VideoManager.hpp"

size_t VideoManager::frameIndex = 1;
double VideoManager::fps = 18.0;
cv::Size VideoManager::frameSize = cv::Size(640, 360);

cv::Mat VideoManager::frame;
std::chrono::milliseconds VideoManager::currentTime;
std::time_t VideoManager::timestamp;
cv::Mat VideoManager::timestampMat;
uint8_t VideoManager::key;

extern SharedData sharedData;

void VideoManager::runVideoRecorder()
{
    // Setup of the video parameters
    cv::VideoWriter videoWriter("video.avi", cv::VideoWriter::fourcc('H', '2', '6', '4'), fps, frameSize);

    if (!videoWriter.isOpened())
    {
        std::cerr << "Failed to open video writer" << std::endl;
        return;
    }

    // Open the index file
    std::ofstream timestampFile("video_timestamps.txt");
    if (!timestampFile.is_open())
        throw std::runtime_error("Failed to open video_timestamps.txt file");

    // Start recording both UWB and Video streams
    sharedData.startRecording();

    std::cout << "Video is recording..." << std::endl;
    std::cout << "Possible interactions" << std::endl;
    std::cout << "  p: pause recording" << std::endl;
    std::cout << "  c: continue recording" << std::endl;
    std::cout << "  s: stop and save recording" << std::endl;

    while (true)
    {
        if (!sharedData.isRecordingPaused())
        {
            frame = Camera::getFrame();

            if (frame.empty())
                break;

            videoWriter.write(frame);

            // record timestamp + frameIndex of the video frame for later synchronization with UWB records
            currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
            timestamp = currentTime.count();
            timestampFile << frameIndex << " " << timestamp << std::endl;

            frameIndex++;
        }

        cv::imshow("Frame", frame);

        key = cv::waitKey(60); // corresponds to 17 fps
        if (key == 'p')
            sharedData.pauseRecording();
        if (key == 'c') // continue recording
            sharedData.startRecording();
        if (key == 's')
        {
            sharedData.setTerminationFlag(); // notify UWB server about termination
            break;
        }
    }

    std::cout << "Saving video! Please wait..." << std::endl;
    try
    {

        videoWriter.release();
        timestampFile.close();
        std::cout << "Video has been saved successfully!" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
