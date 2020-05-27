void setup() {
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
  pinMode(10, INPUT);

  pinMode(11, INPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  digitalWrite(4, HIGH);
  digitalWrite(7, HIGH);
  Serial.begin(9600);
}

void loop() {
  for (float pwm = 0; pwm <= 255.0; pwm += 0.1) {
    analogWrite(6, pwm);
    analogWrite(5, pwm);
    float t1 = pulseIn(10, HIGH, 25000);
    float t2 = pulseIn(11, HIGH, 25000);
    float rpm1 = 60000000 / (1400 * t1);
    float rpm2 = 60000000 / (1400 * t2);
    Serial.print(pwm);
    Serial.print(",");
    Serial.print(t1);
    Serial.print(",");
    Serial.print(rpm1);
    Serial.print(",");
    Serial.print(t2);
    Serial.print(",");
    Serial.println(rpm2);

  }
}
