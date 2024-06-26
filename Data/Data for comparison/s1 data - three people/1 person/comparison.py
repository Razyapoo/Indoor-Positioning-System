import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns


# Placeholder for estimated coordinates (replace with actual data)
uwb_coords = pd.DataFrame({'x': [], 'y': []})
optical_coords = pd.DataFrame({'x': [], 'y': []})
model_coords = pd.DataFrame({'x': [], 'y': []})

frames = pd.read_csv('/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/Data for comparison/s1 data - three people/1 person/uwb_to_bb_mapping_restricted_up_to_10m.txt', sep=' ', header=None, usecols=[0])
frames.columns = ['Frames']

# Load the reference coordinates
ref_coords_path = '/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/Data for comparison/s1 data - three people/1 person/reference_coordinates_restricted_up_to_10m.txt'
ref_coords = pd.read_csv(ref_coords_path, sep=' ', header=None)
ref_coords.columns = ['x', 'y']

uwb_coords = pd.read_csv('/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/Data for comparison/s1 data - three people/1 person/uwb_to_bb_mapping_restricted_up_to_10m.txt', sep=' ', header=None, usecols=[1, 2])
uwb_coords.columns = ['x', 'y']

optical_coords = pd.read_csv('/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/Data for comparison/s1 data - three people/1 person/optical_to_bb_mapping_restricted_up_to_10m.txt', sep=' ', header=None, usecols=[1, 2])
optical_coords.columns = ['x', 'y']

model_coords = pd.read_csv('/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/Data for comparison/s1 data - three people/1 person/pixel_to_real_to_bb_mapping_restricted_up_to_10m.txt', sep=' ', header=None, usecols=[1, 2])
model_coords.columns = ['x', 'y']


plt.figure(figsize=(10, 6))

plt.scatter(ref_coords['x'], ref_coords['y'], color='blue', label='Reference Coordinates', s=30)
plt.scatter(uwb_coords['x'], uwb_coords['y'], color='green', label='UWB Estimated Coordinates', s=30)

plt.xlabel('X Coordinate', fontsize=14)
plt.ylabel('Y Coordinate', fontsize=14)
plt.title('Reference and Estimated UWB Coordinates. Test Environment 3.', fontsize=18)
plt.legend(fontsize=10)
plt.xticks(fontsize=12)
plt.yticks(fontsize=12)
plt.grid(True)


plt.figure(figsize=(10, 6))

plt.scatter(ref_coords['x'], ref_coords['y'], color='blue', label='Reference Coordinates', s=30)
plt.scatter(model_coords['x'], model_coords['y'], color='orange', label='Pixel-to-Real Estimated Coordinates', s=30)

plt.xlabel('X Coordinate', fontsize=14)
plt.ylabel('Y Coordinate', fontsize=14)
plt.title('Reference and Estimated Pixel-to-Real Coordinates. Test Environment 3.', fontsize=18)
plt.legend(fontsize=10)
plt.xticks(fontsize=12)
plt.yticks(fontsize=12)
plt.grid(True)


plt.figure(figsize=(10, 6))

plt.scatter(ref_coords['x'], ref_coords['y'], color='blue', label='Reference Coordinates', s=30)
plt.scatter(optical_coords['x'], optical_coords['y'], color='red', label='Optical Estimated Coordinates', s=30)

plt.xlabel('X Coordinate', fontsize=14)
plt.ylabel('Y Coordinate', fontsize=14)
plt.title('Reference and Estimated Optical Coordinates. Test Environment 3.', fontsize=18)
plt.legend(fontsize=10)
plt.xticks(fontsize=12)
plt.yticks(fontsize=12)
plt.grid(True)



plt.show()



