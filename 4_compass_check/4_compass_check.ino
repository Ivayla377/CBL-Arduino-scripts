#include <Arduino_BMI270_BMM150.h>

// --- Calibration values from Jupiter Notebook script ---
float magOffsetX = 6.5;   // Hard iron offset
float magOffsetY = -2.0;
float magOffsetZ = -12.5;

float magScaleX = 1.03;   // Soft iron scale
float magScaleY = 1.0;
float magScaleZ = 0.97;

// Magnetic declination in degrees for your location (example: 2.73° for Europe)
const float declination = 2.73;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.println("Compass Ready. Move board slowly to see heading...");
}

void loop() {
  float mx, my, mz;

  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(mx, my, mz);

    // Apply hard iron offsets
    mx -= magOffsetX;
    my -= magOffsetY;
    mz -= magOffsetZ;

    // Apply soft iron scaling
    mx *= magScaleX;
    my *= magScaleY;
    mz *= magScaleZ;

    // Compute heading in degrees (in horizontal plane, X-Y)
    float heading = atan2(my, mx) * 180.0 / PI;

    // Apply magnetic declination
    heading += declination;

    // Wrap to [0, 360]
    if (heading < 0) heading += 360;
    if (heading > 360) heading -= 360;

    // Print raw + corrected values to help debug
    Serial.print("mx: "); Serial.print(mx, 1);
    Serial.print("  my: "); Serial.print(my, 1);
    Serial.print("  mz: "); Serial.print(mz, 1);
    Serial.print("  Heading: "); Serial.println(heading, 1);
  }

  delay(200);
}
