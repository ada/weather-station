#define pwmPin 7

int prevVal = LOW;
long th, tl, h, l, ppm;

void setup() {
  Serial.begin(9600);
  pinMode(pwmPin, INPUT);
}

void loop() {
  long tt = millis();
  int myVal = digitalRead(pwmPin);

  if (myVal == HIGH) {

    if (myVal != prevVal) {
      h = tt;
      tl = h - l;
      prevVal = myVal;
    }
  }  else {

    if (myVal != prevVal) {
      l = tt;
      th = l - h;
      prevVal = myVal;
      ppm = 2000 * (th - 2) / (th + tl - 4);
      Serial.println("PPM = " + String(ppm));
    }
  }
}
