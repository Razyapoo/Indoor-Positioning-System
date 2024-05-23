#include "StereoCalibrator.hpp"

// bool StereoCalibrator::videoAsSource = false;
cv::VideoCapture StereoCalibrator::leftVideoSource, StereoCalibrator::rightVideoSource;

bool StereoCalibrator::intrinsicParamsSaved = false, StereoCalibrator::extrinsicParamsSaved = false, StereoCalibrator::rectificationParamsSaved = false;
std::string StereoCalibrator::intrinsicFilePath = "", StereoCalibrator::extrinsicFilePath = "", StereoCalibrator::rectificationFilePath = "";
uint8_t StereoCalibrator::chessboardHeight = 6, StereoCalibrator::chessboardWidth = 9;
uint16_t StereoCalibrator::imageCounter = 0;
float StereoCalibrator::squareSize = 0.024f, StereoCalibrator::alpha = 0;
const cv::Size StereoCalibrator::chessboardSize = cv::Size(StereoCalibrator::chessboardHeight, StereoCalibrator::chessboardWidth);
const cv::Size StereoCalibrator::imageSize = cv::Size(640, 360);
const cv::TermCriteria StereoCalibrator::criteriaMono = cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 100, 0.001);
const cv::TermCriteria StereoCalibrator::criteriaStereo = cv::TermCriteria(cv::TermCriteria::COUNT, 100, 1e-5);
const int StereoCalibrator::numberOfCells = StereoCalibrator::chessboardHeight * StereoCalibrator::chessboardWidth;

double StereoCalibrator::StereoCalibrator::reprojectionErrorLeft, StereoCalibrator::StereoCalibrator::reprojectionErrorRight;
std::vector<std::vector<cv::Point3f>> StereoCalibrator::objectPoints;
std::vector<std::vector<cv::Point2f>> StereoCalibrator::imagePointsLeft, StereoCalibrator::imagePointsRight;
std::vector<cv::Point2f> StereoCalibrator::cornersLeft, StereoCalibrator::cornersRight;
std::vector<cv::Point3f> StereoCalibrator::objectPoint;
// Working with frames
cv::Mat StereoCalibrator::imageLeft, StereoCalibrator::imageRight, StereoCalibrator::grayLeft, StereoCalibrator::grayRight;
bool StereoCalibrator::foundLeft, StereoCalibrator::foundRight;
// Intrinsic calibration
cv::Mat StereoCalibrator::cameraMatrixLeft, StereoCalibrator::cameraMatrixRight, StereoCalibrator::distortionCoeffsLeft, StereoCalibrator::distortionCoeffsRight;
std::vector<cv::Mat> StereoCalibrator::rotationVecsLeft, StereoCalibrator::rotationVecsRight, StereoCalibrator::translationVecsLeft, StereoCalibrator::translationVecsRight;
cv::Mat StereoCalibrator::optimalCameraMatrixLeft, StereoCalibrator::optimalCameraMatrixRight;
// Extrinsic calibration
cv::Mat StereoCalibrator::rotationMatrixCommon, StereoCalibrator::translationVector, StereoCalibrator::essentialMatrix, StereoCalibrator::fundamentalMatrix;
// Rectification
cv::Mat StereoCalibrator::rotationMatrixLeft, StereoCalibrator::rotationMatrixRight, StereoCalibrator::projectionMatrixLeft, StereoCalibrator::projectionMatrixRight, StereoCalibrator::disparityMatrix;
// Undistortion
cv::Mat StereoCalibrator::mapLeftX, StereoCalibrator::mapLeftY, StereoCalibrator::mapRightX, StereoCalibrator::mapRightY;
cv::Mat StereoCalibrator::undistortedImageLeft, StereoCalibrator::undistortedImageRight;
cv::Mat StereoCalibrator::rectifiedPair;
uint8_t StereoCalibrator::key;

