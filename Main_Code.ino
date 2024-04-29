#include <avr/io.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// LCD Addresses
LiquidCrystal_I2C lcd1(0x27, 16, 2);
LiquidCrystal_I2C lcd2(0x26, 16, 2);

// Sensor Pins
const int CANSensorPin = A0;
const int PETSensorPin = 2;
const int PRESENCESensorPin = 3;
// Pin 4 and 5 for Weight Sensor

// Threshold Level
const int induct_level = 1018;

// Button Pins
const int water_button = 33;
const int wifi_buttons = 37;
const int reset_button = 39;

// Reward Pins
const int wifi_rewards = 7;
const int water_reward = 5;

// Servo
Servo accepts_servo;
Servo returns_servo;
Servo entrance_servo;
Servo segregate_servo;

void setup() {
  // Sensor Setup
  pinMode(PETSensorPin, INPUT);
  pinMode(PRESENCESensorPin, INPUT);

  // Button Setup
  pinMode(water_button, INPUT_PULLUP);
  pinMode(wifi_buttons, INPUT_PULLUP);
  pinMode(reset_button, INPUT_PULLUP);

  // Reward Pin
  pinMode(wifi_rewards, OUTPUT);
  pinMode(water_reward, OUTPUT);

  // Servo Setup
  accepts_servo.attach(10);
  returns_servo.attach(11);
  entrance_servo.attach(12);
  segregate_servo.attach(13);

  Serial.begin(9600);

  // LCD Setup
  lcd1.init();
  lcd1.backlight();
  lcd1.setCursor(0, 0);
  lcd1.print("LCD 1");

  lcd2.init();
  lcd2.backlight();
  lcd2.setCursor(0, 0);
  lcd2.print("LCD 2");

  delay(2000);

  lcd1.clear();
  lcd1.setCursor(0, 0);
  lcd1.print(" WiCycle  (RVM) ");
  lcd1.setCursor(0, 1);
  lcd1.print("  by Group - 7  ");

  lcd2.clear();
  lcd2.setCursor(0, 0);
  lcd2.print("     System     ");
  lcd2.setCursor(0, 1);
  lcd2.print("    Starting    ");

  delay(5000);

  accepts_servo.write(0);
  returns_servo.write(180);
  entrance_servo.write(0);
  segregate_servo.write(90);

  Serial.println("System Started");
}

void resetFunc() {
  asm volatile ("  jmp 0");
}

void wifi_as_reward() {
  entrance_servo.write(58);
  delay(200);

  int plasticSensorValue = digitalRead(PETSensorPin);
  int inductiveSensorValue = analogRead(CANSensorPin);
  int presenceSensorValue = digitalRead(PRESENCESensorPin);

  Serial.println(inductiveSensorValue);

  if(presenceSensorValue == LOW) {
    
    delay(1000);
    entrance_servo.write(0);
    delay(1000);

    // Check if plastic is detected
    if (plasticSensorValue == HIGH) {
      Serial.println("PET Detected!");

      lcd2.clear();
      lcd2.setCursor(0, 0);
      lcd2.print("PET Detected!");

      accepts_servo.write(75);
      segregate_servo.write(40);
      delay(2000);
      accepts_servo.write(0);
      segregate_servo.write(90);

      digitalWrite(wifi_rewards, HIGH);
      delay(50);
      digitalWrite(wifi_rewards, LOW);

    } else if (inductiveSensorValue < induct_level && plasticSensorValue == LOW) {
      Serial.println("Can Detected!");

      lcd2.clear();
      lcd2.setCursor(0, 0);
      lcd2.print("Can Detected!");

      accepts_servo.write(75);
      segregate_servo.write(140);
      delay(2000);
      accepts_servo.write(0);
      segregate_servo.write(90);

      digitalWrite(wifi_rewards, HIGH);
      delay(10);
      digitalWrite(wifi_rewards, LOW);
      delay(10);
      digitalWrite(wifi_rewards, HIGH);
      delay(10);
      digitalWrite(wifi_rewards, LOW);

    } else if (inductiveSensorValue > induct_level && plasticSensorValue == LOW) {
      Serial.println("Invalid Input!");

      lcd2.clear();
      lcd2.setCursor(0, 0);
      lcd2.print("Invalid Input!");

      returns_servo.write(100);
      delay(2000);
      returns_servo.write(180);

    }
  } else {
    Serial.println("No Object!");
  
    lcd2.clear();
    lcd2.setCursor(0, 0);
    lcd2.print("No Object!");

  }
}

