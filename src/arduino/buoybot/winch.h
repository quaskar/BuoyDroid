#ifndef WINCH_h
#define WINCH_h

#include <L298N.h>
#include "config.h"

#define VOLTAGE_PER_AMPER 185

class WinchCtrl
{
  public:
  /* Constructor with config.h settings */
  WinchCtrl ()
  {
    
    Winch_Motor.stop();
  };

  /* setup */
  void setup()
  {

  };

  /* loop */
  void loop()
  {

  };


  /* **************************************************************** */
  /* * Debug Crtls                                                  * */
  /* **************************************************************** */ 
  /* stop motion immediately */
  void stop()
  {
    Winch_Motor.stop();
  };

  /* move weight inot park position */
  void up()
  {
    Winch_Motor.backward();
  };

  /* move weigth to ground */
  void down()
  {
    Winch_Motor.forward();
  };

  /* meaure motor current in raw value */
  int current_raw() 
  {
    return analogRead(PIN_CURRENT_MEASURMENT); 
  };

  /* meaure motor current (mA) */
  float current() 
  {
    int rawValue = current_raw();
    float voltage = (rawValue / 1024.0) * 5000;
    return ((voltage - 2500) / VOLTAGE_PER_AMPER);
  };

  /* **************************************************************** */
  /* * Debug Crtls                                                  * */
  /* **************************************************************** */ 
  void debug_help()
  {
    Serial.println("Winch ctrl");
    Serial.println(" ws      Winch stop");
    Serial.println(" wu      Winch up");
    Serial.println(" wd      Winch down");
    Serial.println(" wc      Winch current");
    Serial.println(" wr      Winch raw current");

    Serial.println("");
  };

  void debug_exec(String line)
  {
    if (line[0] == 'w') {
      if (line[1] == 's') {
        stop();
      }

      if (line[1] == 'u') {
        up();
      }

      if (line[1] == 'd') {
        down();
      }

      if (line[1] == 'c') {
        Serial.print("Winch-Current: ");
        Serial.print(current());
        Serial.println("mA");
      }

      if (line[1] == 'r') {
        Serial.print("Winch-Current (raw): ");
        Serial.println(current_raw());
      }
    }
  };


  private:
  L298N Winch_Motor = L298N(PIN_WINCH_EN, PIN_WINCH_N1, PIN_WINCH_N2);
};

#endif