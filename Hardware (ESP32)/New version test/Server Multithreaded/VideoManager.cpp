#include "VideoManager.hpp"

size_t VideoManager::frameIndex = 1;
const double VideoManager::fps = 25.0;
const cv::Size VideoManager::frameSize = cv::Size(640, 360);
// bool VideoManager::isPause = false;

cv::Mat VideoManager::leftFrame, VideoManager::rightFrame;
std::chrono::milliseconds VideoManager::currentTime;
std::time_t VideoManager::timestamp;
cv::Mat VideoManager::timestampMat;
uint8_t VideoManager::key;

extern SharedData sharedData;

void VideoManager::videoRecorder()
{

    // cv::Size frameSize(StereoCamera::leftCamera.get(cv::CAP_PROP_FRAME_WIDTH), StereoCamera::leftCamera.get(cv::CAP_PROP_FRAME_HEIGHT));
    // cv::VideoWriter leftVideoWriter("video_from_left_camera.avi", cv::VideoWriter::fourcc('X', 'V', 'I', 'D'), fps, frameSize);

    cv::VideoWriter leftVideoWriter("video_from_left_camera.avi", cv::VideoWriter::fourcc('H', '2', '6', '4'), fps, frameSize);
    // cv::VideoWriter rightVideoWriter("video_from_right_camera.avi", cv::VideoWriter::fourcc('H', '2', '6', '4'), fps, frameSize);

    if (!leftVideoWriter.isOpened())
    {
        std::cerr << "Failed to open left video writer" << std::endl;
        return;
    }

    // if (!rightVideoWriter.isOpened())
    // {
    //     std::cerr << "Failed to open right video writer" << std::endl;
    //     return;
    // }

    std::ofstream timestampFile("timestamp.txt");
    if (!timestampFile.is_open())
        throw std::runtime_error("Failed to open timestamp.txt file");

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
            leftFrame = StereoCamera::getLeftFrame();
            // rightFrame = StereoCamera::getRightFrame();

            leftVideoWriter.write(leftFrame);
            // rightVideoWriter.write(rightFrame);

            // currentTime = std::chrono::system_clock::now();
            currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
            timestamp = currentTime.count();
            // timestamp = std::time({});
            timestampFile << frameIndex << " " << timestamp << std::endl;

            // std::cout << "Frame " << frameIndex << " is recorded" << std::endl;
            frameIndex++;
        }

        cv::imshow("Left frame", leftFrame);
        // cv::imshow("Right frame", rightFrame);

        key = cv::waitKey(1);
        if (key == 'p')
            // isPause = true;
            sharedData.pauseRecording();
        if (key == 'c')
            // isPause = false;
            sharedData.startRecording();
        if (key == 's')
        {
            sharedData.setTerminationFlag();
            break;
        }

        // std::cout << "Press \"p + Enter\" to pause recording" << std::endl;
        // std::cout << "Press \"c + Enter\" to continue recording" << std::endl;
        // std::cout << "Press \"s + Enter\" to stop recording and save video" << std::endl;

        // std::string input;
        // std::getline(std::cin, input);
        // if (input == "p") {
        //     isPause = true;
        // }
        // else if (input == "c") {
        //     isPause = false;
        // }
        // else if (input == "s") {
        //     break;
        // }
        // else {
        //     std::cout << "Invalid option" << std::endl;
        //     return;
        // }
    }

    std::cout << "Saving video! Please wait..." << std::endl;
    try
    {
        leftVideoWriter.release();
        // rightVideoWriter.release();
        timestampFile.close();
        std::cout << "Video saved successfully!" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

// void VideoManager::videoLoader()
// {
//     cv::VideoCapture leftvideoLoader("video_from_left_camera.avi");
//     cv::VideoCapture rightvideoLoader("video_from_right_camera.avi");

//     if (!leftvideoLoader.isOpened())
//         throw std::runtime_error("Failed to open video from left camera");

//     if (!rightvideoLoader.isOpened())
//         throw std::runtime_error("Failed to open video from right camera");

//     std::ifstream timestampFile("timestamp.txt");
//     if (!timestampFile.is_open())
//         throw std::runtime_error("Failed to open timestamp.txt file");

//     leftvideoLoader.set(cv::CAP_PROP_FPS, fps);
//     rightvideoLoader.set(cv::CAP_PROP_FPS, fps);

//     while (true)
//     {

//         leftvideoLoader >> leftFrame;
//         rightvideoLoader >> rightFrame;

//         if (leftFrame.empty() || rightFrame.empty())
//         {
//             std::cout << "End of video file" << std::endl;
//             break;
//         }

//         cv::imshow("Left Frame", leftFrame);
//         cv::imshow("Right Frame", rightFrame);

//         timestampFile >> frameIndex >> timestamp;

//         // convert the absolute time to a cv::Mat timestamp
//         // timestampMat = cv::Mat::zeros(1, 1, CV_64F);
//         // *timestampMat.ptr<double>(0) = static_cast<double>(std::chrono::system_clock::to_time_t(std::chrono::system_clock::from_time_t(timestamp)));

//         std::cout << "Left frame with index: " << frameIndex << " was captured at " << std::ctime(&timestamp) << std::endl;
//         std::cout << "Right frame with index: " << frameIndex << " was captured at " << std::ctime(&timestamp) << std::endl;

//         key = cv::waitKey(40); // correspond to 25 fps
//         if (key == 'q')
//             break;
//     }

//     leftvideoLoader.release();
//     rightvideoLoader.release();
//     cv::destroyAllWindows();
// }