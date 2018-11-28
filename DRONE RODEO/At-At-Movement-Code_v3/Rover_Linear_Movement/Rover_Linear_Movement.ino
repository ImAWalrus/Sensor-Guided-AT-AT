//-------------------------------------------------//
// Authors: Michael Gonzales, Jesse Ibarra         //            
// Date: 1-6-17                                    //
//                                                 //
// Purpose: Simple rover timed                     //
// linear movement                                 //
//                                                 //
// Dependencies: Adafruit motor library v1         //
//-------------------------------------------------// 

//Imported headers, and declarations
#include <AFMotor.h>
int direction_flag = 0;

//Stop and Turn variables (CAN BE ADJUSTED)
long run_time = 3000;             //milliseconds, move for n seconds and then stop 
long acceleration_delay = 5;      //milliseconds
long decceleration_delay = 5;     //milliseconds

//Motor variables (CAN BE ADJUSTED)
int max_motor_speed = 235; 
AF_DCMotor right_motor_bank_1(1);
AF_DCMotor right_motor_bank_2(2);
AF_DCMotor left_motor_bank_1(3);
AF_DCMotor left_motor_bank_2(4);

//------------------------//
// Arduino Setup Function //
//------------------------//
void setup() {
  
  Serial.begin(9600);

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

    delay(decceleration_delay);
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
  Serial.println("STOPPED\n");
}
//-------------------//
// Arduino MAIN Loop //
//-------------------//
void loop() {

  //Move in the appropriate direction for an amount of time
  if(direction_flag == 0){
    speed_up_forward();
    direction_flag = 1;  
  }
  else{
    speed_up_backward();
    direction_flag = 0;    
  }
  delay(run_time);

  //Slow down to a stop.
  stopall();

}
