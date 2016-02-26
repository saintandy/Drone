
#include <Servo.h>
#include <Wire.h>

const int MPU_addr = 0x68;

// front - white
// back - red

Servo front_left, front_right, back_left, back_right;
int startup_value = 50;
int ch1, ch2, ch3, ch4, ch5, ch6;
int16_t Gx, Gy, Gz;

void engine(int v1, int v2, int v3, int v4) {
  front_left.write(v1);
  front_right.write(v2);
  back_right.write(v3);
  back_left.write(v4);
}

void setup() {
  pinMode(22, INPUT);
  pinMode(24, INPUT);
  pinMode(26, INPUT);
  pinMode(28, INPUT);
  pinMode(30, INPUT);
  pinMode(32, INPUT);
  
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
  
  front_left.attach(2);
  front_right.attach(3);
  back_right.attach(4);
  back_left.attach(5);
  
  for (int i = 1; i <= startup_value; ++i) {
    engine(i, i, i, i);
  }
  
  delay(150);
}

void loop() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x38);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true);
  
  Gx = Wire.read() << 8 | Wire.read();
  Gy = Wire.read() << 8 | Wire.read();
  Gz = Wire.read() << 8 | Wire.read();
  
  ch1 = pulseIn(22, HIGH);
  ch2 = pulseIn(24, HIGH);
  ch3 = pulseIn(26, HIGH);
  ch4 = pulseIn(28, HIGH);
  ch5 = pulseIn(30, HIGH);
  ch6 = pulseIn(32, HIGH);
  
  int power = (ch2 - 1000) / 10 + 50;
  power = max(power, 50);
  power = min(power, 120);
  
  engine(power, power, power, power);
}

