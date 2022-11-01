#define DEBUG true

#include <SPI.h>
#include <WiFi.h>
#include <DW1000.h>
//#include <sys/socket.h>

#include "debug.h"
#include "def.h"
#include "arduino.h"
#include "link.h"

#define PIN_IRQ  34
#define PIN_RST  27
#define PIN_SS  4


/* WiFi settings */
//const char *ssid = "ASUS";
//const char *password = "cuni3103&";
//const char *host = "192.168.1.55";
//const char *ssid = "iPhone Nyx";
//const char *password = "Nera1998&";
//const char *host = "172.20.10.3";
const char *ssid = "K50S_8694";
const char *password = "123456789";
const char *host = "192.168.43.108";
WiFiClient client;

/* Edit tagId */
// Each tag must have a unique tag ID (do not use ID 0)
// DO NOT REMOVE FLAT_TAG
uint16_t tagId;
// UWB anchors and tags must have the same network ID
const uint16_t networkId = 10;

struct MyLink *uwb_data;
// int index_num = 0;
String all_json = "";
//char ack[3] = { '\0' };
String ack = "";

// Current state of a UWB anchor state machine
volatile char state = STATE_SCAN;

// The number of observed anchors
unsigned char num_anchors = 0;
// Index of the current anchor in two way ranging
unsigned char idx_anchor = 0;
// Array of anchors' IDs
uint16_t anchorId[NUM_ANCHORS] = {ID_NONE, ID_NONE};
//uint16_t anchorId[NUM_ANCHORS] = {ID_NONE};
// Array of distance measurements of the corresponding anchors
float distance[NUM_ANCHORS] = {0, 0};
//float distance[NUM_ANCHORS] = {0};

// UWB anchors and tags must have the same replay_delay
DW1000Time reply_delay = DW1000Time(REPLY_DELAY_MS, DW1000Time::MILLISECONDS);
DW1000Time timePollSent;
DW1000Time timePollReceived;
DW1000Time timePollAckSent;
DW1000Time timePollAckReceived;
DW1000Time timeRangeSent;
DW1000Time timeRangeReceived;

// Last time that loop() is called
unsigned long curMillis;
// Last time that a frame is PUSHED INTO THE AIR FROM THE S/W BUFFER
unsigned long lastSent;
// Last time that a UWB device sends or receives a frame
// i.e., meaningful DWM1000 activity
unsigned long lastActivity;
unsigned long lastStateChange;
unsigned long runtime = 0;
boolean toSend = false;

byte txBuffer[FRAME_LEN];
byte rxBuffer[FRAME_LEN];

// Set to true if a frame is pushed into the air and SPI tx interrupt is triggered
boolean sentFrame = false;
// Set to true if a frame is received and SPI rx interrupt is triggered
// Not yet stored into the S/W buffer
boolean receivedFrame = false;

void updateState(int nextState) {
  state = nextState;
  lastStateChange = millis();
}

void updateRoundRobin() {
  idx_anchor++;
  updateState(STATE_ROUNDROBIN);
}

void noteActivity() {
  lastActivity = millis();
}

/*************************************
 * Arduino (master) - DW1000 (slave) *
 *************************************/
void spiReceiveEvent() {
  receivedFrame = true;
}

void spiSendEvent() {
  sentFrame = true;
}

void initDW1000Receiver() {
  DW1000.newReceive();
  DW1000.setDefaults();
  DW1000.receivePermanently(true);
  DW1000.startReceive();
  noteActivity();
}

void setupDW1000() {
  DW1000.begin(PIN_IRQ, PIN_RST);
  DW1000.select(PIN_SS);
  DW1000.newConfiguration();
  DW1000.setDefaults();
  DW1000.setNetworkId(networkId);
  DW1000.setDeviceAddress(tagId);
  DW1000.enableMode(DW1000.MODE_LONGDATA_RANGE_ACCURACY);
  DW1000.commitConfiguration();

  DW1000.attachSentHandler(spiSendEvent);
  DW1000.attachReceivedHandler(spiReceiveEvent);

  initDW1000Receiver();
}

void prepareTx() {
  DW1000.newTransmit();
  DW1000.setDefaults();
}

