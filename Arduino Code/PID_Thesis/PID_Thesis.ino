#define enc_left 10
#define enc_right 11
double current_time, prev_time;

void setup() {
  encpins();
  pinMode(A0, INPUT);
  Serial.begin(9600);
}

void loop() {
  current_time = millis();

  double lspd = mot_spd(enc_left);
  double rspd = mot_spd(enc_right);

  float target = img_spd() + ((lspd + rspd) / 2);

  float lpwm = PID(target, lspd);
  float rpwm = PID(target, rspd);

  if (analogRead(A0) > 200 || analogRead(A0) < 100) {
    digitalWrite(4, LOW);
    digitalWrite(7, LOW);
    analogWrite(6, 50);
    analogWrite(5, 50);
    delay (50);
    digitalWrite(4, HIGH);
    digitalWrite(7, HIGH);
    analogWrite(6, 0);
    analogWrite(5, 0);
  }
  else {
    digitalWrite(4, HIGH);
    digitalWrite(7, HIGH);
    analogWrite(6, lpwm);
    analogWrite(5, rpwm);
  }
 
  Serial.print(target);
  Serial.print(",");
  Serial.print(lspd);
  Serial.print(",");
  Serial.println(rspd);

  prev_time = current_time;
}

double mot_spd (int pin) {
  double t = pulseIn(pin, HIGH, 25000);
  double mps = 0;
  if (t > 0) {
    mps = 2150000 / (14 * t);
  }
  return mps;
}

double img_spd() {
  float timg = pulseIn(3, HIGH, 25000);
  float dist = 0;
  float prev_dist;
  if (timg > 0) {
    dist = (108000 / timg);
  }
  float rel_spd = (dist - prev_dist) / (current_time - prev_time);
  prev_dist = dist;
  return rel_spd;
}

float PID(float target_spd, float current_spd) {
  float PID_p, PID_i, PID_d, PID_val;
  float prev_error;
  float Kp = 7;
  float Ki = 1;
  float Kd = 10;


  float error = target_spd - current_spd;

  PID_p = Kp * error;

  if (error < 200 && error > -200) {
    PID_i += Ki * error;
  }
  else {
    PID_i = 0;
  }

  PID_d = Kd * (error - prev_error) / (current_time - prev_time);

  PID_val = PID_p + PID_i + PID_d;

  prev_error = error;

  if (PID_val >= 255) {
    return 255.0;
  }
  else if (PID_val <= 0) {
    return 0.0;
  }
  else {
    return PID_val;
  }
}

void encpins() {
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
}
