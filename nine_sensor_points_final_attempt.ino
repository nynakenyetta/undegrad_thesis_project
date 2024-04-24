#define numRows 3
#define numCols 3
#define sensorPoints numRows* numCols

int rows[] = { A0, A1, A2 };
int cols[] = { 5, 6, 7 };
int incomingValues[sensorPoints] = {};

void setup() {
  // set all rows and columns to INPUT (high impedance):
  for (int i = 0; i < numRows; i++) {
    pinMode(rows[i], INPUT_PULLUP);
  }
  for (int i = 0; i < numCols; i++) {
    pinMode(cols[i], INPUT);
  }
  Serial.begin(9600);
}

void loop() {

  if (Serial.available() > 0){
  for (int colCount = 0; colCount < numCols; colCount++) {
    pinMode(cols[colCount], OUTPUT);    // set as OUTPUT
    digitalWrite(cols[colCount], LOW);  // set LOW

    for (int rowCount = 0; rowCount < numRows; rowCount++) {
      incomingValues[colCount * numRows + rowCount] = analogRead(rows[rowCount]);  // read INPUT
    }                                                                              // end rowCount

    pinMode(cols[colCount], INPUT);  // set back to INPUT!

  }  // end colCount
  String data;
  // Print the incoming values of the grid:
  for (int i = 0; i < sensorPoints; i++) {
    if (i == 0) {data.concat("S1: "); data.concat(1023 - incomingValues[i]);
    }else if (i == 1){ data.concat(" S2: "); data.concat(1023 - incomingValues[i]);
    }else if (i == 2){ data.concat(" S3: "); data.concat(1023 - incomingValues[i]);
    }else if (i == 3){ data.concat(" S4: "); data.concat(1023 - incomingValues[i]);
    }else if (i == 4){ data.concat(" S5: "); data.concat(1023 - incomingValues[i]);
    }else if (i == 5){ data.concat(" S6: "); data.concat(1023 - incomingValues[i]);
    }else if (i == 6){ data.concat(" S7: "); data.concat(1023 - incomingValues[i]);
    }else if (i == 7){ data.concat(" S8: "); data.concat(1023 - incomingValues[i]); 
    }else if (i == 8){ data.concat(" S9: "); data.concat(1023 - incomingValues[i]);
    }

    //Serial.print(incomingValues[i]);
    //Serial.print("\t"); //this is an escape sequence, you tell it to skip one
  }
  Serial.println(data);
  }
}