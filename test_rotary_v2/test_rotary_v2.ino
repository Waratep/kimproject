/* Arduino New Rotary Encoder Debounce
 
Created by Yvan / https://Brainy-Bits.com

This code is in the public domain...

You can: copy it, use it, modify it, share it or just plain ignore it!
Thx!

*/

// Rotary Encoder Module connections
const int PinSW=36;   // Rotary Encoder Switch
const int PinDT=39;    // DATA signal
const int PinCLK=32;    // CLOCK signal

// Variables to debounce Rotary Encoder
long TimeOfLastDebounce = 0;
int DelayofDebounce = 0.01;

// Store previous Pins state
int PreviousCLK;   
int PreviousDATA;

int displaycounter=0; // Store current counter value



void setup() {
  // Put current pins state in variables
  PreviousCLK=digitalRead(PinCLK);
  PreviousDATA=digitalRead(PinDT);

  // Set the Switch pin to use Arduino PULLUP resistors
  pinMode(PinSW, INPUT_PULLUP);
  Serial.begin(115200);
}
void loop() {
  // If enough time has passed check the rotary encoder
  if ((millis() - TimeOfLastDebounce) > DelayofDebounce) {
    
    check_rotary();  // Rotary Encoder check routine below
    
    PreviousCLK=digitalRead(PinCLK);
    PreviousDATA=digitalRead(PinDT);
    
    TimeOfLastDebounce=millis();  // Set variable to current millis() timer
  }
  
  // Check if Rotary Encoder switch was pressed
  if (digitalRead(PinSW) == LOW) {
    displaycounter=0;  // Reset counter to zero
  }

}


// Check if Rotary Encoder was moved
void check_rotary() {

 if ((PreviousCLK == 0) && (PreviousDATA == 1)) {
    if ((digitalRead(PinCLK) == 1) && (digitalRead(PinDT) == 0)) {
      displaycounter++;
        Serial.println(displaycounter);

    }
    if ((digitalRead(PinCLK) == 1) && (digitalRead(PinDT) == 1)) {
      displaycounter--;
        Serial.println(displaycounter);

    }
  }

if ((PreviousCLK == 1) && (PreviousDATA == 0)) {
    if ((digitalRead(PinCLK) == 0) && (digitalRead(PinDT) == 1)) {
      displaycounter++;
        Serial.println(displaycounter);

    }
    if ((digitalRead(PinCLK) == 0) && (digitalRead(PinDT) == 0)) {
      displaycounter--;
        Serial.println(displaycounter);

    }
  }

if ((PreviousCLK == 1) && (PreviousDATA == 1)) {
    if ((digitalRead(PinCLK) == 0) && (digitalRead(PinDT) == 1)) {
      displaycounter++;
        Serial.println(displaycounter);

    }
    if ((digitalRead(PinCLK) == 0) && (digitalRead(PinDT) == 0)) {
      displaycounter--;
        Serial.println(displaycounter);

    }
  }  

if ((PreviousCLK == 0) && (PreviousDATA == 0)) {
    if ((digitalRead(PinCLK) == 1) && (digitalRead(PinDT) == 0)) {
      displaycounter++;
        Serial.println(displaycounter);

    }
    if ((digitalRead(PinCLK) == 1) && (digitalRead(PinDT) == 1)) {
      displaycounter--;
        Serial.println(displaycounter);

    }
  }            
 }
