// Controller

#define TRIG_A 30               // TANK
#define ECHO_A 31               // TANK
#define TRIG_B 34               // GAS
#define ECHO_B 35               // GAS
#define TRIG_C 38               // WHEAT
#define ECHO_C 39               // WHEAT
#define TRIG_D 42               // PLANT
#define ECHO_D 43               // PLANT

#define pumpA 5                 // TANK  > GAS
#define pumpB 6                 // GAS   > WHEAT
#define pumpC 7                 // WHEAT > PLANT

int     pumpPower = 255;        // 0-255 = 0-5V

int     maximumRange = 36;      // upper limit in cm
int     minimumRange = 6;       // limit of detection in cm
long    distA, duraA;
long    distB, duraB;
long    distC, duraC;
long    distD, duraD;

int     streamINserver;
String  streamTOserver;

boolean control_pumpA = LOW;
boolean control_pumpB = LOW;
boolean control_pumpC = LOW;

void setup()
{
  pinMode(TRIG_A, OUTPUT);
  pinMode(ECHO_A, INPUT);
  pinMode(TRIG_B, OUTPUT);
  pinMode(ECHO_B, INPUT);
  pinMode(TRIG_C, OUTPUT);
  pinMode(ECHO_C, INPUT);
  pinMode(TRIG_D, OUTPUT);
  pinMode(ECHO_D, INPUT);

  pinMode(pumpA, OUTPUT);
  pinMode(pumpB, OUTPUT);
  pinMode(pumpC, OUTPUT);

  Serial.begin(9600);
  establishContact();
}

void loop() {
  ultrasonicA();
  ultrasonicB();
  ultrasonicC();
  ultrasonicD();

  if (Serial.available() > 0) {
    streamINserver = Serial.read();
    if (streamINserver == byte('a') ) {
      control_pumpA = HIGH;
      analogWrite(pumpA, pumpPower);
    }
    if (streamINserver == byte('b')) {
      control_pumpA = LOW;
      analogWrite(pumpA, 0);
    }
    if (streamINserver == byte('c')) {
      control_pumpB = HIGH;
      analogWrite(pumpB, pumpPower);
    }
    if (streamINserver == byte('d')) {
      control_pumpB = LOW;
      analogWrite(pumpB, 0);
    }
    if (streamINserver == byte('e')) {
      control_pumpC = HIGH;
      analogWrite(pumpC, pumpPower);
    }
    if (streamINserver == byte('f')) {
      control_pumpC = LOW;
      analogWrite(pumpC, 0);
    }
    delay(50);
  }
  else {
    streamOUT();
    Serial.println(streamTOserver);
    delay(100);
  }
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("!!");
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
    distA = minimumRange;
  }
  delay(50);
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
    distB = minimumRange;
  }
  delay(50);
}

void ultrasonicC() {
  digitalWrite(TRIG_C, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_C, LOW);
  duraC = pulseIn(ECHO_C, HIGH);
  distC = duraC / 58.2;
  if (distC >= maximumRange) {
    distC = maximumRange;
  }
  if (distC <= minimumRange) {
    distC = minimumRange;
  }
  delay(50);
}

void ultrasonicD() {
  digitalWrite(TRIG_D, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_D, LOW);
  duraD = pulseIn(ECHO_D, HIGH);
  distD = duraD / 58.2;
  if (distD >= maximumRange) {
    distD = maximumRange;
  }
  if (distD <= minimumRange) {
    distD = minimumRange;
  }
  delay(50);
}

void streamOUT() {
  streamTOserver = normalizeData(distA, distB, distC, distD);
}

String normalizeData(int dataA, int dataB, int dataC, int dataD) {
  String A = String(dataA);
  String B = String(dataB);
  String C = String(dataC);
  String D = String(dataD);
  String ret = String("::") + String('a') + A + String('b') + B + String('c') + C + String('d') + D + String('#');
  return ret;
}
