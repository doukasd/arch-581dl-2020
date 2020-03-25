const int LED_R_PIN = 9;
const int LED_G_PIN = 10;
const int LED_B_PIN = 11;
const int BUTTON_PIN = 8;
const int NUM_STATES = 5;

int state = 0;
bool buttonState = LOW;
bool lastButtonState = LOW;
float speed = 0.9;
float currentValue = 0;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("value,red,green,blue");
  
  pinMode(LED_R_PIN, OUTPUT);
  pinMode(LED_G_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
  
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  // read the current state of the button
  buttonState = digitalRead(BUTTON_PIN);

  // if the button is pressed and it was not pressed before, toggle our state
  if(buttonState != lastButtonState && buttonState == HIGH) {
    state = (state + 1) % NUM_STATES;
  }

  // for fun
  if(true) {
    if(state - currentValue < 0.1) {
      state = (state + 1) % NUM_STATES;
    }
  }

  // update last state
  lastButtonState = buttonState;

  // always updat the LEDs
  currentValue = state < currentValue ? currentValue = state : state + (currentValue - state) * speed;  // this will change between [0, 3]
  int red = ledValue(currentValue, 1.0); //inRange(currentValue, 0, 1) ? int((currentValue-0.0) * 255.0) : 0;
  int green = ledValue(currentValue, 2.0); //inRange(currentValue, 1, 2) ? int((currentValue-1.0) * 255.0) : 0;
  int blue = ledValue(currentValue, 3.0); //inRange(currentValue, 2, 3) ? int((currentValue-2.0) * 255.0) : 0;

  // log
  Serial.print(currentValue * 255);
  Serial.print(",");
  Serial.print(red);
  Serial.print(",");
  Serial.print(green);
  Serial.print(",");
  Serial.println(blue);

  // write
  analogWrite(LED_R_PIN, red);
  analogWrite(LED_G_PIN, green);
  analogWrite(LED_B_PIN, blue);

  // add a tiny delay to avoid bouncing
  delay(5);
}

int ledValue(float stateValue, float targetValue) {
  return int(clamp(map(dist(stateValue, targetValue) * 100, 0, 100, 255, 0), 0, 255));
}
float dist(float value, float target) {
  return abs(target - value);
}

float clamp(float value, float min, float max) {
  return max(min(value, max), min);
}

bool inRange(float value, float min, float max) {
  return value > min && value <= max;
}
