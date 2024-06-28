import os
import re

def appendFiles(folderPath):
    commonFilePath = os.path.join(folderPath, 'uwb_to_bb_mapping_entire.txt')
    matchingFiles = []

    for filename in os.listdir(folderPath):
        if re.match(r'uwb_to_bb_mapping_\d+\.txt', filename):
            matchingFiles.append(filename)

    matchingFiles.sort()

    with open(commonFilePath, 'w') as commonFile:
        for filename in matchingFiles:
            filePath = os.path.join(folderPath, filename)
            with open(filePath, 'r') as file:
                for line in file:
                    commonFile.write(line)

folderPath = '/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/Train model/Data for comparison/306 data - single person/export'
appendFiles(folderPath)
