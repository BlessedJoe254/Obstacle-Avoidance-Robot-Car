// Robot Car with 4 Motors, 4 Ultrasonic Sensors, Buzzer, and LED Turn Signals
// Uses single H-bridge for motor control (two motors per channel)

// Motor control pins (using single H-bridge like L298N)
// Left side motors (connected in parallel)
#define LEFT_MOTOR_EN 5    // PWM speed control for left motors
#define LEFT_MOTOR_IN1 6   // Direction control for left motors
#define LEFT_MOTOR_IN2 7   // Direction control for left motors

// Right side motors (connected in parallel)
#define RIGHT_MOTOR_EN 10  // PWM speed control for right motors
#define RIGHT_MOTOR_IN1 8  // Direction control for right motors
#define RIGHT_MOTOR_IN2 9  // Direction control for right motors

// Ultrasonic sensor pins
#define FRONT_TRIG A0
#define FRONT_ECHO A1
#define LEFT_TRIG A2
#define LEFT_ECHO A3
#define RIGHT_TRIG 11
#define RIGHT_ECHO 12
#define BACK_TRIG 13
#define BACK_ECHO A4

// Buzzer pin
#define BUZZER 4

// LED pins for turn signals
#define LEFT_LED A5
#define RIGHT_LED 2
#define FRONT_LED 3
#define BACK_LED A6

// Distance thresholds (in cm)
#define FRONT_DISTANCE_THRESHOLD 30
#define SIDE_DISTANCE_THRESHOLD 15
#define BACK_DISTANCE_THRESHOLD 20

void setup() {
  // Initialize motor control pins
  pinMode(LEFT_MOTOR_EN, OUTPUT);
  pinMode(LEFT_MOTOR_IN1, OUTPUT);
  pinMode(LEFT_MOTOR_IN2, OUTPUT);
  pinMode(RIGHT_MOTOR_EN, OUTPUT);
  pinMode(RIGHT_MOTOR_IN1, OUTPUT);
  pinMode(RIGHT_MOTOR_IN2, OUTPUT);
  
  // Initialize ultrasonic sensors
  pinMode(FRONT_TRIG, OUTPUT);
  pinMode(FRONT_ECHO, INPUT);
  pinMode(LEFT_TRIG, OUTPUT);
  pinMode(LEFT_ECHO, INPUT);
  pinMode(RIGHT_TRIG, OUTPUT);
  pinMode(RIGHT_ECHO, INPUT);
  pinMode(BACK_TRIG, OUTPUT);
  pinMode(BACK_ECHO, INPUT);
  
  // Initialize buzzer
  pinMode(BUZZER, OUTPUT);
  
  // Initialize LED pins
  pinMode(LEFT_LED, OUTPUT);
  pinMode(RIGHT_LED, OUTPUT);
  pinMode(FRONT_LED, OUTPUT);
  pinMode(BACK_LED, OUTPUT);
  
  // Start with motors stopped and all LEDs off
  stopMotors();
  allLEDsOff();
  
  // Serial monitor for debugging
  Serial.begin(9600);
}

void loop() {
  // Read distances from all sensors
  int frontDist = getDistance(FRONT_TRIG, FRONT_ECHO);
  int leftDist = getDistance(LEFT_TRIG, LEFT_ECHO);
  int rightDist = getDistance(RIGHT_TRIG, RIGHT_ECHO);
  int backDist = getDistance(BACK_TRIG, BACK_ECHO);
  
  // Debug output
  Serial.print("Front: ");
  Serial.print(frontDist);
  Serial.print("cm | Left: ");
  Serial.print(leftDist);
  Serial.print("cm | Right: ");
  Serial.print(rightDist);
  Serial.print("cm | Back: ");
  Serial.print(backDist);
  Serial.println("cm");
  
  // Check for obstacles and react
  if (frontDist < FRONT_DISTANCE_THRESHOLD) {
    // Object in front - beep and avoid
    digitalWrite(BUZZER, HIGH);
    digitalWrite(FRONT_LED, HIGH); // Front obstacle indicator
    delay(100);
    digitalWrite(BUZZER, LOW);
    digitalWrite(FRONT_LED, LOW);
    
    // Check which side has more space
    if (leftDist > rightDist && leftDist > SIDE_DISTANCE_THRESHOLD) {
      // More space on left - turn left with left LED signal
      blinkLeftLED();
      turnLeft(150);
    } 
    else if (rightDist > SIDE_DISTANCE_THRESHOLD) {
      // More space on right - turn right with right LED signal
      blinkRightLED();
      turnRight(150);
    } 
    else {
      // No space on sides - go backward with back LED
      digitalWrite(BACK_LED, HIGH);
      moveBackward(200);
      delay(500);
      digitalWrite(BACK_LED, LOW);
    }
  } 
  else if (leftDist < SIDE_DISTANCE_THRESHOLD) {
    // Too close on left - adjust right with right LED
    digitalWrite(RIGHT_LED, HIGH);
    adjustRight(100);
    delay(300);
    digitalWrite(RIGHT_LED, LOW);
  } 
  else if (rightDist < SIDE_DISTANCE_THRESHOLD) {
    // Too close on right - adjust left with left LED
    digitalWrite(LEFT_LED, HIGH);
    adjustLeft(100);
    delay(300);
    digitalWrite(LEFT_LED, LOW);
  } 
  else if (backDist < BACK_DISTANCE_THRESHOLD) {
    // Too close behind - move forward with front LED
    digitalWrite(FRONT_LED, HIGH);
    moveForward(150);
    delay(300);
    digitalWrite(FRONT_LED, LOW);
  } 
  else {
    // No obstacles - move forward
    moveForward(150);
  }
  
  delay(50); // Small delay between readings
}

