#include "arduino.h"

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
  else if (macAddr == "70:B8:F6:D8:F6:24") // D8:BC:38:43:13:18
  {
    myID = 104;
    aDelay = 16392;
    replyDelay = 26000;
  }
}

void checkForReset()
{
  currentTime = millis();
  if ((!sentAck && !receivedAck))
  {
    if ((currentTime - lastActivity) > DEFAULT_RESET_TIMEOUT)
    {
      currentTagAddress = 0;
      isAnchorBusy = false;
      pollCounterForReset = 0;
      expectedMessageType = MSG_TYPE_POLL;
      initReceiver();
      noteActivity();
    }
    return;
  }
}

void initReceiver()
{
  DW1000.newReceive();
  DW1000.setDefaults();
  DW1000.receivePermanently(true);
  DW1000.startReceive();
}

bool checkTagAddress()
{
  if (0 < receivedMessage[1] && receivedMessage[1] < 99)
    return true;
  return false;
}

bool checkSource()
{
  if (checkTagAddress())
  {
    if (currentTagAddress == receivedMessage[1])
      return true;
  }
  return false;
}

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

  prepareReplyDelay();
}

void prepareReplyDelay()
{
  memcpy(currentMessage + 3, &replyDelay, 2);
}

void sendMessage(byte messageType)
{
  DW1000.newTransmit();
  DW1000.setDefaults();

  if (messageType == MSG_TYPE_POLL_ACK)
  {

    //replyDelay = (2 * (myID - 100) + 1) * DEFAULT_REPLY_DELAY_TIME;
//    replyDelay = DEFAULT_REPLY_DELAY_TIME + (myID - 100) * 3 * 1000;

    prepareMessageToSend(MSG_TYPE_POLL_ACK, myID, currentTagAddress);
    pollackReplyDelay = DW1000Time(replyDelay, DW1000Time::MICROSECONDS);
    DW1000.setDelay(pollackReplyDelay);
  }
  else if (messageType == MSG_TYPE_RANGE_REPORT)
  {
    prepareMessageToSend(MSG_TYPE_RANGE_REPORT, myID, currentTagAddress);
    timePollReceived.getTimestamp(currentMessage + 5);
    timePollAckSent.getTimestamp(currentMessage + 10);
    timeRangeReceived.getTimestamp(currentMessage + 15);
  }

  DW1000.setData(currentMessage, sizeof(currentMessage));

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

  //  delay(1000);/
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
  checkForReset();
  if (sentAck)
  {
    sentAck = false;
    noteActivity();

    if (currentMessage[0] == MSG_TYPE_POLL_ACK)
    {
      DW1000.getTransmitTimestamp(timePollAckSent);
    }
  }

  if (receivedAck)
  {
    noteActivity();
    receivedAck = false;

    DW1000.getData(receivedMessage, sizeof(receivedMessage));

    if (receivedMessage[0] == MSG_TYPE_POLL && !isAnchorBusy && checkTagAddress())
    {
      isAnchorBusy = true;
      currentTagAddress = receivedMessage[1];
      DW1000.getReceiveTimestamp(timePollReceived);
      expectedMessageType = MSG_TYPE_RANGE;
      sendMessage(MSG_TYPE_POLL_ACK);
      return;
    }

    if (expectedMessageType == MSG_TYPE_RANGE && receivedMessage[0] == MSG_TYPE_RANGE && checkSourceAndDestination() && isAnchorBusy)
    {
      DW1000.getReceiveTimestamp(timeRangeReceived);
      expectedMessageType = MSG_TYPE_POLL;
      sendMessage(MSG_TYPE_RANGE_REPORT);
      isAnchorBusy = false;

      return;
    }
    else
    {
      if (receivedMessage[0] == MSG_TYPE_POLL)
        isAnchorBusy = false;
    }
  }
}
