#include <Crc16.h>


#define LED       13
#define SLAVE_EN  6
Crc16 crc; 
#include <SoftwareSerial.h>
SoftwareSerial mySerial (2, 3);
float rpm = 123.0;
int rec_id,rec_fc;
int id = 1,time_check;
String value;
void setup() {
  pinMode(LED , OUTPUT);                        // Declare LED pin as output
  pinMode(SLAVE_EN , OUTPUT);                   // Declare Enable pin as output
  Serial.begin(9600);                           // set serial communication baudrate 
  digitalWrite(SLAVE_EN , LOW);                 // Make Enable pin low
  mySerial.begin(9600);                                              // Receiving mode ON 
}

void loop() {
 
   rec_id = mySerial.parseInt();
   rec_fc = mySerial.parseInt();
    if(rec_id == id)                    // if available data is A
    {
      crc.clearCrc();
      crc.updateCrc(rpm);
      value = crc.getCrc();
      time_check = 
      digitalWrite(SLAVE_EN , HIGH);     // Make Enable pin high to send Data
      delay(5);                           // required minimum delay of 5ms
      mySerial.print(rpm);                // Send character A serially
      mySerial.print(',');
      mySerial.println(value);
      
      Serial.println(value);
      mySerial.flush();                     // wait for transmission of data
      delay(5);
      digitalWrite(SLAVE_EN , LOW);      // Receiving mode ON
    }
  
}
