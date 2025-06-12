#include "Arduino_BMI270_BMM150.h"

#define WAIT_TIME 50     // Send data every 50 ms (20Hz)
#define FILTER_ALPHA 0.8 // Low-pass filter coefficient

float x, y, z;
float filteredX = 0, filteredY = 0, filteredZ = 0;
float flexionExtension = 0; 
float ulnarRadial = 0;
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println("Hz");

  delay(1000);

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(filteredX, filteredY, filteredZ);
  }
}

void loop() {
  if (IMU.accelerationAvailable() && millis() - previousMillis >= WAIT_TIME) {
    previousMillis = millis();
    IMU.readAcceleration(x, y, z);

    // Low-pass filter
    filteredX = FILTER_ALPHA * filteredX + (1 - FILTER_ALPHA) * x;
    filteredY = FILTER_ALPHA * filteredY + (1 - FILTER_ALPHA) * y;
    filteredZ = FILTER_ALPHA * filteredZ + (1 - FILTER_ALPHA) * z;

    // Flexion/Extension = pitch = rotation around X-axis
    flexionExtension = atan2(filteredY, filteredZ) * 180.0 / PI;

    // Ulnar/Radial Deviation = roll = rotation around Y-axis
    ulnarRadial = atan2(-filteredX, filteredZ) * 180.0 / PI;

    // Constrain angles
    flexionExtension = constrain(flexionExtension, -90, 90);
    ulnarRadial = constrain(ulnarRadial, -45, 45);

    // Output
    Serial.print(flexionExtension, 1);
    Serial.print(",");
    Serial.println(ulnarRadial, 1);
  }
}
