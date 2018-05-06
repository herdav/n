// Server

import processing.net.*;
import processing.serial.*;
import cc.arduino.*;

Arduino arduino;
Server myServer;
Serial myPort;

String portStream;
int posB, posEnd;
float data_a, data_b;
int val_data_a, val_data_b;
int data_count = 0;
String time;
int rate;
int store = 20;
int[] rate_store = new int[store];
PVector[] graph = new PVector[store];

void setup() {
  size(800, 600);
  myServer = new Server(this, 5204);
  arduino = new Arduino(this, Arduino.list()[0], 57600);

  for (int i = 0; i < store; i++) {
    graph[i] = new PVector();
    graph[i].x = 50+(width-100)/(store-1)*i;
  }
}

void draw() {
  background(200);
  client();
  time();
  noStroke();
  fill(255, 0, 0);
  for (int i = 0; i < store; i++) {
    ellipse(graph[i].x, height/2-graph[i].y, 10, 10);
  }
}

void client() {
  Client thisClient = myServer.available();
  if (thisClient !=null) {
    portStream = thisClient.readString();
    if (portStream != null) {
      println("["+data_count+"] Received data @", time, ">>", portStream);
      posB = portStream.indexOf('b');
      posEnd = portStream.indexOf('#');
      data_a = (float(portStream.substring(1, portStream.indexOf('b'))))/100;
      data_b = (float(portStream.substring(posB + 1, posEnd)))/100;
      val_data_a = int(map(data_a, 1, 4, 0, 255));
      val_data_b = int(map(data_b, 3, 7, 0, 255));     
      println("GAS:   ", data_a, "USD/Btu");
      println("WHEAT: ", data_b, "USD/bu");
      rate_store[data_count] = val_data_b - val_data_a;
      graph[data_count].y = rate_store[data_count];
      data_count++;
      if (data_count == store) {
        data_count = 0;
      }
    }
  }
}

void time() {
  time = year() + "/" + month() + "/" + day() + " " + hour() + ":" + minute() + ":" + second();
}
