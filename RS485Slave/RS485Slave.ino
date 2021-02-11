#include <Crc16.h>


#define LED       13
#define SLAVE_EN  6
Crc16 crc; 
#include <SoftwareSerial.h>
SoftwareSerial mySerial (2, 3);
float rpm_rec = 123.0;
int rec_id,rec_fc;
//int id = 1,time_check;
String value;
float rpm1[4] = {20.3,10.6,9.8,59.2},rpm2[4] = {41.2,69.4,42.3,88.1},rpm3[4] = {33.3,11.1,0,44.4},rpm4[4] = {102.3,45.9,102.0,63.6};
float *rpm;
int i;

volatile uint8_t edge = 0;  //uint8_t
volatile unsigned int overflows = 0;
volatile unsigned long tstart = 0;
volatile unsigned long tstop = 0;
volatile float frequency = 0;
//volatile float rpm = 0;
volatile unsigned long tnow = 0;


void setup() {
  pinMode(7, INPUT); // This is the analog comparator negative input.
  SREG |= _BV(7);
  ACSR |= _BV(1) | _BV(3) | _BV(6);
  ADCSRB |=_BV(6);
  TCCR1A = 0;
  TIMSK1 |= _BV(TOIE1); // enable overflow interrupt.
  TCCR1B |= _BV(CS10);// with no prescaler}
  delay(10);
  
  pinMode(LED , OUTPUT);                        // Declare LED pin as output
  pinMode(SLAVE_EN , OUTPUT);                   // Declare Enable pin as output
  Serial.begin(9600);                           // set serial communication baudrate 
  digitalWrite(SLAVE_EN , LOW);                 // Make Enable pin low
  mySerial.begin(9600);                                              // Receiving mode ON 
}

void loop() {
 
   rec_id = mySerial.parseInt();
   rec_fc = mySerial.parseInt();
   //rpm_rec = RPM();

   decision(rec_id);
    /*if(rec_id == 1)                    // if available data is A
    {
      crc.clearCrc();
      crc.updateCrc(rpm_rec);
      value = crc.getCrc();
      //time_check = 
      digitalWrite(SLAVE_EN , HIGH);     // Make Enable pin high to send Data
      delay(5);                           // required minimum delay of 5ms
      
      mySerial.print(rpm_rec);                // Send character A serially
      mySerial.print(',');
      mySerial.println(value);
      
      Serial.println(rpm_rec);
      mySerial.flush();                     // wait for transmission of data
      delay(5);
      digitalWrite(SLAVE_EN , LOW);      // Receiving mode ON
    }*/
  
}
void decision (int id){

  if(id == 1)                    // if available data is A
    rpm = rpm1;
   else if (id == 2)
    rpm = rpm2;
   else if (id == 3)
    rpm = rpm3;
   else if (id == 4)
    rpm = rpm4;
    
      crc.clearCrc();
      crc.updateCrc(rpm_rec);
      value = crc.getCrc();
      //time_check = 
      digitalWrite(SLAVE_EN , HIGH);     // Make Enable pin high to send Data
      delay(5);                           // required minimum delay of 5ms

      for (i=0;i<4;i++){
        mySerial.print(rpm[i]);                // Send character A serially
        mySerial.println('\r');
      }
      //mySerial.print(',');
      //mySerial.println(value);
      
      for (i=0;i<4;i++){
        Serial.print(rpm[i]);
      }
      Serial.println();
      mySerial.flush();                     // wait for transmission of data
      delay(5);
      digitalWrite(SLAVE_EN , LOW);      // Receiving mode ON
    
  
  
  
  }



  
float RPM(){
  edge = 0;
  overflows = 0;
  ACSR |= _BV(1) | _BV(3) | _BV(6);
  ADCSRB |=_BV(ACME);
  ADCSRA &=~_BV(ADEN);
  ADMUX |= B0000001;  
  TCCR1A = 0;
  TIMSK1 |= _BV(TOIE1); // enable overflow interrupt. 
  TCCR1B |= _BV(CS10);// with no prescaler}
  
while (edge < (2)) {
}
//Serial.println(frequency);
Serial.print("RPM= ");
//Serial.print(rpm);
Serial.print(" , ");
Serial.println(frequency);
delay(300);
//return rpm;
}


ISR(TIMER1_OVF_vect)
{
  overflows += 1;
}

ISR(ANALOG_COMP_vect)
{
edge+=1;

if (edge == 1) { // Start counting edges.
tstart = overflows*65536 + TCNT1;

}
else if (edge == 2) { // Stop counting edges.

tstop = overflows*65536 + TCNT1;
frequency = ((float)16000000/(float)(tstop-tstart));
//rpm = (float)frequency*(float)60;
ACSR = 0;
TCCR1B = 0;
}
  }

