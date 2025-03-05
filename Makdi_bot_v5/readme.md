Version 5 is all about Gesture Recognition & Control of the Robot Arm - recorded using laptop's webcam & processed via Mediapipe Hands & OpenCV. 

Worflow -
1. Webcam/ ESP32 Camera sends a live video feed to the Python which is captured using OpenCV.
2. Using MediaPipe Hands, various gestures are captured & processed. The output of this processing are servo motor angles for the robot arm.
3. The python script communicates the servo angles to ESP32 via HTTP Requests over wifi.

![hand_landmarks](https://github.com/user-attachments/assets/55094157-03ae-40a6-a4b7-5b7700c81e22)

![Screenshot 2025-03-05 180832](https://github.com/user-attachments/assets/c5decde5-7cac-414d-8061-61153ed0edc8)
