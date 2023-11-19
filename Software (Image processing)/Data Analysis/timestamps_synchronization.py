import cv2
import numpy as np
import pandas as pd
import seaborn as sns
from matplotlib import pyplot as plt

video_timestamps_path = "../../Experiments/Experiment 11.05.23/timestamp.txt"
input_video = cv2.VideoCapture(
    "../../Experiments/Experiment 11.05.23/video_from_left_camera.avi")

# with open(video_timestamps_path, 'r') as video_timestamps_file:
# video_timestamps = video_timestamps_file.read()

video_timestamps = pd.read_csv(
    video_timestamps_path, delimiter=' ', header=None)

# video_timestamps = video_timestamps.split('\n')
video_timestamp_record_id = 0
previous_video_timestamp_of_detected = 0
# equals to meters in a stage: stage 1 correspond to 1 meter distnace of tag from anchor
stage_iterator = 1
video_timestamps_and_stages = pd.DataFrame(columns=["Timestamp", "Stage"])
continuous_detection = False

while True:
    ret, frame = input_video.read()

    if not ret:
        break

    # video_timestamp = int(
    #     video_timestamps[video_timestamp_record_id].split(" ")[1])
    video_timestamp = video_timestamps.iloc[video_timestamp_record_id, 1]
    video_timestamp_record_id += 1

    # Detect ArUco markers in the frame
    # Convert frame to grayscale for marker detection
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Load the default dictionary for ArUco markers
    aruco_dict = cv2.aruco.Dictionary_get(cv2.aruco.DICT_6X6_250)

    # Initialize the parameters for marker detection
    parameters = cv2.aruco.DetectorParameters_create()

    # Detect markers
    corners, ids, rejectedImgPoints = cv2.aruco.detectMarkers(
        gray, aruco_dict, parameters=parameters)

    # Draw detected markers if any
    if ids is not None and len(ids) > 0:
        cv2.aruco.drawDetectedMarkers(frame, corners, ids)
        previous_video_timestamp_of_detected = video_timestamp

        video_timestamps_and_stages.loc[len(video_timestamps_and_stages)] = [
            video_timestamp, stage_iterator]

        continuous_detection = True
    elif (video_timestamp - previous_video_timestamp_of_detected) > 3000 and continuous_detection:
        continuous_detection = False
        stage_iterator += 1

    # Display the frame with detected markers
    # cv2.imshow('ArUco Marker Detection', frame)

    # if cv2.waitKey(1) & 0xFF == ord('q'):
    #     break
print(video_timestamps_and_stages)
input_video.release()
# cv2.destroyAllWindows()


# Processing of ESP32 timestamps
# There are two methods to synchronize ESP32 data with camera frames using timestamps.
# 1. As there is prepared pandas dataset having timestamps for each meter of measurement, we can use start and end camera timestamps and then filer ESP32 timestamps falling into that interval
# 2. For each ESP32 record find closest camera frame using some threashold value, e.g. 100 ms

# Implementation of 2.
esp32_timestamps_path = "../../Experiments/Experiment 11.05.23/timestamp_ESP32.txt"

esp32_timestamps = pd.read_csv(
    esp32_timestamps_path, delimiter=' ', header=None)
found_camera_frame = None
video_and_esp32_distances_all = pd.DataFrame(
    columns=["DistanceCamera", "DistanceESP32"])
for record_id in range(len(esp32_timestamps)):
    esp32_timestamp = esp32_timestamps.iloc[record_id, 1]
    found_camera_frames = video_timestamps_and_stages[(
        video_timestamps_and_stages.iloc[:, 0] - esp32_timestamp).abs() < 10]

    if not found_camera_frames.empty:
        camera_distance = found_camera_frames[found_camera_frames['Timestamp']
                                              == found_camera_frames['Timestamp'].min()]['Stage'].iloc[0]
        video_and_esp32_distances_all.loc[len(video_and_esp32_distances_all)] = [
            camera_distance, esp32_timestamps.iloc[record_id, 4]]

video_and_esp32_distances_filtered = pd.DataFrame(
    columns=["DistanceCamera", "DistanceESP32"])
for distance in range(1, 11):
    median_esp32_distance = video_and_esp32_distances_all[video_and_esp32_distances_all['DistanceCamera'] == distance]['DistanceESP32'].median(
    )
    video_and_esp32_distances_filtered.loc[len(video_and_esp32_distances_filtered)] = [
        distance, median_esp32_distance]

print(video_and_esp32_distances_filtered)
sns.scatterplot(data=video_and_esp32_distances_filtered,
                x='DistanceCamera', y='DistanceESP32')
plt.show()
