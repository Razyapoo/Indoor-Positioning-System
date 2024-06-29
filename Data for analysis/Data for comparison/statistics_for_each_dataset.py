import numpy as np
import pandas as pd

def loadData(refPath, uwbPath, opticalPath, modelPath, useCols):
    refCoords = pd.read_csv(refPath, sep=' ', header=None)
    refCoords.columns = ['x', 'y']

    uwbCoords = pd.read_csv(uwbPath, sep=' ', header=None, usecols=useCols)
    uwbCoords.columns = ['x', 'y']

    opticalCoords = pd.read_csv(opticalPath, sep=' ', header=None, usecols=useCols)
    opticalCoords.columns = ['x', 'y']

    modelCoords = pd.read_csv(modelPath, sep=' ', header=None, usecols=useCols)
    modelCoords.columns = ['x', 'y']

    return refCoords, uwbCoords, opticalCoords, modelCoords

def calculateMetrics(refCoords, estCoords):
    maeX = round((refCoords['x'] - estCoords['x']).abs().mean(), 4)
    maeY = round((refCoords['y'] - estCoords['y']).abs().mean(), 4)

    mseX = round(((refCoords['x'] - estCoords['x']) ** 2).mean(), 4)
    mseY = round(((refCoords['y'] - estCoords['y']) ** 2).mean(), 4)

    rmseX = round(np.sqrt(mseX), 4)
    rmseY = round(np.sqrt(mseY), 4)

    return {
        'MAE_X': maeX,
        'MSE_X': mseX,
        'RMSE_X': rmseX,
        'MAE_Y': maeY,
        'MSE_Y': mseY,
        'RMSE_Y': rmseY
    }

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

for dataset in datasets:
    refCoords, uwbCoords, opticalCoords, modelCoords = loadData(
        dataset["refPath"],
        dataset["uwbPath"],
        dataset["opticalPath"],
        dataset["modelPath"],
        dataset["useCols"]
    )
    
    uwbMetrics = calculateMetrics(refCoords, uwbCoords)
    opticalMetrics = calculateMetrics(refCoords, opticalCoords)
    modelMetrics = calculateMetrics(refCoords, modelCoords)
    
    # Print metrics for each dataset
    print(f"Metrics for Dataset: {dataset['titleSuffix']}")
    
    print("\nUWB Metrics:")
    for metric, value in uwbMetrics.items():
        print(f"{metric}: {value}")

    print("\nOptical Metrics:")
    for metric, value in opticalMetrics.items():
        print(f"{metric}: {value}")

    print("\nPixel-to-Real Metrics:")
    for metric, value in modelMetrics.items():
        print(f"{metric}: {value}")
    
    print("\n" + "-"*50 + "\n")