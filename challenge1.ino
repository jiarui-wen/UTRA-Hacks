// Define color sensor pins
#define S0 7
#define S1 8
#define S2 9
#define S3 10
#define sensorOut 11
//#define OE_PIN 6


// Motor A connections
//int enA = 9;
int in1 = 2;
int in2 = 3;
// Motor B connections
//int enB = 3;
int in3 = 4;
int in4 = 5;


//servo
#include <Servo.h>
Servo claw;




// Variables for Color Pulse Width Measurements
int redPW = 0;
int greenPW = 0;
int bluePW = 0;


//Starting threshold values that are created when the car first touches the surface
const int redThresh = 15;
const int blueThresh = 50;
const int greenThresh = 50;


//90 deg turn fraction of power (proportional)
const int turnConst = 568;


//Variables to be observed
int color;
bool first_detect_flag=false;
int prevColor= 0;
int currColor = 0;
//int count=0;
int loopCounter = 0;
bool Ended = false;
int getColour();
int getRedPW();
int getBluePW();
int getGreenPW();
void stopMotors();
void goBackwards();
void goForward();
void turnRight(int angle);
void turnLeft(int angle);
void clawOpen();
void clawClose();








void setup() {
 // put your setup code here, to run once:


 // Colour Sensor Setup - Set S0 - S3 as outputs
 pinMode(S0, OUTPUT);
 pinMode(S1, OUTPUT);
 pinMode(S2, OUTPUT);
 pinMode(S3, OUTPUT);
 //pinMode(OE_PIN, OUTPUT);


 // Set Pulse Width scaling to 20%
 digitalWrite(S0,HIGH);
 digitalWrite(S1,LOW);
 //digitalWrite(OE_PIN, LOW);


 // Set Sensor output as input
 pinMode(sensorOut, INPUT);


 //Motor Setup
 // Set all the motor control pins to outputs
 //pinMode(enA, OUTPUT);
 //pinMode(enB, OUTPUT);
 pinMode(in1, OUTPUT);
 pinMode(in2, OUTPUT);
 pinMode(in3, OUTPUT);
 pinMode(in4, OUTPUT);


   // Turn off motors - Initial state
 digitalWrite(in1, LOW);
 digitalWrite(in2, LOW);
 digitalWrite(in3, LOW);
 digitalWrite(in4, LOW);


 //US Setup
 Serial.begin (9600);


 claw.attach(6);
 //claw.write(0);
  delay(3000);


 //clawClose();
 //delay(1000);




}




void loop() {
 // put your main code here, to run repeatedly:
 color = getColour();
 if(!Ended){


    if(!first_detect_flag){
     if(color==1 || color==2 || color==3){
       first_detect_flag= true;
       currColor = color;
       loopCounter= loopCounter+1;
     }
     else{
       goForward();
     }
   } 


   else{


     if (color == 0 || color == 4) {
       turnLeft(160);
       goForward();
       delay(100);
     }
     else if(color == currColor){
       goForward();
       delay(100);
       stopMotors();
       delay(100);
     }
     else if(color == prevColor){
       goBackwards();
       delay(200);
       stopMotors();
       turnLeft(300);




       goForward();
       delay(150);
       stopMotors();
       delay(100);
     }
     //NEW COLOR
     else{
       goForward();
       delay(200);
       stopMotors();
       delay(200);
       prevColor = currColor;
       currColor = color;
       loopCounter= loopCounter+1;


       if (loopCounter >= 5){
         goBackwards();
         delay(250);
         stopMotors();
         clawOpen();
         delay(1000);
         while (getColour()!=0 || getColour() !=4){
           goBackwards();
         }
         stopMotors();
         Ended = true;
       }
     }
   }
   }
 }


void clawClose(){
 for(int i=1; i<=65; i++)
   claw.write(i);
   delay(200);
}


void clawOpen(){
 claw.write(0);
}


void goForward(){
   // Turn on motor A & B, go fowrwards
   digitalWrite(in1, LOW);
   digitalWrite(in2, HIGH);
   digitalWrite(in3, LOW);
   digitalWrite(in4, HIGH);
}


void goBackwards(){
   // Turn on motor A & B, go fowrwards
   digitalWrite(in1, HIGH);
   digitalWrite(in2, LOW);
   digitalWrite(in3, HIGH);
   digitalWrite(in4, LOW);
}


