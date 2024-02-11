#include "arduino.h"

uint16_t getMyID()
{
  String macAddr = WiFi.macAddress();

  if (macAddr == "D8:BC:38:42:D7:0C") //"70:B8:F6:D8:F6:48"
  {
    return 1;
  }
  else if (macAddr == "70:B8:F6:D8:F6:24")
  {
    return 2;
  }
  else if (macAddr == "70:B8:F6:D8:F6:60")
  {
    return 3;
  }
  else
  {
    return 0;
  }
}

void checkForReset()
{
  currentTime = millis();
  if (!sentAck && !receivedAck)
  {
    if (((currentTime - lastActivity) > DEFAULT_RESET_TIMEOUT) && isRequestFromServerReceived)
    {
      for (size_t i = 0; i < MAX_ANCHORS; i++)
        discoveredAnchors[i] = 0;
      discoveredAnchorsCount = 0;
      initReceiver();
      isTagBusy = true;
      expectedMessageType = MSG_TYPE_POLL_ACK;
      sendMessage(MSG_TYPE_POLL);
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

void connectToWiFi()
{
  WiFi.mode(WIFI_MODE_STA);
  WiFi.setSleep(false);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(300);
  }
}

void connectToServer()
{
  while (!client.connect(host, 30001))
  {
    delay(500);
  }
}

void sendDistancesToServer()
{
  int position = sprintf(msgToSend, "%d %d %f", myID, discoveredAnchors[0], distances[0]);

  if (discoveredAnchorsCount > 1)
  {
    for (int i = 1; i < discoveredAnchorsCount; i++)
    {
      position += sprintf(msgToSend + position, " %d %f", discoveredAnchors[i], distances[i]);
    }
  }

  sprintf(msgToSend + position, "\n");

  client.print(msgToSend);
}

bool isAnchorAddress()
{
  if (100 < receivedMessage[1] && receivedMessage[1] < 199)
    return true;

  return false;
}

bool checkSource()
{
  if (isAnchorAddress() && currentAnchorAddress == receivedMessage[1])
    return true;

  return false;
}

bool checkDestination()
{
  if (myID == receivedMessage[2])
    return true;

  return false;
}

bool checkSourceAndDestination()
{
  return checkSource() && checkDestination();
}

float computeRangeAsymmetric()
{
  DW1000Time round1 = (timePollAckReceived - timePollSent).wrap();
  DW1000Time reply1 = (timePollAckSent - timePollReceived).wrap();
  DW1000Time round2 = (timeRangeReceived - timePollAckSent).wrap();
  DW1000Time reply2 = (timeRangeSent - timePollAckReceived).wrap();
  DW1000Time tof = (round1 * round2 - reply1 * reply2) / (round1 + round2 + reply1 + reply2);
  return tof.getAsMeters();
}

void prepareMessageToSend(byte messageType, byte source, byte destination)
{
  memcpy(currentMessage, &messageType, sizeof(messageType));
  memcpy(currentMessage + 1, &source, sizeof(source));
  memcpy(currentMessage + 2, &destination, sizeof(destination));
}

void prepareMessageToSend(byte messageType, byte source)
{
  memcpy(currentMessage, &messageType, sizeof(messageType));
  memcpy(currentMessage + 1, &source, sizeof(source));
}

void setReplyDelay()
{
  memcpy(&replyDelay, receivedMessage + 3, 2);
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

void sendMessage(byte messageType)
{

  DW1000.newTransmit();
  DW1000.setDefaults();

  if (messageType == MSG_TYPE_POLL)
  {
    prepareMessageToSend(MSG_TYPE_POLL, myID);
  }
  else if (messageType == MSG_TYPE_RANGE)
  {

    prepareMessageToSend(MSG_TYPE_RANGE, myID, currentAnchorAddress);
    setReplyDelay();
    rangeReplyDelay = DW1000Time(replyDelay, DW1000Time::MICROSECONDS);
    DW1000.setDelay(rangeReplyDelay);
  }

  DW1000.setData(currentMessage, sizeof(currentMessage));

  DW1000.startTransmit();
  blinkTimer = millis();
  discoveryTimer = millis();
}

bool isAnchorDiscovered()
{
  for (size_t i = 0; i < MAX_ANCHORS; i++)
  {
    if (discoveredAnchors[i] == receivedMessage[1])
    {
      return true;
    }
  }

  return false;
}

void initTag()
{
  DW1000.begin(PIN_IRQ, PIN_RST);
  DW1000.select(PIN_SS);

  myID = getMyID();

  DW1000.newConfiguration();
  DW1000.setDefaults();
  DW1000.setNetworkId(networkId);
  DW1000.setDeviceAddress(myID);
  DW1000.enableMode(DW1000.MODE_LONGDATA_RANGE_ACCURACY);
  DW1000.setAntennaDelay(16536);
  DW1000.commitConfiguration();

  DW1000.attachSentHandler(handleSent);
  DW1000.attachReceivedHandler(handleReceived);

  initReceiver();

  currentTime = millis();
  while (millis() - currentTime < 1000)
  {
    continue;
  }
  noteActivity();
}

void setup()
{
  initTag();

  connectToWiFi();
  connectToServer();
}

void loop()
{

  checkForReset();
  if (!client.connected())
  {
    connectToServer();
    noteActivity();
  }

  if (client.available() && !isRequestFromServerReceived)
  {
    serverRequest = client.readStringUntil('\n');
    if (serverRequest == "1")
    {
      for (size_t i = 0; i < MAX_ANCHORS; i++)
        discoveredAnchors[i] = 0;
      discoveredAnchorsCount = 0;
      isRequestFromServerReceived = true;
      currentAnchorAddress = 0;
      isTagBusy = true;
      expectedMessageType = MSG_TYPE_POLL_ACK;
      sendMessage(MSG_TYPE_POLL);
      noteActivity();
      return;
    }
  }

  if (isRequestFromServerReceived)
  {
    if (sentAck)
    {
      noteActivity();
      sentAck = false;

      if (currentMessage[0] == MSG_TYPE_POLL)
      {
        DW1000.getTransmitTimestamp(timePollSent);
      }

      if (currentMessage[0] == MSG_TYPE_RANGE)
      {
        DW1000.getTransmitTimestamp(timeRangeSent);
      }
    }

    if (receivedAck)
    {
      receivedAck = false;
      noteActivity();

      DW1000.getData(receivedMessage, sizeof(receivedMessage));

      if (expectedMessageType == receivedMessage[0])
      {
        if (isTagBusy)
        {
          if (expectedMessageType == MSG_TYPE_POLL_ACK && checkDestination())
          {
            if (!isAnchorDiscovered() && isAnchorAddress())
            {
              discoveredAnchors[discoveredAnchorsCount++] = receivedMessage[1];
              currentAnchorAddress = receivedMessage[1];
              DW1000.getReceiveTimestamp(timePollAckReceived);
              expectedMessageType = MSG_TYPE_RANGE_REPORT;
              sendMessage(MSG_TYPE_RANGE);
              noteActivity();
            }
            return;
          }

          if (expectedMessageType == MSG_TYPE_RANGE_REPORT && checkSourceAndDestination())
          {
            timePollReceived.setTimestamp(receivedMessage + 5);
            timePollAckSent.setTimestamp(receivedMessage + 10);
            timeRangeReceived.setTimestamp(receivedMessage + 15);
            float distance = computeRangeAsymmetric();

            distances[discoveredAnchorsCount - 1] = distance;
            isTagBusy = false;
            if (discoveredAnchorsCount < MIN_ANCHORS)
            {
              currentAnchorAddress = 0;
              isTagBusy = true;
              expectedMessageType = MSG_TYPE_POLL_ACK;
              sendMessage(MSG_TYPE_POLL);
              noteActivity();
            }
            return;
          }
        }
      }
    }

    discoveryTimeout = millis();
    if (!isTagBusy && (discoveredAnchorsCount >= MIN_ANCHORS || discoveryTimeout - discoveryTimer > BLINK_DELAY))
    {

      sendDistancesToServer();

      while (client.connected() && !client.available())
        continue;

      ack = client.readStringUntil('\n');
      isRequestFromServerReceived = false;
      for (size_t i = 0; i < MAX_ANCHORS; i++)
        discoveredAnchors[i] = 0;
      discoveredAnchorsCount = 0;
      discoveryTimer = discoveryTimeout;
      noteActivity();
      return;
    }

    blinkCurrentMillis = millis();
    if (!isTagBusy && discoveredAnchorsCount < MIN_ANCHORS && blinkCurrentMillis - blinkTimer > BLINK_DELAY)
    {
      blinkTimer = blinkCurrentMillis;
      currentAnchorAddress = 0;
      isTagBusy = true;
      expectedMessageType = MSG_TYPE_POLL_ACK;
      sendMessage(MSG_TYPE_POLL);
      noteActivity();
    }
  }
}
