// Ultrasonic Sensor Pins
#define TRIG_PIN 9
#define ECHO_PIN 10

// Motor Control Pins (One L298N per motor)
#define M1_ENA 3   // Front Left PWM
#define M1_IN1 4
#define M1_IN2 5

#define M2_ENB 6   // Front Right PWM
#define M2_IN3 7
#define M2_IN4 8

#define M3_ENC 11  // Rear Left PWM
#define M3_IN5 12
#define M3_IN6 13

#define M4_END A0  // Rear Right PWM
#define M4_IN7 A1
#define M4_IN8 A2

// Buzzer Pin (NEW)
#define DANGER_BUZZER_PIN A5

// Speed Parameters
#define FULL_SPEED 150
#define MEDIUM_SPEED 70
#define SLOW_SPEED 50
#define TURN_SPEED 120  // Speed for turning
#define STOP 0

// Distance Thresholds (cm)
#define FAR_DIST 200
#define MID_DIST 140
#define WARNING_DIST 70
#define DANGER_DIST 30

// Buzzer Parameters (NEW)
#define BUZZER_FREQ_WARNING 1000  // 1kHz for warning
#define BUZZER_FREQ_DANGER 2000   // 1.5kHz for danger
#define BUZZER_ON_TIME 100        // 100ms beep
#define BUZZER_OFF_TIME 50        // 50ms pause

// Turn duration (ms)
#define TURN_DURATION 500  // How long to turn right

void setup() {
  Serial.begin(9600);
  
  // Initialize motor pins
  pinMode(M1_ENA, OUTPUT); pinMode(M1_IN1, OUTPUT); pinMode(M1_IN2, OUTPUT);
  pinMode(M2_ENB, OUTPUT); pinMode(M2_IN3, OUTPUT); pinMode(M2_IN4, OUTPUT);
  pinMode(M3_ENC, OUTPUT); pinMode(M3_IN5, OUTPUT); pinMode(M3_IN6, OUTPUT);
  pinMode(M4_END, OUTPUT); pinMode(M4_IN7, OUTPUT); pinMode(M4_IN8, OUTPUT);

  // Ultrasonic sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Danger buzzer (NEW)
  pinMode(DANGER_BUZZER_PIN, OUTPUT);

  allStop();
  Serial.println("4WD System - Right Turn on Stop");
}

void loop() {
  float distance = getDistance();
  Serial.print("Distance: "); Serial.print(distance); Serial.println(" cm");

  // Danger buzzer control (NEW)
  if (distance <= DANGER_DIST) {
    emergencyStop();
    turnRight();
    activateDangerBuzzer(true); // Continuous fast beeping
    Serial.println("! STOPPED & TURNING RIGHT !");
  } 
  else if (distance <= WARNING_DIST) {
    setFrontMotors(SLOW_SPEED);
    setRearMotors(MEDIUM_SPEED);
    activateWarningBuzzer(); // Intermittent beeping
    Serial.println("WARNING: Slow Speed");
  }
  else {
    setAllMotors(distance <= MID_DIST ? MEDIUM_SPEED : 
                (distance <= FAR_DIST ? FULL_SPEED * 0.8 : FULL_SPEED));
    noTone(DANGER_BUZZER_PIN); // Turn off buzzer
  }

  // Set all motors forward
  setAllDirections(HIGH, LOW);
  delay(100);
}

// NEW: Danger buzzer function (continuous fast beep)
void activateDangerBuzzer(bool enable) {
  if(enable) {
    tone(DANGER_BUZZER_PIN, BUZZER_FREQ_DANGER);
  } else {
    noTone(DANGER_BUZZER_PIN);
  }
}

// NEW: Warning buzzer function (intermittent beep)
void activateWarningBuzzer() {
  static unsigned long lastBuzzerTime = 0;
  static bool buzzerState = false;
  
  unsigned long currentMillis = millis();
  
  if(currentMillis - lastBuzzerTime >= (buzzerState ? BUZZER_ON_TIME : BUZZER_OFF_TIME)) {
    buzzerState = !buzzerState;
    if(buzzerState) {
      tone(DANGER_BUZZER_PIN, BUZZER_FREQ_WARNING);
    } else {
      noTone(DANGER_BUZZER_PIN);
    }
    lastBuzzerTime = currentMillis;
  }
}

// === EXISTING FUNCTIONS === //
void turnRight() {
  Serial.println("Executing right turn...");
  
  // Left motors forward
  digitalWrite(M1_IN1, HIGH); digitalWrite(M1_IN2, LOW);
  digitalWrite(M3_IN5, HIGH); digitalWrite(M3_IN6, LOW);
  
  // Right motors backward
  digitalWrite(M2_IN3, LOW); digitalWrite(M2_IN4, HIGH);
  digitalWrite(M4_IN7, LOW); digitalWrite(M4_IN8, HIGH);
  
  // Set turning speed
  analogWrite(M1_ENA, TURN_SPEED);
  analogWrite(M2_ENB, TURN_SPEED);
  analogWrite(M3_ENC, TURN_SPEED);
  analogWrite(M4_END, TURN_SPEED);
  
  delay(TURN_DURATION);
  allStop();
}

void setFrontMotors(int speed) {
  analogWrite(M1_ENA, speed);
  analogWrite(M2_ENB, speed);
}

void setRearMotors(int speed) {
  analogWrite(M3_ENC, speed);
  analogWrite(M4_END, speed);
}

void setAllMotors(int speed) {
  setFrontMotors(speed);
  setRearMotors(speed);
}

void setAllDirections(bool dir1, bool dir2) {
  digitalWrite(M1_IN1, dir1); digitalWrite(M1_IN2, dir2);
  digitalWrite(M2_IN3, dir1); digitalWrite(M2_IN4, dir2);
  digitalWrite(M3_IN5, dir1); digitalWrite(M3_IN6, dir2);
  digitalWrite(M4_IN7, dir1); digitalWrite(M4_IN8, dir2);
}

void allStop() {
  setAllMotors(STOP);
  setAllDirections(LOW, LOW);
  noTone(DANGER_BUZZER_PIN); // NEW: Ensure buzzer stops when car stops
}

void emergencyStop() {
  setFrontMotors(STOP);
  for (int i = MEDIUM_SPEED; i >= STOP; i -= 20) {
    setRearMotors(i);
    delay(15);
  }
  allStop();
}

float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  return pulseIn(ECHO_PIN, HIGH) * 0.034 / 2;
}