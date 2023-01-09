// Radio
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};

//**************remote control****************
struct RECEIVE_DATA_STRUCTURE{
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

RECEIVE_DATA_STRUCTURE mydata_remote;

int RLR = 0;
int RFB = 0;
int RFBa = 0;

int drive1;
int drive2;
int drive1a;
int drive2a;

unsigned long currentMillis;
long previousMillis = 0;    // set up timers
long interval = 10;        // time constant for timer

unsigned long remoteMillis;   // safety timer
int remoteState;

void setup() {

    // initialize serial communication
    Serial.begin(115200);
    
    radio.begin();
    radio.openWritingPipe(addresses[0]); // 00002
    radio.openReadingPipe(1, addresses[1]); // 00001
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();

    pinMode(2, OUTPUT);   // wheel PWMs
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);

    pinMode(6, OUTPUT);   // back linear axis
    pinMode(9, OUTPUT);

    pinMode(10, OUTPUT);  // front linear axis
    pinMode(11, OUTPUT);

    pinMode(22, OUTPUT);  // vertical linear axis
    pinMode(24, OUTPUT);
    pinMode(26, OUTPUT);
    pinMode(28, OUTPUT);
    
  
}   // end of setup

// ********************* MAIN LOOP *******************************

void loop() {  

      
        currentMillis = millis();
        if (currentMillis - previousMillis >= 10) {  // start timed event
          
            previousMillis = currentMillis;


            // check for radio data
            if (radio.available()) {
                    radio.read(&mydata_remote, sizeof(RECEIVE_DATA_STRUCTURE)); 
                    remoteMillis = currentMillis;  
            }             

            // is the remote disconnected for too long ?
            if (currentMillis - remoteMillis > 500) {
              remoteState = 0;
              mydata_remote.RFB = 512;
              mydata_remote.RLR = 512;
              mydata_remote.sw1 = 1;
              mydata_remote.sw2 = 1;
              mydata_remote.sw3 = 1;
              mydata_remote.sw4 = 1;
              mydata_remote.sw5 = 1;
              mydata_remote.sw6 = 1;
              mydata_remote.sw7 = 1;
              mydata_remote.sw8 = 1;
              Serial.println("no data");
            }
            else {
              remoteState = 1;                            
            } 


            // threshold remote data
            // some are reversed based on stick wiring in remote
            RFB = thresholdStick(mydata_remote.RFB)/1.8;   
            RLR = thresholdStick(mydata_remote.RLR)/1.8; 

            drive1 = RFB + RLR;
            drive2 = RFB - RLR;

            drive1 = constrain(drive1, -255,255);
            drive2 = constrain(drive2, -255,255);

            // drive wheels
            //left wheel

            if (drive1 > 0) {
              analogWrite(3, drive1);
              analogWrite(2, 0);
            }

            else if (drive1 < 0) {
              drive1a = abs(drive1);
              analogWrite(2, drive1a);
              analogWrite(3, 0);
            }

            else {
              analogWrite(2, 0);
              analogWrite(3, 0);
            }
            
            //right wheel

            if (drive2 > 0) {
              analogWrite(4, drive2);
              analogWrite(5, 0);
            }

            else if (drive2 < 0) {
              drive2a = abs(drive2);
              analogWrite(5, drive2a);
              analogWrite(4, 0);
            }

            else {
              analogWrite(4, 0);
              analogWrite(5, 0);
            }

            // rear linear axis

            if (mydata_remote.sw4 == 0) {
              analogWrite(6, 75);
              analogWrite(9, 0);
            }

            else if (mydata_remote.sw3 == 0) {
              analogWrite(9, 75);
              analogWrite(6, 0);
            }

            else {
              analogWrite(9, 0);
              analogWrite(6, 0);
            }

            // front axis linear

            if (mydata_remote.sw5 == 0) {
              analogWrite(11, 30);
              analogWrite(10, 0);
            }

            else if (mydata_remote.sw6 == 0) {
              analogWrite(10, 30);
              analogWrite(11, 0);
            }

            else {
              analogWrite(10, 0);
              analogWrite(11, 0);
            }

            // front linear axis

            if (mydata_remote.sw7 == 0) {
              digitalWrite(26, HIGH);
              digitalWrite(28, 0);
            }

            else if (mydata_remote.sw8 == 0) {
              digitalWrite(28, HIGH);
              digitalWrite(26, 0);
            }

            else {
              digitalWrite(26, 0);
              digitalWrite(28, 0);
            }

            // back linear axis

            if (mydata_remote.sw1 == 0) {
              digitalWrite(24, HIGH);
              digitalWrite(22, 0);
            }

            else if (mydata_remote.sw2 == 0) {
              digitalWrite(22, HIGH);
              digitalWrite(24, 0);
            }

            else {
              digitalWrite(22, 0);
              digitalWrite(24, 0);
            }

            /*

            Serial.print(RFB);
            Serial.print(" , ");
            Serial.print(RLR);
            Serial.print(" , ");
            Serial.print(mydata_remote.sw1);
            Serial.print(" , ");
            Serial.print(mydata_remote.sw2);
            Serial.print(" , ");
            Serial.print(mydata_remote.sw3);
            Serial.print(" , ");
            Serial.print(mydata_remote.sw4);
            Serial.print(" , ");
            Serial.print(mydata_remote.sw5);
            Serial.print(" , ");
            Serial.print(mydata_remote.sw6);
            Serial.print(" , ");
            Serial.print(mydata_remote.sw7);
            Serial.print(" , ");
            Serial.println(mydata_remote.sw8);

            */

           


          
        }     // end of timed loop         

   
}       // end  of main loop
