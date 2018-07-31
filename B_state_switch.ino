void drive(int lSpeed, int rSpeed) {
    lPulseWidth=map(lSpeed, -100, 100, 1300, 1700);
    lMotor.writeMicroseconds(lPulseWidth);
    rPulseWidth=map(rSpeed, -100, 100, 1300, 1700);
    rMotor.writeMicroseconds(rPulseWidth);
    delay(10);
}
void switch_setup() {
  Serial.println("STARTING SETUP");
  state = main;
  lMotor.attach(30);
  rMotor.attach(31);
}

void check_switches(){
  Serial.println("check_switches()");
  LEFT = digitalRead(LEFTpin);
  RIGHT = digitalRead(RIGHTpin);
  LEFTswitch = LEFT;
  RIGHTswitch = RIGHT;

  if (LEFTswitch==false)
  {
    state=left;
    currentState = 2;
  }
  else if (RIGHTswitch==false) 
  {
    state=right;
    currentState = 2;
  }
  
  return;
}

/*
void drive_master(){
   //runMainState();
   drive(100,-100);
   return;

  else if(state == left)
  {
    //activelyAvoidingObstacles = true;
    runLeftState(0);
  }
  else if (state == right)
  {
    //activelyAvoidingObstacles = true;
    runRightState(0);
  } 

  switch (state)
  {
    case(main):
      runMainState();
      drive(100,-100);
      break; 
    case(left):
    activelyAvoidingObstacles = true;
      drive(0,100);
      runLeftState(50);
      break;
    case(right):
    activelyAvoidingObstacles = true;
      drive(-100,0); 
      runRightState(50);
      break;
    default:
      Serial.println("ERROR - UNKNOWN STATE");
      break;
  }
  //delay(100); // delay between state changes to allow time for the switch to be released 
}
*/



// Begin the individal state functions
void runMainState()
{
  Serial.println("Main State");
  LEFT = digitalRead(LEFTpin);
  RIGHT = digitalRead(RIGHTpin);
  LEFTswitch = LEFT;
  RIGHTswitch = RIGHT;

  //Now we make the decision to switch between states
  if (LEFTswitch==false) //ENTER THE NECESSARY CONDITION (A BOOLEAN VARIABLE, IF TRUE THEN EXECUTE) FOR SWITCHING TO THE LEFT STATE
  {
    state=left;
    //THE CODE TO SWITCH TO LEFT STATE
  }
  else if (RIGHTswitch==false) //ENTER THE NECESSARY CONDITION (A BOOLEAN VARIABLE, IF TRUE THEN EXECUTE) FOR SWITCHING TO THE RIGHT STATE 
  {
    state=right;
    //THE CODE TO SWITCH TO RIGHT STATE
  }
  else 
  {
    state=main;
    //THE CODE THAT DECIDES WHAT HAPPENS IF NONE OF THE ABOVE CONDITIONS ARE MET
  }
}

void runLeftState()
{
  //counter = 0;
  bool exit = false;
  Serial.println("Left State");
  state = main;
  
  while(exit == false)
  {
    LEFTswitch = digitalRead(LEFTpin);
    drive(0,100);
    if(LEFTswitch == true)
    {
      exit = true;
    }
  }
  /*
  while(counter < t){
    RIGHTswitch = digitalRead(RIGHTpin);
    counter = counter + 1;
    Serial.println(counter);
    if (RIGHTswitch==false)
    {
      state=right;
      break;
    }
  }
  */
}

void runRightState()
{
  //REPEAT THE runMainState() CODE HERE, MAKING THE APPROPRIATE MODIFICATIONS
  //counter = 0;
  bool exit = false;
  state = main;
  Serial.println("Right State");

  while(exit == false)
  {
    RIGHTswitch = digitalRead(RIGHTpin);
    //we continue driving to the RIGHT until our switch is turned off (on?)
    drive(-100,0);
    if(RIGHTswitch == true)
    {
      exit = true;
    }
  }
  /*
  state = main;
  while(counter < t){
    LEFTswitch = digitalRead(LEFTpin);
    counter = counter + 1;
    Serial.println(counter);
    if (LEFTswitch==false)
    {
      state=left;
      break;
    }
  }
  */
}


