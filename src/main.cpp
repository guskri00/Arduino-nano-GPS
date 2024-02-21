#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// The pins for software serial communication
int rxPin = 10;
int txPin = 11;

// Set up a new SoftwareSerial port
SoftwareSerial gpsSerial(rxPin, txPin);

void setup() {
  // Start the hardware serial port
  Serial.begin(9600);
  // Start the software serial port at the baud rate of the GPS
  gpsSerial.begin(9600);

  Serial.println("GPS Receiver Test");
}

void loop() {
  static bool isConnected = false;
  static unsigned long lastValidDataTime = 0;
  
  if (gpsSerial.available()) {
    String line = gpsSerial.readStringUntil('\n');
    // Check if the line is a valid GPRMC sentence
    if (line.startsWith("$GPRMC")) {
      int validIndex = line.indexOf('A', 0); // 'A' in GPRMC sentence means data is valid
      if (validIndex > 0) {
        // We have a valid connection
        if (!isConnected) {
          isConnected = true;
          Serial.println("Connected to GPS!");
        }
        lastValidDataTime = millis(); // Update the last valid data received time
      }
    }
  }
  
  // Check for connection timeout (5 seconds without valid data)
  if (isConnected && millis() - lastValidDataTime > 5000) {
    isConnected = false;
    Serial.println("Disconnected or waiting for GPS signal...");
  }
}
