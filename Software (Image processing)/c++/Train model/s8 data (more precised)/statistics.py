import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns


# Placeholder for estimated coordinates (replace with actual data)
uwb_coords = pd.DataFrame({'x': [], 'y': []})
optical_coords = pd.DataFrame({'x': [], 'y': []})
model_coords = pd.DataFrame({'x': [], 'y': []})

frames = pd.read_csv('/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/s8 New data (more precised)/uwb_to_bb_mapping_entire.txt', sep=' ', header=None, usecols=[0])
frames.columns = ['Frames']

# Load the reference coordinates
ref_coords_path = '/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/s8 New data (more precised)/reference_coordinates.txt'
ref_coords = pd.read_csv(ref_coords_path, sep=' ', header=None)
ref_coords.columns = ['x', 'y']

uwb_coords = pd.read_csv('/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/s8 New data (more precised)/uwb_to_bb_mapping_entire.txt', sep=' ', header=None, usecols=[3, 4])
uwb_coords.columns = ['x', 'y']

optical_coords = pd.read_csv('/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/s8 New data (more precised)/optical_to_bb_mapping_entire.txt', sep=' ', header=None, usecols=[3, 4])
optical_coords.columns = ['x', 'y']

model_coords = pd.read_csv('/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/s8 New data (more precised)/pixel_to_real_to_bb_mapping_entire.txt', sep=' ', header=None, usecols=[3, 4])
model_coords.columns = ['x', 'y']

mae_x = (ref_coords['x'] - uwb_coords['x']).abs().mean()
mae_y = (ref_coords['y'] - uwb_coords['y']).abs().mean()

# Mean Squared Error (MSE)
mse_x = ((ref_coords['x'] - uwb_coords['x']) ** 2).mean()
mse_y = ((ref_coords['y'] - uwb_coords['y']) ** 2).mean()

# Root Mean Squared Error (RMSE)
rmse_x = np.sqrt(mse_x)
rmse_y = np.sqrt(mse_y)

# R-squared (R²)
ss_res_x = ((ref_coords['x'] - uwb_coords['x']) ** 2).sum()
ss_res_y = ((ref_coords['y'] - uwb_coords['y']) ** 2).sum()
ss_tot_x = ((ref_coords['x'] - ref_coords['x'].mean()) ** 2).sum()
ss_tot_y = ((ref_coords['y'] - ref_coords['y'].mean()) ** 2).sum()
r2_x = 1 - (ss_res_x / ss_tot_x)
r2_y = 1 - (ss_res_y / ss_tot_y)

# Pearson Correlation Coefficient
corr_x = ref_coords['x'].corr(uwb_coords['x'])
corr_y = ref_coords['y'].corr(uwb_coords['y'])


print(f"MAE X: {mae_x} MAE Y: {mae_y}")
print(f"MSE X: {mse_x} MSE Y: {mse_y}")
print(f"RMSE X: {rmse_x} RMSE Y: {rmse_y}")
print(f"R2 X: {r2_x} R2 Y: {r2_y}")
print(f"Pearson X: {corr_x} Pearson Y: {corr_y}")
