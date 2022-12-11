/*

For ESP32 UWB or ESP32 UWB Pro

*/

#include <SPI.h>
#include <DW1000Ranging.h>
#include <WiFi.h>
#include "link.h"

#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23
#define DW_CS 4
#define PIN_RST 27
#define PIN_IRQ 34

#define STATE_IDLE 0
#define STATE_REQUEST_ACK_FROM_SERVER 1
#define STATE_CALCULATE_DISTANCE 2
#define STATE_SEND_DISTANCE_TO_SERVER 3

//const char *ssid = "ASUS";
//const char *password = "cuni3103&";
//const char *host = "192.168.1.55";
const char *ssid = "oskar-hotspot";
const char *password = "Nera1998";
const char *host = "10.42.0.1";
WiFiClient client;

byte state = STATE_IDLE;

struct MyLink *uwb_data;
int index_num = 0;
long runtime = 0;
String myID;
String all_json = "";
String clientRequest = "1";
String msgToSend;
char serverGetDistanceAck = '2';
char serverReceivedDistanceAck = '3';
char ack;
uint8_t rangeCounter;
String stringAck;
//uint8_t idx = 0;

void setAddress() {
    String macAddr = WiFi.macAddress();
    if (macAddr == "70:B8:F6:D8:F6:48") {
        myID = "1";
    } else if (macAddr == "70:B8:F6:D8:F6:24") {
        myID = "2";
    } else if (macAddr == "70:B8:F6:D8:F6:60") {
        myID = "3";
    } else {
        Serial.println("Wrong tag MAC address.");
    }
}

