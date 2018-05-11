// Controller

#define ECHO_A 31
#define TRIG_A 30
#define LED 52

int     maximumRange = 30;
int     minimumRange = 5;
long    dist, dura;

char    stream_server;
boolean control = LOW;

void setup()
{
  pinMode(TRIG_A, OUTPUT);
  pinMode(ECHO_A, INPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  establishContact();
}

void loop() {
  ultrasonic();
  if (Serial.available() > 0) {
    stream_server = Serial.read();
    if (stream_server == '1')
    {
      control = !control;
      digitalWrite(LED, control);
    }
    delay(100);
  }
  else {
    Serial.println(dist);
    delay(50);
  }
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("arduino");
    delay(300);
  }
}

void ultrasonic() {
  digitalWrite(TRIG_A, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_A, LOW);
  dura = pulseIn(ECHO_A, HIGH);
  dist = dura / 58.2;
  if (dist >= maximumRange) {
    dist = maximumRange;
  }
  if (dist <= minimumRange) {
    dist = 0;
  }
}
