#include "arduino.h" 

uint16_t getMyID() { 
    String macAddr = WiFi.macAddress(); 
    if (debug) Serial.println(macAddr); 
    if (macAddr == "70:B8:F6:D8:F8:B8") { 
        return 101; 
    } else if (macAddr == "70:B8:F6:D8:F8:28") { 
        return 102; 
    } else { 
        if (debug) Serial.println("Wrong tag MAC address."); 
        return 0; 
    } 
} 

void checkForReset() {
  currentTime = millis();
  if ((!sentAck && !receivedAck)) {
//    if(!repeatedSoftInit && (currentTime - lastActivity > DEFAULT_RESET_TIMEOUT || errorWatch)) {
    if (currentTime - lastActivity > DEFAULT_RESET_TIMEOUT) {
      currentTagAddress = 0;
      isAnchorBusy = false;
      // errorWatch = false;
      pollCounterForReset = 0;
      //repeatedSoftInit = true;
      expectedMessageType = MSG_TYPE_BLINK;
      initReceiver();
      Serial.println("Reinit....");
      noteActivity();
      
    }

//    if (repeatedSoftInit && (currentTime - lastActivity > HARD_RESET_TIMEOUT)) {
//      initAnchor();
//      repeatedSoftInit = false;
//      Serial.println("HARD RESET!");
//    }
    return;
  }
}

void initReceiver() {
  DW1000.newReceive();
  DW1000.setDefaults();
  DW1000.receivePermanently(true);
  DW1000.startReceive();
}

bool checkTagAddress() {
  if (debug) {
    Serial.print("Tag Address: ");
    Serial.println(message[1]);    
  }

  if (0 < message[1] && message[1] < 99) {
    return true;
  } else {
   if (debug) Serial.println("  Wrong Tag Address!!");
  }
}

bool checkSource() {
  if (checkTagAddress()) {
    if (currentTagAddress == message[1]) return true;
    if (debug) {
      Serial.println("Wrong source!");
      Serial.print("Current tag is: ");
      Serial.println(currentTagAddress);
    }
   
  }
  return false;
}

bool checkDestination() {
  if (myID == message[2]) return true;

  if (debug) Serial.println("Wrong destination. Not for me!");
  return false;
}

bool checkSourceAndDestination() {
  return checkSource() && checkDestination();
}
 
 
void prepareMessageToSend(byte messageType, byte source, byte destination) { 
    memcpy(message, &messageType, sizeof(messageType)); 
    memcpy(message + 1, &source, sizeof(source)); 
    memcpy(message + 2, &destination, sizeof(destination)); 
} 
 
void sendMessage(byte messageType) { 
 if (debug) Serial.println("---Send Message---");
  DW1000.newTransmit(); 
  DW1000.setDefaults(); 
  if (messageType == MSG_TYPE_ANCHOR_ADDR) {
   if (debug) Serial.println("  MSG_TYPE_ANCHOR_ADDR");
    prepareMessageToSend(MSG_TYPE_ANCHOR_ADDR, myID, currentTagAddress);
  }  
  else if (messageType == MSG_TYPE_POLL_ACK) { 
   if (debug) Serial.println("  MSG_TYPE_POLL_ACK");
    prepareMessageToSend(MSG_TYPE_POLL_ACK, myID, currentTagAddress); 
    DW1000.setDelay(pollackReplyDelay);
  }  
  else if (messageType == MSG_TYPE_RANGE_REPORT) { 
   if (debug) Serial.println("  MSG_TYPE_RANGE_REPORT");
    prepareMessageToSend(MSG_TYPE_RANGE_REPORT, myID, currentTagAddress); 
    timePollReceived.getTimestamp(message + 3); 
    timePollAckSent.getTimestamp(message + 8); 
    timeRangeReceived.getTimestamp(message + 13); 
  }  
 
  DW1000.setData(message, sizeof(message)); 
   if (debug) {
    Serial.print("  Message content: ");
    for (size_t i = 0; i < sizeof(message); i++) {
       Serial.print(message[i]);
       Serial.print(" ");
    }
    Serial.println(" ");
   }
 
  
  DW1000.startTransmit(); 
  //while (!DW1000.isTransmitDone()) continue; 
 
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

void initAnchor() {
  DW1000.begin(PIN_IRQ, PIN_RST); 
  DW1000.select(PIN_SS); 
 
  myID = getMyID(); 
 
  DW1000.newConfiguration(); 
  DW1000.setDefaults(); 
  DW1000.setDeviceAddress(myID); 
  DW1000.setNetworkId(networkId); 
  DW1000.enableMode(DW1000.MODE_LONGDATA_RANGE_ACCURACY); 
  DW1000.commitConfiguration(); 

  DW1000.attachSentHandler(handleSent);
  DW1000.attachReceivedHandler(handleReceived);
  
  initReceiver();
  noteActivity();
  Serial.println("ANCHOR");

  delay(1000);  
}

void setup() { 
  Serial.begin(115200); 
 
  initAnchor();
} 
 
void loop() { 
  checkForReset();
  
  if (sentAck) {
//    repeatedSoftInit = false;
    sentAck = false;
    noteActivity();

   if (debug) Serial.println("Send something");
    if (message[0] == MSG_TYPE_POLL_ACK) {
      DW1000.getTransmitTimestamp(timePollAckSent);
    }
    
  }
  
  if (receivedAck) {
//    repeatedSoftInit = false;
    receivedAck = false;
    if (debug) Serial.println("Received something");
    
    DW1000.getData(message, sizeof(message)); 

    if (debug) {
       Serial.print("  Message content: ");
       for (size_t i = 0; i < sizeof(message); i++) {
         Serial.print(message[i]);
         Serial.print(" ");
       }
       Serial.println(" ");
    }
   
    
    noteActivity();

    if (expectedMessageType == message[0]) {
      if (expectedMessageType == MSG_TYPE_BLINK && !isAnchorBusy && checkTagAddress()) {
        if (debug) Serial.println("Blink!");
        currentTagAddress = message[1];
        if (debug) {
          Serial.print("Current Tag Address: ");
          Serial.println(currentTagAddress);
        }
        expectedMessageType = MSG_TYPE_POLL;
        sendMessage(MSG_TYPE_ANCHOR_ADDR);
       return;
      }

      if (checkSourceAndDestination()) {
        if (expectedMessageType == MSG_TYPE_POLL && !isAnchorBusy) {
          isAnchorBusy = true;
          DW1000.getReceiveTimestamp(timePollReceived); 
          expectedMessageType = MSG_TYPE_RANGE;
          sendMessage(MSG_TYPE_POLL_ACK);
          return;
        }

        if (expectedMessageType == MSG_TYPE_RANGE && isAnchorBusy) {
          DW1000.getReceiveTimestamp(timeRangeReceived); 
          expectedMessageType = MSG_TYPE_BLINK;
          sendMessage(MSG_TYPE_RANGE_REPORT); 
          isAnchorBusy = false;
          return;
        }

      }
      
   } else {
      // errorWatch = true;
//      if (expectedMessageType == MSG_TYPE_POLL) 
//      {
//        pollCounterForReset++;
//        if (pollCounterForReset > 4) {
//          errorWatch = true;
//        }
//      }
      if (debug) {
        Serial.print("POLL counter: ");
        Serial.println(pollCounterForReset);
        Serial.println("Got wrong message: ");
        Serial.print("  Expected: ");
        Serial.println(expectedMessageType);
        Serial.print("  Received: ");
        Serial.println(message[0]);
      }
     
    }

  }
}
