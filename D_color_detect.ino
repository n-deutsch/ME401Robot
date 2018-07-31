bool isDetectingRed(){
  return color[0] >= 50 /*&& color[1] < 30*/ && color[2] < 50;
  Serial.print("Red is Detected!");
}

bool isDetectingBlue(){
  return color[2] >= 50 && color[0] < 50 /*&& color[1] < 30*/;
    Serial.print("Blue is Detected!");
}

bool isDetectingBlack(){
  return color[0] <= 30 /*&& color[1] <= 70*/ && color[2] <= 30;
}

bool isDetectingNothing(){
  return color[0] >= 50 /*&& color[1] <= 70*/ && color[2] >= 50;
}

int colorDetection() {
// Red Light
    digitalWrite(rPin,LOW);
    digitalWrite(gPin,HIGH);
    digitalWrite(bPin,HIGH);
    delay(300); //wait for the photresistor value to settle
    sensorValue = analogRead(sensorPin); // read the photoresistor value
    color[0]=sensorValue; // record the red reading
    color[0]=constrain(sensorValue,minwhite[0],maxblack[0]); //constrain the reading such that it is between the white and black values
    color[0]=map(color[0],maxblack[0],minwhite[0],0,100); // map the reading between 0 and 100 such that black is 0, 100 is white
    Serial.print("Red: ");Serial.print(color[0]);Serial.print(" "); // output the reading
    
   /*
  //Green Light
    digitalWrite(gPin,LOW);
    digitalWrite(rPin,HIGH);
    digitalWrite(bPin,HIGH);
    delay(100);
    sensorValue = analogRead(sensorPin); 
    color[1]=sensorValue;
    color[1]=constrain(sensorValue,minwhite[1],maxblack[1]);
    color[1]=map(color[1],maxblack[1],minwhite[1],0,100);
    Serial.print("Green: ");Serial.print(color[1]);Serial.print(" ");
    */
    
//Blue Light
    digitalWrite(bPin,LOW); //This is HIGH due to wire problem
    digitalWrite(gPin,HIGH);  //This is LOW due to wire problem
    digitalWrite(rPin,HIGH);
    delay(100);
    sensorValue = analogRead(sensorPin); 
    color[2]=sensorValue;
    color[2]=constrain(sensorValue,minwhite[2],maxblack[2]);
    color[2]=map(color[2],maxblack[2],minwhite[2],0,100);
    Serial.print("Blue: ");Serial.print(color[2]);Serial.print(" ");  
    Serial.println("");
    
    
    if (isDetectingRed() && !rToggle && currentState !=10) {
      redStripeCount = redStripeCount + 1;
      rToggle = true;
      bToggle = false;
      Serial.println("Red Counted RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR");
      Serial.println("redStripeCount: ");
      Serial.print(redStripeCount);
    } 
    else if (isDetectingBlue() && !bToggle && currentState !=10) {
      blueStripeCount = blueStripeCount + 1;
      bToggle = true;
      rToggle = false;
      Serial.println("Blue Counted BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB");
      Serial.println("blueStripeCount: ");
      Serial.print(blueStripeCount);
    } 
    else if (isDetectingBlack()){
      Serial.println("Black Detected!!!!");
      return 1;
    } 
     else if (!isDetectingNothing() && (rToggle || bToggle)) {
        //do nothing
        Serial.println("Color Still Detected");
     }
     else if (isDetectingNothing()) {
      Serial.println("Nothing Detected");
      rToggle = false;
      bToggle = false; 
    }
    return 0;
}


