// include all necessary libs

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Servo.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "your-user-name"
#define AIO_KEY "your-AIO-key"

#define WIFI_SSID "your-ssid"
#define WIFI_PASS "your-password"

// Time synchronization
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000);

WiFiClient client;
Servo servo;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe PetFeeder = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/petfeeder");

// Servo setup
const int servoRestPosition = 10;    // Starting position
const int servoTargetPosition = 130; // Position when event is detected*/

// Time and feeding variables
int hh, mm, ss;
int feed_hour = 0;
int feed_minute = 0;
boolean feed = true; // condition for alarm

void setup()
{
    // Serial communication for debugging
    Serial.begin(115200);

    // Start time client
    timeClient.begin();

    // Attach servo and set initial position
    servo.attach(2);
    servo.write(servoRestPosition);

    // Connect to WiFi
    Serial.print("\n\nConnecting Wifi... "); // Connect to WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi Connected");
    // Serial.println("IP address");
    // Serial.println(WiFi.localIP());
    // Subscribe to the Pet Feeder feed
    mqtt.subscribe(&PetFeeder);
}

void loop()
{
    // Ensure MQTT connection
    MQTT_connect();

    // Update time
    timeClient.update();
    hh = timeClient.getHours();
    mm = timeClient.getMinutes();
    ss = timeClient.getSeconds();
    Serial.print("Time:");

    if (hh > 12)
    {
        hh = hh - 12;
        Serial.print(hh);
        Serial.print(":");
        Serial.print(mm);
        Serial.print(":");
        Serial.print(ss);
        Serial.println(" PM ");
    }

    else
    {
        Serial.print(hh);
        Serial.print(":");
        Serial.print(mm);
        Serial.print(":");
        Serial.print(ss);
        Serial.println(" AM ");
    }

    Serial.print("Feed Time: ");
    Serial.print(feed_hour);
    Serial.print(":");
    Serial.print(feed_minute);

    Adafruit_MQTT_Subscribe *subscription;

    while ((subscription = mqtt.readSubscription(5000)))
    {
        if (subscription == &PetFeeder)
        { // Print the new value to the serial monitor
            Serial.println((char *) PetFeeder.lastread);
            if (!strcmp((char *) PetFeeder.lastread, "ON"))
            {
                open_pet_feeder_gate();
                delay(3000);
                close_pet_feeder_gate();
            }
            if (!strcmp((char *) PetFeeder.lastread, "Morning"))
            {
                feed_hour = 10;
                feed_minute = 30;
            }
            if (!strcmp((char *) PetFeeder.lastread, "Afternoon"))
            {
                feed_hour = 1;
                feed_minute = 30;
            }

            if (!strcmp((char *) PetFeeder.lastread, "Evening"))
            {
                feed_hour = 6;
                feed_minute = 30;
            }
        }
    }

    if (hh == feed_hour && mm == feed_minute && feed == true)
    {
        open_pet_feeder_gate();
        delay(3000);
        close_pet_feeder_gate();
        feed = false;
    }
}

void open_pet_feeder_gate()
{
    servo.write(servoTargetPosition);
}

void close_pet_feeder_gate()
{
    servo.write(servoRestPosition);
}

void MQTT_connect()
{
    int8_t ret;

    // stop if already connected
    if (mqtt.connected())
    {
        return;
    }

    Serial.println("Connecting to MQTT....");

    uint8_t retries = 3;
    while ((ret = mqtt.connect()) != 0)
    { // connect will return 0 for connected
        Serial.println(mqtt.connectErrorString(ret));
        Serial.println("Retrying MQTT Connection in 5 sec....");
        mqtt.disconnect();
        delay(5000);
        retries--;
        if (retries == 0)
        {
            // basically die and wait for WDT to reset manually
            while (1)
                ;
        }
    }

    Serial.println("MQTT Connected!!");
}