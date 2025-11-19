#ifndef LOGGING_H
#define LOGGING_H

#include <WiFiS3.h>
#include <WiFiUdp.h>
#include "config.h"

#define SIGNAL_MAX_NUMBER       32

#define LOGGING_TYPE_UINT8      'B'
#define LOGGING_TYPE_INT8       'b'
#define LOGGING_TYPE_UINT16     'H'
#define LOGGING_TYPE_INT16      'h'
#define LOGGING_TYPE_UINT32     'I'
#define LOGGING_TYPE_INT32      'i'
#define LOGGING_TYPE_UINT64     'Q'
#define LOGGING_TYPE_INT64      'q'
#define LOGGING_TYPE_FLOAT32    'f'
#define LOGGING_TYPE_FLOAT64    'd'


int signalSizeOf (char c)
{
    switch (c)
    {
    case LOGGING_TYPE_UINT8:    return sizeof(unsigned char);   break;
    case LOGGING_TYPE_INT8:     return sizeof(signed char);     break;
    case LOGGING_TYPE_UINT16:   return sizeof(unsigned short);  break;
    case LOGGING_TYPE_INT16:    return sizeof(signed short);    break;
    case LOGGING_TYPE_UINT32:   return sizeof(unsigned int);    break;
    case LOGGING_TYPE_INT32:    return sizeof(signed int);      break;
    case LOGGING_TYPE_UINT64:   return sizeof(unsigned long);   break;
    case LOGGING_TYPE_INT64:    return sizeof(signed long);     break;
    case LOGGING_TYPE_FLOAT32:  return sizeof(float);           break;
    case LOGGING_TYPE_FLOAT64:  return sizeof(double);          break;
    default:                    return -1;                      break;
    }

    return -1;
}


/* **************************************************************** */
/* * Global Variables                                             * */
/* **************************************************************** */ 
unsigned int    SignalNum = 0;
String          SignalNameList;
String          SignalStructStr;
unsigned int    SignalOffset[SIGNAL_MAX_NUMBER];
char            SignalBuffer[SIGNAL_MAX_NUMBER*8+4];

WiFiUDP         SignalUdp;



int registerSignal (String name, char type)
{
    /* check for maximum number of signals reached */
    if (SignalNum >= SIGNAL_MAX_NUMBER)
        return -1;
    
    SignalNameList = SignalNameList + "," + name;
    SignalStructStr += type;

    if (SignalNum == 0)
        SignalOffset[SignalNum]= 0;
    else
        SignalOffset[SignalNum]= SignalOffset[SignalNum-1] + signalSizeOf(SignalStructStr[SignalNum-1]);   

    SignalNum = SignalNum + 1;
    return SignalNum - 1;
};


void setSignal (int signalIdx, void *value)
{
    memcpy (&SignalBuffer[SignalOffset[signalIdx]], value, signalSizeOf(SignalStructStr[signalIdx]));
}


/* **************************************************************** */
/* * Setup Routine                                                * */
/* **************************************************************** */ 
void Logging_setup ()
{
    int status = WL_IDLE_STATUS;

    Serial.println("Starte WiFi Sender...");

    // WLAN verbinden
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print("Verbinde mit WLAN ");
    Serial.println(WIFI_SSID);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWLAN verbunden!");
    Serial.print("IP-Adresse: ");
    Serial.println(WiFi.localIP());

    // UDP starten (optional; für Sender nicht zwingend erforderlich)
    SignalUdp.begin(WIFI_PORT);
    
    registerSignal("MagicNum", LOGGING_TYPE_UINT16);
    registerSignal("MessageId", LOGGING_TYPE_UINT16);
    i = 1375
    setSignal (0, &i);
    i = 1
    setSignal (1, &i);
}

void Logging_announce ()
{
    /* send names of signals */
    SignalUdp.beginPacket(WIFI_DEST_IP, WIFI_PORT);

    SignalUdp.write(SignalNameList.c_str());
    SignalUdp.write("|<");
    SignalUdp.write(SignalStructStr.c_str());
    SignalUdp.endPacket();
}


/* **************************************************************** */
/* * Loop Routine                                                 * */
/* **************************************************************** */ 
void Logging_loop() {
  // UDP-Paket an Multicast-Gruppe senden
  SignalUdp.beginPacket(WIFI_DEST_IP, WIFI_PORT);
  SignalUdp.write(SignalBuffer, SignalOffset[SignalNum-1] + signalSizeOf(SignalStructStr[SignalNum-1]));
  SignalUdp.endPacket();
}


#endif