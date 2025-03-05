#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_PWMServoDriver.h>

// WiFi credentials
const char* ssid = "Phoenix-extend";
const char* password = "7003509770kushal";

// Web Server
WebServer server(80);

// PCA9685 Object
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);  // Default I2C address

// PCA9685 Servo Channels
#define SHOULDER_SERVO 3
#define ELBOW_SERVO 2
#define WRIST_SERVO 1
#define GRIPPER_SERVO 0

// Servo Limits
#define SERVOMIN  150  // Min pulse length
#define SERVOMAX  550  // Max pulse length

// Servo Angle Variables
int shoulderAngle = 150;
int elbowAngle = 135;
int wristAngle = 90;
int gripperAngle = 0;

void setup() {
  Serial.begin(115200);
  
  // Initialize PCA9685
  pwm.begin();
  delay(10);
  pwm.setPWMFreq(50);  // 50Hz PWM for servos
  delay(10);

  armHome();  // Move arm to home position

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("ESP32 Web Server IP: ");
  Serial.println(WiFi.localIP());

  // Define route for receiving servo angles
  server.on("/move", handleMove);

  server.begin();
}

// Function to handle incoming servo angle updates
void handleMove() {
  if (server.hasArg("S")) shoulderAngle = server.arg("S").toInt();
  if (server.hasArg("E")) elbowAngle = server.arg("E").toInt();
  if (server.hasArg("W")) wristAngle = server.arg("W").toInt();
  if (server.hasArg("G")) gripperAngle = server.arg("G").toInt();

  // Apply servo angles
  setServo(SHOULDER_SERVO, shoulderAngle);
  setServo(ELBOW_SERVO, elbowAngle);
  setServo(WRIST_SERVO, wristAngle);
  setServo(GRIPPER_SERVO, gripperAngle);

  Serial.printf("Updated Angles - S:%d, E:%d, W:%d, G:%d\n", shoulderAngle, elbowAngle, wristAngle, gripperAngle);
  server.send(200, "text/plain", "OK");
}

void loop() {
  server.handleClient();
}

// Move servo using PCA9685
void setServo(uint8_t channel, int angle) {
  int pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(channel, 0, pulse);
}

// Move arm to home position
void armHome() {
  setServo(SHOULDER_SERVO, 150);
  delay(250);
  setServo(ELBOW_SERVO, 120);
  delay(250);
  setServo(WRIST_SERVO, 0);
  delay(250);
  setServo(GRIPPER_SERVO, 0);
  delay(250);
}
