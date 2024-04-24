/*
 * Haptic Feedback Armband Controller
 *
 * Authors: Charles Benson and Kameron Young-Tillman
 * Date: 2024
 *
 * This code controls a haptic feedback armband by reading tactile sensor data from a matrix,
 * matching patterns to identify objects, and delivering vibrotactile and pneumatic feedback.
 *
 * Features:
 * - Read sensor matrix values
 * - Calibrate object patterns
 * - Match patterns and identify objects
 * - Control vibration motors based on matched patterns
 * - Calculate average pressure and control pump motor
 * - Trigger vibration hum based on column pressures
 *
 * The controller uses configurable sensor matrix dimensions and supports multiple objects.
 * It continuously samples the matrix, matches patterns, and triggers haptic feedback accordingly.
 */

#define numRows 3                      // num of rows in the sensor matrix
#define numCols 3                      // num of columns in the sensor matrix
#define sensorPoints numRows* numCols  // num of sensor points
#define adjPressureThreshold 0.5       // pressure threshold for determining node pressed
#define VIB1_PIN 9                     // first vibration motor
#define VIB2_PIN 10                    //  second vibration motor
#define VIB3_PIN 3                     //  third vibration motor
#define motorPin 12                    //  main motor pump
#define motorPwmPin 11                 // pwm pin for motor speed control
#define numObjects 3                   // num of calibratable objects

int rows[] = { A0, A1, A2 };         // analog pins array for sensor rows
int cols[] = { 5, 6, 7 };            // digital pins array for sensor columns
int sensorValues[numRows][numCols];  // store sensor readings
int restingValues[numRows][numCols] = {
  { 500, 500, 500 },
  { 500, 500, 500 },
  { 500, 500, 500 }
};  // default resting values for sensors
int calibrationValues[numObjects][numRows][numCols] = {
  { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } },
  { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } },
  { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }
};                     // store calibration data for each object
int pressedCount = 0;  // counter for pressed nodes

void setup() {
  Serial.begin(9600);  //start serial communication 9600 bps

  // initialise row pins as input with pull-up resistors
  for (int i = 0; i < numRows; i++) {
    pinMode(rows[i], INPUT_PULLUP);
  }
  // initialise column pins as input
  for (int i = 0; i < numCols; i++) {
    pinMode(cols[i], INPUT);
  }

  // initialise vibration motor pins as output
  pinMode(VIB1_PIN, OUTPUT);
  pinMode(VIB2_PIN, OUTPUT);
  pinMode(VIB3_PIN, OUTPUT);

  // initialise motor pins as output
  pinMode(motorPin, OUTPUT);
  pinMode(motorPwmPin, OUTPUT);

  // calibrate patterns for each object
  for (int i = 0; i < numObjects; i++) {
    calibratePattern(i);
  }
}

void loop() {
  sampleMatrix();  // sample the sensor matrix
  delay(100);      // delay to reduce sampling rate
}

