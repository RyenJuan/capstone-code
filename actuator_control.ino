// Gantry Control Code - Climate Controlled Box
// CAPSTONE 2, SPRING 2024
// kokoros.d [TEAM 11]

///////////////// ~~~  READ ME  ~~~ ///////////////////
// OPEN SERIAL MONITOR: TOOLS --> SERIAL MONITOR     //
// THIS WILL BE USED FOR USER INPUT AND UPDATES      //
// SHOULD BE SET TO 'Newline' AND '9600 baude rate'  //
///////////////////////////////////////////////////////

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// EDIT THE TARGET POSITIONS BELOW (X AND Y COORDINATES IN MM) //


float x_pos = 30; // [mm] SHOULD BE <= 65 MM
float y_pos = 25; // [mm] SHOULD BE <= 40 MM


// EDIT THE TARGET POSITIONS ABOVE (X AND Y COORDINATES IN MM) //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//Actuator status will be set to 1 when complete
int actuator_status = 0;

// Define the number of steps per revolution for the 1.8deg phase NEMA17 stepper motors
const float stepsPerRev = 200;

// Define ball screw lead
const int screwLead = 10; // in mm

// Determine linear distance traveled per step
float disPerStep = screwLead/stepsPerRev; //in mm

// Divide target positions into total steps required
int x_stepsToTarget = round((x_pos)/disPerStep);
int y_stepsToTarget = round((y_pos)/disPerStep);

// Define the pins for the stepper motors
#define X_STEP_PIN 2
#define X_DIR_PIN 5
#define X_LIMIT_POS_PIN 9
#define X_LIMIT_NEG_PIN 9
#define Y_STEP_PIN 3
#define Y_DIR_PIN 6
#define Y_LIMIT_POS_PIN 10
#define Y_LIMIT_NEG_PIN 10
#define ENABLE_PIN 8

// Define the maximum speed each axis
//const float maxSpeedX = 2; // rpm
//const float maxSpeedY = 2; // rpm

// Determine pulse frequency based on speed
//float pulseX = ((1/(6*maxSpeedX/1.8))*1000000)/2; // pulse in microseconds [um]
//float pulseY = ((1/(6*maxSpeedY/1.8))*1000000)/2; // pulse in microseconds [um]

float speedX = 50000; // pulse time in microseconds
float speedY = 50000;

// Define constants for motor direction
#define CW LOW // Clockwise rotation
#define CCW HIGH // Counter-clockwise rotation

void setup() 
{
  // Enable the shield
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, LOW); // Enable pin is active LOW
  
  Serial.begin(9600); // Initialize serial communication at 9600 baud rate
  Serial.println("Initializing variables...");
  Serial.println("You entered X position [mm] and Y position [mm]:");
  Serial.println(x_pos);
  Serial.println(y_pos);
  
  // Make sure the entered positions are not out of bounds
  if(x_pos > 65)
  {
     Serial.println("Out of bounds X POS was entered, setting to maximum allowable...");
     // set to max acceptable limit in case user does not pause/reupload code
     delay(30000);
     //print that code will continue with overridden position settings 
     x_pos = 60;
  }
  if(y_pos > 40)
  {
     Serial.println("Out of bounds Y POS was entered, setting to maximum allowable...");
     // set to max acceptable limit in case user does not pause/reupload code
     delay(30000);
     //print that code will continue with overridden position settings 
     y_pos = 35;
  }

  // Initialize the limit switches
  pinMode(X_LIMIT_POS_PIN, INPUT_PULLUP);
  pinMode(X_LIMIT_NEG_PIN, INPUT_PULLUP);
  pinMode(Y_LIMIT_POS_PIN, INPUT_PULLUP);
  pinMode(Y_LIMIT_NEG_PIN, INPUT_PULLUP);

  // Initialize step and direction pins
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  
  delay(3000);

  int curr = Serial.available();
  delay(50);
  Serial.println("PRESS ENTER TO BEGIN HOMING PROCESS:");
    
      // Wait for user input
      while (Serial.available() == curr) 
      {
        delay(100); // Wait until something is received
      }
  
  // Reinitialize to the initial position
  homeX();
  homeY();
  
}

