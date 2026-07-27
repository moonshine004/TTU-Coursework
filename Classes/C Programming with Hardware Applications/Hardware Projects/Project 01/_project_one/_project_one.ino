// Traffic Light Simulation

const int RED_NS = 10;   // North/South Red
const int YELLOW_NS = 9; // North/South Yellow
const int GREEN_NS = 8;  // North/South Green
const int RED_EW = 7;    // East/West Red
const int YELLOW_EW = 6; // East/West Yellow
const int GREEN_EW = 5;  // East/West Green

const unsigned long GREEN_TIME = 3000;  // ms
const unsigned long YELLOW_TIME = 1500; // ms
const unsigned long RED_TIME = 1200;    // ms

void initializeLights() {
  // Set all traffic lights as OUTPUT
  for (int pin = 5; pin <= 10; pin++) {
    pinMode(pin, OUTPUT);
  }
}

void setLights(bool ns_red, bool ns_yellow, bool ns_green, bool ew_red, bool ew_yellow, bool ew_green) {
  digitalWrite(RED_NS, ns_red);
  digitalWrite(YELLOW_NS, ns_yellow);
  digitalWrite(GREEN_NS, ns_green);
  digitalWrite(RED_EW, ew_red);
  digitalWrite(YELLOW_EW, ew_yellow);
  digitalWrite(GREEN_EW, ew_green);
}

void setup() {
  initializeLights();
}

void loop() {
  // North/South Red, East/West Green
  setLights(true, false, false, false, false, true);
  delay(GREEN_TIME);

  // North/South Red, East/West Yellow
  setLights(true, false, false, false, true, false);
  delay(YELLOW_TIME);

  // Both Red before switching
  setLights(true, false, false, true, false, false);
  delay(RED_TIME);

  // East/West Red, North/South Green
  setLights(false, false, true, true, false, false);
  delay(GREEN_TIME);

  // East/West Red, North/South Yellow
  setLights(false, true, false, true, false, false);
  delay(YELLOW_TIME);

  // Both Red before switching back
  setLights(true, false, false, true, false, false);
  delay(RED_TIME);
}
