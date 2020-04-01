#include <PWM.h>

/*
   
Electric 'guitar'
Version: 1.0
Author: Marleen Roubos 2020

Hardware:
- Headphones
- Arduino Uno
- Ultrasoon
   
*/

//initialize pins
const int headphone_pin = 10; // headphone connected to PWM pin 10
const int echo_pin = 2; //Sensor Echo connected to pin 2
const int trig_pin = 4; //Sensor Trig connected to pin 4
float distance_target; //distance between sensor and target
float ping_time; //Time ping needs to go back and forth between target
float speed_sound = 343.2; //Speed of sound in m/s
int distance = 0; //distance in integer

//initialize Hz frequency diffrent musical notes
int32_t frequency_c = 264; 
int32_t frequency_d = 297; 
int32_t frequency_e = 330; 
int32_t frequency_f = 352; 
int32_t frequency_g = 396; 
int32_t frequency_a = 440; 
int32_t frequency_b = 495; 

void setup() {
  Serial.begin(9600);
  
  //initialize pins
  pinMode(echo_pin, INPUT);
  pinMode(trig_pin, OUTPUT);
  pinMode(headphone_pin, OUTPUT);
  
  InitTimersSafe(); //initialize all timers except for 0, to save time keeping functions
  Serial.println("Setup complete");
}

void loop() {

  calculatePing();
  calculateDistance();
  printDistance();

//Different musical tone every 10 cm
      if (distance <=10) {
        playTone(frequency_c);
      } 
      else if (distance <=20){
        playTone(frequency_d);
      }
      else if (distance <=30){
        playTone(frequency_e);
      }
     else if (distance <=40){
        playTone(frequency_f);
      }
      else if (distance <=50){
        playTone(frequency_g);
      }
      else if (distance <=60){
        playTone(frequency_a);
      }
     else{
        playTone(0);
      }
  
  delay(100);
    
}


//Calculate total time a ping needs to go back and forth
void calculatePing(){
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2000);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(15);
  digitalWrite(trig_pin, LOW);
 
}

// Calculates the distance basted on Ping speed
void calculateDistance(){
  ping_time = pulseIn(echo_pin, HIGH);  // ping in microseconds
  ping_time = ping_time / 1000000; //ping in seconds
  ping_time = ping_time / 2; //Duration one way for ping
  distance_target = speed_sound * ping_time;
  distance = distance_target * 100; //distance in cm
}

//Prints the distance in console
void printDistance(){
  Serial.print("Distance to target: \t\t");
  Serial.print(distance);
  Serial.println(" cm");
}

//Plays a tone on a specific frequency
void playTone(int32_t frequency){
      pwmWrite(headphone_pin, frequency);
      SetPinFrequency(headphone_pin, frequency);
}
