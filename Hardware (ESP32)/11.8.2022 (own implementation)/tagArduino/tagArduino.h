/* Include */
#include <DW1000.h>
#include <SPI.h>
#include <iterator>
//#include <map>

/* Define chip select, reset and interrupt pins for ESP32 desk */
#define PIN_IRQ  34
#define PIN_RST  27
#define PIN_SS  4

#define DISCOVERY_TIMEOUT               30
#define POLLACK_TIMEOUT                 30
#define RANGEREPORT_TIMEOUT             30
#define DEFAULT_RESET_TIMEOUT           300
#define DEFAULT_RANGE_REPLY_DELAY_TIME  3
#define SLEEP                           50

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


typedef struct anchor {
    uint16_t ID;
    float distance; 
}
/* const */
const uint16_t networkId = 10;


/* volatile */
volatile boolean sentAck = false;
volatile boolean receivedAck = false;
// volatile byte expectedMsgType;

/* time */
// DW1000Time reply_delay = DW1000Time(DEFAULT_POLL_ACK_REPLY_DELAY_TIME, DW1000Time::MICROSECONDS);
DW1000Time rangeReplyDelay = DW1000Time(DEFAULT_RANGE_REPLY_DELAY_TIME, DW1000Time::MILLISECONDS);
DW1000Time timePollSent;
DW1000Time timePollReceived;
DW1000Time timePollAckSent;
DW1000Time timePollAckReceived;
DW1000Time timeRangeSent;
DW1000Time timeRangeReceived;

int32_t timer = 0;
// int16_t blinkCounter;
uint32_t lastActivity;
uint32_t currentTime;
uint32_t lastStateChange;

/* */
byte txFrame[FRAME_SIZE];
byte rxFrame[FRAME_SIZE];
byte state = STATE_IDLE;
byte sentMessageType, receivedMessageType;

uint16_t myID; //range of tag ids is 0..99
uint16_t anchorID;
uint16_t Adelay = 16592;
std::map<uint16_t, float> anchors;
std::map<uint16_t, float>::iterator anchor;

// uint8_t counterForBlink;
// uint8_t counterForPoll;
