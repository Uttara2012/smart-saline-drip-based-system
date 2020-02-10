#include <ESP8266WiFi.h>;
 
#include <WiFiClient.h>;
 
#include <ThingSpeak.h>;
#include <LiquidCrystal.h>

#include <LiquidCrystal_I2C.h>




 
const char* ssid = "Uttu"; //Your Network SSID
 
const char* password = "aaaaaaaa"; //
WiFiClient client;
 
unsigned long channel = 963032; //Your Channel Number (Without Brackets)
 
const char * myWriteAPIKey = "8ILP2Q7I2L1QSOVT"; //Your Write API Key
 
const int pingPin = D5; // Trigger Pin of Ultrasonic Sensor
const int echoPin = D6; // Echo Pin of Ultrasonic Sensor
int value1=200;
int value2=15;
int value3=100;
// Defining led pins

#define green_led D3
#define yellow_led D4
#define red_led D7

int L; // total_length of bottle


// Solenoid Valve

int solenoid_pin = 4;

void setup() {
   Serial.begin(9600); // Starting Serial Terminal
   //lcd.begin(16,2);
  

  // Turn on the backlight.
//  lcd.backlight();
  //lcd.setCursor(0, 0);

   pinMode(green_led,OUTPUT);
   pinMode(yellow_led,OUTPUT);
   pinMode(red_led,OUTPUT);
   pinMode(solenoid_pin,OUTPUT);

   digitalWrite(green_led,LOW);
   digitalWrite(yellow_led,LOW);
   digitalWrite(red_led,LOW);

   digitalWrite(solenoid_pin,LOW);
    
delay(10);
 
// Connect to WiFi network
 
WiFi.begin(ssid, password);
ThingSpeak.begin(client);
}

void loop() {
   /*long duration, inches, cm;
   pinMode(pingPin, OUTPUT);
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   pinMode(echoPin, INPUT);
   duration = pulseIn(echoPin, HIGH);
   inches = microsecondsToInches(duration);
   cm = microsecondsToCentimeters(duration);
   Serial.print(inches);
   Serial.print("in, "); 
   Serial.print(cm);
   Serial.print("cm");
   Serial.println();
   delay(1000);*/


 long distance, duration,inches,cm;

  digitalWrite(pingPin,LOW);
  delay(2);
  digitalWrite(pingPin,HIGH);
  delay(10);
  digitalWrite(pingPin,LOW);
  
  duration = pulseIn(echoPin,HIGH);
  distance = ((duration/2)/29.1);

  if(distance >= value1 || distance < value3)
  {
  Serial.print("OUT OF RANGE");
//  lcd.print("Out Of Range");
  
  }

  else
  {
   Serial.print(distance);
   cm = distance;
   inches = cm*2.54;
  
  
   ThingSpeak.writeField(channel, 1,cm, myWriteAPIKey); //Update in ThingSpeak

   // Setting of Led pins
   
   if( distance<value1 && distance>value3)
   {
    digitalWrite(green_led,HIGH);
    digitalWrite(yellow_led,HIGH);
    digitalWrite(red_led,HIGH);
   }

   else if( distance>value2 && distance<value3)
   {
   digitalWrite(yellow_led,HIGH);
   digitalWrite(red_led,HIGH);
 //   lcd.print("NORMAL");
   }

   else
   {
    while(distance < value2) // Running a while loop untill we get distance < l3
    {
    digitalWrite(red_led,HIGH);
    digitalWrite(solenoid_pin,HIGH); 
  //  lcd.print("EMERGENCY");// Setting of solenoid valve OFF if we have 
    delay(10);
    }
   }
   
  }
  delay(500);
   int emergency = ThingSpeak.readFloatField(channel,emergency);
   if(emergency == 1){
    
    digitalWrite(solenoid_pin, 1);
  //  lcd.print("EMERGENCY");
    digitalWrite(red_led,HIGH);
  }
  else if(emergency == 0){
    digitalWrite(solenoid_pin, 0);
    digitalWrite(red_led,LOW);
   
  
  }
  int Patient1 = ThingSpeak.readFloatField(channel,Patient1);
   if(Patient1 == 1){
    digitalWrite(green_led,LOW);
    //lcd.print("presr-1.xxx   2.yy");
   
  }
  else if(Patient1 == 0){
     //lcd.print("presr-1.pp   2.hyy");
    digitalWrite(yellow_led,LOW);
   
  
  }
  
}

/*long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}*/
  
