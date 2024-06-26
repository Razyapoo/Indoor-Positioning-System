import numpy as np
import pandas as pd
import math

# Load distances from the file
file_path = '/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/1 training video reference values/reference_distances.txt'
data = np.loadtxt(file_path)

# Define anchor coordinates
anchor_coordinates = {
    101: (0, 0),
    102: (2.5, 0)
}

# Function to calculate reference coordinates
def calculate_reference_coordinates(distance_anchor1, distance_anchor2):
    anchor1_coordinates = anchor_coordinates[101]
    anchor2_coordinates = anchor_coordinates[102]
    
    anchor_baseline = math.sqrt((anchor1_coordinates[0] - anchor2_coordinates[0]) ** 2 +
                                (anchor1_coordinates[1] - anchor2_coordinates[1]) ** 2)

    x = abs((distance_anchor1 ** 2 - distance_anchor2 ** 2 + anchor_baseline ** 2) / (2 * anchor_baseline))
    y = math.sqrt(distance_anchor1 ** 2 - x ** 2)

    x_transformed = x + 0.627
    y_transformed = y + 2.08

    return x_transformed, y_transformed

# Calculate reference coordinates for all pairs of distances
reference_coordinates = np.array([calculate_reference_coordinates(d[0], d[1]) for d in data])

# Create a DataFrame for better visualization
reference_df = pd.DataFrame(reference_coordinates, columns=['x', 'y'])

reference_df.to_csv('/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/1 training video reference values/reference_coordinates.txt', index=False, sep=' ', header=False)
