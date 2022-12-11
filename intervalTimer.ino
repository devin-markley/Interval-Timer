#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//Constants
const int SECONDSBTN = 3;  //BTN for adding seconds to the timer
const int MINUTESBTN = 2;  //BTN for adding minutes to the timer
const int STARTBTN = 9;    //BTN for starting the timer
const int MODEBTN = 13;    //BTN for assigning seconds to work

//Declaring Integers indicating mode button state
const int WORK = 0;
const int REST = 1;
const int PERPARE = 2;
const int SETS = 3;


//Variables
int workMinutesLeft = 0;     //Amount of Work Minutes left
int workSecondsLeft = 0;     //Amount of Work Seconds left
int restMinutesLeft = 0;     //Amount of Rest Minutes left
int restSecondsLeft = 0;     //Amount of Rest Seconds left
int perpareMinutesLeft = 0;  //Amount of perpare seconds left
int perpareSecondsLeft = 0;  //Amount of perpare minutes left
int setsLeft = 0;            //Amount of sets left

//Start Button State
int startButtonState = 0;
int lastStartButtonState = 0;

//Sets Button State
int setButtonState = 0;
int lastSetButtonState = 0;

//Mode Button State
int modeRun = 0;
int modeButtonState = 0;
int lastModeButtonState = 0;

//Minutes and Seconds Button States
int secondsButtonState = 0;
int minutesButtonState = 0;

//Declaring x's and y's for text placement on display
int minuteLocationX = 34;
int secondLocationX = 58;
int titelLocationX = 0;
int titleLocationY = 0;
int totalTimeLocationX = 34;
int middleOfScreenY = 32;

//Prototyping Functions
void centerString(const String &buf, int x, int y);
void timer(const String &buf, int x, int y);
bool isButtonPressed(int buttonPin, int &currentButtonState, int &lastButtonState);
void adjustingModeTime(int modeSeconds, int modeMinutes);

void setup() {
  //Declaring Buttons
  pinMode(SECONDSBTN, INPUT);
  pinMode(MINUTESBTN, INPUT);
  pinMode(STARTBTN, INPUT);
  pinMode(MODEBTN, INPUT);

  //Setting text color and size
  display.setTextSize(2);
  display.setTextColor(1);

  //Home Screen
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000);
  display.clearDisplay();
  centerString("Work", 64, 0);
  timer("00:00", totalTimeLocationX, middleOfScreenY);
}

