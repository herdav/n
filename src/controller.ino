// N/Controller
// Created 2018 by David Herren
// https://github.com/herdav/n
// Licensed under the MIT License.
// -------------------------------

#define TRIG_A 22               // TANK
#define ECHO_A 23               // TANK
#define TRIG_B 26               // GAS
#define ECHO_B 27               // GAS
#define TRIG_C 30               // WHEAT
#define ECHO_C 31               // WHEAT
#define TRIG_D 34               // PLANT
#define ECHO_D 35               // PLANT

#define PUMP_A 5                // TANK  > GAS
#define PUMP_B 6                // GAS   > WHEAT
#define PUMP_C 7                // WHEAT > PLANT

#define LED_automa 52
#define LED_manual 50

#define TASTER_automa 46
#define TASTER_manual 44
#define TASTER_pumpA 42
#define TASTER_pumpB 40
#define TASTER_pumpC 38

int     pumpPower    = 140;     // 0-255 = 0-5V
int     maximumRange = 36;      // upper limit in cm
int     minimumRange = 10;      // limit of detection in cm
int     deltaRange   = 3;       // refolow pipe

long    distA, duraA;
long    distB, duraB;
long    distC, duraC;
long    distD, duraD;

int     streamINserver;
String  streamTOserver;

boolean run_automa    = false;
boolean run_manual    = true;
boolean run_pumpA     = false;
boolean run_pumpB     = false;
boolean run_pumpC     = false;

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

  pinMode(PUMP_A, OUTPUT);
  pinMode(PUMP_B, OUTPUT);
  pinMode(PUMP_B, OUTPUT);

  pinMode(LED_automa, OUTPUT);
  pinMode(LED_manual, OUTPUT);
  pinMode(TASTER_automa, INPUT);
  pinMode(TASTER_manual, INPUT);
  pinMode(TASTER_pumpA,  INPUT);
  pinMode(TASTER_pumpB,  INPUT);
  pinMode(TASTER_pumpC,  INPUT);

  Serial.begin(9600);
  establishContact();
}

void loop() {
  manual();

  ultrasonicA();
  ultrasonicB();
  ultrasonicC();
  ultrasonicD();

  if (run_manual == true) {
    if (run_pumpA == true) {
      analogWrite(PUMP_A, pumpPower);
    } else {
      analogWrite(PUMP_A, 0);
    }
    if (run_pumpB == true) {
      analogWrite(PUMP_B, pumpPower);
    } else {
      analogWrite(PUMP_B, 0);
    }
    if (run_pumpC == true) {
      analogWrite(PUMP_C, pumpPower);
    } else {
      analogWrite(PUMP_C, 0);
    }
  }
  if (run_automa == true && Serial.available() > 0) {
    streamINserver = Serial.read();
    if (streamINserver == byte('a') && distA < maximumRange - deltaRange) {
      control_pumpA = HIGH;
      analogWrite(PUMP_A, pumpPower);
    }
    if (streamINserver == byte('b')) {
      control_pumpA = LOW;
      analogWrite(PUMP_A, 0);
    }
    if (streamINserver == byte('c') && distB < maximumRange - deltaRange) {
      control_pumpB = HIGH;
      analogWrite(PUMP_B, pumpPower);
    }
    if (streamINserver == byte('d')) {
      control_pumpB = LOW;
      analogWrite(PUMP_B, 0);
    }
    if (streamINserver == byte('e') && distC < maximumRange - deltaRange) {
      control_pumpC = HIGH;
      analogWrite(PUMP_C, pumpPower);
    }
    if (streamINserver == byte('f')) {
      control_pumpC = LOW;
      analogWrite(PUMP_C, 0);
    }
  } else {
    streamOUT();
    Serial.println(streamTOserver);
  }
  delay(50);
}

void manual() {
  if (digitalRead(TASTER_automa) == HIGH || run_automa == true) {
    run_manual = false;
    digitalWrite(LED_manual, LOW);
    run_automa = true;
    digitalWrite(LED_automa, HIGH);
  }
  if (digitalRead(TASTER_manual) == HIGH || run_manual == true) {
    run_manual = true;
    digitalWrite(LED_manual, HIGH);
    run_automa = false;
    digitalWrite(LED_automa, LOW);
  }
  if (digitalRead(TASTER_pumpA) == HIGH && run_manual == true) {
    run_pumpA = true;
  } else {
    run_pumpA = false;
  }
  if (digitalRead(TASTER_pumpB) == HIGH && run_manual == true) {
    run_pumpB = true;
  } else {
    run_pumpB = false;
  }
  if (digitalRead(TASTER_pumpC) == HIGH && run_manual == true) {
    run_pumpC = true;
  } else {
    run_pumpC = false;
  }
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("!!");
    delay(200);
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
}

void streamOUT() {
  streamTOserver = normalizeData(distA, distB, distC, distD);
}

String normalizeData(int dataA, int dataB, int dataC, int dataD) {
  String A   = String(dataA);
  String B   = String(dataB);
  String C   = String(dataC);
  String D   = String(dataD);
  String ret = String("::") + String('a') + A + String('b') + B + String('c') + C + String('d') + D + String('#');
  return ret;
}
