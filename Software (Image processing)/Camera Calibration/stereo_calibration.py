import numpy as np
import cv2 as cv 
import os
import glob

chessboardSize = (6, 9)
squareSize = 2.1
imgSize = (640, 480)
imageDirPath = "Calibrate Images"
calibratedDataPath = "Calibrated Data"
leftCameraImgPath = "Left Camera"
rightCameraImgPath = "Right Camera"

isCapturingPhase = True

criteria = (cv.TERM_CRITERIA_EPS + cv.TERM_CRITERIA_MAX_ITER, 30, 0.001)

objectPoint = np.zeros((chessboardSize[0] * chessboardSize[1], 3), np.float32)
objectPoint[:,:2] = np.mgrid[0:chessboardSize[0], 0:chessboardSize[1]].T.reshape(-1, 2)
objectPoint *= squareSize

objectPoints = []
imgPointsLeft = []
imgPointsRight = []

def calibrate(img, isLeft):
    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY) 
    isBoardDetected, corners = cv.findChessboardCorners(gray, chessboardSize, None)
    if isBoardDetected == True: 
        objectPoints.append(objectPoint)
        corners = cv.cornerSubPix(gray, corners, (11,11), (-1,-1), criteria)
        if not isCapturingPhase:
            if isLeft: imgPointsLeft.append(corners)
            else: imgPointsRight.append(corners)
        img = cv.drawChessboardCorners(img, chessboardSize, corners, isBoardDetected)
        
    return isBoardDetected, img
        
def calibrateImages():
    
    imagesLeft = glob.glob('{imageDirPath}/{leftCameraImgPath}/*.png')
    imagesRight = glob.glob('{imageDirPath}/{rightCameraImgPath}/*.png')
        
    for imgLeftPath, imgRightPath in zip(imagesLeft, imagesRight):
        imgLeft = cv.imread(imgLeftPath)
        imgRight = cv.imread(imgRightPath)
        calibrate(imgLeft, True)
        calibrate(imgRight, False)
        
def captureImagesWebCam():
        
    leftCamera = cv.VideoCapture(0, cv.CAP_DSHOW)
    rightCamera = cv.VideoCapture(2, cv.CAP_DSHOW)
    count = 0
    while True:
        leftCameraSuccessful, leftCameraFrame = leftCamera.read()
        rightCameraSuccessful, rightCameraFrame = rightCamera.read()
        
        if not leftCameraSuccessful:
            print("It is not possible to open left camera")
            break
        
        if not rightCameraSuccessful:
            print("It is not possible to open right camera")
            break
        
        leftCameraFrameCopy = leftCameraFrame.copy()
        rightCameraFrameCopy = rightCameraFrame.copy()

        isLeftBoardDetected, leftCameraFrame = calibrate(leftCameraFrame, True)
        isRightBoardDetected, rightCameraFrame = calibrate(rightCameraFrame, False)
        
        cv.imshow("leftCameraFrame", leftCameraFrame)
        cv.imshow("leftCameraFrameCopy", leftCameraFrameCopy)
        cv.imshow("rightCameraFrame", rightCameraFrame)
        cv.imshow("rightCameraFrameCopy", rightCameraFrameCopy)
        
        key = cv.waitKey(0)
        if key == ord("q"):
            break
        if key == ord("s") and isLeftBoardDetected and isRightBoardDetected:
            result = cv.imwrite(f"{imageDirPath}/{leftCameraImgPath}/image_to_calibrate_left_camera_{count}.png", leftCameraFrameCopy)
            result = cv.imwrite(f"{imageDirPath}/{rightCameraImgPath}/image_to_calibrate_right_camera_{count}.png", rightCameraFrameCopy)
            print(f"saved images number: {count} into {result}")
            count += 1

    leftCamera.release()
    rightCamera.release()
    cv.destroyAllWindows()

if __name__=="__main__":

    if not os.path.isdir(calibratedDataPath):
        os.makedirs(calibratedDataPath)
    if os.path.listdir(calibratedDataPath):
        files = glob.glob(calibratedDataPath)
        for f in files:
            os.remove(f)
    
    if not os.path.isdir(imageDirPath): 
        os.makedirs(imageDirPath)
    
    if not os.path.listdir(imageDirPath):
        isCapturingPhase = True
        captureImagesWebCam()
        isCapturingPhase = False
        
    calibrateImages()

    rmseLeft, cameraMatrixLeft, distributionCoefficientsLeft, rotationVectorsLeft, translationVectorsLeft = cv.calibrateCamera(objectPoints, imgPointsLeft, imgSize, None, None)
    rmseRight, cameraMatrixRight, distributionCoefficientsRight, rotationVectorsRight, translationVectorsRight = cv.calibrateCamera(objectPoints, imgPointsRight, imgSize, None, None)
    
    newCameraMatrixLeft, roiLeft = cv.getOptimalNewCameraMatrix(cameraMatrixLeft, distributionCoefficientsLeft, imgSize, 1, imgSize)
    newCameraMatrixRight, roiRight = cv.getOptimalNewCameraMatrix(cameraMatrixRight, distributionCoefficientsRight, imgSize, 1, imgSize)
    
    # Stereo calibration
    # - keep the intrinsic parameters same, so the stereo calibration function is not overwhelmed
    flags = 0
    flags |= cv.CALIB_FIX_INTRINSIC
 
    rmse, newCameraMatrixLeft, distributionCoefficientsLeft, newCameraMatrixRight, distributionCoefficientsRight, rotationMatrix, translationVector, essentialMatrix, fundamentalMatrix = cv.stereoCalibrate(objectPoints, imgPointsLeft, imgPointsRight, newCameraMatrixLeft, distributionCoefficientsLeft, newCameraMatrixRight, distributionCoefficientsRight, imgSize, criteria, flags)
    
    # Stereo rectification
    rectifyScale = 1
    rotationMatrixLeft, rotationMatrixRight, projectionMatrixLeft, projectionMatrixRight, disparityToDepthMatrix, roiLeft, roiRight= cv.stereoRectify(newCameraMatrixLeft, distributionCoefficientsLeft, newCameraMatrixRight, distributionCoefficientsRight, imgSize, rotationMatrix, translationVector, rectifyScale, (0,0))

    stereoTransformationMapLeft = cv.initUndistortRectifyMap(newCameraMatrixLeft, distributionCoefficientsLeft, rotationMatrixLeft, projectionMatrixLeft, imgSize, cv.CV_16SC2)
    stereoTransformationMapRight = cv.initUndistortRectifyMap(newCameraMatrixRight, distributionCoefficientsRight, rotationMatrixRight, projectionMatrixRight, imgSize, cv.CV_16SC2)
    
    parametersFile = cv.FileStorage('stereoTransformationMap.xml', cv.FILE_STORAGE_WRITE)
    parametersFile.write('undistortionTransformationMapLeft', stereoTransformationMapLeft[0])
    parametersFile.write('rectificationTransformationMapLeft', stereoTransformationMapLeft[1])
    parametersFile.write('undistortionTransformationMapRight', stereoTransformationMapRight[0])
    parametersFile.write('rectificationTransformationMapRight', stereoTransformationMapRight[1])
 