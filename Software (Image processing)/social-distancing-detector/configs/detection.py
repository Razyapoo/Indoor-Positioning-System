# imports
from .config import NMS_THRESH
from .config import MIN_CONF
import numpy as np
import cv2

# function to detect people


def detect_people(frame, net, ln, size_filter,  personIdx=0):
    # grab dimensions of the frame and initialize the list of results
    (H, W) = frame.shape[:2]
    results = []
    F = 490  # test_2=817 616 490
    # construct a blob from the input frame and then perfrom a forward pass
    # of the YOLO object detector, giving us the bounding boxes and
    # associated probabilities
    blob = cv2.dnn.blobFromImage(
        frame, 1 / 255.0, (640, 640), swapRB=True, crop=False)
    net.setInput(blob)
    layerOutputs = net.forward(ln)

    # initialize lists of detected bounding boxes, centroids, and confidence
    boxes = []
    centroids = []
    centroids_normalized = []
    confidences = []

    # loop over each of the layer outputs
    for output in layerOutputs:
        # loop over each of the detections
        for detection in output:
            # extract teh class ID and confidence(probability) of the current object detection
            scores = detection[5:]
            classID = np.argmax(scores)
            confidence = scores[classID]

            # filter detections by (1) ensuring that the object detected was a person and
            # (2) that the minimum confidence is met
            if classID == personIdx and confidence > MIN_CONF:
                # scale the bounding box coordinates back relative to the size of
                # the image, keeping in mind that YOLO actually returns the center (x, y)-coordinates
                # of the bounding box followed by the boxes' width and height
                box = detection[0:4] * np.array([W, H, W, H])
                avg_width, avg_height = size_filter.update(
                    [box[2], box[3]])

                box = [box[0], box[1], avg_width, avg_height]

                (centerX, centerY, width, height) = np.array(
                    box).astype("int")

                # use the center (x,y)-coordinates to derive the top-left corner of
                # the bounding box
                x = int(centerX - (width / 2))
                y = int(centerY - (height / 2))

                imageX = int(centerX)
                imageY = int(centerY + (height / 2))

                # Camera parameters are computes for 640x360 size of an image. Need a scale factor fro 640x640
                scaleX = 640 / 640
                scaleY = 640 / 360

                fxAdjusted = 490 * scaleX
                fyAdjusted = 491 * scaleY
                cxAdjusted = 310 * scaleX
                cyAdjusted = 150 * scaleY

                # F = f * 1.4 / 1000
                distance = (1760 * fyAdjusted) / height

                # cx and fx from intrinsic calibration
                worldX = (imageX - cxAdjusted) * (distance / fxAdjusted)
                # cy and fy from intrinsic calibration
                worldY = (imageY - cyAdjusted) * (distance / fyAdjusted)

                print(
                    f"Distance from camera: {distance} \n\t Image coordinates: ({imageX}, {imageY}) \n\t World coordinates: ({worldX}, {worldY})")
                # convert center point to centimeter
                # centerX = centerX * distance / F
                # centerY = centerY * distance / F

                # update the list of bounding box coordinates, centroids and confidences
                boxes.append([x, y, int(width), int(height)])
                centroids.append((x, centerY, distance))
                # centroids_normalized.append((centerX / width, centerY / height))
                confidences.append(float(confidence))

    # apply non-maxima suppression to suppress weak, overlapping bounding boxes
    idxs = cv2.dnn.NMSBoxes(boxes, confidences, MIN_CONF, NMS_THRESH)

    # ensure at least one detection exists
    if len(idxs) > 0:
        # loop over the indexes being kept
        for i in idxs.flatten():
            # extract the bounding box coordinates
            (x, y) = (boxes[i][0], boxes[i][1])
            (w, h) = (boxes[i][2], boxes[i][3])

            # update the results list to consist of the person prediction probability,
            # bounding box coordinates, and the centroid
            r = (confidences[i], (x, y, x + w, y + h), centroids[i])
            results.append(r)

    # return the list of results
    return results
