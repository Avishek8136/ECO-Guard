#include <Servo.h>
#include <ArduinoJson.h>

Servo myservo;

#define SERVO_PIN 11
#define TRIG_PIN 2
#define ECHO_PIN 3
#define GAS_SENSOR_PIN A0 // Analog input pin for the MQ2 gas sensor
#define LM35_SENSOR_PIN A1 // Analog input pin for the LM35 temperature sensor

int pos = 0;
boolean fire = false;
int sign = 0;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(GAS_SENSOR_PIN, INPUT);
  pinMode(LM35_SENSOR_PIN, INPUT);
  servo.attach(SERVO_PIN);
  Serial.begin(9600); // Initialize the serial communication with the PC
  Serial1.begin(9600); // Initialize the serial communication with ESP8266
  delay(1000);
  Serial.println("Program started");
}

void loop() {
  // Call the function to continuously rotate the servo motor
  rotateServo();

  // Read sensor data
  int distance = measureDistance();
  int gasSensorValue = analogRead(GAS_SENSOR_PIN);
  float temperatureC = measureTemperature();

  // Send sensor data to ESP8266
  transferData(distance, gasSensorValue, temperatureC);

  // Call the function to measure and display the distance on the serial plotter
  measureDistanceAndPlot();

  // Call the function to measure and display the gas sensor reading on the serial plotter
  measureGasSensorAndPlot();

  // Call the function to measure and display the LM35 temperature sensor reading on the serial plotter
  measureTemperatureAndPlot();

  delay(1000); // Adjust delay as needed
}

void rotateServo() {
  for (int angle = 0; angle <= 90; angle++) {
    myservo.write(angle);
    delay(15); // Adjust the speed of rotation if needed
  }
  for (int angle = 90; angle >= 0; angle--) {
    myservo.write(angle);
    delay(15); // Adjust the speed of rotation if needed
  }
}

int measureDistance() {
  // Measure and return distance
  // ...
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2; // Convert the duration to centimeters (Speed of sound in air is approximately 34 cm/ms)

  Serial.print("Distance: ");
  Serial.print(distance); // Send the distance value to the serial plotter
  Serial.print(" cm \t");

}

float measureTemperature() {
  // Measure and return temperature
  // ...
}

void transferData(int distance, int gasSensorValue, float temperatureC) {
  StaticJsonDocument<200> jsonBuffer;
  JsonObject data = jsonBuffer.to<JsonObject>();
  data["distance"] = distance;
  data["gasValue"] = gasSensorValue;
  data["temperature"] = temperatureC;
  
  Serial1.print("Data:"); // Prefix to identify the data
  serializeJson(data, Serial1); // Send JSON data to ESP8266
  Serial1.println(); // End the data transmission
  jsonBuffer.clear();
}

// The rest of your existing code for measuring and plotting sensor values goes here...
