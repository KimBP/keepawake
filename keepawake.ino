// Keepawake. A 'deadmans' function sounding an alarm if a button is not pressed in due time.
// Configurations:
//   Digital out for beeper.
//   Digital out for LED
//   Digital in for push button,
//   In delayTable configure valid mode pairs (max duration between button pushes - and the number of flashes to indicate mode)

// Usage: A short press on button resets timer (prevents/stops beeper)
//        A long press on button shifts to next 'duration'. LED flashes indicates mode


// Todo: Maybe LED should be lit to indicate power on

enum tKeyMode {
  KEY_NONE,
  KEY_SHORT,
  KEY_LONG,
};


const int beepOut=4; // Connect beeper to this digital port
const int ledOut=13;   // Connect anode LED to this port. Catode to GND
const int keyIn=3;    // Connect a pushbutton between this port and GND

static int sleepTime;
static unsigned long lastKick;
static int modeIdx;

const struct {
  unsigned int delay; // In seconds
  unsigned int flashes;
} delayTable[] = {
  {3*60, 1},
  {6*60, 2},
  {9*60, 3},
  {0,0}
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  pinMode(ledOut, OUTPUT);
  pinMode(beepOut, OUTPUT);
  pinMode(keyIn, INPUT_PULLUP);
  lastKick = millis();
  modeIdx=0;
  modeNext();
}



enum tKeyMode detectKeyPress(int dbg) {
  if (digitalRead(keyIn) == HIGH) {
    return KEY_NONE;
  } else {
    unsigned long t_now = millis();
    waitKeyRelease();
    if (millis() - t_now < 500) {
      Serial.print(dbg);
      Serial.println(": shortkey detected");
      return KEY_SHORT;
    } else {
      Serial.print(dbg);
      Serial.println(": longkey detected");
      return KEY_LONG;
    }
  }
}

void waitKeyRelease() {
  while (digitalRead(keyIn) == LOW) {}
}

void handleRun(tKeyMode key)
{
  static bool outHigh = false;
  unsigned long t_now = millis();
  if (key == KEY_SHORT) {
    lastKick = t_now;
    digitalWrite(beepOut, LOW);
    outHigh = false;
    Serial.println("kick");
  } else {
    if ((outHigh==false) && (t_now > lastKick + sleepTime)) {
      digitalWrite(beepOut, HIGH);
      outHigh=true;
      Serial.println("wakeup");
    }
  }
}

void do_blink(int cnt) {
  for(int i=0; i < cnt; i++) {
    digitalWrite(ledOut,HIGH);
    delay(500);
    digitalWrite(ledOut, LOW);
    delay(500);
  }
}

void modeNext()
{
  modeIdx++;
  if (delayTable[modeIdx].delay == 0) {
    modeIdx=0;
  }
  do_blink(delayTable[modeIdx].flashes);
  sleepTime=delayTable[modeIdx].delay*1000;
//  while (detectKeyPress(0) != KEY_NONE) {}
  Serial.print("Setting delay to ");
  Serial.print(delayTable[modeIdx].delay);
  Serial.println(" seconds");
  lastKick=millis();
}


void loop() {
  tKeyMode keyp = detectKeyPress(1);
  if (keyp == KEY_LONG) {
    modeNext();
  } else {
    handleRun(keyp);
  }
//  if (keyp != KEY_NONE) {
//    while (detectKeyPress(2) != KEY_NONE) {}
//  }
}
