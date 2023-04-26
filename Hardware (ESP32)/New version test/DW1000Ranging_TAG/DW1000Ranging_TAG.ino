/**
 * 
 * @todo
 *  - move strings to flash (less RAM consumption)
 *  - fix deprecated convertation form string to char* startAsTag
 *  - give example description
 */
#include <SPI.h>
#include <WiFi.h>
#include "DW1000Ranging.h"

#define STATE_IDLE 0
#define STATE_REQUEST_ACK_FROM_SERVER 1
#define STATE_SEND_DISTANCE_TO_SERVER 2

// connection pins
const uint8_t PIN_RST = 27; // reset pin
const uint8_t PIN_IRQ = 34; // irq pin
const uint8_t PIN_SS = 4; // spi select pin

const char *ssid = "oskar-hotspot";
const char *password = "Nera1998";
const char *host = "10.42.0.1";

byte state = STATE_IDLE;
String toSend;
String ack;
String myID = "1";
WiFiClient client;

void connectToServer() {
   WiFi.mode(WIFI_STA);
   WiFi.setSleep(false);
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED)
   {
       delay(500);
       Serial.print(".");
   }
   Serial.println("Connected");
   if (client.connect(host, 30001))
   {
       Serial.println("Success");
   } else {
       Serial.println("Unsuccessfull connection");
   }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  //init the configuration
  DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ); //Reset, CS, IRQ pin
  //define the sketch as anchor. It will be great to dynamically change the type of module
  DW1000Ranging.attachNewRange(newRange);
  DW1000Ranging.attachNewDevice(newDevice);
  DW1000Ranging.attachInactiveDevice(inactiveDevice);
  //Enable the filter to smooth the distance
  //DW1000Ranging.useRangeFilter(true);
  
  //we start the module as a tag
  DW1000Ranging.startAsTag("7D:00:22:EA:82:60:3B:9C", DW1000.MODE_LONGDATA_RANGE_ACCURACY);
  connectToServer();
  
}

void loop() {
  
  if (state == STATE_IDLE) {
    Serial.println("Server request");
    if (client.connected()) { 
      client.print(String(myID) + "3");
      state = STATE_REQUEST_ACK_FROM_SERVER;
    } else {
      Serial.println("Server is not available.");
      connectToServer();
      state = STATE_IDLE;
    }
  }

  if (state == STATE_REQUEST_ACK_FROM_SERVER) {
    ack = "";
    delay(20);
    if (client.available()) ack = client.read();
    Serial.print("ACK: ");
    Serial.println(ack);
    if (ack == "51") {
      Serial.println("Server response - ACCEPTED. Start SCAN state");
      state = STATE_SEND_DISTANCE_TO_SERVER;
    }
    else {
      Serial.println("Server does't respond. Probably connection lost.");
      ack = "";
      state = STATE_IDLE;
    }
  }

  if (state == STATE_SEND_DISTANCE_TO_SERVER) {
    DW1000Ranging.loop();
    Serial.println(toSend);
    client.print(toSend);
    ack = "";
    delay(20);
    if (client.available()) ack = client.read();
    Serial.print("ACK: ");
    Serial.println(ack);
    if (ack == "49") {
      Serial.println("  UWB package sent");
      ack = "";
    } else {
      Serial.println("Server does't respond. Data was not send.");
      ack = "";
    }
    state = STATE_IDLE;
  }
  
  
}

void newRange() {
  //Serial.print("from: "); Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress());
 // Serial.print("\t Range: "); Serial.print(DW1000Ranging.getDistantDevice()->getRange()); Serial.print(" m");
  //Serial.print("\t RX power: "); Serial.print(DW1000Ranging.getDistantDevice()->getRXPower()); Serial.println(" dBm");

  toSend = "from: " + String(DW1000Ranging.getDistantDevice()->getShortAddress()) + " Range: " + String(DW1000Ranging.getDistantDevice()->getRange());
  //Serial.println(toSend);
  //client.print(toSend);
}

void newDevice(DW1000Device* device) {
  Serial.print("ranging init; 1 device added ! -> ");
  Serial.print(" short:");
  Serial.println(device->getShortAddress(), HEX);
}

void inactiveDevice(DW1000Device* device) {
  Serial.print("delete inactive device: ");
  Serial.println(device->getShortAddress(), HEX);
}
