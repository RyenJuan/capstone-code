// Team 11; Climate Controlled Box
// Author: Ryan Huang
// Date Written: 3/22/2024
// Last Updated: 4/9/2024
// This code is responsible for: 
//
//      > Humidifier, Dehumidifier, Cooler control
//      > Retrieving Sensor data
//      > Direct Servo Control
//
// NOTES: MODIFY THE TARGET_TEMP AND TARGET_HUMIDITY BEFORE RUNNING
//        IDENTIFY ANY TODOs AND DETERMINE IF ITS WORTH INVESTIGATING

// TODO: Redefine the PIN#s into better, more recognizable, modular names. Use Find and Replace

//***************************************************************************************************
//---------------------------------------------------------------------------------------------------
//----------------------------------- PREPROCESSING MACROS ------------------------------------------
//---------------------------------------------------------------------------------------------------
//***************************************************************************************************

#include "DHT.h"
#include "Servo.h"

#define DHTTYPE DHT22
#define ARRAY_SIZE 12

// ****************************************************
// *********** MODIFY THESE VARIABLES FIRST ***********
// ****************************************************

// Print to the serial monitor if true or allow Python to data
bool printToPython = true;

#define TARGET_TEMP 30          // MODIFY FIRST!!!
#define TARGET_HUMIDITY 50      // MODIFY FIRST!!!
#define coolingTime 10000

// ****************************************************
// *********** MODIFY THESE VARIABLES FIRST ***********
// ****************************************************

// CHAMBER SENSOR PINS
#define DHTPin2 2
#define DHTPIN5 3
#define DHTPin4 4
#define DHTPin7 7

// DEHUMIDIFIER SENSOR PINS
#define DHTPin8 8
#define DHTPin12 12

// Servo Motor Valve Control
#define PIN5 5  // Servo 1 (OUTLET)
#define PIN6 6  // Servo 2 (THREE WAY)
#define PIN13 13 // Servo 3 (THREE WAY)
#define PIN14 A0  // Servo 4 (THREE WAY)


// Chamber PC/Inline Fan Control
#define PIN9 9
#define PIN10 10

// Dehumidifier Dual Fan Control 
#define PIN11 11

// Cooler Power and Fan Control
#define PIN15 A1  // Cooler and Fan

// Mister Power and Fan Control
#define PIN16 A2  // Fan
#define PIN17 A3  // Mister


// Sensor Tolerance
// TODO: Check with Estefan on the potential accuracy and variance of the heater
#define TOL 1.5     // Tolerance for Humidifier/Dehumidfier
#define HTOL 1      // Tolerance for Heater


//***************************************************************************************************
//---------------------------------------------------------------------------------------------------
//---------------------------- INITIALIZATION AND FUNCTION PROTOTYPES -------------------------------
//---------------------------------------------------------------------------------------------------
//***************************************************************************************************


// Function Prototypes
void sense();                                             // This function sense data from all the sensors
void outletOpen(bool open = false);                       // This function controls the outlet 2-Way valve
void threewayValve(int valveNum, int pos);                // This function controls any of the 3-Way valves


// TODO: delete?
unsigned int time;

// Temperature/Humidity data from all the sensors
// TODO: Add weights to the temperature/humidity data closer to the inlets or the heater
float CHAMBER_AVG_TEMP;
float CHAMBER_AVG_HUMID;

// Initialize Chamber Temperature and Humiditiy Sensor Variables
float TC1, TC2, TC4, TC7;
float HC1, HC2, HC4, HC7;


// Initialize Dehumidifier Temperature and Humiditiy Sensor Variables
// UNUSED
float TD8, TD12;
float HD8, HD12;



// DHT Initialization
DHT dht2(DHTPin2, DHTTYPE);
DHT dht5(DHTPIN5, DHTTYPE);
DHT dht4(DHTPin4, DHTTYPE);
DHT dht7(DHTPin7, DHTTYPE);
DHT dht8(DHTPin8, DHTTYPE);
DHT dht12(DHTPin12, DHTTYPE);

