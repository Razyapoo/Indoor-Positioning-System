/*********************************************** Tag (ESP32 UWB) ******************************************
 * This firmware configures the ESP32 UWB device to operate as an Anchor
 * Should be loaded into each device separatelly!!
****************************************************************************************************************/

#include <SPI.h>
#include <WiFi.h>
#include <DW1000.h>

#include "debug.h"

#define PIN_IRQ 34 // GPIO Interrupt Request pin
#define PIN_RST 27 // GPIO Reset pin
#define PIN_SS 4 // GPIO Chip Select pin
#define FRAME_SIZE 20 // type (1), src (2), dst (2), time1 (5), time2 (5), time3 (5): total (20)

// Define message indicators
const byte MSG_TYPE_POLL = 1;
const byte MSG_TYPE_POLL_ACK = 2;
const byte MSG_TYPE_RANGE = 3;
const byte MSG_TYPE_RANGE_REPORT = 4;

#define DEFAULT_RESET_TIMEOUT       500

// DS-TWR measuring time
DW1000Time pollackReplyDelay;
DW1000Time timePollSent;
DW1000Time timePollReceived;
DW1000Time timePollAckSent;
DW1000Time timePollAckReceived;
DW1000Time timeRangeSent;
DW1000Time timeRangeReceived;

const uint16_t networkId = 10;

uint16_t myID; 
uint16_t aDelay; // antenna delay
uint16_t replyDelay; // how much to wait in between reception and transmittion of the signal between an anchor and a tag
unsigned long lastActivity, currentTime; // safety watchdogs

byte currentTagAddress;
byte currentMessage[FRAME_SIZE] = {0};
byte receivedMessage[FRAME_SIZE] = {0};
bool isAnchorBusy = false; // block interruption from other anchors
byte expectedMessageType = MSG_TYPE_POLL; // helps to determine the content of the next message to be received

// Handling events when something was sent / received
boolean sentAck = false; 
boolean receivedAck = false;

boolean debug = false; // debugging

// Function declarations
bool checkTagAddress();
bool checkSource();
bool checkDestination();
bool checkSourceAndDestination();

void setMyProperties();
void initAnchor();
void initReceiver();
void setup();

void handleReceived();
void handleSent();

void checkForReset();

void noteActivity();
void loop();

void prepareReplyDelay();
void prepareMessageToSend(byte messageType, byte source, byte destination);
void sendMessage(byte messageType);