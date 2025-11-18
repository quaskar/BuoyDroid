#ifndef LOGGING_H
#define LOGGING_H

#include <WiFiS3.h>
#include <WiFiUdp.h>
#include "config.h"

#define     NUMBER_SIGNAL_MAX       32



enum eSignalType {
    UINT8   = 0,
    SINT8   = 1,
    UINT16  = 2,
    SINT16  = 3,
    UINT32  = 4,
    SINT32  = 5,
    UINT64  = 6,
    SINT64  = 7,
    FLOAT32 = 8,
    FLOAT64 = 9,
};

int signalSizeOf (eSignalType type)
{
    switch (type)
    {
    case UINT8:     return sizeof(unsigned char);   break;
    case SINT8:     return sizeof(signed char);     break;
    case UINT16:    return sizeof(unsigned short);  break;
    case SINT16:    return sizeof(signed short);    break;
    case UINT32:    return sizeof(unsigned int);    break;
    case SINT32:    return sizeof(signed int);      break;
    case UINT64:    return sizeof(unsigned long);   break;
    case SINT64:    return sizeof(signed long);     break;
    case FLOAT32:   return sizeof(float);           break;
    case FLOAT64:   return sizeof(double);          break;
    default:        return -1;                      break;
    }

    return -1;
}


/* **************************************************************** */
/* * Global Variables                                             * */
/* **************************************************************** */ 
unsigned int    SignalMax = 0
String          SignalNames[NUMBER_SIGNAL_MAX];
eSignalType     SignalTypes[NUMBER_SIGNAL_MAX];
unsigned int    SignalTypesOffset[NUMBER_SIGNAL_MAX];

char            SignalBuffer[NUMBER_SIGNAL_MAX*8];

WiFiUDP udp;



int registerSignal (String name, eSignalType type)
{
    /* check for maximum number of signals reached */
    if (SignalMax >= NUMBER_SIGNAL_MAX)
        return -1;
    
    SignalNames[SignalMax]          = name;
    SignalTypes[SignalMax]          = type;
    if (SignalMax == 0)
        SignalTypesOffset[SignalMax]= 0;
    else
        SignalTypesOffset[SignalMax]= SignalTypesOffset[SignalMax-1] + signalSizeOf(type);   

    SignalMax = SignalMax + 1;
    return SignalMax;
};


void setSignal (int signalIdx, char *value)
{
    memcpy (&SignalBuffer[SignalTypesOffset[signalIdx]], value, signalSizeOf(SignalTypes[signalIdx]);
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
    udp.begin(WIFI_PORT);
}


/* **************************************************************** */
/* * Loop Routine                                                 * */
/* **************************************************************** */ 
void Wifi_loop() {
  // UDP-Paket an Multicast-Gruppe senden
  udp.beginPacket(WIFI_DEST_IP, WIFI_PORT);
  udp.write(SignalBuffer, SignalTypesOffset[SignalMax-1])
  udp.endPacket();
}


#endif