# errors_df = pd.DataFrame({
#     'UWB_x_Error': np.abs(ref_coords['x'] - uwb_coords['x']),
#     'UWB_y_Error': np.abs(ref_coords['y'] - uwb_coords['y']),
#     'Pixel-to-Real_x_Error': np.abs(ref_coords['x'] - model_coords['x']),
#     'Pixel-to-Real_y_Error': np.abs(ref_coords['y'] - model_coords['y']),
#     'Optical_x_Error': np.abs(ref_coords['x'] - optical_coords['x']),
#     'Optical_y_Error': np.abs(ref_coords['y'] - optical_coords['y'])
# })
# errors_melted_df = errors_df.melt(var_name='Method_Error', value_name='Error')
# errors_melted_df['Method'] = errors_melted_df['Method_Error'].apply(lambda x: x.split('_')[0])
# errors_melted_df['Axis'] = errors_melted_df['Method_Error'].apply(lambda x: x.split('_')[1])

# # # # print(errors_melted_df)
# # # Boxplot of errors
# # plt.figure(figsize=(12, 6))
# # sns.boxplot(data=errors_melted_df, x='Method', y='Error', hue='Axis')
# # plt.title('Boxplot of Errors in Coordinates. Test Environment 3.', fontsize=18)
# # plt.ylabel('Absolute Error', fontsize=13)
# # plt.xlabel('Methods', fontsize=12)
# # plt.legend(title='Axis', fontsize=13)
# # plt.xticks(fontsize=13)
# # plt.yticks(fontsize=12)
# # plt.grid(True)
# # # plt.show()

# # # # Histogram of errors for better understanding the distributions
# # # # plt.figure(figsize=(12, 6))
# # # # plt.hist(errors_df['UWB_x_Error'], bins=50, alpha=0.5, label='UWB_x_Error')
# # # # plt.hist(errors_df['UWB_y_Error'], bins=50, alpha=0.5, label='UWB_y_Error')
# # # # plt.hist(errors_df['PixelToReal_x_Error'], bins=50, alpha=0.5, label='PixelToReal_x_Error')
# # # # plt.hist(errors_df['PixelToReal_y_Error'], bins=50, alpha=0.5, label='PixelToReal_y_Error')
# # # # plt.hist(errors_df['Optical_x_Error'], bins=50, alpha=0.5, label='Optical_x_Error')
# # # # plt.hist(errors_df['Optical_y_Error'], bins=50, alpha=0.5, label='Optical_y_Error')
# # # # plt.title('Histogram of Errors by Method and Axis')
# # # # plt.xlabel('Absolute Error')
# # # # plt.ylabel('Frequency')
# # # # plt.legend()
# # # # plt.grid(True)
# # # # plt.show()

# # # Create histograms for each method separately and put all in a grid of figures

# # plt.figure(figsize=(14, 10))
# # plt.suptitle('Histogram of Errors in Coordinates. Test Environment 3.', fontsize=18)
# # # Histogram for UWB_X_Error
# # plt.subplot(3, 2, 1)
# # plt.hist(errors_df['UWB_x_Error'], bins=50, alpha=0.7, color='green')
# # plt.title('UWB X Coordinate Errors', fontsize=14)
# # plt.xlabel('Absolute Error', fontsize=13)
# # plt.ylabel('Frequency', fontsize=13)
# # plt.xticks(fontsize=12)
# # plt.yticks(fontsize=12)
# # plt.grid(True)

# # # Histogram for UWB_Y_Error
# # plt.subplot(3, 2, 2)
# # plt.hist(errors_df['UWB_y_Error'], bins=50, alpha=0.7, color='green')
# # plt.title('UWB Y Coordinate Errors', fontsize=14)
# # plt.xlabel('Absolute Error', fontsize=13)
# # plt.ylabel('Frequency', fontsize=13)
# # plt.xticks(fontsize=12)
# # plt.yticks(fontsize=12)
# # plt.grid(True)