void loop() 
{
    while(actuator_status == 0)
    {
      Serial.println("Preparing to move to position...");
      int actuator_status_x = 0;
      int actuator_status_y = 0;
      
      // Move the x cart into position
      while(actuator_status_x == 0)
      {
        while(digitalRead(X_LIMIT_POS_PIN))
        {
          Serial.println("Moving to x position...");
          moveCartX(x_stepsToTarget, CW);
          break;
        }
        actuator_status_x = 1;
      }
    
      // Move the y cart into position
      while(actuator_status_y == 0)
      {
        while(digitalRead(Y_LIMIT_POS_PIN))
        {
          Serial.println("Moving to y position...");
          moveCartY(y_stepsToTarget, CW);
          break;
        }
        actuator_status_y = 1;
      }
    
      // Prompt the user to input something
      Serial.println("WHEN DISPENSING IS COMPLETE, press ENTER:");
      delay(100);
    
      // Wait for user input
      int curr = Serial.available();
      delay(50);
      while (Serial.available() == curr) 
      {
        delay(100); // Wait until something new is received
      }
    
      delay(500);
      
      // Re-home each actuator
      Serial.println("X-axis re-homing...");
      homeX();
      
      delay(500);

      Serial.println("Y-axis re-homing...");
      homeY();
  
      // Set actuator status to complete
      actuator_status = 1;
      
      Serial.println("DISPENSING PROCESS COMPLETE!");
     }
     
     delay(10000);
 }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Moves the x cart
void moveCartX(int stepsToTarget, int dir)
{
      // Set the direction of rotation
      digitalWrite(X_DIR_PIN, dir);
      for (int i = 0; i < stepsToTarget; i++) 
      {
        digitalWrite(X_STEP_PIN, HIGH);
        delayMicroseconds(speedX); // Adjust delay as needed for your motor and driver
        digitalWrite(X_STEP_PIN, LOW);
        delayMicroseconds(speedX); // Adjust delay as needed for your motor and driver
        if(!digitalRead(X_LIMIT_POS_PIN))
        {
          Serial.println("X limit switch was hit.");
          break;
        }
      }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//moves the y cart
void moveCartY(int stepsToTarget, int dir)
{
        // Set the direction of rotation
        digitalWrite(Y_DIR_PIN, dir);
        for (int i = 0; i < stepsToTarget; i++) 
        {
          digitalWrite(Y_STEP_PIN, HIGH);
          delayMicroseconds(speedY); // Adjust delay as needed for your motor and driver
          digitalWrite(Y_STEP_PIN, LOW);
          delayMicroseconds(speedY); // Adjust delay as needed for your motor and driver
          if(!digitalRead(Y_LIMIT_POS_PIN))
          {
            Serial.println("Y limit switch was hit.");
            break;
          }
        }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Homes the x cart
void homeX() 
{
  Serial.println("Homing X...");
  while(digitalRead(X_LIMIT_NEG_PIN))
      {
          // Set the direction of rotation
          digitalWrite(X_DIR_PIN, CCW);
          
          digitalWrite(X_STEP_PIN, HIGH);
          delayMicroseconds(speedX); // Adjust delay as needed for your motor and driver
          digitalWrite(X_STEP_PIN, LOW);
          delayMicroseconds(speedX); // Adjust delay as needed for your motor and driver
      }
   // Moves cart forward to disengage limit switch
   delay(500);
   for (int i = 0; i < 35; i++) {
     digitalWrite(X_DIR_PIN, CW);
          
     digitalWrite(X_STEP_PIN, HIGH);
     delayMicroseconds(speedX); // Adjust delay as needed for your motor and driver
     digitalWrite(X_STEP_PIN, LOW);
     delayMicroseconds(speedX); // Adjust delay as needed for your motor and driver
   }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Homes the y cart
void homeY()
{
  Serial.println("Homing Y...");
  while(digitalRead(Y_LIMIT_NEG_PIN))
      {
          // Set the direction of rotation
          digitalWrite(Y_DIR_PIN, CCW);
          
          digitalWrite(Y_STEP_PIN, HIGH);
          delayMicroseconds(speedY); // Adjust delay as needed for your motor and driver
          digitalWrite(Y_STEP_PIN, LOW);
          delayMicroseconds(speedY); // Adjust delay as needed for your motor and driver
      }
  // Moves cart forward to disengage limit switch
  delay(500);
  for (int i = 0; i < 35; i++) {
     digitalWrite(Y_DIR_PIN, CW);
          
     digitalWrite(Y_STEP_PIN, HIGH);
     delayMicroseconds(speedY); // Adjust delay as needed for your motor and driver
     digitalWrite(Y_STEP_PIN, LOW);
     delayMicroseconds(speedY); // Adjust delay as needed for your motor and driver
   }
}
