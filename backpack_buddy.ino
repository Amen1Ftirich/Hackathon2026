  // -------- Pins --------
const int PIN_BTN = 2;
const int LED1 = 5;
const int LED2 = 6;
const int LED3 = 9;
const int MIC_PIN = A0;

// -------- States --------
enum State { IDLE, LISTENING };
State currentState = IDLE;

void setLEDsIdle() {
  analogWrite(LED1, 20);
  analogWrite(LED2, 20);
  analogWrite(LED3, 10);
}

void setLEDsListening() {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, LOW);
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_BTN, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  setLEDsIdle();
  Serial.println("READY");
}

const int SAMPLE_RATE = 8000;
const float SECONDS = 1.0;
const int N_SAMPLES = SAMPLE_RATE * SECONDS;

void loop() {
  static bool lastBtn = HIGH;
  bool btn = digitalRead(PIN_BTN);

  if (lastBtn == HIGH && btn == LOW) {
    delay(25);
    if (digitalRead(PIN_BTN) == LOW) {
      Serial.println("START_AUDIO");
      setLEDsListening();

      unsigned long usPerSample = 1000000UL / SAMPLE_RATE;

      for (int i = 0; i < N_SAMPLES; i++) {
        int x = analogRead(MIC_PIN);
        uint8_t b = (uint8_t)(x >> 4); // 8-bit
        Serial.write(b);
        delayMicroseconds(usPerSample);
      }

      Serial.println("\nEND_AUDIO");
      setLEDsIdle();
    }
  }
  lastBtn = btn;
}




