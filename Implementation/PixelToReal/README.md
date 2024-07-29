# Pixel-to-Real model training

The implmentation of the Pixel-to-Real model training.

**Input for model training:**
- `data_to_train_model_experiment_109.txt` - export of synchronized (aligned) pairs consisting of UWB coordinates and pixel coordinates; export from Indoor Positioning System (GUI)
    - File schema:
        ```
        { Frame ID } { UWB X Coordinate } { UWB Y Coordinate } { Pixel x Coordinate } { Pixel y Coordinate } 
        ```
    - Note: Pixel means real pixel in image, not PixelToReal model

**Output:**

- `PixeltoReal_model_to_be_used_in_GUI.json` - prepared (trained) Pixel-to-Real model to use in Indoor Positioning System (GUI). **JSON format**!!!

**Requirements**:
   - [eXtream Gradient Boost](https://github.com/dmlc/xgboost)
   - python 3.8 or newer

## Installation 

   To install the Main GUI application either use the [install.sh](/Implementation/install.sh) to automatically install all the requirement dependencies, or follow these steps:

   1. **Install the required dependencies:**
      ```sh 
      pip install xgboost numpy==1.26.1 pandas==2.1.2 scipy==1.11.4 bottleneck==1.3.4
      ```

   4. **Build and run the application:**

      ```sh
      python3 train_PixeltoReal_model.py
      ```

## Structure of the folder
```
.
├── data_to_train_model_experiment_109.txt      # Data export that is used to train the Pixel-to-Real model
├── PixeltoReal_model_to_be_used_in_GUI.json    # Trained Pixel-to-Real model
├── README.md
└── train_PixeltoReal_model.py                  # Code for training the Pixel-to-Real model
```
