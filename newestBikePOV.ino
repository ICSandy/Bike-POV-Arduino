boolean number0[5][7] = {
     {0,1,1,1,1,1,0},
     {1,0,0,0,0,0,1},
     {1,0,0,0,0,0,1},
     {0,1,1,1,1,1,0},
     {0,0,0,0,0,0,0}
};
boolean number1[5][7] = {
     {0,0,1,0,0,0,1},
     {0,1,0,0,0,0,1},
     {1,1,1,1,1,1,1},
     {0,0,0,0,0,0,1},
     {0,0,0,0,0,0,0}
};
boolean number2[5][7] = {
     {0,1,1,0,0,1,1},
     {1,0,0,0,1,0,1},
     {1,0,0,1,0,0,1},
     {0,1,1,0,0,0,1},
     {0,0,0,0,0,0,0}
};
boolean number3[5][7] = {
     {0,1,0,0,0,1,0},
     {1,0,0,0,0,0,1},
     {1,0,0,1,0,0,1},
     {0,1,1,0,1,1,0},
     {0,0,0,0,0,0,0}
};
boolean number4[5][7] = {
     {1,1,1,0,0,0,0},
     {0,0,1,0,0,0,0},
     {0,0,1,0,0,0,0},
     {1,1,1,1,1,1,1},
     {0,0,0,0,0,0,0}
};
boolean number5[5][7] = {
     {1,1,1,0,0,1,0},
     {1,0,1,0,0,0,1},
     {1,0,1,0,0,0,1},
     {1,0,0,1,1,1,0},
     {0,0,0,0,0,0,0}
};
boolean number6[5][7] = {
     {0,1,1,1,1,1,0},
     {1,0,0,1,0,0,1},
     {1,0,0,1,0,0,1},
     {0,1,0,0,1,1,0},
     {0,0,0,0,0,0,0}
};
boolean number7[5][7] = {
     {1,0,0,0,0,0,0},
     {1,0,0,0,1,1,1},
     {1,0,1,1,0,0,0},
     {1,1,0,0,0,0,0},
     {0,0,0,0,0,0,0}
};
boolean number8[5][7] = {
     {0,1,1,0,1,1,0},
     {1,0,0,1,0,0,1},
     {1,0,0,1,0,0,1},
     {0,1,1,0,1,1,0},
     {0,0,0,0,0,0,0}
};
boolean number9[5][7] = {
     {0,1,1,0,0,0,0},
     {1,0,0,1,0,0,1},
     {1,0,0,1,0,0,1},
     {0,1,1,1,1,1,0},
     {0,0,0,0,0,0,0}
};
boolean letterK[5][7] = {
     {1,1,1,1,1,1,1},
     {0,0,1,0,0,0,0},
     {0,1,0,1,1,0,0},
     {1,0,0,0,0,1,1},
     {0,0,0,0,0,0,0}
};
boolean letterP[5][7] = {
     {1,1,1,1,1,1,1},
     {1,0,0,1,0,0,0},
     {1,0,0,1,0,0,0},
     {0,1,1,0,0,0,0},
     {0,0,0,0,0,0,0}
};
boolean letterH[5][7] = {
     {1,1,1,1,1,1,1},
     {0,0,0,1,0,0,0},
     {0,0,0,1,0,0,0},
     {1,1,1,1,1,1,1},
     {0,0,0,0,0,0,0}
};

int led1 = 13;
int led2 = 12;
int led3 = 11;
int led4 = 10;
int led5 = 9;
int led6 = 8;
int led7 = 7;
int hallPin = 2;
int leds[] = {led1, led2, led3, led4, led5, led6, led7};

unsigned long milliSeconds;
unsigned long lastMilliCount = 0;
unsigned long time = 0;

float radiusWheel = 330.2; // millimeters
float circumfrence;
float radiusLED = 159.15; // millimeters
float slices;

float blinkRate = 0;
int KPH = 0;
float RPS = 0;

boolean magnetPassed = false;

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(hallPin, INPUT);
  
  attachInterrupt(0, updateCalculations, FALLING);
  slices = (2*3.14159265359*radiusLED)/5;
  circumfrence = 2*3.14159265359*radiusWheel;
  Serial.begin(9600);
  time = millis();
}

void loop() {
  if (!(KPH <= 2)) { //blinkrates screw up under 2 kph (~1.2427mph) so we just ignore everything
    for (int i = 0; i < (slices/4); i++) {
      delayMicroseconds(blinkRate * 1000); //blinkrate needs to be converted from millis to mi
    }
    printLED(blinkRate, KPH);
    
    while (!magnetPassed) {
      Serial.println("waiting");// THIS LINE IS REQUIRED FOR IT TO WORK. It's probably got something to do with while loops being emtpy
      //waiting for magnet to pass
    }
    magnetPassed = false;
  }
}