# # # Histogram for Optical_X_Error
# # plt.subplot(3, 2, 3)
# # plt.hist(errors_df['Pixel-to-Real_x_Error'], bins=50, alpha=0.7, color='orange')
# # plt.title('Pixel-to-Real X Coordinate Errors', fontsize=14)
# # plt.xlabel('Absolute Error', fontsize=13)
# # plt.ylabel('Frequency', fontsize=13)
# # plt.xticks(fontsize=12)
# # plt.yticks(fontsize=12)
# # plt.grid(True)

# # # Histogram for Optical_Y_Error
# # plt.subplot(3, 2, 4)
# # plt.hist(errors_df['Pixel-to-Real_y_Error'], bins=50, alpha=0.7, color='orange')
# # plt.title('Pixel-to-Real Y Coordinate Errors', fontsize=14)
# # plt.xlabel('Absolute Error', fontsize=13)
# # plt.ylabel('Frequency', fontsize=13)
# # plt.xticks(fontsize=12)
# # plt.yticks(fontsize=12)
# # plt.grid(True)

# # # Histogram for Optical_X_Error
# # plt.subplot(3, 2, 5)
# # plt.hist(errors_df['Optical_x_Error'], bins=50, alpha=0.7, color='red')
# # plt.title('Optical X Coordinate Errors', fontsize=14)
# # plt.xlabel('Absolute Error', fontsize=13)
# # plt.ylabel('Frequency', fontsize=13)
# # plt.xticks(fontsize=12)
# # plt.yticks(fontsize=12)
# # plt.grid(True)

# # # Histogram for Optical_Y_Error
# # plt.subplot(3, 2, 6)
# # plt.hist(errors_df['Optical_y_Error'], bins=50, alpha=0.7, color='red')
# # plt.title('Optical Y Coordinate Errors', fontsize=14)
# # plt.xlabel('Absolute Error', fontsize=13)
# # plt.ylabel('Frequency', fontsize=13)
# # plt.xticks(fontsize=12)
# # plt.yticks(fontsize=12)
# # plt.grid(True)

# # plt.tight_layout()
# # plt.show()


# # Plot errors by frame
# # plt.figure(figsize=(14, 8))

# window_size = 50
# errors_df['UWB_x_Error_MA'] = errors_df['UWB_x_Error'].rolling(window=window_size).mean()
# errors_df['UWB_y_Error_MA'] = errors_df['UWB_y_Error'].rolling(window=window_size).mean()
# errors_df['Pixel-to-Real_x_Error_MA'] = errors_df['Pixel-to-Real_x_Error'].rolling(window=window_size).mean()
# errors_df['Pixel-to-Real_y_Error_MA'] = errors_df['Pixel-to-Real_y_Error'].rolling(window=window_size).mean()
# errors_df['Optical_x_Error_MA'] = errors_df['Optical_x_Error'].rolling(window=window_size).mean()
# errors_df['Optical_y_Error_MA'] = errors_df['Optical_y_Error'].rolling(window=window_size).mean()

# errors_df['Frames'] = frames[['Frames']].copy()
# # plt.plot(errors_df['Frames'], errors_df['UWB_x_Error'], label='UWB_x_Error', alpha=0.7)
# # plt.plot(errors_df['Frames'], errors_df['UWB_x_Error_MA'], label='UWB_x_Error_MA', color='red', linewidth=2)
# # plt.plot(errors_df['Frames'], errors_df['UWB_y_Error'], label='UWB_y_Error', alpha=0.7)
# # plt.plot(errors_df['Frames'], errors_df['UWB_y_Error_MA'], label='UWB_y_Error_MA', color='red', linewidth=2)

# # plt.plot(errors_df['Frames'], errors_df['PixelToReal_x_Error'], label='PixelToReal_X_Error', alpha=0.7)
# # plt.plot(errors_df['Frames'], errors_df['PixelToReal_x_Error_MA'], label='PixelToReal_x_Error_MA', color='red', linewidth=2)
# # plt.plot(errors_df['Frames'], errors_df['PixelToReal_y_Error'], label='PixelToReal_Y_Error', alpha=0.7)
# # plt.plot(errors_df['Frames'], errors_df['PixelToReal_y_Error_MA'], label='PixelToReal_y_Error_MA', color='red', linewidth=2)

