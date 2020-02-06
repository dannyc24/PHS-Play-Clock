/*
 * Arduino Control sketch for Show control of clock.
 * 
 */

int BRAKEMODEPIN = 9; // Pin for Brake/Coast mode -- (Defined by the Arduino Motor Shield Spec).
int DIRCONTROLPIN = 12; //Pin for Forward/Reverse Control -- (Defined by the Arduino Motor Shield Spec).
const int CONTROLPIN = 3; // Control PWM output (Defined by the Arduino Motor Shield Spec).

const int ANALOGTHRESHOLD = 700; //Analog Value to determine on state on Analog Input Pin

const int RAMPUPSPEED = 20; //Ramp-UP Speed delay in ms -- Set to 20ms. 

//Reversing this... Forward is backwards, Backwards is forwards. 
const int mFORWARD = LOW; //Constant for Direction Control Pin
const int mREVERSE = HIGH; //Constant for Direction Control Pin


int prevState = 0;
int curState = 0;

void setup() {

  //Set Digital I/O Pin state to Digital Output
  pinMode(DIRCONTROLPIN, OUTPUT);
  pinMode(BRAKEMODEPIN, OUTPUT);

  //Set Serial Output (9600 Baud)
  Serial.begin(9600);

  //Set Motor Control (Channel A) To Coast Mode
  digitalWrite(BRAKEMODEPIN,LOW);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int a2 = analogRead(2);
  int a3 = analogRead(3);

  
  
  Serial.print("A2 ");
  Serial.print(a2);
  Serial.println("\n");
  Serial.print("A3: ");
  Serial.print(a3);
  Serial.println("\n");
  
  delay(100);
  if(a2 >= ANALOGTHRESHOLD){
    curState = 1; //Set a State Pointer (Forward = 1);
    if(curState != prevState){
      Serial.println("State Change for Input 0 - Ramp On");
      if(prevState != 0){
        Serial.println("Motor off Safety");
        analogWrite(CONTROLPIN,0);
        delay(1000);
        Serial.println("Now Turn Back On");
      }
      else {
        //Do Nothing
      }
      prevState = curState;
      digitalWrite(DIRCONTROLPIN,mFORWARD); //Change Direction
      rampUp();
    }
  }
  else if (a3 >= ANALOGTHRESHOLD){
    curState = 2; // Set a State Pointer (Reverse = 2)
    
    if(curState != prevState){
      Serial.println("State Change for Input 1 - Ramp On");
      if(prevState != 0){
        Serial.println("Motor Off");
        analogWrite(CONTROLPIN,0); //Stops Motor
        delay(1000);
      }
      else {
        //Do nothing for delay 
      }
      digitalWrite(DIRCONTROLPIN,mREVERSE); //Change Direction 
      rampUp(); //Ramp speed controller up
      prevState = curState;
      
      
      
    }
  }
  else {
    curState = 0;
    if(curState != prevState){
      Serial.println("State Change --> TurnOff");
          Serial.println("Motor Off");
          analogWrite(CONTROLPIN,0);
          prevState = 0; 
          
    }

  }
  
  
  
}

void rampUp(){
  int beginsState = 0; //Assuming an Off State to ramp up from.
  int topState = 240; // This is the Top Speed to adjust
  

  
  while(beginsState < topState){
    analogWrite(CONTROLPIN,beginsState);
    Serial.println("");
    Serial.print("Ramp Up Counter --> Speed: ");
    Serial.print(beginsState);
    Serial.println();
    
    delay(RAMPUPSPEED);
    beginsState++;
    
  }
}
