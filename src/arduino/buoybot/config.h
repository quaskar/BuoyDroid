
#define SERIAL_BAUDRATE         9600

#include "config_wifi.h"
#define WIFI_DEST_IP            "192.168.0.100"
#define WIFI_PORT               5001

#define LOOP_DELAY              100

#define ESC_STB_DIRECTION       1
#define ESC_BB_DIRECTION        -1


#define PID_ROTATION_KP         0.01
#define PID_ROTATION_KI         1 //5
#define PID_ROTATION_KD         0 //1
#define PID_ROTATION_LIMIT      20
#define PID_ROTATION_DIRECTION  1 /* 0:=Direct, 1:=Reverse */


#define PID_SPEED_KP            0.1
#define PID_SPEED_KI            2
#define PID_SPEED_KD            0
#define PID_SPEED_LIMIT         100
#define PID_SPEED_DIRECTION     0 /* 0:=Direct, 1:=Reverse */


/* Pin Config */
#define PIN_WINCH_EN            11
#define PIN_WINCH_N1            12
#define PIN_WINCH_N2            13
#define PIN_CURRENT_MEASURMENT  A3
#define PIN_STB_ESC             5
#define PIN_BB_ESC              6


/* Compass Calibration */
#define COMPASS_OFFSET_X        -20
#define COMPASS_OFFSET_Y        -996
#define COMPASS_OFFSET_Z        -1179
#define COMPASS_SCALE_X         0.92
#define COMPASS_SCALE_Y         0.81
#define COMPASS_SCALE_Z         1.47