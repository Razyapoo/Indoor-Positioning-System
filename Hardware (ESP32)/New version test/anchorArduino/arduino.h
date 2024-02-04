#define DEBUG false
#include <SPI.h>
#include <WiFi.h>
#include <DW1000.h>

#include "debug.h"

/* Define chip select, reset and interrupt pins for ESP32 desk */
#define PIN_IRQ  34
#define PIN_RST  27
#define PIN_SS  4

// Macros in order to avoid hard coding
//#define STATE_IDLE          0
//#define STATE_DISCOVERY     1
//#define STATE_RANGING_INIT  2 
//#define STATE_RANGING       3
//#define STATE_POLL          4
//#define STATE_POLLACK       5
//#define STATE_RANGE         6
//#define STATE_RANGEREPORT   7

//#define BLINK       0
//#define RANGINGINIT 1
//#define POLL        2
//#define POLLACK     3
//#define RANGE       4
//#define RANGEREPORT 5

const byte MSG_TYPE_BLINK = 1;
const byte MSG_TYPE_ANCHOR_ADDR = 2;
const byte MSG_TYPE_POLL = 3;
const byte MSG_TYPE_POLL_ACK = 4;
const byte MSG_TYPE_RANGE = 5;
const byte MSG_TYPE_RANGE_REPORT = 6;

#define setSenderAddr(frame, addr) memcpy(frame + 1, &addr, 2)
#define setReceiverAddr(frame, addr) memcpy(frame + 3, &addr, 2)
#define getSenderAddr(frame, addr) memcpy(&addr, frame + 1, 2)
#define isSenderMatch(frame, addr) !memcmp(frame + 1, &addr, 2)
#define isReceiverMatch(frame, addr) !memcmp(frame + 3, &addr, 2)

// type (1), src (2), dst (2), time1 (5), time2 (5), time3 (5): total (20)
// type (1), src (1), dst (1), time1 (5), time2 (5), time3 (5): total (18)
#define FRAME_SIZE 20

// Parameters are tunable, but anchors and tags must have the same values
#warning "TODO: Temporary values"
/* Close setting */
//#define DISCOVERY_TIMEOUT           100
//#define POLLACK_TIMEOUT             10
//#define RANGE_TIMEOUT               10
//#define RANGEREPORT_TIMEOUT         10
//#define RANGING_INIT_TIMEOUT        10
//#define DEFAULT_REPLY_DELAY_TIME                 3
//#define DEFAULT_RESET_TIMEOUT       1000
//#define SERVER_TIMEOUT_MS           20
//#define SLEEP                       100//300

/* distance setting */
#define DISCOVERY_TIMEOUT           100
#define POLLACK_TIMEOUT             10
#define RANGE_TIMEOUT               10
#define RANGEREPORT_TIMEOUT         10
#define RANGING_INIT_TIMEOUT        10
#define DEFAULT_REPLY_DELAY_TIME    7000
#define DEFAULT_RESET_TIMEOUT       200
#define HARD_RESET_TIMEOUT          3000
#define SERVER_TIMEOUT_MS           20
#define SLEEP                       100//300


/* time */ 
//DW1000Time pollackReplyDelay = DW1000Time(DEFAULT_REPLY_DELAY_TIME, DW1000Time::MICROSECONDS);
DW1000Time pollackReplyDelay;
DW1000Time timePollSent;
DW1000Time timePollReceived;
DW1000Time timePollAckSent;
DW1000Time timePollAckReceived;
DW1000Time timeRangeSent;
DW1000Time timeRangeReceived;
DW1000Time tempTime;

/* const */
const uint16_t networkId = 10;

/* */
uint16_t myID, tagID; // range of anchor ids: 100..199, tag: 0..99
uint16_t Adelay = 16000;
uint16_t replyDelay;
size_t pollCounterForReset = 0;
unsigned long lastSent, lastActivity, currentTime, lastStateChange, rangingInitDelay, runtimeDelay;

//byte state = STATE_IDLE;
byte currentTagAddress;
const byte broadcastAddress = 255;
byte message[FRAME_SIZE] = {0};
byte currentMessage[FRAME_SIZE] = {0};
byte receivedMessage[FRAME_SIZE] = {0};
bool isAnchorBusy = false;
byte expectedMessageType = MSG_TYPE_POLL;
// byte rxFrame[FRAME_SIZE];

/* volatile */
boolean sentAck = false;
boolean receivedAck = false;
boolean protocolFailed = false;
boolean repeatedSoftInit = false;
boolean debug = true;
