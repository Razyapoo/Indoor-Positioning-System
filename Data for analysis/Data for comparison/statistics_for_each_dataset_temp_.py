import os
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
    metricsDf.to_csv(filePath, sep=" ")

def saveMetricsToLatex(metricsDf, filePath, comparisonType, exp, tag):
    exp_num = ''.join(filter(str.isdigit, exp.split('(')[0]))
    with open(filePath, 'w') as f:
        f.write("\\begin{table}[h]\n")
        f.write("\\centering\n")
        f.write("\\begin{tabular}{|c|c|c|c|c|}\n")
        f.write("\\hline\n")
        f.write("Method & Coordinate & $MAE$ & $MSE$ & $RMSE$ \\\\\n")
        f.write("\\hline\n")
        for i, row in metricsDf.iterrows():
            if i % 2 == 0:
                if comparisonType == 'Reference':
                    f.write("\\multirow{2}{*}{{\\footnotesize Error(C\\textsubscript{%s}(E\\textsubscript{%s}, Tag %d), RS\\textsubscript{0.5m}(Tag %d))}} & " % (row['Method'], exp_num, tag, tag))
                else:
                    f.write("\\multirow{2}{*}{{\\footnotesize Error(C\\textsubscript{%s}(E\\textsubscript{%s}, Tag %d), C\\textsubscript{UWB}(E\\textsubscript{%s}, Tag %d))}} & " % (row['Method'], exp_num, tag, exp_num, tag))
            else:
                f.write("& ")
            f.write("\\texttt{%s\\textsubscript{%s}} & %.4f & %.4f & %.4f \\\\\n" % (row['Coordinate'], row['Method'], row['MAE'], row['MSE'], row['RMSE']))
            if i % 2 != 0:
                f.write("\\hline\n")
        f.write("\\end{tabular}\n")
        f.write("\\end{table}\n")

