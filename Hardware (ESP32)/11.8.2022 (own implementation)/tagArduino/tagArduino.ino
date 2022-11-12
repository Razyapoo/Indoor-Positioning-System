#include <SPI.h>
#include <WiFi.h>
#include <DW1000.h>

#include "tagArduino.h"

void noteActivity() {
    lastActivity = millis();
}

void  updateState(byte nextState) {
    state = nextState;
    lastStateChange = millis();
}

void updateRangingPhase() {
    anchor++;
    updateState(STATE_RANGING);
}

void handleSent() {
    sentAck = true;
}

void handleReceived() {
    receivedAck = true;
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

boolean isAnchorAddress(uint16_t addr) {
    return (100 < addr && addr < 199) ? true : false;
}

void transmitInit() {
    DW1000.newTransmit();
    DW1000.setDefaults();
}

void transmit() {
    DW1000.setData(txFrame, FRAME_SIZE);
    DW1000.startTransmit();
}

void transmitBlink() {
    transmitInit();
    txFrame[0] = BLINK;
    setSenderAddr(txFrame, myID);
    transmit();
    noteActivity();
}

void transmitPoll() {
    transmitInit();
    txFrame[0] = POLL;
    setSenderAddr(txFrame, myID);
    setReceiverAddr(txFrame, anchor->first);
    transmit();
    noteActivity();
}

/* Delayed Final frame transmission */
void transmitRange() {
    transmitInit();
    txFrame[0] = RANGE;
    setSenderAddr(txFrame, myID);
    setReceiverAddr(txFrame, anchor->first);
    DW1000.setDelay(rangeReplyDelay);
    transmit();
    noteActivity();
}

void computeRangeAsymmetric() {
    Serial.println(String("timePollSent: ") + String(timePollSent.getAsMicroSeconds()));
    Serial.println(String("timePollReceived: ") + String(timePollReceived.getAsMicroSeconds()));
    Serial.println(String("timePollAckSent: ") + String(timePollAckSent.getAsMicroSeconds()));
    Serial.println(String("timePollAckReceived: ") + String(timePollAckReceived.getAsMicroSeconds()));
    Serial.println(String("timeRangeSent: ") + String(timeRangeSent.getAsMicroSeconds()));
    Serial.println(String("timeRangeReceived: ") + String(timeRangeReceived.getAsMicroSeconds()));
    DW1000Time round1 = (timePollAckReceived - timePollSent).wrap();
    DW1000Time reply1 = (timePollAckSent - timePollReceived).wrap();
    DW1000Time round2 = (timeRangeReceived - timePollAckSent).wrap();
    DW1000Time reply2 = (timeRangeSent - timePollAckReceived).wrap();
    DW1000Time tof = (round1 * round2 - reply1 * reply2) / (round1 + round2 + reply1 + reply2);
    anchor->second = tof.getAsMeters();
}

void addNewAnchor(uint16_t newAnchorID) {
    
    if (isAnchorAddress(newAnchorID)) {
        if (anchors.find(anchorID) == anchors.end()) {
            anchors.insert({newAnchorID, 0});
            Serial.println("  New anchor is added to the list");        
        } else {
            Serial.println("  Anchor is already present in the list");
        }
    } else {
      Serial.println("Not anchor's address");
      }
}

void setAddress() {
    String macAddr = WiFi.macAddress();
    if (macAddr == "70:B8:F6:D8:F6:48") {
        myID = 1;
    } else if (macAddr == "70:B8:F6:D8:F6:24") {
        myID = 2;
    } else if (macAddr == "70:B8:F6:D8:F6:60") {
        myID = 3;
    } else {
        Serial.println("Wrong tag MAC address.");
    }
}

//void connectToServer() {
//    WiFi.mode(WIFI_STA);
//    WiFi.setSleep(false);
//    WiFi.begin(ssid, password);
//    while (WiFi.status() != WL_CONNECTED)
//    {
//        delay(500);
//        Serial.print(".");
//    }
//    Serial.println("Connected");
//    if (client.connect(host, 30001))
//    {
//        Serial.println("Success");
//    } else {
//        Serial.println("Unsuccessfull connection");
//    }
//}

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
    DW1000.setAntennaDelay(Adelay);

    DW1000.attachSentHandler(handleSent);
    DW1000.attachReceivedHandler(handleReceived);

    initReceiver();
    // connectToServer();
 
    Serial.println("setup finished...");
}

