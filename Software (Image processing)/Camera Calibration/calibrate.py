import numpy as np
import cv2 as cv 
import os
import glob

chessboardSize = (6, 9)
squareSize = 2.1
imgSize = (640, 480)
imageDirPath = "Calibrate Images"
calibratedDataPath = "Calibrated Data"


criteria = (cv.TERM_CRITERIA_EPS + cv.TERM_CRITERIA_MAX_ITER, 30, 0.001)

objectPoint = np.zeros((chessboardSize[0] * chessboardSize[1], 3), np.float32)
objectPoint[:,:2] = np.mgrid[0:chessboardSize[0], 0:chessboardSize[1]].T.reshape(-1, 2)
objectPoint *= squareSize

objectPoints = []
imgPoints = []

def calibrate(img):
    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY) 
    isBoardDetected, corners = cv.findChessboardCorners(gray, chessboardSize, None)
    if isBoardDetected == True: 
        objectPoints.append(objectPoint)
        corners = cv.cornerSubPix(gray, corners, (11,11), (-1,-1), criteria)
        imgPoints.append(corners)
        img = cv.drawChessboardCorners(img, chessboardSize, corners, isBoardDetected)
        
    return isBoardDetected, img
        
def calibrateImages():
    imagePaths = os.listdir(imageDirPath)
    for relImagePath in imagePaths:
        imagePath = os.path.join(imageDirPath, relImagePath)
        img = cv.imread(imagePath)
        isBoardDetected, img = calibrate(img)
        if isBoardDetected == True:
            cv.imshow('img', img)
            key = cv.waitKey(1)
            if key == ord("n"):
                continue
            
    cv.destroyAllWindows()
        
def calibrateWebCam():
        
    cap = cv.VideoCapture(2)
    count = 0
    while True:
        _, frame = cap.read()
        copyFrame = frame.copy()
        # imgSize = frame.shape[:2]
        
        isBoardDetected, frame = calibrate(frame)
        cv.putText(frame, f"savedImg: {count}", (30,40),cv.FONT_HERSHEY_PLAIN,1.4,(0,255,0),2,cv.LINE_AA)
        cv.imshow("frame", frame)
        cv.imshow("copyFrame", copyFrame)
        
        key = cv.waitKey(1)
        if key == ord("q"):
            break
        if key == ord("s") and isBoardDetected:
            result = cv.imwrite(f"{imageDirPath}/calib_image_{count}.png", copyFrame)
            print(f"saved image number: {count} into {result}")
            count += 1

    cap.release()
    cv.destroyAllWindows()

def getUndistorted(img):
    newCameraMatrix, roi = cv.getOptimalNewCameraMatrix(cameraMatrix, distributionCoefficients, imgSize, 1, imgSize)
            
    undistortedImage = cv.undistort(img, cameraMatrix, distributionCoefficients, None, newCameraMatrix)
    x, y, w, h = roi
    undistortedImage = undistortedImage[y:y+h, x:x+w]
    cv.imwrite(f"{calibratedDataPath}/{relImagePath}", undistortedImage)
    
    return undistortedImage

def getReprojectionError():
    total_error = 0
    
    for i in range(len(objectPoints)):
        imgPoints2, _ = cv.projectPoints(objectPoints[i], rvecs[i], tvecs[i], cameraMatrix, distributionCoefficients)
        error = cv.norm(imgPoints[i], imgPoints2, cv.NORM_L2)/len(imgPoints2)
        total_error += error
        
    return total_error/len(objectPoints)

if __name__=="__main__":
    
    if os.path.isdir(calibratedDataPath) and os.path.listdir(calibratedDataPath):
        data = np.load(f"{calibratedDataPath}/ProjectionMatrix.npz")
        rmse = data["rmse"]
        cameraMatrix = data["cameraMatrix"]
        distributionCoefficients = data["distributionCoefficients"]
        rVector = data["rVector"]
        tVector = data["tVector"]
    else:
        if not os.path.isdir(calibratedDataPath):
            os.makedirs(calibratedDataPath)
        if os.path.isdir(imageDirPath) and os.path.listdir(imageDirPath):
            calibrateImages()
        else:
            if not os.path.isdir(imageDirPath): 
                os.makedirs(imageDirPath)
            calibrateWebCam()
        
        rmse, cameraMatrix, distributionCoefficients, rvecs, tvecs = cv.calibrateCamera(objectPoints, imgPoints, imgSize, None, None)
        
        print ("RMSE:")
        print(rmse)
        
        print("\n Camera matrix:")
        print(cameraMatrix)
        
        print("\n Distortion coefficient:")
        print(distributionCoefficients)
        
        print("\n Rotation Vectors:")
        print(rvecs)
        
        print("\n Translation Vectors:")
        print(tvecs)
        
        np.savez(f"{calibratedDataPath}/ProjectionMatrix", 	rmse=rmse, cameraMatrix=cameraMatrix, distributionCoefficients=distributionCoefficients, rVector=rvecs, tVector=tvecs)
        
        
    # Undistortion
    # if rmse:
    imagePaths = os.listdir(imageDirPath)
    count = 0
    for relImagePath in imagePaths:
        imagePath = os.path.join(imageDirPath, relImagePath)
        img = cv.imread(imagePath)
        undistortedImage = getUndistorted(img)
        cv.imshow(relImagePath, undistortedImage)
            
    # Reprojection error
    totalError = getReprojectionError()
    print(f"total error: {totalError}")
            
    
        