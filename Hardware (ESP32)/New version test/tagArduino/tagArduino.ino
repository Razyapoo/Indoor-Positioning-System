#include "arduino.h" 
 
byte getMyID() { 
    String macAddr = WiFi.macAddress();
    if (macAddr == "70:B8:F6:D8:F6:48") {
        return 1;
    } else if (macAddr == "70:B8:F6:D8:F6:24") {
        return 2;
    } else if (macAddr == "70:B8:F6:D8:F6:60") {
        return 3;
    } else {
        Serial.println("Wrong tag MAC address.");
        return 0;
    }
} 

void checkForReset() {
  currentTime = millis();
  if(!sentAck && !receivedAck) {
    if(currentTime - lastActivity > DEFAULT_RESET_TIMEOUT) {
      initReceiver();
      Serial.println("Reinit....");
    }
    return;
  }
}

void initReceiver() {
  DW1000.newReceive();
  DW1000.setDefaults();
  DW1000.receivePermanently(true);
  DW1000.startReceive();
}

void connectToWiFi() {
   WiFi.mode(WIFI_MODE_STA);
   WiFi.setSleep(false);
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED)
   {
      delay(300);
      Serial.print('.');
   }
    Serial.println("Connected to WiFi"); 
}

void connectToServer() {
   if (client.connect(host, 30001))
   {
      Serial.println("Connected to server"); 
   } else {
      Serial.println("Failed to connect to servet"); 
   }
}

bool checkAnchorAddress() {
  Serial.print("Anchor Address: ");
  Serial.println(message[1]);
  if (100 < anchorID && anchorID < 199) {
    return true;
  } else {
    Serial.println("  Wrong Anchor Address!!");
  }
    // return (0 < addr && addr < 99) ? true : false; 
}

bool checkSource() {
  if (checkAnchorAddress()) {
    if (currentAnchorAddress == message[1]) 
    {
      return true;
    }
    Serial.println("Wrong source!");
    Serial.print("Current anchor is: ");
    Serial.println(currentAnchorAddress);
  }
  return false;
}

bool checkDestination() {
  if (myID == message[2]) 
  {
    return true;
  }
  Serial.println("Wrong destination. Not for me!");
  return false;
}

bool checkAddresses() {
  return checkSource() && checkDestination();
}


float computeRangeAsymmetric() {

    DW1000Time round1 = (timePollAckReceived - timePollSent).wrap();
    DW1000Time reply1 = (timePollAckSent - timePollReceived).wrap();
    DW1000Time round2 = (timeRangeReceived - timePollAckSent).wrap();
    DW1000Time reply2 = (timeRangeSent - timePollAckReceived).wrap();
    DW1000Time tof = (round1 * round2 - reply1 * reply2)
                      / (round1 + round2 + reply1 + reply2);
    return tof.getAsMeters();
}
 
bool isExpectedReceivedMessageType (byte messageType) { 
    return (messageType == MSG_TYPE_ANCHOR_ADDR || messageType == MSG_TYPE_POLL_ACK || messageType == MSG_TYPE_RANGE_REPORT) ? true : false; 
} 
 
void prepareMessageToSend(byte messageType, byte source, byte destination) { 
    memcpy(message, &messageType, sizeof(messageType)); 
    memcpy(message + 1, &source, sizeof(source)); 
    memcpy(message + 2, &destination, sizeof(destination)); 
} 

void prepareMessageToSend(byte messageType, byte source) { 
    memcpy(message, &messageType, sizeof(messageType)); 
    memcpy(message + 1, &source, sizeof(source));
} 

void handleReceived() {
  receivedAck = true;
}

void handleSent() {
  sentAck = true;
}

