int relayPin = 4; // Choose the pin for the relay
int pirSensorPin = 3; // Choose the input pin for the PIR sensor
int ledPin = 6; // The LED pin
int pirState = true; // Assume no motion detected initially
int val = 0; // Variable for reading the pin status
int minimummSecsLowForInactive = 1; // If the sensor reports low for more than this time, assume no activity
long unsigned int timeLow;
boolean takeLowTime;

// Calibration time (10-60 seconds according to the datasheet)
int calibrationTime = 30;

void setup() {
    pinMode(ledPin, OUTPUT); // Declare LED pin as output
    pinMode(relayPin, OUTPUT); // Declare relay pin as output
    pinMode(pirSensorPin, INPUT); // Declare PIR sensor pin as input
    Serial.begin(9600);

    // Give the sensor time to calibrate
    Serial.print("Calibrating sensor ");
    for (int i = 0; i < calibrationTime; i++) {
        digitalWrite(ledPin, HIGH);
        Serial.print(".");
        delay(500);
        digitalWrite(ledPin, LOW);
        delay(500);
    }

    digitalWrite(ledPin, LOW);

    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);
}

void loop() {
    
    //Serial.println(millis());

    val = digitalRead(pirSensorPin); // Read input value
    if (val == HIGH) {
        digitalWrite(relayPin, HIGH); // Turn relay ON
        if (pirState) {
            pirState = false;
            Serial.println("Motion detected!");
        }
        takeLowTime = true;
    } else {
        digitalWrite(relayPin, LOW); // Turn relay OFF

        if (takeLowTime) {
            timeLow = millis();
            takeLowTime = false;
        }
        if (!pirState && millis() - timeLow > minimummSecsLowForInactive) {
            pirState = true;
            Serial.println("Motion stopped!");

            digitalWrite(ledPin, HIGH);
            delay(50);
            digitalWrite(ledPin, LOW);
        }
    }
}

