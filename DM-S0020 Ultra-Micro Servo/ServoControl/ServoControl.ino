/*
This file needs to be in a folder with the same name as the file.
I made a folder named ServoControl and placed this file in it.
Arduino IDE should create this folder structure when you save a new sketch.
*/

#include <Servo.h>

// Define servo pins - ADJUST TO WHATEVER PINS WE USE AND WHAT BRAILLE BIT IT IS
#define SERVO1_PIN 9
#define SERVO2_PIN 10 
#define SERVO3_PIN 11
#define SERVO4_PIN 12

// Create servo objects
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

// Servo limits and parameters
const int MIN_PULSE_WIDTH = 500;   // CHECK LATER: VERIFY THIS VALUE WITH SERVO SPECIFICATIONS
const int MAX_PULSE_WIDTH = 2400;  // CHECK LATER: VERIFY THIS VALUE WITH SERVO SPECIFICATIONS
const int MIN_ANGLE = 0;           // Minimum angle in degrees
const int MAX_ANGLE = 180;         // Maximum angle in degrees (THIS MIGHT BE TOO MUCH)

// Braille-specific angles
const int DOT_DOWN = 0;            // Represents 0 in Braille (pin down)
const int DOT_UP = 90;             // Represents 1 in Braille (pin up)
const int MOVEMENT_DELAY = 15;     // Delay for smooth movement in milliseconds

// Function prototypes
void centerAllServos();
void resetAllPins();
void setDot(int dotNumber, bool state);
void smoothMove(Servo &servo, int startAngle, int endAngle);
void setAllServos(int angle);
void demoPattern();
void displayBraillePattern(uint8_t pattern);
void processSerialCommand(String command);

void setup() {
  // Initialize Serial for debugging
  Serial.begin(9600);
  // CHECK LATER: Ensure the baud rate matches the Serial Monitor settings

  // Attach servos to pins with specified pulse widths
  servo1.attach(SERVO1_PIN, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  servo2.attach(SERVO2_PIN, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  servo3.attach(SERVO3_PIN, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  servo4.attach(SERVO4_PIN, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);

  // Center all servos on startup
  centerAllServos();
}

// Function to center all servos
void centerAllServos() {
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  servo4.write(90);
  delay(500); // WAIT FOR SERVOS TO MOVE TO CENTER POSITION
}

// Reset all pins to down position (0)
void resetAllPins() {
  setDot(1, false);
  setDot(2, false);
  setDot(3, false);
  setDot(4, false);
  delay(500); // WAIT FOR SERVOS TO MOVE TO DOWN POSITION
}

// Set a specific dot up (true) or down (false)
void setDot(int dotNumber, bool state) {
  int angle = state ? DOT_UP : DOT_DOWN;

  switch(dotNumber) {
    case 1:
      smoothMove(servo1, servo1.read(), angle);
      break;
    case 2:
      smoothMove(servo2, servo2.read(), angle);
      break;
    case 3:
      smoothMove(servo3, servo3.read(), angle);
      break;
    case 4:
      smoothMove(servo4, servo4.read(), angle);
      break;
    default:
      Serial.println("INVALID DOT NUMBER");
      break;
  }
}

// Smooth movement function for better pin control
void smoothMove(Servo &servo, int startAngle, int endAngle) {
  int step = (startAngle < endAngle) ? 1 : -1;

  for (int angle = startAngle; angle != endAngle; angle += step) {
    servo.write(angle);
    delay(MOVEMENT_DELAY);
    // CHECK LATER: Consider using non-blocking delays for better performance
  }
  servo.write(endAngle); // Ensure the final position is set
}

// Function to move all servos to a specific angle
void setAllServos(int angle) {
  angle = constrain(angle, MIN_ANGLE, MAX_ANGLE);
  servo1.write(angle);
  servo2.write(angle);
  servo3.write(angle);
  servo4.write(angle);
  // CHECK LATER: Ensure moving all servos simultaneously does not cause power issues
}

// Example implementation of demoPattern
void demoPattern() {
  // Define a sequence of servo movements for the demo
  setDot(1, true);
  setDot(2, false);
  setDot(3, true);
  setDot(4, false);
  delay(1000); // WAIT FOR 1 SECOND
  resetAllPins();
  delay(1000); // WAIT FOR 1 SECOND
}

// Example implementation of displayBraillePattern
void displayBraillePattern(uint8_t pattern) {
  for (int i = 0; i < 4; i++) {
    bool state = bitRead(pattern, i);
    setDot(i + 1, state);
  }
  // CHECK LATER: Validate that the pattern correctly represents Braille characters
}

// Function to process serial commands
void processSerialCommand(String command) {
  command.trim(); // REMOVE ANY LEADING/TRAILING WHITESPACE

  if (command.length() >= 3 && command.charAt(1) == ':') {
    // Format: pin:state (e.g., "1:1" sets pin 1 to up position)
    int pin = command.charAt(0) - '0';
    int state = command.charAt(2) - '0';
    if (pin >= 1 && pin <= 4 && (state == 0 || state == 1)) {
      setDot(pin, state == 1);
    } else {
      Serial.println("INVALID COMMAND FORMAT");
    }
  } else if (command.length() == 1) {
    // Handle single-character commands
    switch (command.charAt(0)) {
      case 'c': // Center all servos
        centerAllServos();
        break;
      case 'd': // Run demo pattern
        demoPattern();
        break;
      case 'r': // Reset all pins
        resetAllPins();
        break;
      case '0': // Set all pins down
        displayBraillePattern(0b0000);
        break;
      case '1': // Set all pins up
        displayBraillePattern(0b1111);
        break;
      default:
        Serial.println("UNKNOWN COMMAND");
        break;
    }
  } else {
    Serial.println("UNRECOGNIZED COMMAND");
  }
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    processSerialCommand(command);
    // CHECK LATER: Ensure that commands are terminated properly to prevent buffer issues
  }

  // ADDITIONAL FUNCTIONALITY CAN BE ADDED HERE IF NEEDED
}