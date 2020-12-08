#include <Time.h>
#include <TimeLib.h>
#include <Crc16.h>
#include <SoftwareSerial.h>
#define LED       13    // Declare LED pin
#define MASTER_EN   6   // connected to RS485 Enable pin
Crc16 crc; 
SoftwareSerial mySerial (2, 3);
String x;
String value;
int local;
class Request{
  public:
    int id;
    int fc;
  };
Request rq;
void setup() {
  pinMode(LED , OUTPUT);            // Declare LED pin as output
  pinMode(MASTER_EN , OUTPUT);      // Declare Enable pin as output
  Serial.begin(9600);               // set serial communication baudrate 
  digitalWrite(MASTER_EN , LOW);    // Make Enable pin low
  mySerial.begin(9600);             // Receiving mode ON 
}

void loop() {

  //time_t t = now();
  //Serial.println(minute(t));
  crc.clearCrc();
  digitalWrite(MASTER_EN , HIGH);     // Make Enable pin high to send Data
  delay(5);                           // required minimum delay of 5ms
  //if (i%2 == 0)
    //mySerial.println('B');                // Send character A serially
  //else 
    rq.id = 1;
    rq.fc = 1;
    mySerial.print(rq.id); 
    mySerial.println('\r'); 
    mySerial.print(rq.fc); 
    mySerial.println('\r');
  //i++;
  mySerial.flush();                     // wait for transmission of data
  
  digitalWrite(MASTER_EN , LOW);      // Receiving mode ON

 // x = mySerial.readString();
  
  x = mySerial.readStringUntil(',');
  //Serial.println(x);
  mySerial.read();
  value = mySerial.readStringUntil('\n');
  crc.updateCrc(x.toInt());
  local = crc.getCrc();
  if (local == value.toInt())
    Serial.println(x);
    
  delay(500);
}

