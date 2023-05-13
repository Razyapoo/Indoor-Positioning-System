#include "arduino.h" 
 
uint16_t getMyID() { 
    String macAddr = WiFi.macAddress();
    if (macAddr == "70:B8:F6:D8:F6:48") {
        return 1;
    } else if (macAddr == "70:B8:F6:D8:F6:24") {
        return 2;
    } else if (macAddr == "70:B8:F6:D8:F6:60") {
        return 3;
    } else {
        if (debug) Serial.println("Wrong tag MAC address.");
        return 0;
    }
} 

void checkForReset() {
  currentTime = millis();
  if ((!sentAck && !receivedAck) || errorWatch) {
    if ((currentTime - lastActivity > DEFAULT_RESET_TIMEOUT) || errorWatch) {
      discoveredAnchorsCount = 0;
      anchorIndex = 0;
      counterBlink = 0;
      isTagBusy = false;
      initReceiver();
      Serial.println("Reinit....");
      noteActivity();
      blinkTimer = millis();
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
   if (debug) WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED)
   {
      delay(300);
      if (debug) Serial.print('.');
   }
    if (debug) Serial.println("Connected to WiFi"); 
}

void connectToServer() {
   if (client.connect(host, 30001))
   {
      if (debug) Serial.println("Connected to server"); 
   } else {
      if (debug) Serial.println("Failed to connect to servet"); 
   }
}

bool isAnchorAddress() {
  if (debug) {
    Serial.print("Anchor Address: ");
    Serial.println();
  }
 
  if (100 < message[1] && message[1] < 199) {
    return true;
  } else {
    if (debug) Serial.println("  Wrong Anchor Address!! Must be in range (100, 199)");
  }
}

bool checkSource() {
  if (isAnchorAddress() && currentAnchorAddress == message[1]) return true;  
  if (debug) {
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
  if (debug)Serial.println("Wrong destination. Not for me!");
  return false;
}

bool checkSourceAndDestination() {
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

void noteActivity() {
  lastActivity = millis();
}

void sendMessage(byte messageType) { 
  if (debug)Serial.println("---Send Message---");
  DW1000.newTransmit(); 
  DW1000.setDefaults(); 
  if (messageType == MSG_TYPE_BLINK) { 
    if (debug) {
      Serial.print("  MSG_TYPE_BLINK: ");
      Serial.println(MSG_TYPE_BLINK);
    }
    
    prepareMessageToSend(MSG_TYPE_BLINK, myID); 
  }  
  else if (messageType == MSG_TYPE_POLL) { 
    
    if (debug) {
      Serial.print("  MSG_TYPE_POLL: ");
      Serial.println(MSG_TYPE_POLL);
    }
    
    prepareMessageToSend(MSG_TYPE_POLL, myID, currentAnchorAddress); 
  }  
  else if (messageType == MSG_TYPE_RANGE) { 
    if (debug) {
      Serial.print("  MSG_TYPE_RANGE: ");
      Serial.println(MSG_TYPE_RANGE);
    }
    
    prepareMessageToSend(MSG_TYPE_RANGE, myID, currentAnchorAddress); 
    DW1000.setDelay(rangeReplyDelay);
  }  
 
  DW1000.setData(message, sizeof(message));
  
  if (debug) 
  { 
    Serial.print("  Message content: ");
    for (size_t i = 0; i < sizeof(message); i++) {
      Serial.print(message[i]);
      Serial.print(" ");
    }
    Serial.println(" ");
  }
  
   
  DW1000.startTransmit(); 
  //while (!DW1000.isTransmitDone()) continue; 
 
  //DW1000.clearTransmitStatus();
  // lastActivity = millis(); 
} 


bool isAnchorDiscovered() {
  for (size_t i = 0; i < discoveredAnchorsCount; i++) {
    if (discoveredAnchors[i] == message[1]) {
      return true;
    }
  }
  return false;
}

void initTag() {
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
 
  initReceiver();

  blinkTimer = millis();

  Serial.println("TAG");
  delay(1000);
}

void setup() { 
  Serial.begin(115200); 
 
 initTag();
} 
 
void loop() { 
  
  checkForReset();

  if (debug) 
  {
    Serial.print("Current Anchor Address: "); 
    Serial.println(currentAnchorAddress);
  }

  
     
  if (sentAck) {
    sentAck = false;
   if (debug) Serial.println("Sent ACK");

    if (message[0] == MSG_TYPE_POLL) { 
      if (debug) Serial.println("  MSG_TYPE_POLL is sent. Record timestamp");
      DW1000.getTransmitTimestamp(timePollSent);
    }  

    if (message[0] == MSG_TYPE_RANGE) { 
      if (debug) Serial.println("  MSG_TYPE_POLL is sent. Record timestamp");
      DW1000.getTransmitTimestamp(timeRangeSent);
    }  
    
    noteActivity();
  }

  if (receivedAck) {
   if (debug) Serial.println("Received something");
    receivedAck = false;
    noteActivity();

    DW1000.getData(message, sizeof(message));

    if (debug) 
    {
      Serial.print("  Message content: ");
      for (size_t i = 0; i < sizeof(message); i++) {
        Serial.print(message[i]);
        Serial.print(" ");
      }
      Serial.println(" ");
    }
   
    
    if (expectedMessageType == message[0]) {
      if (expectedMessageType == MSG_TYPE_ANCHOR_ADDR && !isTagBusy && checkDestination()) {
        if (!isAnchorDiscovered() && isAnchorAddress()) {
         if (debug) {
          Serial.print("New anchor found: ");
          Serial.println(message[1]);
         }
         
          discoveredAnchors[discoveredAnchorsCount++] = message[1];
          //counterBlink = 0;
        }

        if (discoveredAnchorsCount >= MIN_ANCHORS) {
          isTagBusy = true;
          currentAnchorAddress = discoveredAnchors[anchorIndex];
          expectedMessageType = MSG_TYPE_POLL_ACK;
          sendMessage(MSG_TYPE_POLL);
        }
        return;
      } else {
        if (isTagBusy) {
          if (expectedMessageType == MSG_TYPE_POLL_ACK && checkSourceAndDestination()) {
           if (debug) Serial.println("received POLL ACK!");
            DW1000.getReceiveTimestamp(timePollAckReceived);
            expectedMessageType = MSG_TYPE_RANGE_REPORT;
            sendMessage(MSG_TYPE_RANGE); 
            return;
          }

          if (expectedMessageType == MSG_TYPE_RANGE_REPORT && checkSourceAndDestination()) {
            timePollReceived.setTimestamp(message + 3);
            timePollAckSent.setTimestamp(message + 8);
            timeRangeReceived.setTimestamp(message + 13);
            float distance = computeRangeAsymmetric();
            Serial.print("Distance from anchor ");
            Serial.print(currentAnchorAddress);
            Serial.print(": ");
            Serial.println(distance);
            if (anchorIndex < discoveredAnchorsCount - 1) 
            {
              anchorIndex++;
              isTagBusy = true;
              currentAnchorAddress = discoveredAnchors[anchorIndex];
              expectedMessageType = MSG_TYPE_POLL_ACK;
              sendMessage(MSG_TYPE_POLL);
            } else {
              discoveredAnchorsCount = 0;
              anchorIndex = 0;
              isTagBusy = false;
            }
            return;
          }
        } else {
         if (debug) Serial.println("Tag lost connection with Anchor!!");
        }
      }

    } else {
      
      if (debug) 
      {
        Serial.print("Received wrong message: ");
        Serial.println(message[0]);
        Serial.print("Expected: ");
        Serial.print(expectedMessageType);

      }
      
    }
  }

  if (!isTagBusy && discoveredAnchorsCount < MIN_ANCHORS && millis() - blinkTimer > BLINK_DELAY) {
    if (debug) 
      {
        Serial.print("Number of already discovered anchors: "); 
        Serial.println(discoveredAnchorsCount);
      } 
    blinkTimer = millis();
    expectedMessageType = MSG_TYPE_ANCHOR_ADDR;
    sendMessage(MSG_TYPE_BLINK);
//    counterBlink++;
    if (debug) 
      {
        Serial.print("Blink counter: "); 
        Serial.println(counterBlink);
      } 
    // Sometimes tag is looping infinitely and sending blinks, which are not received by anchor for some reason
//    if (counterBlink > 5) {
//      
//      errorWatch = true;
//      delay(1000);
//    }
  }
}
