/*
 * MIT License
 * 
 * Copyright (c) 2018 burps-cf
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * This code maps PWM values to linear speeds of an Elegoo robot car.
 * 
 */

// www.elegoo.com

// ultrasonic distance sensor sits on top of servo
#include <Servo.h>  //servo library
Servo myservo;      // create servo object to control servo

const int Echo = A4;
const int Trig = A5;

//    The direction of the car's movement
//  ENA   ENB   IN1   IN2   IN3   IN4   Description  
//  HIGH  HIGH  HIGH  LOW   LOW   HIGH  Car is runing forward
//  HIGH  HIGH  LOW   HIGH  HIGH  LOW   Car is runing back
//  HIGH  HIGH  LOW   HIGH  LOW   HIGH  Car is turning left
//  HIGH  HIGH  HIGH  LOW   HIGH  LOW   Car is turning right
//  HIGH  HIGH  LOW   LOW   LOW   LOW   Car is stoped
//  HIGH  HIGH  HIGH  HIGH  HIGH  HIGH  Car is stoped
//  LOW   LOW   N/A   N/A   N/A   N/A   Car is stoped


//define L298n module IO Pin
#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11

const int defaultSpeed = 150;
const int cutOff = 92; // min PWM value to cause car to move
const int stopDistance = 30;

void setSpeed (int speed) {
  speed = constrain(speed, 0, 255);
  analogWrite(ENB,speed);
  analogWrite(ENA,speed);
} // setSpeed


void forward(int speed){
  setSpeed(speed);
  analogWrite(ENB,speed);
  analogWrite(ENA,speed);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  //Serial.print("Forward ");
} // forward


void back(int speed){
  setSpeed(speed);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  Serial.println("Back");
} // back


void left(int speed){
  setSpeed(speed);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  Serial.println("Left");
} // left


void right(int speed){
  setSpeed(speed);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  Serial.println("Right");
} // right


void stop() {
  setSpeed(0);
} // stop


// Ultrasonic distance measurement
int testDistance() {
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(20);
  digitalWrite(Trig, LOW);
  float Fdistance = pulseIn(Echo, HIGH);
  Fdistance= Fdistance / 58;
  return (int)Fdistance;
} // testDistance


// measure speed moving forward closer to an obstacle
void testSpeed(int motorPWM) {
  unsigned long t0, t1; // time stamps
  int d0, d1; // distances
  float speed;

  Serial.print(motorPWM);
  Serial.print(" => ");
  myservo.write(90);  // point ultrasonic sensor forward
  delay(500);

  d0 = testDistance();
  t0 = millis();
  forward (motorPWM);
  while (testDistance() > stopDistance) {
  }
  stop();
  t1 = millis();
  d1 = testDistance();
  delay(100);

  Serial.print(motorPWM);
  delay(100);
  Serial.print(" => ");
  Serial.print(d0);
  Serial.print(" / ");
  Serial.print(d1);
  delay(100);
  Serial.print(" / ");
  Serial.print(d0-d1);
  Serial.print(" | ");
  Serial.print(t0);
  delay(100);
  Serial.print(" / ");
  Serial.print(t1);
  Serial.print(" / ");
  Serial.print(t1-t0);
  delay(100);
  Serial.print(" / ");
  speed = (d0-d1)*1000.0/(t1-t0);

  // speed [cm/s]
  Serial.println(speed, 2);
} // testSpeed


void setup() {
  Serial.begin(9600);

  // init pins for motor control
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  stop();

  // init servo and pins for ultrasonic
  myservo.attach(3);
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
} // setup


const int nSamples = 6;

//Repeat execution
void loop() {

  int sample, pwm;
  char getStr;

  for (sample = 1; sample <= nSamples; sample++){
    // wait for the user
    do {
      getStr = Serial.read();
    } while (getStr != 's');

    // spread samples evenly over valid range of PWM
    pwm = map (sample,1,nSamples,cutOff,254);
    Serial.print (sample);
    Serial.print (" ==> ");
    Serial.println(pwm);
    delay (100);
    testSpeed(pwm);
  }

} // loop

