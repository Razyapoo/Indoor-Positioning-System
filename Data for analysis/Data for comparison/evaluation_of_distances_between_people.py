import os
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from itertools import combinations

def loadParticipantData(basePath, participantFiles, useCols):
    coords = {}
    for tag, filePath in participantFiles.items():
        fileFullPath = os.path.join(basePath, filePath)
        try:
            if 'ground_truth' in filePath:
                data = pd.read_csv(fileFullPath, sep=' ', header=None)
            else:
                data = pd.read_csv(fileFullPath, sep=' ', header=None, usecols=useCols)
            data.columns = ['x', 'y']
            coords[tag] = data
        except pd.errors.ParserError as e:
            print(f"Error reading {fileFullPath}: {e}")
        except ValueError as e:
            print(f"Error with usecols for {fileFullPath}: {e}")
    return coords

def computeDistances(coords):
    distances = {}
    numRows = len(next(iter(coords.values())))
    for i in range(numRows):
        rowDistances = {}
        for (tag1, data1), (tag2, data2) in combinations(coords.items(), 2):
            dist = np.sqrt((data1.iloc[i]['x'] - data2.iloc[i]['x'])**2 + (data1.iloc[i]['y'] - data2.iloc[i]['y'])**2)
            rowDistances[(tag1, tag2)] = dist
        distances[i] = rowDistances
    return distances

def compareDistances(gtDistances, estDistances):
    errors = {}
    for i, gtRowDistances in gtDistances.items():
        if i in estDistances:
            estRowDistances = estDistances[i]
            rowErrors = {}
            for pair in gtRowDistances:
                if pair in estRowDistances:
                    rowErrors[pair] = estRowDistances[pair] - gtRowDistances[pair]
            errors[i] = rowErrors
    return errors

def performStatistics(errors):
    statsByPair = {}
    for rowErrors in errors.values():
        for pair, error in rowErrors.items():
            if pair not in statsByPair:
                statsByPair[pair] = []
            statsByPair[pair].append(error)
    
    statsSummary = {}
    for pair, errors in statsByPair.items():
        errorsDf = pd.DataFrame(errors, columns=['Error'])
        statistics = {
            'MAE': abs(errorsDf['Error']).mean(),
            'Mean': errorsDf['Error'].mean(),
            'Median': errorsDf['Error'].median(),
            'Max': abs(errorsDf['Error']).max(),
            'Min': abs(errorsDf['Error']).min()
        }
        statsSummary[pair] = statistics
    
    return statsByPair, statsSummary

def plotStatistics(errorsByPair, statsSummary, method, titleSuffix, folderToSave):
    combinedErrors = pd.DataFrame()

    for pair, errors in errorsByPair.items():
        errorsDf = pd.DataFrame(errors, columns=['Error'])
        
        pairStr = f"{pair[0]}_{pair[1]}"
        
        plt.figure(figsize=(12, 6))
        sns.boxplot(data=errorsDf, y='Error')
        plt.title(f'Boxplot of Errors in Distance Between Pair {pairStr}. {method} method \n {titleSuffix}', fontsize=18)
        plt.ylabel('Distance Error', fontsize=13)
        plt.xlabel('Pairs of Participants', fontsize=12)
        plt.grid(True)
        plt.tight_layout()
        plt.savefig(f'{folderToSave}/boxplot_distance_errors_{method}_pair_{pairStr}.png')
        plt.close()

        plt.figure(figsize=(14, 10))
        plt.suptitle(f'Histogram of Errors in Distance Between Pair {pairStr} \n {method} method {titleSuffix}', fontsize=18)
        plt.hist(errorsDf['Error'], bins=50, alpha=0.7)
        plt.xlabel('Distance Error', fontsize=13)
        plt.ylabel('Frequency', fontsize=13)
        plt.grid(True)
        plt.tight_layout()
        plt.savefig(f'{folderToSave}/histogram_distance_errors_{method}_pair_{pairStr}.png')
        plt.close()

        errorsDf["Pair"] = pairStr
        combinedErrors = pd.concat([combinedErrors, errorsDf])

    # Plot combined boxplot
    plt.figure(figsize=(12, 6))
    sns.boxplot(data=combinedErrors, x='Pair', y='Error')
    plt.title(f'Combined Boxplot of Errors in Distance for All Pairs. {method} method \n {titleSuffix}', fontsize=18)
    plt.ylabel('Distance Error', fontsize=13)
    plt.xlabel('Pairs of Participants', fontsize=12)
    plt.xticks(fontsize=10)
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(f'{folderToSave}/boxplot_distance_errors_{method}_combined_pairs.png')
    plt.close()

    # Plot combined histogram
    plt.figure(figsize=(14, 10))
    sns.histplot(data=combinedErrors, x='Error', hue='Pair', bins=50, multiple='stack', alpha=0.7)
    plt.title(f'Combined Histogram of Errors in Distance for All Pairs \n {method} method {titleSuffix}', fontsize=18)
    plt.xlabel('Distance Error', fontsize=13)
    plt.ylabel('Frequency', fontsize=13)
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(f'{folderToSave}/histogram_distance_errors_{method}_combined_pairs.png')
    plt.close()

