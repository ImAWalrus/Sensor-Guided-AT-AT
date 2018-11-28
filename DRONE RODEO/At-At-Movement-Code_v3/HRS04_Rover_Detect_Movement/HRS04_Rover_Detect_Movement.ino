//-------------------------------------------------//
// Authors: Michael Gonzales, Jesse Ibarra         //            
// Date: 1-6-17                                    //
//                                                 //
// Purpose: Simple rover object detection          //
// logic using a HC-SR04 sensor, adafruit          //
// motor driver v1 board, and Arduino Uno          //
//                                                 //
// Dependencies: Adafruit motor library v1         //
//-------------------------------------------------// 

//Imported headers, and static pin declarations
#include <AFMotor.h>
#define echoPin 2
#define trigPin 13

//Initialization variables (DO NOT CHANGE)
long duration;
long distance;

//Stop and Turn variables (CAN BE ADJUSTED)
long backup_interval = 500;     //milliseconds
long turn_interval = 1500;      //milliseconds
long acceleration_delay = 1;    //milliseconds
long decceleration_delay = 1;   //microseconds
long distance_polling_delay = 1;//milliseconds
int object_stop_distance = 80;  //Centimeters

//Motor variables (CAN BE ADJUSTED)
int max_motor_speed = 235;      //MAX 255
AF_DCMotor right_motor_bank_1(1);
AF_DCMotor right_motor_bank_2(2);
AF_DCMotor left_motor_bank_1(3);
AF_DCMotor left_motor_bank_2(4);

//------------------------//
// Arduino Setup Function //
//------------------------//
void setup() {
  
  Serial.begin(9600);

  //Set sensor digital pins           
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);

  //Run sensor input to clear initial false positive values
  int i;
  for(i=0;i<50;i++){
    int check_sensor_val = checkfrontdistance();
  } 

  //Gradually speed up motors and go forward
  speed_up_forward();
  
}
//---------------------------------//
// Rover Speed Up Forward Function //
//---------------------------------//
void speed_up_forward(){

  //Set all motors forward
  right_motor_bank_1.run(FORWARD);
  right_motor_bank_2.run(FORWARD);
  left_motor_bank_1.run(FORWARD);
  left_motor_bank_2.run(FORWARD);
  Serial.println("FORWARD");
  
  //Call Acceleration function
  accelerate();
}
//----------------------------------//
// Rover Speed Up Backward Function //
//----------------------------------//
void speed_up_backward(){

  //Set all motors backward
  right_motor_bank_1.run(BACKWARD);
  right_motor_bank_2.run(BACKWARD);
  left_motor_bank_1.run(BACKWARD);
  left_motor_bank_2.run(BACKWARD);
  Serial.println("BACKWARD");
  
  //Call Acceleration function
  accelerate();
}
//---------------------//
// Rover Turn Function //
//---------------------//
void speed_up_turn(){

  right_motor_bank_1.run(FORWARD);
  right_motor_bank_2.run(FORWARD);
  left_motor_bank_1.run(BACKWARD);
  left_motor_bank_2.run(BACKWARD);
  Serial.println("Turn");

  //Call Acceleration function
  accelerate();
  
}
//---------------------//
// Accelerate Function //
//---------------------//
void accelerate(){
  
  //Slowly speed up motors until max speed is reached.
  uint8_t i;
  for (i=0; i<max_motor_speed; i++) {
    
    right_motor_bank_1.setSpeed(i);
    right_motor_bank_2.setSpeed(i);
    
    left_motor_bank_1.setSpeed(i);
    left_motor_bank_2.setSpeed(i);

    delay(acceleration_delay);
  }  
}
//----------------------//
// Deccelerate Function //
//----------------------//
void deccelerate(){
  
  //Slowly speed up motors until max speed is reached.
  uint8_t i;
  for (i=max_motor_speed; i!=0; i--){
    
    right_motor_bank_1.setSpeed(i);
    right_motor_bank_2.setSpeed(i);
    
    left_motor_bank_1.setSpeed(i);
    left_motor_bank_2.setSpeed(i);

    delayMicroseconds(decceleration_delay);
  }  
}
//---------------------//
// Rover Stop Function //
//---------------------//
void stopall(){

  //Slow down motors to a halt.
  deccelerate();

  //Release all Motors
  right_motor_bank_1.run(RELEASE);
  right_motor_bank_2.run(RELEASE);
  left_motor_bank_1.run(RELEASE);
  left_motor_bank_2.run(RELEASE);
  
}
//---------------------------------------//
// Run the turn procedure, backup until  //
// time backup_interval is met, and then //
// turn until turn_interval is met.      //
//---------------------------------------//
void run_turn_procedure(){

  //Slow down to a stop
  stopall();

  //Backup for a predefined amount of time plus motor acceleration time then stop.
  speed_up_backward();
  delay(backup_interval);

  //Slow down to a stop
  stopall();

  //Turn for a predefined amount of time plus motor acceleration time then stop.
  speed_up_turn();
  delay(turn_interval);

  //Slow down to a stop
  stopall();

  //Gradually speed up motors and go forward once more
  speed_up_forward();
  
}
//--------------------------------//
// Use HC-SR04 to detect distance //
// in centimeters.                //
//--------------------------------//
int checkfrontdistance(){

 digitalWrite(trigPin, LOW);
 delayMicroseconds(100);

 digitalWrite(trigPin, HIGH);
 delayMicroseconds(100);
    
 duration = pulseIn(echoPin, HIGH);
 distance = duration*0.034/2;

 return distance;  
}
//-------------------------------//
// Check for front object within //
// object_stop_distance          //
//-------------------------------//
void front_obj_detect(){

  if(checkfrontdistance() <= object_stop_distance){
     run_turn_procedure();
  }
}
//-------------------//
// Arduino MAIN Loop //
//-------------------//
void loop() {

  //Always look for an object in path
  front_obj_detect();
  delay(distance_polling_delay);
  
}
