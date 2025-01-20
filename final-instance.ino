#include <Servo.h>

Servo myservo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position
bool sweepstarted=true;
const int clockwisePosition = 100;   // Servo position for clockwise rotation
const int counterClockwisePosition = 80; // Servo position for counterclockwise rotation

//KEYPAD
#include <Keypad.h>
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {2, 3, 4, 5}; 
byte colPins[COLS] = {6,7,8}; 

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );   

// Variables for user input
String hoursInput = "";
String minutesInput = "";
String secondsInput = "";
int hours = 0, minutes = 0, seconds = 0;
unsigned long interval = 0;           // Interval in milliseconds
unsigned long previousMillis = 0;      // Tracks last execution time
bool intervalSet = false;    
bool timerReached = false;           // Flag to check if timer has reached interval
bool servoMovingClockwise = false;   // Flag to track servo direction          

enum InputState { HOURS, MINUTES, SECONDS };
InputState inputState = HOURS;         // Tracks which part we're entering


//LOAD SENSOR
#include "HX711.h"

#define calibration_factor 417050.0 //This value is obtained using the Calibration sketch

#define LOADCELL_DOUT_PIN  9
#define LOADCELL_SCK_PIN  10
float loadThreshold = 0.050;


HX711 scale;


//ULTRASONIC SENSOR
int led=13; //red Led
int trigpin=12;
int echopin=11;//US sensors
long duration;
int distance;

void setup(){
  Serial.begin(9600);
  Serial.println("Enter hours (press # to confirm):");

  myservo.attach(A1); //servo attached on pin 1
  


  pinMode(trigpin,OUTPUT);
  pinMode(echopin,INPUT);
  pinMode(led,OUTPUT);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0

  Serial.println("Readings:");

}
void loop(){
  //US
  myservo.write(90);
  digitalWrite(trigpin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin,LOW);
  duration=pulseIn(echopin,HIGH);
  distance=0.034*duration/2;
  Serial.print(distance);
  Serial.println("cm");
  if (distance>5 && distance<150){
    digitalWrite(led,HIGH);
    
  }
  if (distance<5){
    digitalWrite(led,LOW);
    
  }

  //keypad
  char key = keypad.getKey();
  if (key) {
    if (key == '#') {  // Confirm input for each part
      if (inputState == HOURS && hoursInput != "") { 
        hours = hoursInput.toInt();
        Serial.print("Hours set to: ");
        Serial.println(hours);
        Serial.println("Enter minutes (press # to confirm):");
        hoursInput = "";  // Clear input and move to minutes
        inputState = MINUTES;
      } 
      else if (inputState == MINUTES && minutesInput != "") {  
        minutes = minutesInput.toInt();
        Serial.print("Minutes set to: ");
        Serial.println(minutes);
        Serial.println("Enter seconds (press # to confirm):");
        minutesInput = "";  // Clear input and move to seconds
        inputState = SECONDS;
      }
      else if (inputState == SECONDS && secondsInput != "") { 
        seconds = secondsInput.toInt();
        Serial.print("Seconds set to: ");
        Serial.println(seconds);

        // Calculate total interval in milliseconds
        interval = (hours * 3600000UL) + (minutes * 60000UL) + (seconds * 1000UL);
        previousMillis = millis();  // Initialize the timer
        intervalSet = true;  // Flag that the interval is set

        Serial.print("Interval set to: ");
        Serial.print(hours);
        Serial.print(" hours, ");
        Serial.print(minutes);
        Serial.print(" minutes, ");
        Serial.print(seconds);
        Serial.println(" seconds.");

       
        hoursInput = "";
        minutesInput = "";
        secondsInput = "";
        inputState = HOURS; 
      }
    } 
    else if (key == '*') {  
      hoursInput = "";
      minutesInput = "";
      secondsInput = "";
      inputState = HOURS;  
      Serial.println("Input cleared. Enter hours (press # to confirm):");
    } 
    else {  
      if (inputState == HOURS) {
        hoursInput += key;
        Serial.print("Hours input: ");
        Serial.println(hoursInput);
      } else if (inputState == MINUTES) {
        minutesInput += key;
        Serial.print("Minutes input: ");
        Serial.println(minutesInput);
      } else if (inputState == SECONDS) {
        secondsInput += key;
        Serial.print("Seconds input: ");
        Serial.println(secondsInput);
      }
    }
  }
  if (intervalSet && millis() - previousMillis >= interval) {
    previousMillis = millis();      
    timerReached = true;            
    servoMovingClockwise = true;     
    Serial.println("Interval reached! Starting servo clockwise.");
  }

  //Load
  Serial.print("Reading: ");
  float loadreading=scale.get_units();
  Serial.print(loadreading,3); //scale.get_units() returns a float
  Serial.print(" kg"); //You can change this to kg but you'll need to refactor the calibration_factor
  Serial.println();
  if (timerReached) {
    if (servoMovingClockwise && sweepstarted) {
      myservo.write(clockwisePosition); 
      Serial.println("Servo moving clockwise...");
      delay(650);
      myservo.write(90);
      sweepstarted=false;
    }
      // Check if the load reading has reached the threshold
    else if (loadreading > loadThreshold && !sweepstarted) {
      Serial.println("Threshold reached! Reversing servo direction.");
      servoMovingClockwise = false;
      sweepstarted=true;
      }
    
    else if (!servoMovingClockwise) {
      myservo.write(counterClockwisePosition); 
      Serial.println("Servo moving counterclockwise...");
      delay(450);
      timerReached=false;
      sweepstarted=true;
    }
  }
  //add any other continuous tasks here
  //servo
}