def aggregateErrorsForPlotting(errorsByPair, method, experiment):
    aggregatedErrors = []
    for pair, errors in errorsByPair.items():
        errorsDf = pd.DataFrame(errors, columns=['Error'])
        errorsDf['Method'] = method
        errorsDf['Pair'] = f"{pair[0]}_{pair[1]}"
        errorsDf['Experiment'] = experiment
        aggregatedErrors.append(errorsDf)
    return pd.concat(aggregatedErrors, ignore_index=True)

def plotCombinedBoxplots(aggregatedErrors, title, titleSuffix, folderToSave):
    plt.figure(figsize=(14, 8))
    sns.boxplot(x='Pair', y='Error', hue='Method', data=aggregatedErrors)
    plt.title(title + "\n" + titleSuffix, fontsize=18)
    plt.ylabel('Distance Error', fontsize=13)
    plt.xlabel('Pairs of Participants', fontsize=12)
    plt.legend(title='Method')
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(f'{folderToSave}/combined_boxplot_distance_errors.png')
    plt.close()

def plotTrend(errorsByPair, frames, method, titleSuffix, folderToSave):
    for pair, errors in errorsByPair.items():
        errorsDf = pd.DataFrame(errors, columns=['Error'])
        
        pairStr = f"{pair[0]}_{pair[1]}"
        
        plt.figure(figsize=(14, 6))
        plt.plot(frames, errorsDf['Error'])
        plt.title(f'Error Trend Over Time in Distance Between Pair {pairStr}. {method} method \n {titleSuffix}', fontsize=18)
        plt.xlabel('Frame', fontsize=13)
        plt.ylabel('Distance Error', fontsize=13)
        plt.grid(True)
        plt.tight_layout()
        plt.savefig(f'{folderToSave}/trend_distance_errors_{method}_pair_{pairStr}.png')
        plt.close()

