#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_PWMServoDriver.h>

// Create PCA9685 object
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);  // Default I2C address

// PCA9685 Servo Channels
#define SHOULDER_SERVO 3
#define ELBOW_SERVO 2
#define WRIST_SERVO 1
#define GRIPPER_SERVO 0

#define SERVO1 11 // Front right knee
#define SERVO2 10 // Front right hip
#define SERVO3 8  // Back right knee
#define SERVO4 9  // Back right hip
#define SERVO5 7  // Front left knee
#define SERVO6 6  // Front left hip
#define SERVO7 4  // Back left knee
#define SERVO8 5  // Back left hip

// Servo Limits
#define SERVOMIN  150  // Min pulse length
#define SERVOMAX  550  // Max pulse length

void setup() {
  Serial.begin(115200);
  
  // Initialize PCA9685
  pwm.begin();
  delay(10);
  pwm.setPWMFreq(50);  // 50Hz PWM for servos
  delay(10);

  home();  // Move servos to initial position
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    // Makdi_bot Commands
    if (command == "F") { forward(); }
    else if (command == "B") { backward(); }
    else if (command == "L") { left(); }
    else if (command == "R") { right(); }
    else if (command == "S") { sleep(); }
    else if (command == "S2") { stand(); }
    else if (command == "H") { home(); }

    // Robot Arm Commands
    else {
      char servoType = command.charAt(0);
      int angle = command.substring(2).toInt();
      int pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);

      switch (servoType) {
        case 'S': pwm.setPWM(SHOULDER_SERVO, 0, pulse); Serial.println("Shoulder moved"); break;
        case 'E': pwm.setPWM(ELBOW_SERVO, 0, pulse); Serial.println("Elbow moved"); break;
        case 'W': pwm.setPWM(WRIST_SERVO, 0, pulse); Serial.println("Wrist moved"); break;
        case 'G': pwm.setPWM(GRIPPER_SERVO, 0, pulse); Serial.println("Gripper moved"); break;
        default: Serial.println("Invalid command! Use B, S, E, W, G followed by angle."); break;
      }
    }
  }
}

// ------------------------------------------------
// Servo Movement Functions via PCA9685
// ------------------------------------------------

void home() {
  Serial.println("Home position");
  setServo(SERVO2, 45);
  setServo(SERVO4, 135);
  setServo(SERVO6, 135);
  setServo(SERVO8, 45);
  delay(100);
  stand();
  delay(100);
  Serial.println("Home ARM");
  setServo(SHOULDER_SERVO, 150);
  setServo(ELBOW_SERVO, 120);
  setServo(WRIST_SERVO, 0);
  setServo(GRIPPER_SERVO, 0);
}

void sleep() {
  Serial.println("sleeping");
  setServo(SERVO1,90);
  delay(250);
  setServo(SERVO7,90);
  delay(250);
  setServo(SERVO3,90);
  delay(250);
  setServo(SERVO5,90);
  delay(250);
}

void stand() {
  Serial.println("Stand");
  setServo(SERVO1, 0);
  delay(250);
  setServo(SERVO7, 0);
  delay(250);
  setServo(SERVO3, 180);
  delay(250);
  setServo(SERVO5, 180);
  delay(250);
}

void forward() {
  Serial.println("Step ahead");
  // Lift knees
  setServo(SERVO3, 170);
  setServo(SERVO5, 170);
  delay(150);
  // Move hips forward
  setServo(SERVO4, 90);
  setServo(SERVO6, 180);
  delay(200);
  // knees down
  setServo(SERVO3, 180);
  setServo(SERVO5, 180);
  delay(150);   
  // Lift knees
  setServo(SERVO1, 10);
  setServo(SERVO7, 10);
  delay(150);
  // Move hips forward
  setServo(SERVO2, 0);
  setServo(SERVO8, 90);
  delay(200);
  // knees down
  setServo(SERVO1, 10);
  setServo(SERVO7, 10);
  delay(150);
  // reset to hips and legs to home
  setServo(SERVO2, 45);
  setServo(SERVO4, 135);
  setServo(SERVO6, 135);
  setServo(SERVO8, 45);
  delay(100);
}

void backward() {  
  Serial.println("Step back");
  
  // Lift knees
  setServo(SERVO3, 170);
  setServo(SERVO5, 170);
  delay(150);
  
  // Move hips backward
  setServo(SERVO4, 180); // 135 to 180
  setServo(SERVO6, 90);  // 135 to 90
  delay(200);
  
  // Knees down
  setServo(SERVO3, 180);
  setServo(SERVO5, 180);
  delay(150);
  
  // Lift knees
  setServo(SERVO1, 10);
  setServo(SERVO7, 10);
  delay(150);
  
  // Move hips forward
  setServo(SERVO2, 90); // 45 to 90
  setServo(SERVO8, 0);  // 45 to 0
  delay(200);
  
  // Knees down
  setServo(SERVO1, 0);
  setServo(SERVO7, 0);
  delay(150);
  
  // Reset hips and legs to home position
  setServo(SERVO2, 45);
  setServo(SERVO4, 135);
  setServo(SERVO6, 135);
  setServo(SERVO8, 45);
  delay(200);
}

void right() {  
  Serial.println("Turning right");
  
  // Lift knees
  setServo(SERVO3, 170);
  setServo(SERVO5, 170);
  delay(200);
  
  // Move hips to right
  setServo(SERVO4, 180); // 135 to 180
  setServo(SERVO6, 180); // 135 to 180
  delay(150);
  
  // Knees down
  setServo(SERVO3, 180);
  setServo(SERVO5, 180);
  delay(150);
  
  // Lift knees
  setServo(SERVO1, 30);
  setServo(SERVO7, 30);
  delay(200);
  
  // Move hips to right
  setServo(SERVO2, 90); // 45 to 90
  setServo(SERVO8, 90); // 45 to 90
  delay(150);
  
  // Knees down
  setServo(SERVO1, 0);
  setServo(SERVO7, 0);
  delay(150);
  
  // Reset hips and legs to home position
  setServo(SERVO2, 45);
  setServo(SERVO4, 135);
  setServo(SERVO6, 135);
  setServo(SERVO8, 45);
  delay(200);
}

void left() {  
  Serial.println("Turning left");
  
  // Lift knees
  setServo(SERVO3, 170);
  setServo(SERVO5, 170);
  delay(200);
  
  // Move hips to left
  setServo(SERVO4, 90); // 135 to 90
  setServo(SERVO6, 90); // 135 to 90
  delay(150);
  
  // Knees down
  setServo(SERVO3, 180);
  setServo(SERVO5, 180);
  delay(150);
  
  // Lift knees
  setServo(SERVO1, 30);
  setServo(SERVO7, 30);
  delay(200);
  
  // Move hips to left
  setServo(SERVO2, 0); // 45 to 0
  setServo(SERVO8, 0); // 45 to 0
  delay(150);
  
  // Knees down
  setServo(SERVO1, 0);
  setServo(SERVO7, 0);
  delay(150);
  
  // Reset hips and legs to home position
  setServo(SERVO2, 45);
  setServo(SERVO4, 135);
  setServo(SERVO6, 135);
  setServo(SERVO8, 45);
  delay(200);
}

// Function to move a servo using PCA9685
void setServo(uint8_t channel, int angle) {
  int pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(channel, 0, pulse);
}