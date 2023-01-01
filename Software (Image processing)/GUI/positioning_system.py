from __future__ import print_function
import json
import time
from PIL import Image, ImageTk
import tkinter as tk
from tkinter import ttk
import threading
import datetime
import cv2
import os
from configs import config
from configs.detection import detectPeople
from scipy.spatial import distance as dist 
import numpy as np
import imutils
import queue
import tkinter.scrolledtext as st
import select, socket
from datetime import datetime

class TKinterVideoStream(tk.Frame):
    def __init__(self, root):
        #call super class (Frame) constructor
        tk.Frame.__init__(self, root)
        #save root layour for later references
        self.root = root
        #load all UI
        self.setupUI()
        
    def setupUI(self):
        #create a header
        self.header = tk.Label(self, text="Video Stream", bg="black", fg="white")
        # self.header.pack(side="top", fill=tk.BOTH, expand=True)
        self.header.grid(row=0, column=0)
        
        #create label to hold image
        self.frameLabel = tk.Label(self)
        self.frameLabel.grid(row=1, column=0, pady=10)
        
    def updateFrame(self, frame):
        #configure image_label with new image 
        self.frameLabel.configure(image=frame)
        #this is to avoid garbage collection, so we hold an explicit reference
        self.frame = frame

class TKinterUWBLabel(tk.Frame):
    def __init__(self, root, tagID ):
        #call super class (Frame) constructor
        tk.Frame.__init__(self, root)
        self.root = root
        self.tagID = tagID
        self.position = "Position: -1"
        self.anchorFirstDistance = "Anchor 101: -1"
        self.anchorSecondDistance = "Anchor 102: -1"
        #load all UI
        self.setupUI()
        
    def setupUI(self):
        self.idLabel = tk.Label(self, text=f"Tag ID: {self.tagID}")
        self.idLabel.grid(row=0, column=0)
        self.positionLabel = tk.Label(self, text=self.position)
        self.positionLabel.grid(row=1, column=0)
        self.anchorList = tk.Label(self, text="Anchor distances:")
        self.anchorList.grid(row=2, column=0)
        self.anchorFirstLabel = tk.Label(self, text=self.anchorFirstDistance)
        self.anchorFirstLabel.grid(row=3, column=1)
        self.anchorSecondLabel = tk.Label(self, text=self.anchorSecondDistance)
        self.anchorSecondLabel.grid(row=4, column=1)
        
    def updateLabel(self, data):
        # if data["Tag ID"] == self.tagID:
        self.position = "Position: 0" #TODO
        for anchor in data["anchors"]:
            if anchor["Anchor ID"] == 101:
                self.anchorFirstDistance = "Anchor 101: " + str(anchor["distance"]) 
            else: 
                self.anchorSecondDistance = "Anchor 102: " + str(anchor["distance"])
        # else: 
        #     self.position = "Position: -1"
        #     self.anchorFirstDistance = "Anchor 101: -1"
        #     self.anchorSecondDistance = "Anchor 101: -1"
            
        self.positionLabel.config(text=self.position)
        self.anchorFirstLabel.config(text=self.anchorFirstDistance)
        self.anchorSecondLabel.config(text=self.anchorSecondDistance)
        

class TKinterUWBLabelPanel(tk.Frame):
    def __init__(self, root):
        #call super class (Frame) constructor
        tk.Frame.__init__(self, root)
        self.root = root
        self.tagList = []
        self.numberOfTags = 3
        #load all UI
        self.setupUI()
        
    def setupUI(self):
        self.header = tk.Label(self, text="Tags", bg="black", fg="white")
        self.header.grid(row=0, column=0)
        
        # self.firstTag = TKinterUWBLabel(self, 1)
        # self.secondTag = TKinterUWBLabel(self, 2)
        # self.thirdTag = TKinterUWBLabel(self, 3)
        for i in range(self.numberOfTags):
            label = TKinterUWBLabel(self, i + 1)
            label.grid(row=i, column=0)
            self.tagList.append(label)
                
    def updateData(self, uwbData):
        for i in range(self.numberOfTags):
            if self.tagList[i].tagID == uwbData["Tag ID"]:
                self.tagList[i].updateLabel(uwbData)
       
        
