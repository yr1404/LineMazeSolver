#include <AFMotor.h>
AF_DCMotor motorleft(1); 
AF_DCMotor motorright(2);

// speed of motor
#define spd 150

// IR Sensor
#define leftFarSensor        A0
#define leftCenterSensor     A1
#define rightCenterSensor    A2
#define rightFarSensor       A3

#define obstacleSensor       A4
 
// IR Sensor line readings
int leftFarReading;
int leftCenterReading;
int rightCenterReading;
int rightFarReading;

// IR Sensor  Obstacle readings
bool obstacleReading;

//for storing path details
char path[60] = {}; // Moves < 60 //
int pathLength;
int readLength;

void setup() {

  pinMode(leftFarSensor, INPUT);
  pinMode(leftCenterSensor, INPUT);
  pinMode(rightCenterSensor, INPUT);
  pinMode(rightFarSensor, INPUT);
  pinMode(obstacleSensor, INPUT);
  
  motorleft.setSpeed(spd);
  motorright.setSpeed(spd);
}

void loop() {
  readSensors();
    //if only the middle two sensors can see the black line
  
  if(leftFarReading == 0 && rightFarReading == 0 && (leftCenterReading == 1 || rightCenterReading == 1)) 
  { 
    straight();                                                                                    
  }

  if(leftFarReading == 1 && rightFarReading == 1 && leftCenterReading == 0 && rightCenterReading == 0){
    leap();
    readSensors(); 
  }
    //otherwise goes to the leftHandWall method
  else
  {                                                                                              
    leftHandWall();                                                                                
  }
}

void readSensors()  //accepts input from sensors
{
  leftCenterReading    = digitalRead(leftCenterSensor);
  leftFarReading       = digitalRead(leftFarSensor);
  rightCenterReading   = digitalRead(rightCenterSensor);
  rightFarReading      = digitalRead(rightFarSensor); 
  obstacleReading      = digitalRead(obstacleSensor);
}

void leftHandWall(){
  
//bool dead_end = leftFarReading == 0 && rightFarReading == 0 && leftCenterReading == 0 && rightCenterReading == 0;

  if (leftCenterReading == 1 || rightCenterReading == 1){
    
    if(leftFarReading == 1 && rightFarReading == 0 ){
     turnLeft();
     readSensors();
    }

    //RIGHT OR STRAIGHT
    if(leftFarReading == 0 && rightFarReading == 1 ){
      turnRight();
      readSensors();
    }

    else if (leftFarReading == 1 && rightFarReading == 1){
      leap();
      readSensors();

      if (leftFarReading == 1 && rightFarReading == 1){
        done();
//        exit(0);
        return;
      }

      else if (leftFarReading == 0 && rightFarReading == 0){
        
        back();
        turnLeft();
        readSensors();
      }
      
    }
  }

   
  if (leftCenterReading == 0 && rightCenterReading == 0){

    // DEAD END
    if(leftFarReading == 0 && rightFarReading == 0){
      turnAround();
      readSensors();
    }

    else if(leftFarReading == 1 && rightFarReading == 0){
      turnLeft();
      readSensors();
    }

    else if (leftFarReading == 0 && rightFarReading == 1){
      turnRight();
      readSensors();
    }
    
  }

}

//void shortPath(){
//  
//}

void straight (){
  motorleft.run(FORWARD);
  motorright.run(FORWARD);
}

void done(){
  motorleft.run(RELEASE);
  motorright.run(RELEASE);
}

void turnLeft() {
  motorleft.run(BACKWARD);
  motorright.run(FORWARD);
}

void turnRight(){
  motorleft.run(FORWARD);
  motorright.run(BACKWARD);
}

void turnAround(){
  motorleft.run(FORWARD);
  motorright.run(BACKWARD);
}

void leap(){
  straight();
  delay(200);
  done();
  delay(100);
}
void back(){
  motorleft.run(BACKWARD);
  motorright.run(BACKWARD);
}
