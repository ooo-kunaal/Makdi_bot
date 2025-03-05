#include <ESP32Servo.h>

Servo servo1; // front right knee
Servo servo2; // front right hip
Servo servo3; // back right knee
Servo servo4; // back right hip

Servo servo5; // front left knee
Servo servo6; // front left hip
Servo servo7; // back left knee
Servo servo8; // back left hip

void setup() {
  Serial.begin(115200);
  
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
  // forward();
  // backward();
  // right();
  // left();
  // hello();
}

void home() { 
  // initialize hip angles
  servo2.write(45);
  servo4.write(135);
  servo6.write(135);
  servo8.write(45);
  delay(100);
  // initialize legs to stand
  stand();
  delay(250);
}

void sleep() {
  servo1.write(90);   // 0 up, 90 flat, 180 down
  servo7.write(90);   // 0 up, 90 flat, 180 down
  servo3.write(90);   // 0 down, 90 flat, 180 up 
  servo5.write(90);   // 0 down, 90 flat, 180 up  
}

void hello() {
  servo1.write(50);
  servo7.write(50);
  servo3.write(130);
  servo5.write(130);
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