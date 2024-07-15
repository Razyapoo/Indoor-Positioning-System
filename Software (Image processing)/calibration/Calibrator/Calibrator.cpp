#include "Calibrator.hpp"

cv::VideoCapture Calibrator::videoSource;

bool Calibrator::intrinsicParamsSaved = false;
std::string Calibrator::intrinsicFilePath = "";
uint8_t Calibrator::chessboardHeight = 6, Calibrator::chessboardWidth = 9;
uint16_t Calibrator::imageCounter = 0;
float Calibrator::squareSize = 24.0f, Calibrator::alpha = 0.97;
const cv::Size Calibrator::chessboardSize = cv::Size(Calibrator::chessboardHeight, Calibrator::chessboardWidth);
const cv::Size Calibrator::imageSize = cv::Size(640, 360);
const cv::TermCriteria Calibrator::criteriaMono = cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.01); //cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 100, 0.001
const int Calibrator::numberOfCells = Calibrator::chessboardHeight * Calibrator::chessboardWidth;

double Calibrator::Calibrator::reprojectionError;
std::vector<std::vector<cv::Point3f>> Calibrator::objectPoints;
std::vector<std::vector<cv::Point2f>> Calibrator::imagePoints;
std::vector<cv::Point2f> Calibrator::corners;
std::vector<cv::Point3f> Calibrator::objectPoint;
// Working with frames
cv::Mat Calibrator::image, Calibrator::gray;
bool Calibrator::found;
// Intrinsic calibration
cv::Mat Calibrator::cameraMatrix, Calibrator::distortionCoeffs;
std::vector<cv::Mat> Calibrator::rotationVecs, Calibrator::translationVecs;
cv::Mat Calibrator::optimalCameraMatrix;
uint8_t Calibrator::key;

void Calibrator::initCameraCalibration()
{

    for (auto i = 0; i < chessboardWidth; i++)
        for (auto j = 0; j < chessboardHeight; j++)
            objectPoint.push_back(cv::Point3f((float)j * squareSize, (float)i * squareSize, 0.0f));

    char key = -1;
    bool found = false;
    uint16_t imageCounter = 0;

    std::cout << "Stage - IDLE. This stage is intended for your preparation. Please press one of the following buttons to continue: " << std::endl;
    std::cout << "  n: continue to the next stage" << std::endl;
    std::cout << "  x: exit" << std::endl;

    cv::namedWindow("Frame");

    while (true)
    {
        image = Camera::getFrame();

        cv::imshow("Frame", image);

        key = cv::waitKey(1);
        if (key == 'x')
            exit(0);
        if (key == 'n')
        {
            cv::destroyAllWindows();
            Calibrator::detectChessboard();
            break;
        }
    }
}

void Calibrator::detectChessboard()
{

    std::cout << "Stage - DETECT CHESSBOARD" << std::endl;
    std::cout << "  n: continue to the next stage" << std::endl;
    std::cout << "  x: exit" << std::endl;
    std::cout << "  s: save the pair of images" << std::endl;
    std::cout << "  c: repeat detection without saving" << std::endl;

    cv::namedWindow("Frame");

    while (true)
    {
        image = Camera::getFrame();

        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

        found = cv::findChessboardCorners(gray, chessboardSize, corners, cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FILTER_QUADS); //, cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_FILTER_QUADS + cv::CALIB_CB_NORMALIZE_IMAGE);

        if (found)
        {
            cv::drawChessboardCorners(image, chessboardSize, corners, found);
            cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1), criteriaMono);
        }

        // // ----- Circles -----
        // found = cv::findCirclesGrid(gray, chessboardSize, corners, cv::CALIB_CB_ASYMMETRIC_GRID); //, cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_FILTER_QUADS + cv::CALIB_CB_NORMALIZE_IMAGE);
        // foundRight = cv::findCirclesGrid(grayRight, chessboardSize, cornersRight, cv::CALIB_CB_ASYMMETRIC_GRID); //, cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_FILTER_QUADS + cv::CALIB_CB_NORMALIZE_IMAGE);

        // // If the pattern is found in both images, draw the circles at the detected corners
        // if (found && foundRight) {
        //     cv::drawChessboardCorners(image, chessboardSize, cv::Mat(corners), found);
        //     cv::drawChessboardCorners(imageRight, chessboardSize, cv::Mat(cornersRight), foundRight);
        //     cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1), criteriaMono);
        //     cv::cornerSubPix(grayRight, cornersRight, cv::Size(11, 11), cv::Size(-1, -1), criteriaMono);
        // }
        // // ----- Circles -----

        cv::imshow("Frame", image);

        key = cv::waitKey(1);

        if (found)
            key = cv::waitKey(0);

        if (key == 'x')
        {
            cv::destroyAllWindows();
            exit(0);
        }
        if (key == 's' && found)
        {
            imagePoints.push_back(corners);
            objectPoints.push_back(objectPoint);
            std::cout << "Number of stored corners: " << ++imageCounter << std::endl;
        }
        if (key == 'c')
        {
            continue;
        }
        if (key == 'n')
        {
            Calibrator::intrinsicCalibration();
            break;
        }
    }
}

