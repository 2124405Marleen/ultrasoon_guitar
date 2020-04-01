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
int32_t frequency_rest = 0;

//Initialize variable melody game
int current_tone = 0;
int current_beat = 0;
long play_duration = 0;
long elapsed_time = 0;

long tempo = 10000;//overal tempo
int pause = 1000;//pause between notes
int rest_count = 100;//Variable to increase Rest lenght

//Init melodies

//Random melodie
int melody_one[] = {frequency_c, frequency_b, frequency_g, frequency_c, frequency_b, 
  frequency_e, frequency_rest, frequency_c, frequency_c, frequency_g, frequency_a, frequency_c};
int beats_one[] = {16,16,16,8,8,16,32,16,16,16,8,8};

int melody_two[] = {frequency_g, frequency_b, frequency_g, frequency_c, frequency_b, 
  frequency_e, frequency_rest, frequency_c, frequency_c, frequency_g, frequency_a, frequency_c};
int beats_two[] = {16,16,16,8,8,16,32,16,16,16,8,8};

int melody_three[] = {frequency_a, frequency_b, frequency_g, frequency_c, frequency_b, 
  frequency_e, frequency_rest, frequency_c, frequency_c, frequency_g, frequency_a, frequency_c};
int beats_three[] = {16,16,16,8,8,16,32,16,16,16,8,8};

int melody_four[] = {frequency_f, frequency_b, frequency_g, frequency_c, frequency_b, 
  frequency_e, frequency_rest, frequency_c, frequency_c, frequency_g, frequency_a, frequency_c};
int beats_four[] = {16,16,16,8,8,16,32,16,16,16,8,8};

void setup() {
  Serial.begin(9600);
  
  //initialize pins
  pinMode(echo_pin, INPUT);
  pinMode(trig_pin, OUTPUT);
  pinMode(headphone_pin, OUTPUT);
  
  InitTimersSafe();         //Not for time keeping funtions
  Serial.println("Setup complete");
}

void loop() {
//guitarGameTones();
guitarGameMusic();
}


//Send pulse to test time ping needs to go back and forth
void testPing(){
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

void playBeat(){
  //reset elapsed_time
  elapsed_time = 0;
  
  if (current_tone > 0){
    //Tone is no rest
    
    while (elapsed_time < play_duration){
      int in_between_pause = current_tone /2;
      digitalWrite(headphone_pin, HIGH);
      delayMicroseconds(in_between_pause);
      digitalWrite(headphone_pin, LOW);
      delayMicroseconds(in_between_pause);
      elapsed_time += (current_tone);
    }
    
  } else {
    //tone is rest
    
      for (int j = 0; j < rest_count; j++){
        delayMicroseconds(play_duration);
      }
   }
}
  
void playMelody(int melody[], int beats[]){

// Melody lenght for looping
int max_count = sizeof(melody) /2;

  for( int i = 0; i < max_count; i++){
    current_tone = melody[i];
    current_beat = beats[i];
    play_duration = current_beat * tempo;
    
    playBeat();
    delayMicroseconds(pause);
    }
  }

//'Guitar' game with musical tones
void guitarGameTones(){
  
  testPing();
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
      //0Hz sound (Quiet)
        playTone(frequency_rest);
      }

  //Wait 100 ms before staring over so sound can play
  delay(10000);
    
  }

//'Guitar' game with melody
void guitarGameMusic(){
  testPing();
  calculateDistance();
  printDistance();

//Every 15 cm diffrent melody
     if (distance <=15) {
       playMelody(melody_one, beats_one);
      } 
     else if (distance <=30){
        playMelody(melody_two, beats_two);
       
      }
     else if (distance <=45){
       playMelody(melody_three, beats_three);
      }
     else if (distance <=60){
       playMelody(melody_four, beats_four);
      }
     else{
      //0Hz sound (Quiet)
        playTone(frequency_rest);
      }

  //Wait 100 ms before staring over so sound can play
  delay(100); 
}
