import numpy as np
import pandas as pd
import math

# Load distances from the file
filePath = 'Data for analysis/Data for comparison/Ground truth distances/s8 1 person/ground_truth_distances.txt'
data = np.loadtxt(filePath)

# Define anchor coordinates
anchorCoordinates = {
    101: (0, 0),
    102: (2.5, 0)
}

# Function to calculate groundTruth coordinates
def calculateGroundTruthCoordinates(distanceAnchor1, distanceAnchor2):
    anchor1Coordinates = anchorCoordinates[101]
    anchor2Coordinates = anchorCoordinates[102]
    
    anchorBaseline = math.sqrt((anchor1Coordinates[0] - anchor2Coordinates[0]) ** 2 +
                                (anchor1Coordinates[1] - anchor2Coordinates[1]) ** 2)

    x = abs((distanceAnchor1 ** 2 - distanceAnchor2 ** 2 + anchorBaseline ** 2) / (2 * anchorBaseline))
    y = math.sqrt(distanceAnchor1 ** 2 - x ** 2)

    xTransformed = x + 0.627
    yTransformed = y + 2.08

    return xTransformed, yTransformed

# Calculate groundTruth coordinates for all pairs of distances
groundTruthCoordinates = np.array([calculateGroundTruthCoordinates(d[0], d[1]) for d in data])

# Create a DataFrame for better visualization
groundTruthDf = pd.DataFrame(groundTruthCoordinates, columns=['x', 'y'])

groundTruthDf.to_csv('Data for analysis/Data for comparison/Ground truth distances/s8 1 person/ground_truth_coordinates.txt', index=False, sep=' ', header=False)