void loop() {
  //if Mode button is was pressed add time to the declared state
  if (isButtonPressed(MODEBTN, modeButtonState, lastModeButtonState)) {
    if (modeRun == WORK) {
      modeRun = REST;
      centerString("Rest", 64, 0);
      char buf[4];
      sprintf(buf, "%02d:%02d", restMinutesLeft, restSecondsLeft);
      timer(buf, totalTimeLocationX, middleOfScreenY);
    } else if (modeRun == REST) {
      modeRun = PERPARE;
      centerString("Prep", 64, 0);
      char buf[4];
      sprintf(buf, "%02d:%02d", perpareMinutesLeft, perpareSecondsLeft);
      timer(buf, totalTimeLocationX, middleOfScreenY);
    } else if (modeRun == PERPARE) {
      modeRun = SETS;
      centerString("Sets", 64, 0);
      char buf[5];
      sprintf(buf, "  %d  ", setsLeft);
      centerString(buf, 64, middleOfScreenY);
    } else if (modeRun == SETS) {
      modeRun = WORK;
      centerString("Work", 64, 0);
      char buf[4];
      sprintf(buf, "%02d:%02d", workMinutesLeft, workSecondsLeft);
      timer(buf, totalTimeLocationX, middleOfScreenY);
    }
  }

  //Storing state for minutes and seconds button
  secondsButtonState = digitalRead(SECONDSBTN);
  minutesButtonState = digitalRead(MINUTESBTN);

  //Storing total time for each mode
switch (modeRun) {
  case WORK:
    if (secondsButtonState == HIGH) {
      if (workSecondsLeft < 60) {
        workSecondsLeft++;
      } else {
        workSecondsLeft = 0;
      }
      char buf[4];
      sprintf(buf, ":%02d", workSecondsLeft);
      timer(buf, secondLocationX, middleOfScreenY);
    }
    if (minutesButtonState == HIGH) {
      if (workMinutesLeft < 60) {
        workMinutesLeft++;
      } else {
        workMinutesLeft = 0;
      }
      char buf[4];
      sprintf(buf, "%02d", workMinutesLeft);
      timer(buf, minuteLocationX, middleOfScreenY);
    }
    break;
  case REST:
    if (secondsButtonState == HIGH) {
      if (restSecondsLeft < 60) {
        restSecondsLeft++;
      } else {
        restSecondsLeft = 0;
      }
      char buf[4];
      sprintf(buf, ":%02d", restSecondsLeft);
      timer(buf, secondLocationX, middleOfScreenY);
    }
    if (minutesButtonState == HIGH) {
      if (restMinutesLeft < 60) {
        restMinutesLeft++;
      } else {
        restMinutesLeft = 0;
      }
      char buf[4];
      sprintf(buf, "%02d", restMinutesLeft);
      timer(buf, minuteLocationX, middleOfScreenY);
    }
    break;
  case PERPARE:
    if (secondsButtonState == HIGH) {
      if (perpareSecondsLeft < 60) {
        perpareSecondsLeft++;
      } else {
        perpareSecondsLeft = 0;
      }
      char buf[4];
      sprintf(buf, ":%02d", perpareSecondsLeft);
      timer(buf, secondLocationX, middleOfScreenY);
    }
    if (minutesButtonState == HIGH) {
      if (perpareMinutesLeft < 60) {
        perpareMinutesLeft++;
      } else {
        perpareMinutesLeft = 0;
      }
      char buf[4];
      sprintf(buf, "%02d", perpareMinutesLeft);
      timer(buf, minuteLocationX, middleOfScreenY);
    }
    break;
  case SETS:
    if (secondsButtonState == HIGH || minutesButtonState == HIGH) {
      if (setsLeft < 50) {
        setsLeft++;
      } else {
        setsLeft = 0;
      }
      centerString("Sets", 64, 0);
      char buf[6];
      sprintf(buf, "  %0d  ", setsLeft);
      //Centering differently for one or two digit numbers
      if (setsLeft < 10) {
        timer(buf, minuteLocationX, middleOfScreenY);
      } else {
        timer(buf, 28, middleOfScreenY);
      }
    }
    break;
}

if (isButtonPressed(STARTBTN, startButtonState, lastStartButtonState)) {
  for (int g = perpareSecondsLeft; g >= 0; g--) {
    centerString("Prep", 64, 0);
    char buf[5];
    sprintf(buf, "%02d:%02d", perpareMinutesLeft, g);
    centerString(buf, 64, middleOfScreenY);
  }
  for (int g = perpareMinutesLeft; g >= 0; g--) {
    char buf[5];
    sprintf(buf, "%02d:%02d", g, perpareSecondsLeft);
    centerString(buf, 64, middleOfScreenY);
    if (g < perpareMinutesLeft) {
      for (int i = 59; i >= 0; i--) {
        char buf[5];
        sprintf(buf, "%02d:%02d", g, i);
        centerString(buf, 64, middleOfScreenY);
        delay(100);
      }
    }
  }

  for (int s = setsLeft; s >= 0; s--) {
    centerString(" Set ", 64, 0);
    char buf[5];
    sprintf(buf, "  %d  ", s);
    centerString(buf, 64, middleOfScreenY);
    delay(1000);

    for (int g = workSecondsLeft; g >= 0; g--) {
      centerString("Work", 64, 0);
      char buf[5];
      sprintf(buf, "%02d:%02d", workMinutesLeft, g);
      centerString(buf, 64, middleOfScreenY);
    }
    for (int g = workMinutesLeft; g >= 0; g--) {
      char buf[5];
      sprintf(buf, "%02d:%02d", g, workSecondsLeft);
      centerString(buf, 64, middleOfScreenY);
      if (g < workMinutesLeft) {
        for (int i = 59; i >= 0; i--) {
          char buf[5];
          sprintf(buf, "%02d:%02d", g, i);
          centerString(buf, 64, middleOfScreenY);
          delay(100);
        }
      }
    }
    for (int g = restSecondsLeft; g >= 0; g--) {
      centerString("Rest", 64, 0);
      char buf[5];
      sprintf(buf, "%02d:%02d", restMinutesLeft, g);
      centerString(buf, 64, middleOfScreenY);
    }
    for (int g = restMinutesLeft; g >= 0; g--) {
      char buf[5];
      sprintf(buf, "%02d:%02d", g, restSecondsLeft);
      centerString(buf, 64, middleOfScreenY);
      if (g < restMinutesLeft) {
        for (int i = 59; i >= 0; i--) {
          char buf[5];
          sprintf(buf, "%02d:%02d", g, i);
          centerString(buf, 64, middleOfScreenY);
          delay(100);
        }
      }
    }
  }
  //Reseting Timer
  setsLeft = 0;
  workMinutesLeft = 0;
  workSecondsLeft = 0;
  restMinutesLeft = 0;
  restSecondsLeft = 0;
  perpareMinutesLeft = 0;
  perpareSecondsLeft = 0;
}
delay(100);
}