void sampleMatrix() {
  pressedCount = 0;  // reset pressed count at each loop iteration

  const int numReadings = 5;  // define number of readings to average from

  // scan each column by setting it low and reading the rows
  for (int colCount = 0; colCount < numCols; colCount++) {
    pinMode(cols[colCount], OUTPUT);
    digitalWrite(cols[colCount], LOW);

    // read each row in the current column
    for (int rowCount = 0; rowCount < numRows; rowCount++) {
      int sum = 0;
      // take multiple readings and average them
      for (int i = 0; i < numReadings; i++) {
        sum += analogRead(rows[rowCount]);
      }
      sensorValues[rowCount][colCount] = sum / numReadings;  // store averaged sensor reading
    }

    pinMode(cols[colCount], INPUT);  // reset column pin to input
  }

  int matchedObject = -1;    // variable to store the index of a matched object pattern
  bool nodePressed = false;  // flag to indicate if any node was pressed
  int tolerance = 500;       // tolerance value for matching sensor reading to calibration

  // check each object's pattern against sensor readings
  for (int obj = 0; obj < numObjects; obj++) {
    bool patternMatched = true;
    for (int i = 0; i < numRows; i++) {
      for (int j = 0; j < numCols; j++) {
        if (isPressed(i, j)) {  // check if node is pressed
          nodePressed = true;
          // check if reading is within the tolerance range
          if (sensorValues[i][j] < calibrationValues[obj][i][j] - tolerance || sensorValues[i][j] > calibrationValues[obj][i][j] + tolerance) {
            patternMatched = false;  // set pattern match flag to false
            break;
          }
        }
      }
      if (!patternMatched) break;  // break outer loop if pattern doesn't match
    }
    if (patternMatched && nodePressed) {  // check if any node was pressed and pattern matched
      matchedObject = obj;                // store the matched object index
      break;                              // exit loop after finding a match
    }
  }

  if (matchedObject != -1) {
    // activate vibration pattern based on matched object
    switch (matchedObject) {
      case 0:  // first object pattern
        Serial.println("PATTERN 1");
        // wave pattern
        for (int i = 0; i < 3; i++) {
          analogWrite(VIB1_PIN, 200);
          delay(200);
          analogWrite(VIB1_PIN, 0);
          analogWrite(VIB2_PIN, 200);
          delay(200);
          analogWrite(VIB2_PIN, 0);
          analogWrite(VIB3_PIN, 200);
          delay(200);
          analogWrite(VIB3_PIN, 0);
        }
        break;

      case 1:  // second object pattern
        Serial.println("PATTERN 2");
        // pulse pattern
        for (int i = 0; i < 3; i++) {
          analogWrite(VIB1_PIN, 200);
          analogWrite(VIB2_PIN, 200);
          analogWrite(VIB3_PIN, 200);
          delay(500);
          analogWrite(VIB1_PIN, 0);
          analogWrite(VIB2_PIN, 0);
          analogWrite(VIB3_PIN, 0);
          delay(500);
        }
        break;

      case 2:  // third object pattern
        Serial.println("PATTERN 3");
        // dotted pattern
        for (int i = 0; i < 5; i++) {
          analogWrite(VIB1_PIN, 200);
          delay(100);
          analogWrite(VIB1_PIN, 0);
          analogWrite(VIB2_PIN, 200);
          delay(100);
          analogWrite(VIB2_PIN, 0);
          analogWrite(VIB3_PIN, 200);
          delay(100);
          analogWrite(VIB3_PIN, 0);
          delay(200);
        }
        break;
    }
  }

  int columnPressures[numCols] = { 0 };  // array to store the pressure in each column
  int totalPressure = 0;                 // variable to sum pressures

  // calculate total pressure and count pressed nodes
  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numCols; j++) {
      if (isPressed(i, j)) {
        totalPressure += sensorValues[i][j];       // add up pressures
        pressedCount++;                            // increment count of pressed nodes
        columnPressures[j] += sensorValues[i][j];  // add pressure to the corresponding column
      }
    }
  }

  int avgPressure = 0;                           // variable to store average pressure
  if (pressedCount >= 2) {                       // calculate average if at least two nodes are pressed
    avgPressure = totalPressure / pressedCount;  // compute average pressure
    // constrain avgpressure to the range 30-100, evade unexpected spikes
    avgPressure = constrain(avgPressure, 30, 200);
    int pumpPwmValue = map(avgPressure, 30, 200, 150, 10);  // map pressure to pwm value
    analogWrite(motorPwmPin, pumpPwmValue);                 // set pwm for motor
    digitalWrite(motorPin, HIGH);                           // turn motor on

    Serial.print("PumpValue" + String(pumpPwmValue) + "PumpValue");  // print pwm value
    delay(100);
  } else {
    digitalWrite(motorPin, LOW);  // turn motor off if less than two nodes are pressed
  }

  // trigger vibration hum based on column pressures
  for (int colCount = 0; colCount < numCols; colCount++) {
    int avgColumnPressure = columnPressures[colCount] / numRows;  // calculate average pressure in the column

    // map the pressure to PWM range 30-120, when pressure reading betw rest and 200
    int vibrationPwm = map(avgColumnPressure, restingValues[0][colCount], 200, 30, 120);
    vibrationPwm = constrain(vibrationPwm, 30, 120);  // constrain the PWM value

    // set the PWM value for the corresponding vibration motor
    if (colCount == 0) {
      analogWrite(VIB1_PIN, vibrationPwm);
    } else if (colCount == 1) {
      analogWrite(VIB2_PIN, vibrationPwm);
    } else if (colCount == 2) {
      analogWrite(VIB3_PIN, vibrationPwm);
    }
  }

  // print the sensor values and average pressure
  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numCols; j++) {
      Serial.print(sensorValues[i][j]);
      Serial.print("\t");
    }
  }
  Serial.print("\nAverage Pressure: ");
  Serial.println(avgPressure);
  Serial.println();
}

bool isPressed(int row, int col) {
  // return true if sensor value is less than adjusted threshold
  if (pressedCount > 0) {
    return sensorValues[row][col] <= restingValues[row][col] * adjPressureThreshold;
  } else {
    return sensorValues[row][col] < restingValues[row][col];
  }
}

void calibratePattern(int objectIndex) {
  // prompt user to place object and press 'c' to calibrate
  Serial.print("Place object ");
  Serial.print(objectIndex + 1);
  Serial.println(" on the matrix and press 'c' to calibrate.");

  while (Serial.read() != 'c') {
    // wait for user to press 'c'
  }

  // sample the matrix and store the calibration values
  for (int colCount = 0; colCount < numCols; colCount++) {
    pinMode(cols[colCount], OUTPUT);
    digitalWrite(cols[colCount], LOW);

    for (int rowCount = 0; rowCount < numRows; rowCount++) {
      calibrationValues[objectIndex][rowCount][colCount] = analogRead(rows[rowCount]);
    }

    pinMode(cols[colCount], INPUT);  // reset pin mode
  }

  Serial.print("Calibration complete for object ");
  Serial.println(objectIndex + 1);

  // print calibration values for debug
  Serial.println("Calibration Values:");
  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numCols; j++) {
      Serial.print(calibrationValues[objectIndex][i][j]);
      Serial.print("\t");
    }
    Serial.println();
  }
}