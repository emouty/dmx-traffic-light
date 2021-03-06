#include <Conceptinetics.h>

//
// CTC-DRA-13-1 ISOLATED DMX-RDM SHIELD JUMPER INSTRUCTIONS
//
// If you are using the above mentioned shield you should
// place the RXEN jumper towards G (Ground), This will turn
// the shield into read mode without using up an IO pin
//
// The !EN Jumper should be either placed in the G (GROUND)
// position to enable the shield circuitry
//   OR
// if one of the pins is selected the selected pin should be
// set to OUTPUT mode and set to LOGIC LOW in order for the
// shield to work
//

//
// The slave device will use a block of 10 channels counting from
// its start address.
//
// If the start address is for example 56, then the channels kept
// by the dmx_slave object is channel 56-66
//
#define DMX_SLAVE_CHANNELS   2

//
// Pin number to change read or write mode on the shield
// Uncomment the following line if you choose to control
// read and write via a pin
//
// On the CTC-DRA-13-1 shield this will always be pin 2,
// if you are using other shields you should look it up
// yourself
//
///// #define RXEN_PIN                2


// Configure a DMX slave controller
DMX_Slave dmx_slave ( DMX_SLAVE_CHANNELS );

// If you are using an IO pin to control the shields RXEN
// the use the following line instead
///// DMX_Slave dmx_slave ( DMX_SLAVE_CHANNELS , RXEN_PIN );

const int PIN_R = 10;
const int PIN_L = 11;

// set pins for 8DIP switch pins
const int PIN_1 = A1;
const int PIN_2 = A2;
const int PIN_3 = A3;
const int PIN_4 = A4;
const int PIN_5 = A5;
const int PIN_6 = 6;
const int PIN_7 = 8;
const int PIN_8 = 9;
// set pins for potentiometer
const int PIN_POT = A0;



// set channels consts
//const int CHANNEL_MODE = 1;
const int CHANNEL_L = 1;
const int CHANNEL_R = 2;
int adress = 0;
// the setup routine runs once when you press reset:
void setup() {
  // Set leds pin as output pin
  pinMode ( PIN_R, OUTPUT );
  pinMode ( PIN_L, OUTPUT );
  // set dip pin as input
  pinMode ( PIN_1, INPUT_PULLUP );
  pinMode ( PIN_2, INPUT_PULLUP );
  pinMode ( PIN_3, INPUT_PULLUP );
  pinMode ( PIN_4, INPUT_PULLUP );
  pinMode ( PIN_5, INPUT_PULLUP );
  pinMode ( PIN_6, INPUT_PULLUP );
  pinMode ( PIN_7, INPUT_PULLUP );
  pinMode ( PIN_8, INPUT_PULLUP );
  pinMode ( PIN_POT, INPUT);

  // Enable DMX slave interface and start recording
  // DMX data
  dmx_slave.enable ();

  // Set start address to 1, this is also the default setting
  // You can change this address at any time during the program
  adress = getDMXAdress();
  if (adress == 0) {
    dmx_slave.setStartAddress (1);
  } else {
    dmx_slave.setStartAddress (adress);
  }


}

// the loop routine runs over and over again forever:
void loop()
{

  //adress = getDMXAdress();
  if (adress == 0) {
    // always on if no DMX adress
    // digitalWrite ( PIN_R , LOW );
    // digitalWrite ( PIN_L , LOW );
    // strobo with potentiometer in A0
    strobo();
  } else {
    //dmx_slave.setStartAddress (adress);
    displayFromDMX();

  }
}

/*int getMode() {
  return dmx_slave.getChannelValue (CHANNEL_MODE);
  }*/

int getValueR() {
  return dmx_slave.getChannelValue (CHANNEL_R);
}

int getValueL() {
  return dmx_slave.getChannelValue (CHANNEL_L);
}

int getDMXAdress() {

  return (int) ( (1 - digitalRead(PIN_1)) +
                 (1 - digitalRead(PIN_2)) * 2 +
                 (1 - digitalRead(PIN_3)) * 4 +
                 (1 - digitalRead(PIN_4)) * 8 +
                 (1 - digitalRead(PIN_5)) * 16 +
                 (1 - digitalRead(PIN_6)) * 32 +
                 (1 - digitalRead(PIN_7)) * 64 +
                 (1 - digitalRead(PIN_8)) * 128
               );
}

void displayFromDMX() {
  // int mode = getMode();
  int valueL = getValueL();
  int valueR = getValueR();
  writeValue ( PIN_L , 255 - valueL );
  writeValue ( PIN_R , 255 - valueR );
  /*if ( mode < 63 ) {
    // mode R & L
    analogWrite ( PIN_R , valueL );
    analogWrite ( PIN_L , valueR );

    } else if ( mode < 128 ) {
    // mode R || L
    analogWrite ( PIN_R , freq );
    analogWrite ( PIN_L , 255 - freq );
    } else if ( mode < 192 ) {
    // mode R
    analogWrite ( PIN_R , freq );
    } else {
    // mode L
    analogWrite ( PIN_L , freq );
    }*/
}

void writeValue(int pin, int value) {
  //  if (value > 250 ) {
  //    digitalWrite(pin, HIGH);
  //  } else {
  analogWrite(pin, value);
  //  }
}



void strobo() {
  int maxValue = 1000;
  int value = maxValue - map(analogRead(PIN_POT), 0, 1023, 0, maxValue);

  if (value == 0 ) {
    // always on
    digitalWrite ( PIN_R , LOW );
    digitalWrite ( PIN_L , LOW );
  }
  else if (value == maxValue ) {
    // always off
    digitalWrite ( PIN_R , HIGH );
    digitalWrite ( PIN_L , HIGH );
  }
  else {
    digitalWrite(PIN_L, LOW);
    digitalWrite(PIN_R, LOW);
    delay(value);
    digitalWrite(PIN_L, HIGH);
    digitalWrite(PIN_R, HIGH);
    delay(value);
  }
}
