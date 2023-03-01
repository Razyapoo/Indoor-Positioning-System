import numpy as np

chessBoardSize = (9, 6)
squareSize = 17

objPoint = np.zeros((chessBoardSize[1] * chessBoardSize[0], 3), np.float32)
objPoint[:,:2] = np.mgrid[0:chessBoardSize[0], 0:chessBoardSize[1]].T.reshape(-1, 2)
# objPoint *= squareSize

print (objPoint)