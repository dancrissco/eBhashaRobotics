#include <WiFi.h>

// Replace with your WiFi credentials
const char* ssid     = "xxxxx";
const char* password = "xxxxx";

// Create web server on port 80
WiFiServer server(80);

// Motor A pins
const int ENA = 13;
const int IN1 = 6;
const int IN2 = 7;

// Motor B pins
const int ENB = 8;
const int IN3 = 9;
const int IN4 = 10;

void setup() {
  Serial.begin(115200);

  // Motor control pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Start WiFi connection
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

// Motor control functions
void moveForward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  digitalWrite(ENA, HIGH); digitalWrite(ENB, HIGH);
}

void moveBackward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  digitalWrite(ENA, HIGH); digitalWrite(ENB, HIGH);
}

void turnLeft() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  digitalWrite(ENA, HIGH); digitalWrite(ENB, HIGH);
}

void turnRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  digitalWrite(ENA, HIGH); digitalWrite(ENB, HIGH);
}

void stopMotors() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            // Respond with basic HTML
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("<h2>ESP32 Motor Web Control</h2>");
            client.println("Click <a href=\"/F\">Forward</a><br>");
            client.println("Click <a href=\"/B\">Backward</a><br>");
            client.println("Click <a href=\"/L\">Left</a><br>");
            client.println("Click <a href=\"/R\">Right</a><br>");
            client.println("Click <a href=\"/S\">Stop</a><br>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        // Command parser with debug
        if (currentLine.endsWith("GET /F")) {
          Serial.println("[CMD] Forward");
          moveForward();
        }
        if (currentLine.endsWith("GET /B")) {
          Serial.println("[CMD] Backward");
          moveBackward();
        }
        if (currentLine.endsWith("GET /L")) {
          Serial.println("[CMD] Left");
          turnLeft();
        }
        if (currentLine.endsWith("GET /R")) {
          Serial.println("[CMD] Right");
          turnRight();
        }
        if (currentLine.endsWith("GET /S")) {
          Serial.println("[CMD] Stop");
          stopMotors();
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

