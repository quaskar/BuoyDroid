#include <WiFiS3.h>
#include <WiFiUdp.h>
#include "config.h"


//#include "arduino_secrets.h" 


/* **************************************************************** */
/* * Global Variables                                             * */
/* **************************************************************** */ 
WiFiUDP udp;
int i = 0;


/* **************************************************************** */
/* * Setup Routine                                                * */
/* **************************************************************** */ 
void Wifi_setup(char ssid[], char pass[]) {
  
  int status = WL_IDLE_STATUS;

  Serial.println("Starte WiFi Sender...");

  // WLAN verbinden
  WiFi.begin(ssid, pass);
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
  const char* message = "Hallo von Arduino UNO R4 WiFi Multicast!";

  // UDP-Paket an Multicast-Gruppe senden
  udp.beginPacket(WIFI_DEST_IP, WIFI_PORT);
  udp.write(String(i).c_str());
  udp.endPacket();

  i = i + 1;
}
