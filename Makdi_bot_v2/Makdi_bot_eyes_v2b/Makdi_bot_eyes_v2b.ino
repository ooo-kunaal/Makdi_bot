#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>

// OLED display dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

// Create OLED display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Servo objects
Servo servo1; // front right knee
Servo servo2; // front right hip
Servo servo3; // back right knee
Servo servo4; // back right hip
Servo servo5; // front left knee
Servo servo6; // front left hip
Servo servo7; // back left knee
Servo servo8; // back left hip

// Single Eye Animation Variables
int eyeX = 64;  // Center X position
int eyeY = 20;  // Center Y position
int eyeWidth = 30, eyeHeight = 35;  // Eye dimensions
int targetOffsetX = 0, targetOffsetY = 0;  // Target eye position
int moveSpeed = 5;  // Controls smooth eye motion
int blinkState = 0;  // 0 = open, 1 = blink
unsigned long lastBlinkTime = 0, moveTime = 0;

void setup() {
  Serial.begin(115200);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("SSD1306 allocation failed");
    for (;;); 
  }
  display.clearDisplay();
  display.display();

  // Attach servos
  servo1.attach(27);
  servo2.attach(14);
  servo3.attach(13);
  servo4.attach(12); 
  servo5.attach(16);
  servo6.attach(4);
  servo7.attach(15);
  servo8.attach(2);

  home();
}

void loop() {
  updateEye();  // Animate single eye
  delay(30);    // Prevent flickering
}

//------------------------------------------------
// OLED Eye Animation Functions
//------------------------------------------------

void updateEye() {
  unsigned long currentTime = millis();

  // Blinking logic (kept unchanged)
  if (currentTime - lastBlinkTime > 4000 && blinkState == 0) {
    blinkState = 1;
    lastBlinkTime = currentTime;
  } else if (currentTime - lastBlinkTime > 150 && blinkState == 1) {
    blinkState = 0;
    lastBlinkTime = currentTime;
  }

  // More frequent left-right movement
  if (currentTime - moveTime > random(1000, 2500) && blinkState == 0) { 
    int moveDirection = random(0, 10);  // More weight on left-right movements

    if (moveDirection < 4) { 
      targetOffsetX = random(-12, -5); // Look left
      targetOffsetY = 0;
    } else if (moveDirection < 8) {
      targetOffsetX = random(5, 12);  // Look right
      targetOffsetY = 0;
    } else {
      targetOffsetX = random(-5, 5);  // Occasionally up/down
      targetOffsetY = random(-5, 5);
    }

    moveTime = currentTime;
  }

  // Smooth transition
  static int offsetX = 0, offsetY = 0;
  offsetX += (targetOffsetX - offsetX) / moveSpeed;
  offsetY += (targetOffsetY - offsetY) / moveSpeed;

  // Clear OLED display
  display.clearDisplay();

  // Draw the eye
  if (blinkState == 0) {
    drawEye(eyeX + offsetX, eyeY + offsetY, "angry");
  } else {
    // Eye blink
    display.fillRect(eyeX - 15, eyeY + eyeHeight / 2 - 2, eyeWidth, 4, WHITE);
  }

  display.display();
}

void drawEye(int x, int y, String emotion) {
  // Outer eye (big circle)
  display.fillCircle(x, y + 10, 25, WHITE);

  // Pupil (smaller circle)
  display.fillCircle(x, y + 10, 8, BLACK);

  // emotions
  if (emotion == "angry") {
    display.fillCircle(x, y + 10 - 20, 20, BLACK);  // Angry
  } else if (emotion == "happy") {
    display.fillCircle(x, y + 10 + 20, 20, BLACK);  // Happy
  } 
  
}

//------------------------------------------------
// Servo Control Functions
//------------------------------------------------

void home() { 
  Serial.println("home");
  servo2.write(45);
  servo4.write(135);
  servo6.write(135);
  servo8.write(45);
  delay(100);
  stand();
  delay(250);
}

void sleep() {
  Serial.println("sleeping");
  servo1.write(90);   // 0 up, 90 flat, 180 down
  servo7.write(90);   // 0 up, 90 flat, 180 down
  servo3.write(90);   // 0 down, 90 flat, 180 up 
  servo5.write(90);   // 0 down, 90 flat, 180 up  
}

void stand() {
  Serial.println("stand");
  servo1.write(0);
  delay(250);
  servo7.write(0);
  delay(250);
  servo3.write(180);
  delay(250);
  servo5.write(180);
  delay(250);
}

