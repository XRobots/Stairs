#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

unsigned long previousMillis = 0;
const long interval = 20;

int but1;
int but2;
int but3;
int but4;

int sw1;
int sw2;
int sw3;
int sw4;
int sw5;

int axis1;
int axis2;
int axis3;
int axis4;
int axis5;
int axis6;

String count;

struct SEND_DATA_STRUCTURE{
//struct __attribute__((__packed__)) SEND_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to send
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO

 
    int16_t RLR;
    int16_t RFB;
    bool sw1;
    bool sw2;
    bool sw3;
    bool sw4;
    bool sw5;
    bool sw6;
    bool sw7;
    bool sw8;


};

struct RECEIVE_DATA_STRUCTURE_REMOTE{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int16_t mode;
  int16_t count;
};

int remoteFlag = 0;

SEND_DATA_STRUCTURE mydata_send;
RECEIVE_DATA_STRUCTURE_REMOTE mydata_remote;

RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};

void setup() {  

  pinMode(24, INPUT_PULLUP); 
  pinMode(26, INPUT_PULLUP); 
  pinMode(28, INPUT_PULLUP); 
  pinMode(30, INPUT_PULLUP); 
  pinMode(32, INPUT_PULLUP); 
  pinMode(34, INPUT_PULLUP); 
  pinMode(36, INPUT_PULLUP); 
  pinMode(38, INPUT_PULLUP); 
  pinMode(40, INPUT_PULLUP); 
  pinMode(42, INPUT_PULLUP); 
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  
  radio.begin();
  radio.openWritingPipe(addresses[1]); // 00001
  radio.openReadingPipe(1, addresses[0]); // 00002
  radio.setPALevel(RF24_PA_MIN);

  radio.stopListening();

  Serial.begin(115200); 
    
}

void loop() {

     unsigned long currentMillis = millis();
         if (remoteFlag == 0 && currentMillis - previousMillis >= 10) { 

              previousMillis = currentMillis;  
                   
              axis1 = analogRead(A1);
              axis2 = analogRead(A0);
              mydata_send.RFB = axis2;
              mydata_send.RLR = axis1;

              mydata_send.sw8 = digitalRead(24);
              mydata_send.sw7 = digitalRead(26);
              
              mydata_send.sw6 = digitalRead(30);
              mydata_send.sw5 = digitalRead(32);
              mydata_send.sw4 = digitalRead(34);
              mydata_send.sw3 = digitalRead(36);
              
              mydata_send.sw2 = digitalRead(40);
              mydata_send.sw1 = digitalRead(42);             
      

              radio.write(&mydata_send, sizeof(SEND_DATA_STRUCTURE));               
              
         }        
        


  }  // end of main loop

