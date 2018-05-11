// Server

import processing.net.*;
import processing.serial.*;
import cc.arduino.*;

Serial myPort;
String portStream_arduino;
boolean firstContact = false;

Server myServer;
String portStream_crawler;
int posAA, posAB, posAC, posBA, posBB, posBC, posEnd;
float data_gas_last_trade, data_gas_52w_heigh, data_gas_52w_low;
float data_wheat_last_trade, data_wheat_52w_heigh, data_wheat_52w_low;
int val_data_gas_last_trade, val_data_gas_52w_heigh, val_data_gas_52w_low;
int val_data_wheat_last_trade, val_data_wheat_52w_heigh, val_data_wheat_52w_low;
int count_data = 0;

Levels level_gas, level_wheat;
int pegel_gas_ref, pegel_wheat_ref;

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
    //trim whitespace and formatting characters (like carriage return)
    portStream_arduino = trim(portStream_arduino);
    println(portStream_arduino);

    //look for our 'A' string to start the handshake
    //if it's there, clear the buffer, and send a request for data
    if (firstContact == false) {
      if (portStream_arduino.equals("arduino")) {
        myPort.clear();
        firstContact = true;
        myPort.write("arduino");
        println("contacted");
      }
    } else { //if we've already established contact, keep getting and parsing data
      println(portStream_arduino);
      if (mousePressed == true) 
      {
        myPort.write('1');
        println('1');
      }
      //myPort.write("A");
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
  posEnd = portStream_crawler.indexOf('#');

  data_gas_last_trade   = (float(portStream_crawler.substring(posAA+2, posAB)))/1000;
  data_gas_52w_heigh    = (float(portStream_crawler.substring(posAB+2, posAC)))/1000;
  data_gas_52w_low      = (float(portStream_crawler.substring(posAC+2, posBA)))/1000;

  data_wheat_last_trade = (float(portStream_crawler.substring(posBA+2, posBB)))/1000;
  data_wheat_52w_heigh  = (float(portStream_crawler.substring(posBB+2, posBC)))/1000;
  data_wheat_52w_low    = (float(portStream_crawler.substring(posBC+2, posEnd)))/1000;  

  val_data_gas_last_trade = 255 - int(map(data_gas_last_trade, data_gas_52w_low, data_gas_52w_heigh, 0, 255));  
  val_data_wheat_last_trade = int(map(data_wheat_last_trade, data_wheat_52w_low, data_wheat_52w_heigh, 0, 255));

  println("[" + count_data + "] Received data @", time, ">>", portStream_crawler);
  println("GAS:   ", data_gas_last_trade, "mmGBP/Btu", val_data_gas_last_trade + "/255");
  println("WHEAT: ", data_wheat_last_trade, "USD/bu", val_data_wheat_last_trade + "/255");

  count_data++;
}

void setup_graphic() {
  level_gas = new Levels(100, height-100);
  level_wheat = new Levels(300, height-100);
}

void draw_graphic() {
  pegel_gas_ref = int(map(float(portStream_arduino), 0, 30, 0, 255));
  pegel_wheat_ref = pegel_gas_ref;

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
  int x, y, level;

  Levels(int xpos, int ypos) {
    x = xpos;
    y = ypos;
  }

  void display(int level, int pegel) {
    int val_level, val_pegel;

    val_level = int(map(level, 0, 255, 0, height-100));
    val_pegel = int(map(pegel, 0, 255, 0, height-100));

    noStroke();
    rectMode(CORNERS);
    fill(200);
    rect(x, y, x + 50, 50);

    fill(250);
    rect(x, y, x + 50, y - val_level);

    fill(0, 100, 255, 100);
    rect(x, y, x + 50, y - val_pegel);

    fill(255);
    textAlign(CENTER);
    text(level, x + 25, y - val_level - 5);

    fill(0, 100, 255, 100);
    textAlign(CENTER);
    text(pegel, x + 25, y - val_pegel - 5);
  }
}