class TKinterStreamLog(tk.Frame):
    def __init__(self, root):
        #call super class (Frame) constructor
        tk.Frame.__init__(self, root)
        #save root layour for later references
        self.root = root
        #load all UI
        self.setupUI()
        self.limit = 20
    
    def setupUI(self):
        #create a header
        self.header = tk.Label(self.root, text="Stream Log", bg="black", fg="white")
        self.header.grid(row=0, column=0)
        
        self.textArea = st.ScrolledText(self.root, width = 82, height = 15, font = ("Times New Roman", 10))
        self.textArea.grid(row=1, column=0, pady=10)
        self.textArea.configure(state ='disabled')
        
    def updateTextArea(self, data):
        # Making the text modifiable
        self.textArea.configure(state ='normal')
        
        # Inserting Text which is read only
        data = str(data) + "\n"
        self.textArea.insert(tk.INSERT, data)
        self.textArea.see(tk.END)
        
        if (int(self.textArea.index('end-1c').split('.')[0]) > self.limit):
            self.textArea.delete('1.0', '2.0')
        
        # Making the text read only
        self.textArea.configure(state ='disabled')
        
class TKinterStreamLogTopLevel(tk.Toplevel):
    def __init__(self, appGui, title, type):
        #call super class (Frame) constructor
        tk.Toplevel.__init__(self, appGui.root)
        # layout
        if (type == "videoStream"):
            self.geometry('600x310+300+500')
        
        if (type == "uwbStream"):
            self.geometry('600x310+1700+500')
        
        self.attributes('-topmost', 'true')
            
        self.title(title)
        self.protocol("WM_DELETE_WINDOW", self.close)
        
        self.root = appGui.root
        self.appGui = appGui
        self.type = type
        self.tkinterStream = TKinterStreamLog(self)
        self.tkinterStream.grid(row=0, column=0)
        ttk.Button(self, text='Hide', command=self.hide).grid(row=3, column=0, padx=90, sticky='e')
        ttk.Button(self, text='Close', command=self.close).grid(row=3, column=0, sticky='e')
        
    # Show the window
    def show(self):
        self.deiconify()
    
    # Hide the window
    def hide(self):
        self.withdraw()
        
    def close(self):
        self.appGui.onCloseStreamLog(self.type)
        self.destroy()

# Temporary
class TKinterComparisonFrame(tk.Frame):
    def __init__(self, root):
        #call super class (Frame) constructor
        tk.Frame.__init__(self, root)
        #save root layour for later references
        self.root = root
        self.videoDistanceCache = -1
        self.uwbAnchorDistanceCache = -1
        self.color = "gray"
        #load all UI
        self.setupUI()
    
    def setupUI(self):

        self.videoStreamLabel = tk.Label(self, text="Distance from video:")
        self.videoStreamLabel.grid(row=1, column=0, sticky='w')
        # self.videoStreamLabel.pack(side="top", fill=tk.BOTH, expand=True, anchor=tk.W)
        
        self.uwbStreamLabel = tk.Label(self, text="Distance from server:")
        self.uwbStreamLabel.grid(row=2, column=0, sticky='w')
        # self.uwbStreamLabel.pack(side="top", fill=tk.BOTH, expand=True, anchor=tk.W)
        
        self.distanceVideo = tk.Label(self, text="0", fg=self.color)
        self.distanceVideo.grid(row=1, column=1)
        
        self.distanceUwb = tk.Label(self, text="0", fg=self.color)
        self.distanceUwb.grid(row=2, column=1)
    
    def updateDistanceVideo(self, dist=-1):
        self.videoDistanceCache = dist;
        self.compare()
        self.distanceVideo.configure(text=self.videoDistanceCache, fg=self.color)
        # self.distanceVideo = tk.Label(self, text=self.videoDistanceCache, fg=self.color)
        # self.distanceVideo.grid(row=1, column=1)
    
    def updateDistanceUwb(self, dist=-1):
        self.uwbAnchorDistanceCache = dist
        self.compare()
        self.distanceUwb.configure(text=self.uwbAnchorDistanceCache, fg=self.color)
        # self.distanceUwb = tk.Label(self, text=self.uwbAnchorDistanceCache, fg=self.color)
        # self.distanceUwb.grid(row=2, column=1)        
    
    def compare(self):
        if self.videoDistanceCache > -1 and self.uwbAnchorDistanceCache > -1:
            if abs(self.videoDistanceCache - (self.uwbAnchorDistanceCache*100)) < 200: self.color = "green"
            else: self.color = "red"
    
        
