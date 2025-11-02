
#define SERIAL_BAUDRATE     9600

#define SECRET_SSID "4in1boot"
#define SECRET_PASS "76148447002638943031"

#define LOOP_DELAY        100


#define PID_ROTATION_KP     0.001
#define PID_ROTATION_KI     0.001 //5
#define PID_ROTATION_KD     0 //1
#define PID_ROTATION_LIMIT  5

#define PID_SPEED_KP        2
#define PID_SPEED_KI        5
#define PID_SPEED_KD        1
#define PID_SPEED_LIMIT     85


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