#include <WiFi.h>

void setup()
{
    Serial.begin(115200);
    delay(1000);
    // Print the MAC address of your device
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());
}

void loop()
{
    // Your code here
}
