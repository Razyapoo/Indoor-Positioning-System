import os
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

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

def calculateMetrics(refCoords, estCoords):
    maeX = (refCoords['x'] - estCoords['x']).abs().mean()
    maeY = (refCoords['y'] - estCoords['y']).abs().mean()

    mseX = ((refCoords['x'] - estCoords['x']) ** 2).mean()
    mseY = ((refCoords['y'] - estCoords['y']) ** 2).mean()

    rmseX = np.sqrt(mseX)
    rmseY = np.sqrt(mseY)

    return {
        'MAE_X': maeX,
        'MSE_X': mseX,
        'RMSE_X': rmseX,
        'MAE_Y': maeY,
        'MSE_Y': mseY,
        'RMSE_Y': rmseY
    }

def saveMetricsToFile(metricsDf, filePath):
    metricsDf.to_csv(filePath, sep=",")

def saveMetricsToLatex(metricsDf, filePath, comparisonType, exp, tag):
    with open(filePath, 'w') as f:
        f.write("\\begin{table}[h]\n")
        f.write("\\centering\n")
        f.write("\\begin{tabular}{|c|c|c|c|c|}\n")
        f.write("\\hline\n")
        f.write("$Method$ & $Coordinate$ & $MAE$ & $MSE$ & $RMSE$ \\\\\n")
        f.write("\\hline\n")
        for i, row in metricsDf.iterrows():
            if i % 2 == 0:
                if comparisonType == 'Reference':
                    f.write("\\multirow{2}{*}{{\\footnotesize Error(C\\textsubscript{%s}(%s, Tag %d), RS\\textsubscript{0.5m}(Tag %d))}} & " % (row['Method'], exp.split('(')[0][1:], tag, tag))
                else:
                    f.write("\\multirow{2}{*}{{\\footnotesize Error(C\\textsubscript{%s}(%s, Tag %d), C\\textsubscript{UWB}(%s, Tag %d))}} & " % (row['Method'], exp.split('(')[0][1:], tag, exp.split('(')[0][1:], tag))
            else:
                f.write("& ")
            f.write("\\texttt{%s\\textsubscript{%s}} & %.4f & %.4f & %.4f \\\\\n" % (row['Coordinate'], row['Method'], row['MAE'], row['MSE'], row['RMSE']))
            if i % 2 != 0:
                f.write("\\hline\n")
        f.write("\\end{tabular}\n")
        f.write("\\end{table}\n")

def saveSummaryMetricsToLatex(metricsDf, filePath):
    with open(filePath, 'w') as f:
        f.write("\\begin{table}[h]\n")
        f.write("\\centering\n")
        f.write("\\begin{tabular}{|c|c|c|c|c|c|}\n")
        f.write("\\hline\n")
        f.write("$Method$ & $Mean$ & $Median$ & $Max$ & $Min$ & $StdDev$ \\\\\n")
        f.write("\\hline\n")
        for i, row in metricsDf.iterrows():
            f.write("%s & %.4f & %.4f & %.4f & %.4f & %.4f \\\\\n" % (row['Method'], row['Mean Distance'], row['Median Distance'], row['Max Distance'], row['Min Distance'], row['Std Dev Distance']))
            f.write("\\hline\n")
        f.write("\\end{tabular}\n")
        f.write("\\end{table}\n")


