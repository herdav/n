// Server

import processing.net.*;
import processing.serial.*;
import cc.arduino.*;

Serial  myPort;
String  portStream_arduino;
int     posA, posB, posE;
int     distA, distB;
boolean firstContact = false;

Server myServer;
String portStream_crawler;
int    posAA, posAB, posAC, posBA, posBB, posBC, posCA, posCB, posEE;
float  data_gas_last_trade, data_gas_52w_heigh, data_gas_52w_low;
float  data_wheat_last_trade, data_wheat_52w_heigh, data_wheat_52w_low;
float  data_gbp_chf, data_usd_chf;
int    val_data_gas_last_trade, val_data_gas_52w_heigh, val_data_gas_52w_low;
int    val_data_wheat_last_trade, val_data_wheat_52w_heigh, val_data_wheat_52w_low;
int    val_data_usd_chf, val_data_gbp_chf;

Levels level_gas, level_wheat;
int    pegel_gas_ref, pegel_wheat_ref;
int    store = 4;

String time;

void setup() {
  size(800, 600);
  myServer = new Server(this, 5204);
  myPort = new Serial(this, Arduino.list()[0], 9600);
  myPort.bufferUntil('\n');
  setup_graphic();
}

void draw() {
  background(50);
  if (firstContact == true) {
    Client thisClient = myServer.available();
    if (thisClient != null) {
      portStream_crawler = thisClient.readString();
      if (portStream_crawler != null) {
        stream_crawler();
      }
    }
    draw_graphic();
  }
}

void serialEvent(Serial myPort) {
  portStream_arduino = myPort.readStringUntil('\n');
  if (portStream_arduino != null) {

    stream_arduino();

    if (firstContact == false) {
      if (portStream_arduino.equals("arduino")) {
        myPort.clear();
        firstContact = true;
        myPort.write("arduino");
        println("contacted");
      }
    } else {
      println(portStream_arduino);
      if (level_gas.run == true) {
        myPort.write(byte('a'));
        println("Control: a [c1=HIGH]");
      } else {
        myPort.write(byte('b'));
        println("Control: b [c1=LOW]");
      }
      if (level_wheat.run == true) {
        myPort.write(byte('c'));
        println("Control: c [c2=HIGH]");
      } else {
        myPort.write(byte('d'));
        println("Control: d [c2=LOW]");
      }
    }
  }
}

void stream_crawler() {
  time = year() + "/" + month() + "/" + day() + " " + hour() + ":" + minute() + ":" + second();

  posAA = portStream_crawler.indexOf("aa");
  posAB = portStream_crawler.indexOf("ab");
  posAC = portStream_crawler.indexOf("ac");
  posBA = portStream_crawler.indexOf("ba");
  posBB = portStream_crawler.indexOf("bb");
  posBC = portStream_crawler.indexOf("bc");
  posCA = portStream_crawler.indexOf("ca");
  posCB = portStream_crawler.indexOf("cb");
  posEE = portStream_crawler.indexOf('#');

  data_gas_last_trade   = da(portStream_crawler.substring(posAA+2, posAB))*data_gbp_chf;
  data_gas_52w_heigh    = da(portStream_crawler.substring(posAB+2, posAC))*data_gbp_chf;
  data_gas_52w_low      = da(portStream_crawler.substring(posAC+2, posBA))*data_gbp_chf;

  data_wheat_last_trade = da(portStream_crawler.substring(posBA+2, posBB))*data_usd_chf;
  data_wheat_52w_heigh  = da(portStream_crawler.substring(posBB+2, posBC))*data_usd_chf;
  data_wheat_52w_low    = da(portStream_crawler.substring(posBC+2, posCA))*data_usd_chf;

  data_gbp_chf          = da(portStream_crawler.substring(posCB+2, posEE));
  data_usd_chf          = da(portStream_crawler.substring(posCA+2, posCB));

  val_data_gas_last_trade   = 255 - int(map(data_gas_last_trade, data_gas_52w_low, data_gas_52w_heigh, 0, 255));  
  val_data_wheat_last_trade = int(map(data_wheat_last_trade, data_wheat_52w_low, data_wheat_52w_heigh, 0, 255));

  println(portStream_crawler);
}

