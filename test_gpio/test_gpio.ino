

void setup() {
  // put your setup code here, to run once:
//  pinMode(16,OUTPUT);
  pinMode(36,INPUT);
  pinMode(39,INPUT);
  pinMode(32,INPUT);

  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
//  digitalWrite(16,1);
//  delay(500);
//  digitalWrite(16,0);
//  delay(500);
  Serial.print(digitalRead(36));
  Serial.print("   ");
  Serial.print(digitalRead(39));
  Serial.print("   ");
  Serial.println(digitalRead(32));

  if(digitalRead(39)){
    while(digitalRead(32));
    Serial.println("<");
    delay(100);
  }
  else if(digitalRead(32)){
    while(digitalRead(39));
    Serial.println(">");
    delay(100);
  }  
}
