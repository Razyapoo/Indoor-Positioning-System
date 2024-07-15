import pandas as pd
import xgboost as xgb

filePath = '/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/data.txt'
columnNames = ['Record_id', 'X_World', 'Y_World', 'x_pixel', 'y_pixel']
loadedData = pd.read_csv(filePath, names=columnNames, delim_whitespace=True)

X = loadedData[["x_pixel", "y_pixel"]]
y = loadedData[["X_World", "Y_World"]]

dTrain = xgb.DMatrix(X, label=y)

# Deterministic
params = {
    'max_depth': 2,
    'eta': 1,
    'objective': 'reg:squarederror',
    'seed': 42,  # Set a fixed random seed to achieve the determinismus
    'nthread': 1  # Set the number of threads to ensure consistency
}

numRound = 100

bst = xgb.train(params, dTrain, numRound)

yPred = bst.predict(dTrain)

result = loadedData[["x_pixel", "y_pixel", "X_World", "Y_World"]]
result['X_World_Predicted'] = yPred[:, 0]
result['Y_World_Predicted'] = yPred[:, 1]
pd.set_option('display.max_rows', None)

print(result[["x_pixel", "y_pixel", "X_World",
      "X_World_Predicted", "Y_World", "Y_World_Predicted"]])


# Save the model
bst.save_model(
    '/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/xgboost_model.json')
