// Digital VU Meter - ECE 2305 Hardware Project 03

const int td = 100;
int analogVoltage = 0;

void setup() {
  for (int pin = 4; pin <= 13; pin++) {
    pinMode(pin, OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  analogVoltage = analogRead(A0);
  Serial.println(analogVoltage);

  int ledCount = map(analogVoltage, 0, 1023, 0, 10); // Map analog input to 0-10 LEDs

  for (int i = 0; i < 10; i++) {
    if (i < ledCount) {
      digitalWrite(4 + i, HIGH); // Turn on LEDs up to ledCount
    } else {
      digitalWrite(4 + i, LOW);  // Turn off the rest
    }
  }

  delay(td);
}
