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

// Eye Animation Variables
int leftEyeX = 45, rightEyeX = 80, eyeY = 18;  // Positions
int eyeWidth = 25, eyeHeight = 30;  // Eye dimensions
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
  updateEyes();  // Animate eyes
  delay(30);  // Prevent flickering
}

//------------------------------------------------
// OLED Eye Animation Functions
//------------------------------------------------

void updateEyes() {
  unsigned long currentTime = millis();

  // Blinking logic
  if (currentTime - lastBlinkTime > 2000 && blinkState == 0) {
    blinkState = 1;  
    lastBlinkTime = currentTime;
  } else if (currentTime - lastBlinkTime > 150 && blinkState == 1) {
    blinkState = 0;
    lastBlinkTime = currentTime;
  }

  // Random eye movement logic
  if (currentTime - moveTime > random(1500, 3000) && blinkState == 0) {
    targetOffsetX = random(-10, 10);
    targetOffsetY = random(-8, 8);
    moveTime = currentTime;
  }

  // Smooth interpolation
  static int offsetX = 0, offsetY = 0;
  offsetX += (targetOffsetX - offsetX) / moveSpeed;
  offsetY += (targetOffsetY - offsetY) / moveSpeed;

  // Clear OLED display
  display.clearDisplay();

  // Draw left eye
  if (blinkState == 0) {
    drawEye(leftEyeX + offsetX, eyeY + offsetY);
  } else {
    display.fillRect(leftEyeX + offsetX, eyeY + offsetY + eyeHeight / 2 - 2, eyeWidth, 4, WHITE);
  }

  // Draw right eye
  if (blinkState == 0) {
    drawEye(rightEyeX + offsetX, eyeY + offsetY);
  } else {
    display.fillRect(rightEyeX + offsetX, eyeY + offsetY + eyeHeight / 2 - 2, eyeWidth, 4, WHITE);
  }

  display.display();
}

// Function to draw a single eye
void drawEye(int x, int y) {
  display.fillRoundRect(x, y, eyeWidth, eyeHeight, 5, WHITE);
}

//------------------------------------------------
// Servo Control Functions
//------------------------------------------------

void home() { 
  servo2.write(45);
  servo4.write(135);
  servo6.write(135);
  servo8.write(45);
  delay(100);
  stand();
  delay(250);
}

void stand() {
  servo1.write(0);
  delay(250);
  servo7.write(0);
  delay(250);
  servo3.write(180);
  delay(250);
  servo5.write(180);
  delay(250);
}

void forward() {
  servo3.write(170);
  servo5.write(170);
  delay(150);
  servo4.write(90);
  servo6.write(180);
  delay(200);
  servo3.write(180);
  servo5.write(180);
  delay(150);
  
  servo1.write(10);
  servo7.write(10);
  delay(150);
  servo2.write(0);
  servo8.write(90);
  delay(200);
  servo1.write(0);
  servo7.write(0);
  delay(150);

  servo2.write(45);
  servo4.write(135);
  servo6.write(135);
  servo8.write(45);
}

void backward() {
  servo3.write(170);
  servo5.write(170);
  delay(150);
  servo4.write(180);
  servo6.write(90);
  delay(200);
  servo3.write(180);
  servo5.write(180);
  delay(150);

  servo1.write(10);
  servo7.write(10);
  delay(150);
  servo2.write(90);
  servo8.write(0);
  delay(200);
  servo1.write(0);
  servo7.write(0);
  delay(150);

  servo2.write(45);
  servo4.write(135);
  servo6.write(135);
  servo8.write(45);
  delay(200);    
}

void right() {
  servo3.write(170);
  servo5.write(170);
  delay(200);
  servo4.write(180);
  servo6.write(180);
  delay(150);
  servo3.write(180);
  servo5.write(180);
  delay(150);

  servo1.write(30);
  servo7.write(30);
  delay(200);
  servo2.write(90);
  servo8.write(90);
  delay(150);
  servo1.write(0);
  servo7.write(0);
  delay(150);

  servo2.write(45);
  servo4.write(135);
  servo6.write(135);
  servo8.write(45);
  delay(200);    
}

void left() {
  servo3.write(170);
  servo5.write(170);
  delay(200);
  servo4.write(90);
  servo6.write(90);
  delay(150);
  servo3.write(180);
  servo5.write(180);
  delay(150);

  servo1.write(30);
  servo7.write(30);
  delay(200);
  servo2.write(0);
  servo8.write(0);
  delay(150);
  servo1.write(0);
  servo7.write(0);
  delay(150);

  servo2.write(45);
  servo4.write(135);
  servo6.write(135);
  servo8.write(45);
  delay(200);    
}
