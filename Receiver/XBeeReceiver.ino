/*   ~ Simple Arduino - xBee Receiver sketch ~

  Read an PWM value from Arduino Transmitter to fade an LED
  The receiving message starts with '<' and closes with '>' symbol.

  Dev: Michalis Vasilakis // Date:2/3/2016 // Info: www.ardumotive.com // Licence: CC BY-NC-SA                    */

//Constants
const int leftPin = 5; //Led to Arduino pin 3 (PWM)
const int rightPin = 6; //Led to Arduino pin 3 (PWM)
const int liftPin = 3;
const int digPin = 9;
const int dumpPin = 10;
const int ledPin = 13;

//Variables
bool started = false; //True: Message is strated
bool ended  = false;//True: Message is finished
char incomingByte ; //Variable to store the incoming byte
char msg[6];    //Message - array from 0 to 3 (4 values - PWM - e.g. 0240)
byte index;     //Index of array

void setup() {
  //Start the serial communication
  Serial.begin(9600); //Baud rate must be the same as is on xBee module
  pinMode(leftPin, OUTPUT);
  pinMode(rightPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // otherwise turn it off
//  setPwmFrequency(leftPin, 4);
//  setPwmFrequency(rightPin, 4);
//  setPwmFrequency(liftPin, 2);
//  setPwmFrequency(digPin, 4);
//  setPwmFrequency(dumpPin, 4);

  TCCR2B = TCCR2B & B11111000 | B00000101; // for PWM frequency of 245.10 Hz
  TCCR0B = TCCR0B & B11111000 | B00000100; // for PWM frequency of 244.14 Hz
  TCCR1B = TCCR1B & B11111000 | B00000100; // for PWM frequency of 122.55 Hz
  
}

void loop() {

  while (Serial.available() > 0) {
    digitalWrite(ledPin, HIGH); // turn the LED on
    //    Serial.print("available");
    //Read the incoming byte
    incomingByte = Serial.read();
    //Start the message when the '<' symbol is received
    if (incomingByte == '<')
    {
      started = true;
      index = 0;
      msg[index] = '\0'; // Throw away any incomplete packet
    }
    //End the message when the '>' symbol is received
    else if (incomingByte == '>')
    {
      ended = true;
      break; // Done reading - exit from while loop!
    }
    //Read the message!
    else
    {
      if (index < 6) // Make sure there is room
      {
        msg[index] = incomingByte; // Add char to array
        index++;
        msg[index] = '\0'; // Add NULL to end
      }
    }
  }

  if (started && ended)
  {
    digitalWrite(ledPin, LOW); // otherwise turn it off
    int value = atoi(msg);
      int leftval = ((value >> 10) & B111110) + 63;
      int rightval = ((value >> 5) & B111110) + 63;
      int dumpval = ((value & B11) == B10) ? 63 : (((value & B11) == 0) ? 32 : 47) ;
      int digval = ((value & B1100) == B1000) ? 63 : (((value & B1100) == 0) ? 32 : 47);
      int liftval = ((value & B110000) == B100000) ? 126 : (((value & B110000) == 0) ? 64 : 94);
      analogWrite(leftPin, leftval);
      analogWrite(rightPin, rightval);
      analogWrite(dumpPin, dumpval);
      analogWrite(digPin, digval);
      analogWrite(liftPin, liftval);
//          Serial.print(value); //Only for debugging
//          Serial.print('\t'); //Only for debugging
//          Serial.print(leftval); //Only for debugging
//          Serial.print('\t'); //Only for debugging
//          Serial.println(rightval); //Only for debugging
    //   Serial.print('\t'); //Only for debugging
    //   Serial.println(value); //Only for debugging
    index = 0;
    msg[index] = '\0';
    started = false;
    ended = false;
  }
}
