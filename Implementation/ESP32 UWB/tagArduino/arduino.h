
/*********************************************** Tag (ESP32 UWB) ******************************************
 * This firmware configures the ESP32 UWB device to operate as a Tag
 * Should be loaded into each device separatelly!!
****************************************************************************************************************/

#include <SPI.h>
#include <DW1000.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFiServer.h>
#include <WiFiClient.h>

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

// Notes about anchors
const size_t MAX_ANCHORS = 2;
const size_t MIN_ANCHORS = 2;
byte discoveredAnchors[MAX_ANCHORS] = {0};
size_t discoveredAnchorsCount = 0;
byte currentAnchorAddress;

bool isTagBusy = false; // blocks interruption from other anchors

#define DEFAULT_RESET_TIMEOUT 500
#define BLINK_DELAY 80 // timeout for emitting some event: send distance to the server, or send requests to anchors to initiate communication 

// WiFi parameters
WiFiClient client;
char ssid[] = "oskar-hotspot";
const char *password = "Nera1998";
const char *host = "10.42.0.1";

// DS-TWR measuring time
DW1000Time rangeReplyDelay;
DW1000Time timePollSent;
DW1000Time timePollReceived;
DW1000Time timePollAckSent;
DW1000Time timePollAckReceived;
DW1000Time timeRangeSent;
DW1000Time timeRangeReceived;

const uint16_t networkId = 10;

String ack = ""; // status of communication

uint16_t myID; 
uint16_t aDelay; // antenna delay
uint16_t replyDelay; // how much to wait in between reception and transmittion of the signal between an anchor and a tag
unsigned long lastActivity, currentTime; // safety watchdogs

float distances[MAX_ANCHORS] = {0};

// Buffers for message content
byte currentMessage[FRAME_SIZE] = {0};
byte receivedMessage[FRAME_SIZE] = {0};
byte expectedMessageType; // what to expect as a next message

// Timers
unsigned long blinkTimer, blinkCurrentMillis, discoveryTimeout, discoveryTimer;

// Communication with server
String serverRequest;
bool isRequestFromServerReceived = false;
char msgToSend[30];

// Handling events when something was sent / received
bool sentAck = false;
bool receivedAck = false;

// Function declarations
float computeRangeAsymmetric();

bool checkSource();
bool checkDestination();
bool checkSourceAndDestination();
bool isAnchorAddress();
bool isAnchorDiscovered();

void setMyProperties();
void initTag();
void initReceiver();
void setup();
void setReplyDelay();
void handleReceived();
void handleSent();

void connectToWiFi();
void connectToServer();

void noteActivity();
void loop();

void checkForReset();

void prepareMessageToSend(byte messageType, byte source, byte destination);
void prepareMessageToSend(byte messageType, byte source);

void sendMessage(byte messageType);
void sendDistancesToServer();