#include <Wire.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// D6 = Rx & D5 = Tx
SoftwareSerial nodemcu(D6, D5);

const char* ssid = "Avishek";
const char* password = "eeeproject";
#define BOTtoken "6598907365:AAHzMOd_MuAUUKnAHqd9JSt8djXO8mOuMw4"
#define CHAT_ID "211246197"

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

void setup() {
  Serial.begin(9600);
  nodemcu.begin(9600);

  while (!Serial) continue;

  configTime(0, 0, "pool.ntp.org");
  client.setTrustAnchors(&cert);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  bot.sendMessage(CHAT_ID, "Wifi Connected!", "");
  bot.sendMessage(CHAT_ID, "System has Started!!", "");
}

int animalCount = 0;
String animalType;

void loop() {
  StaticJsonDocument<1000> jsonBuffer;
  DeserializationError error = deserializeJson(jsonBuffer, nodemcu);
  
  if (error) {
    Serial.println("Error parsing JSON");
    jsonBuffer.clear();
    return;
  }

  if (!jsonBuffer.is<JsonObject>()) {
    Serial.println("Invalid JSON Object");
    jsonBuffer.clear();
    return;
  }

  JsonObject data = jsonBuffer.as<JsonObject>();
  Serial.println("-----------------------------------------");
  Serial.println("Json object Received..");
  int count = data["count"];
  String type = data["type"];

  if (count > 0) {
    Serial.print("Received Animal Count: ");
    Serial.println(count);
    Serial.print("Animal Type: ");
    Serial.println(type);

    animalCount = count;
    animalType = type;
    
    String message = "Wildlife Detected!\n";
    String countMessage = "Number of Animals Detected: " + String(animalCount) + "\n";
    String typeMessage = "Animal Type: " + animalType + "\n";
    String finalMessage = message + countMessage + typeMessage;

    bot.sendMessage(CHAT_ID, finalMessage, "");
  } else {
    Serial.println("No wildlife detected.");
    animalCount = 0;
    animalType = "";
  }

  jsonBuffer.clear();
}
