
# Automated Pet Feeding System

An IoT-based pet feeding system that automates feeding schedules and allows real-time control and monitoring through Google Assistant and the Blynk app. The system also integrates with Adafruit IO and IFTTT for automation and cloud connectivity.

## Features

- **Voice Control**: Use Google Assistant to feed your pet.
- **Real-Time Monitoring**: Monitor feeding status and manually control food dispensing with the Blynk app.
- **Scheduled Feeding**: Set timers for automatic feeding based on your pet's needs.
- **Cloud Integration**: Adafruit IO for cloud-based control and IFTTT for automation.

## Tech Stack

- **Hardware**: ESP8266/ESP32, Servo Motor, Sensors (optional)
- **Cloud**: Adafruit IO, IFTTT
- **Mobile App**: Blynk
- **Software**: Arduino IDE

## Requirements

- Arduino IDE installed
- Blynk Library for ESP8266/ESP32
- Adafruit MQTT Library

## Hardware Setup

| ESP8266 Pin | Component         | Description                   |
|-------------|-------------------|-------------------------------|
| D1          | Servo Signal Pin   | Controls the servo motor      |
| VCC         | Servo Motor VCC    | Power supply for the servo    |
| GND         | Servo Motor GND    | Ground                        |

## How to Use

1. **Clone the repository**:
   ```bash
   git clone https://github.com/AyushAggarwal1/IOT-Pet-Feeder.git
   cd IOT-Pet-Feeder
2. **Install the required libraries:**

    Blynk Library for ESP8266/ESP32
    Adafruit MQTT Library
    Upload the code to your ESP8266/ESP32 microcontroller through the Arduino IDE.

3. **Configure Blynk and Adafruit IO:**
Add your Wi-Fi credentials, Blynk Auth Token, and Adafruit IO username/key in the code.

4. **Use Google Assistant:**
Create an IFTTT applet to trigger feeding via Google Assistant voice commands.

5. **Monitor in Real-Time:**
Open the Blynk app to manually control and monitor feeding.

## Demostrated Videos

https://user-images.githubusercontent.com/98796628/169439834-7e5528d1-3431-403d-93d4-ffb8f357d8e4.mp4



https://user-images.githubusercontent.com/98796628/169439945-1a42a4da-f526-4863-a208-d0ee36f33493.mp4



https://user-images.githubusercontent.com/98796628/169439989-c2513fc2-5939-4d3e-922a-ac65de34a8fc.mp4



https://user-images.githubusercontent.com/98796628/169440051-7c11777f-ea8e-4402-9de6-52ad6e1386e5.mp4

## Future Improvements

- Add weight sensors to measure the food level.
- Notify users when the food supply is low.
- Implement machine learning for predictive feeding schedules.