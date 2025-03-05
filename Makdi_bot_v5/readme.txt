Version 5 is all about Gesture Recognition & Control of the Robot Arm - recorded using laptop's webcam & processed via Mediapipe Hands & OpenCV. 

Worflow -
1. Webcam/ ESP32 Camera sends a live video feed to the Python which is captured using OpenCV.
2. Using MediaPipe Hands, various gestures are captured & processed. The output of this processing are servo motor angles for the robot arm.
3. The python script communicates the servo angles to ESP32 via HTTP Requests over wifi.

