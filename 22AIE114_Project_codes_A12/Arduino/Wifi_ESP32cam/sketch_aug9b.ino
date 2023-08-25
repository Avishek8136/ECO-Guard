#include <Wire.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
// D6 = Rx & D5 = Tx
SoftwareSerial nodemcu(D6, D5);
//intiliazing wifi module to send the telegrm messages
const char* ssid = "Avishek"; // Enter your WIFI SSID
const char* password = "eeeproject"; // Enter your WIFI Password
#define BOTtoken "6598907365:AAHzMOd_MuAUUKnAHqd9JSt8djXO8mOuMw4" // Enter the bottoken you got from botfather
#define CHAT_ID "211246197" // Enter your chatID you got from chatid bot
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
//end of telegram messsnage intialization 
void setup() {
  // Initialize Serial port
  Serial.begin(9600);
  nodemcu.begin(9600);
  //intialize lcd port
  while (!Serial) continue;
  // Serial.begin(115200);
  configTime(0, 0, "pool.ntp.org");
  client.setTrustAnchors(&cert);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  int a = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    a++;
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  bot.sendMessage(CHAT_ID, "Wifi Connected!", "");
  bot.sendMessage(CHAT_ID, "System has Started!!", "");
}
void loop() {
  StaticJsonDocument<1000> jsonBuffer;
  DeserializationError error = deserializeJson(jsonBuffer, nodemcu);
  if (error) {
    Serial.println("Error parsing JSON");
    jsonBuffer.clear();
    return;
  }
  // Check if the received data is an object
  if (!jsonBuffer.is<JsonObject>()) {
    Serial.println("Invalid JSON Object");
    jsonBuffer.clear();
    return;
  }
  JsonObject data = jsonBuffer.as<JsonObject>();
  Serial.println("-----------------------------------------");
  //jsonBuffer.clear();
  Serial.println("Json object Recieved..");
  float distance=data["distance"];
  String a =String (distance);
    Serial.print("Recieved Status  :");
    Serial.println("fire");
    Serial.print("Direction ::");
    Serial.print("status");
      bot.sendMessage(CHAT_ID,a ,"");
  jsonBuffer.clear();
}