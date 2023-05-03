
//DAC CONNECTION PLAN
//use PORTA for DAC1 => PIN22(LSB),PIN23,PIN24,PIN25,PIN26,PIN27,PIN28,PIN29(MSB)
//use PORTC for DAC2 => PIN37(LSB),PIN36,PIN35,PIN34,PIN33,PIN32,PIN31,PIN30(MSB)
//use PORTL for DAC3 => PIN49(LSB),PIN48,PIN47,PIN46,PIN45,PIN44,PIN43,PIN42(MSB)

//CONTROL INPUT (Pull Down)
//use A8 as DAC1 left
//use A9 as DAC1 right
//use A10 as DAC2 left
//use A11 as DAC2 right
//use A12 as DAC3 left
//use A13 as DAC3 right


//use timer 3
inline void set_dac1(char value){
  PORTA = (unsigned char)value;
}

inline void set_dac2(char value){
  PORTC = (unsigned char)value;
}

inline void set_dac3(char value){
  PORTL = (unsigned char)value;
}

void setup() {
  // SET DAC Pins to output
  DDRA=0xFF;
  DDRC=0xFF;
  DDRL=0xFF;
  //Periode = FCPU / N / 2^BITS_DAC ... N=Prescaler
  TCCR4A = 0;
  TCCR4B = 0;
  OCR4A = 12500; //N=1, FCPU=16MHz, BITS_DAC=8 => f=5Hz
  TCCR4B |= (1<<WGM42)|(0<<CS42)|(0<<CS41)|(1<<CS40); //Mode=CTC, N=1
  TIMSK4 |= (1<<OCIE4A);
 
  pinMode(A8,INPUT_PULLUP); 
  pinMode(A9,INPUT_PULLUP); 
  pinMode(A10,INPUT_PULLUP); 
  pinMode(A11,INPUT_PULLUP); 
  pinMode(A12,INPUT_PULLUP); 
  pinMode(A13,INPUT_PULLUP); 
}

volatile char incr[3] = {0,0,0};
char counter[3]={0,0,0};

void loop() {
  // Check Buttons
 if(digitalRead(A8) == 0){
   incr[0] = 1;
 }else if(digitalRead(A9) == 0){
   incr[0] = -1;
 }else{
   incr[0] = 0;
 } 
//=> short version: 
  
  // incr[0] = (digitalRead(A8) == 0) ? 1 : (digitalRead(A9) == 0) ? -1 : 0;
  // incr[1] = (digitalRead(A10) == 0) ? 1 : (digitalRead(A11) == 0) ? -1 : 0;
  // incr[2] = (digitalRead(A12) == 0) ? 1 : (digitalRead(A13) == 0) ? -1 : 0;
  
  // delay(100);
}

ISR (TIMER4_COMPA_vect){
  set_dac1(counter[0]+=incr[0]);
  set_dac2(counter[1]+=incr[1]);
  set_dac3(counter[2]+=incr[2]);
}