def saveMetricsToLatex(statsSummary, type, titleSuffix, filePath):
    with open(filePath, 'w') as f:
        f.write("\\begin{table}[h]\n")
        f.write("\\centering\n")
        f.write("{\\small\n")
        f.write("\\begin{tabular}{|c|c|c|c|c|}\n")
        f.write("\\hline\n")
        f.write("$Method$ & $MAE$ & $Median$ & $Min$ & $Max$ \\\\\n")
        f.write("\\hline\n")
        for idx in statsSummary.index:
            # pairStr = f"{idx[1]}_{idx[2]}"
            method = 'UWB' if (idx[0] == 'UWB') else 'P2R' if (idx[0] == 'Pixel-to-Real') else 'Opt' 
            if type == 'gt':
                f.write("{\\scriptsize Error(DPeople\\textsubscript{%s}(Tag %d, Tag %d), DPeople\\textsubscript{RS}(Tag %d, Tag %d))} & " % (method, int(idx[1]), int(idx[2]), int(idx[1]), int(idx[2])))
            else:
                f.write("{\\scriptsize Error(DPeople\\textsubscript{%s}(Tag %d, Tag %d), DPeople\\textsubscript{UWB}(Tag %d, Tag %d))} & " % (method, int(idx[1]), int(idx[2]), int(idx[1]), int(idx[2])))
            stats = statsSummary.loc[idx]
            f.write(f"{stats['MAE']:.3f} & {stats['Median']:.3f} & {stats['Min']:.3f} & {stats['Max']:.3f} \\\\\n")
            f.write("\\hline\n")
        f.write("\\end{tabular}\n")
        f.write("}\n")
        f.write("\\end{table}\n")