// Function to measure distance from ultrasonic sensor
int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;
  
  return distance;
}

// LED control functions
void blinkLeftLED() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(LEFT_LED, HIGH);
    delay(200);
    digitalWrite(LEFT_LED, LOW);
    delay(200);
  }
}

void blinkRightLED() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(RIGHT_LED, HIGH);
    delay(200);
    digitalWrite(RIGHT_LED, LOW);
    delay(200);
  }
}

void allLEDsOff() {
  digitalWrite(LEFT_LED, LOW);
  digitalWrite(RIGHT_LED, LOW);
  digitalWrite(FRONT_LED, LOW);
  digitalWrite(BACK_LED, LOW);
}

// Motor control functions for 4 motors (2 per side)
void moveForward(int speed) {
  // Left motors forward
  analogWrite(LEFT_MOTOR_EN, speed);
  digitalWrite(LEFT_MOTOR_IN1, HIGH);
  digitalWrite(LEFT_MOTOR_IN2, LOW);
  
  // Right motors forward
  analogWrite(RIGHT_MOTOR_EN, speed);
  digitalWrite(RIGHT_MOTOR_IN1, HIGH);
  digitalWrite(RIGHT_MOTOR_IN2, LOW);
}

void moveBackward(int speed) {
  // Left motors backward
  analogWrite(LEFT_MOTOR_EN, speed);
  digitalWrite(LEFT_MOTOR_IN1, LOW);
  digitalWrite(LEFT_MOTOR_IN2, HIGH);
  
  // Right motors backward
  analogWrite(RIGHT_MOTOR_EN, speed);
  digitalWrite(RIGHT_MOTOR_IN1, LOW);
  digitalWrite(RIGHT_MOTOR_IN2, HIGH);
}

void turnLeft(int speed) {
  // Left motors backward
  analogWrite(LEFT_MOTOR_EN, speed);
  digitalWrite(LEFT_MOTOR_IN1, LOW);
  digitalWrite(LEFT_MOTOR_IN2, HIGH);
  
  // Right motors forward
  analogWrite(RIGHT_MOTOR_EN, speed);
  digitalWrite(RIGHT_MOTOR_IN1, HIGH);
  digitalWrite(RIGHT_MOTOR_IN2, LOW);
}

void turnRight(int speed) {
  // Left motors forward
  analogWrite(LEFT_MOTOR_EN, speed);
  digitalWrite(LEFT_MOTOR_IN1, HIGH);
  digitalWrite(LEFT_MOTOR_IN2, LOW);
  
  // Right motors backward
  analogWrite(RIGHT_MOTOR_EN, speed);
  digitalWrite(RIGHT_MOTOR_IN1, LOW);
  digitalWrite(RIGHT_MOTOR_IN2, HIGH);
}

void adjustLeft(int speed) {
  // Left motors slow forward
  analogWrite(LEFT_MOTOR_EN, speed/2);
  digitalWrite(LEFT_MOTOR_IN1, HIGH);
  digitalWrite(LEFT_MOTOR_IN2, LOW);
  
  // Right motors stop
  analogWrite(RIGHT_MOTOR_EN, 0);
  digitalWrite(RIGHT_MOTOR_IN1, LOW);
  digitalWrite(RIGHT_MOTOR_IN2, LOW);
}

void adjustRight(int speed) {
  // Left motors stop
  analogWrite(LEFT_MOTOR_EN, 0);
  digitalWrite(LEFT_MOTOR_IN1, LOW);
  digitalWrite(LEFT_MOTOR_IN2, LOW);
  
  // Right motors slow forward
  analogWrite(RIGHT_MOTOR_EN, speed/2);
  digitalWrite(RIGHT_MOTOR_IN1, HIGH);
  digitalWrite(RIGHT_MOTOR_IN2, LOW);
}

void stopMotors() {
  // Stop all motors
  digitalWrite(LEFT_MOTOR_IN1, LOW);
  digitalWrite(LEFT_MOTOR_IN2, LOW);
  digitalWrite(RIGHT_MOTOR_IN1, LOW);
  digitalWrite(RIGHT_MOTOR_IN2, LOW);
  analogWrite(LEFT_MOTOR_EN, 0);
  analogWrite(RIGHT_MOTOR_EN, 0);
}