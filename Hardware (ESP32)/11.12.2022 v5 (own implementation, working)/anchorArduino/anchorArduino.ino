#include "arduino.h"

void updateState(int nextState) {
  state = nextState;
  lastStateChange = millis();
}

void noteActivity() {
  lastActivity = millis();
}

void setAddress() {
    String macAddr = WiFi.macAddress();
    if (macAddr == "70:B8:F6:D8:F8:B8") {
        myID = 101;
    } else if (macAddr == "70:B8:F6:D8:F8:28") {
        myID = 102;
    } else {
        Serial.println("Wrong tag MAC address.");
    }
}

boolean isTagAddress(uint16_t addr) {
    return (0 < addr && addr < 99) ? true : false;
}

void checkForReset() {
    if(!sentAck && !receivedAck) {
  		if(currentTime - lastActivity > DEFAULT_RESET_TIMEOUT) {
  			initReceiver();
        Serial.println("Reinit....");
  		}
  		return;
  	}
}

void handleReceived() {
  receivedAck = true;
}

void handleSent() {
  sentAck = true;
}

void initReceiver() {
  DW1000.newReceive();
  DW1000.setDefaults();
  DW1000.receivePermanently(true);
  DW1000.startReceive();
  noteActivity();
}

void prepareTx() {
  DW1000.newTransmit();
  DW1000.setDefaults();
}

void startTx() {
  DW1000.setData(txFrame, FRAME_SIZE);
  DW1000.startTransmit();
  lastSent = 0;
}

void transmitRangingInit() {
  prepareTx();
  txFrame[0] = RANGINGINIT;
  setSenderAddr(txFrame, myID);
  setReceiverAddr(txFrame, tagID);
  //SET_SRC(txFrame, myID, ADDR_SIZE);
  //SET_DST(txFrame, sender, ADDR_SIZE);
  startTx();
}

void transmitPollAck() {
  prepareTx();
  txFrame[0] = POLLACK;
  setSenderAddr(txFrame, myID);
  setReceiverAddr(txFrame, tagID);
  //SET_SRC(txFrame, myID, ADDR_SIZE);
  //SET_DST(txFrame, tagID, ADDR_SIZE);
  DW1000.setDelay(pollackReplyDelay);
  startTx();
}

void transmitRangeReport() {
  prepareTx();
  txFrame[0] = RANGEREPORT;
  setSenderAddr(txFrame, myID);
  setReceiverAddr(txFrame, tagID);
  timePollReceived.getTimestamp(txFrame + 5);
  timePollAckSent.getTimestamp(txFrame + 10);
  timeRangeReceived.getTimestamp(txFrame + 15);
  startTx();
}

void setup() {
  Serial.begin(115200);

  DW1000.begin(PIN_IRQ, PIN_RST);
  DW1000.select(PIN_SS);

  setAddress();

  DW1000.newConfiguration();
  DW1000.setDefaults();
  DW1000.setNetworkId(networkId);
  DW1000.setDeviceAddress(myID);
  DW1000.enableMode(DW1000.MODE_LONGDATA_RANGE_ACCURACY);
  DW1000.commitConfiguration();

  DW1000.attachSentHandler(handleSent);
  DW1000.attachReceivedHandler(handleReceived);
  
  DW1000.setAntennaDelay(Adelay);

  initReceiver();

}

void loop() {
  currentTime = millis();

  if (state == STATE_RANGING_INIT
      && currentTime - lastStateChange > RANGING_INIT_TIMEOUT) {
    //PRINTLN(F("Seems Pending Pong lost. Return to IDLE"));
    updateState(STATE_IDLE);
  }

  if (state == STATE_RANGE
      && ((lastSent && currentTime - lastSent > RANGE_TIMEOUT)
          || currentTime - lastStateChange > 2 * RANGE_TIMEOUT)) {
    /*
     * Check RANGE message timeout when state is waiting for RANGE message
     */
    //PRINTLN(F("RANGE timeout. Return to IDLE"));
    updateState(STATE_IDLE);
    return;
  }

  checkForReset();
  
  if (sentAck) {
    PRINTLN(F("Sent something"));
    sentAck = false;
    noteActivity();
    lastSent = lastActivity;

    sentMessageType = txFrame[0];

    if (sentMessageType != RANGINGINIT && sentMessageType != POLLACK && sentMessageType != RANGEREPORT) return;

    if (state == STATE_RANGING_INIT && sentMessageType == RANGINGINIT) {
      //PRINTLN(F("  Ranging phase initialized sent. Return to IDLE"));
      updateState(STATE_IDLE);
      return;
    }

    if (sentMessageType == POLLACK) {
      //PRINTLN(F("  POLLACK sent. Getting timestamp..."));
      DW1000.getTransmitTimestamp(timePollAckSent);
    }

    if (sentMessageType == RANGEREPORT) {
      //PRINTLN(F("  RANGEREPORT sent"));
    }
  }

  if (receivedAck) {
    //PRINTLN(F("Received something"));
    receivedAck = false;
    noteActivity();
    DW1000.getData(rxFrame, FRAME_SIZE);
    getSenderAddr(rxFrame, tagID);
    if (!isTagAddress(tagID)) return; 

    PRINT(F("Received something from tag: ")); PRINTLN(tagID);

    receivedMessageType = rxFrame[0];
    //PRINT(F("Received msg: ")); PRINTLN(receivedMessageType);
    if (receivedMessageType != BLINK && receivedMessageType != POLL && receivedMessageType != RANGE) return;
    //PRINT(F("Current state: ")); PRINTLN(state);
    if (state == STATE_IDLE) {
      PRINTLN(F("  State: IDLE"));
      if (receivedMessageType == BLINK) {
        PRINTLN(F("    Received BLINK. Reply with RANGING INIT"));
        rangingInitDelay = random(0, 90);
        delay(rangingInitDelay);
        transmitRangingInit();
        updateState(STATE_RANGING_INIT);
        return;
      }

      if (receivedMessageType == POLL) {
        PRINTLN(F("    Received POLL"));
        if (!isReceiverMatch(rxFrame, myID)) return;
        PRINTLN(F("      Reply with POLLACK"));
        DW1000.getReceiveTimestamp(timePollReceived);
        transmitPollAck();
        updateState(STATE_RANGE);
        return;
      }
    }

    if (state == STATE_RANGING_INIT) {
      PRINTLN(F("  State: STATE_RANGING_INIT"));
      PRINTLN(F("    Ignore all received frames"));
      /*
       * PONG message is pending to be transmitted
       * Anchor should ignore all other messages
       */
      return;
    }

    if (state == STATE_RANGE && receivedMessageType == RANGE && isSenderMatch(rxFrame, tagID)) {
      Serial.println("Received RANGE");
      DW1000.getReceiveTimestamp(timeRangeReceived);
      transmitRangeReport();
      updateState(STATE_IDLE);
      return;
    }
  }
}