def calculateErrors(refCoords, uwbCoords, modelCoords, opticalCoords, compareWith):
    if compareWith == 'ref':
        errorsDf = pd.DataFrame({
            'UWB_x_Error': np.abs(refCoords['x'] - uwbCoords['x']),
            'UWB_y_Error': np.abs(refCoords['y'] - uwbCoords['y']),
            'Pixel-to-Real_x_Error': np.abs(refCoords['x'] - modelCoords['x']),
            'Pixel-to-Real_y_Error': np.abs(refCoords['y'] - modelCoords['y']),
            'Optical_x_Error': np.abs(refCoords['x'] - opticalCoords['x']),
            'Optical_y_Error': np.abs(refCoords['y'] - opticalCoords['y'])
        })
    elif compareWith == 'uwb':
        errorsDf = pd.DataFrame({
            'Pixel-to-Real_x_Error': np.abs(uwbCoords['x'] - modelCoords['x']),
            'Pixel-to-Real_y_Error': np.abs(uwbCoords['y'] - modelCoords['y']),
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
            plt.hist(errorsDf[f'{method}_{axis}_Error'], bins=50, alpha=0.7, label=f'{method}_{axis}', color='green' if method == 'UWB' else ('orange' if method == 'Pixel-to-Real' else 'red'))
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

def plotSplits(referenceSplits, estimatedSplits, estimatedLabel, estColor, title, refAlpha=0.3, estAlpha=1, type="", fileName="", folderToSave=None):
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
        plt.savefig(f'{folderToSave}/reference_vs_estimated_{type}_scatter_plot_{fileName}.png')
    else:
        plt.show()
    plt.close()

def plotSplitsMulti(referenceSplits=None, uwbSplits=None, pixelToRealSplits=None, opticalSplits=None, title=None, refAlpha=0.3, estAlpha=1, fileName="", folderToSave=None):
    """Plot the splits for the reference and estimated coordinates from multiple methods"""
    plt.figure(figsize=(15, 10))
    
    if referenceSplits:
        for i, refSplit in enumerate(referenceSplits):
            plt.plot(refSplit['x'], refSplit['y'], 'o-', color='blue', alpha=refAlpha, label='Reference Coordinates' if i == 0 else "")

    if uwbSplits:
        for i, estSplit in enumerate(uwbSplits):
            plt.plot(estSplit['x'], estSplit['y'], 'x-', color='green', alpha=estAlpha, label='UWB Estimated Coordinates' if i == 0 else "")
    
    if pixelToRealSplits:
        for i, estSplit in enumerate(pixelToRealSplits):
            plt.plot(estSplit['x'], estSplit['y'], 'x-', color='orange', alpha=estAlpha, label='Pixel-to-Real Estimated Coordinates' if i == 0 else "")

    if opticalSplits:
        for i, estSplit in enumerate(opticalSplits):
            plt.plot(estSplit['x'], estSplit['y'], 'x-', color='red', alpha=estAlpha, label='Optical Estimated Coordinates' if i == 0 else "")

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
        plt.savefig(f'{folderToSave}/common_scatter_plot_{fileName}.png')
    else:
        plt.show()
    plt.close()
        
datasets = [
    {
        "refPath": "./s8 data - single person/reference_coordinates.txt",
        "uwbPath": "./s8 data - single person/uwb_to_bb_mapping_entire.txt",
        "opticalPath": "./s8 data - single person/optical_to_bb_mapping_entire.txt",
        "modelPath": "./s8 data - single person/pixel_to_real_to_bb_mapping_entire.txt",
        "useCols": [3, 4],
        "titleSuffix": "E109(DA_S8_S5(T1_A2_TPh_Md_Wpn)) - full area (17 meters)",
        "fileName": "e109_full_area",
        "splitSizes": [28] + [29] * 6,
        "tag": 1
    },
    {
        "refPath": "./s8 data - single person/reference_coordinates_reduced_range.txt",
        "uwbPath": "./s8 data - single person/uwb_to_bb_mapping_entire_reduced_range.txt",
        "opticalPath": "./s8 data - single person/optical_to_bb_mapping_entire_reduced_range.txt",
        "modelPath": "./s8 data - single person/pixel_to_real_to_bb_mapping_entire_reduced_range.txt",
        "useCols": [3, 4],
        "titleSuffix": "E109(DA_S8_S5(T1_A2_TPh_Md_Wpn)) - reduced area (10 meters)",
        "fileName": "e109_reduced_area",
        "splitSizes": [14] + [15] * 6,
        "tag": 1
    },
    {
        "refPath": "./s301 data - single person/reference_coordinates.txt",
        "uwbPath": "./s301 data - single person/export/uwb_to_bb_mapping_entire.txt",
        "opticalPath": "./s301 data - single person/export/optical_to_bb_mapping_entire.txt",
        "modelPath": "./s301 data - single person/export/pixel_to_real_to_bb_mapping_entire.txt",
        "useCols": [3, 4],
        "titleSuffix": "E113(DA_S301_S5(T1_A2_TPh_Md_Wpn))",
        "fileName": "e113",
        "splitSizes": [12] * 6 + [11],
        "tag": 1
    },
    {
        "refPath": "./s8 data - three people/1 person/reference_coordinates.txt",
        "uwbPath": "./s8 data - three people/1 person/uwb_to_bb_mapping.txt",
        "opticalPath": "./s8 data - three people/1 person/optical_to_bb_mapping.txt",
        "modelPath": "./s8 data - three people/1 person/pixel_to_real_to_bb_mapping.txt",
        "useCols": [1, 2],
        "titleSuffix": "E118(DA_S8_S6(T3_A4_TPh_Md_Wp)) - Person 1 - full area (16 meters)",
        "fileName": "e118_full_area_person1",
        "splitSizes": [27],
        "tag": 1
    },
    {
        "refPath": "./s8 data - three people/2 person/reference_coordinates.txt",
        "uwbPath": "./s8 data - three people/2 person/uwb_to_bb_mapping.txt",
        "opticalPath": "./s8 data - three people/2 person/optical_to_bb_mapping.txt",
        "modelPath": "./s8 data - three people/2 person/pixel_to_real_to_bb_mapping.txt",
        "useCols": [1, 2],
        "titleSuffix": "E118(DA_S8_S6(T3_A4_TPh_Md_Wp)) - Person 2 - full area (16 meters)",
        "fileName": "e118_full_area_person2",
        "splitSizes": [27],
        "tag": 2
    },
    {
        "refPath": "./s8 data - three people/3 person/reference_coordinates.txt",
        "uwbPath": "./s8 data - three people/3 person/uwb_to_bb_mapping.txt",
        "opticalPath": "./s8 data - three people/3 person/optical_to_bb_mapping.txt",
        "modelPath": "./s8 data - three people/3 person/pixel_to_real_to_bb_mapping.txt",
        "useCols": [1, 2],
        "titleSuffix": "E118(DA_S8_S6(T3_A4_TPh_Md_Wp)) - Person 3 - full area (16 meters)",
        "fileName": "e118_full_area_person3",
        "splitSizes": [27],
        "tag": 3
    },
    {
        "refPath": "./s8 data - three people/1 person/reference_coordinates_reduced_range.txt",
        "uwbPath": "./s8 data - three people/1 person/uwb_to_bb_mapping_reduced_range.txt",
        "opticalPath": "./s8 data - three people/1 person/optical_to_bb_mapping_reduced_range.txt",
        "modelPath": "./s8 data - three people/1 person/pixel_to_real_to_bb_mapping_reduced_range.txt",
        "useCols": [1, 2],
        "titleSuffix": "E118(DA_S8_S6(T3_A4_TPh_Md_Wp)) - Person 1 - reduced area (10 meters)",
        "fileName": "e118_reduced_area_person1",
        "splitSizes": [15],
        "tag": 1
    },
    {
        "refPath": "./s8 data - three people/2 person/reference_coordinates_reduced_range.txt",
        "uwbPath": "./s8 data - three people/2 person/uwb_to_bb_mapping_reduced_range.txt",
        "opticalPath": "./s8 data - three people/2 person/optical_to_bb_mapping_reduced_range.txt",
        "modelPath": "./s8 data - three people/2 person/pixel_to_real_to_bb_mapping_reduced_range.txt",
        "useCols": [1, 2],
        "titleSuffix": "E118(DA_S8_S6(T3_A4_TPh_Md_Wp)) - Person 2 - reduced area (10 meters)",
        "fileName": "e118_reduced_area_person2",
        "splitSizes": [15],
        "tag": 2
    },
    {
        "refPath": "./s8 data - three people/3 person/reference_coordinates_reduced_range.txt",
        "uwbPath": "./s8 data - three people/3 person/uwb_to_bb_mapping_reduced_range.txt",
        "opticalPath": "./s8 data - three people/3 person/optical_to_bb_mapping_reduced_range.txt",
        "modelPath": "./s8 data - three people/3 person/pixel_to_real_to_bb_mapping_reduced_range.txt",
        "useCols": [1, 2],
        "titleSuffix": "E118(DA_S8_S6(T3_A4_TPh_Md_Wp)) - Person 3 - reduced area (10 meters)",
        "fileName": "e118_reduced_area_person3",
        "splitSizes": [15],
        "tag": 3
    },
    {
        "refPath": "./s301 data - two people/1 person - 1 tag/reference_coordinates.txt",
        "uwbPath": "./s301 data - two people/1 person - 1 tag/uwb_to_bb_mapping.txt",
        "opticalPath": "./s301 data - two people/1 person - 1 tag/optical_to_bb_mapping.txt",
        "modelPath": "./s301 data - two people/1 person - 1 tag/pixel_to_real_to_bb_mapping.txt",
        "useCols": [3, 4],
        "titleSuffix": "E124(DA_S301_S6(T2_A4_TPh_Md_Wp)) - Person 1",
        "fileName": "e124_person1",
        "splitSizes": [9],
        "tag": 1
    },
    {
        "refPath": "./s301 data - two people/2 person - 2 tag/reference_coordinates.txt",
        "uwbPath": "./s301 data - two people/2 person - 2 tag/uwb_to_bb_mapping.txt",
        "opticalPath": "./s301 data - two people/2 person - 2 tag/optical_to_bb_mapping.txt",
        "modelPath": "./s301 data - two people/2 person - 2 tag/pixel_to_real_to_bb_mapping.txt",
        "useCols": [3, 4],
        "titleSuffix": "E124(DA_S301_S6(T2_A4_TPh_Md_Wp)) - Person 2",
        "fileName": "e124_person2",
        "splitSizes": [9],
        "tag": 2
    }
]
# Directory paths
baseFolder = "../../Archive/Images/plots"
baseFolderMetrics = "../../Archive/Metrics"

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
        comparisonFolder = 'Comparison with reference coordinates' if compareWith == 'ref' else 'Comparison with uwb coordinates'
        comparisonSuffixFileName = 'reference_coordinates' if compareWith == 'ref' else 'uwb_coordinates'
        fileName = f'{dataset["fileName"]}_{comparisonSuffixFileName}'
        comparisonSuffix = 'Reference Coordinates' if compareWith == 'ref' else 'UWB Coordinates'
        titleSuffix = f'{dataset["titleSuffix"]} \n Compared with {comparisonSuffix}'

        # folderToSave = createDirectories(baseFolder, comparisonFolder, dataset["fileName"])
        boxplotCoordinatesFolder = createDirectories(baseFolder, comparisonFolder, 'Basic plots showing errors in coordinates', dataset["fileName"])
        distanceErrorFolder = createDirectories(baseFolder, comparisonFolder, 'Distance errors', dataset["fileName"])
        distanceErrorMetricsFolder = createDirectories(baseFolderMetrics, comparisonFolder, 'Distance error metrics', dataset["fileName"])
        errorTrendFolder = createDirectories(baseFolder, comparisonFolder, 'Error trend', dataset["fileName"])
        statisticsFolder = createDirectories(baseFolderMetrics, comparisonFolder, 'Statistics', dataset["fileName"])
        
        errorsDf = calculateErrors(refCoords, uwbCoords, modelCoords, opticalCoords, compareWith)
        plotErrors(errorsDf, titleSuffix, fileName, boxplotCoordinatesFolder)
    
        frames = pd.read_csv(dataset["uwbPath"], sep=' ', header=None, usecols=[0])
        frames.columns = ['Frames']
    
        plotErrorTrend(errorsDf, frames, windowSize=50, titleSuffix=titleSuffix, fileName=fileName, folderToSave=errorTrendFolder)
    
        if compareWith == 'ref':
            distanceErrors = pd.DataFrame({
                'UWB_Distance': np.sqrt((refCoords['x'] - uwbCoords['x'])**2 + (refCoords['y'] - uwbCoords['y'])**2),
                'Pixel_to_Real_Distance': np.sqrt((refCoords['x'] - modelCoords['x'])**2 + (refCoords['y'] - modelCoords['y'])**2),
                'Optical_Distance': np.sqrt((refCoords['x'] - opticalCoords['x'])**2 + (refCoords['y'] - opticalCoords['y'])**2)
            })

            distanceMetrics = {
                'Method': ['UWB', 'Pixel-to-Real', 'Optical'],
                'Mean Distance': [distanceErrors['UWB_Distance'].mean(), distanceErrors['Pixel_to_Real_Distance'].mean(), distanceErrors['Optical_Distance'].mean()],
                'Median Distance': [distanceErrors['UWB_Distance'].median(), distanceErrors['Pixel_to_Real_Distance'].median(), distanceErrors['Optical_Distance'].median()],
                'Max Distance': [distanceErrors['UWB_Distance'].max(), distanceErrors['Pixel_to_Real_Distance'].max(), distanceErrors['Optical_Distance'].max()],
                'Min Distance': [distanceErrors['UWB_Distance'].min(), distanceErrors['Pixel_to_Real_Distance'].min(), distanceErrors['Optical_Distance'].min()],
                'Std Dev Distance': [distanceErrors['UWB_Distance'].std(), distanceErrors['Pixel_to_Real_Distance'].std(), distanceErrors['Optical_Distance'].std()]
            }
        else:
            distanceErrors = pd.DataFrame({
                'Pixel_to_Real_Distance': np.sqrt((uwbCoords['x'] - modelCoords['x'])**2 + (uwbCoords['y'] - modelCoords['y'])**2),
                'Optical_Distance': np.sqrt((uwbCoords['x'] - opticalCoords['x'])**2 + (uwbCoords['y'] - opticalCoords['y'])**2)
            })

            distanceMetrics = {
                'Method': ['Pixel-to-Real', 'Optical'],
                'Mean Distance': [distanceErrors['Pixel_to_Real_Distance'].mean(), distanceErrors['Optical_Distance'].mean()],
                'Median Distance': [distanceErrors['Pixel_to_Real_Distance'].median(), distanceErrors['Optical_Distance'].median()],
                'Max Distance': [distanceErrors['Pixel_to_Real_Distance'].max(), distanceErrors['Optical_Distance'].max()],
                'Min Distance': [distanceErrors['Pixel_to_Real_Distance'].min(), distanceErrors['Optical_Distance'].min()],
                'Std Dev Distance': [distanceErrors['Pixel_to_Real_Distance'].std(), distanceErrors['Optical_Distance'].std()]
            }
        
        distanceMetricsDF = pd.DataFrame(distanceMetrics)
        distanceMetricsDF.to_csv(f'{distanceErrorMetricsFolder}/{fileName}_distance_error_metrics.csv', sep=",")

        plotDistanceErrors(distanceErrors, frames, titleSuffix, fileName, distanceErrorFolder)
        summaryMetricsPath = os.path.join(distanceErrorMetricsFolder, f"{fileName}_distance_error_summary.tex")
        saveSummaryMetricsToLatex(distanceMetricsDF, summaryMetricsPath)

        # Plot splits for scatter plots
        if compareWith == 'ref':
            scatterPlotFolder = createDirectories(baseFolder, comparisonFolder, 'Scatter plots', dataset["fileName"])
            scatterPlotCommonFolder = createDirectories(baseFolder, 'Comparison with reference coordinates', 'Scatter plot common', dataset["fileName"])
            plotSplits(refSplits, uwbSplits, 'Uwb Estimated Coordinates', 'green', f'Reference vs Estimated Uwb Coordinates - {titleSuffix}', refAlpha=0.3, estAlpha=1, fileName=dataset["fileName"], type='uwb', folderToSave=scatterPlotFolder)
            plotSplits(refSplits, modelSplits, 'PixelToReal Estimated Coordinates', 'orange', f'Reference vs Estimated PixelToReal Coordinates - {titleSuffix}', refAlpha=0.3, estAlpha=1, fileName=dataset["fileName"], type='pixel_to_real', folderToSave=scatterPlotFolder)
            plotSplits(refSplits, opticalSplits, 'Optical Estimated Coordinates', 'red', f'Reference vs Estimated Optical Coordinates - {titleSuffix}', refAlpha=0.3, estAlpha=1, fileName=dataset["fileName"], type='optical', folderToSave=scatterPlotFolder)
            plotSplitsMulti(referenceSplits=refSplits, uwbSplits=uwbSplits, pixelToRealSplits=modelSplits, opticalSplits=opticalSplits, title=f'Reference vs Estimated Coordinates - {titleSuffix}', refAlpha=0.3, estAlpha=1, fileName=f'{dataset["fileName"]}_complete', folderToSave=scatterPlotCommonFolder)
            plotSplitsMulti(referenceSplits=refSplits, pixelToRealSplits=modelSplits, opticalSplits=opticalSplits, title=f'Reference vs Estimated Coordinates - {titleSuffix}', refAlpha=0.3, estAlpha=1, fileName=f'{dataset["fileName"]}_pixel_to_real_vs_optical', folderToSave=scatterPlotCommonFolder)
            plotSplitsMulti(referenceSplits=refSplits, uwbSplits=uwbSplits, pixelToRealSplits=modelSplits, title=f'Reference vs Estimated Coordinates - {titleSuffix}', refAlpha=0.3, estAlpha=1, fileName=f'{dataset["fileName"]}_uwb_vs_pixel_to_real', folderToSave=scatterPlotCommonFolder)
        
        # Metrics Calculation
        uwbMetrics = calculateMetrics(refCoords, uwbCoords)
        opticalMetrics = calculateMetrics(refCoords, opticalCoords)
        modelMetrics = calculateMetrics(refCoords, modelCoords)
        opticalVsUwbMetrics = calculateMetrics(uwbCoords, opticalCoords)
        modelVsUwbMetrics = calculateMetrics(uwbCoords, modelCoords)
        
        # Create DataFrame for reference comparison
        refMetricsDf = pd.DataFrame([
            {'Method': 'UWB', 'Coordinate': 'x', 'MAE': uwbMetrics['MAE_X'], 'MSE': uwbMetrics['MSE_X'], 'RMSE': uwbMetrics['RMSE_X']},
            {'Method': 'UWB', 'Coordinate': 'y', 'MAE': uwbMetrics['MAE_Y'], 'MSE': uwbMetrics['MSE_Y'], 'RMSE': uwbMetrics['RMSE_Y']},
            {'Method': 'P2R', 'Coordinate': 'x', 'MAE': modelMetrics['MAE_X'], 'MSE': modelMetrics['MSE_X'], 'RMSE': modelMetrics['RMSE_X']},
            {'Method': 'P2R', 'Coordinate': 'y', 'MAE': modelMetrics['MAE_Y'], 'MSE': modelMetrics['MSE_Y'], 'RMSE': modelMetrics['RMSE_Y']},
            {'Method': 'Opt', 'Coordinate': 'x', 'MAE': opticalMetrics['MAE_X'], 'MSE': opticalMetrics['MSE_X'], 'RMSE': opticalMetrics['RMSE_X']},
            {'Method': 'Opt', 'Coordinate': 'y', 'MAE': opticalMetrics['MAE_Y'], 'MSE': opticalMetrics['MSE_Y'], 'RMSE': opticalMetrics['RMSE_Y']}
        ])
        
        # Create DataFrame for UWB comparison
        uwbMetricsDf = pd.DataFrame([
            {'Method': 'P2R', 'Coordinate': 'x', 'MAE': modelVsUwbMetrics['MAE_X'], 'MSE': modelVsUwbMetrics['MSE_X'], 'RMSE': modelVsUwbMetrics['RMSE_X']},
            {'Method': 'P2R', 'Coordinate': 'y', 'MAE': modelVsUwbMetrics['MAE_Y'], 'MSE': modelVsUwbMetrics['MSE_Y'], 'RMSE': modelVsUwbMetrics['RMSE_Y']},
            {'Method': 'Opt', 'Coordinate': 'x', 'MAE': opticalVsUwbMetrics['MAE_X'], 'MSE': opticalVsUwbMetrics['MSE_X'], 'RMSE': opticalVsUwbMetrics['RMSE_X']},
            {'Method': 'Opt', 'Coordinate': 'y', 'MAE': opticalVsUwbMetrics['MAE_Y'], 'MSE': opticalVsUwbMetrics['MSE_Y'], 'RMSE': opticalVsUwbMetrics['RMSE_Y']}
        ])
        
        # Save LaTeX formatted metrics
        if compareWith == 'ref':
            saveMetricsToLatex(refMetricsDf, os.path.join(statisticsFolder, "statistical_metrics.tex"), 'Reference', dataset['titleSuffix'], dataset['tag'])
            saveMetricsToFile(refMetricsDf, os.path.join(statisticsFolder, "statistical_metrics.csv"))
        else:
            saveMetricsToLatex(uwbMetricsDf, os.path.join(statisticsFolder, "statistical_metrics.tex"), 'UWB', dataset['titleSuffix'], dataset['tag'])
            saveMetricsToFile(uwbMetricsDf, os.path.join(statisticsFolder, "statistical_metrics.csv"))
        
