
/*
Example of BH1750 library usage.
This example initalises the BH1750 object using the default
high resolution mode and then makes a light level reading every second.
Connection:
 VCC-5v
 GND-GND
 SCL-SCL(analog pin 5)
 SDA-SDA(analog pin 4)
 ADD-NC or GND
*/

#include <Wire.h>
#include <BH1750.h>
#include <VirtualWire.h>


BH1750 lightMeter;
const int MAX_LUX = 10;

/* MQ-4 Methane Sensor Circuit with Arduino */

const int AOUTpin=0; //the AOUT pin of the methane sensor goes into analog pin A0 of the arduino
const int DOUTpin=8; //the DOUT pin of the methane sensor goes into digital pin D8 of the arduino
const int stateLedPin=13; //the anode of the LED connects to digital pin D13 of the arduino

int methanLimit;
int methanValue;

/* Transmitter */

char *wcstate = "free"; //Creates the variable nibble with the value of 0000  


void setup() {
  Serial.begin(9600);
  lightMeter.begin();
  pinMode(DOUTpin, INPUT); //sets the pin as an input to the arduino
  pinMode(stateLedPin, OUTPUT); //sets the pin as an output of the arduino
  Serial.println("Running...");

 
  vw_set_ptt_inverted(true);   
  vw_setup(2000);   
  vw_set_tx_pin(3); //Configure the D3 pin to read the data   
}


void loop() {
  // Light Sensor
  uint16_t lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");


  //Gas methane
  methanValue = analogRead(AOUTpin); //reads the analaog value from the methane sensor's AOUT pin
  methanLimit = digitalRead(DOUTpin); //reads the digital value from the methane sensor's DOUT pin
  Serial.print("Methane value: ");
  Serial.println(methanValue); //prints the methane value
  Serial.print("Methan Limit: ");
  Serial.println(methanLimit); //prints the limit reached as either LOW or HIGH (above or underneath)

  delay(100);

  if (lux > 10) {
    wcstate = "occupato";
  } else if (lux <= 10 && methanLimit == LOW) {
    wcstate = "free_for_emergency";
  } else {
    wcstate = "free";
  }

  if (wcstate == "occupato") {
    digitalWrite(stateLedPin, HIGH); //if occupato, LED turns on as status indicator
  } else {
    digitalWrite(stateLedPin, LOW); //if not occupato, LED remains off
  }

  vw_send((uint8_t *)wcstate, strlen(wcstate)); //Sends the variable wcstate   
  vw_wait_tx(); //Waits the end of transmission   

  Serial.println(wcstate);
  Serial.println("-------------");
  
  delay(200);
}