// Servo initialization
Servo valve0; // Outlet Vakve
Servo valve1; 
Servo valve2;
Servo valve3;

//***************************************************************************************************
//---------------------------------------------------------------------------------------------------
//--------------------------------------- VARIABLE DECLARATION --------------------------------------
//---------------------------------------------------------------------------------------------------
//***************************************************************************************************



// NOTES: This is Dehumidifier Sensor data if needed. Currently unused for any control purposes. 
float POST_DH_TEMP = TD8;
float POST_DH_HUMID = HD8;
float PRE_DH_TEMP = TD12;
float PRE_DH_HUMID = HD12;


//***************************************************************************************************
//---------------------------------------------------------------------------------------------------
//------------------------------ SENSOR, PIN, AND SERVO ARRAY DECLARATIONS --------------------------
//---------------------------------------------------------------------------------------------------
//***************************************************************************************************

// Array containing all sensor readings 
static float results[ARRAY_SIZE] = {TC1, HC1, TC2, HC2, TC4, HC4, TC7, HC7, TD8, HD8, TD12, HD12};

// Array containing output pin references
// TODO: Update this to account for the additional pins
byte outputPins[] = {PIN5, PIN6, PIN13, PIN9, PIN10, PIN11, PIN15, PIN16, PIN17, PIN14};
byte pinCount = sizeof(outputPins) / sizeof(outputPins[0]);

// Array containing 3-Way valve motor object references 
Servo threewayRefs[] = {valve1, valve2, valve3};


//***************************************************************************************************
//---------------------------------------------------------------------------------------------------
//---------------------------------------- ALL RELEVANT FUNCTIONS -----------------------------------
//---------------------------------------------------------------------------------------------------
//***************************************************************************************************

void sense() {
  // CAUTION: Multiple sensors may fail, but they will output NaN

  // Chamber Sensor Readings
  HC1 = dht2.readHumidity();
  TC1 = dht2.readTemperature();

  HC2 = dht5.readHumidity();
  TC2 = dht5.readTemperature();

  HC4 = dht4.readHumidity();
  TC4 = dht4.readTemperature();

  HC7 = dht7.readHumidity();
  TC7 = dht7.readTemperature();

  //Dehumidifier Readings
  TD8 = dht8.readTemperature();
  HD8 = dht8.readHumidity();

  TD12 = dht12.readTemperature();
  HD12 = dht12.readHumidity();

  // CHAMBER_AVG_TEMP = (0.15*TC1 + 0.35*TC2 + 0.35*TC4 + 0.15*TC7);
  CHAMBER_AVG_TEMP = TC2;
  CHAMBER_AVG_HUMID = (HC1 + HC2 + HC4 + HC7)/4;

  // Serial.println((String) HC1 + "% | " + TC1 + "C | " + HC2 + "% | " + TC2 + "C | " + HC4 + "% | " + TC4 + "C | ");
  // Serial.println((String)"Left Wall " + TC1 + " |  RTD  " + TC2 + " | Middle Wall " + TC4 + " | Right Wall " + TC7);

  // Serial.println((String) CHAMBER_AVG_HUMID + "% | " + CHAMBER_AVG_TEMP + "C");
  if (!printToPython) {
    Serial.println((String) CHAMBER_AVG_HUMID + "% | " + CHAMBER_AVG_TEMP + "C");
  }
  else {
    Serial.println((String) CHAMBER_AVG_HUMID + " " + CHAMBER_AVG_TEMP);
  }
  delay(700);

         
}

// TODO: Determine if 0 or 90 is the open or closed position. Check with Demitri/Moti/Will
void outletOpen(bool open) {
  if (open) {
    valve0.write(90);              
  } 
  else if (!open) {
    valve0.write(0);
  }
}