# # plt.plot(errors_df['Frames'], errors_df['Optical_x_Error'], label='Optical_X_Error', alpha=0.7)
# # plt.plot(errors_df['Frames'], errors_df['Optical_x_Error_MA'], label='Optical_x_Error_MA', color='red', linewidth=2)
# # plt.plot(errors_df['Frames'], errors_df['Optical_y_Error'], label='Optical_Y_Error', alpha=0.7)
# # plt.plot(errors_df['Frames'], errors_df['Optical_y_Error_MA'], label='Optical_y_Error_MA', color='red', linewidth=2)








# # plt.figure(figsize=(14, 10))
# # plt.suptitle('Error Trend Over Time. Test Environment 3.', fontsize=18)
# # # UWB
# # plt.subplot(3, 2, 1)
# # plt.plot(errors_df['Frames'], errors_df['UWB_x_Error'], label='UWB_x_Error', alpha=0.7)
# # plt.plot(errors_df['Frames'], errors_df['UWB_x_Error_MA'], label='UWB_x_Error_MA', color='red', linewidth=2)
# # plt.xlabel('Frame', fontsize=13)
# # plt.ylabel('Absolute Error', fontsize=13)
# # plt.title('UWB X Coordinate', fontsize=14)
# # plt.xticks(fontsize=12)
# # plt.yticks(fontsize=12)
# # plt.legend()
# # plt.grid(True)

# # # UWB
# # plt.subplot(3, 2, 2)
# # plt.plot(errors_df['Frames'], errors_df['UWB_y_Error'], label='UWB_y_Error', alpha=0.7)
# # plt.plot(errors_df['Frames'], errors_df['UWB_y_Error_MA'], label='UWB_y_Error_MA', color='red', linewidth=2)
# # plt.xlabel('Frame', fontsize=13)
# # plt.ylabel('Absolute Error', fontsize=13)
# # plt.title('UWB Y Coordinate', fontsize=14)
# # plt.xticks(fontsize=12)
# # plt.yticks(fontsize=12)
# # plt.legend()
# # plt.grid(True)

# # # Pixel To Real
# # plt.subplot(3, 2, 3)
# # plt.plot(errors_df['Frames'], errors_df['Pixel-to-Real_x_Error'], label='Pixel-to-Real_X_Error', alpha=0.7)
# # plt.plot(errors_df['Frames'], errors_df['Pixel-to-Real_x_Error_MA'], label='Pixel-to-Real_x_Error_MA', color='red', linewidth=2)
# # plt.xlabel('Frame', fontsize=13)
# # plt.ylabel('Absolute Error', fontsize=13)
# # plt.title('Pixel-to-Real X Coordinate', fontsize=14)
# # plt.xticks(fontsize=12)
# # plt.yticks(fontsize=12)
# # plt.legend()
# # plt.grid(True)

# # plt.subplot(3, 2, 4)
# # plt.plot(errors_df['Frames'], errors_df['Pixel-to-Real_y_Error'], label='Pixel-to-Real_Y_Error', alpha=0.7)
# # plt.plot(errors_df['Frames'], errors_df['Pixel-to-Real_y_Error_MA'], label='Pixel-to-Real_y_Error_MA', color='red', linewidth=2)
# # plt.xlabel('Frame', fontsize=13)
# # plt.ylabel('Absolute Error', fontsize=13)
# # plt.title('Pixel-to-Real Y Coordinate', fontsize=14)
# # plt.xticks(fontsize=12)
# # plt.yticks(fontsize=12)
# # plt.legend()
# # plt.grid(True)

