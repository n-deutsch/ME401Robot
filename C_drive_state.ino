
//default function called when currentstate==0
void drive_master(){
   drive(93,-100);
   return;
}


void sense() {
  Serial.println("sense()");
  LongSensorValue=analogRead(LongSensorPin);
  //delay(100);
  //delay(100);
  //IMPLEMENT READING TO DISTANCE CONVERSION FOR THE FIRST SENSOR
  Long_Vlin=(((LongSensorValue/1023.0)*3.3));
  LongLength= (14.014*pow(Long_Vlin,4.0) - 113.58*pow(Long_Vlin,3.0) + 342.96*pow(Long_Vlin,2.0) - 477.77*Long_Vlin + 298.14);
  if(LongLength >= 37) { 
    Serial.println("No obstruction...");
    currentState =0;  // straight driving
    //drive(93,-100 );
    colorDetectResult = colorDetection();
    if(colorDetectResult == 1) {
     Serial.println("BLACK detected!");
     delay(2000);  //!!!UNCOMMENT THIS FOR BLACK!!!
     currentState = 10; //!!!UNCOMMENT THIS FOR BLACK!!!
    }
  } 
  else if (LongLength < 37) { 
    Serial.println("Obstruction detected!");
    drive(0,0);
    currentState = 1; // Scanning
    colorDetectResult = colorDetection();
    if(colorDetectResult == 1) {
     Serial.println("BLACK detected!");
     delay(2000);  //!!!UNCOMMENT THIS FOR BLACK!!!
     currentState = 10; //!!!UNCOMMENT THIS FOR BLACK!!!
   }
  } 
Serial.print("Long Sensor:");
Serial.print(LongSensorValue);
Serial.print("Long Length:");
Serial.print(LongLength);
Serial.println("");
  
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
