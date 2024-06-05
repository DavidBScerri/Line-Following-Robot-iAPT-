#include <Servo.h>

#define L_SENSOR_PIN A2
#define M_SENSOR_PIN A1
#define R_SENSOR_PIN A0

#define PIN_Motor_STBY_A 3
#define PIN_Motor_STBY_B 4
#define PIN_Motor_PWMA 5
#define PIN_Motor_PWMB 6
#define PIN_Motor_AIN_1 7
#define PIN_Motor_BIN_1 8

#define PIN_Servo 10

#define TRIG_PIN 13
#define ECHO_PIN 12

Servo myservo;

int speed = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(L_SENSOR_PIN, INPUT);
  pinMode(M_SENSOR_PIN, INPUT);
  pinMode(R_SENSOR_PIN, INPUT);

  myservo.attach(PIN_Servo);
  myservo.write(88);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void forward(){
  digitalWrite(PIN_Motor_STBY_A, HIGH);
  digitalWrite(PIN_Motor_AIN_1, HIGH);
  
  digitalWrite(PIN_Motor_STBY_B, HIGH);
  digitalWrite(PIN_Motor_BIN_1, HIGH);

  if(speed < 50){
    for (speed; speed <= 50; speed++){
      analogWrite(PIN_Motor_PWMA, speed);
      analogWrite(PIN_Motor_PWMB, speed);
      delay(20);
    }
  }
  else{
    analogWrite(PIN_Motor_PWMA, speed);
    analogWrite(PIN_Motor_PWMB, speed);
  }
}

void turnLeft(){
  digitalWrite(PIN_Motor_STBY_A, HIGH);
  digitalWrite(PIN_Motor_AIN_1, HIGH);
  analogWrite(PIN_Motor_PWMA, speed*2);

  digitalWrite(PIN_Motor_STBY_B, HIGH);
  digitalWrite(PIN_Motor_BIN_1, LOW);
  analogWrite(PIN_Motor_PWMB, speed);
}

void turnRight(){
  digitalWrite(PIN_Motor_STBY_A, HIGH);
  digitalWrite(PIN_Motor_AIN_1, LOW);
  analogWrite(PIN_Motor_PWMA, speed);

  digitalWrite(PIN_Motor_STBY_B, HIGH);
  digitalWrite(PIN_Motor_BIN_1, HIGH);
  analogWrite(PIN_Motor_PWMB, speed*2);
}

void reverse(){
  digitalWrite(PIN_Motor_STBY_A, HIGH);
  digitalWrite(PIN_Motor_AIN_1, LOW);
  analogWrite(PIN_Motor_PWMA, speed);

  digitalWrite(PIN_Motor_STBY_B, HIGH);
  digitalWrite(PIN_Motor_BIN_1, LOW);
  analogWrite(PIN_Motor_PWMB, speed);
}

void stopMov(){
  if(speed > 0){
    for (speed; speed >= 0; speed--) {
      analogWrite(PIN_Motor_PWMA, speed);
      analogWrite(PIN_Motor_PWMB, speed);
      delay(20);
    }
  }
  
  analogWrite(PIN_Motor_PWMA, 0);
  analogWrite(PIN_Motor_PWMB, 0);
}

void loop(){
  long duration, distance;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * (0.034 / 2); 
  
  int L_sensor = digitalRead(L_SENSOR_PIN);
  int M_sensor = digitalRead(M_SENSOR_PIN);
  int R_sensor = digitalRead(R_SENSOR_PIN);
  int repeat = 1;

  if(distance > 10){
    if(L_sensor && M_sensor && R_sensor){ // 1 1 1 = intersection or stop
      forward();
      delay(50);

      L_sensor = digitalRead(L_SENSOR_PIN);
      M_sensor = digitalRead(M_SENSOR_PIN);
      R_sensor = digitalRead(R_SENSOR_PIN);

      if(!L_sensor && !M_sensor && !R_sensor){
        stopMov();
      }
    }
    else if(L_sensor && M_sensor && !R_sensor){ // 1 1 0 = left
      forward();
      delay(50);
      
      L_sensor = digitalRead(L_SENSOR_PIN);
      M_sensor = digitalRead(M_SENSOR_PIN);
      R_sensor = digitalRead(R_SENSOR_PIN);

      if(L_sensor && !M_sensor && !R_sensor || L_sensor && M_sensor && !R_sensor){
        do{
          turnLeft();

          L_sensor = digitalRead(L_SENSOR_PIN);
          M_sensor = digitalRead(M_SENSOR_PIN);
          R_sensor = digitalRead(R_SENSOR_PIN);
          
          if(!L_sensor && M_sensor && !R_sensor){
            repeat = 0;
          }
        } while(repeat == 1);
      }
    }
    else if(L_sensor && !M_sensor && R_sensor){ // 1 0 1 = stop (just incase but highly unlikely)
      forward();
    }
    else if(L_sensor && !M_sensor && !R_sensor){ // 1 0 0 = left
      forward();
      delay(50);

      L_sensor = digitalRead(L_SENSOR_PIN);
      M_sensor = digitalRead(M_SENSOR_PIN);
      R_sensor = digitalRead(R_SENSOR_PIN);

      if(L_sensor && !M_sensor && !R_sensor || L_sensor && M_sensor && !R_sensor){
        do{
          turnLeft();

          L_sensor = digitalRead(L_SENSOR_PIN);
          M_sensor = digitalRead(M_SENSOR_PIN);
          R_sensor = digitalRead(R_SENSOR_PIN);
          
          if(!L_sensor && M_sensor && !R_sensor){
            repeat = 0;
          }
        } while(repeat == 1);
      }
    }
    else if(!L_sensor && M_sensor && R_sensor){ // 0 1 1 = right
      forward();
      delay(50);

      L_sensor = digitalRead(L_SENSOR_PIN);
      M_sensor = digitalRead(M_SENSOR_PIN);
      R_sensor = digitalRead(R_SENSOR_PIN);

      if(!L_sensor && !M_sensor && R_sensor || !L_sensor && M_sensor && R_sensor){
        do{
          turnRight();

          L_sensor = digitalRead(L_SENSOR_PIN);
          M_sensor = digitalRead(M_SENSOR_PIN);
          R_sensor = digitalRead(R_SENSOR_PIN);
          
          if(!L_sensor && M_sensor && !R_sensor){
            repeat = 0;
          }
        } while(repeat == 1);
      }
    }
    else if(!L_sensor && M_sensor && !R_sensor){ // 0 1 0 = straight
      forward();
    }
    else if(!L_sensor && !M_sensor && R_sensor){ // 0 0 1 = right
      forward();
      delay(50);

      L_sensor = digitalRead(L_SENSOR_PIN);
      M_sensor = digitalRead(M_SENSOR_PIN);
      R_sensor = digitalRead(R_SENSOR_PIN);

      if(!L_sensor && !M_sensor && R_sensor || !L_sensor && M_sensor && R_sensor){
        do{
          turnRight();

          L_sensor = digitalRead(L_SENSOR_PIN);
          M_sensor = digitalRead(M_SENSOR_PIN);
          R_sensor = digitalRead(R_SENSOR_PIN);
          
          if(!L_sensor && M_sensor && !R_sensor){
            repeat = 0;
          }
        } while(repeat == 1);
      }
    }
    else if(!L_sensor && !M_sensor && !R_sensor){ // 0 0 0 = stop
      stopMov();
    }
  }
  else{
    stopMov();
  }
}