# # # Optical
# # plt.subplot(3, 2, 5)
# # plt.plot(errors_df['Frames'], errors_df['Optical_x_Error'], label='Optical_X_Error', alpha=0.7)
# # plt.plot(errors_df['Frames'], errors_df['Optical_x_Error_MA'], label='Optical_x_Error_MA', color='red', linewidth=2)
# # plt.xlabel('Frame', fontsize=13)
# # plt.ylabel('Absolute Error', fontsize=13)
# # plt.title('Optical X Coordinate', fontsize=14)
# # plt.xticks(fontsize=12)
# # plt.yticks(fontsize=12)
# # plt.legend()
# # plt.grid(True)

# # plt.subplot(3, 2, 6)
# # plt.plot(errors_df['Frames'], errors_df['Optical_y_Error'], label='Optical_Y_Error', alpha=0.7)
# # plt.plot(errors_df['Frames'], errors_df['Optical_y_Error_MA'], label='Optical_y_Error_MA', color='red', linewidth=2)
# # plt.xlabel('Frame', fontsize=13)
# # plt.ylabel('Absolute Error', fontsize=13)
# # plt.title('Optical Y Coordinate', fontsize=14)
# # plt.xticks(fontsize=12)
# # plt.yticks(fontsize=12)
# # plt.legend()
# # plt.grid(True)

# # plt.tight_layout()
# # plt.show()



# distance_errors = pd.DataFrame({'UWB_Distance': [], 'Pixel_to_Real_Distance': [], 'Optical_Distance': []})
# # Calculate the Euclidean distance between UWB points and reference coordinates
# distance_errors['UWB_Distance'] = np.sqrt(
#     (ref_coords['x'] - uwb_coords['x'])**2 + (ref_coords['y'] - uwb_coords['y'])**2
# )

# # Calculate the Euclidean distance between Pixel to Real points and reference coordinates
# distance_errors['Pixel_to_Real_Distance'] = np.sqrt(
#     (ref_coords['x'] - model_coords['x'])**2 + (ref_coords['y'] - model_coords['y'])**2
# )

# # Calculate the Euclidean distance between Optical points and reference coordinates
# distance_errors['Optical_Distance'] = np.sqrt(
#     (ref_coords['x'] - optical_coords['x'])**2 + (ref_coords['y'] - optical_coords['y'])**2
# )

# # Calculate summary statistics for distances
# distance_metrics = {
#     'Method': ['UWB', 'Pixel-to-Real', 'Optical'],
#     'Mean Distance': [distance_errors['UWB_Distance'].mean(), distance_errors['Pixel_to_Real_Distance'].mean(), distance_errors['Optical_Distance'].mean()],
#     'Median Distance': [distance_errors['UWB_Distance'].median(), distance_errors['Pixel_to_Real_Distance'].median(), distance_errors['Optical_Distance'].median()],
#     'Max Distance': [distance_errors['UWB_Distance'].max(), distance_errors['Pixel_to_Real_Distance'].max(), distance_errors['Optical_Distance'].max()],
#     'Min Distance': [distance_errors['UWB_Distance'].min(), distance_errors['Pixel_to_Real_Distance'].min(), distance_errors['Optical_Distance'].min()],
#     'Std Dev Distance': [distance_errors['UWB_Distance'].std(), distance_errors['Pixel_to_Real_Distance'].std(), distance_errors['Optical_Distance'].std()]
# }

# distance_metrics_df = pd.DataFrame(distance_metrics)

# print(distance_metrics_df)

# # # Plotting the distance distributions for UWB and Optical methods
# # plt.figure(figsize=(14, 8))

# # # Histogram for UWB distances
# # plt.subplot(2, 1, 1)
# # plt.hist(distance_errors['UWB_Distance'], bins=50, alpha=0.7, color='blue')
# # plt.title('Histogram of UWB Distances')
# # plt.xlabel('Distance')
# # plt.ylabel('Frequency')
# # plt.grid(True)

