import processing.serial.*; //import the Serial library
import net.java.games.input.*;
import org.gamecontrolplus.*;
import org.gamecontrolplus.gui.*;

ControlDevice cont;
ControlIO control;
 Serial myPort;  //the Serial port object

String val;
// since we're doing serial handshaking, 
// we need to check if we've heard from the microcontroller
boolean firstContact = false;
int count = 7000;

int status;
int leftDrive;
int rightDrive;
int lift;
int dig;
int dump;


void setup() {
  size(200, 200); //make our canvas 200 x 200 pixels big
  //  initialize your serial port and set the baud rate to 9600
  myPort = new Serial(this, Serial.list()[0], 9600);
  myPort.bufferUntil('\n'); 
  control = ControlIO.getInstance(this);
  cont = control.getMatchedDevice("ManualControl");
}

public void getUserInput(){
  leftDrive = (int)map(cont.getSlider("LeftDrive").getValue(), -1, 1, 0, 31);
  rightDrive = (int)map(cont.getSlider("RightDrive").getValue(), -1, 1, 31, 0);
  
  if(cont.getButton("LiftUp").pressed()){
    if(cont.getSlider("LiftDown").getValue() > 0) lift = 1;
    else lift = 2;
  }
  else{
    if(cont.getSlider("LiftDown").getValue() > 0) lift = 0;
    else lift = 1;
  }
  
  if(cont.getButton("DigUp").pressed()){
    if(cont.getButton("DigDown").pressed()) dig = 1;
    else dig = 2;
  }
  else{
    if(cont.getButton("DigDown").pressed()) dig = 0;
    else dig = 1;
  }
  
  if(cont.getButton("DumpUp").pressed()){
    if(cont.getButton("DumpDown").pressed()) dump = 1;
    else dump = 2;
  }
  else{
    if(cont.getButton("DumpDown").pressed()) dump = 0;
    else dump = 1;
  }
  
  status = dump;
  status |= dig << 2;
  status |= lift << 4;
  status |= rightDrive << 6;
  status |= leftDrive << 11;
}

void draw() {
  getUserInput();
  //we can leave the draw method empty, 
  //because all our programming happens in the serialEvent (see below)
}

void serialEvent( Serial myPort) {
//put the incoming data into a String - 
//the '\n' is our end delimiter indicating the end of a complete packet
val = myPort.readStringUntil('\n');
//make sure our data isn't empty before continuing
if (val != null) {
  //trim whitespace and formatting characters (like carriage return)
  val = trim(val);
  println(val);

  //look for our 'A' string to start the handshake
  //if it's there, clear the buffer, and send a request for data
  if (firstContact == false) {
    if (val.equals("A")) {
      myPort.clear();
      firstContact = true;
      myPort.write("A");
      println("contact");
    }
  }
  else { //if we've already established contact, keep getting and parsing data
    println(val);

    //if (mousePressed == true) 
    //{                           //if we clicked in the window
      myPort.write("<" + str(status) + ">");        //send a 1
      //count += 20;
      //myPort.write("<7000>");        //send a 1
      //println("1000");
    //}

    // when you've parsed the data you have, ask for more:
    myPort.write("A");
    }
  }
  delay(100);
}