def processExperiments(datasets, baseFolder, baseFolderMetrics):
    for dataset in datasets:
        aggregatedErrors = []
        basePath = dataset["basePath"]
        participantFiles = {
            "ground_truth": dataset["gtFiles"],
            "uwb": dataset["uwbFiles"],
            "optical": dataset["opticalFiles"],
            "model": dataset["modelFiles"]
        }

        gtCoords = loadParticipantData(basePath, participantFiles["ground_truth"], dataset["useCols"])
        uwbCoords = loadParticipantData(basePath, participantFiles["uwb"], dataset["useCols"])
        opticalCoords = loadParticipantData(basePath, participantFiles["optical"], dataset["useCols"])
        modelCoords = loadParticipantData(basePath, participantFiles["model"], dataset["useCols"])

        if len(gtCoords) < 2:
            continue  # Skip experiments with less than 2 participants

        # Compute distances
        gtDistances = computeDistances(gtCoords)
        uwbDistances = computeDistances(uwbCoords)
        opticalDistances = computeDistances(opticalCoords)
        modelDistances = computeDistances(modelCoords)

        for compareWith in ['gt', 'uwb']:
            comparisonFolder = 'Comparison with ground truth coordinates' if compareWith == 'gt' else 'Comparison with uwb coordinates'
            comparisonSuffixFileName = 'ground_truth_coordinates' if compareWith == 'gt' else 'uwb_coordinates'
            fileName = f'{dataset["fileName"]}_{comparisonSuffixFileName}'
            comparisonSuffix = 'Ground Truth Coordinates' if compareWith == 'gt' else 'UWB Coordinates'
            titleSuffix = f'{dataset["titleSuffix"]} \n Compared with {comparisonSuffix}'

            # Compare distances
            if compareWith == 'gt':
                uwbErrors = compareDistances(gtDistances, uwbDistances)
                opticalErrors = compareDistances(gtDistances, opticalDistances)
                modelErrors = compareDistances(gtDistances, modelDistances)
            else:
                opticalErrors = compareDistances(uwbDistances, opticalDistances)
                modelErrors = compareDistances(uwbDistances, modelDistances)

            # Perform statistics
            if compareWith == 'gt':
                uwbErrorsByPair, uwbStats = performStatistics(uwbErrors)
                opticalErrorsByPair, opticalStats = performStatistics(opticalErrors)
                modelErrorsByPair, modelStats = performStatistics(modelErrors)
            else:
                opticalErrorsByPair, opticalVsUwbStats = performStatistics(opticalErrors)
                modelErrorsByPair, modelVsUwbStats = performStatistics(modelErrors)

            # Create directories for saving plots and metrics
            plotFolderToSave = os.path.join(baseFolder, comparisonFolder, 'Basic plots showing errors in distance between people (BoxPlots and Histograms)', dataset["fileName"])
            trendFolderToSave = os.path.join(baseFolder, comparisonFolder, 'Distance between people error trend over time', dataset["fileName"])
            metricsFolderToSave = os.path.join(baseFolderMetrics, comparisonFolder, 'Distance between people error statistics', dataset["fileName"])

            os.makedirs(plotFolderToSave, exist_ok=True)
            os.makedirs(trendFolderToSave, exist_ok=True)
            os.makedirs(metricsFolderToSave, exist_ok=True)

            # Save statistics to CSV
            for pair, errors in opticalErrorsByPair.items():
                pairStr = f"{pair[0]}_{pair[1]}"
                errorsDf = pd.DataFrame(errors, columns=['Error'])
                errorsDf.to_csv(os.path.join(metricsFolderToSave, f"optical_distance_errors_pair_{pairStr}.csv"), index=False)
            
            for pair, errors in modelErrorsByPair.items():
                pairStr = f"{pair[0]}_{pair[1]}"
                errorsDf = pd.DataFrame(errors, columns=['Error'])
                errorsDf.to_csv(os.path.join(metricsFolderToSave, f"model_distance_errors_pair_{pairStr}.csv"), index=False)

            if compareWith == 'gt':
                for pair, errors in uwbErrorsByPair.items():
                    pairStr = f"{pair[0]}_{pair[1]}"
                    errorsDf = pd.DataFrame(errors, columns=['Error'])
                    errorsDf.to_csv(os.path.join(metricsFolderToSave, f"uwb_distance_errors_pair_{pairStr}.csv"), index=False)

            # Save statistics summary
            if compareWith == 'gt':
                statsSummary = pd.concat([pd.DataFrame(uwbStats).T, pd.DataFrame(modelStats).T, pd.DataFrame(opticalStats).T], keys=['UWB', 'Pixel-to-Real', 'Optical'])
            else:
                statsSummary = pd.concat([pd.DataFrame(modelVsUwbStats).T, pd.DataFrame(opticalVsUwbStats).T], keys=['Pixel-to-Real', 'Optical'])
            statsSummary.to_csv(os.path.join(metricsFolderToSave, f"distance_error_stats_summary_{comparisonSuffixFileName}.csv"))

            # Export statistics to LaTeX
            saveMetricsToLatex(statsSummary, 'gt' if compareWith == 'gt' else 'uwb', titleSuffix, os.path.join(metricsFolderToSave, f"distance_error_stats_summary_{comparisonSuffixFileName}.tex"))

            # Plot statistics
            if compareWith == 'gt':
                plotStatistics(uwbErrorsByPair, uwbStats, 'UWB', titleSuffix, plotFolderToSave)
            plotStatistics(opticalErrorsByPair, opticalVsUwbStats if compareWith == 'uwb' else opticalStats, 'Optical', titleSuffix, plotFolderToSave)
            plotStatistics(modelErrorsByPair, modelVsUwbStats if compareWith == 'uwb' else modelStats, 'Pixel-to-Real', titleSuffix, plotFolderToSave)

            if compareWith == 'gt':
                aggregatedErrors.append(aggregateErrorsForPlotting(uwbErrorsByPair, 'UWB', dataset["titleSuffix"]))
            aggregatedErrors.append(aggregateErrorsForPlotting(opticalErrorsByPair, 'Optical', dataset["titleSuffix"]))
            aggregatedErrors.append(aggregateErrorsForPlotting(modelErrorsByPair, 'Pixel-to-Real', dataset["titleSuffix"]))

            # Plot trend over time
            frames = pd.read_csv(os.path.join(basePath, participantFiles["uwb"]["1"]), sep=' ', header=None, usecols=[0])
            frames.columns = ['Frames']
            if compareWith == 'gt':
                plotTrend(uwbErrorsByPair, frames, 'UWB', titleSuffix, trendFolderToSave)
            plotTrend(opticalErrorsByPair, frames, 'Optical', titleSuffix, trendFolderToSave)
            plotTrend(modelErrorsByPair, frames, 'Pixel-to-Real', titleSuffix, trendFolderToSave)

             # Combine all errors into a single DataFrame and plot combined boxplots
            combinedErrorsDf = pd.concat(aggregatedErrors, ignore_index=True)
            plotCombinedBoxplots(combinedErrorsDf, 'Combined Boxplot of Errors in Distance between Pairs', titleSuffix, plotFolderToSave)

