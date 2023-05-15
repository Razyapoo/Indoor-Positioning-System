// currently tag is module #5
// The purpose of this code is to set the tag address and antenna delay to default.
// this tag will be used for calibrating the anchors.

#include <SPI.h>
#include "DW1000Ranging.h"
#include "DW1000.h"
#include <WiFi.h>
#include <WiFiClient.h>

#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23
#define DW_CS 4

#define STATE_IDLE 0
#define STATE_RANGING 1

// connection pins
const uint8_t PIN_RST = 27; // reset pin
const uint8_t PIN_IRQ = 34; // irq pin
const uint8_t PIN_SS = 4;   // spi select pin


char ssid[] = "oskar-hotspot";
const char *password = "Nera1998";
const char *host = "10.42.0.1";
const int serverPort = 30001;

byte state = STATE_IDLE;
struct Anchor {
  uint16_t ID;
  double distance;
};
// TAG antenna delay defaults to 16384
// leftmost two bytes below will become the "short address"
char* tag_addr;// = "72:00:22:EA:82:60:3B:9C";

const size_t minAnchorNumber = 2;
const size_t maxAnchorNumber = 5;
Anchor anchors[maxAnchorNumber];

size_t anchorCounter = 0;
size_t distanceCounter = 0;

uint16_t shortAddress;
uint8_t myID;
String msgToSend;
long int start, duration;
WiFiClient client;

void setAddress() {
    String macAddr = WiFi.macAddress();
    //Serial.print("mac addr: ");
    //Serial.println(macAddr);
    if (macAddr == "70:B8:F6:D8:F6:48") {
        myID = 1;
        tag_addr = "70:B8:F6:D8:F6:48";
    } else if (macAddr == "70:B8:F6:D8:F6:24") {
        myID = 2;
        tag_addr = "70:B8:F6:D8:F6:24";
    } else if (macAddr == "70:B8:F6:D8:F6:60") {
        myID = 3;
        tag_addr = "70:B8:F6:D8:F6:60";
    } else {
        //Serial.println("Wrong tag MAC address.");
        //Serial.print(macAddr);
    }
}

void connectToWiFi() {
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  //Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  //Serial.println("Connected to Wi-Fi");
}

void connectToServer() {
  //Serial.print("Connecting to server: ");
  //Serial.println(host);

  if (!client.connect(host, serverPort)) {
    //Serial.println("Connection failed");
  } else {
    //Serial.println("Successful connection");
  }
}

void setup()
{
  //Serial.begin(115200);
  delay(1000);

  //start = millis();
  //init the configuration
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ); //Reset, CS, IRQ pin

  DW1000Ranging.attachNewRange(newRange);
  DW1000Ranging.attachNewDevice(newDevice);
  DW1000Ranging.attachInactiveDevice(inactiveDevice);

  setAddress();
  // start as tag, do not assign random short address
  DW1000Ranging.startAsTag(tag_addr, DW1000.MODE_LONGDATA_RANGE_LOWPOWER, false);
  //duration = millis() - start;
  //Serial.print("--------------------");
  //Serial.print("Duration of setup: ");
  //Serial.println(duration);
  //Serial.print("--------------------");
  connectToWiFi();
  connectToServer();
}

void loop()
{
  if (state == STATE_IDLE) {
    //Serial.println("---IDLE state---");
    // Wait for server response to start
    //start = millis();
    while (client.connected() && !client.available()) {
      //Serial.println("wait for server response");
      delay(10);
    }
    //duration = millis() - start;
    //Serial.print("--------------------");
    //Serial.print("Wait time before start conversation with anchors: ");
    //Serial.println(duration);
    //Serial.print("--------------------");

    if (WiFi.status() == WL_CONNECTION_LOST) {
      //Serial.println("Connection with WiFi lost. Reconnecting...");
      connectToWiFi();
    }

    if (!client.connected()) {
      //Serial.println("Connection with WiFi lost. Reconnecting...");
      connectToServer();
    }

    String serverRequest = client.readStringUntil('\n');
    //Serial.print("Server Request: ");
    //Serial.println(serverRequest);


    //start = millis();
    if (serverRequest == "1") {
      //Serial.println("---Received request from server: OK. Start ranging phase---");
      state = STATE_RANGING;
    } 
    
  } else {
    DW1000Ranging.loop();
  }
  
  //DW1000Ranging.loop();
}

