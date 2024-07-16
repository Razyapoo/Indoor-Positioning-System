#include "arduino.h"

// Set ID and Antenna delay for each tag separately
// we operate with 3 tags
void setMyProperties()
{
  String macAddr = WiFi.macAddress();

  if (macAddr == "D8:BC:38:42:FB:74")
  {
    aDelay = 16526;
    myID = 1;
  }
  else if (macAddr == "D8:BC:38:42:FF:5C")
  {
    aDelay = 16532;
    myID = 2;
  }
  else if (macAddr == "D8:BC:38:42:F3:C4")
  {
    aDelay = 16524;
    myID = 3;
  }
  
}

// Safety check if the tag is blocked
// If the tag is reset like this, it does not interrupt the connection with the server
// instead it tries to reach the anchor again
void checkForReset()
{
  currentTime = millis();
  // if nothing is received during the timeout
  if (!sentAck && !receivedAck) 
  {
    if (((currentTime - lastActivity) > DEFAULT_RESET_TIMEOUT) && isRequestFromServerReceived)
    {
      // Reinit
      for (size_t i = 0; i < MAX_ANCHORS; i++)
        discoveredAnchors[i] = 0;
      discoveredAnchorsCount = 0;
      initReceiver();
      isTagBusy = true; // start communicating with anchors again
      expectedMessageType = MSG_TYPE_POLL_ACK;
      sendMessage(MSG_TYPE_POLL);
      noteActivity(); // activity watchdog
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
    delay(500); // wait until repeating
  }
}

void sendDistancesToServer()
{
  // Prepare message with measured distances
  // *position* is helping structure for forming the message
  int position = sprintf(msgToSend, "%d %d %f", myID, discoveredAnchors[0], distances[0]);

  if (discoveredAnchorsCount > 1)
  {
    for (int i = 1; i < discoveredAnchorsCount; i++)
    {
      position += sprintf(msgToSend + position, " %d %f", discoveredAnchors[i], distances[i]);
    }
  }

  sprintf(msgToSend + position, "\n");

  // Send distances to the server
  client.print(msgToSend);
}

// Check for correct anchor address 
bool isAnchorAddress()
{
  if (100 < receivedMessage[1] && receivedMessage[1] < 199)
    return true;

  return false;
}

// Check if received message is from the expected anchor
bool checkSource()
{
  if (isAnchorAddress() && currentAnchorAddress == receivedMessage[1])
    return true;

  return false;
}

// Chekc if the message is intended to this Tag (me)
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

/* Asymmetric DS-TWR:
*  - we do not expect the reply time to be symmetric for anchor and tag
*  - we assume the possibility of clock drifts
*/
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

// broadcast
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

// Activity watchdog
// helps to detect if the tag is blocked in communication
void noteActivity()
{
  lastActivity = millis();
}

void sendMessage(byte messageType)
{
  // Preparation for transmitting the message  
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
  
  // Send message to the anchor
  DW1000.startTransmit();
  blinkTimer = millis();
  discoveryTimer = millis();
}

// Check if the anchor has already been detected
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

// Init as a Tag
void initTag()
{
  DW1000.begin(PIN_IRQ, PIN_RST);
  DW1000.select(PIN_SS);

  setMyProperties();

  DW1000.newConfiguration();
  DW1000.setDefaults();
  DW1000.setNetworkId(networkId);
  DW1000.setDeviceAddress(myID);
  DW1000.enableMode(DW1000.MODE_LONGDATA_RANGE_ACCURACY);
  DW1000.setAntennaDelay(aDelay);
  DW1000.commitConfiguration();

  DW1000.attachSentHandler(handleSent);
  DW1000.attachReceivedHandler(handleReceived);

  initReceiver();

  // Wait until everything is set up correctly
  // delay() is not working correctly
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

  // If the server and tag are free: let's communicate!
  if (client.available() && !isRequestFromServerReceived)
  {
    serverRequest = client.readStringUntil('\n');
    if (serverRequest == "1") // received "Measure!" request from the server
    {
      for (size_t i = 0; i < MAX_ANCHORS; i++)
        discoveredAnchors[i] = 0;
      discoveredAnchorsCount = 0;
      isRequestFromServerReceived = true;
      currentAnchorAddress = 0;
      isTagBusy = true;
      expectedMessageType = MSG_TYPE_POLL_ACK;
      sendMessage(MSG_TYPE_POLL); // Broadcast to initiate a communication with anchors
      noteActivity();
      return;
    }
  }

  if (isRequestFromServerReceived) //safety check
  {
    // If something has been sent
    if (sentAck) 
    {
      noteActivity();
      sentAck = false;

      // Record the transmission time for DS-TWR 
      if (currentMessage[0] == MSG_TYPE_POLL)
      {
        DW1000.getTransmitTimestamp(timePollSent);
      }

      if (currentMessage[0] == MSG_TYPE_RANGE)
      {
        DW1000.getTransmitTimestamp(timeRangeSent);
      }
    }

    // If something has been received
    if (receivedAck)
    {
      receivedAck = false;
      noteActivity();

      // Unpack the received message
      DW1000.getData(receivedMessage, sizeof(receivedMessage));

      if (expectedMessageType == receivedMessage[0])
      {
        if (isTagBusy)
        {
          if (expectedMessageType == MSG_TYPE_POLL_ACK && checkDestination())
          {
            // If new anchor is discovered
            if (!isAnchorDiscovered() && isAnchorAddress())
            {
              discoveredAnchors[discoveredAnchorsCount++] = receivedMessage[1];
              currentAnchorAddress = receivedMessage[1];
              DW1000.getReceiveTimestamp(timePollAckReceived); // record timestamp of reception for DS-TWR
              expectedMessageType = MSG_TYPE_RANGE_REPORT;
              sendMessage(MSG_TYPE_RANGE);
              noteActivity();
            }
            return;
          }

          // Calculate the distance (using DS-TWR) 
          //  if it is last message in communication with the anchor
          if (expectedMessageType == MSG_TYPE_RANGE_REPORT && checkSourceAndDestination())
          {
            // Get timestamps from message for DS-TWR
            timePollReceived.setTimestamp(receivedMessage + 5);
            timePollAckSent.setTimestamp(receivedMessage + 10);
            timeRangeReceived.setTimestamp(receivedMessage + 15);
            float distance = computeRangeAsymmetric();

            distances[discoveredAnchorsCount - 1] = distance;
            isTagBusy = false;
            // Start discovering new anchors
            //  if the desired number of anchors has not yet been found
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

    // If the desired number of anchors has been found
    //  it is time to send measurements to the server!
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

    // Otherwise, try to discover new anchors
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