void loop() {
    //checkForReset();
    currentTime = millis();

    if (state == STATE_IDLE) {
        Serial.println("STATE_IDLE - Clear anchor list...");
        anchors.clear();
        transmitBlink();
        Serial.println("  Start DISCOVERY PHASE");
        updateState(STATE_DISCOVERY);
        return;
    }

    if (state == STATE_DISCOVERY && (currentTime - lastStateChange > DISCOVERY_TIMEOUT)) {
        Serial.println("STATE_DISCOVERY - finished");
        if (anchors.size() == 0) {
            Serial.println("No anchor found. Return to STATE_IDLE and SLEEP for 50 ms");
            updateState(STATE_IDLE);
            delay(SLEEP);
        } else {
            Serial.println("Some anchors are found. Start ranging phase");
            anchor = anchors.begin();
            updateState(STATE_RANGING);
        }
        return;
    }

    if (state == STATE_POLLACK && (currentTime - lastStateChange > POLLACK_TIMEOUT)) {
        Serial.println("STATE_POLLACK - timeout. Continue RANGING PHASE");
        updateRangingPhase();
        return;
    }

    if (state == STATE_RANGEREPORT && (currentTime - lastStateChange > RANGEREPORT_TIMEOUT)) {
        Serial.println("STATE_RANGEREPORT - timeout. Continue RANGING PHASE");
        updateRangingPhase();
        return;
    }

    if (state == STATE_RANGING) {
        Serial.println("RANGING STATE");
        if (anchor != anchors.end()) {
            Serial.println("Not all of anchors are ranged. Continue ranging phase with next anchor.");
            transmitPoll();
            updateState(STATE_POLLACK);
        } else {
            Serial.println("All anchors are scanned. Here is a list of distances: ");
            for (anchor = anchors.begin(); anchor != anchors.end(); anchor++) {
                Serial.println(String(" Anchor ") + String(anchor->first) + String(": ") + String(anchor->second, 2));
            }
            Serial.println("Return to STATE_IDLE and SLEEP for 50 ms");
            updateState(STATE_IDLE);
            delay(SLEEP);
        }
        return;
    }

    if (sentAck) {
        Serial.println("sent something...");
        sentAck = false;
        noteActivity();
        sentMessageType = txFrame[0];

        if (sentMessageType != POLL && sentMessageType != RANGE) return;

        if (sentMessageType == POLL) {
            Serial.println("POLL sent...");
            DW1000.getTransmitTimestamp(timePollSent);
        } 

        if (sentMessageType == RANGE) {
            Serial.println("RANGE sent...");
            DW1000.getTransmitTimestamp(timeRangeSent);
        }
    }

    if (receivedAck) {
        Serial.println("received something...");
        receivedAck = false;
        noteActivity();
        DW1000.getData(rxFrame, FRAME_SIZE);
        receivedMessageType = rxFrame[0];
        
        if (!isReceiverMatch(rxFrame, myID)) {
          Serial.println("Not for me!");
          return;
        }

        if (receivedMessageType != RANGINGINIT && receivedMessageType != POLLACK && receivedMessageType != RANGEREPORT) {
          Serial.println("Wrong received message!");
          return;
        }

        if (state == STATE_DISCOVERY && receivedMessageType == RANGINGINIT) {
            getSenderAddr(rxFrame, anchorID);
            Serial.println(String("Received accepted ranging init from achor ") + String(anchorID));
            addNewAnchor(anchorID);
            return;
        }

        if (isSenderMatch(rxFrame, anchor->first)) {
            if (state == STATE_POLLACK && receivedMessageType == POLLACK) {
                Serial.println("Received POLLACK");
                DW1000.getTransmitTimestamp(timePollAckReceived);
                transmitRange();
                Serial.println("RANGE request sent");
                updateState(STATE_RANGEREPORT);
                return;
            }

            if (state == STATE_RANGEREPORT && receivedMessageType == RANGEREPORT) {
                timePollReceived.setTimestamp(rxFrame + 5);
                timePollAckSent.setTimestamp(rxFrame + 10);
                timeRangeReceived.setTimestamp(rxFrame + 15);
                computeRangeAsymmetric();
                updateRangingPhase();
                return;
            }
        }
    }
}
