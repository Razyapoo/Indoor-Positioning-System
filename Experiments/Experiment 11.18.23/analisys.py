import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

uwb_data = pd.read_csv('Experiment 1/test_5m.txt', header=None)

ground_truth_data = pd.DataFrame(np.tile(5, 82))

difference_series = uwb_data - ground_truth_data

# # Scatter plot
# plt.scatter(ground_truth_data, difference_series)
# plt.xlabel('Ground Truth Distances')
# plt.ylabel('Difference')
# plt.title('Differences between UWB Distances and Ground Truth')
# plt.show()

# # Histogram
# plt.hist(difference_series, bins=20)
# plt.xlabel('Difference')
# plt.ylabel('Frequency')
# plt.title('Histogram of Differences')
# plt.show()

# mae = difference_series.abs().mean()
# rmse = (difference_series ** 2).mean() ** 0.5

# print(f'Mean Absolute Error: {mae}')
# print(f'Root Mean Square Error: {rmse}')


# z_scores = (uwb_data - uwb_data.mean()) / uwb_data.std()
# threshold_z = 1
# outliers_z = uwb_data[abs(z_scores) > threshold_z]

# # Finding outliers using IQR
# Q1 = uwb_data.quantile(0.25)
# Q3 = uwb_data.quantile(0.75)
# IQR = Q3 - Q1
# threshold_iqr = 1.5
# outliers_iqr = uwb_data[(uwb_data < (Q1 - threshold_iqr * IQR))
#                         | (uwb_data > (Q3 + threshold_iqr * IQR))]

# # Display outliers
# print("Outliers (Z-score):")
# print(outliers_z.dropna())

# print("\nOutliers (IQR):")
# print(outliers_iqr.dropna())

# # Create a boxplot
# plt.boxplot(uwb_data)
# plt.title('Boxplot of UWB Data')
# plt.show()


# Calculate displacement
displacement = ((uwb_data - uwb_data.shift()) ** 2).sum(axis=1) ** 0.5

# Set a threshold to distinguish movement from staying
movement_threshold = 0.1  # Adjust this threshold based on your data characteristics

# Identify indices where displacement exceeds the threshold
moving_indices = displacement > movement_threshold

# Plot the position data and highlight moving periods
plt.figure(figsize=(10, 6))
plt.plot(uwb_data, label='Position Data', color='blue')
plt.scatter(uwb_data.index[moving_indices],
            uwb_data[moving_indices], color='red', label='Moving Periods')
plt.xlabel('Index')
plt.ylabel('Position')
plt.title('Tag Movement Analysis')
plt.legend()
plt.show()

# Display information about moving and staying periods
print("Moving Periods:")
print(uwb_data.index[moving_indices])

print("\nStaying Periods:")
print(uwb_data.index[~moving_indices])
