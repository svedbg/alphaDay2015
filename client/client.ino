#include <VirtualWire.h> //Includes library VirtualWire.h

void setup() {
  Serial.begin(9600);
    
  //++++++++++++++Initializes the receiver module+++++++++++++++++++   
  vw_set_ptt_inverted(true);   
  vw_setup(2000);   
  vw_set_rx_pin(3); //Configure the pin D3 to read the data   
  vw_rx_start(); //Starts reading data from the receiver module   
  //==============================================================   

  //Configure the pins 6 and 9 as output   
  for (int i = 11; i <= 13; i++) {     
    pinMode(i, OUTPUT);
  }
  Serial.println("Running...");

}

String state;

void unknownState() {

  for (int i = 0; i < 5; i++) {
    
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    delay(100);
    
    digitalWrite(13, LOW);
    digitalWrite(12, HIGH);
    delay(100);
    
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    digitalWrite(11, HIGH);
    delay(100);
    
  }
  
  digitalWrite(11, LOW);
}

int retry = 0;

void loop() {
  
  uint8_t buf[VW_MAX_MESSAGE_LEN]; //Variable for storing the data buffer   
  uint8_t buflen = VW_MAX_MESSAGE_LEN; //Variable for storing the buffer size
  
  if (vw_get_message(buf, &buflen)) { // We check if we have received data
    
    state = String();

    buf[buflen] = '\0'; // Terminate
    
    String(state.concat(String((char *)buf)));
  

    if (String(state) == "occupato") {
      digitalWrite(13, HIGH);
      digitalWrite(12, LOW);
      digitalWrite(11, LOW);
    } else if (String(state) == "free_for_emergency") {
      digitalWrite(13, LOW);
      digitalWrite(12, HIGH);
      digitalWrite(11, LOW);
    } else if (String(state) == "free") {
      digitalWrite(13, LOW);
      digitalWrite(12, LOW);
      digitalWrite(11, HIGH);
    } else {
      unknownState();
    }
    retry = 0;
  }
  
  retry++;
  
  if (retry > 50) {
    retry = 0;
    unknownState();
  }
  
  delay(200);
  
}
