"""
/*********************************************** XGBoost ***************************************************************
 * eXtreme Gradient Boosting is a machine learning algorithm that works based on gradient boosted decision tree.
 *   - https://dl.acm.org/doi/10.1145/2939672.2939785
***********************************************************************************************************************/
"""
import pandas as pd
import xgboost as xgb

# Load data
filePath = '/home/oskar/Documents/Master Thesis/Software (Image processing)/Train model/data.txt'
columnNames = ['Record_id', 'X_World', 'Y_World', 'x_pixel', 'y_pixel']
loadedData = pd.read_csv(filePath, names=columnNames, delim_whitespace=True)

X = loadedData[["x_pixel", "y_pixel"]]
y = loadedData[["X_World", "Y_World"]]

dTrain = xgb.DMatrix(X, label=y)

# Ensure the model is deterministic
params = {
    'max_depth': 2,
    'eta': 1,
    'objective': 'reg:squarederror',
    'seed': 42,  # Set a fixed random seed to achieve the determinism
    'nthread': 1  # Set the number of threads to ensure consistency
}

numRound = 100
bst = xgb.train(params, dTrain, numRound)

yPred = bst.predict(dTrain)

result = loadedData[["x_pixel", "y_pixel", "X_World", "Y_World"]]
result['X_World_Predicted'] = yPred[:, 0]
result['Y_World_Predicted'] = yPred[:, 1]
pd.set_option('display.max_rows', None)

# Print reults
print(result[["x_pixel", "y_pixel", "X_World",
      "X_World_Predicted", "Y_World", "Y_World_Predicted"]])

# Save the model
bst.save_model(
    '/home/oskar/Documents/Master Thesis/Software (Image processing)/Train model/xgboost_model.json')
