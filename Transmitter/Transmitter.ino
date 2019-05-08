char val; // Data received from the serial port
boolean ledState = LOW; //to toggle our LED
const int leftPin = 9; //Led to Arduino pin 3 (PWM)
const int rightPin = 10; //Led to Arduino pin 3 (PWM)
const int liftPin = 3;
const int digPin = 5;
const int dumpPin = 6;
const int ledPin = 13;
int count;
bool started= false;//True: Message is strated
bool ended  = false;//True: Message is finished 
char incomingByte ; //Variable to store the incoming byte
char msg[5];    //Message - array from 0 to 3 (4 values - PWM - e.g. 0240)
byte index;     //Index of array
void setup() 
{
  
  pinMode(leftPin, OUTPUT);
  pinMode(rightPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(digPin, OUTPUT);
  pinMode(dumpPin, OUTPUT);
  pinMode(liftPin, OUTPUT);
  //initialize serial communications at a 9600 baud rate
  Serial.begin(9600);
  establishContact();  // send a byte to establish contact until receiver responds 
}

void loop()
{
  
  while (Serial.available()>0){
    incomingByte = Serial.read();
    if(incomingByte == '<'){
     started = true;
     index = 0;
     msg[index] = '\0'; // Throw away any incomplete packet
   }
   else if(incomingByte == '>'){
     ended = true;
     break; // Done reading - exit from while loop!
   }
   else{
     if(index < 5){
       msg[index] = incomingByte; // Add char to array
       index++;
       msg[index] = '\0'; // Add NULL to end
     }
   }
 }
  
 if(started && ended)
 {
    int value = atoi(msg);
    int leftval = (value >> 8) & 0xF8;
    int rightval = (value >> 3) & 0xF8;
    int dumpval = (value << 6) & 0xC0;
    int digval = (value << 4) & 0xC0;
    int liftval = (value << 2) & 0xC0;
    analogWrite(leftPin, (int)leftval);
    analogWrite(rightPin, (int)rightval);
    analogWrite(dumpPin, dumpval);
    analogWrite(digPin, digval);
    analogWrite(liftPin, liftval);
    Serial.print('<'); //Only for debugging
    Serial.print(value); //Only for debugging
    Serial.println('>'); //Only for debugging
//    Serial.print('\t'); //Only for debugging
//    Serial.print(leftval); //Only for debugging
//    Serial.print('\t'); //Only for debugging
//    Serial.println(rightval); //Only for debugging
   
   index = 0;
   msg[index] = '\0';
   started = false;
   ended = false;
 }
//  delay(100);
}
 
//{
//  if (Serial.available() > 0) { // If data is available to read,
//    val = Serial.read(); // read it and store it in val
//    if(val == '<'){
//      started = true;
//      index = 0;
//      msg[index] = '\0';
//    }
//
//    if(val == '1') //if we get a 1
//    {
//       ledState = !ledState; //flip the ledState
//       digitalWrite(ledPin, ledState); 
//       count+=100;
//    }
//    delay(100);
//  } 
//    else {
////    Serial.print("Hello, world!"); //send back a hello world
//    Serial.print("<");
//    Serial.print(count);
//    Serial.println(">");
//    delay(50);
//    }
//}

void establishContact() {
  while (Serial.available() <= 0) {
  Serial.println("A");   // send a capital A
  delay(300);
  }
}