void startTx() {
  DW1000.setData(txBuffer, FRAME_LEN);
  DW1000.startTransmit();
  // timeout will be asserted after tx interrupt
  lastSent = 0;
}

void transmitPing() {
  prepareTx();
  txBuffer[0] = FTYPE_PING;
  SET_SRC(txBuffer, tagId, ADDR_SIZE);
  startTx();
}

void transmitPoll() {
  prepareTx();
  txBuffer[0] = FTYPE_POLL;
  SET_SRC(txBuffer, tagId, ADDR_SIZE);
  SET_DST(txBuffer, anchorId[idx_anchor], ADDR_SIZE);
  startTx();
}

void transmitRange() {
  prepareTx();
  txBuffer[0] = FTYPE_RANGE;
  SET_SRC(txBuffer, tagId, ADDR_SIZE);
  SET_DST(txBuffer, anchorId[idx_anchor], ADDR_SIZE);
  DW1000.setDelay(reply_delay);
  startTx();
}

void calculateRange() {
    // asymmetric two-way ranging (more computation intense, less error prone)
    // See APS013: The Implementation of Two-Way Ranging with he DW1000
    // https://decawave.com/sites/default/files/aps013_dw1000_and_two_way_ranging_v2.2.pdf pp. 4
    DW1000Time round1 = (timePollAckReceived - timePollSent).wrap();
    DW1000Time reply1 = (timePollAckSent - timePollReceived).wrap();
    DW1000Time round2 = (timeRangeReceived - timePollAckSent).wrap();
    DW1000Time reply2 = (timeRangeSent - timePollAckReceived).wrap();
    DW1000Time tof = (round1 * round2 - reply1 * reply2)
                      / (round1 + round2 + reply1 + reply2);
    distance[idx_anchor] = tof.getAsMeters();
}

/********
 * Main *
 ********/
void setup() {
  //setupI2C();
  setupDW1000();
#if DEBUG
  Serial.begin(115200);
#endif // DEBUG
  String mc_addr = WiFi.macAddress();
  if (mc_addr == "70:B8:F6:D8:F6:48") {
    tagId = FLAG_TAG | 1;
  } else if (mc_addr == "70:B8:F6:D8:F6:24") {
    tagId = FLAG_TAG | 2;
  } else if (mc_addr == "70:B8:F6:D8:F6:60") {
    tagId = FLAG_TAG | 3;
  } else {
    Serial.println("Wrong tag MAC address.");
  }
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
    
  delay(1000);
  uwb_data = init_link(tagId);
  //PRINTLN(F("Setup finished"));
  //PRINTLN(F("=============="));
}

