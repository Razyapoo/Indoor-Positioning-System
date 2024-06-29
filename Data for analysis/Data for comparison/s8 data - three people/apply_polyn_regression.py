import pandas as pd
import numpy as np
import json

# Read polynomial parameters from JSON file
with open('/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/Data for comparison/s1 data - three people/polynomial_regression_params.json', 'r') as file:
    polynomialParameters = json.load(file)

# Read the data from the text file into a pandas DataFrame
df = pd.read_csv('/home/oskar/Documents/Master Thesis/Experiments/Video Player - test s8 - 3 people - parallel/UWB_timestamps.txt', sep=' ', header=None, 
                 names=['recordId', 'timestamp', 'tagId', 'anchor1', 'value1', 'anchor2', 'value2'])

# Convert columns to appropriate data types
df = df.astype({'recordId': int, 'timestamp': int, 'tagId': int, 'anchor1': int, 'value1': float, 'anchor2': int, 'value2': float})

def applyPolynomial(X, params):
    intercept, beta1, beta2 = params
    return intercept + beta1 * X + beta2 * X**2

# Apply polynomial regression and update values for each Tag_ID and Anchor pair
for tag in df['tagId'].unique():
    for anchor in [101, 102]:
        if str(tag) in polynomialParameters and str(anchor) in polynomialParameters[str(tag)]:
            params = polynomialParameters[str(tag)][str(anchor)]
            # Update value1 where anchor1 == anchor
            mask = (df['tagId'] == tag) & (df['anchor1'] == anchor)
            df.loc[mask, 'value1'] = applyPolynomial(df.loc[mask, 'value1'], params)
            # Update value2 where anchor2 == anchor
            mask = (df['tagId'] == tag) & (df['anchor2'] == anchor)
            df.loc[mask, 'value2'] = applyPolynomial(df.loc[mask, 'value2'], params)

# Write the result to a new text file
df.to_csv('/home/oskar/Documents/Master Thesis/Experiments/Video Player - test s8 - 3 people - parallel - after correction (PR)/UWB_timestamps.txt', sep=' ', index=False, header=False)
