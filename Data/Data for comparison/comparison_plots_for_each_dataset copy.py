import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os

def createDirectories(baseFolder, comparisonFolder, diagramType, experimentName):
    """Create directories for saving plots."""
    folderToSave = os.path.join(baseFolder, comparisonFolder, diagramType, experimentName)
    os.makedirs(folderToSave, exist_ok=True)
    return folderToSave

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

def calculateErrors(refCoords, uwbCoords, modelCoords, opticalCoords, compareWith):
    if compareWith == 'ref':
        errorsDf = pd.DataFrame({
            'UWB_x_Error': np.abs(refCoords['x'] - uwbCoords['x']),
            'UWB_y_Error': np.abs(refCoords['y'] - uwbCoords['y']),
            'Pixel_to_Real_x_Error': np.abs(refCoords['x'] - modelCoords['x']),
            'Pixel_to_Real_y_Error': np.abs(refCoords['y'] - modelCoords['y']),
            'Optical_x_Error': np.abs(refCoords['x'] - opticalCoords['x']),
            'Optical_y_Error': np.abs(refCoords['y'] - opticalCoords['y'])
        })
    elif compareWith == 'uwb':
        errorsDf = pd.DataFrame({
            'Pixel_to_Real_x_Error': np.abs(uwbCoords['x'] - modelCoords['x']),
            'Pixel_to_Real_y_Error': np.abs(uwbCoords['y'] - modelCoords['y']),
            'Optical_x_Error': np.abs(uwbCoords['x'] - opticalCoords['x']),
            'Optical_y_Error': np.abs(uwbCoords['y'] - opticalCoords['y'])
        })
    return errorsDf

def plotErrors(errorsDf, titleSuffix, fileName, folderToSave):
    errorsMeltedDf = errorsDf.melt(var_name='Method_Error', value_name='Error')
    errorsMeltedDf['Method'] = errorsMeltedDf['Method_Error'].apply(lambda x: x.split('_')[0])
    errorsMeltedDf['Axis'] = errorsMeltedDf['Method_Error'].apply(lambda x: x.split('_')[1])

    plt.figure(figsize=(12, 6))
    sns.boxplot(data=errorsMeltedDf, x='Method', y='Error', hue='Axis')
    plt.title(f'Boxplot of Errors - {titleSuffix}', fontsize=18)
    plt.ylabel('Absolute Error', fontsize=13)
    plt.xlabel('Methods', fontsize=12)
    plt.legend(title='Axis', fontsize=13)
    plt.xticks(fontsize=13)
    plt.yticks(fontsize=12)
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(f'{folderToSave}/boxplot_errors_{fileName}.png')
    plt.close()

    plt.figure(figsize=(14, 10))
    plt.suptitle(f'Histogram of Errors in Coordinates - {titleSuffix}', fontsize=18)

    methods = errorsDf.columns.str.split('_').str[0].unique()
    for i, method in enumerate(methods, 1):
        for axis in ['x', 'y']:
            plt.subplot(len(methods), 2, i*2-1 if axis == 'x' else i*2)
            plt.hist(errorsDf[f'{method}_{axis}_Error'], bins=50, alpha=0.7, label=f'{method}_{axis}', color='green' if method == 'UWB' else ('orange' if method == 'Pixel_to_Real' else 'red'))
            plt.title(f'{method} {axis.upper()} Coordinate Errors', fontsize=14)
            plt.xlabel('Absolute Error', fontsize=13)
            plt.ylabel('Frequency', fontsize=13)
            plt.xticks(fontsize=12)
            plt.yticks(fontsize=12)
            plt.grid(True)

    plt.tight_layout()
    plt.savefig(f'{folderToSave}/histogram_errors_{fileName}.png')
    plt.close()

def plotErrorTrend(errorsDf, frames, windowSize, titleSuffix, fileName, folderToSave):
    methods = errorsDf.columns.str.split('_').str[0].unique()
    for method in methods:
        errorsDf[f'{method}_x_Error_MA'] = errorsDf[f'{method}_x_Error'].rolling(window=windowSize).mean()
        errorsDf[f'{method}_y_Error_MA'] = errorsDf[f'{method}_y_Error'].rolling(window=windowSize).mean()

    errorsDf['Frames'] = frames[['Frames']].copy()

    plt.figure(figsize=(14, 10))
    plt.suptitle(f'Error Trend Over Time - {titleSuffix}', fontsize=18)

    for i, method in enumerate(methods, 1):
        for axis in ['x', 'y']:
            plt.subplot(len(methods), 2, i*2-1 if axis == 'x' else i*2)
            plt.plot(errorsDf['Frames'], errorsDf[f'{method}_{axis}_Error'], label=f'{method}_{axis}_Error', alpha=0.7)
            plt.plot(errorsDf['Frames'], errorsDf[f'{method}_{axis}_Error_MA'], label=f'{method}_{axis}_Error_MA', color='red', linewidth=2)
            plt.xlabel('Frame', fontsize=13)
            plt.ylabel('Absolute Error', fontsize=13)
            plt.title(f'{method} {axis.upper()} Coordinate', fontsize=14)
            plt.xticks(fontsize=12)
            plt.yticks(fontsize=12)
            plt.legend()
            plt.grid(True)

    plt.tight_layout()
    plt.savefig(f'{folderToSave}/error_trend_{fileName}.png')
    plt.close()

