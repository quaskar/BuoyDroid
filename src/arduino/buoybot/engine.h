#ifndef ENGINE_H
#define ENGINE_H

#include <Servo.h>
#include <QMC5883LCompass.h>
#include "config.h"

#define ESC_MIN_MICROSEC    1000
#define ESC_MAX_MICROSEC    2001
#define ESC_MID_MICROSEC    (ESC_MAX_MICROSEC-ESC_MIN_MICROSEC) / 2 + ESC_MIN_MICROSEC

class EngineCtrl
{
  public:
  /* Constructor with config.h settings */
  EngineCtrl ()
  {
    
  };

  void setup ()
  {
    Esc_Bb.attach  (PIN_STB_ESC, ESC_MIN_MICROSEC, ESC_MAX_MICROSEC);
    Esc_Stb.attach (PIN_BB_ESC,  ESC_MIN_MICROSEC, ESC_MAX_MICROSEC);

    calibrate();

    Compass.init();
    Compass.setCalibrationOffsets(COMPASS_OFFSET_X, COMPASS_OFFSET_Y, COMPASS_OFFSET_Z);
    Compass.setCalibrationScales(COMPASS_SCALE_X, COMPASS_SCALE_Y, COMPASS_SCALE_Z);

    target_yaw   = 0.0;
    target_speed = 0.0;
  }

  /* loop */
  void loop()
  {
  };


  /* **************************************************************** */
  /* * Debug Crtls                                                  * */
  /* **************************************************************** */ 
  void debug_help()
  {
    Serial.println("Engine ctrl");
    Serial.println(" es         Engine stop");
    Serial.println(" ess<speed> Engine set speed [-100;+100] 0=stopped");
    Serial.println(" esy<yaw>   Engine set yaw rate");
    Serial.println("");

    Serial.println("Compass ctrl");
    Serial.println(" cp         Print compass value");
    Serial.println(" cc         Calibrate compass");
  };

  void debug_exec(String line)
  {
    if (line[0] == 'e') {
      if (line[1] == 's') {
        if (line[2] == 's') {
          int value = line.substring(3).toDouble();
          value = max(value, -100);
          value = min(value, 100);
          value = map(value, -100, 100, ESC_MIN_MICROSEC, ESC_MAX_MICROSEC);

          Esc_Bb.writeMicroseconds  (value);
          Esc_Stb.writeMicroseconds  (value);

          Serial.print("Set speed: ");
          Serial.print(Esc_Stb.readMicroseconds());
          Serial.print(";");
          Serial.print(Esc_Bb.readMicroseconds());
          loop();
        }
        else if (line[2] == 'y') {
          target_yaw = line.substring(3).toDouble();
          Serial.print("Set target yaw rate: ");
          Serial.println(target_yaw);
        }
        else {
          Esc_Bb.writeMicroseconds  (ESC_MID_MICROSEC);
          Esc_Stb.writeMicroseconds  (ESC_MID_MICROSEC);
          Serial.println("Engine stopped");
          loop();  
        }
      }
    }
    else if (line[0] == 'c')
    {
      if (line[1] == 'p')
      {
        Serial.print("Compass: ");
        Compass.read();
        Serial.print((double)Compass.getAzimuth());
        Serial.println("°");
      }
      else if (line[1] == 'c')
      {
        Serial.println("CALIBRATING. Keep moving your sensor...");
        Compass.calibrate();

        Serial.println("DONE. Copy the lines below and paste it into your projects config file.);");
        Serial.println();
        
        Serial.print("#define COMPASS_OFFSET_X        ");
        Serial.println(Compass.getCalibrationOffset(0));
        Serial.print("#define COMPASS_OFFSET_Y        ");
        Serial.println(Compass.getCalibrationOffset(1));
        Serial.print("#define COMPASS_OFFSET_Z        ");
        Serial.println(Compass.getCalibrationOffset(2));
        
        Serial.print("#define COMPASS_SCALE_X         ");
        Serial.println(Compass.getCalibrationScale(0));
        Serial.print("#define COMPASS_SCALE_Y         ");
        Serial.println(Compass.getCalibrationScale(1));
        Serial.print("#define COMPASS_SCALE_Z         ");
        Serial.println(Compass.getCalibrationScale(2));
      }
    }
  };

/* **************************************************************** */
/* * Global Variables                                             * */
/* **************************************************************** */ 
  private:
  Servo           Esc_Bb; 
  Servo           Esc_Stb;
  QMC5883LCompass Compass; 

  double target_yaw   = 0.0;
  double target_speed = 0.0;


  void calibrate()
  {
    // calibration to low point 
    Esc_Bb.writeMicroseconds  (ESC_MIN_MICROSEC);
    Esc_Stb.writeMicroseconds (ESC_MIN_MICROSEC);

    delay(200);

    // calibration to mid point
    Esc_Bb.writeMicroseconds  (ESC_MID_MICROSEC);
    Esc_Stb.writeMicroseconds (ESC_MID_MICROSEC);  

    delay(200);
  };
};

#endif