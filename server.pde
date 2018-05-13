// Server

import processing.net.*;
import processing.serial.*;
import cc.arduino.*;

Serial myPort;
String  portStream_arduino;
int     posA, posB, posE;
int     distA, distB;
boolean firstContact = false;

Server myServer;
String portStream_crawler;
int    posAA, posAB, posAC, posBA, posBB, posBC, posEE;
float  data_gas_last_trade, data_gas_52w_heigh, data_gas_52w_low;
float  data_wheat_last_trade, data_wheat_52w_heigh, data_wheat_52w_low;
int    val_data_gas_last_trade, val_data_gas_52w_heigh, val_data_gas_52w_low;
int    val_data_wheat_last_trade, val_data_wheat_52w_heigh, val_data_wheat_52w_low;
int    count_data = 0;

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
        println("Controller: a (high)");
      } else {
        myPort.write(byte('b'));
        println("Controller: b (low)");
      }
      if (level_wheat.run == true) {
        myPort.write(byte('c'));
        println("Controller: c (high)");
      } else {
        myPort.write(byte('d'));
        println("Controller: d (low)");
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
  posEE = portStream_crawler.indexOf('#');

  data_gas_last_trade   = (float(portStream_crawler.substring(posAA+2, posAB)))/1000;
  data_gas_52w_heigh    = (float(portStream_crawler.substring(posAB+2, posAC)))/1000;
  data_gas_52w_low      = (float(portStream_crawler.substring(posAC+2, posBA)))/1000;

  data_wheat_last_trade = (float(portStream_crawler.substring(posBA+2, posBB)))/1000;
  data_wheat_52w_heigh  = (float(portStream_crawler.substring(posBB+2, posBC)))/1000;
  data_wheat_52w_low    = (float(portStream_crawler.substring(posBC+2, posEE)))/1000;  

  val_data_gas_last_trade = 255 - int(map(data_gas_last_trade, data_gas_52w_low, data_gas_52w_heigh, 0, 255));  
  val_data_wheat_last_trade = int(map(data_wheat_last_trade, data_wheat_52w_low, data_wheat_52w_heigh, 0, 255));

  //println("[" + count_data + "] Received data @", time, ">>", portStream_crawler);
  //println("GAS:   ", data_gas_last_trade, "mmGBP/Btu", val_data_gas_last_trade + "/255");
  //println("WHEAT: ", data_wheat_last_trade, "USD/bu", val_data_wheat_last_trade + "/255");
  
  println(portStream_crawler);

  count_data++;
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
  text("GAS: " + data_gas_last_trade + " mmGBP/Btu", level_gas.x, level_gas.y + 25);
  text(data_gas_52w_heigh + " - " + data_gas_52w_low + " | 0 - 255", level_gas.x, level_gas.y + 40);

  fill(0);
  level_wheat.display(val_data_wheat_last_trade, pegel_wheat_ref);
  fill(255);
  textAlign(LEFT);
  text("WHEAT: " + data_wheat_last_trade + " USD/bu", level_wheat.x, level_wheat.y + 25);
  text(data_wheat_52w_low + " - " + data_wheat_52w_heigh + " | 0 - 255", level_wheat.x, level_wheat.y + 40);
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
