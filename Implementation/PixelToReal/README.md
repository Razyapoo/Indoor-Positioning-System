# Camera Intrinsic Calibration

Contains the code for training of the Pixel-to-Real model.

- `data_to_train_model_experiment_109.txt` - export of pairs consisting of UWB coordinates and pixel coordinates; export from Indoor Positioning System (GUI)
    - File schema:
        ```
        { Frame ID } { UWB X Coordinate } { UWB Y Coordinate } { Pixel X Coordinate } { Pixel Y Coordinate } 
        ```
- `PixeltoReal_model_to_be_used_in_GUI.json` - prepared Pixel-to-Real model to use in Indoor Positioning System (GUI)

**Requirements**:
- eXtream Gradient Boost:
    - `pip install xgboost`
    - installation guide: https://xgboost.readthedocs.io/en/stable/install.html

## Structure
```
.
├── data_to_train_model_experiment_109.txt      # Data export
├── PixeltoReal_model_to_be_used_in_GUI.json    # Trained model
├── README.md
└── train_PixeltoReal_model.py                  # Code for training the model
```