import numpy as np
import pandas as pd

# Placeholder for estimated coordinates (replace with actual data)
uwb_coords = pd.DataFrame({'x': [], 'y': []})
optical_coords = pd.DataFrame({'x': [], 'y': []})
model_coords = pd.DataFrame({'x': [], 'y': []})

frames = pd.read_csv('/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/Data for comparison/s301 data - single person/export/uwb_to_bb_mapping_entire.txt', sep=' ', header=None, usecols=[0])
frames.columns = ['Frames']

# Load the reference coordinates
ref_coords_path = '/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/Data for comparison/s301 data - single person/reference_coordinates.txt'
ref_coords = pd.read_csv(ref_coords_path, sep=' ', header=None)
ref_coords.columns = ['x', 'y']

# Load estimated coordinates
uwb_coords = pd.read_csv('/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/Data for comparison/s301 data - single person/export/uwb_to_bb_mapping_entire.txt', sep=' ', header=None, usecols=[3, 4])
uwb_coords.columns = ['x', 'y']

optical_coords = pd.read_csv('/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/Data for comparison/s301 data - single person/export/optical_to_bb_mapping_entire.txt', sep=' ', header=None, usecols=[3, 4])
optical_coords.columns = ['x', 'y']

model_coords = pd.read_csv('/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/Data for comparison/s301 data - single person/export/pixel_to_real_to_bb_mapping_entire.txt', sep=' ', header=None, usecols=[3, 4])
model_coords.columns = ['x', 'y']

# Function to calculate metrics
def calculate_metrics(ref_coords, est_coords):
    mae_x = round((ref_coords['x'] - est_coords['x']).abs().mean(), 4)
    mae_y = round((ref_coords['y'] - est_coords['y']).abs().mean(), 4)

    mse_x = round(((ref_coords['x'] - est_coords['x']) ** 2).mean(), 4)
    mse_y = round(((ref_coords['y'] - est_coords['y']) ** 2).mean(), 4)

    rmse_x = round(np.sqrt(mse_x), 4)
    rmse_y = round(np.sqrt(mse_y), 4)

    corr_x = round(ref_coords['x'].corr(est_coords['x']), 4)
    corr_y = round(ref_coords['y'].corr(est_coords['y']), 4)

    return {
        'MAE_X': mae_x,
        'MSE_X': mse_x,
        'RMSE_X': rmse_x,
        'Pearson_X': corr_x,
        'MAE_Y': mae_y,
        'MSE_Y': mse_y,
        'RMSE_Y': rmse_y,
        'Pearson_Y': corr_y
    }

# Calculate metrics for each method
uwb_metrics = calculate_metrics(ref_coords, uwb_coords)
optical_metrics = calculate_metrics(ref_coords, optical_coords)
model_metrics = calculate_metrics(ref_coords, model_coords)

# Print metrics
print("UWB Metrics:")
for metric, value in uwb_metrics.items():
    print(f"{metric}: {value}")

print("\nOptical Metrics:")
for metric, value in optical_metrics.items():
    print(f"{metric}: {value}")

print("\nPixel-to-Real Metrics:")
for metric, value in model_metrics.items():
    print(f"{metric}: {value}")