# Example datasets list (use your actual datasets)
datasets = [
    {
        "basePath": "./s8 data - three people",
        "gtFiles": {"1": "1 person/ground_truth_coordinates.txt", "2": "2 person/ground_truth_coordinates.txt", "3": "3 person/ground_truth_coordinates.txt"},
        "uwbFiles": {"1": "1 person/uwb_to_bb_mapping.txt", "2": "2 person/uwb_to_bb_mapping.txt", "3": "3 person/uwb_to_bb_mapping.txt"},
        "opticalFiles": {"1": "1 person/optical_to_bb_mapping.txt", "2": "2 person/optical_to_bb_mapping.txt", "3": "3 person/optical_to_bb_mapping.txt"},
        "modelFiles": {"1": "1 person/pixel_to_real_to_bb_mapping.txt", "2": "2 person/pixel_to_real_to_bb_mapping.txt", "3": "3 person/pixel_to_real_to_bb_mapping.txt"},
        "useCols": [1, 2],
        "titleSuffix": "E118(DA_S8_S6(T3_A4_TPh_Md_Wp)) - full area (16.08 meters)",
        "fileName": "e118_full_area",
    },
    {
        "basePath": "./s8 data - three people",
        "gtFiles": {"1": "1 person/ground_truth_coordinates_reduced_range.txt", "2": "2 person/ground_truth_coordinates_reduced_range.txt", "3": "3 person/ground_truth_coordinates_reduced_range.txt"},
        "uwbFiles": {"1": "1 person/uwb_to_bb_mapping_reduced_range.txt", "2": "2 person/uwb_to_bb_mapping_reduced_range.txt", "3": "3 person/uwb_to_bb_mapping_reduced_range.txt"},
        "opticalFiles": {"1": "1 person/optical_to_bb_mapping_reduced_range.txt", "2": "2 person/optical_to_bb_mapping_reduced_range.txt", "3": "3 person/optical_to_bb_mapping_reduced_range.txt"},
        "modelFiles": {"1": "1 person/pixel_to_real_to_bb_mapping_reduced_range.txt", "2": "2 person/pixel_to_real_to_bb_mapping_reduced_range.txt", "3": "3 person/pixel_to_real_to_bb_mapping_reduced_range.txt"},
        "useCols": [1, 2],
        "titleSuffix": "E118(DA_S8_S6(T3_A4_TPh_Md_Wp)) - reduced area (10.08 meters)",
        "fileName": "e118_reduced_area",
    },
    {
        "basePath": "./s301 data - two people",
        "gtFiles": {"1": "1 person - 1 tag/ground_truth_coordinates.txt", "2": "2 person - 2 tag/ground_truth_coordinates.txt"},
        "uwbFiles": {"1": "1 person - 1 tag/uwb_to_bb_mapping.txt", "2": "2 person - 2 tag/uwb_to_bb_mapping.txt"},
        "opticalFiles": {"1": "1 person - 1 tag/optical_to_bb_mapping.txt", "2": "2 person - 2 tag/optical_to_bb_mapping.txt"},
        "modelFiles": {"1": "1 person - 1 tag/pixel_to_real_to_bb_mapping.txt", "2": "2 person - 2 tag/pixel_to_real_to_bb_mapping.txt"},
        "useCols": [3, 4],
        "titleSuffix": "E124(DA_S301_S6(T2_A4_TPh_Md_Wp)) - full area (7.08 meters)",
        "fileName": "e124",
    }
]

# Define base folders
baseFolder = "../../Archive/Images/Plots"
baseFolderMetrics = "../../Archive/Statistics"

# Process experiments
processExperiments(datasets, baseFolder, baseFolderMetrics)