void hello() {
  Serial.println("say hello");
  servo1.write(50);   // 0 up, 90 flat, 180 down
  servo7.write(50);   // 0 up, 90 flat, 180 down
  servo3.write(130);   // 0 down, 90 flat, 180 up 
  servo5.write(130);   // 0 down, 90 flat, 180 up  
  delay(200);
  servo1.write(180);
  delay(200);
  // Step 3: Shake hip (servo2) while keeping the knee up
  for (int i = 0; i < 3; i++) {  // Shake hip back and forth 3 times
    servo2.write(45+15);  // Move hip slightly forward
    delay(150);
    servo2.write(45-15);  // Move hip slightly backward
    delay(150);
  }
  home();
}

void forward() { 
  Serial.println("step ahead");
  // Lift knees
  servo3.write(170);
  servo5.write(170);
  delay(150);  
  // Move hips forward
  servo4.write(90);  // 135 to 90
  servo6.write(180); // 135 to 180
  delay(200);
  // knees down
  servo3.write(180);
  servo5.write(180);
  delay(150);   
  // Lift knees
  servo1.write(10);
  servo7.write(10);
  delay(150);  
  // Move hips forward
  servo2.write(0);  // 45 to 0
  servo8.write(90); // 45 to 90
  delay(200);   
  // knees down
  servo1.write(0);
  servo7.write(0);
  delay(150);   
  // reset to hips and legs to home
  servo2.write(45);
  servo4.write(135);
  servo6.write(135);
  servo8.write(45);
  delay(200);    
}

void backward() { 
  Serial.println("step back");
  // Lift knees
  servo3.write(170);
  servo5.write(170);
  delay(150);  
  // Move hips backwards
  servo4.write(180); // 135 to 180
  servo6.write(90);  // 135 to 90
  delay(200);
  // knees down
  servo3.write(180);
  servo5.write(180);
  delay(150);   
  // Lift knees
  servo1.write(10);
  servo7.write(10);
  delay(150);  
  // Move hips forward
  servo2.write(90); // 45 to 90
  servo8.write(0);  // 45 to 0
  delay(200);   
  // knees down
  servo1.write(0);
  servo7.write(0);
  delay(150);   
  // reset to hips and legs to home
  servo2.write(45);
  servo4.write(135);
  servo6.write(135);
  servo8.write(45);
  delay(200);    
}

void right() { 
  Serial.println("turning right");
  // Lift knees
  servo3.write(170);
  servo5.write(170);
  delay(200);  
  // Move hips to right
  servo4.write(180); // 135 to 180
  servo6.write(180); // 135 to 180
  delay(150);
  // knees down
  servo3.write(180);
  servo5.write(180);
  delay(150);   
  // Lift knees
  servo1.write(30);
  servo7.write(30);
  delay(200);  
  // Move hips to right
  servo2.write(90); // 45 to 90
  servo8.write(90); // 45 to 90
  delay(150);   
  // knees down
  servo1.write(0);
  servo7.write(0);
  delay(150);   
  // reset to hips and legs to home
  servo2.write(45);
  servo4.write(135);
  servo6.write(135);
  servo8.write(45);
  delay(200);    
}

void left() { 
  Serial.println("turning left");
  // Lift knees
  servo3.write(170);
  servo5.write(170);
  delay(200);  
  // Move hips to left
  servo4.write(90); // 135 to 90
  servo6.write(90); // 135 to 90
  delay(150);
  // knees down
  servo3.write(180);
  servo5.write(180);
  delay(150);     
  // Lift knees
  servo1.write(30);
  servo7.write(30);
  delay(200);  
  // Move hips to right
  servo2.write(0); // 45 to 0
  servo8.write(0); // 45 to 0
  delay(150);   
  // knees down
  servo1.write(0);
  servo7.write(0);
  delay(150);   
  // reset to hips and legs to home
  servo2.write(45);
  servo4.write(135);
  servo6.write(135);
  servo8.write(45);
  delay(200);    
}

void jump() {
  // STEP 1: sleep
  Serial.println("jump step 1");
  sleep();
  delay(100);
  // STEP 2: lift off
  Serial.println("jump step 2");
  servo3.write(135); 
  servo5.write(135);   
  servo1.write(45);    
  servo7.write(45);    
  delay(200);          
  // STEP 3: Land & sleep again
  Serial.println("jump step 3");
  sleep();
  delay(100);
}

