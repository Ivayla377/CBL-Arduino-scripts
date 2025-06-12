#include <Arduino.h>
#include <Arduino_BMI270_BMM150.h>
#include <MadgwickAHRS.h>

Madgwick filter;

// Calibration values
float magOffsetX = 6.5, magOffsetY = -2.0, magOffsetZ = -12.0;

unsigned long lastUpdate = 0;
const float sampleRateHz = 100.0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.println("IMU initialized");
  filter.begin(sampleRateHz);
}

void loop() {
  float ax, ay, az;
  float gx, gy, gz;
  float mx, my, mz;

  if (IMU.accelerationAvailable() &&
      IMU.gyroscopeAvailable() &&
      IMU.magneticFieldAvailable()) {

    IMU.readAcceleration(ax, ay, az);
    IMU.readGyroscope(gx, gy, gz);
    IMU.readMagneticField(mx, my, mz);

    // Apply magnetometer calibration offset
    mx -= magOffsetX;
    my -= magOffsetY;
    mz -= magOffsetZ;

    // filter.update(-gx, -gy, -gz, ax, ay, az, my, -mx, -mz);
    // filter.update(gx, gy, -gz, ax, ay, -az, -mx, -my, mz);
    filter.update(gx, gy, gz, ax, ay, -az, my, mx, mz);

    float roll  = filter.getRoll();  // in degrees
    float pitch = filter.getPitch(); // in degrees
    float yaw   = filter.getYaw();   // in degrees

    Serial.print(pitch, 2); Serial.print(", ");
    Serial.print(roll, 2); Serial.print(", ");
    Serial.println(yaw, 2);
  }

  delay(10);
}