class AppGUI():
    def __init__(self):
         #initialize the gui toolkit
        self.root = tk.Tk()
        
        self.root.geometry("1270x700")
        self.root.title("Position Analysis System")
        self.root.resizable(0, 0)
        
        # 0 column
        # self.root.grid_columnconfigure(0, weight=1)
        
        #create left screen view
        self.tkinterVideoStream = TKinterVideoStream(self.root)
        self.tkinterVideoStream.grid(row=0, column=0, sticky='n', padx=10, pady=10)
        
        # self.tkinterUwbServerStream = TKinterUwbServerStreamLog(self.root)
        # self.tkinterUwbServerStream.grid(row=0, column=1, sticky='n', padx=10, pady=10)
        # self.UwbServerStream.pack(side='right', anchor=tk.N)
        
        self.tkinterVideoStreamLogTopLevel = TKinterStreamLogTopLevel(self, title="Video Stream", type="videoStream")
        ttk.Button(self.root, text='Show video stream log', command=lambda: self.openStreamLog("videoStream")).grid(row=1, column=0, sticky='e', padx=10)
        
        self.tkinterComparisonFrame = TKinterComparisonFrame(self.root)
        self.tkinterComparisonFrame.grid(row=2, column=0, pady=10, padx=10, sticky='w')
        
        # 1st column
        self.root.grid_columnconfigure(1, weight=1)
        
        self.tkinterUwbLabelPanel = TKinterUWBLabelPanel(self.root)
        self.tkinterUwbLabelPanel.grid(row=0, column=1, pady=10, padx=10, sticky='w')
        
        self.tkinterUwbServerStreamLogTopLevel = TKinterStreamLogTopLevel(self, title="Uwb Stream", type="uwbStream")
        ttk.Button(self.root, text='Show uwb server log', command=lambda: self.openStreamLog("uwbStream")).grid(row=1, column=1,sticky='se')
        
        
    def launch(self):
        #start the gui loop to listen for events
        self.root.mainloop()
            
    def openStreamLog(self, type):
        if type == "videoStream":
            # show if exists, otherwise create
            if self.tkinterVideoStreamLogTopLevel:
                self.tkinterVideoStreamLogTopLevel.show()
            else:
                self.tkinterVideoStreamLogTopLevel = TKinterStreamLogTopLevel(self, title="Video Stream", type="videoStream")
        
        if type == "uwbStream":
            # show if exists, otherwise create
            if self.tkinterUwbServerStreamLogTopLevel:
                self.tkinterUwbServerStreamLogTopLevel.show()
            else:
                self.tkinterUwbServerStreamLogTopLevel = TKinterStreamLogTopLevel(self, title="Uwb Stream", type="uwbStream")
    
    def onCloseStreamLog(self, type):
            if type == "videoStream":
                self.tkinterVideoStreamLogTopLevel = None

            if type == "uwbStream":
                self.tkinterUwbServerStreamLogTopLevel = None
                
    def updateOutputFrameVideo(self, frame=None, videoDistance=-1):
        if not frame is None :
            frame = imutils.resize(frame, 700)
            #convert frame to PIL library format which is required for Tk toolkit
            frame = Image.fromarray(frame)
            
            #convert frame to Tk toolkit format
            frame = ImageTk.PhotoImage(frame)

            self.tkinterVideoStream.updateFrame(frame)
        
        if videoDistance != -1:
            self.tkinterComparisonFrame.updateDistanceVideo(videoDistance)
            # Update log if exists
            if self.tkinterVideoStreamLogTopLevel:
                self.tkinterVideoStreamLogTopLevel.tkinterStream.updateTextArea(videoDistance)
    
    def updateOutputFrameUwb(self, uwbDataJSON=""):
        if uwbDataJSON:
            print(uwbDataJSON)
            # Update log if exists
            if self.tkinterUwbServerStreamLogTopLevel:
                self.tkinterUwbServerStreamLogTopLevel.tkinterStream.updateTextArea(uwbDataJSON)
            # Try to parse data, because when there is no detected anchor, data are coming in other than JSON format
            try:
                uwbData = json.loads(uwbDataJSON)
                self.tkinterUwbLabelPanel.updateData(uwbData)
                dist = uwbData["anchors"][0]["distance"]
                self.tkinterComparisonFrame.updateDistanceUwb(dist)
            except:
                uwbData = ""

                

