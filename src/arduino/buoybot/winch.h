#ifndef WINCH_h
#define WINCH_h

#include <L298N.h>
#include "config.h"

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


  /* **************************************************************** */
  /* * Debug Crtls                                                  * */
  /* **************************************************************** */ 
  void debug_help()
  {
    Serial.println("Winch ctrl");
    Serial.println(" WS      Winch stop");
    Serial.println(" WU      Winch up");
    Serial.println(" WD      Winch down");
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
    }
  };


  private:
  L298N Winch_Motor = L298N(PIN_WINCH_EN, PIN_WINCH_N1, PIN_WINCH_N2);
};

#endif