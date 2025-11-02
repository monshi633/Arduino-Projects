// C++ code
#include <Adafruit_LiquidCrystal.h>

// LED screen
Adafruit_LiquidCrystal lcd_1(0);

// Water level measurement
const int waterLevelPin = A0;
const int waterLevelPin_3 = 13;
const int waterLevelPin_2 = 12;
const int waterLevelPin_1 = 8;
const int redLevelPin = 11;
const int blueLevelPin = 10;
const int greenLevelPin = 9;
int waterLevel = 0;

// Water temperature measurement
const int waterTempPin = A1;
const int redTempPin = 6;
const int blueTempPin = 5;
const int greenTempPin = 3;
int waterTemp = 0;

// Motor
const int motorPin = 2;
const int motorLEDPin = 4;
bool isMotorOn = false;

// Functions
void setRGBColor(int redPin, int redValue, int greenPin, int greenValue, int bluePin, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}

void setMotor(bool motorStatus) {
  if (motorStatus) {
    digitalWrite(motorPin, HIGH);
  } else {
    digitalWrite(motorPin, LOW);
  }
  isMotorOn = motorStatus;
}

void setup()
{
  Serial.begin(9600);

  //LED screen
  lcd_1.begin(16, 2);
  lcd_1.setCursor(0, 0);
  lcd_1.print("Water level:");
  lcd_1.setCursor(0, 1);
  lcd_1.print("Water  temp:");

  // Water level measurement
  pinMode(waterLevelPin, INPUT); // Potentiometer (a.k.a. water level simulator)
  pinMode(waterLevelPin_3, OUTPUT);
  pinMode(waterLevelPin_2, OUTPUT);
  pinMode(waterLevelPin_1, OUTPUT);
  pinMode(redLevelPin, OUTPUT);
  pinMode(blueLevelPin, OUTPUT);
  pinMode(greenLevelPin, OUTPUT);

  // Water temperature measurement
  pinMode(waterTempPin, INPUT);
  pinMode(redTempPin, OUTPUT);
  pinMode(blueTempPin, OUTPUT);
  pinMode(greenTempPin, OUTPUT);

  // Motor
  pinMode(motorPin, OUTPUT);
  pinMode(motorLEDPin, OUTPUT);
}

void loop()
{
  // Flush LED screen values
  lcd_1.setCursor(12, 0);
  lcd_1.print("    ");
  lcd_1.setCursor(12, 1);
  lcd_1.print("    ");
  delay(10);

  // Water level measurement
  waterLevel = map(analogRead(waterLevelPin), 0, 1023, 0, 100); // Convert value to %

  Serial.print("Water level: ");
  Serial.print(waterLevel);

  lcd_1.setCursor(12, 0);
  lcd_1.print(waterLevel);
  lcd_1.setCursor(15, 0);
  lcd_1.print("%");
  delay(10);

  if (waterLevel >= 90) {
    digitalWrite(waterLevelPin_3, HIGH);
    digitalWrite(waterLevelPin_2, HIGH);
    digitalWrite(waterLevelPin_1, HIGH);
    setRGBColor(redLevelPin, 0, greenLevelPin, 0, blueLevelPin, 255);
    setMotor(false); // Turn off water pump
  } else if (waterLevel >= 60) {
    digitalWrite(waterLevelPin_3, LOW);
    digitalWrite(waterLevelPin_2, HIGH);
    digitalWrite(waterLevelPin_1, HIGH);
    setRGBColor(redLevelPin, 0, greenLevelPin, 0, blueLevelPin, 255);
  } else if (waterLevel >= 30) {
    digitalWrite(waterLevelPin_3, LOW);
    digitalWrite(waterLevelPin_2, LOW);
    digitalWrite(waterLevelPin_1, HIGH);
    setRGBColor(redLevelPin, 0, greenLevelPin, 0, blueLevelPin, 255);
  } else if (waterLevel >= 20) {
    digitalWrite(waterLevelPin_3, LOW);
    digitalWrite(waterLevelPin_2, LOW);
    digitalWrite(waterLevelPin_1, LOW); //
    setRGBColor(redLevelPin, 0, greenLevelPin, 0, blueLevelPin, 255);
  } else if (waterLevel >= 10){
    digitalWrite(waterLevelPin_3, LOW);
    digitalWrite(waterLevelPin_2, LOW);
    digitalWrite(waterLevelPin_1, LOW);
    setRGBColor(redLevelPin, 0, greenLevelPin, 0, blueLevelPin, 255);
    setMotor(true); // Turn on water pump
  } else { // Something's wrong with the pump
    digitalWrite(waterLevelPin_3, LOW);
    digitalWrite(waterLevelPin_2, LOW);
    digitalWrite(waterLevelPin_1, LOW);
    setRGBColor(redLevelPin, 255, greenLevelPin, 0, blueLevelPin, 0);
    setMotor(false); // Turn off water pump
  }
  
  if (isMotorOn) {
    digitalWrite(motorLEDPin, HIGH);
    delay(25);
    digitalWrite(motorLEDPin, LOW);
  }

  // Water temperature measurement
  waterTemp = map(((analogRead(waterTempPin) - 20) * 3.04), 0, 1023, -40, 125); // Translate temp to Celsius

  Serial.print("; Water temp: ");
  Serial.print(waterTemp);
  Serial.println(" Celsius");

  lcd_1.setCursor(12, 1);
  lcd_1.print(waterTemp);
  lcd_1.setCursor(15, 1);
  lcd_1.print("C");
  delay(10);

  if (waterTemp < 0) {
    setRGBColor(redTempPin, 0, greenTempPin, 0, blueTempPin, 255); // blue
  } else if (waterTemp < 10) {
    setRGBColor(redTempPin, 0, greenTempPin, 255, blueTempPin, 255); // cyan
  } else if (waterTemp < 20) {
    setRGBColor(redTempPin, 255, greenTempPin, 255, blueTempPin, 255); // white
  } else if (waterTemp < 30) {
    setRGBColor(redTempPin, 255, greenTempPin, 255, blueTempPin, 0); // yellow
  } else if (waterTemp < 50) {
    setRGBColor(redTempPin, 255, greenTempPin, 128, blueTempPin, 0); // orange
  } else if (waterTemp < 70) {
    setRGBColor(redTempPin, 255, greenTempPin, 0, blueTempPin, 0); // red
  } else {
    setRGBColor(redTempPin, 255, greenTempPin, 0, blueTempPin, 255); // purple
  }

  delay(50);
}
