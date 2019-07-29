const byte interruptPin = 39;
volatile int interruptCounter = 0;
int numberOfInterrupts = 0;

#define PIN_HIGHBIT (39)
#define PIN_LOWBIT  (32) 

int state, prevState = 0; 
float count = 0;
int encoderStates[4][4] = {
 {  0, -1,  1,  0 }, 
 {  1,  0,  0, -1 }, 
 { -1,  0,  0,  1 }, 
 {  0,  1, -1,  0 }, 
};

//portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
// 
//void IRAM_ATTR handleInterrupt() {
//  portENTER_CRITICAL_ISR(&mux);
//  state = (digitalRead(PIN_HIGHBIT) << 1) | digitalRead(PIN_LOWBIT);
//  portEXIT_CRITICAL_ISR(&mux);
//}
//
//void IRAM_ATTR handleInterrupt2() {
//  portENTER_CRITICAL_ISR(&mux);
//  state = (digitalRead(PIN_HIGHBIT) << 1) | digitalRead(PIN_LOWBIT);
//  portEXIT_CRITICAL_ISR(&mux);
//}

void setup() {
 
  Serial.begin(115200);
  Serial.println("Monitoring interrupts: ");
  pinMode(39, INPUT);
  pinMode(32, INPUT);

//  attachInterrupt(digitalPinToInterrupt(39), handleInterrupt, FALLING);
//  attachInterrupt(digitalPinToInterrupt(32), handleInterrupt2, FALLING);

}
 
void loop() {
  state = (digitalRead(PIN_HIGHBIT) << 1) | digitalRead(PIN_LOWBIT);
  count += encoderStates[prevState][state] * 0.1;
  if (count >= 4){
    count = 4;
  }else if (count <= 0){
    count = 0;
  }
  if (state != prevState) {
    Serial.println(count);
  }
  prevState = state;
}