def plotDistanceErrors(distanceErrors, frames, titleSuffix, fileName, folderToSave):
    plt.figure(figsize=(14, 6))
    for method, color in zip(distanceErrors.columns, ['green', 'orange', 'red']):
        plt.scatter(frames['Frames'], distanceErrors[method], alpha=0.5, label=method, color=color)
    plt.title(f'Scatter Plot of Distance Errors - {titleSuffix}', fontsize=16)
    plt.xlabel('Frames', fontsize=14)
    plt.ylabel('Distance Errors', fontsize=14)
    plt.xticks(fontsize=12)
    plt.yticks(fontsize=12)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(f'{folderToSave}/scatter_distance_errors_{fileName}.png')
    plt.close()

    plt.figure(figsize=(14, 10))
    plt.suptitle(f'Histogram Plot of Distance Errors - {titleSuffix}', fontsize=16)

    for i, method in enumerate(distanceErrors.columns, 1):
        plt.subplot(3, 1, i)
        plt.hist(distanceErrors[method], bins=50, alpha=0.7, label=method, color='green' if 'UWB' in method else ('orange' if 'Pixel' in method else 'red'))
        plt.title(f'Distance Errors between {method.split("_")[0]} and Ground Truth Coordinates', fontsize=14)
        plt.xlabel('Distance Errors', fontsize=14)
        plt.ylabel('Frequency', fontsize=14)
        plt.xticks(fontsize=12)
        plt.yticks(fontsize=12)
        plt.grid(True)

    plt.tight_layout()
    plt.savefig(f'{folderToSave}/histogram_distance_errors_{fileName}.png')
    plt.close()

def createSplits(data, splitSizes):
    """Split data into parts based on the splitSizes"""
    indices = np.cumsum(splitSizes)
    return np.split(data, indices[:-1])

def plotSplits(referenceSplits, estimatedSplits, estimatedLabel, estColor, title, refAlpha=0.3, estAlpha=1, folderToSave=None):
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
    plt.tight_layout()
    if folderToSave:
        fileNameToSave = folderToSave + "/" + title.replace(" ", "_").replace("\n", "_")
        plt.savefig(fileNameToSave)
    else:
        plt.show()
    plt.close()