void Calibrator::intrinsicCalibration()
{
    std::cout << "Stage - Compute Intrinsic Parameters" << std::endl;

    cameraMatrix = cv::initCameraMatrix2D(objectPoints, imagePoints, imageSize, 0);

    reprojectionError = cv::calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix, distortionCoeffs, rotationVecs, translationVecs);

    bool ok = cv::checkRange(cameraMatrix) && cv::checkRange(distortionCoeffs);
    if (!ok)
    {
        std::cout << "ERROR: camera was not calibrated" << std::endl;
    }

    optimalCameraMatrix = cv::getOptimalNewCameraMatrix(cameraMatrix, distortionCoeffs, imageSize, alpha, imageSize, 0);

    std::cout << "Intrinsic calibration is finished" << std::endl;
    std::cout << "Mean calibration Error Camera: " << cv::sqrt(reprojectionError / imageCounter) << std::endl;

    std::cout << "Reprojection Error Camera: " << Calibrator::getReprojectionError(objectPoints, imagePoints, rotationVecs, translationVecs, cameraMatrix, distortionCoeffs) << std::endl;

    std::cout << "Camera Matrix: " << cameraMatrix << std::endl;
    std::cout << "Optimal Camera Matrix: " << optimalCameraMatrix << std::endl;
    std::cout << "Dist coeffs: " << distortionCoeffs << std::endl;

    std::cout << "Please press one of the following buttons to continue:" << std::endl;
    std::cout << "  x: exit" << std::endl;
    std::cout << "  s: save intrinsic parameters" << std::endl;

    cv::Mat undistortedImage;

    while (true)
    {
        
        
        image = Camera::getFrame();
        cv::undistort(image, undistortedImage, cameraMatrix, distortionCoeffs, optimalCameraMatrix);
        cv::imshow("Frame undistorted", undistortedImage);

        key = cv::waitKey(1);

        if (key == 'x')
        {
            std::cout << "Calibration is finished!!" << std::endl;
            exit(0);
        }
        if (key == 's')
        {
            try
            {
                std::filesystem::path filePath(intrinsicFilePath);
                std::filesystem::create_directories(filePath.parent_path());
                cv::FileStorage fs(intrinsicFilePath, cv::FileStorage::WRITE);
                if (!fs.isOpened())
                {
                    throw std::runtime_error("Failed to open file for writing");
                }
                fs << "cameraMatrix" << cameraMatrix;
                fs << "optimalCameraMatrix" << optimalCameraMatrix;
                fs << "distortionCoeffs" << distortionCoeffs;

                fs.release();
                if (fs.isOpened())
                {
                    throw std::runtime_error("Failed to close file after writing");
                }

                std::cout << "The intrinsic parameters have been saved successfully!" << std::endl;
                intrinsicParamsSaved = true;
                std::cout << "Calibration is finished!!" << std::endl;
                exit(0);
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    }
}

double Calibrator::getReprojectionError(const std::vector<std::vector<cv::Point3f>> &objectPoints, const std::vector<std::vector<cv::Point2f>> &imagePoints, const std::vector<cv::Mat> &rotationVecs, const std::vector<cv::Mat> &translationVecs, const cv::Mat &cameraMatrix, const cv::Mat &distortionCoeffs)
{

    double totalError = 0.0;
    double localError = 0.0;
    double rms = 0.0;
    int numPoints = 0;
    std::vector<cv::Point2f> projectedPoints;

    for (int i = 0; i < objectPoints.size(); i++)
    {
        cv::projectPoints(objectPoints[i], rotationVecs[i], translationVecs[i], cameraMatrix, distortionCoeffs, projectedPoints);
        localError = cv::norm(imagePoints[i], projectedPoints, cv::NORM_L2);

        totalError += localError * localError;
        numPoints += objectPoints[i].size();
    }

    rms = std::sqrt(totalError / numPoints);

    return rms;
}
