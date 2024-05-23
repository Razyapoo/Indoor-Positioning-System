from ultralytics import YOLO


model = YOLO("yolov8m.pt")
path = model.export(format="onnx")
