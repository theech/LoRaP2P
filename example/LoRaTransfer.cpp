/*

  This example shows how to connect to an EBYTE transceiver
  using an Arduino Nano

  This code for for the sender


  connections
  Module      Arduino
  M0          4
  M1          5
  Rx          2 (This is the MCU Tx lined)
  Tx          3 (This is the MCU Rx line)
  Aux         6
  Vcc         3V3
  Gnd         Gnd

*/

#include <arduino.h>
#include "LoRa_E32.h"
#include <SoftwareSerial.h>

#define PIN_RX 2
#define PIN_TX 3
// #define PIN_M0 4
// #define PIN_M1 5
// #define PIN_AX 6

// ---------- esp8266 pins --------------
// LoRa_E32 e32ttl(D2, D3, D5, D7, D6);
// LoRa_E32 e32ttl(D2, D3, D5, D7, D6); // Config without connect AUX and M0 M1
// #include <SoftwareSerial.h>
// SoftwareSerial mySerial(PIN_TX, PIN_RX); // e32 TX e32 RX
// LoRa_E32 e32ttl(&mySerial, PIN_AX, PIN_M0, PIN_M1);

// #include <SoftwareSerial.h>
//  SoftwareSerial mySerial(D2, D3);// e32 TX e32 RX
//  LoRa_E32 e32ttl(&mySerial, D5, D7, D6);
//  -------------------------------------

// ---------- Arduino pins --------------
// LoRa_E32 e32ttl(2, 3, 5, 7, 6);
LoRa_E32 e32ttl(PIN_RX, PIN_TX); // Config without connect AUX and M0 M1

// #include <SoftwareSerial.h>
//  SoftwareSerial mySerial(2, 3); // e32 TX e32 RX
//  LoRa_E32 e32ttl(&mySerial, 5, 7, 6);
//  -------------------------------------

void printParameters(struct Configuration configuration);
void printModuleInformation(struct ModuleInformation moduleInformation);
// The setup function is called once at startup of the sketch
void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for native USB
    }
    delay(100);

    e32ttl.begin();

    // After set configuration comment set M0 and M1 to low
    // and reboot if you directly set HIGH M0 and M1 to program
    ResponseStructContainer c;
    c = e32ttl.getConfiguration();
    Configuration configuration = *(Configuration *)c.data;
    // configuration.ADDL = 0x01;
    // configuration.ADDH = 0x00;
    // configuration.CHAN = 0x04;
    // configuration.OPTION.fixedTransmission = FT_FIXED_TRANSMISSION;
    // e32ttl.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
    printParameters(configuration);
    // ---------------------------
}

// The loop function is called in an endless loop
void loop()
{
    delay(2000);

    Serial.println("Send message to 00 03 04");
    ResponseStatus rs = e32ttl.sendFixedMessage(0, 3, 0x04, "LoRa");
    Serial.println(rs.getResponseDescription());
}

void printParameters(struct Configuration configuration)
{
    Serial.println("----------------------------------------");

    Serial.print(F("HEAD : "));
    Serial.print(configuration.HEAD, BIN);
    Serial.print(" ");
    Serial.print(configuration.HEAD, DEC);
    Serial.print(" ");
    Serial.println(configuration.HEAD, HEX);
    Serial.println(F(" "));
    Serial.print(F("AddH : "));
    Serial.println(configuration.ADDH, BIN);
    Serial.print(F("AddL : "));
    Serial.println(configuration.ADDL, BIN);
    Serial.print(F("Chan : "));
    Serial.print(configuration.CHAN, DEC);
    Serial.print(" -> ");
    Serial.println(configuration.getChannelDescription());
    Serial.println(F(" "));
    Serial.print(F("SpeedParityBit     : "));
    Serial.print(configuration.SPED.uartParity, BIN);
    Serial.print(" -> ");
    Serial.println(configuration.SPED.getUARTParityDescription());
    Serial.print(F("SpeedUARTDatte  : "));
    Serial.print(configuration.SPED.uartBaudRate, BIN);
    Serial.print(" -> ");
    Serial.println(configuration.SPED.getUARTBaudRate());
    Serial.print(F("SpeedAirDataRate   : "));
    Serial.print(configuration.SPED.airDataRate, BIN);
    Serial.print(" -> ");
    Serial.println(configuration.SPED.getAirDataRate());

    Serial.print(F("OptionTrans        : "));
    Serial.print(configuration.OPTION.fixedTransmission, BIN);
    Serial.print(" -> ");
    Serial.println(configuration.OPTION.getFixedTransmissionDescription());
    Serial.print(F("OptionPullup       : "));
    Serial.print(configuration.OPTION.ioDriveMode, BIN);
    Serial.print(" -> ");
    Serial.println(configuration.OPTION.getIODroveModeDescription());
    Serial.print(F("OptionWakeup       : "));
    Serial.print(configuration.OPTION.wirelessWakeupTime, BIN);
    Serial.print(" -> ");
    Serial.println(configuration.OPTION.getWirelessWakeUPTimeDescription());
    Serial.print(F("OptionFEC          : "));
    Serial.print(configuration.OPTION.fec, BIN);
    Serial.print(" -> ");
    Serial.println(configuration.OPTION.getFECDescription());
    Serial.print(F("OptionPower        : "));
    Serial.print(configuration.OPTION.transmissionPower, BIN);
    Serial.print(" -> ");
    Serial.println(configuration.OPTION.getTransmissionPowerDescription());

    Serial.println("----------------------------------------");
}
void printModuleInformation(struct ModuleInformation moduleInformation)
{
    Serial.println("----------------------------------------");
    Serial.print(F("HEAD BIN: "));
    Serial.print(moduleInformation.HEAD, BIN);
    Serial.print(" ");
    Serial.print(moduleInformation.HEAD, DEC);
    Serial.print(" ");
    Serial.println(moduleInformation.HEAD, HEX);

    Serial.print(F("Freq.: "));
    Serial.println(moduleInformation.frequency, HEX);
    Serial.print(F("Version  : "));
    Serial.println(moduleInformation.version, HEX);
    Serial.print(F("Features : "));
    Serial.println(moduleInformation.features, HEX);
    Serial.println("----------------------------------------");
}