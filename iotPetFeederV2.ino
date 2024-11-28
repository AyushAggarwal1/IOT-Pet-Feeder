// Include all necessary libs
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Servo.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Adafruit IO credentials
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "your-user-name"
#define AIO_KEY "your-AIO-key"

// Wi-Fi credentials
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
const int servoRestPosition = 10;     // Resting position
const int servoTargetPosition = 130; // Target position for feeding

// Time and feeding variables
int feed_hour = 0;   // Default feeding hour
int feed_minute = 0; // Default feeding minute
bool feed = true;    // Feeding flag

void setup() {
    // Serial communication for debugging
    Serial.begin(115200);

    // Start time client
    timeClient.begin();

    // Attach servo and set initial position
    servo.attach(2);
    servo.write(servoRestPosition);

    // Connect to WiFi
    Serial.print("\n\nConnecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected!");

    // Subscribe to the Pet Feeder feed
    mqtt.subscribe(&PetFeeder);
}

void loop() {
    // Ensure MQTT connection
    MQTT_connect();

    // Update time
    timeClient.update();
    int hh = timeClient.getHours();
    int mm = timeClient.getMinutes();

    // Print current time
    Serial.printf("Current Time: %02d:%02d\n", hh, mm);

    // Handle MQTT subscription
    Adafruit_MQTT_Subscribe *subscription;
    while ((subscription = mqtt.readSubscription(5000))) {
        if (subscription == &PetFeeder) {
            String command = (char *)PetFeeder.lastread;
            Serial.println("Received MQTT message: " + command);

            if (command == "ON") {
                open_pet_feeder_gate();
                delay(3000);
                close_pet_feeder_gate();
            } else if (command == "Morning") {
                feed_hour = 10;
                feed_minute = 30;
                Serial.println("Set feeding time to Morning: 10:30");
            } else if (command == "Afternoon") {
                feed_hour = 13; // 1:30 PM in 24-hour format
                feed_minute = 30;
                Serial.println("Set feeding time to Afternoon: 13:30");
            } else if (command == "Evening") {
                feed_hour = 18; // 6:30 PM in 24-hour format
                feed_minute = 30;
                Serial.println("Set feeding time to Evening: 18:30");
            } else {
                Serial.println("Unknown command received.");
            }
        }
    }

    // Feeding logic based on schedule
    if (feed && hh == feed_hour && mm == feed_minute) {
        Serial.println("Feeding time! Opening pet feeder gate...");
        open_pet_feeder_gate();
        delay(3000);
        close_pet_feeder_gate();
        feed = false; // Prevent multiple triggers
    }

    // Reset feeding flag after feeding time has passed
    if (!feed && (hh != feed_hour || mm != feed_minute)) {
        feed = true;
        Serial.println("Feeding flag reset for next schedule.");
    }

    delay(1000); // Delay for stability
}

void open_pet_feeder_gate() {
    Serial.println("Opening pet feeder gate...");
    servo.write(servoTargetPosition);
}

void close_pet_feeder_gate() {
    Serial.println("Closing pet feeder gate...");
    servo.write(servoRestPosition);
}

void MQTT_connect() {
    int8_t ret;

    // Stop if already connected
    if (mqtt.connected()) {
        return;
    }

    Serial.println("Connecting to MQTT...");

    uint8_t retries = 3;
    while ((ret = mqtt.connect()) != 0) { // Connect will return 0 if connected
        Serial.println(mqtt.connectErrorString(ret));
        Serial.println("Retrying MQTT connection in 5 seconds...");
        mqtt.disconnect();
        delay(5000);
        retries--;
        if (retries == 0) {
            Serial.println("Failed to connect to MQTT. Restarting...");
            ESP.restart(); // Restart system to recover
        }
    }
    Serial.println("MQTT Connected!");
}
