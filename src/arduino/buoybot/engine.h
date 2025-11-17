#ifndef ENGINE_H
#define ENGINE_H

#include <Servo.h>
#include <QMC5883LCompass.h>
#include <PID_v1.h>
#include "config.h"

#define ESC_MIN_MICROSEC    1000
#define ESC_MAX_MICROSEC    2001
#define ESC_MID_MICROSEC    (ESC_MAX_MICROSEC-ESC_MIN_MICROSEC) / 2 + ESC_MIN_MICROSEC


double compass_value  = 0.0;
double compass_target = 0.0;
double yawrate_set    = 0.0;

double speed_value    = 0.0;
double speed_target   = 0.0;
double speed_set      = 0.0;

PID    PID_Rotation(&compass_value, &yawrate_set, &compass_target, 0.0, 0.0, 0.0, 0);
PID    PID_Speed(&speed_set, &speed_set, &speed_target, 0.0, 0.0, 0.0, 0);


class EngineCtrl
{
  public:
  /* Constructor with config.h settings */
  EngineCtrl ()
  {
    
  };

  void setup ()
  {
    /* ESC initializations */
    Esc_Bb.attach  (PIN_STB_ESC, ESC_MIN_MICROSEC, ESC_MAX_MICROSEC);
    Esc_Stb.attach (PIN_BB_ESC,  ESC_MIN_MICROSEC, ESC_MAX_MICROSEC);
    calibrate();

    /* Compass initialization */
    Compass.init();
    Compass.setCalibrationOffsets(COMPASS_OFFSET_X, COMPASS_OFFSET_Y, COMPASS_OFFSET_Z);
    Compass.setCalibrationScales(COMPASS_SCALE_X, COMPASS_SCALE_Y, COMPASS_SCALE_Z);

    /* Rotation PID controller */
    PID_Rotation.SetMode(AUTOMATIC);
    PID_Rotation.SetOutputLimits(-PID_ROTATION_LIMIT, PID_ROTATION_LIMIT);
    PID_Rotation.SetTunings(PID_ROTATION_KP, PID_ROTATION_KI, PID_ROTATION_KD);
    PID_Rotation.SetControllerDirection(PID_ROTATION_DIRECTION);

    /* Speed PID controller */
    PID_Speed.SetMode(AUTOMATIC);
    PID_Speed.SetOutputLimits(-PID_SPEED_LIMIT, PID_SPEED_LIMIT);
    PID_Speed.SetTunings(PID_SPEED_KP, PID_SPEED_KI, PID_SPEED_KD);
    PID_Speed.SetControllerDirection(PID_SPEED_DIRECTION);
    
    Compass.read();
    compass_target  = (double)Compass.getAzimuth();
    speed_target    = 0.0;
  }

  /* loop */
  void loop()
  {
    Compass.read();
    compass_value = (double)Compass.getAzimuth();

    PID_Rotation.Compute();
    PID_Speed.Compute();
    speed_value = speed_set;

    int v_Bb = map(speed_set + yawrate_set, -ESC_BB_DIRECTION*100, ESC_BB_DIRECTION*100, ESC_MIN_MICROSEC, ESC_MAX_MICROSEC);
    int v_Stb = map(speed_set - yawrate_set, -ESC_STB_DIRECTION*100, ESC_STB_DIRECTION*100, ESC_MIN_MICROSEC, ESC_MAX_MICROSEC);
    Esc_Bb.writeMicroseconds  (v_Bb);
    Esc_Stb.writeMicroseconds  (v_Stb);
  };

  double getSpeed() {return speed_value; };
  double getSpeedTarget() {return speed_target; };
  double getSpeedSet() {return speed_set; };

  double getCompass() {return compass_value; };
  double getCompassTarget() {return compass_target; };
  double getYawRateSet() {return yawrate_set; };


  /* **************************************************************** */
  /* * Debug Crtls                                                  * */
  /* **************************************************************** */ 
  void debug_help()
  {
    Serial.println("Engine ctrl");
    Serial.println(" es         Engine stop");
    Serial.println(" ess<speed> Engine set speed [-100;+100] 0=stopped");
    Serial.println(" esc<yaw>   Engine set yaw rate");
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
          speed_target = value;
        }
        else if (line[2] == 'c') {
          compass_target = line.substring(3).toDouble();
          Serial.print("Set target yaw rate: ");
          Serial.println(compass_target);
        }
        else {
          Esc_Bb.writeMicroseconds  (ESC_MID_MICROSEC);
          Esc_Stb.writeMicroseconds  (ESC_MID_MICROSEC);
          Serial.println("Engine stopped");
        }
      }
    }
    else if (line[0] == 'c')
    {
      if (line[1] == 's')
      {
        int value = line.substring(2).toInt();
        value = max(value, -180);
        value = min(value, 180);

        compass_target = value;

        Serial.print("Set compass: ");
        Serial.print(value);
        Serial.println("°");
      }
      else if (line[1] == 'p')
      {
        Serial.print("Compass: ");
        Serial.print(compass_value);
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