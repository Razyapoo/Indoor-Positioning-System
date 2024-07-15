#include "Camera.hpp"

int Camera::cameraIndex;
cv::Mat Camera::frame;
cv::VideoCapture Camera::camera;

void Camera::initCamera(const int &cameraIndex)
{
    Camera::cameraIndex = cameraIndex;
    camera = cv::VideoCapture(cameraIndex);

    // safety check
    if (!camera.isOpened())
    {
        throw std::runtime_error("Failed to open camera " + std::to_string(cameraIndex));
    }
}

void Camera::release()
{
    camera.release();
    cv::destroyAllWindows();
}

cv::Mat &Camera::getFrame()
{
    if (!camera.read(frame))
    {
        throw std::runtime_error("Failed to read the frame from the camera " + std::to_string(Camera::cameraIndex));
    }

    return frame;
}

cv::Size Camera::getCameraSize()
{
    return cv::Size(camera.get(cv::CAP_PROP_FRAME_WIDTH), camera.get(cv::CAP_PROP_FRAME_HEIGHT));
}

double Camera::getCameraFPS()
{
    return camera.get(cv::CAP_PROP_FPS);
}