void sendMessage(byte messageType) { 
  Serial.println("---Send Message---");
  DW1000.newTransmit(); 
  DW1000.setDefaults(); 
  if (messageType == MSG_TYPE_BLINK) { 
    Serial.print("  MSG_TYPE_BLINK: ");
    Serial.println(MSG_TYPE_BLINK);
    prepareMessageToSend(MSG_TYPE_BLINK, myID); 
  }  
  else if (messageType == MSG_TYPE_POLL) { 
    Serial.print("  MSG_TYPE_POLL: ");
    Serial.println(MSG_TYPE_POLL);
    prepareMessageToSend(MSG_TYPE_POLL, myID, currentAnchorAddress); 
  }  
  else if (messageType == MSG_TYPE_RANGE) { 
    Serial.print("  MSG_TYPE_RANGE: ");
    Serial.println(MSG_TYPE_RANGE);
    prepareMessageToSend(MSG_TYPE_RANGE, myID, currentAnchorAddress); 
    DW1000.setDelay(rangeReplyDelay);
  }  
 
  DW1000.setData(message, sizeof(message));
  Serial.print("  Message content: ");
  for (size_t i = 0; i < sizeof(message); i++) {
    Serial.print(message[i]);
    Serial.print(" ");
  }
  Serial.println(" ");
   
  DW1000.startTransmit(); 
  while (!DW1000.isTransmitDone()) continue; 
 
  DW1000.clearTransmitStatus();
  // lastActivity = millis(); 
} 
 
// bool messageReceived(byte expectedMessageType) { 
//   Serial.println("---Receive Message---");
//   //DW1000.newReceive();  
//   //DW1000.setDefaults();
//   //DW1000.startReceive(); 
 
//   //while (!DW1000.isReceiveDone() && !DW1000.isReceiveTimeout()); 
//   //if (DW1000.isReceiveTimeout()) { 
//     //DW1000.clearReceiveTimeout(); 
//    // Serial.println("  Receive Timeout");
//    // return false; 
//   //} 
   
//   DW1000.getData(message, sizeof(message)); 
//   Serial.print("  Message content: ");
//   for (size_t i = 0; i < sizeof(message); i++) {
//     Serial.print(message[i]);
//     Serial.print(" ");
//   }
//   Serial.println(" ");
  
//   byte anchorAddress = message[1];
//   if (!checkAnchorAddress(anchorAddress)) {
//     Serial.println("  Wrong Anchor Address!");
//     return false; 
//   }

//   if (message[0] == MSG_TYPE_ANCHOR_ADDR && expectedMessageType == MSG_TYPE_ANCHOR_ADDR) { 
//     currentAnchorAddress = message[1]; 
//   } 

//   if (message[0] == expectedMessageType && currentAnchorAddress == anchorAddress) { 
//     Serial.print("  message: ");
//     Serial.println(expectedMessageType);
//     DW1000.clearReceiveStatus(); 
//     return true; 
//   } 

//   Serial.println("!!!Received Wrong Message!!!");
//   return false; 

// } 

bool isAnchorDiscovered(byte anchorAddress) {
  for (size_t i = 0; i < discoveredAnchorsCount; i++) {
    if (discoveredAnchors[i] == anchorAddress) {
      return true;
    }
  }
  return false;
}

void discoverAnchors() {
  while (discoveredAnchorsCount < MIN_ANCHORS) {
    sendMessage(MSG_TYPE_BLINK);
    if (messageReceived(MSG_TYPE_ANCHOR_ADDR)) {
      if (!isAnchorDiscovered(currentAnchorAddress)) {
        Serial.print("New anchor found: ");
        Serial.println(currentAnchorAddress);
        discoveredAnchors[discoveredAnchorsCount++] = currentAnchorAddress;
      }
    }
  }
}

 
void setup() { 
  Serial.begin(115200); 
 
  DW1000.begin(PIN_IRQ, PIN_RST); 
  DW1000.select(PIN_SS); 
 
  myID = getMyID(); 
 
  DW1000.newConfiguration(); 
  DW1000.setDefaults(); 
  DW1000.setNetworkId(networkId); 
  DW1000.setDeviceAddress(myID); 
  DW1000.enableMode(DW1000.MODE_LONGDATA_RANGE_ACCURACY); 
  DW1000.commitConfiguration(); 

  DW1000.attachSentHandler(handleSent);
  DW1000.attachReceivedHandler(handleReceived);
  
  // DW1000.newReceive();
  // DW1000.setDefaults();
  // DW1000.receivePermanently(true);
  // DW1000.startReceive();
  initReceiver();
  // initReceiver();
  lastActivity = millis();
  blinkTimer = millis();

  Serial.println("TAG");
} 
 
