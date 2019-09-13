/*

  Example code for connecting a Parallax GPS module to the Arduino

  Igor Gonzalez Martin. 05-04-2007
  igor.gonzalez.martin@gmail.com

  English translation by djmatic 19-05-2007

  Listen for the $GPRMC string and extract the GPS location data from this.
  Display the result in the Arduino's serial monitor.

*/

#include <string.h>
#include <ctype.h>

int ledPin = 13;                  // LED test pin
int rxPin = 16;                    // RX PIN
int txPin = 17;                    // TX TX
int byteGPS = -1;
char linea[300] = "";
char comandoGPR[7] = "$GPRMC";
int cont = 0;
int bien = 0;
int conta = 0;
int indices[13];
int counter = 0;
String a = "$GPRMC,154655,A,4428.1874,N,00440.5185,W,000.7,000.0,050407,,,A*6C";
String vel = "";
void setup() {
  pinMode(ledPin, OUTPUT);       // Initialize LED pin
  Serial.begin(115200);
  for (int i = 0; i < 300; i++) linea[i] = ' ';
  a.trim();
  for (int i = 0 ; i < a.length() ; i++){
    if(a[i] == ',') counter++;
    if(counter >=7  and counter < 8){
      if(a[i] != ',') vel += a[i];
    }
    Serial.print(a[i]);
  }
  Serial.println();
  Serial.println(vel);
  Serial.println(vel.toFloat());
}

void loop() {

}