datasets = [
    {
        "refPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - single person/reference_coordinates.txt",
        "uwbPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - single person/uwb_to_bb_mapping_entire.txt",
        "opticalPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - single person/optical_to_bb_mapping_entire.txt",
        "modelPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - single person/pixel_to_real_to_bb_mapping_entire.txt",
        "useCols": [3, 4],
        "titleSuffix": "E109(DA_S8_S5(T1_A2_TPh_Md_Wpn)) - full area (16 meters)",
        "fileName": "e109_full_area",
        "splitSizes": [28] + [29] * 6
    },
    {
        "refPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - single person/reference_coordinates_reduced_range.txt",
        "uwbPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - single person/uwb_to_bb_mapping_entire_reduced_range.txt",
        "opticalPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - single person/optical_to_bb_mapping_entire_reduced_range.txt",
        "modelPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - single person/pixel_to_real_to_bb_mapping_entire_reduced_range.txt",
        "useCols": [3, 4],
        "titleSuffix": "E109(DA_S8_S5(T1_A2_TPh_Md_Wpn)) - reduced area (10 meters)",
        "fileName": "e109_reduced_area",
        "splitSizes": [14] + [15] * 6
    },
    {
        "refPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s301 data - single person/reference_coordinates.txt",
        "uwbPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s301 data - single person/export/uwb_to_bb_mapping_entire.txt",
        "opticalPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s301 data - single person/export/optical_to_bb_mapping_entire.txt",
        "modelPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s301 data - single person/export/pixel_to_real_to_bb_mapping_entire.txt",
        "useCols": [3, 4],
        "titleSuffix": "E113(DA_S301_S5(T1_A2_TPh_Md_Wpn))",
        "fileName": "e113",
        "splitSizes": [12] * 6 + [11]
    },
    {
        "refPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/1 person/reference_coordinates.txt",
        "uwbPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/1 person/uwb_to_bb_mapping.txt",
        "opticalPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/1 person/optical_to_bb_mapping.txt",
        "modelPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/1 person/pixel_to_real_to_bb_mapping.txt",
        "useCols": [1, 2],
        "titleSuffix": "E118(DA_S8_S6(T3_A4_TPh_Md_Wp)) - Person 1 - full area (16 meters)",
        "fileName": "e118_full_area_person1",
        "splitSizes": [27]
    },
    {
        "refPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/2 person/reference_coordinates.txt",
        "uwbPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/2 person/uwb_to_bb_mapping.txt",
        "opticalPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/2 person/optical_to_bb_mapping.txt",
        "modelPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/2 person/pixel_to_real_to_bb_mapping.txt",
        "useCols": [1, 2],
        "titleSuffix": "E118(DA_S8_S6(T3_A4_TPh_Md_Wp)) - Person 2 - full area (16 meters)",
        "fileName": "e118_full_area_person2",
        "splitSizes": [27]
    },
    {
        "refPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/3 person/reference_coordinates.txt",
        "uwbPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/3 person/uwb_to_bb_mapping.txt",
        "opticalPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/3 person/optical_to_bb_mapping.txt",
        "modelPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/3 person/pixel_to_real_to_bb_mapping.txt",
        "useCols": [1, 2],
        "titleSuffix": "E118(DA_S8_S6(T3_A4_TPh_Md_Wp)) - Person 3 - full area (16 meters)",
        "fileName": "e118_full_area_person3",
        "splitSizes": [27]
    },
    {
        "refPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/1 person/reference_coordinates_reduced_range.txt",
        "uwbPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/1 person/uwb_to_bb_mapping_reduced_range.txt",
        "opticalPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/1 person/optical_to_bb_mapping_reduced_range.txt",
        "modelPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/1 person/pixel_to_real_to_bb_mapping_reduced_range.txt",
        "useCols": [1, 2],
        "titleSuffix": "E118(DA_S8_S6(T3_A4_TPh_Md_Wp)) - Person 1 - reduced area (10 meters)",
        "fileName": "e118_reduced_area_person1",
        "splitSizes": [15]
    },
    {
        "refPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/2 person/reference_coordinates_reduced_range.txt",
        "uwbPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/2 person/uwb_to_bb_mapping_reduced_range.txt",
        "opticalPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/2 person/optical_to_bb_mapping_reduced_range.txt",
        "modelPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/2 person/pixel_to_real_to_bb_mapping_reduced_range.txt",
        "useCols": [1, 2],
        "titleSuffix": "E118(DA_S8_S6(T3_A4_TPh_Md_Wp)) - Person 2 - reduced area (10 meters)",
        "fileName": "e118_reduced_area_person2",
        "splitSizes": [15]
    },
    {
        "refPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/3 person/reference_coordinates_reduced_range.txt",
        "uwbPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/3 person/uwb_to_bb_mapping_reduced_range.txt",
        "opticalPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/3 person/optical_to_bb_mapping_reduced_range.txt",
        "modelPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s8 data - three people/3 person/pixel_to_real_to_bb_mapping_reduced_range.txt",
        "useCols": [1, 2],
        "titleSuffix": "E118(DA_S8_S6(T3_A4_TPh_Md_Wp)) - Person 3 - reduced area (10 meters)",
        "fileName": "e118_reduced_area_person3",
        "splitSizes": [15]
    },
    {
        "refPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s301 data - two people/1 person - 1 tag/reference_coordinates.txt",
        "uwbPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s301 data - two people/1 person - 1 tag/uwb_to_bb_mapping.txt",
        "opticalPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s301 data - two people/1 person - 1 tag/optical_to_bb_mapping.txt",
        "modelPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s301 data - two people/1 person - 1 tag/pixel_to_real_to_bb_mapping.txt",
        "useCols": [3, 4],
        "titleSuffix": "E124(DA_S301_S6(T2_A4_TPh_Md_Wp)) - Person 1",
        "fileName": "e124_person1",
        "splitSizes": [9]
    },
    {
        "refPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s301 data - two people/2 person - 2 tag/reference_coordinates.txt",
        "uwbPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s301 data - two people/2 person - 2 tag/uwb_to_bb_mapping.txt",
        "opticalPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s301 data - two people/2 person - 2 tag/optical_to_bb_mapping.txt",
        "modelPath": "/home/oskar/Documents/Master Thesis/Data/Data for comparison/s301 data - two people/2 person - 2 tag/pixel_to_real_to_bb_mapping.txt",
        "useCols": [3, 4],
        "titleSuffix": "E124(DA_S301_S6(T2_A4_TPh_Md_Wp)) - Person 2",
        "fileName": "e124_person2",
        "splitSizes": [9]
    }
]

