import numpy as np
import pandas as pd
import math

# Load distances from the file
filePath = '/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/Data for comparison/Reference distances/s8 3 people/reference_distances_person_3.txt'
data = np.loadtxt(filePath)

# Define anchor coordinates
anchorCoordinates = {
    101: (2.5, 0),
    102: (0, 0)
}

# Function to calculate reference coordinates
def calculateReferenceCoordinates(distanceAnchor1, distanceAnchor2):
    anchor1Coordinates = anchorCoordinates[101]
    anchor2Coordinates = anchorCoordinates[102]
    
    anchorBaseline = math.sqrt((anchor1Coordinates[0] - anchor2Coordinates[0]) ** 2 +
                                (anchor1Coordinates[1] - anchor2Coordinates[1]) ** 2)

    x = abs((distanceAnchor2 ** 2 - distanceAnchor1 ** 2 + anchorBaseline ** 2) / (2 * anchorBaseline))
    y = math.sqrt(distanceAnchor2 ** 2 - x ** 2)

    xTransformed = x + 0.627
    yTransformed = y + 2.08

    return xTransformed, yTransformed

# Calculate reference coordinates for all pairs of distances
referenceCoordinates = np.array([calculateReferenceCoordinates(d[0], d[1]) for d in data])

# Create a DataFrame for better visualization
referenceDf = pd.DataFrame(referenceCoordinates, columns=['x', 'y'])

referenceDf.to_csv('/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/Data for comparison/Reference distances/s8 3 people/reference_coordinates_person_3.txt', index=False, sep=' ', header=False)
