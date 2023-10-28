#include <iostream>

char msgToSend[30];
uint16_t myID;
size_t discoveredAnchorsCount = 3;
uint8_t discoveredAnchors[3] = {0, 1, 2};
float distances[3] = {0.12, 1.23, 2.23};

int main()
{
    int position = sprintf(msgToSend, "%d %d %f", myID, discoveredAnchors[0], distances[0]);
    // msgToSend = String(myID) + String(discoveredAnchors[0]) + distances[0];
    if (discoveredAnchorsCount > 1)
    {
        for (int i = 1; i < discoveredAnchorsCount; i++)
        {
            // msgToSend += "; Anchor ID: " + String(discoveredAnchors[i]) + " Distance: " + distances[i];
            position += sprintf(msgToSend + position, "%d %d %f", myID, discoveredAnchors[i], distances[i]);
        }
    }

    sprintf(msgToSend + position, "\n");

    std::cout << msgToSend;
}