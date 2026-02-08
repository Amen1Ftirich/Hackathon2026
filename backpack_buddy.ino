// =====================
// Pin definitions
// =====================
#define MIC_PIN A0
#define BTN_PIN 2

#define LED1 5   // LISTENING / IDLE
#define LED2 6   // THINKING
#define LED3 9   // SPEAKING

// =====================
// Audio settings
// =====================
const int SAMPLE_RATE = 8000;
const float SECONDS = 1.0;
const int N_SAMPLES = SAMPLE_RATE * SECONDS;

// =====================
// System state enum
// =====================
enum SystemState {
  STATE_IDLE,
  STATE_LISTENING,
  STATE_THINKING,
  STATE_SPEAKING
};

SystemState currentState = STATE_IDLE;

// =====================
// SETUP
// =====================
void setup() {
  Serial.begin(115200);

  pinMode(BTN_PIN, INPUT_PULLUP);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  updateLEDs();   // start in IDLE
}

// =====================
// MAIN LOOP
// =====================
void loop() {
  // ALWAYS listen for laptop commands
  readSerialCommands();

  // Handle button press for recording
  static bool lastBtn = HIGH;
  bool btn = digitalRead(BTN_PIN);

  if (lastBtn == HIGH && btn == LOW) {
    delay(25); // debounce
    if (digitalRead(BTN_PIN) == LOW) {
      recordAudio();
    }
  }

  lastBtn = btn;
}

// =====================
// SERIAL COMMAND HANDLER
// =====================
void readSerialCommands() {
  if (!Serial.available()) return;

  String line = Serial.readStringUntil('\n');
  line.trim();

  if (line == "STATE:IDLE") {
    currentState = STATE_IDLE;
  } 
  else if (line == "STATE:LISTENING") {
    currentState = STATE_LISTENING;
  } 
  else if (line == "STATE:THINKING") {
    currentState = STATE_THINKING;
  } 
  else if (line == "STATE:SPEAKING") {
    currentState = STATE_SPEAKING;
  }

  updateLEDs();
}

// =====================
// LED CONTROL
// =====================
void updateLEDs() {
  switch (currentState) {
    case STATE_IDLE:
      ledsIdle();
      break;
    case STATE_LISTENING:
      ledsListening();
      break;
    case STATE_THINKING:
      ledsThinking();
      break;
    case STATE_SPEAKING:
      ledsSpeaking();
      break;
  }
}

void ledsIdle() {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
}

void ledsListening() {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, LOW);
}

void ledsThinking() {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
}

void ledsSpeaking() {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, HIGH);
}

// =====================
// AUDIO RECORDING
// =====================
void recordAudio() {
  Serial.println("START_AUDIO");


  unsigned long usPerSample = 1000000UL / SAMPLE_RATE;

  for (int i = 0; i < N_SAMPLES; i++) {
    int x = analogRead(MIC_PIN);          // 0–4095
    uint8_t sample = (uint8_t)(x >> 4);  // convert to 8-bit
    Serial.write(sample);
    delayMicroseconds(usPerSample);
  }

  Serial.println("\nEND_AUDIO");
}

/*
#define MIC_PIN A0
#define BTN_PIN 2

#define LED1 5
#define LED2 6
#define LED3 9

enum SystemState {
  STATE_IDLE,
  STATE_LISTENING,
  STATE_THINKING,
  STATE_SPEAKING
};

SystemState currentState = STATE_IDLE;

void setup() {
  Serial.begin(115200);

  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  //setIdle();
}

void setIdle() {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
}

void setListening() {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, LOW);
}

void setThinking() {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
}

void setSpeaking() {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, HIGH);
}

void readSerialState() {
  if (!Serial.available()) return;

  String cmd = Serial.readStringUntil('\n');
  cmd.trim();

  if (cmd == "IDLE") setIdle();
  else if (cmd == "LISTENING") setListening();
  else if (cmd == "THINKING") setThinking();
  else if (cmd == "SPEAKING") setSpeaking();
}

void loop() {
  readSerialState();
}*/