baseFolder = "/home/oskar/Documents/Images from inkscape/plots"

for dataset in datasets:
    refCoords, uwbCoords, opticalCoords, modelCoords = loadData(
        dataset["refPath"],
        dataset["uwbPath"],
        dataset["opticalPath"],
        dataset["modelPath"],
        dataset["useCols"]
    )

    refSplits = createSplits(refCoords, dataset["splitSizes"])
    uwbSplits = createSplits(uwbCoords, dataset["splitSizes"])
    opticalSplits = createSplits(opticalCoords, dataset["splitSizes"])
    modelSplits = createSplits(modelCoords, dataset["splitSizes"])

    for compareWith in ['ref', 'uwb']:
        comparisonFolder = 'comparison_with_reference_coordinates' if compareWith == 'ref' else 'comparison_with_uwb_coordinates'
        comparisonSuffixFileName = 'reference_coordinates' if compareWith == 'ref' else 'uwb_coordinates'
        fileName = f'{dataset["fileName"]}_{comparisonSuffixFileName}'
        comparisonSuffix = 'Reference Coordinates' if compareWith == 'ref' else 'UWB Coordinates'
        titleSuffix = f'{dataset["titleSuffix"]} \n Compared with {comparisonSuffix}'

        # Creating folders for different plot types within the comparison folders
        boxplotFolder = createDirectories(baseFolder, comparisonFolder, 'boxplot_errors', dataset["fileName"])
        histogramFolder = createDirectories(baseFolder, comparisonFolder, 'histogram_errors', dataset["fileName"])
        errorTrendFolder = createDirectories(baseFolder, comparisonFolder, 'error_trend', dataset["fileName"])
        scatterPlotFolder = createDirectories(baseFolder, comparisonFolder, 'scatter_plots', dataset["fileName"])

        errorsDf = calculateErrors(refCoords, uwbCoords, modelCoords, opticalCoords, compareWith)
        
        # Save plots to respective folders
        plotErrors(errorsDf, titleSuffix, fileName, boxplotFolder)
    
        frames = pd.read_csv(dataset["uwbPath"], sep=' ', header=None, usecols=[0])
        frames.columns = ['Frames']
    
        plotErrorTrend(errorsDf, frames, windowSize=50, titleSuffix=titleSuffix, fileName=fileName, folderToSave=errorTrendFolder)
    
        if compareWith == 'ref':
            distanceErrors = pd.DataFrame({
                'UWB_Distance': np.sqrt((refCoords['x'] - uwbCoords['x'])**2 + (refCoords['y'] - uwbCoords['y'])**2),
                'Pixel_to_Real_Distance': np.sqrt((refCoords['x'] - modelCoords['x'])**2 + (refCoords['y'] - modelCoords['y'])**2),
                'Optical_Distance': np.sqrt((refCoords['x'] - opticalCoords['x'])**2 + (refCoords['y'] - opticalCoords['y'])**2)
            })
        else:
            distanceErrors = pd.DataFrame({
                'Pixel_to_Real_Distance': np.sqrt((uwbCoords['x'] - modelCoords['x'])**2 + (uwbCoords['y'] - modelCoords['y'])**2),
                'Optical_Distance': np.sqrt((uwbCoords['x'] - opticalCoords['x'])**2 + (uwbCoords['y'] - opticalCoords['y'])**2)
            })

        plotDistanceErrors(distanceErrors, frames, titleSuffix, fileName, histogramFolder)

        # Plot splits for scatter plots
        plotSplits(refSplits, uwbSplits, 'Uwb Estimated Coordinates', 'green', f'Reference vs Estimated Uwb Coordinates - {titleSuffix}', refAlpha=0.3, estAlpha=1, folderToSave=scatterPlotFolder)
        plotSplits(refSplits, modelSplits, 'PixelToReal Estimated Coordinates', 'orange', f'Reference vs Estimated PixelToReal Coordinates - {titleSuffix}', refAlpha=0.3, estAlpha=1, folderToSave=scatterPlotFolder)
        plotSplits(refSplits, opticalSplits, 'Optical Estimated Coordinates', 'red', f'Reference vs Estimated Optical Coordinates - {titleSuffix}', refAlpha=0.3, estAlpha=1, folderToSave=scatterPlotFolder)
