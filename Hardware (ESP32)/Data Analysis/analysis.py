from sklearn.metrics import mean_squared_error
from sklearn.preprocessing import PolynomialFeatures
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression


column_names = [
    'Measurement_ID', 'Timestamp', 'Tag_ID',
    'First_Anchor_ID', 'Distance_From_First_Anchor',
    'Second_Anchor_ID', 'Distance_From_Second_Anchor'
]

file_path = "UWB_timestamps.txt"

data = pd.read_csv(file_path, delim_whitespace=True, names=column_names)


######################## Linear Regression ###############################

# # Plotting the distance measurements over time to identify standing and moving periods
# plt.figure(figsize=(15,6))

# # Plotting distance from the first anchor
# plt.plot(data['Timestamp'], data['Distance_From_First_Anchor'],
#          label='Distance from First Anchor', color='blue')

# # Adding labels and title
# plt.xlabel('Timestamp')
# plt.ylabel('Distance (meters)')
# plt.title('Distance from First Anchor Over Time')
# plt.legend()

# # Show the plot
# plt.show()

# # Calculating a rolling standard deviation to identify periods of low and high variability
# # Window size for rolling calculation, can be adjusted based on the data frequency
# window_size = 10

# data['Rolling_STD'] = data['Distance_From_First_Anchor'].rolling(window=window_size).std()

# # with pd.option_context('display.max_rows', None, 'display.max_columns', None):
# #     print(data['Rolling_STD'])

# # Plotting the rolling standard deviation over time
# plt.figure(figsize=(15,6))

# # Plotting rolling standard deviation
# plt.plot(data['Timestamp'], data['Rolling_STD'],
#          label='Rolling Standard Deviation', color='red')

# # Adding labels and title
# plt.xlabel('Timestamp')
# plt.ylabel('Standard Deviation (meters)')
# plt.title('Rolling Standard Deviation of Distance Measurements Over Time')
# plt.legend()

# # Show the plot
# plt.show()

# # Setting a threshold for the rolling standard deviation to classify standing vs moving
# # This threshold can be adjusted based on the data characteristics
# # std_threshold = data['Rolling_STD'].mean() / 2

# std_threshold = 0.1

# # print("Standard Threshold: ", std_threshold)

# # Classifying each measurement as 'standing' or 'moving'
# data['Status'] = np.where((data['Rolling_STD'] <= std_threshold) | (
#     data['Rolling_STD'].isna()), 'standing', 'moving')

# # with pd.option_context('display.max_rows', None, 'display.max_columns', None):
# #     print(data['Status'])

# # Identifying the points where the status switches between standing and moving
# data['Status_Change'] = data['Status'].ne(
#     data['Status'].shift())

# # with pd.option_context('display.max_rows', None, 'display.max_columns', None):
# #     print(data[['Distance_From_First_Anchor',
# #           'Rolling_STD', 'Status', 'Status_Change']])


# # Filtering data by 'standing' status
# standing_data = data.loc[data['Status'] == 'standing'].copy()

# # Calculating segments using Status_Change column
# standing_data['Segment'] = standing_data['Status_Change'].cumsum()

# # Calculating mean distances for each segment
# mean_distances_per_segment = standing_data.groupby(
#     'Segment')['Distance_From_First_Anchor'].mean()

# # with pd.option_context('display.max_rows', None, 'display.max_columns', None):
# #     print(standing_data.columns)

# # Comparison with Ground Truth

# # Known ground truth distances (1 to 10 meters)
# ground_truth_distances = range(1,11)

# # Calculating deviation for each segment (mean distance - ground truth)
# # deviations = mean_distances_per_segment - ground_truth_distances

# # Creating a DataFrame for comparison
# # comparison_df = pd.DataFrame({
# #     'Segment': mean_distances_per_segment.index,
# #     'Mean Distance': mean_distances_per_segment.values,
# #     'Ground Truth': ground_truth_distances,
# #     'Deviation': deviations.values
# #})

# # print(comparison_df)

# ###
# # Given that the deviations are relatively small and seem to increase slightly with distance, a linear mapping function might be a suitable starting point. This function would take the form:
# # Adjusted Distance = a × Measured Distance + b
# # where a and b are coefficients that we determine based on the deviation pattern. We can use linear regression to find these coefficients, fitting the function to the mean distances and their corresponding ground truth values.
# ###

# # # Preparing the data for linear regression
# # # Independent variable (Segment number)
# # X = np.array(mean_distances_per_segment.values).reshape(-1,1)
# # # Dependent variable (Mean Distance)
# # y = ground_truth_distances

# # # Creating a linear regression model
# # model = LinearRegression()
# # model.fit(X, y)

# # # Coefficients a and b
# # a = model.coef_[0]
# # b = model.intercept_

# # # Applying the corrected mapping function to the original dataset
# # data['Corrected_Adjusted_Distance'] = data['Distance_From_First_Anchor'] * a + b

# # with pd.option_context('display.max_rows', None, 'display.max_columns', None):
# #     print(data[    ['Distance_From_First_Anchor', 'Corrected_Adjusted_Distance'    ]])

# # # Now we can calculate adjusted values. They are still different from the ground truth values, but this approach can be applied iteratively.


# ######################## Piecewise Regression ###############################

# # Piecewise regression allows us to model the data differently for distances below and above the 5-meter calibration point. This approach acknowledges that the relationship between the measured and true distances may vary depending on whether the tag is closer to or farther from the anchor than the calibration distance.
# # For piecewise regression, we'll do the following:
# # Segment the Data: Split the data into two segments based on the 5-meter mark.
# # Perform Linear Regression on Each Segment: Apply a separate linear regression to each segment.
# # Compare Results: Evaluate how each model fits the data in its respective range.


# #############################################################################


# ######################## Polynomial Regression ##############################


# degree = 2

# polynomial_features = PolynomialFeatures(degree=degree)
# X_poly = polynomial_features.fit_transform(
#     mean_distances_per_segment.values.reshape(-1,1))

# model = LinearRegression()
# model.fit(X_poly, ground_truth_distances)
# y_poly_pred = model.predict(X_poly)

# # Calculating the mean squared error
# rmse = np.sqrt(mean_squared_error(ground_truth_distances, y_poly_pred))

# # Coefficients of the polynomial regression
# coefficients = model.coef_

# rmse, coefficients

# y_poly_pred = model.predict(polynomial_features.transform(
#     data[    ['Distance_From_First_Anchor'    ]]))
# y_poly_pred

# #############################################################################