bool isButtonPressed(int buttonPin, int &currentButtonState, int &lastButtonState) {
  currentButtonState = digitalRead(buttonPin);
  // compare the buttonState to its previous state
  if (currentButtonState != lastButtonState) {
    lastButtonState = currentButtonState;
    // if the state has changed, return true
    if (currentButtonState == HIGH) {
      return true;
    }
  }
  // save the current state as the last state, for next time through the loop
  return false;
}

void centerString(const String &buf, int x, int y) {
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(buf, x, y, &x1, &y1, &w, &h);  //calc width of new string
  display.setCursor(x - w / 2, y);
  display.setTextColor(1);
  display.fillRect(x - w / 2, y, w, h, 0);
  display.print(buf);
  display.display();
}

void timer(const String &buf, int x, int y) {
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(buf, x, y, &x1, &y1, &w, &h);  //calc width of new string
  display.setCursor(x, y);
  display.fillRect(x, y, w, h, 0);
  display.print(buf);
  display.display();
}

void adjustingModeTime(int modeSeconds, int modeMinutes) {
  if (secondsButtonState == HIGH) {
    if (modeSeconds < 60) {
      modeSeconds++;
    } else {
      modeSeconds = 0;
    }
    char buf[4];
    sprintf(buf, ":%02d", modeSeconds);
    timer(buf, secondLocationX, middleOfScreenY);
  }
  if (minutesButtonState == HIGH) {
    if (modeMinutes < 60) {
      modeMinutes++;
    } else {
      modeMinutes = 0;
    }
    char buf[4];
    sprintf(buf, "%02d", modeMinutes);
    timer(buf, minuteLocationX, middleOfScreenY);
  }
}

void timer(int modeSecondsLeft, int modeMinutesLeft) {
  for (int g = modeSecondsLeft; g >= 0; g--) {
    centerString("Work", 64, 0);
    char buf[5];
    sprintf(buf, "%02d:%02d", modeMinutesLeft, g);
    centerString(buf, 64, middleOfScreenY);
  }
  for (int g = modeMinutesLeft; g >= 0; g--) {
    char buf[5];
    sprintf(buf, "%02d:%02d", g, modeSecondsLeft);
    centerString(buf, 64, middleOfScreenY);
    if (g < modeMinutesLeft) {
      for (int i = 59; i >= 0; i--) {
        char buf[5];
        sprintf(buf, "%02d:%02d", g, i);
        centerString(buf, 64, middleOfScreenY);
        delay(100);
      }
    }
  }
}