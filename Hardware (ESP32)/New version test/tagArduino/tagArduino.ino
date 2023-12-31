#include "arduino.h"

uint16_t getMyID()
{
  String macAddr = WiFi.macAddress();
  if (macAddr == "70:B8:F6:D8:F6:48")
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
    if (debug) {
      Serial.println("Wrong tag MAC address.");
      }
      
    return 0;
  }
}

void checkForReset()
{
  currentTime = millis();
  if (!sentAck && !receivedAck)
  {
    //    if (debug) {
    //      Serial.print("currentTime: ");
    //      Serial.println(currentTime);
    //      Serial.print("lastActivity: ");
    //      Serial.println(lastActivity);
    //    }
    if (currentTime - lastActivity > DEFAULT_RESET_TIMEOUT && isRequestFromServerReceived)
    {
      for (size_t i = 0; i < MAX_ANCHORS - 1; i++)
        discoveredAnchors[i] = 0;
      discoveredAnchorsCount = 0;
      initReceiver();
      if (debug)
        Serial.println("Reinit....");
      // blinkTimer = millis();
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
    if (debug)
      Serial.print('.');
  }
  if (debug)
    Serial.println("Connected to WiFi");
}

void connectToServer()
{
  if (debug)
    Serial.print("Connecting to server");
  while (!client.connect(host, 30001))
  {
    delay(500);
    if (debug)
      Serial.print(".");
  }
  if (debug)
  {
    Serial.println();
    Serial.println("Connected to server");
  }
}

void sendDistancesToServer()
{
  if (debug)
    Serial.println("Sending distances to server");

  //  int position = sprintf(msgToSend, "%d %d %f %s", myID, discoveredAnchors[0], distances[0], roundTimestamps[0]);
  int position = sprintf(msgToSend, "%d %d %f", myID, discoveredAnchors[0], distances[0]);

  // msgToSend = String(myID) + String(discoveredAnchors[0]) + distances[0];
  if (discoveredAnchorsCount > 1)
  {
    for (int i = 1; i < discoveredAnchorsCount; i++)
    {
      // msgToSend += "; Anchor ID: " + String(discoveredAnchors[i]) + " Distance: " + distances[i];
      //      position += sprintf(msgToSend + position, " %d %d %f %s", myID, discoveredAnchors[i], distances[i], roundTimestamps[i]);
      position += sprintf(msgToSend + position, " %d %d %f", myID, discoveredAnchors[i], distances[i]);
    }
  }

  sprintf(msgToSend + position, "\n");

  client.print(msgToSend);
  if (debug)
  {
    Serial.print("Distances sent: ");
    Serial.println(msgToSend);
  }
}

bool isAnchorAddress()
{
  if (debug)
  {
    Serial.print("Anchor Address: ");
    Serial.println();
  }

  if (100 < receivedMessage[1] && receivedMessage[1] < 199)
  {
    return true;
  }
  else
  {
    if (debug)
      Serial.println("  Wrong Anchor Address!! Must be in range (100, 199)");
  }
}

bool checkSource()
{
  if (isAnchorAddress() && currentAnchorAddress == receivedMessage[1])
    return true;
  if (debug)
  {
    Serial.println("Wrong source!");
    Serial.print("Current anchor is: ");
    Serial.println(currentAnchorAddress);
  }

  return false;
}

bool checkDestination()
{
  if (myID == receivedMessage[2])
  {
    if (debug)
      Serial.println("Destination is correct. It is for me!");
    return true;
  }
  if (debug)
    Serial.println("Wrong destination. Not for me!");
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
  if (debug)
    Serial.println("---Send Message---");
  DW1000.newTransmit();
  DW1000.setDefaults();

  if (messageType == MSG_TYPE_POLL)
  {

    if (debug)
    {
      Serial.print("  MSG_TYPE_POLL: ");
      Serial.println(MSG_TYPE_POLL);
    }

    prepareMessageToSend(MSG_TYPE_POLL, myID);
  }
  else if (messageType == MSG_TYPE_RANGE)
  {
    if (debug)
    {
      Serial.print("  MSG_TYPE_RANGE: ");
      Serial.println(MSG_TYPE_RANGE);
    }

    prepareMessageToSend(MSG_TYPE_RANGE, myID, currentAnchorAddress);
    setReplyDelay();
    if (debug)
    {
      Serial.print("Reply Delay: ");
      Serial.println(replyDelay);
    }
    rangeReplyDelay = DW1000Time(replyDelay, DW1000Time::MICROSECONDS);
    DW1000.setDelay(rangeReplyDelay);
  }

  DW1000.setData(currentMessage, sizeof(currentMessage));

  if (debug)
  {
    Serial.print(" Current Message content: ");
    for (size_t i = 0; i < sizeof(currentMessage); i++)
    {
      Serial.print(currentMessage[i]);
      Serial.print(" ");
    }
    Serial.println(" ");
  }

  DW1000.startTransmit();
  blinkTimer = millis();
  discoveryTimer = millis();
  // while (!DW1000.isTransmitDone()) continue;

  // DW1000.clearTransmitStatus();
  //  lastActivity = millis();
}

bool isAnchorDiscovered()
{
  for (size_t i = 0; i < MAX_ANCHORS - 1; i++)
  {
    if (discoveredAnchors[i] == receivedMessage[1])
    {
      if (debug)
      {
        Serial.println("Anchor is already discovered!");
        Serial.print("i = ");
        Serial.println(i);
        Serial.print("anchor address: ");
        Serial.println(receivedMessage[1]);
        Serial.print("discoveredAnchorsCount: ");
        Serial.println(discoveredAnchorsCount);
      }
      return true;
    }
  }
  if (debug)
    Serial.println("Anchor is not yet discovered!");
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
  // 16536 - 5 m/
  // 16384 - standard
  // Tag 2: 16424
  DW1000.commitConfiguration();

  DW1000.attachSentHandler(handleSent);
  DW1000.attachReceivedHandler(handleReceived);

  initReceiver();

  // blinkTimer = millis();

  if (debug)
    Serial.println("TAG");
  delay(1000);
}

void setup()
{
  Serial.begin(115200);
  initTag();

  connectToWiFi();
  connectToServer();
}

void loop()
{

  checkForReset();

  //  if (debug)
  //  {
  //    Serial.print("Is request from server received: ");
  //    Serial.println(isRequestFromServerReceived);
  //  }

  if (!client.connected())
  {
    connectToServer();
  }

  if (client.available() && !isRequestFromServerReceived)
  {
    serverRequest = client.readStringUntil('\n');
    if (serverRequest == "1")
    {
      if (debug)
        Serial.println("Received start command from server");
      for (size_t i = 0; i < MAX_ANCHORS - 1; i++)
        discoveredAnchors[i] = 0;
      discoveredAnchorsCount = 0;
      isRequestFromServerReceived = true;
      // blinkTimer = millis();
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
    // if (debug) Serial.println("----------isRequestFromServerReceived----------");
    if (sentAck)
    {
      noteActivity();
      sentAck = false;
      if (debug)
        Serial.println("Sent ACK");

      if (currentMessage[0] == MSG_TYPE_POLL)
      {
        if (debug)
          Serial.println("  MSG_TYPE_POLL is sent. Record timestamp");
        DW1000.getTransmitTimestamp(timePollSent);
      }

      if (currentMessage[0] == MSG_TYPE_RANGE)
      {
        if (debug)
          Serial.println("  MSG_TYPE_RANGE is sent. Record timestamp");
        DW1000.getTransmitTimestamp(timeRangeSent);
      }
    }

    if (receivedAck)
    {
      if (debug)
        Serial.println("Received something");
      receivedAck = false;
      noteActivity();

      DW1000.getData(receivedMessage, sizeof(receivedMessage));

      if (debug)
      {
        Serial.print("  Message content: ");
        for (size_t i = 0; i < sizeof(receivedMessage); i++)
        {
          Serial.print(receivedMessage[i]);
          Serial.print(" ");
        }
        Serial.println(" ");
      }

      if (expectedMessageType == receivedMessage[0])
      {
        if (isTagBusy)
        {
          if (expectedMessageType == MSG_TYPE_POLL_ACK && checkDestination())
          {
            if (!isAnchorDiscovered() && isAnchorAddress())
            {
              if (debug)
                Serial.println("received POLL ACK!");
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

            // Remember all round timestamps for debugging
            //            /sprintf(roundTimestamps[discoveredAnchorsCount - 1], "round1: %f reply1: %f round2: %f reply2: %f tof: %f ", round1.getAsMicroSeconds(), round2.getAsMicroSeconds(), round2.getAsMicroSeconds(), reply2.getAsMicroSeconds(), tof.getAsMicroSeconds());

            if (debug)
            {

              Serial.print("Distance from anchor ");
              Serial.print(currentAnchorAddress);
              Serial.print(": ");
              Serial.println(distance);
            }

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
        else
        {
          if (debug)
            Serial.println("Tag lost connection with Anchor!!");
        }
        // }
      }
      else
      {

        if (debug)
        {
          Serial.print("Received wrong message: ");
          Serial.println(receivedMessage[0]);
          Serial.print("Expected: ");
          Serial.print(expectedMessageType);
        }
      }
    }

    discoveryTimeout = millis();
    if (!isTagBusy && (discoveredAnchorsCount >= MIN_ANCHORS || discoveryTimeout - discoveryTimer > BLINK_DELAY))
    {
      if (debug)
        Serial.println("All anchors are discovered! Sending to server!");

      sendDistancesToServer();

      while (client.connected() && !client.available())
        ;

      ack = client.readStringUntil('\n');
      if (ack == "7")
      {
        if (debug)
          Serial.println("Server acknowledged distance");
      }
      else
      {
        if (debug)
        {
          Serial.println("Server did not acknowledge distance");
          Serial.print("Acknowledgement: ");
          Serial.println(ack);
        }
      }
      isRequestFromServerReceived = false;
      for (size_t i = 0; i < MAX_ANCHORS - 1; i++)
        discoveredAnchors[i] = 0;
      discoveredAnchorsCount = 0;
      discoveryTimer = discoveryTimeout;
      noteActivity();
      return;
    }

    blinkCurrentMillis = millis();
    if (!isTagBusy && discoveredAnchorsCount < MIN_ANCHORS && blinkCurrentMillis - blinkTimer > BLINK_DELAY)
    {
      if (debug)
      {
        Serial.print("Number of already discovered anchors: ");
        Serial.println(discoveredAnchorsCount);
      }
      blinkTimer = blinkCurrentMillis;
      currentAnchorAddress = 0;
      isTagBusy = true;
      expectedMessageType = MSG_TYPE_POLL_ACK;
      sendMessage(MSG_TYPE_POLL);
      noteActivity();
    }
  }
  else
  {
    if (debug)
      Serial.println("---------------No request from server--------------");
  }
}
