void setup() {
  Serial.begin(9600);
}

int a,b; 
void loop() {
  b = analogRead(A0);
  Serial.println(b - a, DEC);
  a = b;
}
