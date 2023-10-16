/*
 * EBYTE LoRa E22
 * Send a string message to a fixed point ADDH ADDL CHAN 0 2 23
 *
 * Write a string on serial monitor or reset to resend default value.
 *
 * Send a fixed point message, you must check that the transmitter and receiver have different
 * CHANNEL ADDL or ADDH, check down the correct configuration
 *
 * Renzo Mischianti <https://www.mischianti.org>
 * https://www.mischianti.org/category/my-libraries/ebyte-lora-e22-devices/
 *
 */

// With FIXED SENDER configuration
// #define DESTINATION_ADDL 3

// With FIXED RECEIVER configuration
#define DESTINATION_ADDL 3

#include "Arduino.h"
#include "LoRa_E22.h"
#include "stdlib.h"
#include "stdio.h"

// ---------- esp8266 pins --------------
// LoRa_E22 e22ttl(RX, TX, AUX, M0, M1);  // Arduino RX <-- e22 TX, Arduino TX --> e22 RX
// LoRa_E22 e22ttl(D3, D4, D5, D7, D6); // Arduino RX <-- e22 TX, Arduino TX --> e22 RX AUX M0 M1
// LoRa_E22 e22ttl(D2, D3); // Config without connect AUX and M0 M1

// #include <SoftwareSerial.h>
//  SoftwareSerial mySerial(D2, D3); // Arduino RX <-- e22 TX, Arduino TX --> e22 RX
//  LoRa_E22 e22ttl(&mySerial, D5, D7, D6); // AUX M0 M1
//  -------------------------------------

// ---------- Arduino pins --------------
LoRa_E22 e22ttl(4, 5, 3, 7, 6); // Arduino RX <-- e22 TX, Arduino TX --> e22 RX AUX M0 M1
// LoRa_E22 e22ttl(4, 5); // Config without connect AUX and M0 M1

// #include <SoftwareSerial.h>
//  SoftwareSerial mySerial(4, 5); // Arduino RX <-- e22 TX, Arduino TX --> e22 RX
//  LoRa_E22 e22ttl(&mySerial, 3, 7, 6); // AUX M0 M1
//  -------------------------------------

// ------------- Arduino Nano 33 IoT -------------
// LoRa_E22 e22ttl(&Serial1, 2, 4, 6); //  RX AUX M0 M1
// -------------------------------------------------

// ------------- Arduino MKR WiFi 1010 -------------
// LoRa_E22 e22ttl(&Serial1, 2, 4, 6); //  RX AUX M0 M1
// -------------------------------------------------

// ---------- esp32 pins --------------
// LoRa_E22 e22ttl(&Serial2, 18, 21, 19); //  RX AUX M0 M1

// LoRa_E22 e22ttl(&Serial2, 22, 4, 18, 21, 19, UART_BPS_RATE_9600); //  esp32 RX <-- e22 TX, esp32 TX --> e22 RX AUX M0 M1
//  -------------------------------------

void printParameters(struct Configuration configuration);

void setup()
{
    Serial.begin(9600);
    delay(500);

    // Startup all pins and UART
    e22ttl.begin();

    ResponseStructContainer c;
    c = e22ttl.getConfiguration();
    // It's important get configuration pointer before all other operation
    Configuration configuration = *(Configuration *)c.data;

    // configuration parameter
    // configuration.ADDL = 0x04;
    // configuration.ADDH = 0x00;

    // configuration.CHAN = 23;

    // configuration.SPED.uartBaudRate = UART_BPS_9600;
    // configuration.SPED.airDataRate = AIR_DATA_RATE_010_24;
    // configuration.SPED.uartParity = MODE_00_8N1;

    // configuration.OPTION.subPacketSetting = SPS_240_00;
    // configuration.OPTION.RSSIAmbientNoise = RSSI_AMBIENT_NOISE_DISABLED;
    // configuration.OPTION.transmissionPower = POWER_22;

    // configuration.TRANSMISSION_MODE.enableRSSI = RSSI_DISABLED;
    // configuration.TRANSMISSION_MODE.fixedTransmission = FT_FIXED_TRANSMISSION;
    // configuration.TRANSMISSION_MODE.enableRepeater = REPEATER_DISABLED;
    // configuration.TRANSMISSION_MODE.enableLBT = LBT_DISABLED;
    // configuration.TRANSMISSION_MODE.WORTransceiverControl = WOR_RECEIVER;
    // configuration.TRANSMISSION_MODE.WORPeriod = WOR_2000_011;
    // e22ttl.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);

    Serial.println(c.status.getResponseDescription());
    Serial.println(c.status.code);

    printParameters(configuration);
    c.close();
}

