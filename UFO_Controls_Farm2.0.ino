///////////////////// UFO CONTROLS - FARM 2.0 /////////////////////

//Author: Kristopher Buote on November 1, 2017
//Updated: Jared Holitski on Feburary 20th, 2018

// This program controls the Urban Farming Organization's Rolling Vertical Farm

//The primary functions are as follows:
//1) The pump is turned on or off based on the water sensors input; protects the pump from running dry
//2) The lights are activated on an interval based on a real-time clock (RTC)
//that is kept on even if power is cut off. There is a battery in the data-logger shield.
//3) Exhaust fans are turned on/off based on the air temperature/humidity (DHT22 sensor)
//4) Vent is opened with servo motors to allow airflow



///////////////////// PUMP CONTROLS INITIALIZATION /////////////////////
int baud = 9600;
int in4 = 4; // digital pin connection for output to the relay in1 - this will open/close relay
int empty_tank = 6; // digital pin connection for input from empty tank sensor
int half_tank = 1; // digital pin connection for input from half tank sensor
int full_tank = 11; // digital pin connection for input from full tank sensor
int led_empty = 10;  // digital pin connection for led_empty 
//int led_half = 5;  // digital pin connection for led_half 
//int led_full = 4;  // digital pin connection for led_full

int empty_level = 0; // initialize empty sensor reading variable
int half_level = 0; // initialize water sensor reading variable
int full_level = 0; // initialize water sensor reading variable


///////////////////// LIGHT CONTROLS INITIALIZATION/////////////////////

#include <Wire.h>
#include "RTClib.h" // Real Time Clock Library

RTC_PCF8523 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
int current_hour = 1; //initialize variable 
int in1 = 7; // Digitial Pin that inputs to the relay switch that controls the farm's lights

///////////////////// FAN CONTROLS INITIALIZATION/////////////////////
// Air Temperature/Humidity sensor is DHT 22 from Adafruit//
// tutorial: https://learn.adafruit.com/dht/connecting-to-a-dhtxx-sensor
// github code: https://github.com/adafruit/DHT-sensor-library/blob/master/examples/DHTtester/DHTtester.ino

#include "Adafruit_Sensor.h"
#include "DHT.h"
#define DHTPIN 3     // what digital pin DHT temp. sensor is connected to on arduino
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor.
int in2 = 8; // digital pin to relay input for hot fans
int in3 = 9; // digital pin to relay input for hotter fans


////////////////////// SERVO CONTROLS INITIALIZATION ////////////////////
// used this source to build the model needed
// https://www.arduino.cc/en/Tutorial/Sweep 
#include <Servo.h>

Servo vent_a;  // create servo object for the vent side a
Servo vent_b;  // create servo object for the vent side b

int stopped = 90;
int movingforward = 100;
int movingbackward = 80;
  // open for servo a 
 // closed for both servos a & b
  

void setup() { // starts the setup loop

  
  ///////////////////// PUMP CONTROLS SETUP/////////////////////
  Serial.begin(baud);
  pinMode(empty_tank, INPUT); //pin setup for empty tank sensor
  pinMode(half_tank, INPUT); //pin setup for half tank sensor
  pinMode(full_tank, INPUT); //pin setup for full tank sensor
  pinMode(in4, OUTPUT); //pin setup for pump relay - opens/closes pump relay 
  pinMode(led_empty, OUTPUT); // pin setup for led_empty 
  //pinMode(led_half, OUTPUT); // pin setup for led_half 
  //pinMode(led_full, OUTPUT); // pin setup for led_full 
  digitalWrite(led_empty, HIGH); // Turn on led_empty initially
  //digitalWrite(led_half, HIGH); // Turn on led_half initially
  //digitalWrite(led_full, HIGH); // Turn on led_full initially

  //Turn OFF any power to the Relay channels
  digitalWrite(in1,LOW);
  delay(2000); //Wait 2 seconds before starting sequence
  
  ///////////////////// LIGHT CONTROLS SETUP/////////////////////

  pinMode(in1, OUTPUT);
  
    while (!Serial) {
      delay(1);  // for Leonardo/Micro/Zero
    }
  
    Serial.begin(baud);
    if (! rtc.begin()) {
      Serial.println("Couldn't find RTC");
      while (1);
    }
  
    if (! rtc.initialized()) {
      Serial.println("RTC is NOT running!");
      // following line sets the RTC to the date & time this sketch was compi_empty_empty
      // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      // This line sets the RTC with an explicit date & time, for example to set
      // January 21, 2014 at 3am you would call: rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
      //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      rtc.adjust(DateTime(2018,4,10,16,32,2));
    }
    
  /////////////////////FAN CONTROLS SETUP/////////////////////
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  Serial.begin(baud);
  Serial.println("DHT22 test!");
  
  dht.begin();
    // set up digital pin as output to fan relay
    
  ////////////////////// SERVO SETUP ////////////////////

  vent_a.attach(5);  // attaches the vent_a servo on pin 11 to the servo object
  vent_b.attach(2);  // attaches the vent_b servo on pin 12 to the servo object
} // ends the setup loop



