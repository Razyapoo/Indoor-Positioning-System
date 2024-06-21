import numpy as np
import pandas as pd
from sklearn.ensemble import RandomForestRegressor
from sklearn.preprocessing import PolynomialFeatures
from sklearn.linear_model import LinearRegression
from sklearn.model_selection import train_test_split
from sklearn.metrics import mean_squared_error
import json
import xgboost as xgb

file_path = '/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/data.txt'
column_names = ['Record_id', 'X_World', 'Y_World', 'x_pixel', 'y_pixel']
data_loaded = pd.read_csv(file_path, names=column_names, delim_whitespace=True)


# # print(data_loaded.head())


# # Step 1: Define inputs and outputs
# input_data = data_loaded[["x_pixel", "y_pixel"]].to_numpy()
# Y = data_loaded[["X_World", "Y_World"]].to_numpy()

# X = data_loaded[["x_pixel", "y_pixel"]]
# y_X = data_loaded["X_World"]
# y_Y = data_loaded["Y_World"]

# file_path = '/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/data copy'
# column_names = ['Record_id', 'X_World', 'Y_World', 'x_pixel', 'y_pixel']
# data_loaded = pd.read_csv(file_path, names=column_names, delim_whitespace=True)

# X_test = data_loaded[["x_pixel", "y_pixel"]]

# # Generating the feature matrix for a 2nd-degree polynomial model
# # Features: 1, x, y, x^2, y^2, xy
# n = input_data.shape[0]
# X = np.ones((n, 6))  # Initialize feature matrix with ones for intercept
# X[:, 1] = input_data[:, 0]  # x
# X[:, 2] = input_data[:, 1]  # y
# X[:, 3] = input_data[:, 0]**2  # x^2
# X[:, 4] = input_data[:, 1]**2  # y^2
# X[:, 5] = input_data[:, 0] * input_data[:, 1]  # xy

# # Linear Regression to find coefficients for y1 and y2
# coefficients_y1 = np.linalg.inv(X.T @ X) @ X.T @ Y[:, 0]
# coefficients_y2 = np.linalg.inv(X.T @ X) @ X.T @ Y[:, 1]

# print("The polynomial coefficients for y1 are:", coefficients_y1)
# print("The polynomial coefficients for y2 are:", coefficients_y2)

# # Predicting for a new input pair (x=3, y=2)
# new_input = np.array([154, 265])
# new_features = np.array([1, new_input[0], new_input[1], new_input[0]
#                         ** 2, new_input[1]**2, new_input[0] * new_input[1]])

# y1_pred = new_features @ coefficients_y1
# y2_pred = new_features @ coefficients_y2

# print(f"Predicted values at (x=244, y=174): y1={y1_pred}, y2={y2_pred}")


# # Generating the feature matrix for a 3rd-degree polynomial model
# # Features: 1, x, y, x^2, y^2, xy, x^3, y^3, x^2y, xy^2
# n = input_data.shape[0]
# X = np.ones((n, 10))  # Initialize feature matrix with ones for intercept
# X[:, 1] = input_data[:, 0]  # x
# X[:, 2] = input_data[:, 1]  # y
# X[:, 3] = input_data[:, 0]**2  # x^2
# X[:, 4] = input_data[:, 1]**2  # y^2
# X[:, 5] = input_data[:, 0] * input_data[:, 1]  # xy
# X[:, 6] = input_data[:, 0]**3  # x^3
# X[:, 7] = input_data[:, 1]**3  # y^3
# X[:, 8] = (input_data[:, 0]**2) * input_data[:, 1]  # x^2y
# X[:, 9] = input_data[:, 0] * (input_data[:, 1]**2)  # xy^2

# Generating the feature matrix for a 4th-degree polynomial model
# Features: 1, x, y, x^2, y^2, xy, x^3, y^3, x^2y, xy^2, x^4, y^4, x^3y, x^2y^2, xy^3
# n = input_data.shape[0]
# X = np.ones((n, 15))  # Initialize feature matrix with ones for intercept
# X[:, 1] = input_data[:, 0]  # x
# X[:, 2] = input_data[:, 1]  # y
# X[:, 3] = input_data[:, 0]**2  # x^2
# X[:, 4] = input_data[:, 1]**2  # y^2
# X[:, 5] = input_data[:, 0] * input_data[:, 1]  # xy
# X[:, 6] = input_data[:, 0]**3  # x^3
# X[:, 7] = input_data[:, 1]**3  # y^3
# X[:, 8] = (input_data[:, 0]**2) * input_data[:, 1]  # x^2y
# X[:, 9] = input_data[:, 0] * (input_data[:, 1]**2)  # xy^2
# X[:, 10] = input_data[:, 0]**4  # x^4
# X[:, 11] = input_data[:, 1]**4  # y^4
# X[:, 12] = (input_data[:, 0]**3) * input_data[:, 1]  # x^3y
# X[:, 13] = (input_data[:, 0]**2) * (input_data[:, 1]**2)  # x^2y^2
# X[:, 14] = input_data[:, 0] * (input_data[:, 1]**3)  # xy^3

# # Linear Regression to find coefficients for y1 and y2
# coefficients_y1 = np.linalg.inv(X.T @ X) @ X.T @ Y[:, 0]
# coefficients_y2 = np.linalg.inv(X.T @ X) @ X.T @ Y[:, 1]

# print("The polynomial coefficients for y1 are:", coefficients_y1)
# print("The polynomial coefficients for y2 are:", coefficients_y2)

# # Predicting for a new input pair (x=3, y=2)
# new_input = np.array([181, 233])
# # new_features = np.array([1, new_input[0], new_input[1], new_input[0]**2, new_input[1]**2,
# #                          new_input[0] *
# #                          new_input[1], new_input[0]**3, new_input[1]**3,
# #                          (new_input[0]**2) * new_input[1], new_input[0] * (new_input[1]**2)])

