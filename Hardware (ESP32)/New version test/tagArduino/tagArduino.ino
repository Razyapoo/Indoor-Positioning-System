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
  if (!sentAck && !receivedAck) {
    if (currentTime - lastActivity > DEFAULT_RESET_TIMEOUT) {
      for (size_t i = 0; i < MAX_ANCHORS - 1; i++) discoveredAnchors[i] = 0;
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
 
  if (100 < receivedMessage[1] && receivedMessage[1] < 199) {
    return true;
  } else {
    if (debug) Serial.println("  Wrong Anchor Address!! Must be in range (100, 199)");
  }
}

bool checkSource() {
  if (isAnchorAddress() && currentAnchorAddress == receivedMessage[1]) return true;  
  if (debug) {
    Serial.println("Wrong source!");
    Serial.print("Current anchor is: ");
    Serial.println(currentAnchorAddress);
  }
 

  return false;
}

bool checkDestination() {
  if (myID == receivedMessage[2]) 
  {
    if (debug)Serial.println("Destination is correct. It is for me!");
    return true;
  }
  if (debug) Serial.println("Wrong destination. Not for me!");
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
    memcpy(currentMessage, &messageType, sizeof(messageType)); 
    memcpy(currentMessage + 1, &source, sizeof(source)); 
    memcpy(currentMessage + 2, &destination, sizeof(destination)); 
} 

void prepareMessageToSend(byte messageType, byte source) { 
    memcpy(currentMessage, &messageType, sizeof(messageType)); 
    memcpy(currentMessage + 1, &source, sizeof(source));
} 

void setReplyDelay() {
  memcpy(&replyDelay, receivedMessage + 3, 2);
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
  // if (messageType == MSG_TYPE_BLINK) { 
  //   if (debug) {
  //     Serial.print("  MSG_TYPE_BLINK: ");
  //     Serial.println(MSG_TYPE_BLINK);
  //   }
    
  //   prepareMessageToSend(MSG_TYPE_BLINK, myID); 
  // }  
  // else 
  if (messageType == MSG_TYPE_POLL) { 
    
    if (debug) {
      Serial.print("  MSG_TYPE_POLL: ");
      Serial.println(MSG_TYPE_POLL);
    }
    
    prepareMessageToSend(MSG_TYPE_POLL, myID); 
  }  
  else if (messageType == MSG_TYPE_RANGE) { 
    if (debug) {
      Serial.print("  MSG_TYPE_RANGE: ");
      Serial.println(MSG_TYPE_RANGE);
    }
    
    prepareMessageToSend(MSG_TYPE_RANGE, myID, currentAnchorAddress);
    setReplyDelay();
    if (debug) {
      Serial.print("Reply Delay: ");
      Serial.println(replyDelay);
    }
    rangeReplyDelay = DW1000Time(replyDelay, DW1000Time::MICROSECONDS);
    DW1000.setDelay(rangeReplyDelay);
  }  
 
  DW1000.setData(currentMessage, sizeof(currentMessage));
  
  if (debug) 
  { 
    Serial.print(" Current Message content: ");
    for (size_t i = 0; i < sizeof(currentMessage); i++) {
      Serial.print(currentMessage[i]);
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
  for (size_t i = 0; i < MAX_ANCHORS - 1; i++) {
    if (discoveredAnchors[i] == receivedMessage[1]) {
      if (debug) {
        Serial.println("Anchor is already discovered!");
        Serial.print("i = ");
        Serial.println(i);
        Serial.print("anchor address: ");
        Serial.println(receivedMessage[1]);
        Serial.print("discoveredAnchorsCount: ");
        Serial.println(discoveredAnchorsCount);
      }
      return true;
    }
  }
  if (debug) Serial.println("Anchor is not yet discovered!");
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
  
  //checkForReset();

//  if (debug) 
//  {
//    Serial.print("Current Anchor Address: "); 
//    Serial.println(currentAnchorAddress);
//  }

  
     
  if (sentAck) {
    sentAck = false;
   if (debug) Serial.println("Sent ACK");

    if (currentMessage[0] == MSG_TYPE_POLL) { 
      if (debug) Serial.println("  MSG_TYPE_POLL is sent. Record timestamp");
      DW1000.getTransmitTimestamp(timePollSent);
    }  

    if (currentMessage[0] == MSG_TYPE_RANGE) { 
      if (debug) Serial.println("  MSG_TYPE_RANGE is sent. Record timestamp");
      DW1000.getTransmitTimestamp(timeRangeSent);
    }  
    
    noteActivity();
  }

  if (receivedAck) {
   if (debug) Serial.println("Received something");
    receivedAck = false;
    noteActivity();

    DW1000.getData(receivedMessage, sizeof(receivedMessage));

    if (debug) 
    {
      Serial.print("  Message content: ");
      for (size_t i = 0; i < sizeof(receivedMessage); i++) {
        Serial.print(receivedMessage[i]);
        Serial.print(" ");
      }
      Serial.println(" ");
    }
   
    
    if (expectedMessageType == receivedMessage[0]) {
        if (isTagBusy) {
          if (expectedMessageType == MSG_TYPE_POLL_ACK && checkDestination()) {
            if (!isAnchorDiscovered() && isAnchorAddress()) {  
              if (debug) Serial.println("received POLL ACK!");
              //memcpy(currentMessage, receivedMessage, sizeof( receivedMessage));
              discoveredAnchors[discoveredAnchorsCount++] = receivedMessage[1];
              currentAnchorAddress = receivedMessage[1];
              DW1000.getReceiveTimestamp(timePollAckReceived);
              expectedMessageType = MSG_TYPE_RANGE_REPORT;
              sendMessage(MSG_TYPE_RANGE); 
            }
            return;
          }
        
          if (expectedMessageType == MSG_TYPE_RANGE_REPORT && checkSourceAndDestination()) {
            //memcpy(currentMessage, receivedMessage, sizeof( receivedMessage));
            timePollReceived.setTimestamp(receivedMessage + 5);
            timePollAckSent.setTimestamp(receivedMessage + 10);
            timeRangeReceived.setTimestamp(receivedMessage + 15);
            float distance = computeRangeAsymmetric();
            Serial.print("Distance from anchor ");
            Serial.print(currentAnchorAddress);
            Serial.print(": ");
            Serial.println(distance);
            isTagBusy = false;
            // if (anchorIndex < discoveredAnchorsCount - 1) 
            // {
            //   anchorIndex++;
            //   isTagBusy = true;
            //   currentAnchorAddress = discoveredAnchors[anchorIndex];
            //   expectedMessageType = MSG_TYPE_POLL_ACK;
            //   sendMessage(MSG_TYPE_POLL);
            // } else {
            //   discoveredAnchorsCount = 0;
            //   currentAnchorAddress = 0;
            //   anchorIndex = 0;
            //   isTagBusy = false;
            // }
            // return;
            if (discoveredAnchorsCount < MIN_ANCHORS) {
              currentAnchorAddress = 0;
              isTagBusy = true;
              expectedMessageType = MSG_TYPE_POLL_ACK;
              sendMessage(MSG_TYPE_POLL);
            }
            return;
          }
        } else {
         if (debug) Serial.println("Tag lost connection with Anchor!!");
        }
      // }

    } else {
      
      if (debug) 
      {
        Serial.print("Received wrong message: ");
        Serial.println(receivedMessage[0]);
        Serial.print("Expected: ");
        Serial.print(expectedMessageType);

      }
      
    }
  }

  // blinkCurrentMillis = millis();
  if (!isTagBusy && discoveredAnchorsCount >= MIN_ANCHORS) {
    if (debug) Serial.println("All anchors are discovered! Re-submitting poll"); 
    for (size_t i = 0; i < MAX_ANCHORS - 1; i++) discoveredAnchors[i] = 0;
    discoveredAnchorsCount = 0;
    blinkTimer = millis();
    currentAnchorAddress = 0;
    isTagBusy = true;
    expectedMessageType = MSG_TYPE_POLL_ACK;
    sendMessage(MSG_TYPE_POLL);
    return;
  }

  blinkCurrentMillis = millis();
  if (!isTagBusy && discoveredAnchorsCount < MIN_ANCHORS && blinkCurrentMillis - blinkTimer > BLINK_DELAY) {
    if (debug) 
      {
        Serial.print("Number of already discovered anchors: "); 
        Serial.println(discoveredAnchorsCount);
        // Serial.print("Current millis: "); 
        // Serial.println(blinkCurrentMillis);
        // Serial.print("Blink Timer: "); 
        // Serial.println(blinkTimer);
      } 
    blinkTimer = blinkCurrentMillis;
    currentAnchorAddress = 0;
    isTagBusy = true;
    expectedMessageType = MSG_TYPE_POLL_ACK;
    sendMessage(MSG_TYPE_POLL);
  }

  
}
