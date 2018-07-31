void frequency_stuff(){ 
  Serial.print("Val: ");
  int state = readIRFrequency();
  if (state == NOSIGNAL)
  {
   Serial.println("NO SIGNAL DETECTED");
  }
  else if (state == DETECT)
  {
   Serial.println("BEACON DETECTED!");
  }  
  delay(100);   // Wait just a tad so we don't print too fast.
}
    
 bool readIRFrequency(){
    if (frequency >= 75 && frequency < 135){
      return true; //Changed from DETECT
    }
    else{
      return false; //Changed from NOSIGNAL
    }
   }
/*
//Use this for range of frequency readings
void turnTowardsBeacon(){
  //turn left until frequency starts decreasing
  float lastFrequency = frequency;
  const float epsilon = .001;
  drive(-50,-50);
  while (lastFrequency < (frequency + epsilon)){
    delay(50);
    lastFrequency = frequency;
  }
  
  //turn right until frequency starts decreasing
 drive(50,50);
 lastFrequency = -1; //reset lastFrequency
  while (lastFrequency < (frequency + epsilon)){
    delay(50);
    lastFrequency = frequency;
  }
  drive(0,0);
}

*/

//=================================================//Use this for only HIGH and LOW readings=================================================
void turnTowardsBeaconAlt(){
  int driveValue = 25;
  //Note: A border is defined as the intersection of "HIGH" and "LOW" IR frequency signals
  //Turn left until we hit a border
  bool lastFrequency = readIRFrequency();
  
  drive(driveValue,driveValue);
 
  while (lastFrequency == readIRFrequency()){
    delay(10);
    Serial.println("readIR Frequency: ");
    Serial.print(readIRFrequency());
    Serial.println("Last Frequency: ");
    Serial.print(lastFrequency);
     if (lastFrequency != readIRFrequency()){
      lastFrequency = readIRFrequency();
      Serial.print("Broken Loop!!!!!!!!");
      break; 
     }
     lastFrequency = readIRFrequency();
  }
  
  Serial.print("====================================Hit Border 1 =======================================");
  /*
 //DELETE FOR FULL SWEEP OF IR!!!!!!!!!!!!!!!!
  //If HIGH-->LOW, turn right until we hit the opposite border
  if (lastFrequency){
    driveValue = -driveValue;
  }
  //Else If LOW-->HIGH, continue turning left until we hit the opposite border

  drive(driveValue,driveValue);
  
  delay(100);
  //Grab current time
  unsigned long currentTime = millis();
  while (lastFrequency == readIRFrequency()){
    delay(10);
    Serial.println("readIR Frequency: ");
    Serial.print(readIRFrequency());
    Serial.println("Last Frequency: ");
    Serial.print(lastFrequency);
    if (lastFrequency != readIRFrequency()){
      lastFrequency = readIRFrequency();
      Serial.print("Broken Loop!!!!!!!!");
      break; 
    }
    lastFrequency = readIRFrequency();
  }
 
   Serial.print("====================================Hit Border 2 =======================================");
   drive(100,-100);
  //Grab elapsed time
  unsigned long elapsedTime = millis() - currentTime ;
  
  //Turn back to the center of the signal

driveValue = -driveValue;
  drive(driveValue,driveValue);
  delay(elapsedTime/2);
  drive(0,0);
*/ //CODE FOR CORRECTION --- SCREWED IF TURNS 360

}

 //DELETE FOR FULL SWEEP OF IR!!!!!!!
void park_setup(){
  // Initialize the sensor on pin 12 as an input
  pinMode(IRSensorInputPin,INPUT);
  
  // Initialize the timer interrupt that decodes the IR beacon signal
  //attachCoreTimerService(Decoder);
} 



bool park_this_baby(){
  bool parkingSucceeded = false;
  bool parkOnBlue = false;
  
  if (blueStripeCount>redStripeCount){  //park on blue
    parkOnBlue = true;
    Serial.print("MUST PARK ON BLUE!!!");
  }
  else if (blueStripeCount<redStripeCount){  //park on red
    //do nothing
    Serial.print("MUST PARK ON RED!!!");
  }
  else { Serial.print("Cannot decide on color..."); 
  }

//Frequency detection: Orient car toward beacon ===========================================
  turnTowardsBeaconAlt(); //UNCOMMENT TO RUN!!!!!!!!!

      //Drive toward beacon =================================================
  bool lookingForColor = true;
      
  while (lookingForColor){
      drive_master();
      delay(50);
      
      //Check if over color =================================================
      colorDetection();
    if (isDetectingRed() || isDetectingBlue()) {
      delay(100); //drive forward to the center of the color ---> CHANGE THIS IF CAR IS TOO FAR FROM WALL!!!!
      lookingForColor = false;
      if (isDetectingRed()){
        Serial.println("Detecting Red");
        //Serial.println(color);
      }
      else Serial.println("Detecting Blue");
    }
  }  
  //Sitting on color, check if it is correct
  
 //================================================================= Color parking ===========================================================================
  colorDetection();
  if (parkOnBlue && isDetectingBlue()){
    //finished with project
    parkingSucceeded = true;
    Serial.println("Parking Completed");
  }
  else if (!parkOnBlue && isDetectingRed()){
    //finished with project
    parkingSucceeded = true;
    Serial.println("Parking Completed");
  }
  else {
       if (isDetectingRed()){
        delay(100);
        drive(100,100); //turn right
        delay(400); //almost turn 90
        drive(0,0); 
       }
       else if (isDetectingBlue()){
         delay(100);
        drive(-100,-100); //turn left
        delay(400); //almost turn 90
        drive(0,0);
       }
      //drive(0,0);
      //new_direction(); CODE FOR LOOKING AT WALLS
      
      drive_master();
      delay(10); //Not sure why this is in there
      int currentIteration = 0; 
      int maxIteration = 50;
     
         if (parkOnBlue && isDetectingBlue()){
          //finished with project
          parkingSucceeded = true;
          Serial.println("Parking Completed");
        }
        else if (!parkOnBlue && isDetectingRed()){
          //finished with project
          parkingSucceeded = true;
          Serial.println("Parking Completed");
        }
     
      while (currentIteration <= maxIteration){
        colorDetection();
        currentIteration++;
        Serial.print("Current Iteration:");
        Serial.print(currentIteration);
        
        if (parkOnBlue && isDetectingBlue()){
          //finished with project
          delay(250);
          parkingSucceeded = true;
          Serial.println("Parking Completed");
        }
        else if (!parkOnBlue && isDetectingRed()){
          //finished with project
          delay(250);
          parkingSucceeded = true;
          Serial.println("Parking Completed");
        }
        
         if ((currentIteration >= maxIteration) || parkingSucceeded){
          //did not find correct color: parking failed
          if (!parkingSucceeded){
          Serial.println("Parking Failed");
          }
         break; 
        } 
      }
    }

    
 return parkingSucceeded;
}


