/* Include */
#include <SPI.h>
#include <WiFi.h>
#include <DW1000.h>

/* Define chip select, reset and interrupt pins for ESP32 desk */
#define PIN_IRQ  34
#define PIN_RST  27
#define PIN_SS  4

#define DISCOVERY_TIMEOUT                   30
#define RANGING_INIT_TIMEOUT                5
#define RANGE_TIMEOUT                       10
#define DEFAULT_RESET_TIMEOUT               300
#define DEFAULT_POLL_ACK_REPLY_DELAY_TIME   3

#define STATE_IDLE              0
#define STATE_DISCOVERY         1
#define STATE_RANGING_INIT      2
#define STATE_RANGING           3
#define STATE_POLLACK           4
#define STATE_RANGE             5
#define STATE_RANGEREPORT       6

#define BLINK           0
#define RANGINGINIT     1
#define POLL            2
#define POLLACK         3
#define RANGE           4
#define RANGEREPORT     5

#define FRAME_SIZE 20

#define setSenderAddr(frame, addr) memcpy(frame + 1, &addr, 2)
#define setReceiverAddr(frame, addr) memcpy(frame + 3, &addr, 2)
#define getSenderAddr(frame, addr) memcpy(&addr, frame + 1, 2)
#define isSenderMatch(frame, addr) !memcmp(frame + 1, &addr, 2)
#define isReceiverMatch(frame, addr) !memcmp(frame + 3, &addr, 2)

/* const */
const uint16_t networkId = 10;


/* volatile */
volatile boolean sentAck = false;
volatile boolean receivedAck = false;

/* time */
DW1000Time pollackReplyDelay = DW1000Time(DEFAULT_POLL_ACK_REPLY_DELAY_TIME, DW1000Time::MILLISECONDS);
DW1000Time timePollSent;
DW1000Time timePollReceived;
DW1000Time timePollAckSent;
DW1000Time timePollAckReceived;
DW1000Time timeRangeSent;
DW1000Time timeRangeReceived;

uint32_t lastActivity;
uint32_t currentTime;
uint32_t lastStateChange;
uint32_t rangingInitDelay;

/* */
uint16_t myID = 101; // range of anchor ids is 100..199
uint16_t tagID;
byte txFrame[FRAME_SIZE];
byte rxFrame[FRAME_SIZE];
byte state = STATE_IDLE;
byte sentMessageType, receivedMessageType;
