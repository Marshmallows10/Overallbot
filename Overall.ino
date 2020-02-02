#include <FastLED.h>

#define NUM_LEDS 30
#define LED_PIN 52

CRGB leds[NUM_LEDS];

int relay = 22;
int buzzer = 53;     // the pin that the LED is atteched to
int sensor = 48;  // the pin that the sensor is atteched to
int sensor1 = 49;
int sensor2 = 50;
int sensor3 = 51;
int state = LOW;             // by default, no motion detected
int LEFT,FRONT,RIGHT,BACK = 0;                 // variable to store the sensor status (value)

int Trig1 = A8; //FRONT
int Echo1 = A9;
int Trig2 = A10; //LEFT
int Echo2 = A11;
int Trig3 = A12; //RIGHT
int Echo3 = A13;

int RPWM_Left=3;
int LPWM_Left=4;
int RPWM_Right=7;
int LPWM_Right=8;
// timer 0
int R_EN_Left=5;
int L_EN_Left=6;
int R_EN_Right=9;
int L_EN_Right=10;

int Left_Distance,Front_Distance, Right_Distance = 0;
long duration1, Ldistance,duration2, Fdistance,duration3, Rdistance;

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

  pinMode(relay, OUTPUT);

  pinMode(sensor, INPUT);    // initialize sensor as an input
  
  pinMode(Echo1, INPUT);
  pinMode(Trig1, OUTPUT);
  pinMode(Echo2, INPUT);
  pinMode(Trig2, OUTPUT);
  pinMode(Echo3, INPUT);
  pinMode(Trig3, OUTPUT);
  
  for(int i=5;i<9;i++){
   pinMode(i,OUTPUT);
  }
   
   delay(1000);
    Serial.begin(9600);
  }
void Red() {

   for (int i = 0; i <= 29; i++) {
    leds[i] = CRGB ( 255, 0 , 0);
    FastLED.show();
   }
}

void Green() {

   for (int i = 0; i <= 29; i++) {
    leds[i] = CRGB ( 0, 255 , 0);
    FastLED.show();

   }   
}

void Forward(){
    Serial.println("Forward");
  analogWrite(LPWM_Left,0);
  analogWrite(LPWM_Right,0);
  analogWrite(RPWM_Left,0);
  analogWrite(RPWM_Right,0);
    
  digitalWrite(R_EN_Left,HIGH);
  digitalWrite(L_EN_Left,HIGH);
  digitalWrite(R_EN_Right,HIGH);
  digitalWrite(L_EN_Right,HIGH);

  analogWrite(LPWM_Left,255);
 analogWrite(LPWM_Right,255);

  }
  void Left(){
    Serial.println("Left");
  digitalWrite(R_EN_Left,HIGH);
  digitalWrite(L_EN_Left,HIGH);
  digitalWrite(R_EN_Right,HIGH);
  digitalWrite(L_EN_Right,HIGH); 

  analogWrite(LPWM_Left,255);
 analogWrite(LPWM_Right,0);

 
  }
  void Right(){
    Serial.println("Right");
  digitalWrite(R_EN_Left,HIGH);
  digitalWrite(L_EN_Left,HIGH);
  digitalWrite(R_EN_Right,HIGH);
  digitalWrite(L_EN_Right,HIGH);
  
  analogWrite(LPWM_Left,0);
 analogWrite(LPWM_Right,255);
 
  }
  void Back(){
    Serial.println("Back");
  analogWrite(LPWM_Left,0);
  analogWrite(LPWM_Right,0);
  analogWrite(RPWM_Left,0);
  analogWrite(RPWM_Right,0);
    
  digitalWrite(R_EN_Left,HIGH);
  digitalWrite(L_EN_Left,HIGH);
  digitalWrite(R_EN_Right,HIGH);
  digitalWrite(L_EN_Right,HIGH);
  
  analogWrite(RPWM_Left,255);
 analogWrite(RPWM_Right,255);
  }
  void Stop(){
    Serial.println("Stop");
  digitalWrite(R_EN_Left,HIGH);
  digitalWrite(L_EN_Left,HIGH);
  digitalWrite(R_EN_Right,HIGH);
  digitalWrite(L_EN_Right,HIGH);

    analogWrite(RPWM_Left,0);
 analogWrite(RPWM_Right,0);
  }

int left_test(){

  digitalWrite(Trig2, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(Trig2, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(Trig2, LOW);
  duration1 = pulseIn(Echo2, HIGH);
  Ldistance= duration1 * 0.034/2;
  Serial.print("LEFT Distance: ");
  Serial.println(Ldistance);

  
  }
long front_test(){
  
  digitalWrite(Trig1, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(Trig1, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(Trig1, LOW);
  duration2 = pulseIn(Echo1, HIGH);
  Fdistance= duration2 * 0.034/2;
  Serial.print("Front Distance: ");
  Serial.println(Fdistance);
  
  }
  

 int right_test(){

  digitalWrite(Trig3, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(Trig3, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(Trig3, LOW);
  duration3 = pulseIn(Echo3, HIGH);
  Rdistance= duration3 * 0.034/2;
  Serial.print("Right Distance: ");
  Serial.println(Rdistance);

  }

void motion(){
  LEFT = digitalRead(sensor);   // read sensor value
  FRONT = digitalRead(sensor1);
  RIGHT = digitalRead(sensor2);
  BACK = digitalRead(sensor3);
   if (LEFT == HIGH || FRONT == HIGH || RIGHT == HIGH || BACK == HIGH) {           // check if the sensor is HIGH
    digitalWrite(buzzer, HIGH);   // turn buzzer ON
    delay(100);                // delay 100 milliseconds 
    digitalWrite(relay, LOW);
    delay(5000);
    Red();
    delay(100);
    FastLED.show();
    
    if (state == LOW) {
      digitalWrite(relay, HIGH);
      Serial.println("Motion detected!"); 
      state = HIGH;       // update variable state to HIGH
    }
  } 
  else {
      digitalWrite(buzzer, LOW); // turn buzzer OFF
      delay(200);             // delay 200 milliseconds 
      Green();
      delay(100);
      FastLED.show();
      if (state == HIGH){
        Serial.println("Motion stopped!");
        state = LOW;       // update variable state to LOW
    }
  }
  }
  
void loop() {
  Left_Distance = left_test();
  delay(10);
  Right_Distance = right_test();
  delay(10);
  Front_Distance = front_test();
  delay(10);

  motion();
  
if(Fdistance <= 50){
    if(Rdistance > Ldistance){
      if((Rdistance <= 25) && (Ldistance <= 50)){
        Stop();
        delay(200);
        Back();
        delay(500);
      }
      else{
        Right();
        delay(500);
      }
      }
      else if(Rdistance < Ldistance){
        if((Rdistance <= 25) && (Ldistance <= 50)){
        Stop();
        delay(200);
        Back();
        delay(500);
      }
      else{
        Left();
        delay(500);
      }
    }
    }

    else if(Rdistance <= 25){
      Left();
      delay(500);
    }

    else if(Ldistance <= 50){
      Right();
      delay(500);
    }

    else{
      Forward();
      delay(1000);
    }

 
}
