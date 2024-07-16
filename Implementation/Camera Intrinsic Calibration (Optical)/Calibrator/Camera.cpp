#include "Camera.h"

int Camera::cameraIndex;
cv::VideoCapture Camera::camera;

void Camera::initCamera(const int &cameraIndex)
{
    Camera::cameraIndex = cameraIndex;

    camera = cv::VideoCapture(cameraIndex);

    if (!camera.isOpened())
    {
        throw std::runtime_error("Failed to open camera " + std::to_string(cameraIndex));
    }
}

cv::Mat Camera::getFrame()
{
    cv::Mat readFrame;
    if (!camera.read(readFrame))
    {
        throw std::runtime_error("Failed to read the frame from the camera " + std::to_string(Camera::cameraIndex));
    }

    return readFrame;
}

void Camera::release()
{
    camera.release();
    cv::destroyAllWindows();
}