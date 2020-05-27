void setup() {
  pinMode(3, INPUT);
  Serial.begin(9600);
}

void loop() {
  float timg = pulseIn(3, HIGH, 25000);  
  Serial.println(timg);
}
