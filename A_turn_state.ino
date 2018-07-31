
void turn_setup() {
  //DC STETUP
  // Set up the serial port in case we want output or input
  //Serial.begin(9600); REDUNDANT
  Serial.println("Timer Interrupt Encoder Test:");
  
  lMotor.attach(30);
  rMotor.attach(31);
  
  // Set up the quadrature inputs
  pinMode(2, INPUT);
  pinMode(20, INPUT);
  errorLeft = false;
  lastLeftA = digitalRead(2);
  lastLeftB = digitalRead(20);
  
  //timer interrupt is to read the encoder position and generate the PID output
  attachCoreTimerService(MyCallback);
  
  // Set up the motor outputs
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  
  digitalWrite(3,0);
  digitalWrite(4,0);
  
  SoftPWMServoPWMWrite(3, 0);
}


void new_direction() {
  /*
  //IR LOOP RAW
  ShortSensorValue=analogRead(ShortSensorPin);//READ FROM ONE OF THE SENSORS
  delay(100); //wait for the measurement to settle
  //IMPLEMENT READING TO DISTANCE CONVERSION FOR THE FIRST SENSOR
  //READ FROM THE OTHER SENSOR
  //IMPLEMENT READING TO DISTANCE CONVERSION FOR THE FIRST SENSOR
  Short_Short_Vlin=(((ShortSensorValue/1023.0)*3.3));
  ShortLength= (-2.8554*pow(Short_Short_Short_Vlin,5.0) + 27.181*pow(Short_Short_Vlin,4.0)- 100.24*pow(Short_Short_Vlin,3.0) + 181.08*pow(Short_Short_Vlin,2.0) - 167.68*Short_Short_Vlin + 75.18);
  Serial.print("Short Sensor:");
  Serial.print(ShortSensorValue); //PRINT THE OUTPUT FOR THE FIRST SENSOR IN DISTANCE UNITS, CHANGE THE VARIABLE NAME ACCORDINGLY
  Serial.print("Short Length:");
  Serial.print(ShortLength); //PRINT THE OUTPUT FOR THE SECOND SENSOR, CHANGE THE VARIABLE NAME ACCORDINGLY
  Serial.println("");
  */
  
  //DC MOTOR LOOP
  long newPosition = position;
  
  targetAngle = -105*(1070/360);  //Set angle
  Serial.print("Target Angle 1: ");
  Serial.print(targetAngle);
  delay(1000);
  ShortSensorValue=analogRead(ShortSensorPin);  //Read sensor value
  Short_Vlin=(((ShortSensorValue/1023.0)*3.3));
  ShortLength= (-2.8554*pow(Short_Vlin,5.0) + 27.181*pow(Short_Vlin,4.0)- 100.24*pow(Short_Vlin,3.0) + 181.08*pow(Short_Vlin,2.0) - 167.68*Short_Vlin + 75.18);
  float rightlook = ShortLength;
  Serial.print("rightlook");
  Serial.println(rightlook);
  
  targetAngle = 105*(1070/360);  //Set angle opposite direction
  Serial.print("Target Angle 2: ");
  Serial.print(targetAngle);
  delay(1000);
  ShortSensorValue=analogRead(ShortSensorPin);  //Read sensor value
  Short_Vlin=(((ShortSensorValue/1023.0)*3.3));
  ShortLength= (-2.8554*pow(Short_Vlin,5.0) + 27.181*pow(Short_Vlin,4.0)- 100.24*pow(Short_Vlin,3.0) + 181.08*pow(Short_Vlin,2.0) - 167.68*Short_Vlin + 75.18);
  float leftlook = ShortLength;
  Serial.print("leftlook");
  Serial.println(leftlook);
  
  targetAngle = 0;
  delay(100);
  
  if (rightlook <= leftlook) {
    drive(-100,-100); //turn left
    delay(540);
    drive(0,0);
  }   
  else if (leftlook <= rightlook) {
  drive(100,100); //turn right
  delay(540);
  drive(0,0);
  }
  //delay(50); 
}

//timer interrupt ISR
uint32_t MyCallback(uint32_t currentTime) {
  //read the enconder position
  char newLeftA = digitalRead(2);
  char newLeftB = digitalRead(20);
  static int lastVal = digitalRead(12);
  static int iters = 0;
  static int counter2 = 0;
  position += (newLeftA ^ lastLeftB) - (lastLeftA ^ newLeftB);
  
  if((lastLeftA ^ newLeftA) & (lastLeftB ^ newLeftB))
    {
        errorLeft = true;
    }
  
  lastLeftA = newLeftA;
  lastLeftB = newLeftB;
    
  //run the PID calculation if the sampling time elapsed since the previous calculation.
  if (counter % 100*pidSampleTime == 0)
  {
   input = position;           
   setpoint=map(targetAngle,-360,360,-1200,1200);
   kp = Kp;
   ki = Ki * SampleTimeInSec;
   kd = Kd / SampleTimeInSec;
   error=setpoint-input;
   errorAngle=map(error,0,1200,0,360);
   iterm +=ki*error;
   if (iterm>outmax) iterm=outmax;
   else if (iterm<outmin) iterm=outmin;
   dinput=input-lastinput;
   output=kp*error+iterm-kd*dinput;
   if(output > outmax) output = outmax;
   else if(output < outmin) output = outmin;
   lastinput=input;
   
    if ((motorType== "Forward" && output < 0) || (motorType== "Reverse" && output > 0))
    {
      digitalWrite(4,1);
      
    }
    else
    {
      digitalWrite(4,0);
    }  
    SoftPWMServoPWMWrite(3,abs(output));
    counter = 0;
  }
    
 if(counter % 100 ==0)
 {
  if (iters < windowIters)
  {
    int newVal = digitalRead(12);
    if (newVal == HIGH && lastVal == LOW)
    {
      counter2++;
    }
    lastVal = newVal;
  }
  
  else
  {
    frequency = 1000.0*(float)counter2/(float)windowTime;

    counter2 = 0;
    int newVal = digitalRead(12);    
    lastVal = newVal;
    iters = 0;
  }

  iters++;
 }
  counter++;
  
  return (currentTime + CORE_TICK_RATE/100);
}


/*
void drive(int lSpeed, int rSpeed) {
   lPulseWidth = map(lSpeed, -100, 100, 1300, 1700);
   lMotor.writeMicroseconds(lPulseWidth);
   rPulseWidth = map(rSpeed, -100, 100, 1300, 1700);
   rMotor.writeMicroseconds(rPulseWidth);
   delay(15);
}
*/