datasets = [
    {
        "refPath": "./s8 data - single person/reference_coordinates.txt",
        "uwbPath": "./s8 data - single person/uwb_to_bb_mapping_entire.txt",
        "opticalPath": "./s8 data - single person/optical_to_bb_mapping_entire.txt",
        "modelPath": "./s8 data - single person/pixel_to_real_to_bb_mapping_entire.txt",
        "useCols": [3, 4],
        "splitSizes": [28] + [29] * 6,
        "titleSuffix": "E109(DA_S8_S5(T1_A2_TPh_Md_Wpn)) - full area (16 meters)",
        "folderName": "e109_full_area",
        "tag": 1
    },
    {
        "refPath": "./s8 data - single person/reference_coordinates_reduced_range.txt",
        "uwbPath": "./s8 data - single person/uwb_to_bb_mapping_entire_reduced_range.txt",
        "opticalPath": "./s8 data - single person/optical_to_bb_mapping_entire_reduced_range.txt",
        "modelPath": "./s8 data - single person/pixel_to_real_to_bb_mapping_entire_reduced_range.txt",
        "useCols": [3, 4],
        "splitSizes": [14] + [15] * 6,
        "titleSuffix": "E109(DA_S8_S5(T1_A2_TPh_Md_Wpn)) - reduced area (10 meters)",
        "folderName": "e109_reduced_area",
        "tag": 1
    },
    {
        "refPath": "./s301 data - single person/reference_coordinates.txt",
        "uwbPath": "./s301 data - single person/export/uwb_to_bb_mapping_entire.txt",
        "opticalPath": "./s301 data - single person/export/optical_to_bb_mapping_entire.txt",
        "modelPath": "./s301 data - single person/export/pixel_to_real_to_bb_mapping_entire.txt",
        "useCols": [3, 4],
        "splitSizes": [12] * 6 + [11],
        "titleSuffix": "E113(DA_S301_S5(T1_A2_TPh_Md_Wpn))",
        "folderName": "e113",
        "tag": 1
    },
    {
        "refPath": "./s8 data - three people/1 person/reference_coordinates.txt",
        "uwbPath": "./s8 data - three people/1 person/uwb_to_bb_mapping.txt",
        "opticalPath": "./s8 data - three people/1 person/optical_to_bb_mapping.txt",
        "modelPath": "./s8 data - three people/1 person/pixel_to_real_to_bb_mapping.txt",
        "useCols": [1, 2],
        "splitSizes": [27],
        "titleSuffix": "E118(DA_S8_S6(T3_A4_TPh_Md_Wp)) - Person 1 - full area (16 meters)",
        "folderName": "e118_full_area_person1",
        "tag": 1
    },
    {
        "refPath": "./s8 data - three people/2 person/reference_coordinates.txt",
        "uwbPath": "./s8 data - three people/2 person/uwb_to_bb_mapping.txt",
        "opticalPath": "./s8 data - three people/2 person/optical_to_bb_mapping.txt",
        "modelPath": "./s8 data - three people/2 person/pixel_to_real_to_bb_mapping.txt",
        "useCols": [1, 2],
        "splitSizes": [27],
        "titleSuffix": "E118(DA_S8_S6(T3_A4_TPh_Md_Wp)) - Person 2 - full area (16 meters)",
        "folderName": "e118_full_area_person2",
        "tag": 2
    },
    {
        "refPath": "./s8 data - three people/3 person/reference_coordinates.txt",
        "uwbPath": "./s8 data - three people/3 person/uwb_to_bb_mapping.txt",
        "opticalPath": "./s8 data - three people/3 person/optical_to_bb_mapping.txt",
        "modelPath": "./s8 data - three people/3 person/pixel_to_real_to_bb_mapping.txt",
        "useCols": [1, 2],
        "splitSizes": [27],
        "titleSuffix": "E118(DA_S8_S6(T3_A4_TPh_Md_Wp)) - Person 3 - full area (16 meters)",
        "folderName": "e118_full_area_person3",
        "tag": 3
    },
    {
        "refPath": "./s8 data - three people/1 person/reference_coordinates_reduced_range.txt",
        "uwbPath": "./s8 data - three people/1 person/uwb_to_bb_mapping_reduced_range.txt",
        "opticalPath": "./s8 data - three people/1 person/optical_to_bb_mapping_reduced_range.txt",
        "modelPath": "./s8 data - three people/1 person/pixel_to_real_to_bb_mapping_reduced_range.txt",
        "useCols": [1, 2],
        "splitSizes": [15],
        "titleSuffix": "E118(DA_S8_S6(T3_A4_TPh_Md_Wp)) - Person 1 - reduced area (10 meters)",
        "folderName": "e118_reduced_area_person1",
        "tag": 1
    },
    {
        "refPath": "./s8 data - three people/2 person/reference_coordinates_reduced_range.txt",
        "uwbPath": "./s8 data - three people/2 person/uwb_to_bb_mapping_reduced_range.txt",
        "opticalPath": "./s8 data - three people/2 person/optical_to_bb_mapping_reduced_range.txt",
        "modelPath": "./s8 data - three people/2 person/pixel_to_real_to_bb_mapping_reduced_range.txt",
        "useCols": [1, 2],
        "splitSizes": [15],
        "titleSuffix": "E118(DA_S8_S6(T3_A4_TPh_Md_Wp)) - Person 2 - reduced area (10 meters)",
        "folderName": "e118_reduced_area_person2",
        "tag": 2
    },
    {
        "refPath": "./s8 data - three people/3 person/reference_coordinates_reduced_range.txt",
        "uwbPath": "./s8 data - three people/3 person/uwb_to_bb_mapping_reduced_range.txt",
        "opticalPath": "./s8 data - three people/3 person/optical_to_bb_mapping_reduced_range.txt",
        "modelPath": "./s8 data - three people/3 person/pixel_to_real_to_bb_mapping_reduced_range.txt",
        "useCols": [1, 2],
        "splitSizes": [15],
        "titleSuffix": "E118(DA_S8_S6(T3_A4_TPh_Md_Wp)) - Person 3 - reduced area (10 meters)",
        "folderName": "e118_reduced_area_person3",
        "tag": 3
    },
    {
        "refPath": "./s301 data - two people/1 person - 1 tag/reference_coordinates.txt",
        "uwbPath": "./s301 data - two people/1 person - 1 tag/uwb_to_bb_mapping.txt",
        "opticalPath": "./s301 data - two people/1 person - 1 tag/optical_to_bb_mapping.txt",
        "modelPath": "./s301 data - two people/1 person - 1 tag/pixel_to_real_to_bb_mapping.txt",
        "useCols": [3, 4],
        "splitSizes": [9],
        "titleSuffix": "E124(DA_S301_S6(T2_A4_TPh_Md_Wp)) - Person 1",
        "folderName": "e124_person1",
        "tag": 1
    },
    {
        "refPath": "./s301 data - two people/2 person - 2 tag/reference_coordinates.txt",
        "uwbPath": "./s301 data - two people/2 person - 2 tag/uwb_to_bb_mapping.txt",
        "opticalPath": "./s301 data - two people/2 person - 2 tag/optical_to_bb_mapping.txt",
        "modelPath": "./s301 data - two people/2 person - 2 tag/pixel_to_real_to_bb_mapping.txt",
        "useCols": [3, 4],
        "splitSizes": [9],
        "titleSuffix": "E124(DA_S301_S6(T2_A4_TPh_Md_Wp)) - Person 2",
        "folderName": "e124_person2",
        "tag": 2
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
    
    # Directory paths
    refDir = f"../../Archive/Metrics/Comparison with reference coordinates/Statistics/{dataset['folderName']}"
    uwbDir = f"../../Archive/Metrics/Comparison with uwb coordinates/Statistics/{dataset['folderName']}"
    
    # Create directories if they don't exist
    os.makedirs(refDir, exist_ok=True)
    os.makedirs(uwbDir, exist_ok=True)
    
    # Save LaTeX formatted metrics
    saveMetricsToLatex(refMetricsDf, os.path.join(refDir, "statistical_metrics.tex"), 'Reference', dataset['titleSuffix'], dataset['tag'])
    saveMetricsToLatex(uwbMetricsDf, os.path.join(uwbDir, "statistical_metrics.tex"), 'UWB', dataset['titleSuffix'], dataset['tag'])
    
    # Save CSV formatted metrics
    saveMetricsToFile(refMetricsDf, os.path.join(refDir, "statistical_metrics.csv"))
    saveMetricsToFile(uwbMetricsDf, os.path.join(uwbDir, "statistical_metrics.csv"))
