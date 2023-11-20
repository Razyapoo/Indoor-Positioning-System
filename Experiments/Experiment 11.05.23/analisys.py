import pandas as pd
import numpy as np

from sklearn.linear_model import LinearRegression

file_path = '/content/sample_data/tag_data.txt'

column_names = [
    'Measurement_ID', 'Timestamp', 'Tag_ID',
    'First_Anchor_ID', 'Distance_From_First_Anchor',
    'Second_Anchor_ID', 'Distance_From_Second_Anchor'
]

# Re-importing and preparing the data again due to the reset state
raw_data = pd.read_csv(file_path, delim_whitespace=True, names=column_names)

# Calculating rolling standard deviation for 'standing' detection
rolling_std = raw_data['Distance_From_First_Anchor'].rolling(window=10).std()
raw_data['Status'] = np.where((rolling_std <= 0.1) | (np.isnan(rolling_std)), 'standing', 'moving')

# Identifying status changes for segmentation
raw_data['Status_Change'] = raw_data['Status'].ne(raw_data['Status'].shift())
standing_data = raw_data[raw_data['Status'] == 'standing'].copy()
standing_data['Segment'] = standing_data['Status_Change'].cumsum()


# # Grouping data by segments and fitting linear regression models for each segment
# segment_models = {}
# for segment in standing_data['Segment'].unique():
#     segment_data = standing_data[standing_data['Segment'] == segment]
#     model = LinearRegression().fit(segment_data[['Distance_From_First_Anchor']], np.full(len(segment_data), segment))
#     segment_models[segment] = model

# # Displaying the number of segments and a sample model's coefficients
# len(segment_models), segment_models

# a = segment_models[1].coef_[0]
# b = segment_models[1].intercept_

# adjusted_data_1m = standing_data[standing_data['Segment'] == 1].copy()
# adjusted_data_1m['Corrected_Adjusted_Distance'] = adjusted_data_1m['Distance_From_First_Anchor'] * a + b

# a,b

### Worser than with mean values

model = LinearRegression().fit(standing_data[['Distance_From_First_Anchor']], standing_data['Segment'])
model


a = model.coef_[0]
b = model.intercept_

adjusted_data_1m = standing_data[standing_data['Segment'] == 10].copy()
adjusted_data_1m['Corrected_Adjusted_Distance'] = adjusted_data_1m['Distance_From_First_Anchor'] * a + b
adjusted_data_1m[['Distance_From_First_Anchor', 'Corrected_Adjusted_Distance']]


########################### Polynomial regression ##############################

from sklearn.preprocessing import PolynomialFeatures
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error

degree = 2

mean_adjusted_distances_per_segment_new = standing_data.groupby('Segment')['Distance_From_First_Anchor'].mean()
ground_truth_distances = range(1, 11)

polynomial_features = PolynomialFeatures(degree=degree)
X_poly = polynomial_features.fit_transform(mean_adjusted_distances_per_segment_new.values.reshape(-1, 1))

model = LinearRegression()
model.fit(X_poly, ground_truth_distances)
y_poly_pred = model.predict(X_poly)

# Calculating the mean squared error
rmse = np.sqrt(mean_squared_error(ground_truth_distances, y_poly_pred))

# Coefficients of the polynomial regression
coefficients = model.coef_

rmse, coefficients

y_poly_pred = model.predict(polynomial_features.transform(raw_data[['Distance_From_First_Anchor']]))
y_poly_pred
