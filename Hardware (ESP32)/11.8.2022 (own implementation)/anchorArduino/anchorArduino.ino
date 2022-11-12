#include "anchorArduino.h"


void updateState(byte nextState) {
    state = nextState;
    lastStateChange = millis();
}

void noteActivity() {
    lastActivity = millis();
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

void checkForReset() {
    if(!sentAck && !receivedAck) {
  		if(currentTime - lastActivity > DEFAULT_RESET_TIMEOUT) {
  			initReceiver();
       Serial.println("Reinit....");
  		}
  		return;
  	}
}

boolean isTagAddress(uint16_t addr) {
    return (0 < addr && addr < 99) ? true : false;
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

void transmitInit() {
    DW1000.newTransmit();
    DW1000.setDefaults();
}

void transmit() {
    DW1000.setData(txFrame, FRAME_SIZE);
    DW1000.startTransmit();
}

void transmitRangingInit() {
    transmitInit();
    txFrame[0] = RANGINGINIT;
    setSenderAddr(txFrame, myID);
    setReceiverAddr(txFrame, tagID);
    transmit();
}

void transmitPollAck() {
    transmitInit();
    txFrame[0] = POLLACK;
    Serial.println(String("   in POLLACK transmit. Anchor source: ") + String(myID) + String(" Tag target: ") + String(tagID));
    setSenderAddr(txFrame, myID);
    setReceiverAddr(txFrame, tagID);
    DW1000.setDelay(pollackReplyDelay);
    transmit();
}

void transmitRangeReport() {
    transmitInit();
    txFrame[0] = RANGEREPORT;
    setSenderAddr(txFrame, myID);
    setReceiverAddr(txFrame, tagID);
    timePollReceived.getTimestamp(txFrame + 5);
    timePollAckSent.getTimestamp(txFrame + 10);
    timeRangeReceived.getTimestamp(txFrame + 15);
    transmit();
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

    initReceiver();
}

void loop() {
    Serial.println("in loop...");
    
    currentTime = millis();
    checkForReset();
    
    if (state == STATE_RANGING_INIT && (currentTime - lastStateChange > RANGING_INIT_TIMEOUT)) {
        Serial.println("RANGING INIT TIMEOUT");
        updateState(STATE_IDLE);
    }

    if (state == STATE_RANGE && (currentTime - lastStateChange > RANGE_TIMEOUT)) {
        Serial.println("RANGE TIMEOUT");
        updateState(STATE_IDLE);
    }

    if (sentAck) {
        Serial.println("sent something");
        sentAck = false;
        noteActivity();
        sentMessageType = txFrame[0];

        if (sentMessageType != RANGINGINIT && sentMessageType != POLLACK) return;
        
        if (sentMessageType == RANGINGINIT) {
            Serial.println("init ranging");
            updateState(STATE_IDLE);
            return;
        }

        if (sentMessageType == POLLACK) {
            DW1000.getTransmitTimestamp(timePollAckSent);
        }
    }

    if (receivedAck) {
        Serial.println("received something");
        receivedAck = false;
        noteActivity();
        DW1000.getData(rxFrame, FRAME_SIZE);
        receivedMessageType = rxFrame[0];

        getSenderAddr(rxFrame, tagID);
        
        if (!isTagAddress(tagID)) return; 
        Serial.println(String("Received from tag: ") + String(tagID));
        if (receivedMessageType != BLINK && receivedMessageType != POLL && receivedMessageType != RANGE) return;

        if (state == STATE_IDLE) {
            Serial.println("IDLE STATE...");
            if (receivedMessageType == BLINK) {
                Serial.println("Received BLINK");
                rangingInitDelay = random(0, DISCOVERY_TIMEOUT - RANGING_INIT_TIMEOUT - 1);
                delay(rangingInitDelay);
                transmitRangingInit();
                Serial.println("RANGING INIT transmitted. Switch to RANGING INIT STATE");
                updateState(STATE_RANGING_INIT);
                return;
            }

            if (receivedMessageType == POLL) {
                if (!isReceiverMatch(rxFrame, myID)) return;
                Serial.println("Received POLL");
                DW1000.getReceiveTimestamp(timePollReceived);
                transmitPollAck();
                Serial.println("POLLACK sent");
                updateState(STATE_RANGE);
                return;
            }
        }

        if (state == STATE_RANGE && receivedMessageType == RANGE && isReceiverMatch(rxFrame, myID)) {
            Serial.println("Received RANGE");
            DW1000.getReceiveTimestamp(timeRangeReceived);
            transmitRangeReport();
            updateState(STATE_IDLE);
            return;
        }
    }
}
