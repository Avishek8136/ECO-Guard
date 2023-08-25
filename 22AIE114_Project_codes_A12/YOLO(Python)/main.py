import io
import cv2
import telebot
import requests

# TODO: 1.1 Get your environment variables
chat_id='5922863020'
bot_id = "6598907365:AAHzMOd_MuAUUKnAHqd9JSt8djXO8mOuMw4"
channel_id="@amrita_eco"
url = f"https://api.telegram.org/bot{bot_id}/sendPhoto"
bot = telebot.TeleBot(bot_id)
start_message = "Hello, I'm your bot and I'm starting up!"
bot.send_message(chat_id, start_message)

# Custom categories
category_map = {
    'beasts': ['bear', 'elephant', 'zebra', 'giraffe','dog', 'cat','cow','sheep'],
    'birds': ['bird'],
    'person': ['person'],
    'vehicle': ['bicycle','car','motorcycle','bus','truck','train','boat'],
    'signs':['street sign','stop sign'],
    'cutting_objects':['knife','fork'],
    'danger': ['snake', 'spider']
}

# Function to map the COCO class to its category
def map_to_category(class_name):
    for category, classes in category_map.items():
        if class_name in classes:
            return category
    return 'other'  # Return 'other' if the class doesn't belong to any custom category

# Load the COCO class names
classNames = []
classFile = 'coco.names'
with open(classFile, 'rt') as f:
    classNames = f.read().rstrip('\n').split('\n')

configPath = 'ssd_mobilenet_v3_large_coco_2020_01_14.pbtxt'
weightsPath = 'frozen_inference_graph.pb'

net = cv2.dnn_DetectionModel(weightsPath, configPath)
net.setInputSize(320, 320)
net.setInputScale(1.0/127.5)
net.setInputMean((127.5, 127.5, 127.5))
net.setInputSwapRB(True)
print("Press backspace or ESC to close the cam")
# Open the camera
cap = cv2.VideoCapture(0)  # Change the argument to the camera index if you have multiple cameras

while True:
    ret, img = cap.read()  # Read a frame from the camera


    classIds, confs, bbox = net.detect(img, confThreshold=0.5)

    if len(classIds) != 0:
        for classId, confidence, box in zip(classIds.flatten(), confs.flatten(), bbox):
            class_name = classNames[classId-1]
            category = map_to_category(class_name)
            # Set color based on category
            if (category == 'beasts')or(category=='danger')or(category=='cutting_objects'):
                color = (0,0 ,255)  # red (for beasts & dangerous things)
                if(category=='cutting_objects'):
                    message="Alert! Sharp objects detected Detected"
                    bot.send_message(chat_id, message)
                else:
                    message="Alert! Beast Detected"
                    bot.send_message(chat_id, message)
                image_bytes = cv2.imencode(".jpg", img)[1].tobytes()

                # Wrap the byte array in a file-like object
                with io.BytesIO(image_bytes) as f:
                    # Set the name attribute of the file-like object
                    f.name = "image.jpg"

                    # Create an InputFile object
                    media_photo = telebot.types.InputFile(f)

                    # Send the photo to the user
                    bot.send_photo(chat_id, media_photo)
                    data = {
                        "chat_id": channel_id,
                        "text": message
                    }
                    cv2.imwrite("image.jpg", img)
                    media_photo = {"photo": ("image.jpg", open("image.jpg", "rb"))}
                    response = requests.post(url, data=data,files=media_photo)
            elif category == 'birds':
                color = (0, 255, 0)    #  (for birds)
            elif category == 'person':
                color = (0, 255, 255)    # Yellow (for persons)
            elif (category == 'vehicle') or (category=="signs"):
                color = (255, 0, 0)      # Blue (for vehicle & signs)
            else:
                color = (255, 255,255)    # White (for other objects)

            cv2.rectangle(img, box, color=color, thickness=3)
            cv2.putText(img, category, (box[0]+10, box[1]+30), cv2.FONT_HERSHEY_COMPLEX, 1, color, 2)

    cv2.imshow('Object Detection', img)  # Show the image

    # Wait for 'ESC' or 'Backspace" key to be pressed to end the program
    tecla = cv2.waitKey(5) & 0xFF
    if ((tecla == 27) or (tecla==8)):
        break

cap.release()  # Release the camera
cv2.destroyAllWindows()
bot.send_message(chat_id, "Bye Bye Shutting down")
bot.polling()