void newRange()
{
    //Serial.println("---RANGING state New Range---");
    //Serial.print("Distance counter range beginning: ");
    //Serial.println(distanceCounter);
//  Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress());
  //Serial.print(",");
  //Serial.println(DW1000Ranging.getDistantDevice()->getRange());
  //Serial.println(anchors.size());
  //if (DW1000Ranging.getDistantDevice()->getShortAddress() == 83) Serial.println("Equal 83");
  //else if (DW1000Ranging.getDistantDevice()->getShortAddress() == 84) Serial.println("Equal 84");
  //std::vector<Anchor>::iterator anchor = std::find_if(std::begin(anchors), std::end(anchors), [&device](const DW1000Device& _anchor){ return _anchor.anchorID == device.anchorID;});
  //if (anchor != anchors.end()) {
    
  // }
  shortAddress = DW1000Ranging.getDistantDevice()->getShortAddress();
  //Serial.print("newRange - shortAddress - ");
  //Serial.println(shortAddress);
  
  for (Anchor& anchor: anchors) {
    //Serial.println(anchor.ID);
    //Serial.println(anchor.distance);
    if (anchor.ID == shortAddress && anchor.distance == -1.0)
    {
      //Serial.println("New range added");
      anchor.distance = DW1000Ranging.getDistantDevice()->getRange();
      distanceCounter++;
      //Serial.print("Distance counter new range added: ");
      //Serial.println(distanceCounter);
      //Serial.print("Anchor distance: ");
      //Serial.println(anchor.distance);
    }
  }
  
  if (distanceCounter == minAnchorNumber && anchorCounter == minAnchorNumber) {
    String msgToSend = "";
    //duration = millis() - start;
    //Serial.print("--------------------");
    //Serial.print("Duration of Tag and Anchors communication: ");
    //Serial.println(duration);
    //Serial.print("--------------------");
    msgToSend = "{\"Tag ID\": " + String(myID) + ", \"anchors\": [";
        
    for (Anchor& anchor: anchors) {
      if (anchor.distance > 0) {
        //Serial.print("Anchor: ");
        //Serial.print(anchor.ID);
        //Serial.print(" Distance: ");
        //Serial.println(anchor.distance);
        msgToSend += "{ \"Anchor ID\": " + String(anchor.ID) + ", \"distance\": " + String(anchor.distance) + " },";
      }
      anchor.distance = -1.0;
    }
    msgToSend += "]}";
    distanceCounter = 0;

    // Send the estimated distance
    client.print(msgToSend);
    //Serial.println(msgToSend);

    // Wait for server acknowledgment
    while (client.connected() && !client.available()) {
      //Serial.print("Wait for server acknowledgment");
      delay(10);
    }

    // Read server acknowledgment
    String ack = client.readStringUntil('\n');
    //Serial.print("ACK: ");
    //Serial.println(ack);
    if (ack == "7") {
      //Serial.println("Server acknowledged distance");
    } else {
      //Serial.println("Server did not acknowledge distance");
    }
    //duration = millis() - start;
    //Serial.print("--------------------");
    //Serial.print("Duration of overall communication: ");
    //Serial.println(duration);
    //Serial.print("--------------------");
    //Serial.println("Next iteration");
    if (ack != "7") {
      //Serial.println("ACK is not 7");
      delay(1000);      
    } 
    
    
    state = STATE_IDLE;

    
  }

}

void newDevice(DW1000Device *device)
{

  //if (device->getShortAddress() == 83) Serial.println("Equal");
  //std::vector<Anchor>::iterator anchor = std::find_if(std::begin(anchors), std::end(anchors), [&device](const DW1000Device& _anchor){ return _anchor.anchorID == device.anchorID;});
  //std::vector<DW1000Device*>::iterator anchor = std::find_if(std::begin(anchors), std::end(anchors), );
//  Serial.print("Anchor: ");
//  Serial.println(anchor->anchorID);
  bool newAnchor = true;
  shortAddress = device->getShortAddress();
  
  for (Anchor anchor: anchors) {
    if (anchor.ID == shortAddress) newAnchor = false;
  }

  if (newAnchor && anchorCounter < maxAnchorNumber){
    //Serial.print("Device added: ");
    //Serial.println(shortAddress, HEX);

    //Serial.print("anchorCounter: ");
    //Serial.println(anchorCounter);

    // Find first empty index, because it could be deleted anyone from any index 
    size_t firstEmptyIndex;
    for (firstEmptyIndex = 0; firstEmptyIndex < maxAnchorNumber; firstEmptyIndex++) {
      if (anchors[firstEmptyIndex].ID == 0) break;  
    }
    anchors[firstEmptyIndex] = {shortAddress, -1.0};
    anchorCounter++;
  }
  
//  if (anchor == anchors.end()) 
//  {
    //Serial.print("Device added: ");
    //Serial.println(device->getShortAddress(), HEX);
//    anchors.push_back({device->getShortAddress(), 0});
//  }
}

void inactiveDevice(DW1000Device *device)
{
  
  //std::vector<DW1000Device*>::iterator anchor = std::find_if(std::begin(anchors), std::end(anchors), [&device](DW1000Device* _anchor){ return _anchor->getShortAddress() == device->getShortAddress();});
//  std::vector<DW1000Device*>::iterator anchor = std::find_if(std::begin(anchors), std::end(anchors), *device);
//  std::vector<Anchor>::iterator anchor = std::find_if(std::begin(anchors), std::end(anchors), [&device](const DW1000Device& _anchor){ return _anchor.anchorID == device.anchorID;});
//  if (anchor != anchors.end()) {
//    anchors.erase(anchor);
    //Serial.print("delete inactive device: ");
    //Serial.println(device->getShortAddress(), HEX);
//  } 

    shortAddress = device->getShortAddress();
    for (Anchor& anchor: anchors) {
      if (anchor.ID == shortAddress)
      {
        anchor.ID = 0;
        anchorCounter--;
      }
    }
//
//  bool anchorExists = false;
//  shortAddress = device->getShortAddress()
//  
//  for (size_t i = 0; i < anchorCounter; i++) {
//    if (anchor.ID == shortAddress) {
//      Serial.print("Anchor with index");
//      Serial.print(i);
//      Serial.println(" is removed");
//      Serial.print("ID: ");
//      Serial.println(anchor.ID);
//      anchors[i] = null;
//      anchorCounter--;
//      distanceCounter--;
//    }
//  }

}
