#include "arduino.h" 

byte getMyID() { 
    String macAddr = WiFi.macAddress(); 
    Serial.println(macAddr); 
    if (macAddr == "70:B8:F6:D8:F8:B8") { 
        return 101; 
    } else if (macAddr == "70:B8:F6:D8:F8:28") { 
        return 102; 
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

bool checkTagAddress() {
  Serial.print("Tag Address: ");
  Serial.println(message[1]);
  if (0 < tagID && tagID < 99) {
    return true;
  } else {
    Serial.println("  Wrong Tag Address!!");
  }
    // return (0 < addr && addr < 99) ? true : false; 
}

bool checkSource() {
  if (checkTagAddress()) {
    if (tagID == message[1]) 
    {
      return true;
    }
    Serial.println("Wrong source!");
    Serial.print("Current tag is: ");
    Serial.println(tagID);
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
 
bool isExpectedReceivedMessageType (byte messageType) { 
    return (messageType == MSG_TYPE_BLINK || messageType == MSG_TYPE_POLL || messageType == MSG_TYPE_RANGE) ? true : false; 
} 
 
void prepareMessageToSend(byte messageType, byte source, byte destination) { 
    memcpy(message, &messageType, sizeof(messageType)); 
    memcpy(message + 1, &source, sizeof(source)); 
    memcpy(message + 2, &destination, sizeof(destination)); 
} 
 
void sendMessage(byte messageType) { 
  Serial.println("---Send Message---");
  DW1000.newTransmit(); 
  DW1000.setDefaults(); 
  if (messageType == MSG_TYPE_ANCHOR_ADDR) {
    Serial.println("  MSG_TYPE_ANCHOR_ADDR");
    prepareMessageToSend(MSG_TYPE_ANCHOR_ADDR, myID, tagID);
  }  
  else if (messageType == MSG_TYPE_POLL_ACK) { 
    Serial.println("  MSG_TYPE_POLL_ACK");
    prepareMessageToSend(MSG_TYPE_POLL_ACK, myID, tagID); 
    DW1000.setDelay(pollackReplyDelay); 
  }  
  else if (messageType == MSG_TYPE_RANGE_REPORT) { 
    Serial.println("  MSG_TYPE_RANGE_REPORT");
    prepareMessageToSend(MSG_TYPE_RANGE_REPORT, myID, tagID); 
    timePollReceived.getTimestamp(message + 3); 
    timePollAckSent.getTimestamp(message + 8); 
    timeRangeReceived.getTimestamp(message + 13); 
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
  Serial.println("    message is sent");
  // if (messageType == MSG_TYPE_POLL_ACK) { 
  //   DW1000.getTransmitTimestamp(timePollAckSent); 
  // } 
 
 // DW1000.clearTransmitStatus(); 
} 
 
// bool messageReceived(byte expectedMessageType) { 
//   Serial.println("---Receive Message---");
   
//   // DW1000.newReceive(); 
//   // DW1000.setDefaults();
//   // DW1000.startReceive(); 

//   // while (!DW1000.isReceiveDone() && !DW1000.isReceiveTimeout()); 
//   // if (DW1000.isReceiveTimeout()) { 
//   //   Serial.println("  Receive Timeout");
//   //   //DW1000.clearReceiveTimeout(); 
//   //   return false; 
//   // } 

//   DW1000.getData(message, sizeof(message)); 
//   Serial.print("  Message content: ");
//   for (size_t i = 0; i < sizeof(message); i++) {
//     Serial.print(message[i]);
//     Serial.print(" ");
//   }
//   Serial.println(" ");
  
//   Serial.print("  Tag Address: ");
//   Serial.println(message[1]);
//   if (!isTagAddress(message[1])) {
//     Serial.print("  Wrong Tag Address: ");
//     Serial.println(message[1]);
//     return false; 
//   }
 
//   if (message[0] == MSG_TYPE_BLINK && expectedMessageType == MSG_TYPE_BLINK) { 
//     tagID = message[1]; 
//   } 
   
//   if (message[0] == expectedMessageType && message[1] == tagID) {
//     Serial.print("  message: ");
//     Serial.println(expectedMessageType);
//     if (expectedMessageType == MSG_TYPE_POLL || expectedMessageType == MSG_TYPE_RANGE) { 
//       Serial.print("Received time before clearReceiveStatus: "); 
//       DW1000.getReceiveTimestamp(tempTime); 
//       Serial.println(tempTime); 
//     } 
//     DW1000.clearReceiveStatus(); 
//     if (expectedMessageType == MSG_TYPE_POLL || expectedMessageType == MSG_TYPE_RANGE) { 
//       Serial.print("Received time after clearReceiveStatus: "); 
//       DW1000.getReceiveTimestamp(tempTime); 
//       Serial.println(tempTime); 
//     } 
 
//     return true; 
//   } 
  
//   Serial.println("!!!Received Wrong Message!!!");
//   Serial.print("  Expected message: ");
//   Serial.println(expectedMessageType);
//   Serial.print("  Received message: ");
//   Serial.println(message[0]);
//   Serial.print("  Expected tag: ");
//   Serial.println(tagID);
//   Serial.print("  Received tag: ");
//   Serial.println(message[1]);
//   return false; 
 
// } 

void handleReceived() {
  receivedAck = true;
}

void handleSent() {
  sentAck = true;
}
 
void setup() { 
  Serial.begin(115200); 
 
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
  lastActivity = millis();
  Serial.println("ANCHOR");
} 
 
void loop() { 
  checkForReset();
  
  if (sentAck) {
    sentAck = false;
    lastActivity = millis();
    lastSent = lastActivity;

    Serial.println("Send something");
    if (message[0] == MSG_TYPE_POLL_ACK) {
      DW1000.getTransmitTimestamp(timePollAckSent);
    }

  }
  
  if (receivedAck) {
    receivedAck = false;
    lastActivity = millis();
    Serial.println("Received something");
    
    DW1000.getData(message, sizeof(message)); 
    
    Serial.print("  Message content: ");
    for (size_t i = 0; i < sizeof(message); i++) {
      Serial.print(message[i]);
      Serial.print(" ");
    }
    Serial.println(" ");
    
    // if (messageReceived(MSG_TYPE_BLINK) && !isAnchorBusy) { 

    if (expectedMessageType == message[0]) {
      if (expectedMessageType == MSG_TYPE_BLINK && !isAnchorBusy && checkTagAddress(message[1])) {
        tagID = message[1];
        sendMessage(MSG_TYPE_ANCHOR_ADDR);
        expectedMessageType = MSG_TYPE_POLL;
      }

      if (checkAddresses()) {
        if (expectedMessageType == MSG_TYPE_BLINK && !isAnchorBusy) {
          isAnchorBusy = true;
          DW1000.getReceiveTimestamp(timePollReceived); 
          sendMessage(MSG_TYPE_POLL_ACK); 
          expectedMessageType = MSG_TYPE_RANGE;
        }

        if (expectedMessageType == MSG_TYPE_BLINK && isAnchorBusy) {
          DW1000.getReceiveTimestamp(timeRangeReceived); 
          sendMessage(MSG_TYPE_RANGE_REPORT); 
          expectedMessageType = MSG_TYPE_BLINK;
          isAnchorBusy = false;
        }
      }
    } else {
      Serial.println("Got wrong message: ");
      Serial.print("  Expected: ");
      Serial.println(expectedMessageType);
      Serial.print("  Received: ");
      Serial.println(message[0]);
    }
    // if (expectedMessageType == message[0] && expectedMessageType == MSG_TYPE_BLINK && checkTagAddress(message[1]) && !isAnchorBusy) { 
      
    // // } else if (messageReceived(MSG_TYPE_POLL) && !isAnchorBusy) {
    // } else if (expectedMessageType == message[0] && expectedMessageType == MSG_TYPE_POLL && checkTagAddress(message[1]) && tagID == message[1] && !isAnchorBusy) {
      
    // // } else if (messageReceived(MSG_TYPE_RANGE) && isAnchorBusy) { 
    // } else if (expectedMessageType == message[0] && expectedMessageType == MSG_TYPE_RANGE && checkTagAddress(message[1]) && tagID == message[1] && !isAnchorBusy) {
      
    // }
  }
}