class VideoDistanceAnalyzer(threading.Thread):
    def __init__(self, camID, mainGUI, callbackQueue):
        # call super class (Thread) constructor
        threading.Thread.__init__(self)
        
        self.camID = camID
        self.toStop = False
        self.isRunning = False
        self.mainGUI = mainGUI
        self.callbackQueue = callbackQueue
        
        # self.Frame = None
        self.net = None
        self.labels = None
        self.ln = None
        # self.distance = 0
        
        self.readNN()
    
    def openCamera(self):
        # setup stream
        self.isRunning = True
        self.videoStream = cv2.VideoCapture(self.camID)
        if not self.videoStream.isOpened():
            raise ValueError("Unable to open video source", self.camID)
        
    def readNN(self):
        print("[INFO] Analyzer setup: ")
        # load the COCO class labels the YOLO model was trained on
        labelsPath = os.path.sep.join([config.MODEL_PATH, "coco.names"])
        self.labels = open(labelsPath).read().strip().split("\n")

        # derive the paths to the YOLO weights and model configuration
        weightsPath = os.path.sep.join([config.MODEL_PATH, "yolov4-tiny.weights"])
        configPath = os.path.sep.join([config.MODEL_PATH, "yolov4-tiny.cfg"])

        # load the YOLO object detector trained on COCO dataset (80 classes)
        print(" [INFO] loading YOLO from disk...")
        self.net = cv2.dnn.readNetFromDarknet(configPath, weightsPath)

        # check if GPU is to be used or not
        if config.USE_GPU:
            # set CUDA s the preferable backend and target
            print(" [INFO] setting preferable backend and target to CUDA...")
            self.net.setPreferableBackend(cv2.dnn.DNN_BACKEND_CUDA)
            self.net.setPreferableTarget(cv2.dnn.DNN_TARGET_CUDA)

        # determine only the "output" layer names that we need from YOLO
        self.ln = self.net.getLayerNames()
        self.ln = [self.ln[i - 1] for i in self.net.getUnconnectedOutLayers()]

    def computeDistance(self, frame):
        
        results = detectPeople(frame, self.net, self.ln, personIdx=self.labels.index("person"))
        distance = 0

        # initialize the set of indexes that violate the minimum social distance
        violate = set()
            # ensure there are at least two people detections (required in order to compute the
        # the pairwise distance maps)
        if len(results) >= 2:
            # extract all centroids from the results and compute the Euclidean distances
            # between all pairs of the centroids
            centroids = np.array([r[2] for r in results])
            # loop over the upper triangular of the distance matrix
            for i in range(0, centroids.shape[0]):
                # for j in range(i+1, D.shape[1]):
                for j in range(0, centroids.shape[0]):
                    # check to see if the distance between any two centroid pairs is less
                    # than the configured number of pixels
                    if i < j:
                        dist = np.sqrt(pow(centroids[i][0]-centroids[j][0],2) + pow(centroids[i][1]-centroids[j][1],2) + pow(centroids[i][2]-centroids[j][2],2))
                        if (dist < config.MIN_DISTANCE): 
                            # update the violation set with the indexes of the centroid pairs
                            violate.add(i)
                            violate.add(j)
        
        # loop over the results
        for (i, (prob, bbox, centroid)) in enumerate(results):
            # extract teh bounding box and centroid coordinates, then initialize the color of the annotation
            (startX, startY, endX, endY) = bbox
            (cX, cY, distance) = centroid
            color = (0, 255, 0)

            # if the index pair exists within the violation set, then update the color
            if i in violate:
                color = (0, 0, 255)

            text = "Depth: {} cm".format(round(distance))
            cv2.putText(frame, text, (startX + 10, startY + 100),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, color, 2)
            # draw (1) a bounding box around the person and (2) the centroid coordinates of the person
            cv2.rectangle(frame, (startX, startY), (endX, endY), color, 2)

        # draw the total number of social distancing violations on the output frame
        text = "Social Distancing Violations: {}".format(len(violate))
        cv2.putText(frame, text, (10, frame.shape[0] - 25), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 0, 255), 3)
        
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        
        if self.callbackQueue.full() == False:
            #put the update UI callback to queue so that main thread can execute it
            self.callbackQueue.put((lambda: self.updateMainThread(frame, distance)))
        
    def run(self):
        self.openCamera()
        
        while True:
            if self.toStop:
                self.isRunning = False
                break
            
            grabbed, frame = self.videoStream.read()
            
            if not grabbed:
                break
            
            frame = imutils.resize(frame, width=700)
            
            self.computeDistance(frame)
            
            # cv2.imshow("Frame", frame)
            # if self.callbackQueue.full() == False:
            #     #put the update UI callback to queue so that main thread can execute it
            #     self.callbackQueue.put((lambda: self.updateMainThread(frame, distance)))
    
    def updateMainThread(self, frame, distance):
        self.mainGUI.updateOutputFrameVideo(frame=frame, videoDistance=distance)
        
    # def __del__(self):
    #     self.release()
        
    def release(self):
        self.toStop = True
        self.videoStream.release()
        
    # def stop(self):
        
        
        
