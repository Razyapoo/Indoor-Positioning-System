# How to compute the distance between two object 

## 1 method 

- Draw the grid on the floor (1m x 1m). Then detect on what cell a person is located. Compute location of a person inside a cell: we know the offset from the boundaries of the cell in pixels and we know the number of pixels of the boundary and we know that it is 1m in real world. Then we can compute the proportion and then compute the actual real distance.  
- https://answers.opencv.org/question/177732/how-to-measure-distance-between-2-objects-in-a-video-edited/
- https://dsp.stackexchange.com/questions/46399/how-to-measure-distance-between-two-3d-objects-using-cameras
- https://github.com/livinlargeinvenus/YOLOv4_Social_Distance
- Bird Eye view: 
  - https://medium.com/@birla.deepak26/social-distancing-ai-using-python-deep-learning-c26b20c9aa4c
  - https://www.ncbi.nlm.nih.gov/pmc/articles/PMC8249827/