void connectToServer() {
   Serial.begin(115200);
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

void setup()
{
    connectToServer();

    //init the configuration
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    DW1000Ranging.initCommunication(PIN_RST, DW_CS, PIN_IRQ);
    DW1000Ranging.attachNewRange(newRange);
    DW1000Ranging.attachNewDevice(newDevice);
    DW1000Ranging.attachInactiveDevice(inactiveDevice);

    //we start the module as a tag
    DW1000Ranging.startAsTag("7D:00:22:EA:82:60:3B:9C", DW1000.MODE_LONGDATA_RANGE_ACCURACY);

    setAddress();

    uwb_data = init_link();
}

void loop()
{   

  if (state == STATE_IDLE) {
    Serial.println("Server request");
    if (client.connected()) { 
      client.print(String(myID) + "1");
      state = STATE_REQUEST_ACK_FROM_SERVER;
    } else {
      Serial.println("Server is not available.");
      connectToServer();
      state = STATE_IDLE;
    }
  }

  if (state == STATE_REQUEST_ACK_FROM_SERVER) {
    Serial.println("in request ack from server server state");
    ack = '\0';
    delay(20);
    if (client.available()) ack = client.read();
    Serial.println("ACK: ");
    Serial.println(ack);
    if (ack == '3') {
      Serial.println("Server response - ACCEPTED. Start SCAN state");
      runtime = millis();
      state = STATE_CALCULATE_DISTANCE;
    }
    else {
      Serial.println("Server does't respond. Probably connection lost.");
      ack = '\0';
      state = STATE_IDLE;
    }
  }

  if (state == STATE_SEND_DISTANCE_TO_SERVER) {
    //delay(100);
    //make_link_json(uwb_data, &all_json);
    Serial.println("in send distance to server state");
    //Serial.println(all_json);
    //send_udp(&all_json);
    ack = '\0';
    delay(20);
    if (client.available()) ack = client.read();
      
    if (ack == '5') {
      //toSend = false;
      Serial.println("  UWB package sent");
      ack = '\0';
    } else {
      Serial.println("Server does't respond. Data was not send.");
      ack = '\0';
    }
    //delay(1000);
    state = STATE_IDLE;
  }

    if (state == STATE_CALCULATE_DISTANCE) {
        //Serial.println("In loop. Calculating distances...");
        DW1000Ranging.loop();
        //rangeCounter++;
        //Serial.print("rangeCounter: "); Serial.println(rangeCounter);
        if (millis() - runtime > 300) {
          make_link_json(myID, uwb_data, &all_json);
          send_udp(&all_json);
          state = STATE_SEND_DISTANCE_TO_SERVER;
          runtime = 0;
        }
        
     }




  
//    if (state == STATE_IDLE) {
//
//        if (client.connected()) {
//            msgToSend = myID + clientRequest;
//            client.print(msgToSend);
//            ack = '\0';
////            idx = 0;/
//            if (client.available()) ack = client.read();
//            stringAck = ack;
//            Serial.print("Ack: "); Serial.println(stringAck); 
//            if (stringAck == myID) {
//                runtime = millis();
//                state = STATE_CALCULATE_DISTANCE;
//            } else {
//              Serial.print("Server is busy with TAG: "); Serial.println(stringAck);
//              delay(100);
//            }
//        } else {
//            Serial.println("Server is not connected. Reconnecting...");
//            connectToServer();
//            // state = STATE_IDLE;
//        }
//    }




//       if (state == STATE_IDLE) {
//        if (client.connected()) {
//            msgToSend = myID + clientRequest;
//            client.print(msgToSend);
//            delay(20);
//            state = STATE_SERVER_ACK;
//        } else {
//            Serial.println("Server is not connected. Reconnecting...");
//            connectToServer();
//            // state = STATE_IDLE;
//        }
//
//    } else if (state == STATE_SERVER_ACK) {
//        //if (client.connected()) {
//          Serial.println("In while. Waiting for server response...");
//          ack = '\0';
//          if (client.available()) {
//              ack = client.read();
//          }
//          Serial.print("Ack: "); Serial.println(ack); 
//          if (ack == serverGetDistanceAck) {
//              state = STATE_CALCULATE_DISTANCE;
//              runtime = millis();
//          } else if (ack == serverReceivedDistanceAck){
//              Serial.println("Server received distance. Return to IDLE");
//              state = STATE_IDLE;
//          } else {
//              Serial.println("Strange ack. Return to IDLE");
//              state = STATE_IDLE;
//          }
//              // break;
//        //} else {
//         // state = STATE_IDLE;
//        //}
//    } else if (state == STATE_CALCULATE_DISTANCE) {
//        Serial.println("In loop. Calculating distances...");
//        DW1000Ranging.loop();
//        if ((millis() - runtime) > 100)
//        {
//            make_link_json(myID, uwb_data, &all_json);
//            if (client.connected()) client.print(all_json);
//            delay(20);
//            state = STATE_SERVER_ACK;
//        }
//    }

}

void newRange()
{
    char c[30];

//    Serial.print("from: ");
//    Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
//    Serial.print("\t Range: ");
//    Serial.print(DW1000Ranging.getDistantDevice()->getRange());
//    Serial.print(" m");
//    Serial.print("\t RX power: ");
//    Serial.print(DW1000Ranging.getDistantDevice()->getRXPower());
//    Serial.println(" dBm");
    fresh_link(uwb_data, DW1000Ranging.getDistantDevice()->getShortAddress(), DW1000Ranging.getDistantDevice()->getRange(), DW1000Ranging.getDistantDevice()->getRXPower());
    rangeCounter++;
    //Serial.print("rangeCounter: "); Serial.println(rangeCounter);
    if (rangeCounter > 4) {
      make_link_json(myID, uwb_data, &all_json);
      send_udp(&all_json);
      state = STATE_SEND_DISTANCE_TO_SERVER;
      rangeCounter = 0;
    }
    
}

void newDevice(DW1000Device *device)
{
    Serial.print("ranging init; 1 device added ! -> ");
    Serial.print(" short:");
    Serial.println(device->getShortAddress(), HEX);

    add_link(uwb_data, device->getShortAddress());
}

void inactiveDevice(DW1000Device *device)
{
    Serial.print("delete inactive device: ");
    Serial.println(device->getShortAddress(), HEX);

    delete_link(uwb_data, device->getShortAddress());
}

void send_udp(String *msg_json)
{
    if (client.connected())
    {
        client.print(*msg_json);
        Serial.println("UDP send");
    }
}