void loop() {
  curMillis = millis();
  // Safety watchdog to avoid stuck in PONG state
  // 1. If SPI tx interrupt captured (confirmed that PING is broadcasted)
  // 2. If SPI tx interrupt is not captured for some reason

    /* Send data to server */
  if (state == STATE_SEND_REQUEST_TO_SERVER) {
    PRINTLN(F("Server request"));
    if (client.connected()) { 
      client.print("REQUEST");
      updateState(STATE_REQUEST_ACK_FROM_SERVER);
    } else {
      PRINTLN(F("Server is not available"));
    }
  }

  if (state == STATE_REQUEST_ACK_FROM_SERVER) {
      //&& ((lastSent && curMillis - lastSent > SERVER_TIMEOUT_MS)
        //  || curMillis - lastStateChange > 2 * SERVER_TIMEOUT_MS)) {
//    if (client.available()) ack = client.read();/
    ack = "";
    delay(300);
    while (client.available()) { 
       delay(3);
       char c = client.read();
       ack += c;
    } 
    PRINTLN(ack);
    if (ack == "OK") {
//    if (strcmp(ack, "OK") == 0) {
        PRINTLN(F("Server response - OK"));
        updateState(STATE_SEND_DISTANCE_TO_SERVER);
      }
    else {
      PRINTLN(F("Server does't respond. Probably connection lost."));
      ack = "";
      updateState(STATE_SCAN);
    }
  }

  if (state == STATE_SEND_DISTANCE_TO_SERVER) {
    make_link_json(uwb_data, &all_json);
    client.print(all_json);
    ack = "";
    delay(300);
    while (client.available()) { 
       delay(3);
       char c = client.read();
       ack += c;
    }
    if (ack == "RECEIVED") {
      toSend = false;
      PRINTLN(F("  UWB package sent"));
      updateState(STATE_SCAN);
      ack = "";
    } else {
      PRINTLN(F("Server does't respond. Data was not send."));
      ack = "";
    }
  }
  
  if (state == STATE_PONG
      && ((lastSent && curMillis - lastSent > PONG_TIMEOUT_MS)
          || curMillis - lastStateChange > 2 * PONG_TIMEOUT_MS)) {
    //PRINTLN(F("PONG timeout"));
    // if (num_anchors < 2) {
    if (num_anchors < 2) {
      // Seems not enough anchors exist (lack of deployment, collision, delayed tx)
      //PRINTLN(F("  Not enough anchors scanned. Return to IDLE"));
      updateState(STATE_SCAN);
      return;
    } else {
      // 3 or more anchors are observed
      // Proceed to two way ranging in a round robin manner
      //PRINTLN(F("  Starting ROUNDROBIN..."));
      idx_anchor = 0;
      updateState(STATE_ROUNDROBIN);
      return;
    }
  }
  // Safety watchdog to avoid stuck in POLLACK state
  // 1. If SPI tx interrupt captured (confirmed that POLL is sent)
  // 2. If SPI tx interrupt is not captured for some reason
  if (state == STATE_POLLACK
      && ((lastSent && curMillis - lastSent > POLLACK_TIMEOUT_MS)
          || curMillis - lastStateChange > 2 * POLLACK_TIMEOUT_MS)) {
    PRINTLN(F("POLLACK timeout"));
    //PRINTLN(F("  Return to ROUNDROBIN"));
    updateRoundRobin();
    return;
  }
  // Safety watchdog to avoid stuck in RANGEREPORT state
  // 1. If SPI tx interrupt captured (confirmed that RANGE is sent)
  // 2. If SPI tx interrupt is not captured for some reason
  if (state == STATE_RANGEREPORT
      && ((lastSent && curMillis - lastSent > RANGEREPORT_TIMEOUT_MS)
          || curMillis - lastStateChange > 2 * RANGEREPORT_TIMEOUT_MS)) {
    //PRINTLN(F("RANGEREPORT timeout"));
    //PRINTLN(F("  Return to ROUNDROBIN"));
    updateRoundRobin();
    return;
  }
  // Arduino didn't capture SIP tx/rx interrupts for more than RESET_TIMEOUT_MS
//  if (!sentFrame && !receivedFrame && curMillis - lastActivity > RESET_TIMEOUT_MS) {
//    ////PRINTLN(F("Seems transceiver not working. Re-init it."));
//    //initDW1000Receiver();
//    state = STATE_SCAN;
//    return;
//  }

  if (state == STATE_SCAN) {
    //PRINTLN(F("State: SCAN"));
    //PRINTLN(F("  Initializing arrays..."));
    for (idx_anchor = 0; idx_anchor < NUM_ANCHORS; idx_anchor++) {
      delete_link(uwb_data, anchorId[idx_anchor]);
      anchorId[idx_anchor] = ID_NONE;
      distance[idx_anchor] = 0;
    }
    idx_anchor = 0;
    num_anchors = 0;
    //PRINTLN(F("  Sending PING..."));
    transmitPing();
    updateState(STATE_PONG);
    return;
  }

  if (state == STATE_ROUNDROBIN) {
    //PRINTLN(F("State: ROUNDROBIN"));
    //PRINT(F("  idx_anchor: ")); PRINTLN(idx_anchor);
    if (idx_anchor < num_anchors) {
      PRINTLN(F("  Sending POLL..."));
      transmitPoll();
      updateState(STATE_POLLACK);
    } else {
      //PRINTLN(F("  Ranging all anchors. Return to IDLE"));
      PRINT(F("  send to server idx_anchor: ")); PRINTLN(idx_anchor);
      if (toSend) updateState(STATE_SEND_REQUEST_TO_SERVER);
      else updateState(STATE_SCAN);
    }
    return;
  }

  // SPI tx interrupt is captured
  // Update some time variables, state
  // Extract DW1000 high-precision time value if needed
  if (sentFrame) {
    //PRINTLN(F("Sent something"));
    sentFrame = false;
    noteActivity();
    lastSent = lastActivity;
    if (txBuffer[0] == FTYPE_PING) {
      //PRINTLN(F("  PING sent"));
    }
    if (txBuffer[0] == FTYPE_POLL) {
      //PRINTLN(F("  POLL sent. Getting timestamp..."));
      DW1000.getTransmitTimestamp(timePollSent);
    }
    if (txBuffer[0] == FTYPE_RANGE) {
      //PRINTLN(F("  RANGE sent. Getting timestamp..."));
      DW1000.getTransmitTimestamp(timeRangeSent);
    }
  }

  // SPI rx interrupt is captured
  //  Update some time variables, state
  // Extract DW1000 high-precision time value if needed
  if (receivedFrame) {
    //PRINTLN(F("Received something"));
    receivedFrame = false;
    noteActivity();
    DW1000.getData(rxBuffer, FRAME_LEN);

    if (state == STATE_PONG) {
      //PRINTLN(F("  State: PONG"));
      if (rxBuffer[0] != FTYPE_PONG) {
        //PRINTLN(F("    Not PONG"));
        return;
      }
      if (!DOES_DST_MATCH(rxBuffer, tagId, ADDR_SIZE)) {
        //PRINTLN(F("    Not for me"));
        return;
      }
      #warning "This may store anchors with the same ID"
      memcpy(&anchorId[idx_anchor], rxBuffer + 1, ADDR_SIZE);
      PRINT(F("    Anchor")); PRINT(idx_anchor); PRINT(F("(")); PRINT(anchorId[idx_anchor]); PRINTLN(F(") found"));
      //if (num_anchors < 2) {
      if (anchorId[idx_anchor] > 0) {
        num_anchors++;
        idx_anchor++;
      }
      //}
      //num_anchors = min(num_anchors, 5);
      //idx_anchor %= 2;
      return;
    }

    if (state == STATE_POLLACK) {
      //PRINTLN(F("  State: POLLACK"));
      if (rxBuffer[0] != FTYPE_POLLACK) {
        //PRINTLN(F("    Not POLLACK"));
        return;
      }
      if (!DOES_SRC_MATCH(rxBuffer, anchorId[idx_anchor], ADDR_SIZE)) {
        //PRINTLN(F("    Not from counter part"));
        return;
      }
      if (!DOES_DST_MATCH(rxBuffer, tagId, ADDR_SIZE)) {
        //PRINTLN(F("    Not for me"));
        return;
      }
      //PRINTLN(F("    Received POLLACK. Getting timestamp..."));
      DW1000.getReceiveTimestamp(timePollAckReceived);
      //PRINTLN(F("    Sending RANGE..."));
      transmitRange();
      updateState(STATE_RANGEREPORT);
      return;
    }

    if (state == STATE_RANGEREPORT) {
      //PRINTLN(F("  State: RANGEREPORT"));
      if (rxBuffer[0] != FTYPE_RANGEREPORT) {
        //PRINTLN(F("    Not RANGEREPORT"));
        return;
      }
      if (!DOES_SRC_MATCH(rxBuffer, anchorId[idx_anchor], ADDR_SIZE)) {
        //PRINTLN(F("    Not from countere part"));
        return;
      }
      if (!DOES_DST_MATCH(rxBuffer, tagId, ADDR_SIZE)) {
        //PRINTLN(F("    Not for me"));
        return;
      }
      //PRINTLN(F("    Received RANGEREPORT. Getting timestamps..."));
      timePollReceived.setTimestamp(rxBuffer + 5);
      timePollAckSent.setTimestamp(rxBuffer + 10);
      timeRangeReceived.setTimestamp(rxBuffer + 15);
      PRINTLN(F("    Calculating range..."));
      calculateRange();
      PRINT(F("  anchor ID: ")); PRINTLN(anchorId[idx_anchor]);
      PRINT(F("    distance: ")); PRINTLN(distance[idx_anchor]);
      add_link(uwb_data, anchorId[idx_anchor]);
      fresh_link(uwb_data, anchorId[idx_anchor], distance[idx_anchor]);
      toSend = true;
      
      updateRoundRobin();
      return;
    }
  }
}
