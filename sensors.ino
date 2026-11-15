// SmartFit sensor node (Tinkercad, Arduino Uno)
// HC-SR04: TRIG->D9, ECHO->D10 | TMP36 -> A0 | Pot (weight) -> A1 | Pot (heart rate) -> A2
// Buzzer -> D8, LED -> D7 | LCD optional
const int TRIG = 9, ECHO = 10, BUZ = 8, LED = 7;
const float MOUNT_CM = 220.0;  // sensor fixed 220 cm above the floor

void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT); pinMode(ECHO, INPUT);
  pinMode(BUZ, OUTPUT);  pinMode(LED, OUTPUT);
}

void loop() {
  digitalWrite(TRIG, LOW); delayMicroseconds(2);
  digitalWrite(TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  float dist = pulseIn(ECHO, HIGH) * 0.0343 / 2.0;
  float height = MOUNT_CM - dist;                       // person's height in cm

  float volts = analogRead(A0) * 5.0 / 1023.0;
  float tempC = (volts - 0.5) * 100.0;                  // TMP36
  float weight = map(analogRead(A1), 0, 1023, 30, 150); // simulated load cell (kg)
  int hr = map(analogRead(A2), 0, 1023, 40, 160);       // simulated pulse (bpm)

  // EDGE decision: runs on the microcontroller, no internet needed
  bool alert = (tempC >= 37.8) || (hr > 100) || (hr < 50);
  digitalWrite(LED, alert); digitalWrite(BUZ, alert);

  // CSV line: paste into the web app -> height,weight,temp,heartRate
  Serial.print(height, 0); Serial.print(",");
  Serial.print(weight, 0); Serial.print(",");
  Serial.print(tempC, 1);  Serial.print(",");
  Serial.println(hr);
  delay(1000);
}
