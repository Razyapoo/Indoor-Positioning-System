1. Model is trained on data: Neural network test, single person, s8. Therefore, this data corresponds to training data.

2. Polynomial regression was trained for each line separately.

3. Optical method assume that people are approximatelly 15 cm away from paper stickers, therefore -15 cm is subtructed from the distances between camera and a person.

4. Environment Setup:
- Origin of UWB coordinate system:
    - Anchor 101

- Origin of world coordinate system:
    - left wall, at camera line
    - Anchor 101 has world coordinates (0.627, 2.08)

- Anchor baseline: 2.5 m

- Camera distances from:
    - left wall: 0.627 + 1.25 (half of anchor baseline) = 1.877 m
    - right wall: 2.362 m
    - anchor baseline: 2.08 m

- Anchor 101 (Origin of UWB cs) distances from:
    - left wall: 0.627 m
    - right wall: 3.612 m
    - UWB corrdinates: (0, 0)
    - World coordinates (0.627, 2.08)
    
- Anchor 102 distances from:
    - left wall: 3.127 m
    - right wall: 1.112 m
    - UWB corrdinates: (2.5, 0)
    - World coordinates (3.127, 2.08)

- Height of person: 1.78 m

- Height of door: 2.085 m