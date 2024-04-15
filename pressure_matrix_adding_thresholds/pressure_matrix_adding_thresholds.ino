/*
Pressure Sensor Matrix Code
parsing through a pressure sensor matrix grid by switching individual
rows/columns to be HIGH, LOW or INPUT (high impedance) to detect
location and pressure.
>> https://www.kobakant.at/DIY/?p=7443
*/
#define numRows 3
#define numCols 3
#define sensorPoints numRows*numCols

int rows[] = {A0, A1, A2};
int cols[] = {5,6,7};
int sensorValues[numRows][numCols]; // array to store sensor values

void setup() {
  Serial.begin(9600);
  
  for (int i = 0; i < numRows; i++) {
    pinMode(rows[i], INPUT_PULLUP);
  }
  
  for (int i = 0; i < numCols; i++) {
    pinMode(cols[i], INPUT);
  }
}

void loop() {
  for (int colCount = 0; colCount < numCols; colCount++) {
    pinMode(cols[colCount], OUTPUT); // set as OUTPUT
    digitalWrite(cols[colCount], LOW); // set LOW

    for (int rowCount = 0; rowCount < numRows; rowCount++) {
      sensorValues[rowCount][colCount] = analogRead(rows[rowCount]); // read INPUT
    }// end rowCount

    pinMode(cols[colCount], INPUT); // set back to INPUT!
  }// end colCount

  // Print the incoming values of the grid:
  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numCols; j++) {
     // Serial.print(sensorValues[i][j]);
      //if (j < numCols - 1) Serial.print("\t");
    }
    //Serial.println();
  }
  //Serial.println();

  // Add threshold-based logic here
  // For example, to detect a pressure above a certain value:
  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numCols; j++) {
      if (sensorValues[i][j] <= 30) {
        //Serial.print("Pressure detected at row: ");
        Serial.print(i);
        Serial.print(" ");
        //Serial.print(", column: ");
        Serial.println(j);
      }
    }
  }

  delay(10);
}