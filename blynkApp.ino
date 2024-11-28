#include <BlynkSimpleEsp8266.h>
#include <Servo.h>


char authId[] = "auth-key";
char ssid[] = "wifi-name";
char password = "wifi-password";

Servo servo;

int pinValue;
const int servoRestPosition = 10; 		//starting position
const int servoTargetPosition = 130;		//position when event is triggered	

//BLYNK_WRITE() this auto run when we click button on blynk app
BLYNK_WRITE(V3){
    pinValue = param.asInt(); // setting pinValue as integer for 'on pinValue is 1 and for 'off' pinValue is 0

    if(pinValue == 1){      // on condition
        Serial.println(pinValue);
        open_pet_feeder_gate();
        delay(3000);
        close_pet_feeder_gate();
    }
}

void setup(){
    Serial.begin(115200);
    Blynk.begin(auth, ssid, password);
    servo.attach(2);
    servo.write(0);
    delay(200);
}

void loop(){
    Blynk.run()
}

void open_pet_feeder_gate(){
    servo.write(servoTargetPosition);
}

void close_pet_feeder_gate(){
    servo.write(servoRestPosition);
}