int getMilliSeconds () {
  time = millis();
  milliSeconds = time - lastMilliCount; //gets ms count since last roation
  lastMilliCount = time;
  return milliSeconds;
}

void updateCalculations() {
  magnetPassed = true;

  calculateRPS(); // rps needs to happen first

  calculateBlinkRate();

  calculateKPH();
}

float calculateRPS() {
  RPS = 1000/(float)getMilliSeconds();
}

void calculateBlinkRate() {
  blinkRate = 1000/(RPS*(slices));
}

int calculateKPH() {
  KPH = (circumfrence*RPS*60*60)/1000000;
}

void printLED (float blinkRate, int KPH) {
  String num = String(KPH);
  blinkRate = blinkRate * 1000; //from milliseconds to microseconds
  
  for (int i = 0; i < 5; i++) {
    printCharacter(blinkRate, num.charAt(i));
  }


  for (int i = 0; i < 3; i++) { // adding spaces between number and kph
    for (int j = 0; j < 5; j++) {
      digitalWrite(leds[j], LOW);
    }
    delayMicroseconds(blinkRate);
  }
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 7; j++) {
      digitalWrite(leds[j], letterK[i][j]);
    }
    delayMicroseconds(blinkRate);
  }
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 7; j++) {
      digitalWrite(leds[j], letterP[i][j]);
    }
    delayMicroseconds(blinkRate);
  }
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 7; j++) {
      digitalWrite(leds[j], letterH[i][j]);
    }
    delayMicroseconds(blinkRate);
  }

  for (int i = 0; i < 5; i++) {//setting all leds to low
      digitalWrite(leds[i], LOW);
  }
}

void printCharacter(float blinkRate, char num) {
      if (num == '0') { // This is my sloppy way to work around the fact C can't return array objects >:(
        for (int i = 0; i < 5; i++) {
          for (int j = 0; j < 7; j++) {
            digitalWrite(leds[j], number0[i][j]);
          }
          delayMicroseconds(blinkRate);
        }
      }
      else if (num == '1') {
        for (int i = 0; i < 5; i++) {
          for (int j = 0; j < 7; j++) {
            digitalWrite(leds[j], number1[i][j]);
          }
          delayMicroseconds(blinkRate);
        }
      }
      else if (num == '2') {
        for (int i = 0; i < 5; i++) {
          for (int j = 0; j < 7; j++) {
            digitalWrite(leds[j], number2[i][j]);
          }
          delayMicroseconds(blinkRate);
        }
      }
      else if (num == '3') {
        for (int i = 0; i < 5; i++) {
          for (int j = 0; j < 7; j++) {
            digitalWrite(leds[j], number3[i][j]);
          }
          delayMicroseconds(blinkRate);
        }
      }
      else if (num == '4') {
        for (int i = 0; i < 5; i++) {
          for (int j = 0; j < 7; j++) {
            digitalWrite(leds[j], number4[i][j]);
          }
          delayMicroseconds(blinkRate);
        }
      }
      else if (num == '5') {
        for (int i = 0; i < 5; i++) {
          for (int j = 0; j < 7; j++) {
            digitalWrite(leds[j], number5[i][j]);
          }
          delayMicroseconds(blinkRate);
        }
      }
      else if (num == '6') {
        for (int i = 0; i < 5; i++) {
          for (int j = 0; j < 7; j++) {
            digitalWrite(leds[j], number6[i][j]);
          }
          delayMicroseconds(blinkRate);
        }
      }
      else if (num == '7') {
        for (int i = 0; i < 5; i++) {
          for (int j = 0; j < 7; j++) {
            digitalWrite(leds[j], number7[i][j]);
          }
          delayMicroseconds(blinkRate);
        }
      }
      else if (num == '8') {
        for (int i = 0; i < 5; i++) {
          for (int j = 0; j < 7; j++) {
            digitalWrite(leds[j], number8[i][j]);
          }
          delayMicroseconds(blinkRate);
        }
      }
      else if (num == '9') {
        for (int i = 0; i < 5; i++) {
          for (int j = 0; j < 7; j++) {
            digitalWrite(leds[j], number9[i][j]);
          }
          delayMicroseconds(blinkRate);
        }
      } else {
        for (int i = 0; i < 5; i++) {
          digitalWrite(leds[i], LOW);
        }
      }
}
