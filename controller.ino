// Controller

#define TRIG_A 30
#define ECHO_A 31
#define TRIG_B 34
#define ECHO_B 35

#define LED1 52
#define LED2 53

int     maximumRange = 30;
int     minimumRange = 5;
long    distA, duraA;
long    distB, duraB;

int     streamINserver;
String  streamTOserver;

boolean control1 = LOW;
boolean control2 = LOW;

void setup()
{
  pinMode(TRIG_A, OUTPUT);
  pinMode(ECHO_A, INPUT);
  pinMode(TRIG_B, OUTPUT);
  pinMode(ECHO_B, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  
  Serial.begin(9600);
  establishContact();
}

void loop() {
  ultrasonicA();
  ultrasonicB();

  if (Serial.available() > 0) {
    streamINserver = Serial.read();
    if (streamINserver == byte('a') ) {
      control1 = HIGH;
      digitalWrite(LED1, control1);
    }
    if (streamINserver == byte('b')) {
      control1 = LOW;
      digitalWrite(LED1, control1);
    }
    if (streamINserver == byte('c')) {
      control2 = HIGH;
      digitalWrite(LED2, control2);
    }
    if (streamINserver == byte('d')) {
      control2 = LOW;
      digitalWrite(LED2, control2);
    }
    delay(100);
  }
  else {
    streamOUT();
    Serial.println(streamTOserver);
    //Serial.println(streamINserver);
    delay(50);
  }
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("arduino");
    delay(300);
  }
}

void ultrasonicA() {
  digitalWrite(TRIG_A, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_A, LOW);
  duraA = pulseIn(ECHO_A, HIGH);
  distA = duraA / 58.2;
  if (distA >= maximumRange) {
    distA = maximumRange;
  }
  if (distA <= minimumRange) {
    distA = 0;
  }
}

void ultrasonicB() {
  digitalWrite(TRIG_B, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_B, LOW);
  duraB = pulseIn(ECHO_B, HIGH);
  distB = duraB / 58.2;
  if (distB >= maximumRange) {
    distB = maximumRange;
  }
  if (distB <= minimumRange) {
    distB = 0;
  }
}

void streamOUT() {
  streamTOserver = normalizeData(distA, distB);
}

String normalizeData(int a, int b) {
  String A = String(a);
  String B = String(b);
  String ret = String("Arduino: ") + String('a') + A + String('b') + B + String('#');
  return ret;
}
