1. Model is trained on data: Neural network test, single person, s8

2. It was not possible to detect standing periods. Therefore, polynomial regression function was trained on data: Neural network test, single person, s8. For each line and each tag - anchor pair there was extracted polynomial regression parameters and applied to this dataset. Observed area was split into 3 sub-areas. After people are detected, based on pixel coordinates, the corresponding polynomial regression parameters are applied. 

2.1 This also proves that once polynomial regression is trained on one data, it can be applied on another data.
    - Values are not so good compared to when polynomial regression is calculated specificialy for the working (current) video.
    - Function is trained on Tag 1 only! Unfortunatelly, each tag might produce its own error in data. -> therefore, each video need for its own polynomial regression function for each pair of anchor and tag. 

3. Optical method assume that people are approximatelly 15 cm away from paper stickers, therefore -15 cm is subtructed from the distances between camera and a person.

4. Model showed slightly better result than UWB?

5. On large distances detection is bad, boxes are either shorter or longer than actual person is. Therefore on distances larger than 7 meter from camera, coordinates are poorly detected.

4. Environment Setup:
- Origin of UWB coordinate system:
    - Anchor 102

- Origin of world coordinate system:
    - left wall, at camera line
    - Anchor 102 has world coordinates (0.627, 2.08)

- Anchor baseline: 2.5 m

- Camera distances from:
    - left wall: 0.627 + 1.25 (half of anchor baseline) = 1.877 m
    - right wall: 2.362 m
    - anchor baseline: 2.08 m

- Anchor 102 (Origin of UWB cs) distances from:
    - left wall: 0.627 m
    - right wall: 3.612 m
    - UWB corrdinates: (0, 0)
    - World coordinates (0.627, 2.08)
    
- Anchor 101 distances from:
    - left wall: 3.127 m
    - right wall: 1.112 m
    - UWB corrdinates: (2.5, 0)
    - World coordinates (3.127, 2.08)

- Anchor 103 (Origin of UWB cs) distances from:
    - left wall: 0.627 m
    - right wall: 3.612 m
    - UWB corrdinates: (0, 15)
    - World coordinates (0.627, 17.08)
    
- Anchor 104 distances from:
    - left wall: 3.127 m
    - right wall: 1.112 m
    - UWB corrdinates: (2.5, 15)
    - World coordinates (3.127, 17.08)


- Height of people: Oskar, Peter are 1.78 m. Stepan is 1.86 m

- Height of door: 2.085 m