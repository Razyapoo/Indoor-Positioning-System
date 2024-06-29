import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

def loadData(refPath, uwbPath, opticalPath, modelPath, useCols):
    """Load data from given file paths"""
    refCoords = pd.read_csv(refPath, sep=' ', header=None)
    refCoords.columns = ['x', 'y']

    uwbCoords = pd.read_csv(uwbPath, sep=' ', header=None, usecols=useCols)
    uwbCoords.columns = ['x', 'y']

    opticalCoords = pd.read_csv(opticalPath, sep=' ', header=None, usecols=useCols)
    opticalCoords.columns = ['x', 'y']

    modelCoords = pd.read_csv(modelPath, sep=' ', header=None, usecols=useCols)
    modelCoords.columns = ['x', 'y']

    return refCoords, uwbCoords, opticalCoords, modelCoords

def createSplits(data, splitSizes):
    """Split data into parts based on the splitSizes"""
    indices = np.cumsum(splitSizes)
    return np.split(data, indices[:-1])

def plotSplits(referenceSplits, estimatedSplits, estimatedLabel, estColor, title, refAlpha=0.3, estAlpha=1):
    """Plot the splits for the reference and estimated coordinates"""
    plt.figure(figsize=(15, 10))

    for i, (refSplit, estSplit) in enumerate(zip(referenceSplits, estimatedSplits)):
        refLabel = 'Reference Coordinates'
        estLabel = f'{estimatedLabel}'
        plt.plot(refSplit['x'], refSplit['y'], 'o-', color='blue', alpha=refAlpha, label=refLabel)
        plt.plot(estSplit['x'], estSplit['y'], 'x-', color=estColor, alpha=estAlpha, label=estLabel)

    plt.xlabel('X Coordinate', fontsize=14)
    plt.ylabel('Y Coordinate', fontsize=14)
    plt.title(title, fontsize=18)
    handles, labels = plt.gca().get_legend_handles_labels()
    byLabel = dict(zip(labels, handles))
    plt.legend(byLabel.values(), byLabel.keys(), fontsize=12, loc='upper right')
    plt.xticks(fontsize=12)
    plt.yticks(fontsize=12)
    plt.grid(True)
    plt.show()
    

