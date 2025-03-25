// === Pin Setup ===
const int sensorPins[5] = {A1, A2, A3, A4, A5};
const int sensorThreshold = 500;

// Motor driver pins
const int ENA = 3;
const int IN1 = 4;
const int IN2 = 5;

const int ENB = 9;
const int IN3 = 7;
const int IN4 = 8;

// === Proportional Control Constants ===
float Kp = 8.50;
int baseSpeed = 75;

void setup() {
  // Motor control pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Sensor pins
  for (int i = 0; i < 5; i++) {
    pinMode(sensorPins[i], INPUT);
  }

  Serial.begin(9600);
  Serial.println("Robot ready — starting line follow...");
}

void loop() {
  // --- Sensor Reading ---
  int sensorValues[5];
  int weights[5] = {-1, -4, 0, 4, 1};
  int position = 0;
  int activeCount = 0;

  Serial.print("Sensors: ");
  for (int i = 0; i < 5; i++) {
    sensorValues[i] = analogRead(sensorPins[i]);
    Serial.print(sensorValues[i]);
    Serial.print(" ");

    if (sensorValues[i] < sensorThreshold) {
      position += weights[i];
      activeCount++;
    }
  }

  if (activeCount == 0) {
    Serial.println(" -> ❌ Line lost!");
    stopMotors();
    return;
  }

  // --- Proportional Control ---
  int error = position / activeCount;
  int correction = Kp * error;

  int leftSpeed = baseSpeed - correction;
  int rightSpeed = baseSpeed + correction;

  leftSpeed = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);

  Serial.print(" | Error: ");
  Serial.print(error);
  Serial.print(" | L: ");
  Serial.print(leftSpeed);
  Serial.print(" R: ");
  Serial.println(rightSpeed);

  moveForward(leftSpeed, rightSpeed);
  delay(10);  // Stabilizer
}

// === Motor Control Functions ===

void moveForward(int left, int right) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, right);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, left);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

