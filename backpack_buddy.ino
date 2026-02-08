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

void loop() {
  static bool lastBtn = HIGH;
  bool btn = digitalRead(PIN_BTN);

  if (lastBtn == HIGH && btn == LOW) {
    delay(25);
    if (digitalRead(PIN_BTN) == LOW) {
      Serial.println("BTN PRESSED");
      currentState = LISTENING;
      setLEDsListening();

      // sample mic for debug
      for (int i = 0; i < 20; i++) {
        int mic = analogRead(MIC_PIN);
        Serial.print("MIC: ");
        Serial.println(mic);
        delay(50);
      }

      currentState = IDLE;
      //setLEDsIdle();
      Serial.println("DONE");
    }
  }
  lastBtn = btn;
}



