import numpy as np
import cv2 as cv 
import os
import glob

chessBoardSize = (9, 6)
squareSize = 17
imgSize = (640, 480)
imageDirPath = "Calibrate Images"
calibratedDataPath = "Calibrated Data"


criteria = (cv.TERM_CRITERIA_EPS + cv.TERM_CRITERIA_MAX_ITER, 30, 0.001)

objPoint = np.zeros((chessBoardSize[1] * chessBoardSize[0], 3), np.float32)
objPoint[:,:2] = np.mgrid[0:chessBoardSize[0], 0:chessBoardSize[1]].T.reshape(-1, 2)
objPoint *= squareSize

objPoints = []
imgPoints = []

def calibrate(img):
    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY) 
    isBoardDetected, corners1 = cv.findChessboardCorners(gray, chessBoardSize, None)
    if isBoardDetected == True: 
        objPoints.append(objPoint)
        corners2 = cv.cornerSubPix(gray, corners1, (11,11), (-1,-1), criteria)
        imgPoints.append(corners2)
        img = cv.drawChessboardCorners(img, chessBoardSize, corners2, isBoardDetected)
        
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
        
    cap = cv.VideoCapture(0)
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
    newCamMtx, roi = cv.getOptimalNewCameraMatrix(camMtx, dist, imgSize, 1, imgSize)
            
    undistImg = cv.undistort(img, camMtx, dist, None, newCamMtx)
    x, y, w, h = roi
    undistImg = undistImg[y:y+h, x:x+w]
    cv.imwrite(f"{calibratedDataPath}/{relImagePath}", undistImg)
    
    return undistImg

def getReprojectionError():
    mean_error = 0
    
    for i in range(len(objPoints)):
        imgPoints2, _ = cv.projectPoints(objPoints[i], rvecs[i], tvecs[i], camMtx, dist)
        error = cv.norm(imgPoints[i], imgPoints2, cv.NORM_L2)/len(imgPoints2)
        mean_error += error
        
    return mean_error

if __name__=="__main__":
    
    if not os.path.isdir(calibratedDataPath):
        os.makedirs(calibratedDataPath)
        if not os.path.isdir(imageDirPath):
            os.makedirs(imageDirPath)
            calibrateWebCam()
        else: 
            calibrateImages()
        
        ret, camMtx, dist, rvecs, tvecs = cv.calibrateCamera(objPoints, imgPoints, imgSize, None, None)
        
        print(" Camera matrix:")
        print(camMtx)
        
        print("\n Distortion coefficient:")
        print(dist)
        
        print("\n Rotation Vectors:")
        print(rvecs)
        
        print("\n Translation Vectors:")
        print(tvecs)

        if not os.path.isdir(calibratedDataPath):
            os.makedirs(calibratedDataPath)
        
        np.savez(f"{calibratedDataPath}/ProjectionMatrix", 	ret=ret, cameraMatrix=camMtx, distCoeffs=dist, rVector=rvecs, tVector=tvecs)
        
    else:
        data = np.load(f"{calibratedDataPath}/ProjectionMatrix.npz")
        ret = data["ret"]
        camMtx = data["camMtx"]
        distCoeffs = data["distCoeffs"]
        rVector = data["rVector"]
        tVector = data["tVector"]
        
    # Undistortion
    if ret == True:
        imagePaths = os.listdir(imageDirPath)
        for relImagePath in imagePaths:
            imagePath = os.path.join(imageDirPath, relImagePath)
            img = cv.imread(imagePath)
            undistImg = getUndistorted(img)
            cv.imshow(relImagePath, undistImg)
            
    # Reprojection error
    totalError = getReprojectionError()
    print(f"total error: {totalError}")
            
    
        