void StereoCalibrator::initCameraCalibration()
{

    for (auto i = 0; i < chessboardWidth; i++)
        for (auto j = 0; j < chessboardHeight; j++)
            objectPoint.push_back(cv::Point3f((float)j * squareSize, (float)i * squareSize, 0.0f));

    char key = -1;
    bool foundLeft = false;
    uint16_t imageCounter = 0;

    // StereoCamera cameraLeft = StereoCamera::StereoCamera(0);
    // StereoCamera cameraRight = StereoCamera::StereoCamera(1);

    std::cout << "Stage - IDLE. This stage is intended for your preparation. Please press one of the following buttons to continue: " << std::endl;
    std::cout << "  n: continue to the next stage" << std::endl;
    std::cout << "  x: exit" << std::endl;

    cv::namedWindow("Left Frame");

    while (true)
    {
        imageLeft = StereoCamera::getLeftFrame();

        // std::cout << "Image size: " << imageLeft.size() << std::endl;
        cv::imshow("Left Frame", imageLeft);

        key = cv::waitKey(1);
        if (key == 'x')
            exit(0);
        if (key == 'n')
        {
            cv::destroyAllWindows();
            StereoCalibrator::detectChessboard();
            break;
        }
    }
}

void StereoCalibrator::detectChessboard()
{

    std::cout << "Stage - DETECT CHESSBOARD" << std::endl;
    std::cout << "  n: continue to the next stage" << std::endl;
    std::cout << "  x: exit" << std::endl;
    std::cout << "  s: save the pair of images" << std::endl;
    std::cout << "  c: repeat detection without saving" << std::endl;

    cv::namedWindow("Left Frame");

    while (true)
    {
        imageLeft = StereoCamera::getLeftFrame();

        cv::cvtColor(imageLeft, grayLeft, cv::COLOR_BGR2GRAY);

        foundLeft = cv::findChessboardCorners(grayLeft, chessboardSize, cornersLeft, cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FILTER_QUADS); //, cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_FILTER_QUADS + cv::CALIB_CB_NORMALIZE_IMAGE);

        if (foundLeft)
        {
            // cv::TermCriteria criteria(cv::CV_TERMCRIT_EPS | cv::CV_TERMCRIT_ITER, 30, 0.01);
            cv::drawChessboardCorners(imageLeft, chessboardSize, cornersLeft, foundLeft);
            cv::cornerSubPix(grayLeft, cornersLeft, cv::Size(11, 11), cv::Size(-1, -1), criteriaMono);
        }

        // // ----- Circles -----
        // foundLeft = cv::findCirclesGrid(grayLeft, chessboardSize, cornersLeft, cv::CALIB_CB_ASYMMETRIC_GRID); //, cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_FILTER_QUADS + cv::CALIB_CB_NORMALIZE_IMAGE);
        // foundRight = cv::findCirclesGrid(grayRight, chessboardSize, cornersRight, cv::CALIB_CB_ASYMMETRIC_GRID); //, cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_FILTER_QUADS + cv::CALIB_CB_NORMALIZE_IMAGE);

        // // If the pattern is found in both images, draw the circles at the detected corners
        // if (foundLeft && foundRight) {
        //     cv::drawChessboardCorners(imageLeft, chessboardSize, cv::Mat(cornersLeft), foundLeft);
        //     cv::drawChessboardCorners(imageRight, chessboardSize, cv::Mat(cornersRight), foundRight);
        //     cv::cornerSubPix(grayLeft, cornersLeft, cv::Size(11, 11), cv::Size(-1, -1), criteriaMono);
        //     cv::cornerSubPix(grayRight, cornersRight, cv::Size(11, 11), cv::Size(-1, -1), criteriaMono);
        // }
        // // ----- Circles -----

        cv::imshow("Left Frame", imageLeft);

        key = cv::waitKey(1);

        if (foundLeft)
            key = cv::waitKey(0);

        if (key == 'x')
        {
            cv::destroyAllWindows();
            exit(0);
        }
        if (key == 's' && foundLeft)
        {
            imagePointsLeft.push_back(cornersLeft);
            objectPoints.push_back(objectPoint);
            std::cout << "Number of stored corners: " << ++imageCounter << std::endl;
        }
        if (key == 'c')
        {
            continue;
        }
        if (key == 'n')
        {
            StereoCalibrator::intrinsicCalibration();
            break;
        }
    }
}

