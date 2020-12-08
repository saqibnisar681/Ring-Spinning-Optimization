volatile uint8_t edge = 0;  //uint8_t
volatile unsigned int overflows = 0;
volatile unsigned long tstart = 0;
volatile unsigned long tstop = 0;
volatile float frequency = 0;
volatile float rpm = 0;
volatile unsigned long tnow = 0;
void setup(void) {
  pinMode(7, INPUT); // This is the analog comparator negative input.


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
  ADMUX |= B0000001;  
  TCCR1A = 0;
  TIMSK1 |= _BV(TOIE1); // enable overflow interrupt. 
  TCCR1B |= _BV(CS10);// with no prescaler}
  
while (edge < (2)) {
}
//Serial.println(frequency);
Serial.print("RPM= ");
Serial.print(rpm);
Serial.print(" , ");
Serial.println(frequency);
delay(300);
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
}
