
// Controller

#define TRIG_A 22               // TANK
#define ECHO_A 23               // TANK
#define TRIG_B 26               // GAS
#define ECHO_B 27               // GAS
#define TRIG_C 30               // WHEAT
#define ECHO_C 31               // WHEAT
#define TRIG_D 34               // PLANT
#define ECHO_D 35               // PLANT

#define PUMP_A 5                 // TANK  > GAS
#define PUMP_B 6                 // GAS   > WHEAT
#define PUMP_C 7                 // WHEAT > PLANT

#define LED_automa 52
#define LED_manual 50

#define TASTER_automa 46
#define TASTER_manual 44
#define TASTER_pump_A 42
#define TASTER_pump_B 40
#define TASTER_pump_C 38

boolean run_automa = false;
boolean run_manual = true;
boolean run_pump_A = false;
boolean run_pump_B = false;
boolean run_pump_C = false;

int     pumpPower  = 255;        // 0-255 = 0-5V

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

  pinMode(PUMP_A, OUTPUT);
  pinMode(PUMP_B, OUTPUT);
  pinMode(PUMP_B, OUTPUT);

  pinMode(LED_automa, OUTPUT);
  pinMode(LED_manual, OUTPUT);
  pinMode(TASTER_automa, INPUT);
  pinMode(TASTER_manual, INPUT);
  pinMode(TASTER_pump_A, INPUT);
  pinMode(TASTER_pump_B, INPUT);
  pinMode(TASTER_pump_C, INPUT);

  Serial.begin(9600);
  establishContact();
}

void loop() {
  manual();
  if (run_manual == true) {
    if (run_pump_A == true) {
      analogWrite(PUMP_A, pumpPower);
    } else {
      analogWrite(PUMP_A, 0);
    }
    if (run_pump_B == true) {
      analogWrite(PUMP_B, pumpPower);
    } else {
      analogWrite(PUMP_B, 0);
    }
    if (run_pump_C == true) {
      analogWrite(PUMP_C, pumpPower);
    } else {
      analogWrite(PUMP_C, 0);
    }
  }

  if (run_automa == true) {
    ultrasonicA();
    ultrasonicB();
    ultrasonicC();
    ultrasonicD();
    //delay(50);
    if (Serial.available() > 0) {
      streamINserver = Serial.read();
      if (streamINserver == byte('a') ) {
        control_pumpA = HIGH;
        analogWrite(PUMP_A, pumpPower);
      }
      if (streamINserver == byte('b')) {
        control_pumpA = LOW;
        analogWrite(PUMP_A, 0);
      }
      if (streamINserver == byte('c')) {
        control_pumpB = HIGH;
        analogWrite(PUMP_B, pumpPower);
      }
      if (streamINserver == byte('d')) {
        control_pumpB = LOW;
        analogWrite(PUMP_B, 0);
      }
      if (streamINserver == byte('e')) {
        control_pumpC = HIGH;
        analogWrite(PUMP_C, pumpPower);
      }
      if (streamINserver == byte('f')) {
        control_pumpC = LOW;
        analogWrite(PUMP_C, 0);
      }
      delay(50);
    } else {
      streamOUT();
      Serial.println(streamTOserver);
      delay(100);
    }
  }
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
  if (digitalRead(TASTER_pump_A) == HIGH && run_manual == true) {
    run_pump_A = true;
  } else {
    run_pump_A = false;
  }
  if (digitalRead(TASTER_pump_B) == HIGH && run_manual == true) {
    run_pump_B = true;
  } else {
    run_pump_B = false;
  }
  if (digitalRead(TASTER_pump_C) == HIGH && run_manual == true) {
    run_pump_C = true;
  } else {
    run_pump_C = false;
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
  //delay(delay_us);
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
  //delay(delay_us);
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
  //delay(delay_us);
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
  //delay(delay_us);
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
