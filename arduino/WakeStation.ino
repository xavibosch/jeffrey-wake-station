constexpr uint8_t PIR_PIN = 2;
constexpr uint8_t SOUND_PIN = A0;
constexpr uint8_t BUTTON_PIN = 4;

constexpr int SOUND_THRESHOLD = 620;
constexpr unsigned long MOTION_COOLDOWN_MS = 5000;
constexpr unsigned long SOUND_COOLDOWN_MS = 1200;
constexpr unsigned long DEBOUNCE_MS = 45;

unsigned long lastMotion = 0;
unsigned long lastSound = 0;
unsigned long lastButtonChange = 0;
bool buttonState = HIGH;
bool lastButtonReading = HIGH;

void emitEvent(const char* event) {
  Serial.print(event);
  Serial.print(',');
  Serial.println(millis());
}

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(115200);
}

void loop() {
  const unsigned long now = millis();

  if (digitalRead(PIR_PIN) == HIGH && now - lastMotion >= MOTION_COOLDOWN_MS) {
    lastMotion = now;
    emitEvent("MOTION");
  }

  if (analogRead(SOUND_PIN) >= SOUND_THRESHOLD && now - lastSound >= SOUND_COOLDOWN_MS) {
    lastSound = now;
    emitEvent("SOUND");
  }

  const bool reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonReading) lastButtonChange = now;
  if (now - lastButtonChange >= DEBOUNCE_MS && reading != buttonState) {
    buttonState = reading;
    if (buttonState == LOW) emitEvent("BUTTON");
  }
  lastButtonReading = reading;

  delay(5);
}
