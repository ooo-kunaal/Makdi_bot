import cv2
import numpy as np
import urllib.request
import requests
from requests.adapters import HTTPAdapter
from requests.packages.urllib3.util.retry import Retry
import mediapipe as mp
import math
import time

# ESP32 Robot Arm Controller IP
ESP32_ROBOT_IP = "http://192.168.0.191"

# ESP32-CAM Stream URL
# ESP32_URL = "http://192.168.0.192/cam-hi.jpg"

# Initialize MediaPipe Hands
mp_hands = mp.solutions.hands
mp_drawing = mp.solutions.drawing_utils
hands = mp_hands.Hands(min_detection_confidence=0.6, min_tracking_confidence=0.6)

# Servo Angle Ranges
SHOULDER_MIN, SHOULDER_MAX = 30, 150
ELBOW_MIN, ELBOW_MAX = 45, 120
WRIST_FIXED = 0
GRIPPER_OPEN, GRIPPER_CLOSE = 90, 0

# Home Position
HOME_SHOULDER = 150
HOME_ELBOW = 120
HOME_WRIST = 0
HOME_GRIPPER = 0

# Wrist Y-Position Limits for Elbow Mapping
WRIST_Y_MIN, WRIST_Y_MAX = 0.4, 0.9

# Raw Angle Limits for Shoulder Mapping
SHOULDER_RAW_MIN, SHOULDER_RAW_MAX = 80, 160

# Store last angles for fist logic (start with home)
previous_angles = {"S": 150, "E": 120}

# State Variable
no_hand_state = False

# Helper Function to Clamp Values
clamp = lambda n, minn, maxn: max(min(maxn, n), minn)

def send_arm_command(shoulder, elbow, wrist, gripper, force_home=False):
    global previous_angles

    if force_home:
        shoulder, elbow, wrist, gripper = HOME_SHOULDER, HOME_ELBOW, HOME_WRIST, HOME_GRIPPER
        previous_angles["S"], previous_angles["E"] = HOME_SHOULDER, HOME_ELBOW
    else:
        pass

    url = f"{ESP32_ROBOT_IP}/move?S={shoulder}&E={elbow}&W={wrist}&G={gripper}"
    try:
        session = requests.Session()
        retry = Retry(connect=3, backoff_factor=0.5)
        adapter = HTTPAdapter(max_retries=retry)
        session.mount('http://', adapter)
        session.get(url, timeout=5)
        print(f"Sent Angles → S:{shoulder}, E:{elbow}, W:{wrist}, G:{gripper}")
    except requests.exceptions.RequestException as e:
        print(f"Failed to send command: {e}")

    # Update previous angles
    previous_angles["S"] = shoulder
    previous_angles["E"] = elbow

def is_fist(hand_landmarks, palm_size):
    """Check if hand is a fist based on finger tip distances from wrist."""
    WRIST = hand_landmarks.landmark[0]
    distance_sum = sum(math.sqrt((WRIST.x - hand_landmarks.landmark[i].x)**2 + (WRIST.y - hand_landmarks.landmark[i].y)**2) for i in [8, 12, 16, 20])
    avg_distance = distance_sum / palm_size
    return avg_distance < 6.5

def classify_hand_position(hand_landmarks):
    
    #----------------------------------------
    # 1. Store landmark positions
    #----------------------------------------
    WRIST = hand_landmarks.landmark[0]
    INDEX_MCP = hand_landmarks.landmark[5]

    #----------------------------------------
    # 2. Shoulder Servo Angle (Wrist & Index MCP)
    #----------------------------------------
    angle_x = math.atan2((WRIST.y - INDEX_MCP.y), (WRIST.x - INDEX_MCP.x)) * (180 / math.pi)
    angle_x = clamp(angle_x, SHOULDER_RAW_MIN, SHOULDER_RAW_MAX) # ensure raw angles are between 80 (max) and 160 (min)
    shoulder_angle = round(((angle_x - SHOULDER_RAW_MIN) * (SHOULDER_MIN - SHOULDER_MAX)) / (SHOULDER_RAW_MAX - SHOULDER_RAW_MIN) + SHOULDER_MAX)

    #----------------------------------------
    # 3. Elbow Servo Angle (Wrist Y-Position)
    #----------------------------------------
    wrist_y = clamp(WRIST.y, WRIST_Y_MIN, WRIST_Y_MAX) # ensure values are between 0.4 (min) and 0.9 (max)
    elbow_angle = round(((wrist_y - WRIST_Y_MIN) * (ELBOW_MIN - ELBOW_MAX)) / (WRIST_Y_MAX - WRIST_Y_MIN) + ELBOW_MAX)

    #----------------------------------------
    # 4. Wrist Servo Angle (Fixed)
    #----------------------------------------
    WRIST_FIXED = 0
    
    #----------------------------------------
    # 5. Gripper Angle & Palm Size
    #----------------------------------------
    palm_size = math.sqrt((WRIST.x - INDEX_MCP.x)**2 + (WRIST.y - INDEX_MCP.y)**2)
    gripper = GRIPPER_CLOSE if is_fist(hand_landmarks, palm_size) else GRIPPER_OPEN

    return shoulder_angle, elbow_angle, WRIST_FIXED, gripper

def process_frame(frame):
    global no_hand_state
    try:
        # img_resp = urllib.request.urlopen(ESP32_URL, timeout=2)
        # imgnp = np.array(bytearray(img_resp.read()), dtype=np.uint8)
        # frame = cv2.imdecode(imgnp, -1)
        
        frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        
        results = hands.process(frame_rgb)
        if results.multi_hand_landmarks:
            if no_hand_state:
                no_hand_state = False
                print("Hand detected again, resuming normal updates.")
            for hand_landmarks in results.multi_hand_landmarks:
                mp_drawing.draw_landmarks(frame, hand_landmarks, mp_hands.HAND_CONNECTIONS)
                shoulder, elbow, wrist, gripper = classify_hand_position(hand_landmarks)
                send_arm_command(shoulder, elbow, wrist, gripper)
        else:
            if not no_hand_state:
                no_hand_state = True
                print("No hand detected, resetting to home position (once).")
                send_arm_command(HOME_SHOULDER, HOME_ELBOW, HOME_WRIST, HOME_GRIPPER, force_home=True)
            else:
                pass
        
        cv2.putText(frame,
                    f"S:{previous_angles['S']} E:{previous_angles['E']} W:{WRIST_FIXED} G:{gripper if 'gripper' in locals() else HOME_GRIPPER}",
                    (20, 40), cv2.FONT_HERSHEY_SIMPLEX, 1, (0,255,0), 2)
        return frame
    except Exception as e:
        print(f"Error: {e}")
        return None

# Laptop webcam
cap = cv2.VideoCapture(0)

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        print("Failed to capture frame from webcam")
        break
    
    frame = process_frame(frame)
    time.sleep(0.05) # short delay to avoid flooding
    
    if frame is not None:
        # cv2.imshow("Hand Gesture Control - ESP32-CAM", frame)
        cv2.imshow("Hand Gesture Control - Webcam", frame)

    if cv2.waitKey(1) & 0xFF == ord("q"):
        break

cap.release()
cv2.destroyAllWindows()