class UWBServer(threading.Thread):
    def __init__(self, mainGUI, callbackQueue):
        # call super class (Thread) constructor
        threading.Thread.__init__(self)
        # setup stream
        self.toStop = False
        self.isRunning = False
        self.mainGUI = mainGUI
        self.callbackQueue = callbackQueue
        
        self.server = None
        self.inputs = []
        self.outputs = []
        self.isServerBusy = False
        # self.receivedData = None
        self.requestSentTime = None
        
    def startServer(self):
        print("Startup server")
        self.isRunning = True
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.server.setblocking(0)
        self.server.bind(('', 30001))
        self.server.listen()
        self.inputs = [self.server]
        
    def run(self):
        # print("in server run")
        self.startServer()
        
        while self.inputs:
            
            if self.toStop:
                self.isRunning = False
                break
            
            readable, writable, exceptional = select.select(self.inputs, self.outputs, self.inputs)
            # print("There is some event on socket")
            if readable:
                for s in readable:
                    if s is self.server:
                        if s.fileno() == -1:
                            print("Server is closed")
                            break
                        
                        connection, client_address = s.accept()
                        print('Connected to: ' + client_address[0] + ':' + str(client_address[1]))
                        connection.setblocking(1)
                        self.outputs.append(connection)
                        
                    else:
                        data = s.recv(1024)
                        if data:
                            receivedData = data.decode('utf-8')
                            # print(receivedData)
                            if self.callbackQueue.full() == False:
                                #put the update UI callback to queue so that main thread can execute it
                                self.callbackQueue.put((lambda: self.mainGUI.updateOutputFrameUwb(uwbDataJSON=receivedData)))
                            ack = "7" # ack received
                            s.send(str.encode(ack))
                            if s in self.inputs:
                                self.inputs.remove(s)
                            if s not in self.outputs:
                                self.outputs.append(s)
                            self.isServerBusy = False
            elif (len(self.inputs) > 1) and (time.perf_counter() - self.requestSentTime) > 0.3:
                for s in self.inputs:
                    if s != self.server:
                        print("close connection")
                        # if s in self.outputs:
                        #     self.outputs.remove(s)
                        if s in self.inputs:
                            self.inputs.remove(s)
                        self.isServerBusy = False
                        s.close()

            if writable and not self.isServerBusy:
                self.isServerBusy = True
                s = writable.pop(0)
                s.send(str.encode("1"))
                self.outputs.remove(s)
                self.inputs.append(s)
                self.requestSentTime = time.perf_counter()
                

            if exceptional:
                for s in exceptional:
                    if s in self.outputs:
                        self.outputs.remove(s)
                    if s in self.inputs:
                        self.inputs.remove(s)
                    self.isServerBusy = False
                    s.close()
    
    # def updateMainThread(self, receivedData):
    #     self.mainGUI.updateOutputFrameUwb(uwbDataJSON=receivedData)

    def stopServer(self):
        self.toStop = True
        self.server.shutdown(socket.SHUT_RDWR)
        self.server.close()
        
