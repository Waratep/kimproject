#define PIN_HIGHBIT (39)
#define PIN_LOWBIT  (32)

int state, prevState = 0, count = 0;

int encoderStates[4][4] = {
 {  0, -1,  1,  0 }, 
 {  1,  0,  0, -1 }, 
 { -1,  0,  0,  1 }, 
 {  0,  1, -1,  0 }, 
};

void setup() {
  pinMode(PIN_HIGHBIT, INPUT);
  pinMode(PIN_LOWBIT, INPUT);
  Serial.begin(115200); 
}

void loop() {
  state = (digitalRead(PIN_HIGHBIT) << 1) | digitalRead(PIN_LOWBIT);
  count += encoderStates[prevState][state];
  if (count >= 96){
    count = 96;
  }else if (count <= 4){
    count = 4;
  }
  if (state != prevState) {
//    Serial.println(map(count,0,96,0,3));
    Serial.println(count);
  }


  prevState = state;
}
