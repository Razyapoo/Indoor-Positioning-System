import numpy as np
import pandas as pd

data = pd.read_csv("""Experiment 8 - 2 anchors, 3 tags, 5m only
Anchor 102 Left Wall Line - Anchor 101 Right Wall Line./Experiment 8.6 - Tag 3 Left Wall Line - Tag 2 Central Line - Tag 1 Right Wall Line/data_raw.txt""", delimiter=' ', header=None)

tag_1 = data[data.iloc[:, 2] == 1]
tag_2 = data[data.iloc[:, 2] == 2]
tag_3 = data[data.iloc[:, 2] == 3]

median_tag_1_101 = tag_1.iloc[:, 4].median()
mean_tag_1_101 = tag_1.iloc[:, 4].mean()
max_tag_1_101 = tag_1.iloc[:, 4].max()
min_tag_1_101 = tag_1.iloc[:, 4].min()

median_tag_1_102 = tag_1.iloc[:, 7].median()
mean_tag_1_102 = tag_1.iloc[:, 7].mean()
max_tag_1_102 = tag_1.iloc[:, 7].max()
min_tag_1_102 = tag_1.iloc[:, 7].min()

print(f"""
    Tag 1: 
        Anchor 101:
            Median: {median_tag_1_101} 
            Mean: {mean_tag_1_101}
            Min: {min_tag_1_101}
            Max: {max_tag_1_101}
      
        Anchor 102:
            Median: {median_tag_1_102} 
            Mean: {mean_tag_1_102}
            Min: {min_tag_1_102}
            Max: {max_tag_1_102}
""")


median_tag_2_101 = tag_2.iloc[:, 4].median()
mean_tag_2_101 = tag_2.iloc[:, 4].mean()
max_tag_2_101 = tag_2.iloc[:, 4].max()
min_tag_2_101 = tag_2.iloc[:, 4].min()

median_tag_2_102 = tag_2.iloc[:, 7].median()
mean_tag_2_102 = tag_2.iloc[:, 7].mean()
max_tag_2_102 = tag_2.iloc[:, 7].max()
min_tag_2_102 = tag_2.iloc[:, 7].min()

print(f"""
    Tag 2: 
        Anchor 101:
            Median: {median_tag_2_101} 
            Mean: {mean_tag_2_101}
            Min: {min_tag_2_101}
            Max: {max_tag_2_101}
      
        Anchor 102:
            Median: {median_tag_2_102} 
            Mean: {mean_tag_2_102}
            Min: {min_tag_2_102}
            Max: {max_tag_2_102}
""")


median_tag_3_101 = tag_3.iloc[:, 4].median()
mean_tag_3_101 = tag_3.iloc[:, 4].mean()
max_tag_3_101 = tag_3.iloc[:, 4].max()
min_tag_3_101 = tag_3.iloc[:, 4].min()

median_tag_3_102 = tag_3.iloc[:, 7].median()
mean_tag_3_102 = tag_3.iloc[:, 7].mean()
max_tag_3_102 = tag_3.iloc[:, 7].max()
min_tag_3_102 = tag_3.iloc[:, 7].min()

print(f"""
    Tag 3: 
        Anchor 101:
            Median: {median_tag_3_101} 
            Mean: {mean_tag_3_101}
            Min: {min_tag_3_101}
            Max: {max_tag_3_101}
      
        Anchor 102:
            Median: {median_tag_3_102} 
            Mean: {mean_tag_3_102}
            Min: {min_tag_3_102}
            Max: {max_tag_3_102}
""")
