import pandas as pd

uwb_data = pd.read_csv("timestamp_ESP32.txt", header=None, delimiter=" ")

print(uwb_data.iloc[:, 4])
