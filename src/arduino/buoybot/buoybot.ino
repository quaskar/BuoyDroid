/*
  WiFi UDP Send and Receive String

  This sketch waits for a UDP packet on localPort using the WiFi module.
  When a packet is received an Acknowledge packet is sent to the client on port remotePort

  created 30 December 2012
  by dlf (Metodo2 srl)

  Find the full UNO R4 WiFi Network documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/wifi-examples#wi-fi-udp-send-receive-string
 */

#include "serial.h"
//#include "wifi.h"
#include <PID_v1.h>
#include "winch.h"
#include "engine.h"

#include "config.h"

#define VERSION_MAJOR 0
#define VERSION_MINOR 0
#define VERSION_PATCH 1



/* **************************************************************** */
/* * Global Variables                                             * */
/* **************************************************************** */ 
char broadcastBuffer[256];

double Ctrl_Speed = 0.0;
double Ctrl_YawRate = 0.0;

WinchCtrl Winch;
EngineCtrl Engine;



// PID controller
//PID PID_Rotation(&Compass_Value, &Ctrl_YawRate, &Compass_Target, PID_ROTATION_KP, PID_ROTATION_KI, PID_ROTATION_KD, DIRECT);
//PID PID_Speed(&Speed_Value, &Speed_Value, &Speed_Target,PID_SPEED_KP, PID_SPEED_KI, PID_SPEED_KD, DIRECT);


/* **************************************************************** */
/* * Local Routines                                               * */
/* **************************************************************** */ 
void fillBroadcastBuffer()
{
  //sprintf(broadcastBuffer, "c:%d;y:%f\ntc%d", (int)Compass_Value, Ctrl_YawRate, (int)Compass_Target);
}

/* **************************************************************** */ 
void print_version() {
  Serial.print("BuoyBot v");
  Serial.print(String(VERSION_MAJOR));
  Serial.print(".");
  Serial.print(String(VERSION_MINOR));
  Serial.print(".");
  Serial.println(String(VERSION_PATCH));

}

/* **************************************************************** */ 
void print_help() {
  print_version();
  Serial.println("Command reference:");
  Serial.println("");

  /* sub module help text */
  Winch.debug_help();
  Engine.debug_help();
}

/* **************************************************************** */ 
void parse_commandLine(String line) {

  if (line[0] == '?') {
    print_help();
  }

  // sub module debug ctrls
  Winch.debug_exec(line);
  Engine.debug_exec(line);
}


/* **************************************************************** */
/* * Setup Routine                                                * */
/* **************************************************************** */ 
void setup() {

  Serial_setup(SERIAL_BAUDRATE);
  //Wifi_setup(SECRET_SSID, SECRET_PASS);
  Engine.setup();

  //PID_Rotation.SetMode(AUTOMATIC);
  //PID_Rotation.SetOutputLimits(-PID_ROTATION_LIMIT, PID_ROTATION_LIMIT);

  //PID_Speed.SetMode(AUTOMATIC);
  //PID_Speed.SetOutputLimits(-PID_SPEED_LIMIT, PID_SPEED_LIMIT);
}


/* **************************************************************** */
/* * Loop Routine                                                 * */
/* **************************************************************** */ 
void loop() {
  //PID_Rotation.Compute();
  //PID_Speed.Compute();
  Winch.loop();
  Engine.loop();

  //Wifi_loop();
  Serial_loop();

  //fillBroadcastBuffer();
  //Wifi_sendLine(broadcastBuffer);
  //Serial_sendLine(broadcastBuffer);  

  if (Serial_lineReceived) {
    //Serial_sendLine((char*)Serial_lineData.c_str());
    parse_commandLine(Serial_lineData);
    Serial_releaseLine();
  }

  delay (LOOP_DELAY);
}
