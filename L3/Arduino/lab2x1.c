void setup() {
 
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(4, OUTPUT);
}


void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  
  digitalWrite(4, LOW);  
  delay(1000);                     
  digitalWrite(LED_BUILTIN, LOW);   
  digitalWrite(4, HIGH);   
  delay(1000);                     
}