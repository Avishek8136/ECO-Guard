import streamlit as st
import cv2
import telebot
import requests
import numpy as np
import io

# Initialize Telegram bot
bot_id = st.secrets["bot_id"]
chat_id = st.secrets["chat_id"]
channel_id = st.secrets["channel_id"]
url = f"https://api.telegram.org/bot{bot_id}/sendPhoto"
bot = telebot.TeleBot(bot_id)

# Helper function to send photos to Telegram
def sendbot(img, message):
    image_bytes = cv2.imencode(".jpg", img)[1].tobytes()
    with io.BytesIO(image_bytes) as f:
        f.name = "image.jpg"
        media_photo = telebot.types.InputFile(f)
        bot.send_photo(chat_id, media_photo)
        data = {"chat_id": channel_id, "text": message}
        requests.post(url, data=data, files={"photo": f})

# Load the COCO class names
classNames = []
classFile = 'coco.names'
with open(classFile, 'rt') as f:
    classNames = f.read().rstrip('\n').split('\n')

# Load model configuration and weights
configPath = 'ssd_mobilenet_v3_large_coco_2020_01_14.pbtxt'
weightsPath = 'frozen_inference_graph.pb'
net = cv2.dnn_DetectionModel(weightsPath, configPath)
net.setInputSize(320, 320)
net.setInputScale(1.0 / 127.5)
net.setInputMean((127.5, 127.5, 127.5))
net.setInputSwapRB(True)

# Streamlit UI and camera handling
st.title("Real-time Object Detection and Alerts")
st.write("Capture an image using the camera below:")

# Capture image from the browser
image_data = st.camera_input("Camera")

if image_data is not None:
    # Convert the image from Streamlit format to OpenCV format
    file_bytes = np.asarray(bytearray(image_data.read()), dtype=np.uint8)
    img = cv2.imdecode(file_bytes, 1)
    
    # Perform object detection
    classIds, confs, bbox = net.detect(img, confThreshold=0.5)
    
    # Detect and categorize objects
    for classId, confidence, box in zip(classIds.flatten(), confs.flatten(), bbox):
        class_name = classNames[classId - 1]
        # Customize colors and labels based on the category
        color = (0, 255, 0) if class_name == "person" else (0, 0, 255)  # Green for persons, red for others
        cv2.rectangle(img, box, color=color, thickness=3)
        cv2.putText(img, class_name, (box[0] + 10, box[1] + 30), cv2.FONT_HERSHEY_SIMPLEX, 1, color, 2)

    # Display detection results in Streamlit
    st.image(cv2.cvtColor(img, cv2.COLOR_BGR2RGB), caption="Detected Objects", use_column_width=True)

    # Send alert to Telegram if specific objects are detected
    if "person" in [classNames[id - 1] for id in classIds.flatten()]:
        sendbot(img, "Alert! Person Detected")
