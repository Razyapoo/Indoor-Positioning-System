import pandas as pd
import numpy as np

file1 = '/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/3 people export/UWB_timestamps.txt'
df1 = pd.read_csv(file1, sep=' ', header=None, 
                  names=['recordId', 'timestamp', 'tagId', 'sensor1', 'value1', 'sensor2', 'value2'])

dfsByTag = {tag: df1[df1['tagId'] == tag] for tag in df1['tagId'].unique()}

sampledDfs = {tag: df.sample(n=100, random_state=1) for tag, df in dfsByTag.items()}


# file2 = '/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/3 people export/video_timestamps.txt'
# df2 = pd.read_csv(file2, sep=' ', header=None, names=['id', 'timestamp'])

# def findClosestTimestamp(timestamp, df):
#     closestIdx = (df['timestamp'] - timestamp).abs().idxmin()
#     return df.loc[closestIdx, 'id']

# closestIds = {tag: df.apply(lambda row: findClosestTimestamp(row['timestamp'], df2), axis=1)
#               for tag, df in sampledDfs.items()}

# for tag, ids in closestIds.items():
#     print(f"Tag ID: {tag}")
#     print(ids)