// TODO: Determine which positions are the right positions for each motor
void threewayValve(int valveNum, int pos) {
  if (valveNum == 1) {
    threewayRefs[0].write(pos);
  } 
  else if (valveNum == 2) {
    threewayRefs[1].write(pos);
  }
  else if (valveNum == 3) {
    threewayRefs[2].write(pos);
  }
}

//***************************************************************************************************
//---------------------------------------- CODE BEGINS ----------------------------------------------

void setup() {
  Serial.begin(115200);

  // DHT Sensor Initialization
  pinMode(DHTPin2, INPUT);
  pinMode(DHTPIN5, INPUT);
  pinMode(DHTPin4, INPUT);
  pinMode(DHTPin7, INPUT);
  pinMode(DHTPin8, INPUT);
  pinMode(DHTPin12, INPUT);
  dht2.begin();
  dht5.begin();
  dht4.begin();
  dht7.begin();
  dht8.begin();
  dht12.begin();


  // Set logic Pins to output for Valve, Dehumidifier, Cooler, Humidifier control
  // Servo Motor Valve Control
  pinMode(PIN5, OUTPUT);
  pinMode(PIN6, OUTPUT);
  pinMode(PIN13, OUTPUT);

  // Chamber PC/Inline Fan Control
  pinMode(PIN9, OUTPUT);
  pinMode(PIN10, OUTPUT);

  // Dehumidifier Dual Fan Control 
  pinMode(PIN11, OUTPUT);

  // Cooler Fan and Power Control
  pinMode(PIN15, OUTPUT);

  // Fan and Mister Power Control
  pinMode(PIN16, OUTPUT);
  pinMode(PIN17, OUTPUT);

  valve0.attach(PIN5);
  valve1.attach(PIN6);
  valve2.attach(PIN13);
  valve3.attach(PIN14);

}


