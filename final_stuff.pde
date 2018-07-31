#include <Servo.h>
#include <SoftPWMServo.h>
//#include "globals.h"

//States
enum StateMachineState {
  main = 0,
  left = 1,
  right = 2,
};

//DC MOTOR VARIABLES
static volatile char lastLeftA;
static volatile char lastLeftB;
static volatile bool errorLeft;
volatile long position = 0;

String motorType="Forward";
double Kp=10,Ki=0,Kd=3;
int targetAngle=0;
int errorAngle=0;
float incomingByte = 0;
String inString= "";
int conversion=1070;

//initialization of the variables for the PID calculation
double input=0, lastinput=0, output=0, setpoint=0;
double error=0, iterm=0,dinput=0;
double kp=0, ki=0, kd=0;
double outmax=2400;
double outmin=-2400;
int pidSampleTime = 10;
double SampleTimeInSec = ((double)pidSampleTime/1000);
long counter=1;


//IR Distance
float myVariabe=4.356;
//short
int ShortSensorPin=A9;
float ShortSensorValue;
float Short_Vlin;
float ShortLength;
//long
int LongSensorPin=A10;
float LongSensorValue;
float Long_Vlin;
float LongLength;

//SERVO VARIABLES
Servo lMotor;
Servo rMotor;
int valL=30;
int valR=31;
int lPulseWidth = 1500;
int rPulseWidth = 1500;
int lSpeed=30;
int rSpeed=31;

//Switches
bool LEFTswitch = false;
bool RIGHTswitch = false;
int LEFTpin = 7;
int RIGHTpin = 6;
int LEFT = 0;
int RIGHT = 0;


//States
StateMachineState state;
int currentState = 0;
int nextState = 0;

// Color Variables
int sensorPin = 0;    // select the analog input pin for the photoresistor
int bPin = 9;  // select the digital output pin for blue LED
int gPin = 10; // select the digital output pin for green LED 
int rPin = 11; // select the digital output pin for red LED
int sensorValue = 0;  // variable to store the value coming from the photoresistor
int maxblack[]={375,512,560};// the max reading that will be obtained from a black surface {RED,GREEN,BLUE}
int minwhite[]={177,220,256}; // the min reading that will be obtained from a white surface {RED,GREEN,BLUE}
int color[]={0,0,0}; // array for the readings
int redStripeCount = 0;
int blueStripeCount = 0;
int colorDetectResult = 0;
int rToggle = 0;
int bToggle = 0;

//park variables
int windowTime = 100;   // ms
int sampleTime= 1;     // ms
int windowIters = windowTime/sampleTime;
float frequency = 0;
const int IRSensorInputPin = 12;




#define DETECT 1
#define NOSIGNAL -1

void setup(){
  Serial.begin(9600);
  turn_setup();
  switch_setup();
  pinMode(rPin, OUTPUT);
  pinMode(gPin, OUTPUT);
  pinMode(bPin, OUTPUT);
}

void loop(){
  Serial.println("===============CENTRAL LOOP===============");
  //if we are NOT currently parking

  if(currentState != 11 && currentState != 10)
  {
    sense(); //sensor & color
    check_switches(); //switches
  }

  switch(currentState){
  case 0: drive_method(); break; //drive
  case 1: turn_method(); break; //sensor turn
  case 2: switch_method(); break; //switch turn
  case 10: park_method(); break; //park
  case 11: Stop();
  }
  
  return;
}

void drive_method(void)
{
  Serial.println("Driving...");
  drive_master();
}

void turn_method(void)
{
  Serial.println("Turning...");
  new_direction();
}

void switch_method(void)
{
  Serial.println("Switch flipped...");
  if(state == left)
  {
    Serial.println("LEFT SWITCH");
    runLeftState();
  }
  else if(state == right)
  {
    Serial.println("RIGHT SWITCH");
    runRightState();
  }
  else
  {
    Serial.print("UNKNOWN STATE GOD HELP US ALL");
    Serial.print(state);
  }
}

void park_method(void)
{
  Serial.println("Parking...");
      drive(0,0); //Stand still
      Serial.print("redStripeCount:");
      Serial.println(redStripeCount);
      Serial.print("blueStripeCount:");
      Serial.println(blueStripeCount);
      delay(1000);
   if (park_this_baby()){
     currentState = 11; //program does nothing (end)
     Serial.println("Parking Completed");
   }
   else {
    Serial.println("Parking Failed... Trying Again"); 
   }
  
  return;
}

void Stop(void)
{
 drive(0,0); 
}
