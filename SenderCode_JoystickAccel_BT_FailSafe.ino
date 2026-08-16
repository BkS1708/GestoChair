#include <SoftwareSerial.h>

//SoftwareSerial bluetooth(4,5); // RX, TX
SoftwareSerial bluetooth(5,4); // RX, TX


// Joystick pins
int joyX = A0;
int joyY = A1;

// Accelerometer pins
int accX = A5;
int accY = A6;

// Mode switch
int switchPin = 11;

bool mode = 1;  // 0 = Joystick , 1 = Gesture
bool lastSwitchState = HIGH;

char command = 'S';
char lastCommand = 'X';

// Heartbeat timer
unsigned long lastSendTime = 0;
const int sendInterval = 200; // send command every 200ms

void setup()
{
  Serial.begin(9600);
  bluetooth.begin(9600);

  pinMode(switchPin, INPUT_PULLUP);

  Serial.println("System Ready");
}

void loop()
{
  bool switchState = digitalRead(switchPin);

  // Toggle mode when switch pressed
  if(lastSwitchState == HIGH && switchState == LOW)
  {
    mode = !mode;

    if(mode == 0)
      Serial.println("Joystick Mode");
    else
      Serial.println("Gesture Mode");

    delay(300);
  }

  lastSwitchState = switchState;

  if(mode == 0)
    joystickControl();
  else
    gestureControl();

  // Heartbeat transmission
  if(millis() - lastSendTime > sendInterval)
  {
    bluetooth.write(command);
    lastSendTime = millis();
  }

  // Print only when command changes
  if(command != lastCommand)
  {
    Serial.println(command);
    lastCommand = command;
  }
}

void joystickControl()
{
  int x = analogRead(joyX);
  int y = analogRead(joyY);

  if (y > 650)
    command = 'F';

  else if (y < 350)
    command = 'B';

  else if (x < 350)
    command = 'L';

  else if (x > 650)
    command = 'R';

  else
    command = 'S';
}

void gestureControl()
{
  int x = analogRead(accX);
  int y = analogRead(accY);

  if (x > 360)
    command = 'F';

  else if (x < 300)
    command = 'B';

  else if (y < 300)
    command = 'R';

  else if (y > 360)
    command = 'L';

  else
    command = 'S';
}