class Wrapper():
    def __init__(self):
        self.mainGUI = AppGUI()
        self.videoStreamCallbackQueue = queue.Queue()
        self.uwbStreamCallbackQueue = queue.Queue()
        self.runVideoDistanceAnalyzer = True
        self.runUwbServer = True
        
        self.videoDistanceAnalyzer = VideoDistanceAnalyzer(0, self.mainGUI, self.videoStreamCallbackQueue)
        self.uwbServer = UWBServer(self.mainGUI, self.uwbStreamCallbackQueue)

        #save attempts made to fetch webcam video in case of failure 
        self.attemptsVideoStream = 0
        self.attemptsUwbStream = 0
        
        #register callback for being called when GUI window is closed
        self.mainGUI.root.protocol("WM_DELETE_WINDOW", self.close)
        
        #start webcam
        if self.runVideoDistanceAnalyzer:
            self.startAnalyzerThread()
        
        #start uwb server
        if self.runUwbServer:
            self.startServerThread()
        
        #start fetching streams
        self.fetchVideoStream()
        self.fetchUwbStream()

    def startAnalyzerThread(self):
        self.videoDistanceAnalyzer.start()
        
    def startServerThread(self):
        self.uwbServer.start()
                
    def fetchVideoStream(self):
        try:
            #while True:
            #try to get a callback put by videoDistanceAnalyzer
            #if there is no callback and callbackQueue is empty
            #then this function will throw a Queue.Empty exception 
            callback = self.videoStreamCallbackQueue.get_nowait()
            callback()
            self.attemptsVideoStream = 0
            self.mainGUI.root.after(1, self.fetchVideoStream)
                
        except queue.Empty:
            if (self.attemptsVideoStream <= 50):
                self.attemptsVideoStream = self.attemptsVideoStream + 1
                self.mainGUI.root.after(100, self.fetchVideoStream)
    
    def fetchUwbStream(self):
        try:
            #while True:
            #try to get a callback put by uwbServer
            #if there is no callback and callbackQueue is empty
            #then this function will throw a Queue.Empty exception 
            callback = self.uwbStreamCallbackQueue.get_nowait()
            callback()
            self.attemptsUwbStream = 0
            self.mainGUI.root.after(1, self.fetchUwbStream)
                
        except queue.Empty:
            if (self.attemptsUwbStream <= 50):
                self.attemptsUwbStream = self.attemptsUwbStream + 1
                self.mainGUI.root.after(100, self.fetchUwbStream)
                    
    def launch(self):
        self.mainGUI.launch()
        
    def stopAnalyzerThread(self):
        self.videoDistanceAnalyzer.release()
        self.videoDistanceAnalyzer.join()

    def stopServerThread(self):
        self.uwbServer.stopServer()
        self.uwbServer.join()

    def close(self):
        if self.videoDistanceAnalyzer.isRunning:
            self.stopAnalyzerThread()
        if self.uwbServer.isRunning:
            self.stopServerThread()
        
        self.mainGUI.root.destroy()
        
    # def __del__(self):
    #     # self.videoDistanceAnalyzer.stop()
    #     # self.uwbServer.stop()
    #     self.close()
        
        
if __name__=="__main__":
    wrapper = Wrapper()
    wrapper.launch()