void stopMotors(){
 // Turn off motors
   digitalWrite(in1, LOW);
   digitalWrite(in2, LOW);
   digitalWrite(in3, LOW);
   digitalWrite(in4, LOW);
}




int getColour(){ // Returns colour sensed. 0 - black, 1 - red, 2 - green, 3 - blue
 // Read Red Pulse Width
 redPW = getRedPW();
 // Delay to stabilize sensor
 delay(150);


 // Read Green Pulse Width
 greenPW = getGreenPW();
 // Delay to stabilize sensor
 delay(150);


 // Read Blue Pulse Width
 bluePW = getBluePW();
 // Delay to stabilize sensor
 delay(150);


 //The second the car recieves power it takes into account the black surface and notes down the threshold colors.
 //Multiplied by 0.7 and 0.8 because that is roughly the ratio it falls by when a color is detected
 /*if (!callibrated){
   redThresh = redPW;
   blueThresh = bluePW;
   greenThresh = greenPW;
   callibrated = true;
 }*/


 if (redPW < redThresh){
   if (bluePW < 0.65 * blueThresh && greenPW < 0.65 * greenThresh){
       //Serial.print("White detected - ");
       //Serial.print("(" + String(redPW) + ", " + String(greenPW) + ", " + String(bluePW) + ")");
       return 4;
   }
   else{
     //Serial.print("Red Detected - ");
     //Serial.print("(" + String(redPW) + ", " + String(greenPW) + ", " + String(bluePW) + ")");
     return 1;
   }
 }


 else if (bluePW < blueThresh){
   //Serial.print("Blue Detected - ");
   return 3;
 }


 else if (greenPW < greenThresh){
   //Serial.print("Green Detected - ");
   //Serial.print("(" + String(redPW) + ", " + String(greenPW) + ", " + String(bluePW) + ")");
   return 2;
 }
  //Serial.print("(" + String(redPW) + ", " + String(greenPW) + ", " + String(bluePW) + ")");


 return 0;
 }




// Function to read Red Pulse Widths
int getRedPW() {
 // Set sensor to read Red only
 digitalWrite(S2,LOW);
 digitalWrite(S3,LOW);
 // Define integer to represent Pulse Width
 int PW;
 // Read the output Pulse Width
 PW = pulseIn(sensorOut, LOW);
 // Return the value
 return PW;
}


// Function to read Green Pulse Widths
int getGreenPW() {
 // Set sensor to read Green only
 digitalWrite(S2,HIGH);
 digitalWrite(S3,HIGH);
 // Define integer to represent Pulse Width
 int PW;
 // Read the output Pulse Width
 PW = pulseIn(sensorOut, LOW);
 // Return the value
 return PW;
}


// Function to read Blue Pulse Widths
int getBluePW() {
 // Set sensor to read Blue only
 digitalWrite(S2,LOW);
 digitalWrite(S3,HIGH);
 // Define integer to represent Pulse Width
 int PW;
 // Read the output Pulse Width
 PW = pulseIn(sensorOut, LOW);
 // Return the value
 return PW;
}


void turnLeft(int a){ // Sets motors to turn left. a is angle in degrees
   int d = a * 7 / 9;
     // Turn on motor A & B, go Left
   digitalWrite(in1, HIGH);
   digitalWrite(in2, LOW);
   digitalWrite(in3, LOW);
   digitalWrite(in4, HIGH);
   delay(d);


   // Turn off motors
   digitalWrite(in1, LOW);
   digitalWrite(in2, LOW);
   digitalWrite(in3, LOW);
   digitalWrite(in4, LOW);




}


void turnRight(int a){ // Sets motors to turn left. a is angle in degrees
   int d = a * 7 / 9;
     // Turn on motor A & B, go Right
   digitalWrite(in1, LOW);
   digitalWrite(in2, HIGH);
   digitalWrite(in3, HIGH);
   digitalWrite(in4, LOW);
   delay(d);


   // Turn off motors
   digitalWrite(in1, LOW);
   digitalWrite(in2, LOW);
   digitalWrite(in3, LOW);
   digitalWrite(in4, LOW);
}