# new_features = np.array([1, new_input[0], new_input[1], new_input[0]**2, new_input[1]**2,
#                          new_input[0] *
#                          new_input[1], new_input[0]**3, new_input[1]**3,
#                          (new_input[0]**2) *
#                          new_input[1], new_input[0] * (new_input[1]**2),
#                          new_input[0]**4, new_input[1]**4, (new_input[0]
#                                                             ** 3) * new_input[1],
#                          (new_input[0]**2) * (new_input[1]**2), new_input[0] * (new_input[1]**3)])


# y1_pred = new_features @ coefficients_y1
# y2_pred = new_features @ coefficients_y2

# print(f"Predicted values at (x=244, y=174): y1={y1_pred}, y2={y2_pred}")


# # Step 2: Polynomial Features Transformation
# poly = PolynomialFeatures(degree=3, include_bias=False)
# X_poly = poly.fit_transform(X)
# X_poly_test = poly.fit_transform(X_test)

# # Step 3: Split data into training and testing sets for both outputs
# # X_train, X_test, y_X_train, y_X_test = train_test_split(
# # X_poly, y_X, test_size=0.2, random_state=42)

# # _, _, y_Y_train, y_Y_test = train_test_split(
# # X_poly, y_Y, test_size=0.2, random_state=42)

# # Step 4: Model to predict X_World
# model_X = LinearRegression()
# model_X.fit(X_poly, y_X)


# # Model to predict Y_World
# model_Y = LinearRegression()
# model_Y.fit(X_poly, y_Y)

# r_sq_x = model_X.score(X_poly, y_X)
# intercept_x, coefficients_x = model_X.intercept_, model_X.coef_
# y_pred_x = model_X.predict(X_poly_test)

# np.set_printoptions(suppress=True)

# print(f"coefficient of determination: {r_sq_x}")
# print(f"intercept: {intercept_x}")
# print(f"coefficients:\n{coefficients_x}")
# print(f"predicted response:\n{y_pred_x}")

# r_sq_y = model_Y.score(X_poly, y_Y)
# intercept_y, coefficients_y = model_Y.intercept_, model_Y.coef_
# y_pred_y = model_Y.predict(X_poly_test)

# print(f"coefficient of determination: {r_sq_y}")
# print(f"intercept: {intercept_y}")
# print(f"coefficients:\n{coefficients_y}")
# print(f"predicted response:\n{y_pred_y}")


# # # Step 5: Evaluate the models
# # # mse_X = mean_squared_error(y_X_test, y_X_pred)
# # # mse_Y = mean_squared_error(y_Y_test, y_Y_pred)

# # # (mse_X, mse_Y)


# # # model_params = {
# # # "model_X_coefficients": model_X.coef_.tolist(),
# # # "model_X_intercept": model_X.intercept_,
# # # "model_Y_coefficients": model_Y.coef_.tolist(),
# # # "model_Y_intercept": model_Y.intercept_,
# # # "polynomial_powers": poly.powers_.tolist()
# # # }

# # # with open('params.json', 'w') as file:
# # # json.dump(model_params, file)


# X_train = data_loaded[["x_pixel", "y_pixel"]]
# y_train = data_loaded[["X_World", "Y_World"]]

# X_train = data_loaded.iloc[::3, 3:5]
# y_train = data_loaded.iloc[::3, 1:3]
# Splitting the dataset into training and testing sets
# X_train, X_test, y_train, y_test = train_test_split(
#     X, y, test_size=0.2, random_state=42)

# file_path = '/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/data copy'
# column_names = ['Record_id', 'X_World', 'Y_World', 'x_pixel', 'y_pixel']
# data_loaded = pd.read_csv(file_path, names=column_names, delim_whitespace=True)

X = data_loaded[["x_pixel", "y_pixel"]]
y = data_loaded[["X_World", "Y_World"]]

# Training a Random Forest Regressor
# rf = RandomForestRegressor(n_estimators=10000, random_state=42)
# rf.fit(X, y)

# # Predicting on the test set
# y_pred = rf.predict(X)

# # Calculating the mean squared error
# mse = mean_squared_error(y, y_pred)
# rmse = np.sqrt(mse)

# # Outputting the RMSE as an accuracy metric
# print("RMSE: ", rmse)
# print(y_pred)

dtrain = xgb.DMatrix(X, label=y)
# params = {'max_depth': 2, 'eta': 1, 'objective': 'reg:squarederror'}
# params = {
#     'max_depth': 3,
#     'min_child_weight': 1,
# #     'n_estimators': 10,
# #     'learning_rate': 0.05,
# #     'subsample': 0.8,
# #     'colsample_bytree': 0.8,
#     #     'lambda': 1.0,
#     #     'alpha': 1.0,
#     #     'gamma': 1.0,
#     'objective': 'reg:squarederror'  # or 'binary:logistic' for classification
# }

# Deterministic
params = {
    'max_depth': 2,
    'eta': 1,
    'objective': 'reg:squarederror',
    'seed': 42,  # Set a fixed random seed
    'nthread': 1  # Set the number of threads to ensure consistency
}

num_round = 100

bst = xgb.train(params, dtrain, num_round)

y_pred = bst.predict(dtrain)

result = data_loaded[["x_pixel", "y_pixel", "X_World", "Y_World"]]
result['X_World_Predicted'] = y_pred[:, 0]
result['Y_World_Predicted'] = y_pred[:, 1]
pd.set_option('display.max_rows', None)

print(result[["x_pixel", "y_pixel", "X_World",
      "X_World_Predicted", "Y_World", "Y_World_Predicted"]])


# Save the model
bst.save_model(
    '/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/xgboost_model.json')
