#include <SoftwareSerial.h>

//SoftwareSerial bluetooth(10,11); // RX, TX - Arduino Uno Og
SoftwareSerial bluetooth(4,5); // RX, TX

char command;

/*int IN1 = 6;
int IN2 = 7;
int IN3 = 8;
int IN4 = 9;*/

int IN1 = 12;
int IN2 = 13;
int IN3 = 14;
int IN4 = 15;

// Safety timer
unsigned long lastCommandTime = 0;
const int timeout = 2000; // 2 seconds

void setup()
{
  Serial.begin(9600);
  bluetooth.begin(9600);

  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);

  stopMotor(); // start safely stopped
  bluetooth.println("START");
}


void loop()
{
  Serial.print("Available = ");
  Serial.println(bluetooth.available());

  while(bluetooth.available())
  {
      char c = bluetooth.read();

      Serial.print("Received: ");
      Serial.println(c);

      command = c;
      lastCommandTime = millis();
  }

  delay(100);
}

/*
void loop()
{
  if(bluetooth.available())
  {
    command = bluetooth.read();

    lastCommandTime = millis(); // reset safety timer

    Serial.println(command);   // debug

    switch(command)
    {
      case 'F': forward(); break;
      case 'B': backward(); break;
      case 'L': left(); break;
      case 'R': right(); break;
      case 'S': stopMotor(); break;
    }
  }

  // Watchdog safety check
  if(millis() - lastCommandTime > timeout)
  {
    stopMotor();
  }
}
*/

void left()
{
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
}

void right()
{
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
}

void backward()
{
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
}

void forward()
{
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
}

void stopMotor()
{
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
}