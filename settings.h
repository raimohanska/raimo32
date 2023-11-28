// For fading LEDS using MOSFET transistor
#define DIMMER_LIGHT_PIN 13
// For switching with a relay
//#define RELAY_LIGHT_PIN 13
//#define DHT_PIN 5
//#define MOTION_PIN 4
//#define DHT_ENABLE_PIN 14
//#define DHT_ENABLE_LEVEL HIGH
#define CHANGE_THRESHOLD 1.0
//#define ILLUMINATION_PIN A0
#define ILLUMINATION_FAST true
#define GREEN_LED_PIN 2
#define RED_LED_PIN 12
//#define RESET_BUTTON_PIN 15
//#define RESET_BUTTON_STATE HIGH

#define RED_LED_PWM 1
#define GREEN_LED_PWM 2
#define DIMMER_PWM 3

#define RESET_SIGNAL_PIN 16

#define CONN_WIFI_FAIL 0
#define CONN_WIFI_CONNECTING 1
#define CONN_WIFI_CONNECTED 2
#define CONN_SERVER_CONNECTING 3
#define CONN_SERVER_FAIL 4
#define CONN_SERVER_CONNECTED 5
#define CONN_SETUP 6

#define PING_INTERVAL_SECS 30
#define MEASUREMENT_INTERVAL_SECS 30   // frequency of measuring temperature/humidity/illumination
#define MEASUREMENT_MAX_AGE 60 // at least 1 of _ measurements must be sent

#ifdef RELAY_LIGHT_PIN
    #ifdef DIMMER_LIGHT_PIN
        Failure! Relay and dimmer are mutually exclusive
    #endif
#endif