float da(String data) {
  float val;
  val = float(data)/1000;
  return val;  
}

void stream_arduino() {
  portStream_arduino = trim(portStream_arduino);

  if (portStream_arduino.contains("Arduino: ") == true) {
    posA = portStream_arduino.indexOf("a");
    posB = portStream_arduino.indexOf("b");
    posE = portStream_arduino.indexOf("#");

    distA   = int(portStream_arduino.substring(posA+1, posB));
    distB   = int(portStream_arduino.substring(posB+1, posE));
  }
  println(portStream_arduino);
}

void setup_graphic() {
  level_gas = new Levels(100, height-100);
  level_wheat = new Levels(300, height-100);
}

void draw_graphic() {
  pegel_gas_ref   = int(map(distA, 0, 30, 0, 255));
  pegel_wheat_ref = int(map(distB, 0, 30, 0, 255));

  fill(0);
  level_gas.display(val_data_gas_last_trade, pegel_gas_ref);
  fill(255);
  textAlign(LEFT);
  text("GAS: " + rd(data_gas_last_trade) + " CHF/mmBtu", level_gas.x, level_gas.y + 25);
  text(rd(data_gas_52w_heigh) + " - " + rd(data_gas_52w_low) + " | 0 - 255", level_gas.x, level_gas.y + 40);
  text(data_gbp_chf + " CHF/GBP", level_gas.x, level_gas.y + 55);

  fill(0);
  level_wheat.display(val_data_wheat_last_trade, pegel_wheat_ref);
  fill(255);
  textAlign(LEFT);
  text("WHEAT: " + rd(data_wheat_last_trade) + " CHF/bu", level_wheat.x, level_wheat.y + 25);
  text(rd(data_wheat_52w_low) + " - " + rd(data_wheat_52w_heigh) + " | 0 - 255", level_wheat.x, level_wheat.y + 40);
  text(data_usd_chf + " CHF/USD", level_wheat.x, level_wheat.y + 55);
}

class Levels {
  int x, y, level, cycle, int_val_pegel;
  int[] store = new int[200];
  boolean run;

  Levels(int xpos, int ypos) {
    x = xpos;
    y = ypos;
  }

  void display(int level, int pegel) {
    int val_level;
    int val_pegel;
    int delta_level_pegel;

    val_level = int(map(level, 0, 255, 0, height-150));
    val_pegel = int(map(pegel, 0, 255, 0, height-150));

    delta_level_pegel = val_level - int_val_pegel;

    if (delta_level_pegel > 0) {
      fill(0, 255, 0);
      run = true;
    } else {
      fill(255, 0, 0);
      run = false;
    }
    ellipse(x+25, 25, 10, 10);

    if (cycle < store.length) {
      store[cycle] = val_pegel;
    }
    for (int i = 0; i < store.length; i++) {
      int_val_pegel += store[i];
    }
    int_val_pegel = int_val_pegel / store.length;
    cycle++;
    if (cycle == store.length) {
      cycle = 0;
    }

    noStroke();
    rectMode(CORNERS);
    fill(200);
    rect(x, y, x + 50, 50);

    fill(250);
    rect(x, y, x + 50, y - val_level);

    fill(0, 100, 255, 100);
    rect(x, y, x + 50, y - int_val_pegel);

    fill(255);
    textAlign(CENTER);
    text(level, x + 25, y - val_level - 5);

    fill(0, 100, 255, 100);
    textAlign(CENTER);
    text(pegel, x + 25, y - int_val_pegel - 5);
  }
}

float rd(float data) {
  float val;
  val = float(int(data*1000))/1000; 
  return val;
}