# # # Histogram for Optical distances
# # plt.subplot(2, 1, 2)
# # plt.hist(distance_errors['Optical_Distance'], bins=50, alpha=0.7, color='red')
# # plt.title('Histogram of Optical Distances')
# # plt.xlabel('Distance')
# # plt.ylabel('Frequency')
# # plt.grid(True)

# # plt.tight_layout()
# # plt.show()

# # Combine the distances into a single DataFrame for easier plotting
# distance_data = pd.melt(distance_errors, value_vars=['UWB_Distance', 'Pixel_to_Real_Distance', 'Optical_Distance'], var_name='Method', value_name='Distance')

# # # Box Plot
# # plt.figure(figsize=(14, 6))
# # sns.boxplot(x='Method', y='Distance', data=distance_data)
# # plt.title('Box Plot of UWB and Optical Distances')
# # plt.grid(True)
# # plt.show()

# # # Violin Plot
# # plt.figure(figsize=(14, 6))
# # sns.violinplot(x='Method', y='Distance', data=distance_data)
# # plt.title('Violin Plot of UWB and Optical Distances')
# # plt.grid(True)
# # plt.show()

# # Scatter Plot: Comparing UWB and Optical distances
# plt.figure(figsize=(14, 6))
# plt.scatter(errors_df['Frames'], distance_errors['UWB_Distance'], alpha=0.5, label='UWB', color='green')
# plt.scatter(errors_df['Frames'], distance_errors['Pixel_to_Real_Distance'], alpha=0.5, label='Pixel-to-Real', color='orange')
# plt.scatter(errors_df['Frames'], distance_errors['Optical_Distance'], alpha=0.5, label='Optical', color='red')
# plt.title('Scatter Plot of Distance Errors between Estimated Coordinates \nand Ground Truth Coordinates Over Time. Test Environment 3.', fontsize=16)
# plt.xlabel('Frames', fontsize=14)
# plt.ylabel('Distance Errors', fontsize=14)
# plt.xticks(fontsize=12)
# plt.yticks(fontsize=12)
# plt.legend()
# plt.grid(True)
# # plt.show()

# # Histogram Plot: Distribution of UWB and Optical distances
# plt.figure(figsize=(14, 10))
# plt.suptitle('Histogram Plot of Distance Errors between Estimated Coordinates \nand Ground Truth Coordinates Over Time. Test Environment 3.', fontsize=16)
# plt.subplot(3, 1, 1)
# plt.hist(distance_errors['UWB_Distance'], bins=50, alpha=0.7, label='UWB', color='green')
# plt.title('Distance Errors between UWB and Ground Truth Coordinates', fontsize=14)
# plt.xlabel('Distance Errors', fontsize=14)
# plt.ylabel('Frequency', fontsize=14)
# plt.xticks(fontsize=12)
# plt.yticks(fontsize=12)
# plt.grid(True)

# plt.subplot(3, 1, 2)
# plt.hist(distance_errors['Pixel_to_Real_Distance'], bins=50, alpha=0.7, label='Pixel-to-Real', color='orange')
# plt.title('Distance Errors between Pixel-to-Real and Ground Truth Coordinates', fontsize=14)
# plt.xlabel('Distance Errors', fontsize=14)
# plt.ylabel('Frequency', fontsize=14)
# plt.xticks(fontsize=12)
# plt.yticks(fontsize=12)
# plt.grid(True)

# plt.subplot(3, 1, 3)
# plt.hist(distance_errors['Optical_Distance'], bins=50, alpha=0.7, label='Optical', color='red')
# plt.title('Distance Errors between Optical and Ground Truth Coordinates', fontsize=14)
# plt.xlabel('Distance Errors', fontsize=14)
# plt.ylabel('Frequency', fontsize=14)
# plt.xticks(fontsize=12)
# plt.yticks(fontsize=12)
# plt.grid(True)
# plt.tight_layout()

# plt.show()