#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address and dimensions

const byte ROWS = 4;  // Number of rows in the keypad
const byte COLS = 4;  // Number of columns in the keypad

char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 44, 42, 38, 40 };  // Connect keypad ROW0, ROW1, ROW2, ROW3 to these Arduino pins.
byte colPins[COLS] = { 36, 34, 32, 30 };  // Connect keypad COL0, COL1, COL2, COL3 to these Arduino pins.

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Servo servoMotor;

const int servoPin = 6;         // Pin number connected to the servo signal wire
const int doorOpenAngle = 90;   // Angle to open the door
const int doorClosedAngle = 0;  // Angle to close the door

const int pirPin = 9;
const int buzzerPin = 2;
const int ledPin = 13;

const String password = "1234";  // Set your desired password

void setup() {
  lcd.begin();                  // Initialize the LCD
  servoMotor.attach(servoPin);  // Attach the servo to the specified pin
  closeDoor();                  // Close the door initially
  pinMode(pirPin, INPUT);       // Set the PIR pin as input
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  char key = keypad.getKey();  // Read the keypad input

  if (key) {
    lcd.clear();                   // Clear the LCD screen
    lcd.setCursor(0, 0);           // Set the cursor to the first row
    lcd.print("Enter Password:");  // Print "Enter Password:" on the LCD
    lcd.setCursor(0, 1);           // Set the cursor to the second row

    String enteredPassword = "";

    while (enteredPassword.length() < password.length()) {
      if (key) {
        lcd.print("*");  // Display '*' for each entered character
        enteredPassword += key;
      }
      key = keypad.getKey();
      delay(100);
    }

    if (enteredPassword == password) {
      lcd.clear();
      lcd.print("Password Match!");
      openDoor();  // Open the door if the password is correct
      delay(2000);
      lcd.clear();
      bool auth = true;

      while (auth) {
        delay(3000);
        while (digitalRead(pirPin) == HIGH) {
          lcd.clear();
          lcd.print("Person Entered!");
          digitalWrite(ledPin, HIGH);
        
          if (digitalRead(pirPin) == LOW) {
            auth = false;
            digitalWrite(ledPin, LOW);
            lcd.clear();
            lcd.print("Person Out!");
            delay(5000);
            closeDoor();
            break;
          }
        }
      }

    } else {
      lcd.clear();
      lcd.print("Wrong Password!");

      bool guest = true;

      while (guest) {

        delay(3000);

           while (digitalRead(pirPin) == HIGH) {
            lcd.clear();
             lcd.print("Theif Entered!");
            digitalWrite(ledPin, HIGH);
             digitalWrite(buzzerPin, HIGH);
            delay(3000);

          if (digitalRead(pirPin) == LOW) {
            guest = false;
            digitalWrite(ledPin, LOW);
            digitalWrite(buzzerPin, LOW);
            lcd.clear();
            lcd.print("Theif Out!");
            delay(2000);
            break;
          }
        }

          lcd.clear();
            lcd.print("Outer!");

        // if (digitalRead(pirPin) == HIGH) {
        //   lcd.clear();
        //   lcd.print("Thief Detected!");
        //   delay(1000);
        //   lcd.clear();
        //   digitalWrite(ledPin, HIGH);
        //   lcd.print("LED ON");
        //   delay(1000);
        //   lcd.clear();
        //   digitalWrite(buzzerPin, HIGH);
        //   lcd.print("Buzzer ON");
        //   delay(1000);
        //   lcd.clear();
        // } else {
        //   bool guest = false;
        //   lcd.clear();
        //   digitalWrite(ledPin, LOW);
        //   lcd.print("LED Off");
        //   delay(1000);
        //   lcd.clear();
        //   digitalWrite(buzzerPin, LOW);
        //   lcd.print("Buzzer Off");
        //   delay(1000);
        //   lcd.clear();
        //   break;
        // }
      }

                lcd.clear();
            lcd.print("Outer ee!");
    }
  }
}

void openDoor() {
  servoMotor.write(doorOpenAngle);
}

void closeDoor() {
  servoMotor.write(doorClosedAngle);
}