void loop() { 
//  /if (discoveredAnchorsCount < MIN_ANCHORS) discoverAnchors();
  
  checkForReset();

  if (!isTagBusy && discoveredAnchorsCount < MIN_ANCHORS && millis() - blinkTimer > BLINK_DELAY) {
    blinkTimer = millis();
    sendMessage(MSG_TYPE_BLINK);
    expectedMessageType = MSG_TYPE_ANCHOR_ADDR;
  }
     
  if (sentAck) {
    sentAck = false;
    Serial.println("Sent ACK");
    lastActivity = millis();
    lastSent = lastActivity;

    if (message[0] == MSG_TYPE_POLL) { 
      Serial.println("  MSG_TYPE_POLL is sent. Record timestamp");
      DW1000.getTransmitTimestamp(timePollSent);
    }  
    
    if (message[0] == MSG_TYPE_RANGE) { 
      Serial.println("  MSG_TYPE_RANGE is sent. Record timestamp");
      DW1000.getTransmitTimestamp(timeRangeSent);
    }  
  }

  if (receivedAck) {
    receivedAck = false;
    lastActivity = millis();

    if (expectedMessageType == message[0]) {
      if (expectedMessageType == MSG_TYPE_ANCHOR_ADDR && !isTagBusy) {
        if (!isAnchorDiscovered(currentAnchorAddress)) {
          Serial.print("New anchor found: ");
          Serial.println(currentAnchorAddress);
          discoveredAnchors[discoveredAnchorsCount++] = currentAnchorAddress;
        }

        if (discoveredAnchorsCount >= MIN_ANCHORS) {
          isTagBusy = true;
          currentAnchorAddress = discoveredAnchors[anchorIndex];
          sendMessage(MSG_TYPE_POLL);
          expectedMessageType = MSG_TYPE_POLL_ACK;
        }
      } else {
        if (isTagBusy) {
          if (expectedMessageType == MSG_TYPE_POLL_ACK && checkAddresses()) {
            DW1000.getReceiveTimestamp(timePollAckReceived);
            sendMessage(MSG_TYPE_RANGE); 
            expectedMessageType = MSG_TYPE_RANGE_REPORT;
          }

          if (expectedMessageType == MSG_TYPE_RANGE_REPORT && checkAddresses()) {
            timePollReceived.setTimestamp(message + 5);
            timePollAckSent.setTimestamp(message + 10);
            timeRangeReceived.setTimestamp(message + 15);
            float distance = computeRangeAsymmetric();
            Serial.print('Distance from anchor ');
            Serial.print(currentAnchorAddress);
            Serial.print(': ');
            Serial.print(distance);
            if (anchorIndex < discoveredAnchorsCount) 
            {
              anchorIndex++;
            } else {
              discoveredAnchorsCount = 0;
            }
            isTagBusy = false;
          }
        } else {
          Serial.println("Tag lost connection with Anchor!!");
        }
      }

    }

    // if (!isTagBusy && messageReceived(MSG_TYPE_ANCHOR_ADDR)) {
      
      
    // } else if (isTagBusy && messageReceived(MSG_TYPE_POLL_ACK)) { 
    //   DW1000.getReceiveTimestamp(timePollAckReceived);
    //   sendMessage(MSG_TYPE_RANGE); 
    // } else if (isTagBusy && messageReceived(MSG_TYPE_RANGE_REPORT)) { 
    //   timePollReceived.setTimestamp(message + 5);
    //   timePollAckSent.setTimestamp(message + 10);
    //   timeRangeReceived.setTimestamp(message + 15);
    //   float distance = computeRangeAsymmetric();
    //   Serial.print('Distance from anchor ');
    //   Serial.print(currentAnchorAddress);
    //   Serial.print(': ');
    //   Serial.print(distance);
    //   if (anchorIndex < discoveredAnchorsCount) 
    //   {
    //     anchorIndex++;
    //   } else {
    //     discoveredAnchorsCount = 0;
    //   }
    //   isTagBusy = false;
    // } 

  }

  // for (size_t anchorIndex = 0; anchorIndex < discoveredAnchorsCount; anchorIndex++) {
    // currentAnchorAddress = discoveredAnchors[anchorIndex];

  // }
  
}