void loop() { // starts the control loop
  
  ///////////////////// PUMP CONTROLS///////////////////// 
  //empty_level = digitalRead(half_tank);
//if (empty_level == LOW) // LOW is no stimulus from sensor. Therefore the light is not on and the tank is empty. 
//  {
// digitalWrite(in4, HIGH); //Send high signal to NC (Normally Closed) Relay -> opens circuit (pump off)
//digitalWrite(led_empty, LOW); // turns on the empty led
//   Serial.println("TANK  NOT EMPTY"); //Print high/low to serial monitor
// delay(500);
    
//  }
// else
// {
// digitalWrite(in4, LOW); // Send Low signal to NC Relay -> leaves circuit closed (pump on)
// digitalWrite(led_empty, HIGH); // turns off the empty led
// Serial.println("TANK EMPTY"); //Print high/low to serial monitor
/// delay(500);
    
    
 // }
  
  //Serial.println(empty_level, DEC);
  
/////////////////////LIGHT CONTROLS///////////////////// 
/// !!! Lights are Normally closed circuit !!!

  DateTime now = rtc.now(); //read the time
  
  //format the time and print to serial monitor
  Serial.println();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
 

  
  if (now.hour() < 6 || now.hour() >= 20) { //check if the time is between 6AM and 21PM in 24 hour time
      Serial.println("   ||   Lights on");
      digitalWrite(in1, LOW); //   Send high signal when between 6 AM and 9 PM - turn lights on
  }
  else {
      Serial.println("  ||   Lights OFF");  //else turn lights off
      digitalWrite(in1, HIGH );
  }
  
  Serial.println();
  
  
  ///////////////////// . FAN CONTROLS . /////////////////////
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();


  // Check if any reads failed_empty_empty and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

// PUT IN DIFFERENT TEMPERATURE (15 C) IF NIGHTTIME!!!!!!!!!!!!
// Fan is on NORMALLY OPEN (NO) in relay
    if (t >= 24.00 || h > 50.00){                           // If the temp > 24 C or humditity > 50 % -> activate fan
        digitalWrite(in2, LOW);                             // Turn on first fan
        digitalWrite(in3, LOW);                             // Turn on second fan
       Serial.println("Fans on");  
           //  vent_a.write(movingforward);
           //  vent_b.write(movingbackward);
           //Serial.print("Vent Drafting");//write fans on
     }

    else {
        digitalWrite(in2, HIGH);                          // Else, leave fan 1 off
        digitalWrite(in3, HIGH);                          // Else, leave fan 2 off
        Serial.println("Fans off");                           //   write fans on
           // vent_a.write(stopped);
           // vent_b.write(stopped); 
         //Serial.print("Vent Closed");
     }

     
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");

///////////////////// . FAN CONTROLS . /////////////////////

if(t >= 24.00 || h > 50.00){
  vent_a.write(movingforward);
  vent_b.write(movingbackward);
}

else{
vent_a.write(stopped);
vent_b.write(stopped);
}

delay(5000); //Only run the code every 5 seconds

}