void water_as_reward() {
  entrance_servo.write(58);
  delay(200);
  
  int plasticSensorValue = digitalRead(PETSensorPin);
  int inductiveSensorValue = analogRead(CANSensorPin);
  int presenceSensorValue = digitalRead(PRESENCESensorPin);

  Serial.println(inductiveSensorValue);

  if(presenceSensorValue == LOW) {
    
    delay(1000);
    entrance_servo.write(0);
    delay(1000);

    // Check if plastic is detected
    if (plasticSensorValue == HIGH) {
      Serial.println("PET Detected!");

      lcd2.clear();
      lcd2.setCursor(0, 0);
      lcd2.print("PET Detected!");

      accepts_servo.write(75);
      segregate_servo.write(40);
      delay(2000);
      digitalWrite(water_reward, HIGH);
      delay(100);
      digitalWrite(water_reward, LOW);
      accepts_servo.write(0);
      segregate_servo.write(90);

    } else if (inductiveSensorValue < induct_level && plasticSensorValue == LOW) {
      Serial.println("Can Detected!");

      lcd2.clear();
      lcd2.setCursor(0, 0);
      lcd2.print("Can Detected!");

      accepts_servo.write(75);
      segregate_servo.write(140);
      delay(2000);
      digitalWrite(water_reward, HIGH);
      delay(100);
      digitalWrite(water_reward, LOW);
      delay(100);
      digitalWrite(water_reward, HIGH);
      delay(100);
      digitalWrite(water_reward, LOW);
      accepts_servo.write(0);
      segregate_servo.write(90);

    } else if (inductiveSensorValue > induct_level && plasticSensorValue == LOW) {
      Serial.println("Invalid Input!");

      lcd2.clear();
      lcd2.setCursor(0, 0);
      lcd2.print("Invalid Input!");

      returns_servo.write(100);
      delay(2000);
      returns_servo.write(180);
    }
  } else {
    Serial.println("No Object!");

    entrance_servo.write(58);
    delay(500);
    
    lcd2.clear();
    lcd2.setCursor(0, 0);
    lcd2.print("No Object!");

  }
}

void loop() {

  lcd1.clear();
  lcd1.setCursor(0, 0);
  lcd1.print("  WiCycle  RVM  ");
  lcd1.setCursor(0, 1);
  lcd1.print("   by Group 7   ");

  lcd2.clear();
  lcd2.setCursor(0, 0);
  lcd2.print("Select a Reward:");
  lcd2.setCursor(0, 1);
  lcd2.print("WiFi Water Reset");

  if (digitalRead(wifi_buttons) == LOW) {
    Serial.println("WIFI as Reward!");
    delay(500);

    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("  Your Reward:  ");
    lcd1.setCursor(0, 1);
    lcd1.print("WIFI Connection!");

    while (digitalRead(reset_button) != LOW) {   // Run indefinitely until reset option is selected
      entrance_servo.write(58);
      delay(500);
      wifi_as_reward();                          // Call the function to detect plastic or can
      delay(1000);
    }

    Serial.println("Resetting...");
    delay(1000);
    resetFunc();
  } 
  
  else if (digitalRead(water_button) == LOW) {
    Serial.println("Water as Reward!");
    delay(500);

    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("  Your Reward:  ");
    lcd1.setCursor(0, 1);
    lcd1.print(" Drinking Water ");

    while (digitalRead(reset_button) != LOW) {   // Run indefinitely until reset option is selected
      water_as_reward();                      // Call the function to detect plastic or can
      delay(1000);
    }

    Serial.println("Resetting...");
    delay(1000);
    resetFunc();
  } 

  else if (digitalRead(reset_button) == LOW) {
    Serial.println("Resetting...");

    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("  Your Reward:  ");
    lcd1.setCursor(0, 1);
    lcd1.print("  Resetting...  ");

    delay(1000);
    resetFunc(); // Call the reset function
  }
}
