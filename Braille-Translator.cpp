/*
  Braille Translator for Arduino
  Translates input strings into Braille patterns and controls digital outputs accordingly.
  
  Hardware Setup:
  - Connect 6 LEDs (or other actuators) to digital pins 2 through 7.
    - Pin 2: Dot 1
    - Pin 3: Dot 2
    - Pin 4: Dot 3
    - Pin 5: Dot 4
    - Pin 6: Dot 5
    - Pin 7: Dot 6
*/
/* UNCOMENT TO USE
#define BRAILLE_SIZE 6

// Define the digital pins for Braille dots
const int braillePins[BRAILLE_SIZE] = {2, 3, 4, 5, 6, 7};

// Braille patterns for lowercase letters a-z
const char *braille_patterns[26] = {
  "100000", "101000", "110000", "110100", "100100", // a-e
  "111000", "111100", "101100", "011000", "011100", // f-j
  "100010", "101010", "110010", "110110", "100110", // k-o
  "111010", "111110", "101110", "011010", "011110", // p-t
  "100011", "101011", "011101", "110011", "110111", // u-y
  "100111"  // z
};

// Braille patterns for numbers 0-9
const char *braille_numbers[10] = {
  "010110", "100000", "101000", "110000", "110100", // 0-4
  "100100", "111000", "111100", "101100", "011000"  // 5-9
};

// Braille patterns for punctuation
const char *braille_space = "000000";
const char *braille_period = "010011";
const char *braille_comma = "010000";
const char *braille_question = "011001";
const char *braille_exclamation = "011010";

// Timing variables
unsigned long startTime;
unsigned long endTime;

// Function to initialize Braille pins
void initializeBraillePins() {
  for (int i = 0; i < BRAILLE_SIZE; i++) {
    pinMode(braillePins[i], OUTPUT);
    digitalWrite(braillePins[i], LOW); // Initialize all dots to OFF
  }
}

// Function to set Braille dots based on pattern
void setBrailleDots(const char *pattern) {
  for (int i = 0; i < BRAILLE_SIZE; i++) {
    if (pattern[i] == '1') {
      digitalWrite(braillePins[i], HIGH);
    } else {
      digitalWrite(braillePins[i], LOW);
    }
  }
}

// Function to get Braille pattern for a character
const char* getBraillePattern(char c) {
  if (c >= 'a' && c <= 'z') {
    return braille_patterns[c - 'a'];
  } else if (c >= '0' && c <= '9') {
    // Braille numbers typically preceded by a number sign, omitted for simplicity
    return braille_numbers[c - '0'];
  } else {
    switch (c) {
      case ' ':
        return braille_space;
      case '.':
        return braille_period;
      case ',':
        return braille_comma;
      case '?':
        return braille_question;
      case '!':
        return braille_exclamation;
      default:
        return braille_space; // Represent unknown characters as space
    }
  }
}

// Function to translate and display Braille
void translateAndDisplay(const char *input) {
  Serial.println("Braille translation:");

  startTime = millis(); // Start timing

  for (int i = 0; i < strlen(input); i++) {
    char c = input[i];
    const char *pattern = getBraillePattern(c);
    
    // Print Braille pattern to Serial
    Serial.print(pattern);
    Serial.print(" ");
    
    // Set Braille dots
    setBrailleDots(pattern);
    
    // Simulate output by printing pin states
    Serial.println();
    for (int j = 0; j < BRAILLE_SIZE; j++) {
      Serial.print("Output ");
      Serial.print(j + 1);
      Serial.print(": ");
      Serial.println(pattern[j] == '1' ? "ON" : "OFF");
    }
    Serial.println();
    
    delay(1000); // Wait for 1 second before next character
  }

  endTime = millis(); // End timing

  // Calculate processing time
  double totalTime = (endTime - startTime);
  double averageTime = totalTime / strlen(input);
  
  Serial.print("Total processing time: ");
  Serial.print(totalTime, 2);
  Serial.println(" ms");
  
  Serial.print("Average time per character: ");
  Serial.print(averageTime, 2);
  Serial.println(" ms");
}

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB
  }
  
  // Initialize Braille pins
  initializeBraillePins();
  
  Serial.println("Braille Translator");
  Serial.println("=================");
  Serial.println("Enter a string (lowercase letters, numbers, and basic punctuation):");
}

void loop() {
  // Check if data is available on Serial
  if (Serial.available() > 0) {
    // Read the incoming string
    String inputString = Serial.readStringUntil('\n');
    
    // Convert to lowercase
    inputString.toLowerCase();
    
    // Convert String to char array
    char input[100];
    inputString.toCharArray(input, 100);
    
    // Print the input
    Serial.print("Processing: ");
    Serial.println(input);
    */