void loop() {

  // Pin Output Error Redundancy: Set all pin outputs to low before starting the control loop
  for (byte i = 0; i < pinCount; i++) {
    digitalWrite(outputPins[i], LOW);
  }


  //***************************************************************************************************
  //---------------------------------------------------------------------------------------------------
  //--------------------------------- PRESS S TO BEGIN THE LOOP ---------------------------------------
  //---------------------------------------------------------------------------------------------------
  //***************************************************************************************************
 

  // Wait to begin the control loop until keystroke 'S' is pressed.
  if (!printToPython)  {
    Serial.println("To begin the temperature and humidity control\n Switch on the heater and press S in the Serial Monitor");
  
    while (true) {    
      if (Serial.available() > 0) {
        if (Serial.read() == 'S')  {
          break;
        }
      }  
    }
  }

  sense(); // Update all the sensor measurements for the current iteration


  //***************************************************************************************************
  //---------------------------------------------------------------------------------------------------
  //--------------------------------------- COOLER SUBSYSTEM ------------------------------------------
  //------------------------------------ Wait 2 minutes to cool ---------------------------------------
  //---------------------------------------------------------------------------------------------------
  //***************************************************************************************************
 


  if (true) {
    
    // Open the cooling path valves 
    //TODO: Check the angles
    outletOpen(true);
    threewayValve(1, 0);
    threewayValve(2, 93);
    threewayValve(3, 0);

    digitalWrite(PIN15, HIGH);
    digitalWrite(PIN10, HIGH);
    digitalWrite(PIN9, LOW);
    if (!printToPython) {
      Serial.println("Status: Cooling");
    }
    


    // delay((int) coolingTime);
    //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    delay(coolingTime);

    // Close both inlet and outlet valves. 
    outletOpen(false);
    threewayValve(3, 180);

    digitalWrite(PIN15, LOW);
    digitalWrite(PIN10, LOW);
    digitalWrite(PIN9, LOW);
  }
  sense();


  //***************************************************************************************************
  //---------------------------------------------------------------------------------------------------
  //--------------------------------------- HEATER SUBSYSTEM ------------------------------------------
  //------------- Wait for the Heater and Temperature Controller to set chamber temperature------------
  //---------------------------------------------------------------------------------------------------
  //***************************************************************************************************


  // while (true) {
  // if (!printToPython) {
  //   Serial.println("Status: Heating");
  // }
  //   sense(); // Update all the sensor measurements for the current iteration

  //   // CHAMBER INLET FAN
  //   digitalWrite(PIN9, HIGH);

  //   if (abs(CHAMBER_AVG_TEMP - TARGET_TEMP) < HTOL) { 
  //     // Temperature has been overshot within the heater tolerance. Move onto Humidifier/Dehumidifier 
  //     break; 
  //   }
  // }


  //***************************************************************************************************
  //---------------------------------------------------------------------------------------------------
  //------------------------------- HUMIDIFER/DEHUMIDIFIER SUBSYSTEM ----------------------------------
  //---------- Assuming temperature is within tolerance. Begin Dehumidifying or Humidifiying ----------
  //---------------------------------------------------------------------------------------------------
  //***************************************************************************************************


  if (CHAMBER_AVG_HUMID < (TARGET_HUMIDITY - TOL)) {
    while (true) {
      sense(); // Update all the sensor measurements for the current iteration
      if (CHAMBER_AVG_HUMID < (TARGET_HUMIDITY - TOL)) {       // Humidify within tolerance
        if (!printToPython) {
          Serial.println("Status: Humidifying");
        }
        
        outletOpen(false);                       
        threewayValve(3, 100);

        // MISTER POWER AND FAN (same switch)
        digitalWrite(PIN16, HIGH);
        digitalWrite(PIN17, HIGH);

        // CHAMBER INLET
        digitalWrite(PIN9, HIGH);
      }
      else if (abs(CHAMBER_AVG_HUMID - TARGET_HUMIDITY) < TOL) {
        // Target Humidity reached within tolerance. Break the loop
        break;
      } 
    }
  }
  else {
    while(true) {
      if (CHAMBER_AVG_HUMID > (TARGET_HUMIDITY - TOL)) {      // Dehumidify within tolerance
        if (!printToPython) {
          Serial.println("Status: Dehumidifying");
        }
        outletOpen(true);
        threewayValve(1,93);
        threewayValve(2,0);
        threewayValve(3,0);

        // CHAMBER OUTLET
        digitalWrite(PIN9, HIGH);

        // CHAMBER INLET
        digitalWrite(PIN10, HIGH);     

        // BOTH DEHUMIDIFIER FANS
        digitalWrite(PIN11, HIGH);

        sense();
    
      } 
      else if (abs(CHAMBER_AVG_HUMID - TARGET_HUMIDITY) < TOL) {
        // Target Humidity reached within tolerance. Break the loop
        break;
      }
    } 
  }

  //******************************************************************************************************
  //------------------------------------------------------------------------------------------------------
  //-------------------------------- DESIRED CONDITIONS REACHED (ideally) --------------------------------
  //------------------------------ SWITCH ALL SYSTEMS OFF | CLOSE ALL VALVES -----------------------------
  //******************************************************************************************************
 

  // // Pin Output Error Redundancy: Switch all output pins off (turn off all systems)
  for (byte i = 0; i < pinCount; i++) {
    digitalWrite(outputPins[i], LOW);
  }

  // TODO: Check the valves
  outletOpen(false);
  threewayValve(3, 180);


  // CONTINUE READING TEMPERATURE AND HUMIDITY EVEN AFTER EVERYTHING SHUTS OFF

  if (!printToPython) {
    Serial.println("The temperature/humidity control is complete. Continue sensing temperature and humidity."
                "\n Press 'S' again to break the loop and restart the cycle "
                "\n If you want to change the desired temperature and humidity, modify line 32 and 33. Then upload");
  }

  while(true) {
    sense();
    if (Serial.available() > 0) {
      if (Serial.read() == 'S')  {
        break;
      }
    }  
  }
  Serial.println("Restarting the loop");
}


