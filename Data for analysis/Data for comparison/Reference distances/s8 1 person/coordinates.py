import numpy as np
import pandas as pd
import math

# Load distances from the file
filePath = '/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/1 training video reference values/reference_distances.txt'
data = np.loadtxt(filePath)

# Define anchor coordinates
anchorCoordinates = {
    101: (0, 0),
    102: (2.5, 0)
}

# Function to calculate reference coordinates
def calculateReferenceCoordinates(distanceAnchor1, distanceAnchor2):
    anchor1Coordinates = anchorCoordinates[101]
    anchor2Coordinates = anchorCoordinates[102]
    
    anchorBaseline = math.sqrt((anchor1Coordinates[0] - anchor2Coordinates[0]) ** 2 +
                                (anchor1Coordinates[1] - anchor2Coordinates[1]) ** 2)

    x = abs((distanceAnchor1 ** 2 - distanceAnchor2 ** 2 + anchorBaseline ** 2) / (2 * anchorBaseline))
    y = math.sqrt(distanceAnchor1 ** 2 - x ** 2)

    xTransformed = x + 0.627
    yTransformed = y + 2.08

    return xTransformed, yTransformed

# Calculate reference coordinates for all pairs of distances
referenceCoordinates = np.array([calculateReferenceCoordinates(d[0], d[1]) for d in data])

# Create a DataFrame for better visualization
referenceDf = pd.DataFrame(referenceCoordinates, columns=['x', 'y'])

referenceDf.to_csv('/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/1 training video reference values/reference_coordinates.txt', index=False, sep=' ', header=False)
