#include <ESP32Servo.h>

// Define servo objects
Servo shoulderServo;
Servo elbowServo;
Servo wristServo;
Servo gripperServo;

// Define GPIO pins for each servo

#define SHOULDER_PIN 26
#define ELBOW_PIN 33
#define WRIST_PIN 32
#define GRIPPER_PIN 5

void setup() {
  Serial.begin(9600);

  // Attach servos to respective GPIO pins
  shoulderServo.attach(SHOULDER_PIN);
  elbowServo.attach(ELBOW_PIN);
  wristServo.attach(WRIST_PIN);
  gripperServo.attach(GRIPPER_PIN);

  armHome();
  delay(100);
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "H") { 
      armHome();
    }
    else if (command == "hi") { 
      armWave();
    }
    else {
      char servoType = command.charAt(0);
      int angle = command.substring(2).toInt();
      
      switch (servoType) {
        case 'S': shoulderServo.write(angle); Serial.println("Shoulder moved"); break;
        case 'E': elbowServo.write(angle); Serial.println("Elbow moved"); break;
        case 'W': wristServo.write(angle); Serial.println("Wrist moved"); break;
        case 'G': gripperServo.write(angle); Serial.println("Gripper moved"); break;
        default: Serial.println("Invalid command! Use H, Hi or S, E, W, G followed by angle."); break;
      }
    }
  }
}
}

void armHome() {
  shoulderServo.write(150);
  delay(250);
  elbowServo.write(135);
  delay(250);
  wristServo.write(0);
  delay(250);
  gripperServo.write(0);  // 0 = Closed, 90 = Open
  delay(250);
}

void armWave() {
  Serial.println("Arm waving...");
  for (int i = 0; i < 3; i++) {
    wristServo.write(45);
    delay(300);
    wristServo.write(0);
    delay(300);
  }
  Serial.println("Wave complete");
}