# List of paths for different datasets
datasets = [
    {
        "refPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - single person/reference_coordinates.txt",
        "uwbPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - single person/uwb_to_bb_mapping_entire.txt",
        "opticalPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - single person/optical_to_bb_mapping_entire.txt",
        "modelPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - single person/pixel_to_real_to_bb_mapping_entire.txt",
        "useCols": [3, 4],
        "splitSizes": [28] + [29] * 6,
        "titleSuffix": "E109(DA_S8_S5(T1_A2_TPh_Md_Wpn)) - full area (16 meters)"
    },
    {
        "refPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - single person/reference_coordinates_reduced_range.txt",
        "uwbPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - single person/uwb_to_bb_mapping_entire_reduced_range.txt",
        "opticalPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - single person/optical_to_bb_mapping_entire_reduced_range.txt",
        "modelPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - single person/pixel_to_real_to_bb_mapping_entire_reduced_range.txt",
        "useCols": [3, 4],
        "splitSizes": [14] + [15] * 6,
        "titleSuffix": "E109(DA_S8_S5(T1_A2_TPh_Md_Wpn)) - reduced area (10 meters)"
    },
    {
        "refPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s301 data - single person/reference_coordinates.txt",
        "uwbPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s301 data - single person/export/uwb_to_bb_mapping_entire.txt",
        "opticalPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s301 data - single person/export/optical_to_bb_mapping_entire.txt",
        "modelPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s301 data - single person/export/pixel_to_real_to_bb_mapping_entire.txt",
        "useCols": [3, 4],
        "splitSizes": [12] * 6 + [11],
        "titleSuffix": "E113(DA_S301_S5(T1_A2_TPh_Md_Wpn))"
    },
    {
        "refPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/1 person/reference_coordinates.txt",
        "uwbPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/1 person/uwb_to_bb_mapping.txt",
        "opticalPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/1 person/optical_to_bb_mapping.txt",
        "modelPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/1 person/pixel_to_real_to_bb_mapping.txt",
        "useCols": [1, 2],
        "splitSizes": [27],
        "titleSuffix": "E118(DA_S8_S6(T3_A4_TPh_Md_Wp)) - Person 1 - full area (16 meters)", 
    },
    {
        "refPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/2 person/reference_coordinates.txt",
        "uwbPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/2 person/uwb_to_bb_mapping.txt",
        "opticalPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/2 person/optical_to_bb_mapping.txt",
        "modelPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/2 person/pixel_to_real_to_bb_mapping.txt",
        "useCols": [1, 2],
        "splitSizes": [27],
        "titleSuffix": "E118(DA_S8_S6(T3_A4_TPh_Md_Wp)) - Person 2 - full area (16 meters)", 
    },
    {
        "refPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/3 person/reference_coordinates.txt",
        "uwbPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/3 person/uwb_to_bb_mapping.txt",
        "opticalPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/3 person/optical_to_bb_mapping.txt",
        "modelPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/3 person/pixel_to_real_to_bb_mapping.txt",
        "useCols": [1, 2],
        "splitSizes": [27],
        "titleSuffix": "E118(DA_S8_S6(T3_A4_TPh_Md_Wp)) - Person 3 - full area (16 meters)", 
    },
    {
        "refPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/1 person/reference_coordinates_reduced_range.txt",
        "uwbPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/1 person/uwb_to_bb_mapping_reduced_range.txt",
        "opticalPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/1 person/optical_to_bb_mapping_reduced_range.txt",
        "modelPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/1 person/pixel_to_real_to_bb_mapping_reduced_range.txt",
        "useCols": [1, 2],
        "splitSizes": [15],
        "titleSuffix": "E118(DA_S8_S6(T3_A4_TPh_Md_Wp)) - Person 1 - reduced area (10 meters)", 
    },
    {
        "refPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/2 person/reference_coordinates_reduced_range.txt",
        "uwbPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/2 person/uwb_to_bb_mapping_reduced_range.txt",
        "opticalPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/2 person/optical_to_bb_mapping_reduced_range.txt",
        "modelPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/2 person/pixel_to_real_to_bb_mapping_reduced_range.txt",
        "useCols": [1, 2],
        "splitSizes": [15],
        "titleSuffix": "E118(DA_S8_S6(T3_A4_TPh_Md_Wp)) - Person 2 - reduced area (10 meters)", 
    },
    {
        "refPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/3 person/reference_coordinates_reduced_range.txt",
        "uwbPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/3 person/uwb_to_bb_mapping_reduced_range.txt",
        "opticalPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/3 person/optical_to_bb_mapping_reduced_range.txt",
        "modelPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/3 person/pixel_to_real_to_bb_mapping_reduced_range.txt",
        "useCols": [1, 2],
        "splitSizes": [15],
        "titleSuffix": "E118(DA_S8_S6(T3_A4_TPh_Md_Wp)) - Person 3 - reduced area (10 meters)", 
    },
    {
        "refPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s301 data - two people/1 person - 1 tag/reference_coordinates.txt",
        "uwbPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s301 data - two people/1 person - 1 tag/uwb_to_bb_mapping.txt",
        "opticalPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s301 data - two people/1 person - 1 tag/optical_to_bb_mapping.txt",
        "modelPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s301 data - two people/1 person - 1 tag/pixel_to_real_to_bb_mapping.txt",
        "useCols": [3, 4],
        "splitSizes": [9],
        "titleSuffix": "E124(DA_S301_S6(T2_A4_TPh_Md_Wp)) - Person 1"
    },
    {
        "refPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s301 data - two people/2 person - 2 tag/reference_coordinates.txt",
        "uwbPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s301 data - two people/2 person - 2 tag/uwb_to_bb_mapping.txt",
        "opticalPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s301 data - two people/2 person - 2 tag/optical_to_bb_mapping.txt",
        "modelPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s301 data - two people/2 person - 2 tag/pixel_to_real_to_bb_mapping.txt",
        "useCols": [3, 4],
        "splitSizes": [9],
        "titleSuffix": "E124(DA_S301_S6(T2_A4_TPh_Md_Wp)) - Person 2"
    },
]

# Process each dataset
for dataset in datasets:
    refCoords, uwbCoords, opticalCoords, modelCoords = loadData(
        dataset["refPath"], dataset["uwbPath"], dataset["opticalPath"], dataset["modelPath"], dataset["useCols"]
    )

    refSplits = createSplits(refCoords, dataset["splitSizes"])
    uwbSplits = createSplits(uwbCoords, dataset["splitSizes"])
    opticalSplits = createSplits(opticalCoords, dataset["splitSizes"])
    modelSplits = createSplits(modelCoords, dataset["splitSizes"])

    titleSuffix = dataset["titleSuffix"]

    # Plotting
    plotSplits(refSplits, uwbSplits, 'UWB Estimated Coordinates', 'green', f'Reference vs Estimated UWB Coordinates - {titleSuffix}', refAlpha=0.3, estAlpha=1)
    plotSplits(refSplits, modelSplits, 'Pixel-to-Real Estimated Coordinates', 'orange', f'Reference vs Estimated Pixel-to-Real Coordinates - {titleSuffix}', refAlpha=0.3, estAlpha=1)
    plotSplits(refSplits, opticalSplits, 'Optical Estimated Coordinates', 'red', f'Reference vs Estimated Optical Coordinates - {titleSuffix}', refAlpha=0.3, estAlpha=1)
