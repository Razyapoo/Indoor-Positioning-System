import pandas as pd
import numpy as np

if __name__ == "__main__":

    row_data_esp32 = pd.read_csv('ESP32_records.txt', delimiter=' ', header=None)

    ## Preparing ESP32 dataset
     
    # Cleaning data: Drop first row, since it is noise
    row_data_esp32 = row_data_esp32.drop(row_data_esp32.index[0])
    row_data_esp32[0] -= 1

    row_number = 1
    value = 0
    data_esp32 = np.empty((0, 2))
    # print(row_data_esp32.shape[0])
    while (row_number < row_data_esp32.shape[0]):
        filtered_rows = row_data_esp32[(row_data_esp32.iloc[:, 4] >= value) & (row_data_esp32.iloc[:, 4] <= value + 1)]
        filtered_rows = filtered_rows.iloc[:, 4].to_numpy().reshape(1, -1)
        np.append(data_esp32, filtered_rows, axis = 0)
        row_number += filtered_rows.shape[0]
        value += 1

    print(data_esp32)
