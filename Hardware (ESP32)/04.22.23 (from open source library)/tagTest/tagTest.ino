#include <Arduino.h>
#include <DW1000.h>
#include <DW1000Time.h>
#include <ESP8266WiFi.h>

// Constants
const uint8_t ANCHOR_ADDRESS = 11;
const uint8_t TAG_ADDRESS = 1;

const byte MSG_TYPE_DISC = 0x01;
const byte MSG_TYPE_POLL = 0x02;
const byte MSG_TYPE_RESP = 0x03;
const byte MSG_TYPE_FINAL = 0x04;
const byte MSG_TYPE_REPORT = 0x05;

const int IRQ_PIN = 2;
const int RST_PIN = 5;
const int SS_PIN = 4;

// Wi-Fi credentials
const char* ssid = "<WIFI_SSID>";
const char* password = "<WIFI_PASSWORD>";

// Server connection
const char* serverIp = "<SERVER_IP>";
const uint16_t serverPort = <SERVER_PORT>;

// Function declarations
void connectToWifi();
void connectToServer();
void waitForStartCommand();
void sendDistancesToServer();

void discoverAnchors();
void performRanging();
double calculateDistance();

// Variables
WiFiClient client;

bool anchorsDiscovered[255] = {false};
byte discoveredAnchors[255];
// double distances[255];
int discoveredAnchorCount = 0;
bool firstAnchor = true;
String msgToSend;

DW1000Time timePollSent, timeFinalSent, timePollAckReceived;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Tag");

  DW1000.begin(IRQ_PIN, RST_PIN);
  SPI.begin(SS_PIN);

  DW1000.newConfiguration();
  DW1000.setDefaults();
  DW1000.setDeviceAddress(TAG_ADDRESS);
  DW1000.commitConfiguration();

  //pinMode(LED_BUILTIN, OUTPUT);

  //connectToWifi();
  //connectToServer();
}

void loop() {
  // waitForStartCommand();
  discoverAnchors();
  performRanging();
  // sendDistancesToServer();
}

void connectToWifi() {
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to Wi-Fi");
}

void connectToServer() {
  Serial.print("Connecting to server");
  while (!client.connect(serverIp, serverPort)) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to server");
}

void waitForStartCommand() {
  Serial.println("Waiting for start command from server");
  while (client.available() == 0);
  Serial.println("Received start command from server");
}

void sendDistancesToServer() {
  Serial.println("Sending distances to server");
  for (int i = 0; i < discoveredAnchorCount; i++) {
    client.print(discoveredAnchors[i]);
    client.print(": ");
    client.println(calculateDistance());
  }
  Serial.println("Distances sent");
}

void discoverAnchors() {
  Serial.println("Discovering anchors");
  discoveredAnchorCount = 0;
  while (discoveredAnchorCount < 2) {
    DW1000.newTransmit();
    byte message[] = {MSG_TYPE_DISC};
    DW1000.setData(message, sizeof(message));
    DW1000.setReceiver(ANCHOR_ADDRESS + discoveredAnchorCount);
    DW1000.startTransmit();
    while (!DW1000.isTransmitDone());
    DW1000.clearTransmitStatus();

    DW1000.newReceive();
    DW1000.startReceive();
    while (!DW1000.isReceiveDone() && !DW1000.isReceiveTimeout());
    if (DW1000.isReceiveTimeout()) {
      DW1000.clearReceiveTimeout();
      // discoveredAnchorCount++;
      continue;
    }
    byte receivedMessage[10];
    DW1000.getData(receivedMessage, sizeof(receivedMessage));
    if (receivedMessage[0] == ANCHOR_ADDRESS + discoveredAnchorCount && !isAnchorDiscovered(receivedMessage[0])) {
      discoveredAnchors[discoveredAnchorCount] = receivedMessage[0];
      discoveredAnchorCount++;
    }
    DW1000.clearReceiveStatus();
  }
  Serial.print("Anchors discovered: ");
  Serial.println(discoveredAnchorCount);
}

void performRanging() {
  msgToSend = "";
  msgToSend = "{\"Tag ID\": " + String(TAG_ADDRESS) + ", \"anchors\": [";
  for (int i = 0; i < discoveredAnchorCount; i++) {
    // Poll
    sendMessage(MSG_TYPE_POLL, discoveredAnchors[i]);
    if (!messageReceived(MSG_TYPE_RESP, discoveredAnchors[i])) {
      continue;
    }
    saveTimestamps(MSG_TYPE_RESP);

    sendMessage(MSG_TYPE_FINAL, discoveredAnchors[i]);
    if (!messageReceived(MSG_TYPE_REPORT, discoveredAnchors[i])) {
      continue;
    }
    saveTimestamps(MSG_TYPE_FINAL);

    double distance = calculateDistance();
    if (distance > 0) {
      if (firstAnchor) {
        msgToSend += "{ \"Anchor ID\": " + String(discoveredAnchors[i]) + ", \"distance\": " + String(distance) + " }"; 
        firstAnchor = false;
      }
      else msgToSend += ", { \"Anchor ID\": " + String(discoveredAnchors[i]) + ", \"distance\": " + String(distance) + " }";

    }
    // // Send distance
    // sendMessage(MSG_TYPE_DIST, discoveredAnchors[i]);
  }
  msgToSend += "]}";
  firstAnchor = true;
}

double calculateDistance() {
  // Calculate the distance using the collected timestamps
  // You can replace the below lines with your desired distance calculation method
  DW1000Time timeOfFlight = (t2 - timePollSent) / 2;
  double distance = timeOfFlight.getAsMeters();
  return distance;
}

void sendMessage(byte messageType, byte receiverAddress) {
  DW1000.newTransmit();
  byte message[] = {messageType};
  DW1000.setData(message, sizeof(message));
  DW1000.setReceiver(receiverAddress);
  DW1000.startTransmit();
  while (!DW1000.isTransmitDone());
  DW1000.clearTransmitStatus();
}

bool messageReceived(byte expectedMessageType, byte senderAddress) {
  DW1000.newReceive();
  DW1000.startReceive();
  while (!DW1000.isReceiveDone() && !DW1000.isReceiveTimeout());
  if (DW1000.isReceiveTimeout()) {
    DW1000.clearReceiveTimeout();
    return false;
  }
  byte message[10];
  DW1000.getData(message, sizeof(message));
  if (message[0] == expectedMessageType && DW1000.getSender() == senderAddress) {
    DW1000.clearReceiveStatus();
    return true;
  }
  return false;
}

void saveTimestamps(const byte& messageType) {
  if (messageType == MSG_TYPE_RESP){
    DW1000.getTransmitTimestamp(timePollSent);
    DW1000.getReceiveTimestamp(timePollAckReceived);
  } else if (messageType == MSG_TYPE_FINAL){
    DW1000.getTransmitTimestamp(timeFinalSent);
    // DW1000.getReceiveTimestamp(t2);
  }
}

bool isAnchorDiscovered(byte& anchorAddress) {
  for (byte i = 0; i < anchorCount; i++) {
    if (discoveredAnchors[i] == anchorAddress) {
      return true;
    }
  }
  return false;
}
