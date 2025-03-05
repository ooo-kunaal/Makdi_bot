import cv2
import numpy as np
import urllib.request
import requests
import mediapipe as mp

# 🔹 ESP32 Robot Arm Controller IP
ESP32_ROBOT_IP = "http://192.168.0.197"

# 🔹 ESP32-CAM Stream URL
ESP32_URL = "http://192.168.0.198/cam-hi.jpg"  # Update with your ESP32 IP

# 🔹 Initialize MediaPipe Hands
mp_hands = mp.solutions.hands
mp_drawing = mp.solutions.drawing_utils
hands = mp_hands.Hands(min_detection_confidence=0.5, min_tracking_confidence=0.5)

# 🔹 Define Hand Gesture Mapping
OPEN_HAND = "open"
CLOSED_FIST = "fist"

def send_gripper_command(gripper_angle):
    """Send gripper command to ESP32"""
    url = f"{ESP32_ROBOT_IP}/move?S=150&E=120&W=0&G={gripper_angle}"
    try:
        requests.get(url)
        print(f"Gripper moved to {gripper_angle}°")
    except Exception as e:
        print(f"Failed to send command: {e}")

def classify_hand_gesture(landmarks):
    """Classify hand gesture based on landmarks"""
    if not landmarks:
        return None  # No classification if no hand is detected

    # Calculate the distance between fingers
    thumb_tip = landmarks[4]  # Thumb tip
    index_tip = landmarks[8]  # Index finger tip
    middle_tip = landmarks[12]  # Middle finger tip
    ring_tip = landmarks[16]  # Ring finger tip
    pinky_tip = landmarks[20]  # Pinky tip

    # Check if all fingers are open (open hand)
    if (index_tip.y < landmarks[6].y and
        middle_tip.y < landmarks[10].y and
        ring_tip.y < landmarks[14].y and
        pinky_tip.y < landmarks[18].y):
        return OPEN_HAND

    # Check if fingers are curled (fist)
    if (index_tip.y > landmarks[6].y and
        middle_tip.y > landmarks[10].y and
        ring_tip.y > landmarks[14].y and
        pinky_tip.y > landmarks[18].y):
        return CLOSED_FIST

    return None  # No classification if it's an unknown gesture

def process_frame():
    """Process frames from ESP32-CAM and detect hand gestures"""
    try:
        # Fetch image from ESP32-CAM
        img_resp = urllib.request.urlopen(ESP32_URL)
        imgnp = np.array(bytearray(img_resp.read()), dtype=np.uint8)
        frame = cv2.imdecode(imgnp, -1)

        # Convert frame to RGB (required for MediaPipe)
        frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        # Detect hands
        results = hands.process(frame_rgb)
        gesture = None

        if results.multi_hand_landmarks:
            for hand_landmarks in results.multi_hand_landmarks:
                # Draw hand landmarks
                mp_drawing.draw_landmarks(frame, hand_landmarks, mp_hands.HAND_CONNECTIONS)
                gesture = classify_hand_gesture(hand_landmarks.landmark)

        # Send gripper command based on detected gesture
        if gesture == OPEN_HAND:
            send_gripper_command(90)  # Open gripper
        elif gesture == CLOSED_FIST:
            send_gripper_command(0)  # Close gripper

        cv2.putText(frame, f"Gesture: {gesture if gesture else 'No gesture'}", (20, 40), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

        return frame

    except Exception as e:
        print(f"Error: {e}")
        return None

# 🔹 Start Live Detection
while True:
    frame = process_frame()
    if frame is not None:
        cv2.imshow("Hand Gesture Control - ESP32-CAM", frame)

    if cv2.waitKey(1) & 0xFF == ord("q"):
        break

cv2.destroyAllWindows()
