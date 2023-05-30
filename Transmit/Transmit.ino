/*
   RadioLib SX127x Transmit Example

   This example transmits packets using SX1278 LoRa radio module.
   Each packet contains up to 256 bytes of data, in the form of:
    - Arduino String
    - null-terminated char array (C-string)
    - arbitrary binary data (byte array)

   Other modules from SX127x/RFM9x family can also be used.

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx127xrfm9x---lora-modem

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

// include the library
#include <RadioLib.h>

// SX1278 has the following connections:
// NSS pin:   10   -> D7
// DIO0 pin:  2    -> D0
// RESET pin: 9    -> D1
// DIO1 pin:  3    -> D2
SX1276 radio = new Module(D7, D0, D1, D2);
bool serial = false;
byte data[222];

void setup() {
  if (serial) Serial.begin(9600);
  delay(3000);

  // initialize SX1278 with default settings
  if (serial) Serial.print(F("[SX1278] Initializing ... "));

/*begin   (   float   freq = 434.0,
    float   bw = 125.0, 137.0 MHz to 1020.0 MHz. 
    uint8_t   sf = 9,
    uint8_t   cr = 7,
    uint8_t   syncWord = RADIOLIB_SX127X_SYNC_WORD,
    int8_t    power = 10,
    uint16_t    preambleLength = 8,
    uint8_t   gain = 0 
  )*/
  
  int state = radio.begin(434.0, 125.0, 12, 7, RADIOLIB_SX127X_SYNC_WORD, 10, 8, 0);
  if (state == RADIOLIB_ERR_NONE) {
    if (serial) Serial.println(F("success!"));
  } else {
    if (serial) Serial.print(F("failed, code "));
    if (serial) Serial.println(state);
    while (true) {
      delay(500);
      if (serial) Serial.println("Failed");
    }
  }

  for (int i = 0; i < 222; i++) data[i] = i;

  // radio.setSpreadingFactor(9); // Fom 6 to 12
  // radio.setOutputPower(17);      // From 2 to 20
}

void loop() {
  if (serial) Serial.print(F("[SX1278] Transmitting packet ... "));

  // you can transmit C-string or Arduino string up to
  // 256 characters long
  // NOTE: transmit() is a blocking method!
  //       See example SX127x_Transmit_Interrupt for details
  //       on non-blocking transmission method.

  
  int state = radio.transmit(data, 222);

  // you can also transmit byte array up to 256 bytes long
  /*
    byte byteArr[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    int state = radio.transmit(byteArr, 8);
  */

  if (state == RADIOLIB_ERR_NONE) {
    // the packet was successfully transmitted
    if (serial) Serial.println(F(" success!"));

    // print measured data rate
    if (serial) Serial.print(F("[SX1278] Datarate:\t"));
    if (serial) Serial.print(radio.getDataRate());
    if (serial) Serial.println(F(" bps"));
  } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
    // the supplied packet was longer than 256 bytes
    if (serial) Serial.println(F("too long!"));
  } else if (state == RADIOLIB_ERR_TX_TIMEOUT) {
    // timeout occurred while transmitting packet
    if (serial) Serial.println(F("timeout!"));
  } else {
    // some other error occurred
    if (serial) Serial.print(F("failed, code "));
    if (serial) Serial.println(state);
  }

  // wait for a second before transmitting again
  delay(2000);
}
