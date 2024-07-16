#include "arduino.h"

/*
* Set ID, Antenna delay and Reply delay for each anchor separately
* We operate with 4 anchors
* Reply delay should be different for each anchor,
*   otherwise it causes collisions in communications
*   3 ms difference is the best choise in practice 
*/ 
void setMyProperties()
{
  String macAddr = WiFi.macAddress();
  if (macAddr == "70:B8:F6:D8:F8:B8")
  {
    myID = 101;
    aDelay = 16392;
    replyDelay = 17000;
  }
  else if (macAddr == "70:B8:F6:D8:F8:28")
  {
    myID = 102;
    aDelay = 16384;
    replyDelay = 20000;
  }
  else if (macAddr == "D8:BC:38:42:D7:0C")
  {
    myID = 103;
    aDelay = 16372;
    replyDelay = 23000;
  }
  else if (macAddr == "70:B8:F6:D8:F6:24")
  {
    myID = 104;
    aDelay = 16392;
    replyDelay = 26000;
  }
}

// Safety check if the anchor is blocked
void checkForReset()
{
  currentTime = millis();
  // if nothing is received during the timeout
  if ((!sentAck && !receivedAck))
  {
    if ((currentTime - lastActivity) > DEFAULT_RESET_TIMEOUT)
    {
      // Reinit
      currentTagAddress = 0;
      isAnchorBusy = false; // ready to start new communication with a tag
      expectedMessageType = MSG_TYPE_POLL;
      initReceiver();
      noteActivity();
    }
    return;
  }
}

// Init as a receiver
void initReceiver()
{
  DW1000.newReceive();
  DW1000.setDefaults();
  DW1000.receivePermanently(true);
  DW1000.startReceive();
}

// Check for correct tag address 
bool checkTagAddress()
{
  if (0 < receivedMessage[1] && receivedMessage[1] < 99)
    return true;
  return false;
}

// Check if received message is from the expected tag
bool checkSource()
{
  if (checkTagAddress())
  {
    if (currentTagAddress == receivedMessage[1])
      return true;
  }
  return false;
}

// Chekc if the message is intended to this Anchor (me)
bool checkDestination()
{
  if (myID == receivedMessage[2])
    return true;
  expectedMessageType = MSG_TYPE_POLL;
  isAnchorBusy = false;
  return false;
}

bool checkSourceAndDestination()
{
  return checkSource() && checkDestination();
}

void prepareMessageToSend(byte messageType, byte source, byte destination)
{
  memcpy(currentMessage, &messageType, sizeof(messageType));
  memcpy(currentMessage + 1, &source, sizeof(source));
  memcpy(currentMessage + 2, &destination, sizeof(destination));

  prepareReplyDelay(); // send reply delay, so that the tag will delay for the same amount of time
}

void prepareReplyDelay()
{
  memcpy(currentMessage + 3, &replyDelay, 2);
}

void sendMessage(byte messageType)
{
  // Preparation for transmitting the message  
  DW1000.newTransmit();
  DW1000.setDefaults();

  if (messageType == MSG_TYPE_POLL_ACK)
  {
    prepareMessageToSend(MSG_TYPE_POLL_ACK, myID, currentTagAddress);
    pollackReplyDelay = DW1000Time(replyDelay, DW1000Time::MICROSECONDS);
    DW1000.setDelay(pollackReplyDelay);
  }
  else if (messageType == MSG_TYPE_RANGE_REPORT)
  {
    prepareMessageToSend(MSG_TYPE_RANGE_REPORT, myID, currentTagAddress);
    // send all time mesurements in one message -> buffer size is 20 
    timePollReceived.getTimestamp(currentMessage + 5);
    timePollAckSent.getTimestamp(currentMessage + 10);
    timeRangeReceived.getTimestamp(currentMessage + 15);
  }

  DW1000.setData(currentMessage, sizeof(currentMessage));

  // Send message to the tag
  DW1000.startTransmit();
}

void handleReceived()
{
  receivedAck = true;
}

void handleSent()
{
  sentAck = true;
}

void noteActivity()
{
  lastActivity = millis();
}

// Init as an Tag
void initAnchor()
{
  DW1000.begin(PIN_IRQ, PIN_RST);
  DW1000.select(PIN_SS);

  setMyProperties();

  DW1000.newConfiguration();
  DW1000.setDefaults();
  DW1000.setDeviceAddress(myID);
  DW1000.setNetworkId(networkId);
  DW1000.enableMode(DW1000.MODE_LONGDATA_RANGE_ACCURACY);
  DW1000.setAntennaDelay(aDelay);
  DW1000.commitConfiguration();

  DW1000.attachSentHandler(handleSent);
  DW1000.attachReceivedHandler(handleReceived);

  initReceiver();

  // Wait until everything is set up correctly
  currentTime = millis();
  while (millis() - currentTime < 1000)
  {
    continue;
  }
  noteActivity();
}

void setup()
{
  initAnchor();
}

void loop()
{
  checkForReset(); // safety check, if the Anchor works properly

  // If something has been sent
  if (sentAck)
  {
    sentAck = false;
    noteActivity();

    // Record the transmission time for DS-TWR 
    if (currentMessage[0] == MSG_TYPE_POLL_ACK)
    {
      DW1000.getTransmitTimestamp(timePollAckSent);
    }
  }

  // If something has been received
  if (receivedAck)
  {
    noteActivity();
    receivedAck = false;

    // Unpack the received message
    DW1000.getData(receivedMessage, sizeof(receivedMessage));

    if (receivedMessage[0] == MSG_TYPE_POLL && !isAnchorBusy && checkTagAddress())
    {
      isAnchorBusy = true;
      currentTagAddress = receivedMessage[1];
      DW1000.getReceiveTimestamp(timePollReceived); // record timestamp of reception for DS-TWR
      expectedMessageType = MSG_TYPE_RANGE;
      sendMessage(MSG_TYPE_POLL_ACK);
      return;
    }

    if (expectedMessageType == MSG_TYPE_RANGE && receivedMessage[0] == MSG_TYPE_RANGE && checkSourceAndDestination() && isAnchorBusy)
    {
      DW1000.getReceiveTimestamp(timeRangeReceived); // record timestamp of reception for DS-TWR
      expectedMessageType = MSG_TYPE_POLL;
      sendMessage(MSG_TYPE_RANGE_REPORT);
      isAnchorBusy = false; // it was last message to the tag, we are ready to communicate with a new one!
      return;
    }
    else
    {
      // If the received message is POLL (broadcast from a tag)
      //  the Anchor (me) should not acquire busy state
      //  because the tag might choose another anchor for the communication 
      if (receivedMessage[0] == MSG_TYPE_POLL)
        isAnchorBusy = false; 
    }
  }
}