void StereoCalibrator::intrinsicCalibration()
{
    // cv::TermCriteria criteria(cv::CV_TERMCRIT_EPS | cv::CV_TERMCRIT_ITER, 30, 0.1);
    std::cout << "Stage - Compute Intrinsic Parameters" << std::endl;

    cameraMatrixLeft = cv::initCameraMatrix2D(objectPoints, imagePointsLeft, imageSize, 0);

    reprojectionErrorLeft = cv::calibrateCamera(objectPoints, imagePointsLeft, imageSize, cameraMatrixLeft, distortionCoeffsLeft, rotationVecsLeft, translationVecsLeft);

    bool ok = cv::checkRange(cameraMatrixLeft) && cv::checkRange(distortionCoeffsLeft);
    if (!ok)
    {
        std::cout << "ERROR: left camera was not calibrated" << std::endl;
    }

    optimalCameraMatrixLeft = cv::getOptimalNewCameraMatrix(cameraMatrixLeft, distortionCoeffsLeft, imageSize, alpha, imageSize, 0);

    std::cout << "Intrinsic calibration is finished" << std::endl;
    std::cout << "Mean calibration Error Left Camera: " << cv::sqrt(reprojectionErrorLeft / imageCounter) << std::endl;

    std::cout << "Reprojection Error Left Camera: " << StereoCalibrator::getReprojectionError(objectPoints, imagePointsLeft, rotationVecsLeft, translationVecsLeft, cameraMatrixLeft, distortionCoeffsLeft) << std::endl;

    std::cout << "Please press one of the following buttons to continue:" << std::endl;
    std::cout << "  n: continue to the next stage" << std::endl;
    std::cout << "  x: exit" << std::endl;
    std::cout << "  s: save intrinsic parameters" << std::endl;

    while (true)
    {

        key = cv::waitKey(1);

        if (key == 'x')
            exit(0);
        if (key == 's')
        {
            try
            {
                cv::FileStorage fs(intrinsicFilePath, cv::FileStorage::WRITE);
                if (!fs.isOpened())
                {
                    throw std::runtime_error("Failed to open file for writing");
                }
                fs << "cameraMatrixLeft" << cameraMatrixLeft;
                fs << "optimalCameraMatrixLeft" << optimalCameraMatrixLeft;
                fs << "distortionCoeffsLeft" << distortionCoeffsLeft;

                fs.release();
                if (fs.isOpened())
                {
                    throw std::runtime_error("Failed to close file after writing");
                }

                std::cout << "The intrinsic parameters have been saved successfully!" << std::endl;
                intrinsicParamsSaved = true;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
        if (key == 'n')
        {
            StereoCalibrator::extrinsicCalibration();
            break;
        }
    }
}

void StereoCalibrator::extrinsicCalibration()
{
    std::cout << "Stage - Compute Extrinsic Parameters" << std::endl;

    // int flags = cv::CALIB_FIX_INTRINSIC + cv::CALIB_USE_INTRINSIC_GUESS + cv::CALIB_FIX_PRINCIPAL_POINT + cv::CALIB_FIX_FOCAL_LENGTH + cv::CALIB_RATIONAL_MODEL;
    double stereoReprojectionError = cv::stereoCalibrate(objectPoints, imagePointsLeft, imagePointsRight, cameraMatrixLeft, distortionCoeffsLeft, cameraMatrixRight, distortionCoeffsRight, imageSize, rotationMatrixCommon, translationVector, essentialMatrix, fundamentalMatrix, cv::CALIB_FIX_INTRINSIC + cv::CALIB_USE_INTRINSIC_GUESS + cv::CALIB_FIX_PRINCIPAL_POINT + cv::CALIB_FIX_FOCAL_LENGTH + cv::CALIB_RATIONAL_MODEL, criteriaStereo);

    std::cout << "Extrinsic calibration is finished" << std::endl;
    std::cout << "Stereo calibration error: " << cv::sqrt(stereoReprojectionError / (imageCounter * 2)) << std::endl;

    std::cout << "Please press one of the following buttons to continue:" << std::endl;
    std::cout << "  n: continue to the next stage" << std::endl;
    std::cout << "  x: exit" << std::endl;
    std::cout << "  s: save extrinsic parameters" << std::endl;

    while (true)
    {

        key = cv::waitKey(1);

        if (key == 'x')
            exit(0);
        if (key == 's')
        {
            try
            {
                cv::FileStorage fs(extrinsicFilePath, cv::FileStorage::WRITE);

                if (!fs.isOpened())
                {
                    throw std::runtime_error("Failed to open file for writing");
                }

                fs << "rotationMatrixCommon" << rotationMatrixCommon;
                fs << "translationVector" << translationVector;
                fs << "essentialMatrix" << essentialMatrix;
                fs << "fundamentalMatrix" << fundamentalMatrix;

                fs.release();

                if (fs.isOpened())
                {
                    throw std::runtime_error("Failed to close file after writing");
                }

                std::cout << "The extrinsic parameters have been saved successfully!" << std::endl;
                extrinsicParamsSaved = true;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error: " << e.what() << std::endl;
            }

            if (!intrinsicParamsSaved)
            {
                try
                {
                    cv::FileStorage fs(intrinsicFilePath, cv::FileStorage::WRITE);
                    if (!fs.isOpened())
                    {
                        throw std::runtime_error("Failed to open file for writing");
                    }
                    fs << "cameraMatrixLeft" << cameraMatrixLeft;
                    fs << "cameraMatrixRight" << cameraMatrixRight;
                    fs << "optimalCameraMatrixLeft" << optimalCameraMatrixLeft;
                    fs << "optimalCameraMatrixRight" << optimalCameraMatrixRight;
                    fs << "distortionCoeffsLeft" << distortionCoeffsLeft;
                    fs << "distortionCoeffsRight" << distortionCoeffsRight;

                    fs.release();
                    if (fs.isOpened())
                    {
                        throw std::runtime_error("Failed to close file after writing");
                    }

                    std::cout << "The intrinsic parameters have been saved successfully!" << std::endl;
                    intrinsicParamsSaved = true;
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
            }
        }
        if (key == 'n')
        {
            StereoCalibrator::stereoRectification();
            break;
        }
    }
}

void StereoCalibrator::stereoRectification()
{
    std::cout << "Stage - Stereo rectification" << std::endl;
    cv::stereoRectify(cameraMatrixLeft, distortionCoeffsLeft, cameraMatrixRight, distortionCoeffsRight, imageSize, rotationMatrixCommon, translationVector, rotationMatrixLeft, rotationMatrixRight, projectionMatrixLeft, projectionMatrixRight, disparityMatrix, cv::CALIB_ZERO_DISPARITY, -1, imageSize);
    std::cout << "Stereo rectification is finished" << std::endl;

    std::cout << "Undistortion..." << std::endl;
    cv::initUndistortRectifyMap(cameraMatrixLeft, distortionCoeffsLeft, rotationMatrixLeft, projectionMatrixLeft, imageSize, CV_16SC2, mapLeftX, mapLeftY);
    cv::initUndistortRectifyMap(cameraMatrixRight, distortionCoeffsRight, rotationMatrixRight, projectionMatrixRight, imageSize, CV_16SC2, mapRightX, mapRightY);
    std::cout << "Undistortion is finished" << std::endl;

    std::cout << "Result of undistortion:" << std::endl;
    std::cout << "Please press one of the following buttons to continue:" << std::endl;
    std::cout << "  n: continue to the next stage" << std::endl;
    std::cout << "  x: exit" << std::endl;
    std::cout << "  s: save rectification parameters" << std::endl;

    cv::namedWindow("Left Frame");
    cv::namedWindow("Right Frame");
    cv::namedWindow("Rectified pair");

    while (true)
    {
        imageLeft = StereoCamera::getLeftFrame();
        imageRight = StereoCamera::getRightFrame();

        // cv::cvtColor(grayLeft, imageLeft, cv::COLOR_BGR2RGB);
        // cv::cvtColor(grayRight, imageRight, cv::COLOR_BGR2RGB);

        cv::remap(imageLeft, undistortedImageLeft, mapLeftX, mapLeftY, cv::INTER_LINEAR);
        cv::remap(imageRight, undistortedImageRight, mapRightX, mapRightY, cv::INTER_LINEAR);

        // Show separately
        cv::imshow("Left Frame", imageLeft);
        cv::imshow("Right Frame", imageRight);

        // Show together
        rectifiedPair = cv::Mat::zeros(undistortedImageLeft.rows, undistortedImageLeft.cols * 2, undistortedImageLeft.type());

        undistortedImageLeft.copyTo(rectifiedPair(cv::Range::all(), cv::Range(0, undistortedImageLeft.cols)));
        undistortedImageRight.copyTo(rectifiedPair(cv::Range::all(), cv::Range(undistortedImageLeft.cols, undistortedImageLeft.cols * 2)));

        for (auto i = 0; i < undistortedImageRight.rows; i += 10)
        {
            cv::Point start = cv::Point(0, i);
            cv::Point end = cv::Point(rectifiedPair.cols, i);
            cv::line(rectifiedPair, start, end, cv::Scalar(0, 255, 0));
        }

        cv::imshow("Rectified pair", rectifiedPair);

        key = cv::waitKey(1);

        if (key == 'x')
        {
            cv::destroyAllWindows();
            exit(0);
        }
        if (key == 's')
        {
            try
            {
                cv::FileStorage fs(rectificationFilePath, cv::FileStorage::WRITE);
                if (!fs.isOpened())
                {
                    throw std::runtime_error("Failed to open file for writing");
                }
                fs << "rotationMatrixLeft" << rotationMatrixLeft;
                fs << "rotationMatrixRight" << rotationMatrixRight;
                fs << "projectionMatrixLeft" << projectionMatrixLeft;
                fs << "projectionMatrixRight" << projectionMatrixRight;
                fs << "disparityMatrix" << disparityMatrix;
                fs << "mapLeftX" << mapLeftX;
                fs << "mapLeftY" << mapLeftY;
                fs << "mapRightX" << mapRightX;
                fs << "mapRightY" << mapRightY;

                fs.release();

                if (fs.isOpened())
                {
                    throw std::runtime_error("Failed to close file after writing");
                }

                std::cout << "The rectification parameters have been saved successfully!" << std::endl;
                rectificationParamsSaved = true;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error: " << e.what() << std::endl;
            }

            if (!intrinsicParamsSaved)
            {
                try
                {
                    cv::FileStorage fs(intrinsicFilePath, cv::FileStorage::WRITE);
                    if (!fs.isOpened())
                    {
                        throw std::runtime_error("Failed to open file for writing");
                    }
                    fs << "cameraMatrixLeft" << cameraMatrixLeft;
                    fs << "cameraMatrixRight" << cameraMatrixRight;
                    fs << "optimalCameraMatrixLeft" << optimalCameraMatrixLeft;
                    fs << "optimalCameraMatrixRight" << optimalCameraMatrixRight;
                    fs << "distortionCoeffsLeft" << distortionCoeffsLeft;
                    fs << "distortionCoeffsRight" << distortionCoeffsRight;

                    fs.release();
                    if (fs.isOpened())
                    {
                        throw std::runtime_error("Failed to close file after writing");
                    }

                    std::cout << "The intrinsic parameters have been saved successfully!" << std::endl;
                    intrinsicParamsSaved = true;
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
            }

            if (!extrinsicParamsSaved)
            {
                try
                {
                    cv::FileStorage fs(extrinsicFilePath, cv::FileStorage::WRITE);

                    if (!fs.isOpened())
                    {
                        throw std::runtime_error("Failed to open file for writing");
                    }

                    fs << "rotationMatrixCommon" << rotationMatrixCommon;
                    fs << "translationVector" << translationVector;
                    fs << "essentialMatrix" << essentialMatrix;
                    fs << "fundamentalMatrix" << fundamentalMatrix;

                    fs.release();

                    if (fs.isOpened())
                    {
                        throw std::runtime_error("Failed to close file after writing");
                    }

                    std::cout << "The extrinsic parameters have been saved successfully!" << std::endl;
                    extrinsicParamsSaved = true;
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
            }
        }
        if (key == 'n')
        {
            cv::destroyAllWindows();
            std::cout << "Stereo Calibration stage is finished" << std::endl;
            break;
        }
        if (key == 'r')
        {
            cv::destroyAllWindows();
            StereoCalibrator::initCameraCalibration();
        }
    }
}

double StereoCalibrator::getReprojectionError(const std::vector<std::vector<cv::Point3f>> &objectPoints, const std::vector<std::vector<cv::Point2f>> &imagePoints, const std::vector<cv::Mat> &rotationVecs, const std::vector<cv::Mat> &translationVecs, const cv::Mat &cameraMatrix, const cv::Mat &distortionCoeffs)
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