void loop()
{
    float temp = rand() % 49;
    float humd = rand() % 99;

    Serial.println(temp);
    Serial.println(humd);

    String messaggions = "{temp: " + String(temp) + ", humid: " + String(humd) + "}";
    Serial.println(messaggions);

    // Send message
    ResponseStatus rs = e22ttl.sendFixedMessage(0, DESTINATION_ADDL, 23, messaggions);
    // Check If there is some problem of succesfully send
    Serial.println(rs.getResponseDescription());
    delay(4000);

    // If something available
    if (e22ttl.available() > 1)
    {
        // read the String message
        ResponseContainer rc = e22ttl.receiveMessage();

        // Is something goes wrong print error
        if (rc.status.code != 1)
        {
            Serial.println(rc.status.getResponseDescription());
        }
        else
        {
            // Print the data received
            Serial.println(rc.status.getResponseDescription());
            Serial.println(rc.data);
        }
    }
    if (Serial.available())
    {
        String input = Serial.readString();
        e22ttl.sendFixedMessage(0, DESTINATION_ADDL, 23, input);
    }
}

void printParameters(struct Configuration configuration)
{
    Serial.println("----------------------------------------");

    Serial.print(F("HEAD : "));
    Serial.print(configuration.COMMAND, HEX);
    Serial.print(" ");
    Serial.print(configuration.STARTING_ADDRESS, HEX);
    Serial.print(" ");
    Serial.println(configuration.LENGHT, HEX);
    Serial.println(F(" "));
    Serial.print(F("AddH : "));
    Serial.println(configuration.ADDH, HEX);
    Serial.print(F("AddL : "));
    Serial.println(configuration.ADDL, HEX);
    Serial.print(F("NetID : "));
    Serial.println(configuration.NETID, HEX);
    Serial.println(F(" "));
    Serial.print(F("Chan : "));
    Serial.print(configuration.CHAN, DEC);
    Serial.print(" -> ");
    Serial.println(configuration.getChannelDescription());
    Serial.println(F(" "));
    Serial.print(F("SpeedParityBit     : "));
    Serial.print(configuration.SPED.uartParity, BIN);
    Serial.print(" -> ");
    Serial.println(configuration.SPED.getUARTParityDescription());
    Serial.print(F("SpeedUARTDatte     : "));
    Serial.print(configuration.SPED.uartBaudRate, BIN);
    Serial.print(" -> ");
    Serial.println(configuration.SPED.getUARTBaudRateDescription());
    Serial.print(F("SpeedAirDataRate   : "));
    Serial.print(configuration.SPED.airDataRate, BIN);
    Serial.print(" -> ");
    Serial.println(configuration.SPED.getAirDataRateDescription());
    Serial.println(F(" "));
    Serial.print(F("OptionSubPacketSett: "));
    Serial.print(configuration.OPTION.subPacketSetting, BIN);
    Serial.print(" -> ");
    Serial.println(configuration.OPTION.getSubPacketSetting());
    Serial.print(F("OptionTranPower    : "));
    Serial.print(configuration.OPTION.transmissionPower, BIN);
    Serial.print(" -> ");
    Serial.println(configuration.OPTION.getTransmissionPowerDescription());
    Serial.print(F("OptionRSSIAmbientNo: "));
    Serial.print(configuration.OPTION.RSSIAmbientNoise, BIN);
    Serial.print(" -> ");
    Serial.println(configuration.OPTION.getRSSIAmbientNoiseEnable());
    Serial.println(F(" "));
    Serial.print(F("TransModeWORPeriod : "));
    Serial.print(configuration.TRANSMISSION_MODE.WORPeriod, BIN);
    Serial.print(" -> ");
    Serial.println(configuration.TRANSMISSION_MODE.getWORPeriodByParamsDescription());
    Serial.print(F("TransModeTransContr: "));
    Serial.print(configuration.TRANSMISSION_MODE.WORTransceiverControl, BIN);
    Serial.print(" -> ");
    Serial.println(configuration.TRANSMISSION_MODE.getWORTransceiverControlDescription());
    Serial.print(F("TransModeEnableLBT : "));
    Serial.print(configuration.TRANSMISSION_MODE.enableLBT, BIN);
    Serial.print(" -> ");
    Serial.println(configuration.TRANSMISSION_MODE.getLBTEnableByteDescription());
    Serial.print(F("TransModeEnableRSSI: "));
    Serial.print(configuration.TRANSMISSION_MODE.enableRSSI, BIN);
    Serial.print(" -> ");
    Serial.println(configuration.TRANSMISSION_MODE.getRSSIEnableByteDescription());
    Serial.print(F("TransModeEnabRepeat: "));
    Serial.print(configuration.TRANSMISSION_MODE.enableRepeater, BIN);
    Serial.print(" -> ");
    Serial.println(configuration.TRANSMISSION_MODE.getRepeaterModeEnableByteDescription());
    Serial.print(F("TransModeFixedTrans: "));
    Serial.print(configuration.TRANSMISSION_MODE.fixedTransmission, BIN);
    Serial.print(" -> ");
    Serial.println(configuration.TRANSMISSION_MODE.getFixedTransmissionDescription());

    Serial.println("----------------------------------------");
}
