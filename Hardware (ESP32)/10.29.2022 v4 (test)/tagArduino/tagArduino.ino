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
const char *ssid = "ASUS";
const char *password = "cuni3103&";
const char *host = "192.168.1.55";
//const char *ssid = "iPhone Nyx";
//const char *password = "Nera1998&";
//const char *host = "172.20.10.3";
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
// Array of distance measurements of the corresponding anchors
float distance[NUM_ANCHORS] = {0, 0};

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


  if (state == STATE_SCAN) {
    PRINTLN(F("SCANNING..."));
    for (idx_anchor = 0; idx_anchor < NUM_ANCHORS; idx_anchor++) {
      delete_link(uwb_data, anchorId[idx_anchor]);
      anchorId[idx_anchor] = ID_NONE;
      distance[idx_anchor] = 0;
    }

    idx_anchor = 0;
    num_anchors = 0;
    PRINTLN(F("Send ping"));
    transmitPing();
    PRINTLN(F("Update state to PONG"));
    updateState(STATE_PONG);
    return;
  }

  if (state == STATE_PONG
      && ((lastSent && curMillis - lastSent > PONG_TIMEOUT_MS)
          || curMillis - lastStateChange > 2 * PONG_TIMEOUT_MS)) {

    if (num_anchors > 0) {
      PRINT(num_anchors); PRINTLN(F(" are detected. Go to ROUNDROBIN state"));
      idx_anchor = 0;
      updateState(STATE_ROUNDROBIN);
      return;

    } else {
      PRINTLN(F("No anchor is detected. Return to SCAN state..."));
      updateState(STATE_SCAN);
      return;

    }
  }

  if (state == STATE_ROUNDROBIN) {

    if (idx_anchor < num_anchors) {
      PRINTLN(F("  Sending POLL..."));
      transmitPoll();
      updateState(STATE_POLLACK);

    } else {
      PRINTLN(F("  ROUNDROBIN is finished."));
      if (toSend) updateState(STATE_SEND_REQUEST_TO_SERVER);
      else updateState(STATE_SCAN);

    }
    return;
  }

  // Safety watchdog to avoid stuck in POLLACK state
  if (state == STATE_POLLACK
      && ((lastSent && curMillis - lastSent > POLLACK_TIMEOUT_MS)
          || curMillis - lastStateChange > 2 * POLLACK_TIMEOUT_MS)) {
    PRINTLN(F("POLLACK timeout.. No POLLACK were received..."));
    updateRoundRobin();
    return;
  }

  if (state == STATE_RANGEREPORT
      && ((lastSent && curMillis - lastSent > RANGEREPORT_TIMEOUT_MS)
          || curMillis - lastStateChange > 2 * RANGEREPORT_TIMEOUT_MS)) {
    PRINTLN(F("RANGEREPORT timeout.. No RANGEREPORT were received..."));
    updateRoundRobin();
    return;
  }







  // Safety watchdog to avoid stuck in PONG state
  // 1. If SPI tx interrupt captured (confirmed that PING is broadcasted)
  // 2. If SPI tx interrupt is not captured for some reason

    /* Send data to server */
  if (state == STATE_SEND_REQUEST_TO_SERVER) {
    PRINTLN(F("Server request"));
    if (client.connected()) { 
//      /PRINTLN((const char*)(tagId) + (const char*)SIG_REQUEST);
      //char* msg = (char*)tagId;
      //PRINTLN(msg);
      //msg[sizeof(msg)] = '3';
      client.print(String(tagId) + "3");
      updateState(STATE_REQUEST_ACK_FROM_SERVER);
    } else {
      PRINTLN(F("Server is not available."));
      updateState(STATE_SCAN);
    }
  }

  if (state == STATE_REQUEST_ACK_FROM_SERVER) {
    ack = "";
    delay(30);
    // while (client.available()) { 
    //    delay(3);
    //    char c = client.read();
    //    ack += c;
    // } 
    if (client.available()) ack = client.read();
    PRINTLN(F("ACK: "));
    PRINTLN(ack);
    if (ack == "53") {
        PRINTLN(F("Server response - ACCEPTED"));
        updateState(STATE_SEND_DISTANCE_TO_SERVER);
      }
    else {
      PRINTLN(F("Server does't respond. Probably connection lost."));
      ack = "";
      updateState(STATE_SCAN);
    }
  }

  if (state == STATE_SEND_DISTANCE_TO_SERVER) {
    //delay(100);
    make_link_json(uwb_data, &all_json);
    client.print(all_json);
    ack = "";
    delay(30);
    // while (client.available()) { 
    //    delay(3);
    //    char c = client.read();
    //    ack += c;
    // }
    if (client.available()) ack = client.read();
    PRINTLN(F("ACK: "));
    PRINTLN(ack);
    if (ack == "55") {
      toSend = false;
      PRINTLN(F("  UWB package sent"));
      ack = "";
    } else {
      PRINTLN(F("Server does't respond. Data was not send."));
      ack = "";
    }
    updateState(STATE_SCAN);
  }
  
  // Arduino didn't capture SIP tx/rx interrupts for more than RESET_TIMEOUT_MS
