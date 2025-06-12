// SKETCH FOR CALIBRATION WITH THE JUPITER NOTEBOOK  //


#include <Arduino_BMI270_BMM150.h>

int loopcount = 0;
float ax, ay, az;
float gx, gy, gz;
float mx, my, mz;

void setup() {
  Serial.begin(115200);
  while (!Serial);  // Wait for serial monitor

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.println("IMU initialized.");
  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");

  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");

  Serial.print("Magnetometer sample rate = ");
  Serial.print(IMU.magneticFieldSampleRate());
  Serial.println(" Hz");
}

void loop() {

  // Read sensor values
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(ax, ay, az);
  }

  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gx, gy, gz);
  }

  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(mx, my, mz);
  }

  // Raw format for MotionCal
  Serial.print("Raw:");
  Serial.print(int(ax * 8192 / 9.8)); Serial.print(",");
  Serial.print(int(ay * 8192 / 9.8)); Serial.print(",");
  Serial.print(int(az * 8192 / 9.8)); Serial.print(",");
  Serial.print(int(gx * 16)); Serial.print(",");
  Serial.print(int(gy * 16)); Serial.print(",");
  Serial.print(int(gz * 16)); Serial.print(",");
  Serial.print(int(mx * 10)); Serial.print(",");
  Serial.print(int(my * 10)); Serial.print(",");
  Serial.print(int(mz * 10)); Serial.println();

  // Unified format
  Serial.print("Uni:");
  Serial.print(ax, 3); Serial.print(",");
  Serial.print(ay, 3); Serial.print(",");
  Serial.print(az, 3); Serial.print(",");
  Serial.print(gx, 4); Serial.print(",");
  Serial.print(gy, 4); Serial.print(",");
  Serial.print(gz, 4); Serial.print(",");
  Serial.print(mx, 3); Serial.print(",");
  Serial.print(my, 3); Serial.print(",");
  Serial.print(mz, 3); Serial.println();

  delay(10);
}
