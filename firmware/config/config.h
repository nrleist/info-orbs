#ifndef CONFIG_H
#define CONFIG_H

// ============= CONFIGURE THESE FIELDS BEFORE FLASHING ====================================================

// MAIN CONFIGURATION
#define TIMEZONE_API_LOCATION "America/New_York" // Use timezone from this list: https://timezonedb.com/time-zones
#define INVERTED_ORBS false                       // Set to true if using InfoOrbs upside down. Inverts screens and re-orders screens and buttons.
#define WIDGET_CYCLE_DELAY 17                      // Automatically cycle widgets every X seconds, set to 0 to disable
#define LOCALE EN                                 // Language selection for Month and Weekday - possible values are EN, DE, FR

// NIGHTTIME DIMMING
//#define DIM_START_HOUR 22  // Dim the screens at this time (24h format)
//#define DIM_END_HOUR 7     // Undim the screens at this time (24h format)
//#define DIM_BRIGHTNESS 128 // Dim brightness (0-255)

// CLOCK CONFIGURATION
#define FORMAT_24_HOUR false            // Toggle 24 hour clock vs 12 hour clock, change between true/false
#define SHOW_AM_PM_INDICATOR true      // AM/PM on the clock if using 12 hour
#define SHOW_SECOND_TICKS true          // Ticking indicator on the centre clock
#define CLOCK_COLOR 0xd003             // Red for clock
#define CLOCK_SHADOW_COLOR 0x3000      // Clock shadow color (Light brown)
#define CLOCK_SHADOWING true            // Use clock shadow (set to false to disable)
#define USE_CLOCK_NIXIE true            // Set to true to enable Nixie style clock
#define USE_CLOCK_CUSTOM false          // Set to true to enable custom clock (see images/ClockWidget/custom/README.md)
#define DEFAULT_CLOCK ClockType::NORMAL // Set the default clock face: ClockType::NORMAL, ClockType::NIXIE or ClockType::CUSTOM
//#define CLOCK_FONT DSEG7              // Define clock font (defaults to DSEG7, you can also try DSEG14 or FINAL_FRONTIER)

// WEATHER CONFIGURATION
#define WEATHER_LOCATION "Cincinnati, OH" // City/state for the weather, look it up on https://www.visualcrossing.com/weather-data
#define WEATHER_SCREEN_MODE Dark        // Can be either Light or Dark
//#define WEATHER_UNITS_METRIC            // Comment this line out (or delete it) if you want imperial units for the weather

// STOCK TICKER CONFIGURATION
#define STOCK_TICKER_LIST "CINF,PG,TSLA,GE,PRYMY" // Choose 5 securities to track. You can track forex, crypto (symbol/USD) or stocks from any exchange (if one ticker is part of multiple exchanges you can add on "&country=Canada" to narrow down to your ticker)

// PARQET.COM PORTFOLIO CONFIGURATION
//#define PARQET_PORTFOLIO_ID "" // set the id of your parqet.com portfolio. Make sure the portfolio is set to public!

// WEB DATA CONFIGURATION
//#define WEB_DATA_WIDGET_URL "" // Use this to make your own widgets using an API/Webdata source
//#define WEB_DATA_STOCK_WIDGET_URL "http://<insert host here>/stocks.php?stocks=SPY,VT,GOOG,TSLA,GME" // Use this as an alternative to the stock ticker widget

// MQTT CONFIGURATION
//#define MQTT_WIDGET_HOST "192.168.3.40" // MQTT broker host
//#define MQTT_WIDGET_PORT 1883 // MQTT broker port
//#define MQTT_SETUP_TOPIC "info-orbs/setup/orbs" // Setup topic
//#define MQTT_WIDGET_USER "mqttuser" // Leave empty if authentication is not required
//#define MQTT_WIDGET_PASS "mqttuser" // Leave empty if authentication is not required

// WIFI CONFIGURATION
// Normally WiFi should be configured using a smartphone or computer to connect to the Info-Orbs
// configuration portal. But as a fall-back method, WiFi can be setup by uncommenting these 2 lines and
// changing the SSID and password.
//#define WIFI_SSID "MyWiFiRouter" // Wifi router SSID name (use only 2.4 GHz network)
//#define WIFI_PASS "WiFiPassword" // Wifi router password

// TRUETYPE FONT CONFIGURATION
//#define DEFAULT_FONT ROBOTO_REGULAR

// ============= END OF USER CONFIGURATION =================================================================


#undef TFT_MOSI
#undef TFT_MISO
#undef TFT_SCLK
#undef TFT_CS
#undef TFT_DC


#undef ILI9341_DRIVER
#define GC9A01_DRIVER

#define TFT_SDA_READ

#define TFT_MOSI 17
#define TFT_MISO -1
#define TFT_SCLK 23
#define TFT_CS 15
#define TFT_DC 19
#define TFT_RST 18

#define SCREEN_1_CS 13
#define SCREEN_2_CS 33
#define SCREEN_3_CS 32
#define SCREEN_4_CS 25
#define SCREEN_5_CS 21

#if INVERTED_ORBS
    #define BUTTON_OK 27
    #define BUTTON_LEFT 14
    #define BUTTON_RIGHT 26
#else
    #define BUTTON_OK 27
    #define BUTTON_LEFT 26
    #define BUTTON_RIGHT 14
#endif

#define BUTTON_DEBOUNCE_TIME 35        // Debounce buttons for X ms
#define BUTTON_MEDIUM_PRESS_TIME 500    // Medium press is registered after X ms
#define BUTTON_LONG_PRESS_TIME 2000     // Long press is registered after X ms

#define BUTTON_MODE INPUT_PULLDOWN
#define BUSY_PIN 2


#define NTP_SERVER "pool.ntp.org"

#define SCREEN_SIZE 240
#define TFT_WIDTH SCREEN_SIZE
#define TFT_HEIGHT SCREEN_SIZE
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8N
#define LOAD_GFXFF
#define SMOOTH_FONT
#define SPI_FREQUENCY 27000000

#define TIMEZONE_API_KEY "97R9WKDPBLIO"
#define TIMEZONE_API_URL "http://api.timezonedb.com/v2.1/get-time-zone"
#define WEATHER_API_KEY "XW2RDGD6XK432AF25BNK2A3C7"

#define MAX_RETRIES 3

#endif
