/*volatile uint8_t edge = 0;  //uint8_t
volatile unsigned int overflows = 0;
volatile unsigned long tstart = 0;
volatile unsigned long tdelay = 0;
volatile unsigned long tstop = 0;
volatile float frequency = 0;
volatile float rpm = 0;
volatile unsigned long tnow = 0;
volatile unsigned int i=0;
volatile unsigned int time_now=0;
int period = 900;
void setup(void) {
  pinMode(8, OUTPUT); // This is the analog comparator negative input.
  digitalWrite(8,LOW);
  pinMode(3,OUTPUT);
  analogWrite(3,125);
  pinMode(5,OUTPUT);
  analogWrite(5,125);
  
  SREG |= _BV(7);
  ACSR |= _BV(1) | _BV(3) | _BV(6);
  
  ADCSRB |=_BV(6);
  
  TCCR1A = 0;
  
  TIMSK1 |= _BV(TOIE1); // enable overflow interrupt.
  TCCR1B |= _BV(CS10);// with no prescaler}
  delay(10);

  Serial.begin(9600); // For printing the frequency to the terminal
  
}

void loop(void) {
  
  edge = 0;
  overflows = 0;
  ACSR |= _BV(1) | _BV(3) | _BV(6);
  ADCSRB |=_BV(ACME);
  ADCSRA &=~_BV(ADEN);
  
  if (i==0)
    ADMUX = B0000000;  
  else if (i==1)
    ADMUX = B0000001;
  else if (i==2)
    ADMUX = B0000010;  
  else if (i==3)
    ADMUX = B0000011;
    
  TCCR1A = 0;
  TIMSK1 |= _BV(TOIE1); // enable overflow interrupt. 
  TCCR1B |= _BV(CS10);// with no prescaler}

    
  time_now = millis();
  tdelay = millis();
while (edge < (2)) {
  if (millis() - tdelay > 300){
    edge = 2;
    frequency = 0;
    }
}


//Serial.println(frequency);

  time_now = millis();
  //Serial.print("RPM= ");
  //Serial.print(i);
  //Serial.print(",");
  Serial.println(frequency);
while (millis() < time_now + period){}

//delay(30);
i = (i+1)%4;
 
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
rpm = (float)frequency*(float)60;

ACSR = 0;
TCCR1B = 0;

}
}*/

volatile uint8_t edge = 0;  //uint8_t
volatile unsigned int overflows = 0;
volatile unsigned long tstart = 0;
volatile unsigned long tdelay = 0;
volatile unsigned long tstop = 0;
volatile float frequency = 0;
volatile float rpm = 0;
volatile unsigned long tnow = 0;
volatile unsigned int i=0;
volatile unsigned int time_now=0;
int period = 50;


uint8_t ledArray[] = {4, 5, 6, 7};

float frequencyarray[4]={0,0,0,0};

void setup(void) {
  pinMode(8, OUTPUT); // This is the analog comparator negative input.
  digitalWrite(8,LOW);
  for (byte j = 0; j < sizeof ledArray; j++)
  {
    pinMode(ledArray[j], OUTPUT);
    digitalWrite(ledArray[j], LOW);
  }
  
  
  SREG |= _BV(7);
  ACSR |= _BV(1) | _BV(3) | _BV(6);
  
  ADCSRB |=_BV(6);
  
  TCCR1A = 0;
  
  TIMSK1 |= _BV(TOIE1); // enable overflow interrupt.
  TCCR1B |= _BV(CS10);// with no prescaler}
  delay(10);

  Serial.begin(9600); // For printing the frequency to the terminal
  
}

void loop(void) {
  
  edge = 0;
  overflows = 0;
  ACSR |= _BV(1) | _BV(3) | _BV(6);
  ADCSRB |=_BV(ACME);
  ADCSRA &=~_BV(ADEN);
  
  if (i==0)
    ADMUX = B0000001;  
  else if (i==1)
    ADMUX = B0000010;
  else if (i==2)
    ADMUX = B0000000;  
  else if (i==3)
    ADMUX = B0000011;
    
  TCCR1A = 0;
  TIMSK1 |= _BV(TOIE1); // enable overflow interrupt. 
  TCCR1B |= _BV(CS10);// with no prescaler}

    
  time_now = millis();
  tdelay = millis();
while (edge < (2)) {
  if (millis() - tdelay > 300){
    edge = 2;
    frequency = 0;
    }
}


//Serial.println(frequency);

  time_now = millis();
  //Serial.print("RPM= ");
  //Serial.print(i);
  //Serial.print(",");
  if (frequency < 300){
    

 frequencyarray[i]=frequency*60;
  
  }
  if(i == 3){
     for (byte l = 0; l < sizeof ledArray; l++){
    Serial.print( frequencyarray[l]);
    if (l < 3) Serial.print(",");
   }
   Serial.println();
    }

  if((frequency *60) > 5000)  digitalWrite(ledArray[i], HIGH);
  else digitalWrite(ledArray[i], LOW);
  
while (millis() < time_now + period){}

//delay(30);
i = (i+1)%4;
 
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

ACSR = 0;
TCCR1B = 0;

}
}
