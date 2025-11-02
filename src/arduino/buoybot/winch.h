#ifndef WINCH_h
#define WINCH_h

#include <L298N.h>
#include "config.h"

#define VOLTAGE_PER_AMPER 185

class Winch
{
  public:
  /* Constructor with config.h settings */
  Winch ()
  {
    
    Winch_Motor.stop();
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
    Serial.println(" WS      Winch stop");
    Serial.println(" WU      Winch up");
    Serial.println(" WD      Winch down");
    Serial.println(" WC      Winch current");
    Serial.println(" WR      Winch raw current");

    Serial.println("");
  };

  void debug_exec(String line)
  {
    if (line[0] == 'W') {
      if (line[1] == 'S') {
        stop();
      }

      if (line[1] == 'U') {
        up();
      }

      if (line[1] == 'D') {
        down();
      }

      if (line[1] == 'C') {
        Serial.print("Winch-Current: ");
        Serial.print(current());
        Serial.println("mA");
      }

      if (line[1] == 'R') {
        Serial.print("Winch-Current (raw): ");
        Serial.println(current_raw());
      }
    }
  };


  private:
  L298N Winch_Motor = L298N(PIN_WINCH_EN, PIN_WINCH_N1, PIN_WINCH_N2);
};

#endif