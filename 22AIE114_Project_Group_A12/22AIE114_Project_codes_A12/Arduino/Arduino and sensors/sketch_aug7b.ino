#include <Servo.h>

#define SERVO_PIN 9
#define TRIG_PIN 2
#define ECHO_PIN 3
#define GAS_SENSOR_PIN A0 // Analog input pin for the MQ2 gas sensor
#define LM35_SENSOR_PIN A1 // Analog input pin for the LM35 temperature sensor

Servo servo;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(GAS_SENSOR_PIN, INPUT); // Set the gas sensor pin as input
  pinMode(LM35_SENSOR_PIN, INPUT); // Set the LM35 temperature sensor pin as input
  servo.attach(SERVO_PIN);
  Serial.begin(9600); // Initialize the serial communication
}

void loop() {
  // Call the function to continuously rotate the servo motor
  rotateServo();

  // Call the function to measure and display the distance on the serial plotter
  measureDistanceAndPlot();

  // Call the function to measure and display the gas sensor reading on the serial plotter
  measureGasSensorAndPlot();

  // Call the function to measure and display the LM35 temperature sensor reading on the serial plotter
  measureTemperatureAndPlot();
}

void rotateServo() {
  for (int angle = 0; angle <= 45; angle++) {
    servo.write(angle);
    delay(30); // Adjust the speed of rotation if needed
  }
  for (int angle = 45; angle >= 0; angle--) {
    servo.write(angle);
    delay(30); // Adjust the speed of rotation if needed
  }
}

void measureDistanceAndPlot() {
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

void measureGasSensorAndPlot() {
  int gasSensorValue = analogRead(GAS_SENSOR_PIN); // Read the analog value from the gas sensor

  Serial.print("Gas Sensor Value: ");
  Serial.print(gasSensorValue); // Send the gas sensor value to the serial plotter
  Serial.print("\t");
}

void measureTemperatureAndPlot() {
  int lm35SensorValue = analogRead(LM35_SENSOR_PIN); // Read the analog value from the LM35 temperature sensor
  float voltage = lm35SensorValue * (5.0 / 1023.0); // Convert the analog value to voltage
  float temperatureC = (voltage - 0.5) * 100.0; // Calculate temperature in Celsius

  Serial.print("Temperature: ");
  Serial.print(temperatureC); // Send the temperature value to the serial plotter
  Serial.println(" °C");
}