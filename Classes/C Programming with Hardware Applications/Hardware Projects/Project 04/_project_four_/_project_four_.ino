// ECE 2305 – Hardware Project 4: LED Dimmer

// Pin Definitions
const int selectLED1 = 2;    // Button to select LED1
const int selectLED2 = 3;    // Button to select LED2
const int potPin = A0;       // Potentiometer input

const int led1 = 11;         // PWM output to LED1
const int led2 = 10;         // PWM output to LED2

void setup() {
  pinMode(selectLED1, INPUT);
  pinMode(selectLED2, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  Serial.begin(9600); // Optional for debugging
}

void loop() {
  int potValue = analogRead(potPin);          // Read potentiometer
  int pwmValue = map(potValue, 0, 1023, 0, 255); // Map to PWM range

  Serial.print("Potentiometer: ");
  Serial.print(potValue);
  Serial.print(" -> PWM: ");
  Serial.println(pwmValue);

  // Check which button is pressed
  if (digitalRead(selectLED1) == HIGH) {
    analogWrite(led1, pwmValue); // Update LED1 brightness
  } 
  
  if (digitalRead(selectLED2) == HIGH) {
    analogWrite(led2, pwmValue); // Update LED2 brightness
  }

  delay(50); // Small delay for smooth behavior
}