//  if (!sentFrame && !receivedFrame && curMillis - lastActivity > RESET_TIMEOUT_MS) {
//    ////PRINTLN(F("Seems transceiver not working. Re-init it."));
//    //initDW1000Receiver();
//    state = STATE_SCAN;
//    return;
//  }

  

  // SPI tx interrupt is captured
  // Update some time variables, state
  // Extract DW1000 high-precision time value if needed
  if (sentFrame) {
    //PRINTLN(F("Sent something"));
    sentFrame = false;
    noteActivity();
    lastSent = lastActivity;
    if (txBuffer[0] == FTYPE_PING) {
      PRINTLN(F("  PING sent"));
    }
    
    if (txBuffer[0] == FTYPE_POLL) {
      PRINTLN(F("  POLL sent. Getting timestamp..."));
      DW1000.getTransmitTimestamp(timePollSent);
    }
    
    if (txBuffer[0] == FTYPE_RANGE) {
      PRINTLN(F("  RANGE sent. Getting timestamp..."));
      DW1000.getTransmitTimestamp(timeRangeSent);
    }
  }

  // SPI rx interrupt is captured
  //  Update some time variables, state
  // Extract DW1000 high-precision time value if needed
  if (receivedFrame) {
    receivedFrame = false;
    noteActivity();
    DW1000.getData(rxBuffer, FRAME_LEN);

    if (state == STATE_PONG) {
      if (rxBuffer[0] != FTYPE_PONG) {
        return;
      }
      if (!DOES_DST_MATCH(rxBuffer, tagId, ADDR_SIZE)) {
        return;
      }

      #warning "This may store anchors with the same ID"
      memcpy(&anchorId[idx_anchor], rxBuffer + 1, ADDR_SIZE);
      PRINT(F("    Anchor")); PRINT(idx_anchor); PRINT(F("(")); PRINT(anchorId[idx_anchor]); PRINTLN(F(") found"));
      add_link(uwb_data, anchorId[idx_anchor]);
      
      if (anchorId[idx_anchor] > 0) {
        num_anchors++;
        idx_anchor++;
      }

      return;
    }

    if (state == STATE_POLLACK) {
      if (rxBuffer[0] != FTYPE_POLLACK) {
        return;
      }
      if (!DOES_SRC_MATCH(rxBuffer, anchorId[idx_anchor], ADDR_SIZE)) {
        return;
      }
      if (!DOES_DST_MATCH(rxBuffer, tagId, ADDR_SIZE)) {
        return;
      }
      DW1000.getReceiveTimestamp(timePollAckReceived);
      transmitRange();
      updateState(STATE_RANGEREPORT);
      return;
    }

    if (state == STATE_RANGEREPORT) {
      if (rxBuffer[0] != FTYPE_RANGEREPORT) {
        return;
      }
      if (!DOES_SRC_MATCH(rxBuffer, anchorId[idx_anchor], ADDR_SIZE)) {
        return;
      }
      if (!DOES_DST_MATCH(rxBuffer, tagId, ADDR_SIZE)) {
        return;
      }
      timePollReceived.setTimestamp(rxBuffer + 5);
      timePollAckSent.setTimestamp(rxBuffer + 10);
      timeRangeReceived.setTimestamp(rxBuffer + 15);
      PRINTLN(F("    Calculating range..."));
      calculateRange();
      PRINT(F("  anchor ID: ")); PRINTLN(anchorId[idx_anchor]);
      PRINT(F("    distance: ")); PRINTLN(distance[idx_anchor]);

      fresh_link(uwb_data, anchorId[idx_anchor], distance[idx_anchor]);
      toSend = true;
      txBuffer[0] = FTYPE_NONE;
      rxBuffer[0] = FTYPE_NONE;
      
      updateRoundRobin();
      return;
    }
  }
}
